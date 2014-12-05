/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef AR_PMPCDCL_H_
#define AR_PMPCDCL_H_

#include "abortRules.h"

uint32_t cdcl_abortRule_pmpcdcl_lastCheckedLearned;
uint32_t cdcl_abortRule_pmpcdcl_lastNumUnits;

void cdcl_abortRule_pmpcdcl_printHelp();
void cdcl_abortRule_pmpcdcl_prepare();
uint32_t cdcl_abortRule_pmpcdcl_check();
void cdcl_abortRule_pmpcdcl_signalMaintenance();
void cdcl_abortRule_pmpcdcl_signalRestart();


#endif /* AR_PMPCDCL_H_ */
