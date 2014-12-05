/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef SELECTVARRULES_H_
#define SELECTVARRULES_H_

#include "../cdcl.h"

#define CDCL_SELECTVARRULE_MINVALUE 0
#define CDCL_SELECTVARRULE_MAXVALUE 3

#define CDCL_SELECTVARRULE_RANDOM 0	//Pick a not yet assigned variable randomly.
#include "svr_random.h"

#define CDCL_SELECTVARRULE_VSIDS 1	//Pick a variable according to VSIDS.
#include "svr_vsids.h"

#define CDCL_SELECTVARRULE_RVSIDS 2	//Pick a variable according to VSIDS, but from time to time, make a random decision.
#include "svr_rvsids.h"

#define CDCL_SELECTVARRULE_RRVSIDS 3	//VSIDS, sometimes random, reward variables from clauses with small LBD.
#include "svr_rrvsids.h"

//Select Variable Rule plug-in internal data-structures.
float_ty cdcl_selectVarRules_currentActInc;		//This is the value with which we increase the variable activities (this
												//value gets bumped after each conflict analysis by the bump parameter).
float_ty cdcl_selectVarRules_maxActivity;		//The maximum activity currently encountered.

//Intern methods.
#define CDCL_SELECTVARRULE_VARACTINIT_MINVALUE 0
#define CDCL_SELECTVARRULE_VARACTINIT_MAXVALUE 3

#define CDCL_SELECTVARRULE_VARACTINIT_PROVIDED 0
void cdcl_selectVarRules_intern_varActInit_provided();
void cdcl_selectVarRules_intern_varActUpdate_provided();

#define CDCL_SELECTVARRULE_VARACTINIT_OCCNUM 1
void cdcl_selectVarRules_intern_varActInit_occnum();
void cdcl_selectVarRules_intern_varActUpdate_occnum();

#define CDCL_SELECTVARRULE_VARACTINIT_JERESLOWWANG 2
void cdcl_selectVarRules_intern_varActInit_jereslowwang();
void cdcl_selectVarRules_intern_varActUpdate_jereslowwang();

#define CDCL_SELECTVARRULE_VARACTINIT_BINOCC 3
void cdcl_selectVarRules_intern_varActInit_binocc();
void cdcl_selectVarRules_intern_varActUpdate_binocc();

void cdcl_selectVarRules_intern_varActInit();

static inline void cdcl_selectVarRules_printHelp(){
	cdcl_selectVarRule_random_printHelp();
	cdcl_selectVarRule_vsids_printHelp();
	cdcl_selectVarRule_rvsids_printHelp();
	cdcl_selectVarRule_rrvsids_printHelp();
	printf("c      -cdclSelectVarActInit [uint]: Determines how to initialize the variable activities.\n");
	printf("c        %d: ROVIDED. Use the activities provided by the MAIN component (no additional computations).\n",
			CDCL_SELECTVARRULE_VARACTINIT_PROVIDED);
	printf("c        %d: OCCNUM. The larger the occurrence number of the variable the higher its activity.\n",
			CDCL_SELECTVARRULE_VARACTINIT_OCCNUM);
	printf("c        %d: JERESLOWWANG. The larger the Jereslow-Wang score of the variable the higher its activity.\n",
			CDCL_SELECTVARRULE_VARACTINIT_JERESLOWWANG);
	printf("c        %d: BINOCC. The larger the binary occurrences of the variable the higher its activity.\n",
			CDCL_SELECTVARRULE_VARACTINIT_BINOCC);
}

//Extern interface methods -- can be called by the one who initializes the component.
static inline void cdcl_extern_selectVarRules_printNamesAndIDs(char* prefix){
	//SVR
	printf("%s CDCL-SELECTVAR-RULES: CDCL_SELECTVARRULE_RANDOM %d\n", prefix, CDCL_SELECTVARRULE_RANDOM);
	printf("%s CDCL-SELECTVAR-RULES: CDCL_SELECTVARRULE_VSIDS %d\n", prefix, CDCL_SELECTVARRULE_VSIDS);
	printf("%s CDCL-SELECTVAR-RULES: CDCL_SELECTVARRULE_RVSIDS %d\n", prefix, CDCL_SELECTVARRULE_RVSIDS);
	printf("%s CDCL-SELECTVAR-RULES: CDCL_SELECTVARRULE_RRVSIDS %d\n", prefix, CDCL_SELECTVARRULE_RRVSIDS);

	//SVAI
	printf("%s CDCL-SELECTVAR-VARACTINIT-RULES: CDCL_SELECTVARRULE_VARACTINIT_PROVIDED %d\n", prefix, CDCL_SELECTVARRULE_VARACTINIT_PROVIDED);
	printf("%s CDCL-SELECTVAR-VARACTINIT-RULES: CDCL_SELECTVARRULE_VARACTINIT_OCCNUM %d\n", prefix, CDCL_SELECTVARRULE_VARACTINIT_OCCNUM);
	printf("%s CDCL-SELECTVAR-VARACTINIT-RULES: CDCL_SELECTVARRULE_VARACTINIT_JERESLOWWANG %d\n", prefix, CDCL_SELECTVARRULE_VARACTINIT_JERESLOWWANG);
	printf("%s CDCL-SELECTVAR-VARACTINIT-RULES: CDCL_SELECTVARRULE_VARACTINIT_BINOCC %d\n", prefix, CDCL_SELECTVARRULE_VARACTINIT_BINOCC);
}
void (*cdcl_selectVarRule_prepare)();		//This points to one of the selection rules call preparation methods available.
void (*cdcl_selectVarRule_incAct)(cdclVariable*);	//This points to one of the selection rules activity increase methods.
void (*cdcl_selectVarRule_reward)(uint32_t, cdclClause*);//This points to one of the reward methods.
void (*cdcl_selectVarRule_rescale)(uint32_t);//This points to one of the selection rules activity re-scale methods available.
void (*cdcl_selectVarRule_bump)();			//This is called after the CAR did its work to bump the activity increase value.
cdclVariable* (*cdcl_selectVarRule_select)();//This points to one of the selection rules that are available.

void cdcl_extern_selectVarRules_switch(int32_t);//Use to switch the select rule without re-initializing the whole CDCL module.
void cdcl_selectVarRules_extern_varActUpdate();	//Tell the variable selection heuristic to re-organize its priority queue.

//Interface Methods for initializing the component -- called by the one who wants to use the above methods.
void cdcl_selectVarRules_resetPlugin();
void cdcl_selectVarRules_initPlugin();
void cdcl_selectVarRules_disposePlugin();

#endif /* SELECTVARRULES_H_ */
