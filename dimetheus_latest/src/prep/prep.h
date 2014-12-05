/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef PREP_H_
#define PREP_H_

#include "../dimetheus.h"
#include "_prepStrategies/prepStrategies.h"

//MODULES for the preprocessing phase.
#include "PRE/pre.h"
#include "SATELITE/satelite.h"

//Global variables of the preprocessing phase.
int32_t prep_returnCode;	//The return code of the preprocessor phase.

//Methods provided exclusively to this phase (INTERN).
void prep_intern_QSUpdateClauseDisabling(clause*);//Handle all the queue and stack updates if a clause is disabled.
void prep_intern_QSUpdateClauseEnabling(clause*);//Handle all the queue and stack updates if a clause is enabled.
void prep_intern_QSUpdateLiteralDeletion(clause*, int32_t);//Handle the queue/stack updates if a clauses literal is deleted.
void prep_intern_disableClause(clause*);//A helper method to disable clauses (temporarily).
void prep_intern_enableClause(clause*); //A helper method to enable clauses (that where disabled temporarily).
void prep_intern_abandonClause(clause*);//A helper method to get rid of clauses during preprocessing. It updates queues.
void prep_intern_deleteClause(clause*);	//A helper method to delete clauses during preprocessing. It updates the queue.
void prep_intern_deleteLitInClause(clause*, int32_t, int32_t);//A method used during preprocessing to get rid of literals.
void prep_intern_makeClauseRemoved(clause*, int32_t, int32_t, int32_t, coveredLit*);//To transfer clause to the set of removed
									//(BLOCKED/NIVER/...) clauses. CAUTION: the pointer to the covered literals is not usable
									//anymore after calling this method.
void prep_intern_addOriginalClause(int32_t*, int32_t);//To add a new clause as original clause in (i.e. NIVER clauses).
void prep_intern_addLearnedClause(int32_t*, int32_t); //To add a new clause as learned clause.
void prep_intern_addLearnedBinaryClause(int32_t, int32_t);//To add a new clause as learned clause in (i.e. NHBR).
void prep_intern_addLearnedTernaryClause(int32_t, int32_t, int32_t);//To add a new clause as learned clause in (i.e. TERNARY).
void prep_intern_disableVariable(variable* v);//To disable a variable during preprocessing.
void prep_intern_reviveCls();		//Brings back some removed clauses of the formula if the preprocessor did not solve it.
void prep_intern_outputToFile();	//This method writes the resulting formula to the file provided as parameter.
void prep_intern_disableMissingVars(); //Disables variables that have no occurrences in both directions.

//Methods PROVIDED by this phase (EXTERN).
void prep_extern_preprocessing();	//Executes the preprocessing phase.

//Methods to initialize this phase (are called by the one who wants to execute the above functions).
void prep_resetPhase();				//Reset the preprocessing.
void prep_initPhase();				//For initializing once f.n_initial is known.
void prep_disposePhase();				//For freeing all related memory.

#endif /* PREP_H_ */
