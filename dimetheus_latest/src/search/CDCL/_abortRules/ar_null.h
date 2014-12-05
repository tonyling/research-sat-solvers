/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef AR_NULL_H_
#define AR_NULL_H_

#include "abortRules.h"

void cdcl_abortRule_null_printHelp();
void cdcl_abortRule_null_prepare();
uint32_t cdcl_abortRule_null_check();
void cdcl_abortRule_null_signalMaintenance();
void cdcl_abortRule_null_signalRestart();

#endif /* AR_NULL_H_ */
