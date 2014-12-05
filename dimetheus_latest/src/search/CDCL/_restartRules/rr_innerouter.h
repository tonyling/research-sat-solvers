/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef RR_INNEROUTER_H_
#define RR_INNEROUTER_H_

#include "restartRules.h"

uint32_t cdcl_restartRule_innerouter_t;	//How many NEW conflicts must we find until we signal a restart?
uint32_t cdcl_restartRule_innerouter_i;	//How many conflicts for an inner restart.
uint32_t cdcl_restartRule_innerouter_o;	//How many conflicts for an outer restart.

void cdcl_restartRule_innerouter_printHelp();
void cdcl_restartRule_innerouter_prepare();
uint32_t cdcl_restartRule_innerouter_check();
void cdcl_restartRule_innerouter_incAgility(cdclVariable*);
void cdcl_restartRule_innerouter_signalBackjump();

#endif /* RR_INNEROUTER_H_ */
