/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef SLS_H_
#define SLS_H_

#include "../search.h"
#include "_datastructures/slsClauses.h"
#include "_datastructures/slsLiterals.h"
#include "_datastructures/slsVariables.h"

#define SLS_VERSION_MA 2
#define SLS_VERSION_MI 265
#define SLS_VERSION_NAME "SLS"

//GLOBAL data-structures for SLS.
int32_t sls_returnCode;					//The return-code of the call to the stochastic local search component.

uint64_t *sls_assignments;				//The assignments of all the variables as compressed to one bit per variables.
uint32_t sls_varNum;					//The number of variables still assignable.

slsClause *sls_clauses;					//The clauses of the SLS module.
uint32_t sls_clsUsed;					//The number of clauses used by the SLS.
uint32_t sls_clsAvail;					//The number of clauses we have the memory allocated for.
uint32_t sls_clsMaxSize;				//The maximum clause length encountered.
uint32_t sls_clsMinSize;				//The minimum clause length encountered.

slsLiteral *sls_literals;				//The literals of the SLS module.
uint32_t sls_litMaxOcc;					//The maximum occurrence of a single literal.

slsVariable *sls_variables;				//The variables of the SLS module.

slsClause **sls_unsatClauses;			//The clauses currently unsatisfied to the SLS under its assignment.
uint64_t sls_numUnsat;

//PLUGINS for SLS.
#include "_pickAndFlipVarRules/pickAndFlipVarRules.h" //IMPLEMENTS: *sls_pickAndFlipVarRule_: printHelp, _prepare, _paf
#include "_assInitRules/assInitRules.h" //IMPLEMENTS: *sls_assInitRule_: printHelp, _prepare, _ai
#include "_tryRestartRules/tryRestartRules.h" //IMPLEMENTS: *sls_tryRestartRule_: printHelp, _prepare, _retry

//Methods exclusively accessible to the SLS module (INTERN).
void sls_intern_prepareTry();			//Used to create a new random assignment and initialize the unsatisfied clauses.

//Methods PROVIDED by this module (EXTERN).
void sls_extern_printVersion();			//Just print the version information of this module.
void sls_extern_localSearch(uint32_t);	//Performs SLS component local search (UINT one means prepare anew).

//Methods to initialize this module (called by the one who wants to use the provided method).
void sls_resetModule();					//To reset the SLS module.
void sls_initModule();					//To initialize all its data structures.
void sls_disposeModule();				//To dispose all its data structures.

#ifdef VERBOSE_SLS
int32_t sls_verifyAll();				//To verify that the current data-structures are correct (very slow).
#endif

#endif /* SLS_H_ */
