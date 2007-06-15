/* 
 * FreeSWITCH Modular Media Switching Software Library / Soft-Switch Application
 * Copyright (C) 2005/2006, Anthony Minessale II <anthmct@yahoo.com>
 *
 * Version: MPL 1.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is FreeSWITCH Modular Media Switching Software Library / Soft-Switch Application
 *
 * The Initial Developer of the Original Code is
 * Anthony Minessale II <anthmct@yahoo.com>
 * Portions created by the Initial Developer are Copyright (C)
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 * 
 * Chris Parker <cparker@segv.org>
 *
 *
 * mod_radius_cdr.c -- RADIUS CDR Module
 *
 */

#define _BSD_SOURCE 1

#include <switch.h>
#include <sys/stat.h>
#include <freeradius-client.h>
#include "mod_radius_cdr.h"

static const char modname[] = "mod_radius_cdr";

static char		cf[] = "mod_radius_cdr.conf";
static char		my_dictionary[PATH_MAX];
static char *		my_timeout; /* 5 */
static char *		my_retries; /* 3 */
static char		my_servers[SERVER_MAX][255];

static rc_handle *	my_radius_init(void)
{
	rc_handle 	*rad_config;

	rad_config = rc_new();

	if (rad_config == NULL) {
		switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_ERROR, "[mod_radius_cdr] Error initializing rc_handle!\n");
		return NULL;
	}

	rad_config = rc_config_init(rad_config);

	if (rad_config == NULL) {
		switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_ERROR, "[mod_radius_cdr] Error initializing radius config!\n");
		rc_destroy(rad_config);
		return NULL;
	}	

	/* Some hardcoded ( for now ) defaults needed to initialize radius */
	if (rc_add_config(rad_config, "auth_order", "radius", "mod_radius_cdr.c", 0) != 0) {
		switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_ERROR, 
					"[mod_radius_cdr] failed setting auth_order = radius\n");
		rc_destroy(rad_config);
		return NULL;
	}

	if (rc_add_config(rad_config, "seqfile", "/var/run/radius.seq", "mod_radius_cdr.c", 0) != 0) {
		switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_ERROR, 
					"failed setting seqfile =  /var/run/radius.seq\n");
		rc_destroy(rad_config);
		return NULL;
	}


	/* Add the module configs to initialize rad_config */

	for (int i = 0; i < SERVER_MAX && my_servers[i][0] != '\0'; i++) {
		if (rc_add_config(rad_config, "acctserver", my_servers[i], cf, 0) != 0) {
			switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_ERROR, "setting acctserver = %s failed\n", my_servers[i]);
			rc_destroy(rad_config);
			return NULL;
		}
	}

	if (rc_add_config(rad_config, "dictionary", my_dictionary, cf, 0) != 0) {
		switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_ERROR, 
					"failed setting dictionary = %s failed\n", my_dictionary);
		rc_destroy(rad_config);
		return NULL;
	}

	if (rc_add_config(rad_config, "radius_timeout", my_timeout, cf, 0) != 0) {
		switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_ERROR, 
				"failed setting radius_timeout = %s failed\n", my_timeout);
		rc_destroy(rad_config);
		return NULL;
	}

	if (rc_add_config(rad_config, "radius_retries", my_retries, cf, 0) != 0) {
		switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_ERROR, 
				"failed setting radius_retries = %s failed\n", my_retries);
		rc_destroy(rad_config);
	        return NULL;
	}

	/* Read the dictionary file(s) */
	if (rc_read_dictionary(rad_config, rc_conf_str(rad_config, "dictionary")) != 0) {
		switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_ERROR, 
				"failed reading dictionary file(s): %s\n", my_dictionary);
		rc_destroy(rad_config);
		return NULL;
	}

	return rad_config;
}

static switch_status_t my_on_ring(switch_core_session_t *session)
{
	switch_xml_t 	cdr;
	switch_channel_t *channel = switch_core_session_get_channel(session);
	rc_handle 	*rad_config;
	int             retval = 0;
	VALUE_PAIR      *send = NULL;
	UINT4           client_port = 0;
	UINT4           status_type = PW_STATUS_START;

	char 		*uuid_str;

	switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_ERROR, "[mod_radius_cdr] Entering my_on_ring\n");

	rad_config = my_radius_init();

	if (rad_config == NULL) {
		switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_ERROR, "[mod_radius_cdr] Error initializing radius, Start packet not logged.\n");
		return SWITCH_STATUS_TERM;
	}

	if (switch_ivr_generate_xml_cdr(session, &cdr) == SWITCH_STATUS_SUCCESS) {
		uuid_str = switch_core_session_get_uuid(session);
	} else {
		switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_ERROR, "[mod_radius_cdr] Error Generating Data!\n");
		return SWITCH_STATUS_TERM;		
	}

	/* Create the radius packet */

	/* Set Status Type */
	if (rc_avpair_add(rad_config, &send, PW_ACCT_STATUS_TYPE, &status_type, -1, 0) == NULL) {
		switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_ERROR, "[mod_radius_cdr] Failed setting Acct-Status-Type: Start\n");
		rc_destroy(rad_config);
		return SWITCH_STATUS_TERM;
	}			

	if (rc_avpair_add(rad_config, &send, PW_ACCT_SESSION_ID, uuid_str, -1, 0) == NULL) {
		switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_ERROR, "[mod_radius_cdr] Failed adding Acct-Session-ID: %s\n", uuid_str);
		rc_destroy(rad_config);
		return SWITCH_STATUS_TERM;
	}			

	/* Add VSAs */

	if(channel) {
		/*switch_call_cause_t	cause;*/
		switch_caller_profile_t	*profile;

		/*
		cause = switch_channel_get_cause(channel);
		if (rc_avpair_add(rad_config, &send, PW_FS_HANGUPCAUSE, &cause, -1, PW_FS_PEC) == NULL) {
			switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_ERROR, "failed adding Freeswitch-Hangupcause: %d\n", cause);
			rc_destroy(rad_config);
			return SWITCH_STATUS_TERM;
		}
		*/
		
		profile = switch_channel_get_caller_profile(channel);

		if(profile) {
			if(profile->caller_id_number) {
				if (rc_avpair_add(rad_config, &send, PW_FS_SRC, profile->caller_id_number, -1, PW_FS_PEC) == NULL) {
					switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_ERROR, 
							"failed adding Freeswitch-Src: %s\n", profile->caller_id_number);
					rc_destroy(rad_config);
					return SWITCH_STATUS_TERM;
				}
			} 
			if(profile->caller_id_name) {
				if (rc_avpair_add(rad_config, &send, PW_FS_CLID, profile->caller_id_name, -1, PW_FS_PEC) == NULL) {
					switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_ERROR, 
							"failed adding Freeswitch-CLID: %s\n", profile->caller_id_name);
					rc_destroy(rad_config);
					return SWITCH_STATUS_TERM;
				}
			} 
			if(profile->destination_number) {
				if (rc_avpair_add(rad_config, &send, PW_FS_DST, profile->destination_number, -1, PW_FS_PEC) == NULL) {
					switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_ERROR, 
							"failed adding Freeswitch-Dst: %s\n", profile->destination_number);
					rc_destroy(rad_config);
					return SWITCH_STATUS_TERM;
				}
			} 
			if(profile->dialplan) {
				if (rc_avpair_add(rad_config, &send, PW_FS_DIALPLAN, profile->dialplan, -1, PW_FS_PEC) == NULL) {
					switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_ERROR, 
							"failed adding Freeswitch-Dialplan: %s\n", profile->dialplan);
					rc_destroy(rad_config);
					return SWITCH_STATUS_TERM;
				}
			} 
			if(profile->caller_extension && profile->caller_extension->last_application->application_name) {
				if (rc_avpair_add(rad_config, &send, PW_FS_LASTAPP, 
					profile->caller_extension->last_application->application_name, -1, PW_FS_PEC) == NULL) {
					switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_ERROR, 
							"failed adding Freeswitch-Lastapp: %s\n", profile->source);
					rc_destroy(rad_config);
					return SWITCH_STATUS_TERM;
				}
			} 
		} else {
			switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_ERROR, "profile == NULL\n");
		}
	}

	if(rc_acct(rad_config, client_port, send) == OK_RC) {
		switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_ERROR, "[mod_radius_cdr] RADIUS Accounting OK\n");
		retval = SWITCH_STATUS_SUCCESS;
	} else {
		switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_ERROR, "[mod_radius_cdr] RADIUS Accounting Failed\n");
		retval = SWITCH_STATUS_TERM;
	}
	rc_avpair_free(send);

	return(retval);
}

static switch_status_t my_on_hangup(switch_core_session_t *session)
{
	switch_xml_t 	cdr;
	switch_channel_t *channel = switch_core_session_get_channel(session);
	rc_handle 	*rad_config;
	int             retval = 0;
	VALUE_PAIR      *send = NULL;
	UINT4           client_port = 0;
	UINT4           status_type = PW_STATUS_STOP;
	switch_time_t	callstartdate = 0;
	switch_time_t	callanswerdate = 0;
	switch_time_t	callenddate = 0;
	switch_time_t	calltransferdate = 0;
	switch_time_t	billusec = 0;
	UINT4		billsec = 0;
	char 		*uuid_str;
	

	switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_DEBUG, "[mod_radius_cdr] Entering my_on_hangup\n");

	rad_config = my_radius_init();

	if (rad_config == NULL) {
		switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_ERROR, "[mod_radius_cdr] Error initializing radius, session not logged.\n");
		return SWITCH_STATUS_TERM;
	}

	if (switch_ivr_generate_xml_cdr(session, &cdr) == SWITCH_STATUS_SUCCESS) {
		uuid_str = switch_core_session_get_uuid(session);
	} else {
		switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_ERROR, "[mod_radius_cdr] Error Generating Data!\n");
		return SWITCH_STATUS_TERM;		
	}

	/* Create the radius packet */

	/* Set Status Type */
	if (rc_avpair_add(rad_config, &send, PW_ACCT_STATUS_TYPE, &status_type, -1, 0) == NULL) {
		switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_ERROR, "failed adding Acct-Session-ID: %s\n", uuid_str);
		rc_destroy(rad_config);
		return SWITCH_STATUS_TERM;
	}			

	if (rc_avpair_add(rad_config, &send, PW_ACCT_SESSION_ID, uuid_str, -1, 0) == NULL) {
		switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_ERROR, "failed adding Acct-Session-ID: %s\n", uuid_str);
		rc_destroy(rad_config);
		return SWITCH_STATUS_TERM;
	}			

	/* Add VSAs */

	if(channel) {
		switch_call_cause_t	cause;
		switch_caller_profile_t	*profile;

		cause = switch_channel_get_cause(channel);
		if (rc_avpair_add(rad_config, &send, PW_FS_HANGUPCAUSE, &cause, -1, PW_FS_PEC) == NULL) {
			switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_ERROR, "failed adding Freeswitch-Hangupcause: %d\n", cause);
			rc_destroy(rad_config);
			return SWITCH_STATUS_TERM;
		}
		
		profile = switch_channel_get_caller_profile(channel);

		if(profile) {

			switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_DEBUG, "[mod_radius_cdr] Calculating billable time\n");

			/* calculate billable time */
			callstartdate = profile->times->created;
			callanswerdate = profile->times->answered;
			calltransferdate = profile->times->transferred;
			callenddate = profile->times->hungup;

			if(switch_channel_test_flag(channel,CF_ANSWERED)) {
				if(callstartdate && callanswerdate) {
					if(callenddate)
						billusec = callenddate - callanswerdate;
					else if(calltransferdate)
						billusec = calltransferdate - callanswerdate;
				}
			}
			else if(switch_channel_test_flag(channel,CF_TRANSFER)) {
				if(callanswerdate && calltransferdate)
					billusec = calltransferdate - callanswerdate;
			}
			billsec = (billusec / 1000000);

			switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_DEBUG, "[mod_radius_cdr] Finished calculating billable time\n");

			if(profile->caller_id_number) {
				if (rc_avpair_add(rad_config, &send, PW_FS_SRC, profile->caller_id_number, -1, PW_FS_PEC) == NULL) {
					switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_ERROR, 
							"failed adding Freeswitch-Src: %s\n", profile->caller_id_number);
					rc_destroy(rad_config);
					return SWITCH_STATUS_TERM;
				}
			} 
			if(profile->caller_id_name) {
				if (rc_avpair_add(rad_config, &send, PW_FS_CLID, profile->caller_id_name, -1, PW_FS_PEC) == NULL) {
					switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_ERROR, 
							"failed adding Freeswitch-CLID: %s\n", profile->caller_id_name);
					rc_destroy(rad_config);
					return SWITCH_STATUS_TERM;
				}
			} 
			if(profile->destination_number) {
				if (rc_avpair_add(rad_config, &send, PW_FS_DST, profile->destination_number, -1, PW_FS_PEC) == NULL) {
					switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_ERROR, 
							"failed adding Freeswitch-Dst: %s\n", profile->destination_number);
					rc_destroy(rad_config);
					return SWITCH_STATUS_TERM;
				}
			} 
			if(profile->dialplan) {
				if (rc_avpair_add(rad_config, &send, PW_FS_DIALPLAN, profile->dialplan, -1, PW_FS_PEC) == NULL) {
					switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_ERROR, 
							"failed adding Freeswitch-Dialplan: %s\n", profile->dialplan);
					rc_destroy(rad_config);
					return SWITCH_STATUS_TERM;
				}
			} 
			if(profile->caller_extension && profile->caller_extension->last_application->application_name) {
				if (rc_avpair_add(rad_config, &send, PW_FS_LASTAPP, 
					profile->caller_extension->last_application->application_name, -1, PW_FS_PEC) == NULL) {
					switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_ERROR, 
							"failed adding Freeswitch-Lastapp: %s\n", profile->source);
					rc_destroy(rad_config);
					return SWITCH_STATUS_TERM;
				}
			} 
			if (rc_avpair_add(rad_config, &send, PW_FS_BILLUSEC, &billusec, -1, PW_FS_PEC) == NULL) {
				switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_ERROR, 
						"failed adding Freeswitch-Billusec: %u\n", (UINT4)billusec);
				rc_destroy(rad_config);
				return SWITCH_STATUS_TERM;
			}
			if (rc_avpair_add(rad_config, &send, PW_ACCT_SESSION_TIME, &billsec, -1, 0) == NULL) {
				switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_ERROR, 
						"failed adding Acct-Session-Time: %u\n", billsec);
				rc_destroy(rad_config);
				return SWITCH_STATUS_TERM;
			}
		} else { /* no profile, can't create data to send */
			switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_ERROR, "profile == NULL\n");			
		}
	}

	if(rc_acct(rad_config, client_port, send) == OK_RC) {
		switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_DEBUG, "RADIUS Accounting OK\n");
		retval = SWITCH_STATUS_SUCCESS;
	} else {
		switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_ERROR, "RADIUS Accounting Failed\n");
		retval = SWITCH_STATUS_TERM;
	}
	rc_avpair_free(send);

	return(retval);
}

static switch_status_t load_config(void)
{
        switch_xml_t cfg, xml, settings, param;

	int num_servers = 0;

	my_timeout = "5";
	my_retries = "3";

	for( int i = 0; i < SERVER_MAX; i++ ) {
		my_servers[i][0] = '\0';
	}

        if (!(xml = switch_xml_open_cfg(cf, &cfg, NULL))) {
                switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_ERROR, "open of %s failed\n", cf);
                return SWITCH_STATUS_TERM;
        }

        if ((settings = switch_xml_child(cfg, "settings"))) {
                for (param = switch_xml_child(settings, "param"); param; param = param->next) {
                        char *var = (char *) switch_xml_attr_soft(param, "name");
                        char *val = (char *) switch_xml_attr_soft(param, "value");

                        if (!strcmp(var, "acctserver")) {
				if (num_servers < SERVER_MAX) {
					strncpy(my_servers[num_servers],val,255-1);
					num_servers++;
				} else {
					switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_ERROR, 
						"you can only specify %d radius servers, ignoring excess server entry\n", SERVER_MAX);
				}
			}
                        else if (!strcmp(var, "dictionary")) {
				strncpy(my_dictionary,val,PATH_MAX-1);
			}
			else if (!strcmp(var, "radius_timeout")) {
				my_timeout = strdup(val);
			}
			else if (!strcmp(var, "radius_retries")) {
				my_retries = strdup(val);
			}
		}
	}

	switch_xml_free(xml);

	if(num_servers < 1) {
		switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_ERROR, 
				"you must specify at least 1 radius server\n");
		return SWITCH_STATUS_TERM;
	}

	/* If we made it this far, we succeeded */
	return SWITCH_STATUS_SUCCESS;
}

static const switch_state_handler_table_t state_handlers = {
	/*.on_init */ NULL,
	/*.on_ring */ my_on_ring,
	/*.on_execute */ NULL,
	/*.on_hangup */ my_on_hangup,
	/*.on_loopback */ NULL,
	/*.on_transmit */ NULL
};


static const switch_loadable_module_interface_t mod_radius_cdr_module_interface = {
	/*.module_name = */ modname,
	/*.endpoint_interface = */ NULL,
	/*.timer_interface = */ NULL,
	/*.dialplan_interface = */ NULL,
	/*.codec_interface = */ NULL,
	/*.application_interface */ NULL
};

SWITCH_MOD_DECLARE(switch_status_t) switch_module_load(const switch_loadable_module_interface_t **module_interface, char *filename)
{
	if (load_config() != SWITCH_STATUS_SUCCESS) {
                return SWITCH_STATUS_TERM;
        }

	/* test global state handlers */
	switch_core_add_state_handler(&state_handlers);

	*module_interface = &mod_radius_cdr_module_interface;

	/* indicate that the module should continue to be loaded */
	return SWITCH_STATUS_SUCCESS;
}

/* For Emacs:
 * Local Variables:
 * mode:c
 * indent-tabs-mode:t
 * tab-width:4
 * c-basic-offset:4
 * End:
 * For VIM:
 * vim:set softtabstop=4 shiftwidth=4 tabstop=4 expandtab:
 */
