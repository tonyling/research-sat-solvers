/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef SDR_PHASESAVING_H_
#define SDR_PHASESAVING_H_

#include "selectDirRules.h"

void cdcl_selectDirRule_phasesaving_printHelp();
void cdcl_selectDirRule_phasesaving_save(cdclVariable*);
void cdcl_selectDirRule_phasesaving_prepare();
cdclLiteral* cdcl_selectDirRule_phasesaving_select();

#endif /* SDR_PHASESAVING_H_ */
