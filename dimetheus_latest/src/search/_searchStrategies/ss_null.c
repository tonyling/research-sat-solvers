/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "searchStrategies.h"

void search_strategy_null_printHelp(){
	printf("c      %-3d: NULL\n", SEARCH_STRATEGY_NULL);
    printf("c           Behavior: Completely disables search [USES NO MODULES].\n");
}

void search_strategy_null_reset(){
	//Null function does nothing.
}
void search_strategy_null_init(){
	//Null function does nothing.
}
void search_strategy_null_dispose(){
	//Null function does nothing.
}
void search_strategy_null_execute(){
	//Null function does nothing.
}
