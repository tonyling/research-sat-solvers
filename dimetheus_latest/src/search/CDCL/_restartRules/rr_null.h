/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef RR_NULL_H_
#define RR_NULL_H_

#include "restartRules.h"

void cdcl_restartRule_null_printHelp();
void cdcl_restartRule_null_prepare();
uint32_t cdcl_restartRule_null_check();
void cdcl_restartRule_null_incAgility(cdclVariable*);
void cdcl_restartRule_null_signalBackjump();

#endif /* RR_NULL_H_ */
