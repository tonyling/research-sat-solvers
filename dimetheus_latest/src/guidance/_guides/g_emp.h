/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef G_EMP_H_
#define G_EMP_H_

#include "../guidance.h"

void guide_emp_printHelp();
void guide_emp_apply(int, char**);
void guide_emp_enforceAfterAdaptation();

#endif /* G_EMP_H_ */
