/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */


#include "adaptStrategies.h"

void adapt_strategy_null_printHelp(){
	printf("c      %-3d: NULL:\n", ADAPT_STRATEGY_NULL);
    printf("c           Behavior: No adaptation at all.\n");
    printf("c           Adaption: Nothing.\n");
}

void adapt_strategy_null_reset(){
	//Null function does nothing.
}
void adapt_strategy_null_init(){
	//Null function does nothing.
}
void adapt_strategy_null_dispose(){
	//Null function does nothing.
}
void adapt_strategy_null_execute(){
	//Null function does nothing.
}
