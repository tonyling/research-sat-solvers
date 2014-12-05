/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef MR_INACTIVITY_H_
#define MR_INACTIVITY_H_

#include "maintenanceRules.h"

void cdcl_maintenanceRule_inactivity_printHelp();
void cdcl_maintenanceRule_inactivity_prepare();
uint32_t cdcl_maintenanceRule_inactivity_check();
void cdcl_maintenanceRule_inactivity_incAct(cdclClause*);
void cdcl_maintenanceRule_inactivity_rescale();
void cdcl_maintenanceRule_inactivity_bump();
void cdcl_maintenanceRule_inactivity_maintain();

#endif /* MR_INACTIVITY_H_ */
