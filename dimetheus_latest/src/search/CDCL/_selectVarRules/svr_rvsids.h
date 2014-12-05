/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef SVR_RVSIDS_H_
#define SVR_RVSIDS_H_

#include "selectVarRules.h"

void cdcl_selectVarRule_rvsids_printHelp();
void cdcl_selectVarRule_rvsids_prepare();
void cdcl_selectVarRule_rvsids_incAct(cdclVariable*);
void cdcl_selectVarRule_rvsids_reward(uint32_t, cdclClause*);
void cdcl_selectVarRule_rvsids_rescale(uint32_t);
void cdcl_selectVarRule_rvsids_bump();
cdclVariable* cdcl_selectVarRule_rvsids_select();

#endif /* SVR_RVSIDS_H_ */
