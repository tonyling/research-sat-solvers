/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef RR_LUBYAGILITY_H_
#define RR_LUBYAGILITY_H_

#include "restartRules.h"

int32_t cdcl_restartRule_lubyagility_t;	//How many NEW conflicts must we find until we signal a restart?
int32_t cdcl_restartRule_lubyagility_c;	//How many NEW conflicts did we find so far?
int32_t cdcl_restartRule_lubyagility_u;	//The Knuth implementation u.
int32_t cdcl_restartRule_lubyagility_v;	//The Knuth implementation v.

void cdcl_restartRule_lubyagility_printHelp();
void cdcl_restartRule_lubyagility_prepare();
uint32_t cdcl_restartRule_lubyagility_check();
void cdcl_restartRule_lubyagility_incAgility(cdclVariable*);
void cdcl_restartRule_lubyagility_signalBackjump();

#endif /* RR_LUBYAGILITY_H_ */
