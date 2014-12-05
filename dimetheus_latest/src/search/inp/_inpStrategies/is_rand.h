/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef IS_RAND_H_
#define IS_RAND_H_

#include "inpStrategies.h"

void inp_strategy_rand_printHelp();		//Show strategy help.

void inp_strategy_rand_reset();
void inp_strategy_rand_init();
void inp_strategy_rand_dispose();

void inp_strategy_rand_execute();

#endif /* IS_RAND_H_ */
