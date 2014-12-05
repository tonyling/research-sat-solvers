/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef PS_NULL_H_
#define PS_NULL_H_

#include "prepStrategies.h"

void prep_strategy_null_printHelp();	//Print the help.

void prep_strategy_null_reset();		//Null function does nothing.
void prep_strategy_null_init();			//Null function does nothing.
void prep_strategy_null_dispose();		//Null function does nothing.

void prep_strategy_null_execute();		//Null function does nothing.

#endif /* PS_NULL_H_ */
