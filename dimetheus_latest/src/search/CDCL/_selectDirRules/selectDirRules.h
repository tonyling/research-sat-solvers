/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef SELECTDIRRULES_H_
#define SELECTDIRRULES_H_

#include "../cdcl.h"

#define CDCL_SELECTDIRRULE_MINVALUE 0
#define CDCL_SELECTDIRRULE_MAXVALUE 3

#define CDCL_SELECTDIRRULE_RANDOM 0	//Pick an assignment randomly.
#include "sdr_random.h"

#define CDCL_SELECTDIRRULE_PHASESAVING 1 //Pick the last assignment to this variable that was used.
#include "sdr_phasesaving.h"

#define CDCL_SELECTDIRRULE_FAILBINARY 2 //Pick the assignment that fails more binary clauses, or uses the last phase.
#include "sdr_failbinary.h"

#define CDCL_SELECTDIRRULE_PHASEFLIP 3 //Does phase-saving but flips the phase in DL1 from time to time.
#include "sdr_phaseflip.h"

//Select Direction Rule plug-in internal data-structures.
//(none yet)

//Intern methods.
#define CDCL_SELECTDIRRULE_DIRINIT_MINVALUE 0
#define CDCL_SELECTDIRRULE_DIRINIT_MAXVALUE 7

#define CDCL_SELECTDIRRULE_DIRINIT_PROVIDED 0
void cdcl_selectDirRules_intern_dirInit_provided();
void cdcl_selectDirRules_intern_dirUpdate_provided();

#define CDCL_SELECTDIRRULE_DIRINIT_ZEROFIRST 1
void cdcl_selectDirRules_intern_dirInit_zerofirst();
void cdcl_selectDirRules_intern_dirUpdate_zerofirst();

#define CDCL_SELECTDIRRULE_DIRINIT_ONEFIRST 2
void cdcl_selectDirRules_intern_dirInit_onefirst();
void cdcl_selectDirRules_intern_dirUpdate_onefirst();

#define CDCL_SELECTDIRRULE_DIRINIT_MAXOCC 3
void cdcl_selectDirRules_intern_dirInit_maxocc();
void cdcl_selectDirRules_intern_dirUpdate_maxocc();

#define CDCL_SELECTDIRRULE_DIRINIT_JERESLOWWANG 4
void cdcl_selectDirRules_intern_dirInit_jereslowwang();
void cdcl_selectDirRules_intern_dirUpdate_jereslowwang();

#define CDCL_SELECTDIRRULE_DIRINIT_MAXOCCFAIL 5
void cdcl_selectDirRules_intern_dirInit_maxoccfail();
void cdcl_selectDirRules_intern_dirUpdate_maxoccfail();

#define CDCL_SELECTDIRRULE_DIRINIT_JERESLOWWANGFAIL 6
void cdcl_selectDirRules_intern_dirInit_jereslowwangfail();
void cdcl_selectDirRules_intern_dirUpdate_jereslowwangfail();

#define CDCL_SELECTDIRRULE_DIRINIT_BINFAIL 7
void cdcl_selectDirRules_intern_dirInit_binfail();
void cdcl_selectDirRules_intern_dirUpdate_binfail();

void cdcl_selectDirRules_intern_dirInit();

static inline void cdcl_selectDirRules_printHelp(){
	cdcl_selectDirRule_random_printHelp();
	cdcl_selectDirRule_phasesaving_printHelp();
	cdcl_selectDirRule_failbinary_printHelp();
	printf("c      -cdclSelectDirDirInit [uint]: Determines how to initialize the variable derections.\n");
	printf("c        %d: ROVIDED. Use the directions provided by the MAIN component (no additional computations).\n",
			CDCL_SELECTDIRRULE_DIRINIT_PROVIDED);
	printf("c        %d: ZEROFIRST. Use false first.\n",
			CDCL_SELECTDIRRULE_DIRINIT_ZEROFIRST);
	printf("c        %d: ZEROFIRST. Use true first.\n",
			CDCL_SELECTDIRRULE_DIRINIT_ONEFIRST);
	printf("c        %d: MAXOCC. The direction with more occurrences is picked first.\n",
			CDCL_SELECTDIRRULE_DIRINIT_MAXOCC);
	printf("c        %d: JERESLOWWANG. The direction with higher Jereslow-Wang score is picked first.\n",
				CDCL_SELECTDIRRULE_DIRINIT_JERESLOWWANG);
	printf("c        %d: MAXOCCFAIL. The direction with less occurrences is picked first.\n",
				CDCL_SELECTDIRRULE_DIRINIT_MAXOCCFAIL);
	printf("c        %d: JERESLOWWANGFAIL. The direction with lower Jereslow-Wang score is picked first.\n",
				CDCL_SELECTDIRRULE_DIRINIT_JERESLOWWANGFAIL);
	printf("c        %d: BINFAIL. The direction with more implications is picked first.\n",
				CDCL_SELECTDIRRULE_DIRINIT_BINFAIL);
}

//Extern interface methods -- can be called by the one who initializes the component.
static inline void cdcl_extern_selectDirRules_printNamesAndIDs(char* prefix){
	//SDR
	printf("%s CDCL-SELECTDIR-RULES: CDCL_SELECTDIRRULE_RANDOM %d\n", prefix, CDCL_SELECTDIRRULE_RANDOM);
	printf("%s CDCL-SELECTDIR-RULES: CDCL_SELECTDIRRULE_PHASESAVING %d\n", prefix, CDCL_SELECTDIRRULE_PHASESAVING);
	printf("%s CDCL-SELECTDIR-RULES: CDCL_SELECTDIRRULE_FAILBINARY %d\n", prefix, CDCL_SELECTDIRRULE_FAILBINARY);
	printf("%s CDCL-SELECTDIR-RULES: CDCL_SELECTDIRRULE_PHASEFLIP %d\n", prefix, CDCL_SELECTDIRRULE_PHASEFLIP);

	//SDDI
	printf("%s CDCL-SELECTDIR-DIRINIT-RULES: CDCL_SELECTDIRRULE_DIRINIT_PROVIDED %d\n", prefix, CDCL_SELECTDIRRULE_DIRINIT_PROVIDED);
	printf("%s CDCL-SELECTDIR-DIRINIT-RULES: CDCL_SELECTDIRRULE_DIRINIT_ZEROFIRST %d\n", prefix, CDCL_SELECTDIRRULE_DIRINIT_ZEROFIRST);
	printf("%s CDCL-SELECTDIR-DIRINIT-RULES: CDCL_SELECTDIRRULE_DIRINIT_ONEFIRST %d\n", prefix, CDCL_SELECTDIRRULE_DIRINIT_ONEFIRST);
	printf("%s CDCL-SELECTDIR-DIRINIT-RULES: CDCL_SELECTDIRRULE_DIRINIT_MAXOCC %d\n", prefix, CDCL_SELECTDIRRULE_DIRINIT_MAXOCC);
	printf("%s CDCL-SELECTDIR-DIRINIT-RULES: CDCL_SELECTDIRRULE_DIRINIT_JERESLOWWANG %d\n", prefix, CDCL_SELECTDIRRULE_DIRINIT_JERESLOWWANG);
	printf("%s CDCL-SELECTDIR-DIRINIT-RULES: CDCL_SELECTDIRRULE_DIRINIT_MAXOCCFAIL %d\n", prefix, CDCL_SELECTDIRRULE_DIRINIT_MAXOCCFAIL);
	printf("%s CDCL-SELECTDIR-DIRINIT-RULES: CDCL_SELECTDIRRULE_DIRINIT_JERESLOWWANGFAIL %d\n", prefix, CDCL_SELECTDIRRULE_DIRINIT_JERESLOWWANGFAIL);
	printf("%s CDCL-SELECTDIR-DIRINIT-RULES: CDCL_SELECTDIRRULE_DIRINIT_BINFAIL %d\n", prefix, CDCL_SELECTDIRRULE_DIRINIT_BINFAIL);
}
void (*cdcl_selectDirRule_prepare)();		//This points to one of the selection rules call preparation methods available.
void (*cdcl_selectDirRule_save)(cdclVariable*);//This performs a retaining operation for a variable to be unassigned.
cdclLiteral* (*cdcl_selectDirRule_select)();//This points to one of the selection rules that are available.

void cdcl_extern_selectDirRules_switch(int32_t);//Use to switch the select rule without re-initializing the whole CDCL module.
void cdcl_selectDirRules_extern_dirUpdate();//Updates the variable directions using the main information available.

//Interface Methods for initializing the component -- called by the one who wants to use the above methods.
void cdcl_selectDirRules_resetPlugin();
void cdcl_selectDirRules_initPlugin();
void cdcl_selectDirRules_disposePlugin();

#endif /* SELECTDIRRULES_H_ */
