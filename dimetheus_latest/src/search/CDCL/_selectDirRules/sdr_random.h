/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef SDR_RANDOM_H_
#define SDR_RANDOM_H_

#include "selectDirRules.h"

void cdcl_selectDirRule_random_printHelp();
void cdcl_selectDirRule_random_save(cdclVariable*);
void cdcl_selectDirRule_random_prepare();
cdclLiteral* cdcl_selectDirRule_random_select();

#endif /* SDR_RANDOM_H_ */
