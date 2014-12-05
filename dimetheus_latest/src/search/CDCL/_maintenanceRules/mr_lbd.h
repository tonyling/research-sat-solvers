/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef MR_LBD_H_
#define MR_LBD_H_

#include "maintenanceRules.h"

void cdcl_maintenanceRule_lbd_printHelp();
void cdcl_maintenanceRule_lbd_prepare();
uint32_t cdcl_maintenanceRule_lbd_check();
void cdcl_maintenanceRule_lbd_incAct(cdclClause*);
void cdcl_maintenanceRule_lbd_rescale();
void cdcl_maintenanceRule_lbd_bump();
void cdcl_maintenanceRule_lbd_maintain();

#endif /* MR_LBD_H_ */
