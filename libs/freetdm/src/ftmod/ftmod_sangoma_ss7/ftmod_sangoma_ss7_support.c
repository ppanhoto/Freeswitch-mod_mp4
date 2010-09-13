/*
 * Copyright (c) 2009, Konrad Hammel <konrad@sangoma.com>
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
 */

/* INCLUDE ********************************************************************/
#include "ftmod_sangoma_ss7_main.h"
/******************************************************************************/

/* DEFINES ********************************************************************/
/******************************************************************************/

/* GLOBALS ********************************************************************/
uint32_t sngss7_id;
/******************************************************************************/

/* PROTOTYPES *****************************************************************/
uint8_t copy_tknStr_from_sngss7(TknStr str, char *ftdm, TknU8 oddEven);
uint8_t copy_cgPtyNum_from_sngss7(ftdm_caller_data_t *ftdm, SiCgPtyNum *cgPtyNum);
uint8_t copy_cgPtyNum_to_sngss7(ftdm_caller_data_t *ftdm, SiCgPtyNum *cgPtyNum);
uint8_t copy_cdPtyNum_from_sngss7(ftdm_caller_data_t *ftdm, SiCdPtyNum *cdPtyNum);
uint8_t copy_cdPtyNum_to_sngss7(ftdm_caller_data_t *ftdm, SiCdPtyNum *cdPtyNum);

int check_for_state_change(ftdm_channel_t *ftdmchan);
int check_cics_in_range(sngss7_chan_data_t *sngss7_info);
int check_for_reset(sngss7_chan_data_t *sngss7_info);

unsigned long get_unique_id(void);

ftdm_status_t extract_chan_data(uint32_t circuit, sngss7_chan_data_t **sngss7_info, ftdm_channel_t **ftdmchan);

/******************************************************************************/

/* FUNCTIONS ******************************************************************/
uint8_t copy_cgPtyNum_from_sngss7(ftdm_caller_data_t *ftdm, SiCgPtyNum *cgPtyNum)
{

	return 0;
}

/******************************************************************************/
uint8_t copy_cgPtyNum_to_sngss7(ftdm_caller_data_t *ftdm, SiCgPtyNum *cgPtyNum)
{
	int k;
	int j;
	int flag;
	char tmp[2];
	unsigned char lower;
	unsigned char upper;

	/**************************************************************************/
	cgPtyNum->eh.pres		   = PRSNT_NODEF;
	/**************************************************************************/
	cgPtyNum->natAddrInd.pres   = PRSNT_NODEF;
	cgPtyNum->natAddrInd.val	= 0x03;
	/**************************************************************************/
	cgPtyNum->scrnInd.pres	  = PRSNT_NODEF;
	cgPtyNum->scrnInd.val	   = ftdm->screen;
	/**************************************************************************/
	cgPtyNum->presRest.pres	 = PRSNT_NODEF;
	cgPtyNum->presRest.val	  = ftdm->pres;
	/**************************************************************************/
	cgPtyNum->numPlan.pres	  = PRSNT_NODEF;
	cgPtyNum->numPlan.val	   = 0x01;
	/**************************************************************************/
	cgPtyNum->niInd.pres		= PRSNT_NODEF;
	cgPtyNum->niInd.val		 = 0x00;
	/**************************************************************************/
	cgPtyNum->addrSig.pres	  = PRSNT_NODEF;

	/* atoi will search through memory starting from the pointer it is given until
	 * it finds the \0...since tmp is on the stack it will start going through the
	 * possibly causing corruption.  Hard code a \0 to prevent this
	 */
	tmp[1] = '\0';
	k = 0;
	j = 0;
	flag = 0;

	while (1) {
		tmp[0] = ftdm->cid_num.digits[k];

		if (tmp[0] != '\0') {
			if (isdigit(tmp[0])) {
				lower = atoi(&tmp[0]);
				k++;
				tmp[0] = ftdm->cid_num.digits[k];
			} else {
				while (!(isdigit(tmp[0])) && (tmp[0] != '\0')) {
					k++;
					tmp[0] = ftdm->cid_num.digits[k];
				} /* while(!(isdigit(tmp))) */

				if (tmp[0] != '\0') {
					lower = atoi(&tmp[0]);
					k++;
					tmp[0] = ftdm->cid_num.digits[k];
				} else {
					flag = 1;
					lower = 0xf;
				} /* if (tmp != '\0') */
			} /* (isdigit(tmp)) */
		} else {
			flag = 1;
			lower = 0xf;
		} /* if (tmp != '\0') */

		tmp[0] = ftdm->cid_num.digits[k];

		if (tmp[0] != '\0') {
			if (isdigit(tmp[0])) {
				upper = (atoi(&tmp[0])) << 4;
			} else {
				while (!(isdigit(tmp[0])) && (tmp[0] != '\0')) {
					k++;
					tmp[0] = ftdm->cid_num.digits[k];
				} /* while(!(isdigit(tmp))) */

				if (tmp[0] != '\0') {
					upper = (atoi(&tmp[0])) << 4;
					k++;
				} else {
					flag = 1;
					upper = 0xf;
				} /*  if (tmp != '\0') */
			} /* if (isdigit(tmp)) */
		} else {
			if (flag == 1) {
				upper = 0x0;
			} else {
				flag = 1;
				upper = 0xf;
			} /* if (flag == 1) */
		} /* if (tmp != '\0') */

		cgPtyNum->addrSig.val[j] = upper | lower;

		j++;

		if (flag) {
			break;
		} else {
			k++;
		}
	} /* while(1) */

	cgPtyNum->addrSig.len = j;

	/**************************************************************************/
	cgPtyNum->oddEven.pres	  = PRSNT_NODEF;

	cgPtyNum->oddEven.val	   = ((cgPtyNum->addrSig.val[j] >> 4) == 0x0 ) ? 0x01 : 0x00;

	/**************************************************************************/
	return 0;
}

/******************************************************************************/
uint8_t copy_cdPtyNum_from_sngss7(ftdm_caller_data_t *ftdm, SiCdPtyNum *cdPtyNum)
{

	return 0;
}

/******************************************************************************/
uint8_t copy_cdPtyNum_to_sngss7(ftdm_caller_data_t *ftdm, SiCdPtyNum *cdPtyNum)
{
	int k;
	int j;
	int flag;
	char tmp[2];
	unsigned char lower;
	unsigned char upper;

	/**************************************************************************/
	cdPtyNum->eh.pres		   = PRSNT_NODEF;
	/**************************************************************************/
	cdPtyNum->natAddrInd.pres   = PRSNT_NODEF;
	cdPtyNum->natAddrInd.val	= 0x03;
	/**************************************************************************/
	cdPtyNum->numPlan.pres	  = PRSNT_NODEF;
	cdPtyNum->numPlan.val	   = 0x01;
	/**************************************************************************/
	cdPtyNum->innInd.pres	   = PRSNT_NODEF;
	cdPtyNum->innInd.val		= 0x01;
	/**************************************************************************/
	cdPtyNum->addrSig.pres	  = PRSNT_NODEF;

	/* atoi will search through memory starting from the pointer it is given until
	 * it finds the \0...since tmp is on the stack it will start going through the
	 * possibly causing corruption.  Hard code a \0 to prevent this
	 */
	tmp[1] = '\0';
	k = 0;
	j = 0;
	flag = 0;

	while (1) {
		tmp[0] = ftdm->dnis.digits[k];

		if (tmp[0] != '\0') {
			if (isdigit(tmp[0])) {
				lower = atoi(&tmp[0]);
				k++;
				tmp[0] = ftdm->dnis.digits[k];
			} else {
				while (!(isdigit(tmp[0])) && (tmp[0] != '\0')) {
					k++;
					tmp[0] = ftdm->dnis.digits[k];
				} /* while(!(isdigit(tmp))) */

				if (tmp[0] != '\0') {
					lower = atoi(&tmp[0]);
					k++;
					tmp[0] = ftdm->dnis.digits[k];
				} else {
					flag = 1;
					lower = 0xf;
				} /* if (tmp != '\0') */
			} /* (isdigit(tmp)) */
		} else {
			flag = 1;
			lower = 0xf;
		} /* if (tmp != '\0') */

		tmp[0] = ftdm->dnis.digits[k];

		if (tmp[0] != '\0') {
			if (isdigit(tmp[0])) {
				upper = (atoi(&tmp[0])) << 4;
			} else {
				while (!(isdigit(tmp[0])) && (tmp[0] != '\0')) {
					k++;
					tmp[0] = ftdm->dnis.digits[k];
				} /* while(!(isdigit(tmp))) */

				if (tmp[0] != '\0') {
					upper = (atoi(&tmp[0])) << 4;
					k++;
				} else {
					flag = 1;
					upper = 0xf;
				} /*  if (tmp != '\0') */
			} /* if (isdigit(tmp)) */
		} else {
			if (flag == 1) {
				upper = 0x0;
			} else {
				flag = 1;
				upper = 0xf;
			} /* if (flag == 1) */
		} /* if (tmp != '\0') */

		cdPtyNum->addrSig.val[j] = upper | lower;

		j++;

		if (flag) {
			break;
		} else {
			k++;
		}
	} /* while(1) */

	cdPtyNum->addrSig.len = j;

	/**************************************************************************/
	cdPtyNum->oddEven.pres	  = PRSNT_NODEF;

	cdPtyNum->oddEven.val	   = ((cdPtyNum->addrSig.val[j] >> 4) == 0x0 ) ? 0x01 : 0x00;

	/**************************************************************************/
	return 0;
}

/******************************************************************************/
uint8_t copy_tknStr_from_sngss7(TknStr str, char *ftdm, TknU8 oddEven)
{
	uint8_t i;
	uint8_t j;

	/* check if the token string is present */

	if (str.pres == 1) {
		j = 0;

		for (i = 0; i < str.len; i++) {
			sprintf(&ftdm[j], "%d", (str.val[i] & 0x0F));
			j++;
			sprintf(&ftdm[j], "%d", ((str.val[i] & 0xF0) >> 4));
			j++;
		}

		/* if the odd flag is up the last digit is a fake "0" */
		if ((oddEven.pres == 1) && (oddEven.val == 1)) {
			ftdm[j-1] = '\0';
		} else {
			ftdm[j] = '\0';
		}
	} else {
		SS7_ERROR("Asked to copy tknStr that is not present!\n");
		return 1;
	}

	return 0;
}

/******************************************************************************/
int check_for_state_change(ftdm_channel_t *ftdmchan)
{

	/* check to see if there are any pending state changes on the channel and give them a sec to happen*/
	ftdm_wait_for_flag_cleared(ftdmchan, FTDM_CHANNEL_STATE_CHANGE, 500);

	/* check the flag to confirm it is clear now */

	if (ftdm_test_flag(ftdmchan, FTDM_CHANNEL_STATE_CHANGE)) {
		/* the flag is still up...so we have a problem */
		SS7_DEBUG_CHAN(ftdmchan, "FTDM_CHANNEL_STATE_CHANGE flag set for over 500ms, channel state = %s\n",
									ftdm_channel_state2str (ftdmchan->state));

		return 1;
	}

	return 0;
}

/******************************************************************************/
int check_cics_in_range(sngss7_chan_data_t *sngss7_info)
{


#if 0
	ftdm_channel_t		*tmp_ftdmchan;
	sngss7_chan_data_t  *tmp_sngss7_info;
	int 				i = 0;
	
	/* check all the circuits in the range to see if we are the last ckt to reset */
	for ( i = sngss7_info->grs.circuit; i < ( sngss7_info->grs.range + 1 ); i++ ) {
		if ( g_ftdm_sngss7_data.cfg.isupCircuit[i].siglink == 0 ) {
		
			/* get the ftdmchan and ss7_chan_data from the circuit */
			if (extract_chan_data(g_ftdm_sngss7_data.cfg.isupCircuit[i].id, &tmp_sngss7_info, &tmp_ftdmchan)) {
				SS7_ERROR("Failed to extract channel data for circuit = %d!\n", g_ftdm_sngss7_data.cfg.isupCircuit[i].id);
				return 0;
			}

			/* check if the channel still has the reset flag done is up */
			if (!sngss7_test_flag(tmp_sngss7_info, FLAG_GRP_RESET_RX_DN)) {
				SS7_DEBUG_CHAN(tmp_ftdmchan, "[CIC:%d] Still processing reset...\n", tmp_sngss7_info->circuit->cic);
				return 0;
			}
		} /* if not siglink */
	} /* for */

	SS7_DEBUG("All circuits out of reset: circuit=%d, range=%d\n",
				sngss7_info->grs.circuit,
				sngss7_info->grs.range);
	return 1;

#endif

	return 0;

}

/******************************************************************************/
ftdm_status_t extract_chan_data(uint32_t circuit, sngss7_chan_data_t **sngss7_info, ftdm_channel_t **ftdmchan)
{
	/*SS7_FUNC_TRACE_ENTER(__FUNCTION__);*/

	if (g_ftdm_sngss7_data.cfg.isupCkt[circuit].obj == NULL) {
		SS7_ERROR("sngss7_info is Null for circuit #%d\n", circuit);
		return FTDM_FAIL;
	}

	ftdm_assert_return(g_ftdm_sngss7_data.cfg.isupCkt[circuit].obj, FTDM_FAIL, "received message on signalling link or non-configured cic\n");

	*sngss7_info = g_ftdm_sngss7_data.cfg.isupCkt[circuit].obj;

	ftdm_assert_return((*sngss7_info)->ftdmchan, FTDM_FAIL, "received message on signalling link or non-configured cic\n");
	*ftdmchan = (*sngss7_info)->ftdmchan;

	/*SS7_FUNC_TRACE_EXIT(__FUNCTION__);*/
	return FTDM_SUCCESS;
}

/******************************************************************************/
int check_for_reset(sngss7_chan_data_t *sngss7_info)
{

	if (sngss7_test_flag(sngss7_info,FLAG_RESET_RX)) {
		return 1;
	}
	
	if (sngss7_test_flag(sngss7_info,FLAG_RESET_TX)) {
		return 1;
	}
	
	if (sngss7_test_flag(sngss7_info,FLAG_GRP_RESET_RX)) {
		return 1;
	}
	
	if (sngss7_test_flag(sngss7_info,FLAG_GRP_RESET_TX)) {
		return 1;
	}

	return 0;
	
}

/******************************************************************************/
unsigned long get_unique_id(void)
{

	if (sngss7_id < 420000000) {
		sngss7_id++;
	} else {
		sngss7_id = 1;
	}

	return(sngss7_id);
}

/******************************************************************************/

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
/******************************************************************************/
