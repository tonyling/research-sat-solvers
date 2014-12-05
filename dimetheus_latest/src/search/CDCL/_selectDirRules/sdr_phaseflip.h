/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef SDR_PHASEFLIP_H_
#define SDR_PHASEFLIP_H_

#include "selectDirRules.h"

uint32_t cdcl_selectDirRule_phaseflip_isFlipping;
uint32_t cdcl_selectDirRule_phaseflip_notFlipped;

void cdcl_selectDirRule_phaseflip_printHelp();
void cdcl_selectDirRule_phaseflip_save(cdclVariable*);
void cdcl_selectDirRule_phaseflip_prepare();
cdclLiteral* cdcl_selectDirRule_phaseflip_select();

#endif /* SDR_PHASEFLIP_H_ */
