/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef MAINTENANCERULES_H_
#define MAINTENANCERULES_H_

#include "../cdcl.h"

#define CDCL_MAINTENANCERULE_MINVALUE 0
#define CDCL_MAINTENANCERULE_MAXVALUE 4

#define CDCL_MAINTENANCERULE_NULL 0	//If no clause database maintenance is to be performed.
#include "mr_null.h"

#define CDCL_MAINTENANCERULE_INACTIVITY 1	//If CDBM is supposed to erase inactive clauses.
#include "mr_inactivity.h"

#define CDCL_MAINTENANCERULE_LBD 2	//If CDBM is supposed to erase clauses with a bad glue/LBD value.
#include "mr_lbd.h"

#define CDCL_MAINTENANCERULE_LBDINACT 3	//Dynamically switch between activity based and LBD based values to decide removal.
#include "mr_lbdinact.h"

#define CDCL_MAINTENANCERULE_LBDINACTLUBY 4	//Like 3, but reduces the database to the initial size using a Luby series.
#include "mr_lbdinactluby.h"

//Select Variable Rule plug-in internal data-structures.
uint32_t cdcl_maintenanceRules_numPerformed;	//The number of times the maintenance was previously performed.
uint32_t cdcl_maintenanceRules_assignableVars;	//The number of variables in the formula that can be assigned.
uint32_t cdcl_maintenanceRules_targetLearned;	//The number of learned clauses (not counting reasons) we must learn before we
												//perform clause database maintenance.
float_ty cdcl_maintenanceRules_currentActInc;	//This is the value with which we increase the clause activities (this
												//value gets bumped after each conflict analysis by the bump parameter).
float_ty cdcl_maintenanceRules_maxActivity;		//The maximum activity currently encountered.

static inline void cdcl_maintenanceRules_printHelp(){
	cdcl_maintenanceRule_null_printHelp();
	cdcl_maintenanceRule_inactivity_printHelp();
	cdcl_maintenanceRule_lbd_printHelp();
	cdcl_maintenanceRule_lbdinact_printHelp();
	cdcl_maintenanceRule_lbdinactluby_printHelp();
}

//Extern interface methods -- can be called by the one who initializes the component.
static inline void cdcl_extern_maintenanceRules_printNamesAndIDs(char* prefix){
	printf("%s CDCL-MAINTENANCE-RULES: CDCL_MAINTENANCERULE_NULL %d\n", prefix, CDCL_MAINTENANCERULE_NULL);
	printf("%s CDCL-MAINTENANCE-RULES: CDCL_MAINTENANCERULE_INACTIVITY %d\n", prefix, CDCL_MAINTENANCERULE_INACTIVITY);
	printf("%s CDCL-MAINTENANCE-RULES: CDCL_MAINTENANCERULE_LBD %d\n", prefix, CDCL_MAINTENANCERULE_LBD);
	printf("%s CDCL-MAINTENANCE-RULES: CDCL_MAINTENANCERULE_LBDINACT %d\n", prefix, CDCL_MAINTENANCERULE_LBDINACT);
	printf("%s CDCL-MAINTENANCE-RULES: CDCL_MAINTENANCERULE_LBDINACTLUBY %d\n", prefix, CDCL_MAINTENANCERULE_LBDINACTLUBY);
}
void (*cdcl_maintenanceRule_prepare)();		//This points to one of the maintenance rules call preparation methods available.
uint32_t (*cdcl_maintenanceRule_check)();	//This points to one of the maintenance rules check methods available. It returns
											//1 if clause database maintenance seems fruitful, and 0 otherwise.
void (*cdcl_maintenanceRule_incAct)(cdclClause*);//This points to one of the maintenance rules activity increase methods.
void (*cdcl_maintenanceRule_rescale)();		//This points to one of the maintenance rules activity re-scale methods available.
void (*cdcl_maintenanceRule_bump)();		//This is called after the MR did its work to bump the activity increase value.
void (*cdcl_maintenanceRule_maintain)();//This points to one of the maintenance rules that are available.

void cdcl_extern_maintenanceRules_switch(int32_t);//Use to switch the maintenance rule without re-initializing the whole CDCL module.

//Interface Methods for initializing the component -- called by the one who wants to use the above methods.
void cdcl_maintenanceRules_resetPlugin();
void cdcl_maintenanceRules_initPlugin();
void cdcl_maintenanceRules_disposePlugin();

#endif /* MAINTENANCERULES_H_ */
