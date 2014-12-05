/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef AR_HYBRIDMSC_H_
#define AR_HYBRIDMSC_H_

#include "abortRules.h"

uint32_t cdcl_abortRule_hybridmsc_lastCheckedLearned;
uint32_t cdcl_abortRule_hybridmsc_lastNumUnits;
uint32_t cdcl_abortRule_hybridmsc_nonRestarting;

void cdcl_abortRule_hybridmsc_printHelp();
void cdcl_abortRule_hybridmsc_prepare();
uint32_t cdcl_abortRule_hybridmsc_check();
void cdcl_abortRule_hybridmsc_signalMaintenance();
void cdcl_abortRule_hybridmsc_signalRestart();

#endif /* AR_HYBRIDMSC_H_ */
