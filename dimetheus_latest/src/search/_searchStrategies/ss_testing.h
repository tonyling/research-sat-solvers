/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef SS_TESTING_H_
#define SS_TESTING_H_

#include "searchStrategies.h"

void search_strategy_testing_printHelp();			//Show strategy help.

void search_strategy_testing_reset();				//Reset everything for this strategy.
void search_strategy_testing_init();				//Initialize everything for this strategy.
void search_strategy_testing_dispose();				//Dispose everything for this strategy.

void search_strategy_testing_execute();				//Execute the search.

#endif /* SS_TESTING_H_ */
