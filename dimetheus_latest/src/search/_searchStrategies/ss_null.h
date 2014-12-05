/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef SS_NULL_H_
#define SS_NULL_H_

#include "searchStrategies.h"

void search_strategy_null_printHelp();	//Show strategy help.

void search_strategy_null_reset();			//Null function does nothing.
void search_strategy_null_init();			//Null function does nothing.
void search_strategy_null_dispose();		//Null function does nothing.
void search_strategy_null_execute();		//Null function does nothing.


#endif /* SS_NULL_H_ */
