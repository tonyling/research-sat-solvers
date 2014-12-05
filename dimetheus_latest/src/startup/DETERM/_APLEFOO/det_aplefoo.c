/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */


#include "det_aplefoo.h"

void determ_determineInputFormat_aplefoo_resetPlugin(){
	#ifdef VERBOSE_DETERM
	printf("c       DETERM: Resetting APLEFOO plug-in...\n");
	#endif
	//Currently, there is nothing to reset.
}

void determ_determineInputFormat_aplefoo_initPlugin(){
	#ifdef VERBOSE_DETERM
	printf("c       DETERM: Initializing APLEFOO plug-in...\n");
	#endif
	//Currently, there is nothing to initialize.
}

void determ_determineInputFormat_aplefoo_disposePlugin(){
	#ifdef VERBOSE_DETERM
	printf("c       DETERM: Disposing APLEFOO plug-in...\n");
	#endif
	//Currently, there is nothing to dispose.
}

#ifndef COMPILEASLIBRARY
uint32_t determ_determineInputFormat_aplefoo(){
	int32_t chr, chr2;
	char txt[255];
	chr = '#';
	while (chr != 'p' && chr != EOF) {
		while ((chr = fgetc(param_formula)) == 'c') {
			while ((chr2 = fgetc(param_formula)) != EOF && chr2 != '\n');
		}
	}
	ungetc(chr, param_formula);

	if (fscanf(param_formula, "p %s", txt) == 1) {
		if (strcmp("aplefoo", txt) == 0 ) {
			rewind(param_formula);
			return STARTUP_INPUTFORMAT_APLEFOO;
		}
	}

	rewind(param_formula);
	return STARTUP_INPUTFORMAT_UNKNOWN;
}
#else
uint32_t determ_determineInputFormat_aplefoo(){
	if (param_startupInputFormat == STARTUP_INPUTFORMAT_APLEFOO){
		return STARTUP_INPUTFORMAT_APLEFOO;
	} else {
		return STARTUP_INPUTFORMAT_UNKNOWN;
	}
}
#endif
