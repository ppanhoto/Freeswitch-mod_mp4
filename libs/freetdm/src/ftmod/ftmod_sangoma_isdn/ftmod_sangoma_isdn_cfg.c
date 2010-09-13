/*
 * Copyright (c) 2010, Sangoma Technologies
 * David Yat Sin <davidy@sangoma.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * * Neither the name of the original author; nor the names of any contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 ******************************************************************************/

#include "ftmod_sangoma_isdn.h"

ftdm_status_t parse_switchtype(const char* switch_name, ftdm_span_t *span);
ftdm_status_t parse_signalling(const char* signalling, ftdm_span_t *span);

extern ftdm_sngisdn_data_t	g_sngisdn_data;

ftdm_status_t parse_switchtype(const char* switch_name, ftdm_span_t *span)
{
	unsigned i;

	sngisdn_dchan_data_t *dchan_data;
	sngisdn_span_data_t *signal_data = (sngisdn_span_data_t*) span->signal_data;
	switch(span->trunk_type) {
		case FTDM_TRUNK_T1:
			if (!strcasecmp(switch_name, "ni2") ||
				!strcasecmp(switch_name, "national")) {
				signal_data->switchtype = SNGISDN_SWITCH_NI2;
			} else if (!strcasecmp(switch_name, "5ess")) {
				signal_data->switchtype = SNGISDN_SWITCH_5ESS;
			} else if (!strcasecmp(switch_name, "4ess")) {
				signal_data->switchtype = SNGISDN_SWITCH_4ESS;
			} else if (!strcasecmp(switch_name, "dms100")) {
				signal_data->switchtype = SNGISDN_SWITCH_DMS100;
			} else {
				ftdm_log(FTDM_LOG_ERROR, "%s:Unsupported switchtype %s for trunktype:%s\n", span->name, switch_name, ftdm_trunk_type2str(span->trunk_type));
				return FTDM_FAIL;
			}
			break;
		case FTDM_TRUNK_E1:
			if (!strcasecmp(switch_name, "euroisdn") || strcasecmp(switch_name, "etsi")) {
				signal_data->switchtype = SNGISDN_SWITCH_EUROISDN;
			} else if (!strcasecmp(switch_name, "qsig")) {
				signal_data->switchtype = SNGISDN_SWITCH_QSIG;
			} else {
				ftdm_log(FTDM_LOG_ERROR, "%s:Unsupported switchtype %s for trunktype:%s\n", span->name, switch_name, ftdm_trunk_type2str(span->trunk_type));
				return FTDM_FAIL;
			}
			break;
		case FTDM_TRUNK_BRI:
		case FTDM_TRUNK_BRI_PTMP:
			if (!strcasecmp(switch_name, "euroisdn") ||
				!strcasecmp(switch_name, "etsi")) {
				signal_data->switchtype = SNGISDN_SWITCH_EUROISDN;
			} else if (!strcasecmp(switch_name, "insnet") ||
						!strcasecmp(switch_name, "ntt")) {
				signal_data->switchtype = SNGISDN_SWITCH_INSNET;
			} else {
				ftdm_log(FTDM_LOG_ERROR, "%s:Unsupported switchtype %s for trunktype:%s\n", span->name, switch_name, ftdm_trunk_type2str(span->trunk_type));
				return FTDM_FAIL;
			}
			 /* can be > 1 for some BRI variants */
			break;
		default:
			ftdm_log(FTDM_LOG_ERROR, "%s:Unsupported trunktype:%s\n", span->name, switch_name, ftdm_trunk_type2str(span->trunk_type));
			return FTDM_FAIL;
	}
	/* see if we have profile with this switch_type already */
	for (i=1; i <= g_sngisdn_data.num_cc; i++) {
		if (g_sngisdn_data.ccs[i].switchtype == signal_data->switchtype &&
			g_sngisdn_data.ccs[i].trunktype == span->trunk_type) {
			break;
		}
	}
	/* need to create a new switch_type */
	if (i > g_sngisdn_data.num_cc) {
		g_sngisdn_data.num_cc++;
		g_sngisdn_data.ccs[i].switchtype = signal_data->switchtype;
		g_sngisdn_data.ccs[i].trunktype = span->trunk_type;
		ftdm_log(FTDM_LOG_DEBUG, "%s: New switchtype:%s  cc_id:%u\n", span->name, switch_name, i);
	}

	/* add this span to its ent_cc */
	signal_data->cc_id = i;

	/* create a new dchan */ /* for NFAS - no-dchan on b-channels only links */
	g_sngisdn_data.num_dchan++;
	signal_data->dchan_id =  g_sngisdn_data.num_dchan;

	dchan_data = &g_sngisdn_data.dchans[signal_data->dchan_id];
	dchan_data->num_spans++;

	signal_data->span_id = dchan_data->num_spans;
	dchan_data->spans[signal_data->span_id] = signal_data;
	
	ftdm_log(FTDM_LOG_DEBUG, "%s: cc_id:%d dchan_id:%d span_id:%d\n", span->name, signal_data->cc_id, signal_data->dchan_id, signal_data->span_id);

	/* Add the channels to the span */
	for (i=1;i<=span->chan_count;i++) {
		unsigned chan_id;
		ftdm_channel_t *ftdmchan = span->channels[i];
		/* NFAS is not supported on E1, so span_id will always be 1 for E1 so this will work for E1 as well */
		chan_id = ((signal_data->span_id-1)*NUM_T1_CHANNELS_PER_SPAN)+ftdmchan->physical_chan_id;
		dchan_data->channels[chan_id] = (sngisdn_chan_data_t*)ftdmchan->call_data;
		dchan_data->num_chans++;
	}

	return FTDM_SUCCESS;
}

ftdm_status_t parse_signalling(const char* signalling, ftdm_span_t *span)
{
	sngisdn_span_data_t *signal_data = (sngisdn_span_data_t*) span->signal_data;
	if (!strcasecmp(signalling, "net") ||
		!strcasecmp(signalling, "pri_net")||
		!strcasecmp(signalling, "bri_net")) {

		signal_data->signalling = SNGISDN_SIGNALING_NET;
	} else if (!strcasecmp(signalling, "cpe") ||
		!strcasecmp(signalling, "pri_cpe")||
		!strcasecmp(signalling, "bri_cpe")) {

		signal_data->signalling = SNGISDN_SIGNALING_CPE;
	} else {
		ftdm_log(FTDM_LOG_ERROR, "Unsupported signalling/interface %s\n", signalling);
		return FTDM_FAIL;
	}
	return FTDM_SUCCESS;
}

ftdm_status_t ftmod_isdn_parse_cfg(ftdm_conf_parameter_t *ftdm_parameters, ftdm_span_t *span)
{
	unsigned paramindex;
	const char *var, *val;
	sngisdn_span_data_t *signal_data = (sngisdn_span_data_t*) span->signal_data;
	/* Set defaults here */
	signal_data->tei = 0;
	signal_data->min_digits = 8;
	signal_data->overlap_dial = SNGISDN_OPT_DEFAULT;
	signal_data->setup_arb = SNGISDN_OPT_DEFAULT;

	span->default_caller_data.bearer_capability = IN_ITC_SPEECH;

	/* Cannot set default bearer_layer1 yet, as we do not know the switchtype */
	span->default_caller_data.bearer_layer1 = FTDM_INVALID_INT_PARM;

	if (span->trunk_type == FTDM_TRUNK_BRI ||
		span->trunk_type == FTDM_TRUNK_BRI_PTMP) {

		ftdm_span_set_npi("unknown", &span->default_caller_data.dnis.plan);
		ftdm_span_set_ton("unknown", &span->default_caller_data.dnis.type);
		ftdm_span_set_npi("unknown", &span->default_caller_data.cid_num.plan);
		ftdm_span_set_ton("unknown", &span->default_caller_data.cid_num.type);
		ftdm_span_set_npi("unknown", &span->default_caller_data.rdnis.plan);
		ftdm_span_set_ton("unknown", &span->default_caller_data.rdnis.type);
	} else {
		ftdm_span_set_npi("e164", &span->default_caller_data.dnis.plan);
		ftdm_span_set_ton("national", &span->default_caller_data.dnis.type);
		ftdm_span_set_npi("e164", &span->default_caller_data.cid_num.plan);
		ftdm_span_set_ton("national", &span->default_caller_data.cid_num.type);
		ftdm_span_set_npi("e164", &span->default_caller_data.rdnis.plan);
		ftdm_span_set_ton("national", &span->default_caller_data.rdnis.type);
	}

	for (paramindex = 0; ftdm_parameters[paramindex].var; paramindex++) {
		ftdm_log(FTDM_LOG_DEBUG, "Sangoma ISDN key=value, %s=%s\n", ftdm_parameters[paramindex].var, ftdm_parameters[paramindex].val);
		var = ftdm_parameters[paramindex].var;
		val = ftdm_parameters[paramindex].val;
		
		if (!strcasecmp(var, "switchtype")) {
			if (parse_switchtype(val, span) != FTDM_SUCCESS) {
				return FTDM_FAIL;
			}
		} else if (!strcasecmp(var, "signalling") ||
				   !strcasecmp(var, "interface")) {
			if (parse_signalling(val, span) != FTDM_SUCCESS) {
				return FTDM_FAIL;
			}
		} else if (!strcasecmp(var, "tei")) {
			uint8_t tei = atoi(val);
			if (tei > 127) {
				ftdm_log(FTDM_LOG_ERROR, "Invalid TEI %d, valid values are (0-127)", tei);
				return FTDM_FAIL;
			}
			signal_data->tei = tei;
		} else if (!strcasecmp(var, "overlap")) {
			if (!strcasecmp(val, "yes")) {
				signal_data->overlap_dial = SNGISDN_OPT_TRUE;
			} else if (!strcasecmp(val, "no")) {
				signal_data->overlap_dial = SNGISDN_OPT_FALSE;
			} else {
				ftdm_log(FTDM_LOG_ERROR, "Invalid value for parameter:%s:%s\n", var, val);
			}
		} else if (!strcasecmp(var, "setup arbitration")) {
			if (!strcasecmp(val, "yes")) {
				signal_data->setup_arb = SNGISDN_OPT_TRUE;
			} else if (!strcasecmp(val, "no")) {
				signal_data->setup_arb = SNGISDN_OPT_FALSE;
			} else {
				ftdm_log(FTDM_LOG_ERROR, "Invalid value for parameter:%s:%s\n", var, val);
			}
		} else if (!strcasecmp(var, "facility")) {
			if (!strcasecmp(val, "yes")) {
				signal_data->facility = SNGISDN_OPT_TRUE;
			} else if (!strcasecmp(val, "no")) {
				signal_data->facility = SNGISDN_OPT_FALSE;
			} else {
				ftdm_log(FTDM_LOG_ERROR, "Invalid value for parameter:%s:%s\n", var, val);
			}
		} else if (!strcasecmp(var, "min_digits")) {
			signal_data->min_digits = atoi(val);
		} else if (!strcasecmp(var, "outbound-called-ton")) {
			ftdm_span_set_ton(val, &span->default_caller_data.dnis.type);
		} else if (!strcasecmp(var, "outbound-called-npi")) {
			ftdm_span_set_npi(val, &span->default_caller_data.dnis.plan);
		} else if (!strcasecmp(var, "outbound-calling-ton")) {
			ftdm_span_set_ton(val, &span->default_caller_data.cid_num.type);
		} else if (!strcasecmp(var, "outbound-calling-npi")) {
			ftdm_span_set_npi(val, &span->default_caller_data.cid_num.plan);
		} else if (!strcasecmp(var, "outbound-rdnis-ton")) {
			ftdm_span_set_ton(val, &span->default_caller_data.rdnis.type);
		} else if (!strcasecmp(var, "outbound-rdnis-npi")) {
			ftdm_span_set_npi(val, &span->default_caller_data.rdnis.plan);
		} else if (!strcasecmp(var, "outbound-bearer_cap")) {
			ftdm_span_set_bearer_capability(val, &span->default_caller_data.bearer_capability);
		} else if (!strcasecmp(var, "outbound-bearer_layer1")) {
			ftdm_span_set_bearer_layer1(val, &span->default_caller_data.bearer_layer1);
		} else {
			ftdm_log(FTDM_LOG_WARNING, "Ignoring unknown parameter %s\n", ftdm_parameters[paramindex].var);
		}
	}
	signal_data->link_id = span->span_id;
	if (signal_data->switchtype == SNGISDN_SWITCH_INVALID) {
		ftdm_log(FTDM_LOG_ERROR, "%s: switchtype not specified", span->name);
		return FTDM_FAIL;
	}
	if (signal_data->signalling == SNGISDN_SIGNALING_INVALID) {
		ftdm_log(FTDM_LOG_ERROR, "%s: signalling not specified", span->name);
		return FTDM_FAIL;
	}

	if (span->default_caller_data.bearer_layer1 == FTDM_INVALID_INT_PARM) {
		if (signal_data->switchtype == SNGISDN_SWITCH_EUROISDN) {
			span->default_caller_data.bearer_layer1 = IN_UIL1_G711ULAW;
		} else {
			span->default_caller_data.bearer_layer1 = IN_UIL1_G711ALAW;
		}
	}
	return FTDM_SUCCESS;
}


/******************************************************************************/
/* For Emacs:
 * Local Variables:
 * mode:c
 * indent-tabs-mode:t
 * tab-width:4
 * c-basic-offset:4
 * End:
 * For VIM:
 * vim:set softtabstop=4 shiftwidth=4 tabstop=4:
 */
