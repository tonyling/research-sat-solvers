/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "spec_dimacscnf.h"

void speccer_loadSpecs_dimacscnf_resetPlugin(){
	#ifdef VERBOSE_SPECCER
	printf("c       SPECCER: Resetting DIMACS CNF plug-in...\n");
	#endif
	//Currently, there is nothing to reset.
}

void speccer_loadSpecs_dimacscnf_initPlugin(){
	#ifdef VERBOSE_SPECCER
	printf("c       SPECCER: Initializing DIMACS CNF plug-in...\n");
	#endif
	//Currently, there is nothing to initialize.
}

void speccer_loadSpecs_dimacscnf_disposePlugin(){
	#ifdef VERBOSE_SPECCER
	printf("c       SPECCER: Disposing DIMACS CNF plug-in...\n");
	#endif
	//Currently, there is nothing to dispose.
}

#ifndef COMPILEASLIBRARY
void speccer_loadSpecs_dimacscnf(){
	//To load the formula header from file.
	int32_t chr, chr2, pRead = 0;
	char txt[255];

	chr = '#';
	while (chr != 'p' && chr != EOF) {
		while ((chr = fgetc(param_formula)) == 'c') {
			while ((chr2 = fgetc(param_formula)) != EOF && chr2 != '\n');
		}
	}
	ungetc(chr, param_formula);

	if (fscanf(param_formula, "p %s %u %u", txt, &f.n_initial, &f.m_initial) != 3) {
		printf("c ERROR. Problem reading the p-line of the formula file. Does it have format 'p cnf uint uint'?\n");
		speccer_returnCode = SPECCER_ERROR;
		rewind(param_formula);
		return;
	} else {
		pRead = 1;
		f.n_afterPrep = f.n_initial;
	}

	if (!pRead || strcmp("cnf", txt) != 0 ) {
		printf("c ERROR. Formula does not seem to be CNF. Is 'cnf' found in p-line?\n");
		speccer_returnCode = SPECCER_ERROR;
		rewind(param_formula);
		return;
	}

	if (f.n_initial > VAR_OFFSET_MASK){
		printf("c ERROR. The p-line indicates a formula with more than %u variables.\n", VAR_OFFSET_MASK);
		printf("c ERROR. This solver can only solve formulas with less variables.\n");
		speccer_returnCode = SPECCER_ERROR;
		rewind(param_formula);
		return;
	}

	if (f.n_initial == 0){
		printf("c ERROR. Propositional logic does not allow the variable set to be empty, but the p-line states zero.\n");
		speccer_returnCode = SPECCER_ERROR;
		rewind(param_formula);
		return;
	}

	if (f.m_initial > CLS_OFFSET_MASK){
		printf("c ERROR. The p-line indicates a formula with more than %u clauses.\n", CLS_OFFSET_MASK);
		printf("c ERROR. This solver can only solve formulas with less clauses.\n");
		speccer_returnCode = SPECCER_ERROR;
		rewind(param_formula);
		return;
	}

	rewind(param_formula);
}
#else
void speccer_loadSpecs_dimacscnf(){
	//To load the formula header using interface information.
	f.n_initial = dimetheus_ifc_n;
	f.m_initial = dimetheus_ifc_m;
	f.n_afterPrep = f.n_initial;
	if (f.n_initial > VAR_OFFSET_MASK){
		printf("c IFC-ERROR. The information indicates a formula with more than %u variables.\n", VAR_OFFSET_MASK);
		printf("c IFC-ERROR. This solver can only solve formulas with less variables.\n");
		speccer_returnCode = SPECCER_ERROR;
		return;
	}

	if (f.m_initial > CLS_OFFSET_MASK){
		printf("c IFC-ERROR. The information indicates a formula with more than %u clauses.\n", CLS_OFFSET_MASK);
		printf("c IFC-ERROR. This solver can only solve formulas with less clauses.\n");
		speccer_returnCode = SPECCER_ERROR;
		return;
	}
}
#endif
