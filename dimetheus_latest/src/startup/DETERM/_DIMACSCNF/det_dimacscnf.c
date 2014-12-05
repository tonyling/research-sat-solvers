/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "det_dimacscnf.h"

void determ_determineInputFormat_dimacscnf_resetPlugin(){
	#ifdef VERBOSE_DETERM
	printf("c       DETERM: Resetting DIMACS CNF plug-in...\n");
	#endif
	//Currently, there is nothing to reset.
}

void determ_determineInputFormat_dimacscnf_initPlugin(){
	#ifdef VERBOSE_DETERM
	printf("c       DETERM: Initializing DIMACS CNF plug-in...\n");
	#endif
	//Currently, there is nothing to initialize.
}

void determ_determineInputFormat_dimacscnf_disposePlugin(){
	#ifdef VERBOSE_DETERM
	printf("c       DETERM: Disposing DIMACS CNF plug-in...\n");
	#endif
	//Currently, there is nothing to dispose.
}

#ifndef COMPILEASLIBRARY
uint32_t determ_determineInputFormat_dimacscnf(){
	int32_t chr, chr2;
	uint32_t n, m;
	char txt[255];
	chr = '#';
	while (chr != 'p' && chr != EOF) {
		while ((chr = fgetc(param_formula)) == 'c') {
			while ((chr2 = fgetc(param_formula)) != EOF && chr2 != '\n');
		}
	}
	ungetc(chr, param_formula);

	if (fscanf(param_formula, "p %s %u %u", txt, &n, &m) == 3) {
		if (strcmp("cnf", txt) == 0 ) {
			rewind(param_formula);
			return STARTUP_INPUTFORMAT_DIMACSCNF;
		}
	}

	rewind(param_formula);
	return STARTUP_INPUTFORMAT_UNKNOWN;
}
#else
uint32_t determ_determineInputFormat_dimacscnf(){
	if (param_startupInputFormat == STARTUP_INPUTFORMAT_DIMACSCNF){
		return STARTUP_INPUTFORMAT_DIMACSCNF;
	} else {
		return STARTUP_INPUTFORMAT_UNKNOWN;
	}
}
#endif
