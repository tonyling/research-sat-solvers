/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef SDR_FAILBINARY_H_
#define SDR_FAILBINARY_H_

#include "selectDirRules.h"

void cdcl_selectDirRule_failbinary_printHelp();
void cdcl_selectDirRule_failbinary_save(cdclVariable*);
void cdcl_selectDirRule_failbinary_prepare();
cdclLiteral* cdcl_selectDirRule_failbinary_select();

#endif /* SDR_FAILBINARY_H_ */
