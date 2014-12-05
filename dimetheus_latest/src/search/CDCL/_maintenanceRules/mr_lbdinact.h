/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef MR_LBDINACT_H_
#define MR_LBDINACT_H_

#include "maintenanceRules.h"

void cdcl_maintenanceRule_lbdinact_printHelp();
void cdcl_maintenanceRule_lbdinact_prepare();
uint32_t cdcl_maintenanceRule_lbdinact_check();
void cdcl_maintenanceRule_lbdinact_incAct(cdclClause*);
void cdcl_maintenanceRule_lbdinact_rescale();
void cdcl_maintenanceRule_lbdinact_bump();
void cdcl_maintenanceRule_lbdinact_maintain();

#endif /* MR_LBDINACT_H_ */
