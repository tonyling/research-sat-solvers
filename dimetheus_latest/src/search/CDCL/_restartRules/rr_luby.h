/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef RR_LUBY_H_
#define RR_LUBY_H_

#include "restartRules.h"

int32_t cdcl_restartRule_luby_t;	//How many NEW conflicts must we find until we signal a restart?
int32_t cdcl_restartRule_luby_c;	//How many NEW conflicts did we find so far?
int32_t cdcl_restartRule_luby_u;	//The Knuth implementation u.
int32_t cdcl_restartRule_luby_v;	//The Knuth implementation v.

void cdcl_restartRule_luby_printHelp();
void cdcl_restartRule_luby_prepare();
uint32_t cdcl_restartRule_luby_check();
void cdcl_restartRule_luby_incAgility(cdclVariable*);
void cdcl_restartRule_luby_signalBackjump();

#endif /* RR_LUBY_H_ */
