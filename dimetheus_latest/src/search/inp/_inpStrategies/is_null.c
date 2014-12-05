/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "inpStrategies.h"

void inp_strategy_null_printHelp(){
	printf("c      %-3d: NULL\n", INP_STRATEGY_NULL);
    printf("c           Behavior: No in-processing at all.\n");
}

void inp_strategy_null_reset(){
	//Null function does nothing.
}

void inp_strategy_null_init(){
	//Null function does nothing.
}

void inp_strategy_null_dispose(){
	//Null function does nothing.
}

void inp_strategy_null_execute(){
	//Null function does nothing.
}
