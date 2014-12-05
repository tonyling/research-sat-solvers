/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef SCORING_H_
#define SCORING_H_

#include "tools.h"

#ifdef SCORING


void scoring_reset();		//To reset everything back to NULL.
void scoring_init();		//To initialize, once the formula specifics are known.
void scoring_dispose();		//To dispose everything.
void scoring_print();		//To print out the final scores.
#endif

#endif /* SCORING_H_ */
