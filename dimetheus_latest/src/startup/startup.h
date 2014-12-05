/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef STARTUP_H_
#define STARTUP_H_

#include "../dimetheus.h"

uint32_t startup_returnCode;

#define STARTUP_INPUTFORMAT_MINNUM 10
#define STARTUP_INPUTFORMAT_MAXNUM 11

#define STARTUP_INPUTFORMAT_UNKNOWN 0
#define STARTUP_INPUTFORMAT_DIMACSCNF 10
#define STARTUP_INPUTFORMAT_APLEFOO 11

int32_t** startup_cnfClauses;		//After translation, this hosts the clauses of the final CNF.
int32_t startup_cnfClausesNumAvail;	//The number of clauses we have allocated memory for in the first dimension.
int32_t startup_cnfClausesNumUsed;	//The number of clauses the current array really holds.

static inline void startup_extern_printInputFormatName(uint32_t pt){
	if (pt == STARTUP_INPUTFORMAT_DIMACSCNF){
		printf("DIMACSCNF");
	} else if (pt == STARTUP_INPUTFORMAT_APLEFOO){
		printf("APLEFOO");
	} else {
		printf("UNKNOWN");
	}
}

static inline void startup_extern_printKnownFormats(char* prefix){
	printf("%s ", prefix);
	startup_extern_printInputFormatName(STARTUP_INPUTFORMAT_DIMACSCNF);
	printf(" %d\n", STARTUP_INPUTFORMAT_DIMACSCNF);
	printf("%s ", prefix);
	startup_extern_printInputFormatName(STARTUP_INPUTFORMAT_APLEFOO);
	printf(" %d\n", STARTUP_INPUTFORMAT_APLEFOO);
}

//MODULES of the STARTUP phase.
#include "DETERM/determ.h"
#include "SPECCER/speccer.h"
#include "LOADER/loader.h"

void startup_extern_start();

void startup_resetPhase();
void startup_initPhase();
void startup_disposePhase();

#endif /* STARTUP_H_ */
