/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef SS_PMPCDCL_H_
#define SS_PMPCDCL_H_

#include "searchStrategies.h"

#include "../CDCL/_datastructures/cdclClauses.h"
#include "../CDCL/_datastructures/cdclLiterals.h"
#include "../CDCL/_datastructures/cdclVariables.h"

uint32_t search_strategy_pmpcdcl_midBlockSize;		//The number of variables that is to be assigned in MID as a block.
float_ty search_strategy_pmpcdcl_maxMPImpact;		//How much impact is allowed before we disable MP till next unit?
uint32_t search_strategy_pmpcdcl_assignmentMax;		//How many variables must the CDCL assign before it must return?

void search_strategy_pmpcdcl_printHelp();					//Show strategy help.

void search_strategy_pmpcdcl_reset();						//Reset everything for this strategy.
void search_strategy_pmpcdcl_init();						//Initialize everything for this strategy.
void search_strategy_pmpcdcl_dispose();						//Dispose everything for this strategy.

void search_strategy_pmpcdcl_execute();						//Execute the search.

uint32_t search_strategy_pmpcdcl_isCDCLClauseSubsumed(cdclClause*);//Checks if the CDCL clause is subsumed by main formula.
float_ty search_strategy_pmpcdcl_updateMessages(clause*);	//This method computes the omega warning messages for the clause.
float_ty search_strategy_pmpcdcl_retainNewEquivalences();	//This method retains new equivalences found by the CDCL.
float_ty search_strategy_pmpcdcl_convertImpactingClauses();	//This method retains MP impacting clauses learned by CDCL.
float_ty search_strategy_pmpcdcl_retainNewUnits();			//This method propagates new unit assignments found by the CDCL.
void search_strategy_pmpcdcl_assumeAndUpdate();				//Does MP under the given CDCL assignments as assumptions.

#endif /* SS_PMPCDCL_H_ */
