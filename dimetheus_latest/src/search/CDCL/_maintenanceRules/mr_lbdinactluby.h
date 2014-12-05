/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef MR_LBDINACTLUBY_H_
#define MR_LBDINACTLUBY_H_

#include "maintenanceRules.h"

int32_t cdcl_maintenanceRule_lbdinactluby_u;	//The Knuth implementation u.
int32_t cdcl_maintenanceRule_lbdinactluby_v;	//The Knuth implementation v.

void cdcl_maintenanceRule_lbdinactluby_printHelp();
void cdcl_maintenanceRule_lbdinactluby_prepare();
uint32_t cdcl_maintenanceRule_lbdinactluby_check();
void cdcl_maintenanceRule_lbdinactluby_incAct(cdclClause*);
void cdcl_maintenanceRule_lbdinactluby_rescale();
void cdcl_maintenanceRule_lbdinactluby_bump();
void cdcl_maintenanceRule_lbdinactluby_maintain();

#endif /* MR_LBDINACTLUBY_H_ */
