/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef CLASSIFY_H_
#define CLASSIFY_H_

#include "../dimetheus.h"

#define CLASSIFY_INPUTDOMAIN_MINNUM 10
#define CLASSIFY_INPUTDOMAIN_MAXNUM 12

#define CLASSIFY_INPUTDOMAIN_GENERIC 0
#define CLASSIFY_INPUTDOMAIN_RANDOM 10
#define CLASSIFY_INPUTDOMAIN_CRAFTED 11
#define CLASSIFY_INPUTDOMAIN_APPLICATION 12

uint32_t classify_domainVotes_random;
uint32_t classify_domainVotes_crafted;
uint32_t classify_domainVotes_application;

static inline void classify_extern_printInputDomainName(uint32_t id){
	if (id == CLASSIFY_INPUTDOMAIN_GENERIC){
		printf("GENERIC");
	} else if (id == CLASSIFY_INPUTDOMAIN_RANDOM){
		printf("RANDOM");
	} else if (id == CLASSIFY_INPUTDOMAIN_CRAFTED){
		printf("CRAFTED");
	} else if (id == CLASSIFY_INPUTDOMAIN_APPLICATION){
		printf("APPLICATION");
	} else {
		printf("UNKNOWN");
	}
}

static inline void classify_extern_printKnownDomains(char* prefix){
	printf("%s ", prefix); classify_extern_printInputDomainName(CLASSIFY_INPUTDOMAIN_GENERIC); printf(" %d\n", CLASSIFY_INPUTDOMAIN_GENERIC);
	printf("%s ", prefix); classify_extern_printInputDomainName(CLASSIFY_INPUTDOMAIN_RANDOM); printf(" %d\n", CLASSIFY_INPUTDOMAIN_RANDOM);
	printf("%s ", prefix); classify_extern_printInputDomainName(CLASSIFY_INPUTDOMAIN_CRAFTED); printf(" %d\n", CLASSIFY_INPUTDOMAIN_CRAFTED);
	printf("%s ", prefix); classify_extern_printInputDomainName(CLASSIFY_INPUTDOMAIN_APPLICATION); printf(" %d\n", CLASSIFY_INPUTDOMAIN_APPLICATION);
}

#include "_classifyStrategies/classifyStrategies.h"

//MODULES for the classification phase.
#include "ATTRIBUTOR/attributor.h"
#include "RANDOMFOREST/randomforest.h"


//Global variables of the classification phase.
int32_t classify_returnCode;			//The return code of the classification phase.
int32_t classify_numLitsToEnforceUsed;	//The number of literals that can be assigned after attribute computation.
int32_t classify_numLitsToEnforceAvail;	//The number of literals that can be stored for assignment.
int32_t* classify_litsToEnforce;		//Literals that can be assigned after classification; may contain them multiple times.
uint32_t classify_numClasses;			//The number of classes we have in total.
uint32_t* classify_classVotes;			//The number of votes for each class.

//Methods provided exclusively to this phase (INTERN).
void classify_intern_simplify();		//Once the classification is done, the classify might simplify the formula.
void classify_intern_simplify_addLitToEnforce(int32_t);	//This is used to store necessary assignments.
void classify_intern_voteForClass(uint32_t);	//This votes for a specific CNF class.
void classify_intern_setPrecision();	//This sets the precision of floating point values after the attribute computation.
void classify_intern_determineClass();	//This determines a CNF class after the voting process.

//Methods PROVIDED by this phase (EXTERN).
void classify_extern_classify();	//Executes the classification on the current formula.

//Methods to initialize this phase (are called by the one who wants to execute the above functions).
void classify_resetPhase();				//Reset the classification.
void classify_initPhase();				//For the classification.
void classify_disposePhase();			//For freeing all related memory.

#endif /* CLASSIFY_H_ */
