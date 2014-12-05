/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef AS_NULL_H_
#define AS_NULL_H_

#include "adaptStrategies.h"

void adapt_strategy_null_printHelp();		//Print the help.

void adapt_strategy_null_reset();			//Null function does nothing.
void adapt_strategy_null_init();			//Null function does nothing.
void adapt_strategy_null_dispose();			//Null function does nothing.

void adapt_strategy_null_execute();			//Null function does nothing.

#endif /* AS_NULL_H_ */
