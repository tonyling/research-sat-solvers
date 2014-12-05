/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef SVR_RRVSIDS_H_
#define SVR_RRVSIDS_H_

#include "selectVarRules.h"

void cdcl_selectVarRule_rrvsids_printHelp();
void cdcl_selectVarRule_rrvsids_prepare();
void cdcl_selectVarRule_rrvsids_incAct(cdclVariable*);
void cdcl_selectVarRule_rrvsids_reward(uint32_t, cdclClause*);
void cdcl_selectVarRule_rrvsids_rescale(uint32_t);
void cdcl_selectVarRule_rrvsids_bump();
cdclVariable* cdcl_selectVarRule_rrvsids_select();

#endif /* SVR_RRVSIDS_H_ */
