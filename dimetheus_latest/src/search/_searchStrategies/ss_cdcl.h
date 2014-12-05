/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef SS_CDCL_H_
#define SS_CDCL_H_

#include "searchStrategies.h"

void search_strategy_cdcl_printHelp();	//Show strategy help.

void search_strategy_cdcl_reset();		//Reset everything for this strategy.
void search_strategy_cdcl_init();		//Initialize everything for this strategy.
void search_strategy_cdcl_dispose();	//Dispose everything for this strategy.

void search_strategy_cdcl_execute();	//Execute the strategy.

#endif /* SS_CDCL_H_ */
