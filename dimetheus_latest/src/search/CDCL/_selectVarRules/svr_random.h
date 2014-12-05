/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef SVR_RANDOM_H_
#define SVR_RANDOM_H_

#include "selectVarRules.h"

void cdcl_selectVarRule_random_printHelp();
void cdcl_selectVarRule_random_prepare();
void cdcl_selectVarRule_random_incAct(cdclVariable*);
void cdcl_selectVarRule_random_reward(uint32_t, cdclClause*);
void cdcl_selectVarRule_random_rescale(uint32_t);
void cdcl_selectVarRule_random_bump();
cdclVariable* cdcl_selectVarRule_random_select();

#endif /* SVR_RANDOM_H_ */
