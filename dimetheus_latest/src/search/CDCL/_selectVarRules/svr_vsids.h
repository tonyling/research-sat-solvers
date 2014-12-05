/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef SVR_VSIDS_H_
#define SVR_VSIDS_H_

#include "selectVarRules.h"

void cdcl_selectVarRule_vsids_printHelp();
void cdcl_selectVarRule_vsids_prepare();
void cdcl_selectVarRule_vsids_incAct(cdclVariable*);
void cdcl_selectVarRule_vsids_reward(uint32_t, cdclClause*);
void cdcl_selectVarRule_vsids_rescale(uint32_t);
void cdcl_selectVarRule_vsids_bump();
cdclVariable* cdcl_selectVarRule_vsids_select();

#endif /* SVR_VSIDS_H_ */
