/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef RR_INNEROUTERSTAG_H_
#define RR_INNEROUTERSTAG_H_

uint32_t cdcl_restartRule_innerouterstag_t;	//How many NEW conflicts must we find until we signal a restart?
uint32_t cdcl_restartRule_innerouterstag_i;	//How many conflicts for an inner restart.
uint32_t cdcl_restartRule_innerouterstag_o;	//How many conflicts for an outer restart.

void cdcl_restartRule_innerouterstag_printHelp();
void cdcl_restartRule_innerouterstag_prepare();
uint32_t cdcl_restartRule_innerouterstag_check();
void cdcl_restartRule_innerouterstag_incAgility(cdclVariable*);
void cdcl_restartRule_innerouterstag_signalBackjump();

#endif /* RR_INNEROUTERSTAG_H_ */
