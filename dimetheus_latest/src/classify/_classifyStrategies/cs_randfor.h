/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef CS_RANDFOR_H_
#define CS_RANDFOR_H_

#include "classifyStrategies.h"

void classify_strategy_randfor_printHelp();

void classify_strategy_randfor_reset();
void classify_strategy_randfor_init();
void classify_strategy_randfor_dispose();

void classify_strategy_randfor_execute();

#endif /* CS_RANDFOR_H_ */
