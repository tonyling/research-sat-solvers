/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef SS_PMPSLS_H_
#define SS_PMPSLS_H_

#include "searchStrategies.h"

uint32_t search_strategy_pmpsls_midBlockSize;			//The number of variables that is to be assigned in MID as a block.
uint32_t search_strategy_pmpsls_numToAssign;			//The number of literals in the assignment array.
uint32_t* search_strategy_pmpsls_nextToAssign;			//An array holding the next variables to assign in the right order.
float_ty search_strategy_pmpsls_maxMagForSLS;			//If the magnetization of the formula drops below this value, do SLS.

void search_strategy_pmpsls_printHelp();				//Show strategy help.

void search_strategy_pmpsls_reset();					//Reset everything for this strategy.
void search_strategy_pmpsls_init();						//Initialize everything for this strategy.
void search_strategy_pmpsls_dispose();					//Dispose everything for this strategy.

void search_strategy_pmpsls_aquireNextVariables();		//Adds the highest biased variables into the queue with opposite
														//ordering (worst of the best is first).
void search_strategy_pmpsls_orderToAssign();			//Adds the variables from the queue to the array with right order.
int32_t search_strategy_pmpsls_assignOrdered();			//Assigns the ordered variables on the main decision stack.
void search_strategy_pmpsls_retainAllPhases();			//Overrides all the phases with current assignments.

void search_strategy_pmpsls_execute();					//Execute the search.

#endif /* SS_PMPSLS_H_ */
