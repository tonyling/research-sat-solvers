/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef RR_INNEROUTERAGILITY_H_
#define RR_INNEROUTERAGILITY_H_

#include "restartRules.h"

uint32_t cdcl_restartRule_innerouteragility_t;	//How many NEW conflicts must we find until we signal a restart?
uint32_t cdcl_restartRule_innerouteragility_i;	//How many conflicts for an inner restart.
uint32_t cdcl_restartRule_innerouteragility_o;	//How many conflicts for an outer restart.

void cdcl_restartRule_innerouteragility_printHelp();
void cdcl_restartRule_innerouteragility_prepare();
uint32_t cdcl_restartRule_innerouteragility_check();
void cdcl_restartRule_innerouteragility_incAgility(cdclVariable*);
void cdcl_restartRule_innerouteragility_signalBackjump();

#endif /* RR_INNEROUTERAGILITY_H_ */
