/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef CDCL_H_
#define CDCL_H_

#include "../search.h"
#include "_datastructures/cdclClauses.h"
#include "_datastructures/cdclLiterals.h"
#include "_datastructures/cdclVariables.h"
#include "_tools/cdclTools.h"

#define CDCL_VERSION_MA 2
#define CDCL_VERSION_MI 121
#define CDCL_VERSION_NAME "CDCL"

//GLOBAL data-structures for CDCL.
int32_t cdcl_returnCode;				//The return-code of the call to the conflict driven clause learning search component.

uint32_t cdcl_litStamp;					//The current literal stamp.

cdclVariable *cdcl_variables;			//The variables for the CDCL in the current snap-shot formula.

cdclVariable **cdcl_vars_q;				//The variable priority queue pointer.
cdclVariable **cdcl_vars_e;				//The currently enabled variables. Actually, this is cdcl_vars_q + 1.
uint32_t cdcl_vars_e_used;				//The number of variables in the enabled array.

uint32_t cdcl_variables_S_leaf;			//The current leaf of the CDCL variable stack.
cdclVariable** cdcl_variables_S;		//The CDCL variable stack.

cdclLiteral *cdcl_literals;				//The literals for the CDCL in the current snap-shot formula.
uint32_t cdcl_literals_S_leaf;			//The current leaf of the CDCL literals stack.
cdclLiteral** cdcl_literals_S;			//The CDCL literals stack.
uint32_t cdcl_literals_Q_leaf;			//The current leaf of the CDCL literals priority queue.
cdclLiteral** cdcl_literals_Q;			//The CDCL literals priority queue.

cdclClause **cdcl_cls_o;				//The original clauses for the CDCL in the current snap-shot formula.
uint32_t cdcl_cls_o_avail;				//For how many original clauses did we allocate the memory for.
uint32_t cdcl_cls_o_used;				//For how many original clauses do we use the memory.

cdclClause **cdcl_cls_l;				//The learned clauses for the CDCL in the current snap-shot formula.
uint32_t cdcl_cls_l_avail;				//For how many learned clauses did we allocate the memory for.
uint32_t cdcl_cls_l_used;				//For how many learned clauses do we use the memory.

uint32_t cdcl_clauses_Q_leaf;			//The current leaf for the clause priority queue.
uint32_t cdcl_clauses_Q_avail;			//The amount of memory we have available for the clauses stack.
cdclClause** cdcl_clauses_Q;			//The clause priority queue.

uint32_t cdcl_clauses_S_leaf;			//The current leaf for the clause stack.
uint32_t cdcl_clauses_S_avail;			//The amount of memory we have available for the clauses stack.
cdclClause** cdcl_clauses_S;			//The clause stack.

remCdclClause** cdcl_cls_r;				//The removed clauses.
uint32_t cdcl_cls_r_avail;				//How many removed clauses pointers can be used.
uint32_t cdcl_cls_r_used;				//How many are in use.

cdclEqVar* cdcl_eqVars;					//The variables together with equivalence classes.


cdclVariable* cdcl_decVar;				//The decision variable.
cdclLiteral* cdcl_decLit;				//The literal assignment we are propagating in a non-conflicting state.
cdclClause* cdcl_conflictClause;		//The conflict clause (the one that became empty during propagations).
uint32_t cdcl_numLearned;				//The conflict analysis _analyze sets this to how many new clauses it learned.
uint32_t cdcl_numConflicts;				//The number of conflicts detected by the CDCL.
uint32_t cdcl_targetBJLevel;			//The level to which we want to back-jump.

#define CDCL_GET_NUM_ORIGINAL_CLAUSES() ( cdcl_cls_o_used )
#define CDCL_GET_NUM_LEARNED_CLAUSES()	( cdcl_cls_l_used )

//PLUGINS for CDCL.
//SVR IMPLEMENTS: *cdcl_selectVarRules_: printHelp, _prepare, _select, _incAct, _rescale, _bump, _reward
#include "_selectVarRules/selectVarRules.h"
//SDR IMPLEMENTS: *cdcl_selectDirRules_: printHelp, _prepare, _select, _save
#include "_selectDirRules/selectDirRules.h"
//CAR IMPLEMENTS: *cdcl_conflictAnalysisRules_: printHelp, _prepare, _analyze
#include "_conflictAnalysisRules/conflictAnalysisRules.h"
//SLR IMPLEMENTS: *cdcl_strLearnedRules_: printHelp, _prepare, _str
#include "_strLearnedRules/strLearnedRules.h"
//SOR IMPLEMENTS: *cdcl_strOtherRules_: printHelp, _prepare, _str
#include "_strOtherRules/strOtherRules.h"
//CBJLR IMPLEMENTS: *cdcl_computeBJLRules_: printHelp, _prepare, _compute
#include "_computeBJLRules/computeBJLRules.h"
//RR IMPLEMENTS: *cdcl_restartRules_: printHelp, _prepare, _check, _incAgility, signalBackjump
#include "_restartRules/restartRules.h"
//MR IMPLEMENTS: *cdcl_maintenanceRules_: printHelp, _prepare, _check, _maintain, _incAct, _rescale, _bump
#include "_maintenanceRules/maintenanceRules.h"
//IR IMPLEMENTS: *cdcl_inprocessingRules_: printHelp, _prepare, _adapt, _inprocess
#include "_inprocessingRules/inprocessingRules.h"
//AR IMPLEMENTS: *cdcl_abortRules_: printHelp, _prepare, _adapt, _check, _signalMaintenance, _signalRestart
#include "_abortRules/abortRules.h"

//Methods exclusively accessible to the CDCL module (INTERN).
void cdcl_intern_finalizeLearned(); 	//For all learned clauses, this method ensures that the first and second
										//watchers are those literals that have the highest and second highest decision
										//levels in which they were assigned. This method also updates the signature and
										//representative literal (the latter will be the first watcher).
uint32_t cdcl_intern_addUnitsForgetOther();	//In case conflict analysis learned at least one unit clause, this method
										//handles adding their implications to the stack. It also removes all learned clauses
										//from the cdcl_cls_l array -- so it basically forgets everything it learned besides
										//the units. The method returns 1 if no conflict was discovered by only adding units,
										//and 0 otherwise.
void cdcl_intern_addNonUnitsWithAsserts();//In case conflict analysis learned no unit clauses, we use this method to
										//add all learned clauses to the formula. Those that have only one unassigned watcher
										//provide an implication for the current decision level. This cannot give a conflict.
void cdcl_intern_addNonUnitsWithoutAsserts();//In case conflict analysis learned no units and we back-jump to a DL
										//where at least one of the learned clauses is unit. This cannot end in a conflict.
void cdcl_intern_assignEquivVars_assign(cdclVariable*);//The recursive method to assign equivalent variables.
void cdcl_intern_assignEquivVars();		//This method ensures that all equivalent variables are assigned correctly.
void cdcl_intern_addLearnedBinaryClause(int32_t, int32_t);//For adding a new binary clause.
void cdcl_intern_addLearnedTernaryClause(int32_t, int32_t, int32_t);//For adding a new ternary clause.
void cdcl_intern_setClsLbd(cdclClause*);//This sets the LBD of a clause according to the current decision stack.
void cdcl_intern_updateLbdInLastLevel();//For all clauses that are acting reasons in the last decision level, we update the
										//literal blocks distance.
void cdcl_intern_writeFormulaToFile();	//This method will write the formula to the given filename in param_cdclOutput.


//Methods PROVIDED by this module (EXTERN).
void cdcl_extern_printVersion();		//Just print the version information of this module.
void cdcl_extern_learn(uint32_t);		//Performs the conflict driven clause learning (UINT 1 means re-prepare).
void cdcl_extern_reOrganizeRemainingVars();//Performs a re-organization of the variable and value selection heuristics.

//Methods to initialize this module (called by the one who wants to use the provided method).
void cdcl_resetModule();				//To reset the CDCL module.
void cdcl_initModule();					//To initialize all its data structures.
void cdcl_disposeModule();				//To dispose all its data structures.

#ifdef VERBOSE_CDCL
void cdcl_printProgress();				//Prints a single lined status report on the progress of the search.
#endif

#endif /* CDCL_H_ */
