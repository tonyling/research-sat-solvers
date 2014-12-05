/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef IS_NULL_H_
#define IS_NULL_H_

#include "inpStrategies.h"

void inp_strategy_null_printHelp();		//Show strategy help.

void inp_strategy_null_reset();			//Null function does nothing.
void inp_strategy_null_init();			//Null function does nothing.
void inp_strategy_null_dispose();		//Null function does nothing.

void inp_strategy_null_execute();		//Null function does nothing.


#endif /* IS_NULL_H_ */
