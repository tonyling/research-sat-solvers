/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef RR_LUBYSTAG_H_
#define RR_LUBYSTAG_H_

#include "restartRules.h"

int32_t cdcl_restartRule_lubystag_t;	//How many NEW conflicts must we find until we signal a restart?
int32_t cdcl_restartRule_lubystag_c;	//How many NEW conflicts did we find so far?
int32_t cdcl_restartRule_lubystag_u;	//The Knuth implementation u.
int32_t cdcl_restartRule_lubystag_v;	//The Knuth implementation v.

void cdcl_restartRule_lubystag_printHelp();
void cdcl_restartRule_lubystag_prepare();
uint32_t cdcl_restartRule_lubystag_check();
void cdcl_restartRule_lubystag_incAgility(cdclVariable*);
void cdcl_restartRule_lubystag_signalBackjump();

#endif /* RR_LUBYSTAG_H_ */
