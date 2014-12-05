/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef PS_RAND_H_
#define PS_RAND_H_

#include "prepStrategies.h"

void prep_strategy_rand_printHelp();	//Print the help.

void prep_strategy_rand_reset();
void prep_strategy_rand_init();
void prep_strategy_rand_dispose();

void prep_strategy_rand_execute();

#endif /* PS_RAND_H_ */
