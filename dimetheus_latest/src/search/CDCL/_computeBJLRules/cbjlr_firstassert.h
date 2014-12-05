/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef CBJLR_FIRSTASSERT_H_
#define CBJLR_FIRSTASSERT_H_

#include "computeBJLRules.h"

void cdcl_computeBJLRule_firstassert_printHelp();
void cdcl_computeBJLRule_firstassert_prepare();
uint32_t cdcl_computeBJLRule_firstassert_compute();

#endif /* CBJLR_FIRSTASSERT_H_ */
