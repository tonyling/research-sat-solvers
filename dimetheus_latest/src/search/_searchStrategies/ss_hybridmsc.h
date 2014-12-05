/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef SS_HYBRIDMSC_H_
#define SS_HYBRIDMSC_H_

#include "searchStrategies.h"

uint32_t search_strategy_hybridmsc_midBlockSize;	//The number of variables that is to be assigned in MID as a block.
float_ty search_strategy_hybridmsc_maxMagForSLS;	//The maximum magnetization we allow before calling SLS on a sub-formula.
float_ty search_strategy_hybridmsc_maxMPImpact;		//How much impact is allowed before we disable MP till next unit?
uint32_t search_strategy_hybridmsc_assignmentMax;	//How many variables must the CDCL assign before it must return?
uint32_t search_strategy_hybridmsc_assignmentMin;	//How many variables must at least be present for the CDCL to continue?

void search_strategy_hybridmsc_printHelp();	//Show strategy help.

void search_strategy_hybridmsc_reset();		//Reset everything for this strategy.
void search_strategy_hybridmsc_init();		//Initialize everything for this strategy.
void search_strategy_hybridmsc_dispose();	//Dispose everything for this strategy.

void search_strategy_hybridmsc_execute();	//Execute the strategy.

uint32_t search_strategy_hybridmsc_isCDCLClauseSubsumed(cdclClause*);//Checks if the CDCL clause is subsumed by main formula.
float_ty search_strategy_hybridmsc_updateMessages(clause*);	//This method computes the omega warning messages for the clause.
float_ty search_strategy_hybridmsc_retainNewEquivalences();	//This method retains new equivalences found by the CDCL.
float_ty search_strategy_hybridmsc_convertImpactingClauses();//This method retains MP impacting clauses learned by CDCL.
float_ty search_strategy_hybridmsc_retainNewUnits();		//This method propagates new unit assignments found by the CDCL.
void search_strategy_hybridmsc_assumeAndUpdate();			//Does MP under the given CDCL assignments as assumptions.
void search_strategy_hybridmsc_assumeAndLocalSearch();		//Does SLS under the given CDCL assignments as assumptions.


#endif /* SS_HYBRIDMSC_H_ */
