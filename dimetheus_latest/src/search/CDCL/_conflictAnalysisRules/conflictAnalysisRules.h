/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef CONFLICTANALYSISRULES_H_
#define CONFLICTANALYSISRULES_H_

#include "../cdcl.h"

#define CDCL_CONFLICTANALYSISRULE_MINVALUE 0
#define CDCL_CONFLICTANALYSISRULE_MAXVALUE 3

#define CDCL_CONFLICTANALYSISRULE_ALLDEC 0	//Learn a new clause by inverting all the decisions.
#include "car_alldec.h"

#define CDCL_CONFLICTANALYSISRULE_FIRSTUIP 1 //Learn a new clause by analyzing the conflict graph in the FirstUIP scheme.
#include "car_firstuip.h"

#define CDCL_CONFLICTANALYSISRULE_LASTUIP 2 //Learn a new clause by analyzing the conflict graph in the LastUIP scheme.
#include "car_lastuip.h"

#define CDCL_CONFLICTANALYSISRULE_ALLUIP 3 //Learn a new clause for each UIP found in the conflict graph.
#include "car_alluip.h"

//Conflict Analysis Rule plug-in data-structures.
//(none yet)

static inline void cdcl_conflictAnalysisRules_printHelp(){
	cdcl_conflictAnalysisRule_alldec_printHelp();
	cdcl_conflictAnalysisRule_firstuip_printHelp();
	cdcl_conflictAnalysisRule_lastuip_printHelp();
	cdcl_conflictAnalysisRule_alluip_printHelp();
}

//Extern interface methods -- can be called by the one who initializes the component.
static inline void cdcl_extern_conflictAnalysisRules_printNamesAndIDs(char* prefix){
	printf("%s CDCL-CONFLICTANALYSIS-RULES: CDCL_CONFLICTANALYSISRULE_ALLDEC %d\n", prefix, CDCL_CONFLICTANALYSISRULE_ALLDEC);
	printf("%s CDCL-CONFLICTANALYSIS-RULES: CDCL_CONFLICTANALYSISRULE_FIRSTUIP %d\n", prefix, CDCL_CONFLICTANALYSISRULE_FIRSTUIP);
	printf("%s CDCL-CONFLICTANALYSIS-RULES: CDCL_CONFLICTANALYSISRULE_LASTUIP %d\n", prefix, CDCL_CONFLICTANALYSISRULE_LASTUIP);
	printf("%s CDCL-CONFLICTANALYSIS-RULES: CDCL_CONFLICTANALYSISRULE_ALLUIP %d\n", prefix, CDCL_CONFLICTANALYSISRULE_ALLUIP);
}

void (*cdcl_conflictAnalysisRule_prepare)();//This points to one of the analysis rules call preparation methods available.
uint32_t (*cdcl_conflictAnalysisRule_analyze)();//This points to one of the analysis rules that are available. It
												//returns the number x of clauses that have been learned. These clauses are
												//found in the cdcl_cls_l array in the positions cdcl_cls_l_used - x to
												//cdcl_cls_l_used - 1.
												//The conflict analysis will NOT append the clauses in the watcher or
												//occurrence lists, because they might get strengthened further.
												//Conflict analysis must always set the correct size value and terminate the
												//literals array with 0.

void cdcl_extern_conflictAnalysisRules_switch(int32_t);//Use to switch the analysis rule without re-initializing the CDCL.

//Interface Methods for initializing the component -- called by the one who wants to use the above methods.
void cdcl_conflictAnalysisRules_resetPlugin();
void cdcl_conflictAnalysisRules_initPlugin();
void cdcl_conflictAnalysisRules_disposePlugin();

#endif /* CONFLICTANALYSISRULES_H_ */
