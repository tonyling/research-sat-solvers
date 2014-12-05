/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "prepStrategies.h"

void prep_strategy_null_printHelp(){
	printf("c      %-3d: NULL:\n", PREP_STRATEGY_NULL);
    printf("c           Behavior: No preprocessing at all.\n");
}

void prep_strategy_null_reset(){
	//Null function does nothing.
}
void prep_strategy_null_init(){
	//Null function does nothing.
}
void prep_strategy_null_dispose(){
	//Null function does nothing.
}
void prep_strategy_null_execute(){
	//Null function does nothing.
}
