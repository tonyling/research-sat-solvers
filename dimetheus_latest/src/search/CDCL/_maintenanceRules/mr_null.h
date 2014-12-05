/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef MR_NULL_H_
#define MR_NULL_H_

#include "maintenanceRules.h"

void cdcl_maintenanceRule_null_printHelp();
void cdcl_maintenanceRule_null_prepare();
uint32_t cdcl_maintenanceRule_null_check();
void cdcl_maintenanceRule_null_incAct(cdclClause*);
void cdcl_maintenanceRule_null_rescale();
void cdcl_maintenanceRule_null_bump();
void cdcl_maintenanceRule_null_maintain();

#endif /* MR_NULL_H_ */
