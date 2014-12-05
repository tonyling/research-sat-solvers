/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "classifyStrategies.h"

void classify_strategy_null_printHelp(){
	printf("c      %-3d: NULL:\n", CLASSIFY_STRATEGY_NULL);
    printf("c           Behavior: No classification at all.\n");
}

void classify_strategy_null_reset(){
	//Null function does nothing.
}
void classify_strategy_null_init(){
	//Null function does nothing.
}
void classify_strategy_null_dispose(){
	//Null function does nothing.
}
void classify_strategy_null_execute(){
	//Null function does nothing.
}
