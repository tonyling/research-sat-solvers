/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef SS_SLS_H_
#define SS_SLS_H_

#include "searchStrategies.h"

void search_strategy_sls_printHelp();	//Show strategy help.

void search_strategy_sls_reset();		//Reset everything for this strategy.
void search_strategy_sls_init();		//Initialize everything for this strategy.
void search_strategy_sls_dispose();		//Dispose everything for this strategy.

void search_strategy_sls_execute();		//Execute the strategy.

#endif /* SS_SLS_H_ */
