/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "or_dimacscnf.h"

void solwriter_dimacscnf_resetPlugin(){
	#ifdef VERBOSE_SOLWRITER
	printf("c       SOLWRITER: Resetting DIMACS CNF plug-in...\n");
	#endif
	//Currently, there is nothing to reset.
}

void solwriter_dimacscnf_initPlugin(){
	#ifdef VERBOSE_SOLWRITER
	printf("c       SOLWRITER: Initializing DIMACS CNF plug-in...\n");
	#endif
	//Currently, there is nothing to initialize.
}

void solwriter_dimacscnf_disposePlugin(){
	#ifdef VERBOSE_SOLWRITER
	printf("c       SOLWRITER: Disposing DIMACS CNF plug-in...\n");
	#endif
	//Currently, there is nothing to dispose.
}

#ifndef COMPILEASLIBRARY
//Printing the result UNKNOWN.
void solwriter_dimacscnf_printUNKNOWN(){
	#ifndef SILENT
	printf("s UNKNOWN\n");
	#endif
}

//Printing the result SAT.
void solwriter_dimacscnf_printSAT(){
	#ifndef SILENT
	printf("s SATISFIABLE\n");
	#endif
	#ifdef COLLECTSTATS
	float_ty posAssigned = 0, negAssigned = 0, allAssigned = 0;
	#endif
	int32_t i;
	variable* v;
	if (param_solwriterPrintAssignment){
		printf("v ");
		for (i = 1; i < f.n_initial+1; ++i){
			v = main_varData + i;
			if (IS_VAR_UNASSIGNED(v)){
				continue;
			} else {
				if (IS_VAR_TRUE(v)){
					printf("%d ", i);
				} else {
					printf("-%d ", i);
				}
			}
		}
		printf("0\n");
	} else {
		printf("c Assignment omitted (-solwriterPrintAssignment 0 was used).\n");
	}
	#ifdef COLLECTSTATS
	for (i = 1; i < f.n_initial+1; ++i){
		v = main_varData + i;
		if (IS_VAR_UNASSIGNED(v)){
			++stats_solwriter_numUnaVars;
			continue;
		} else {
			if (IS_VAR_TRUE(v)){
				++stats_solwriter_numPosVars;
				++posAssigned;
			} else {
				++stats_solwriter_numNegVars;
				++negAssigned;
			}
		}
	}
	allAssigned = posAssigned + negAssigned;
	if (allAssigned > ZERO){
		stats_solwriter_assignmentBiasInp = (posAssigned - negAssigned) / allAssigned;
		stats_solwriter_assignmentBiasAll = stats_solwriter_assignmentBiasInp;
	}
	stats_solwriter_numAuxVars = 0;
	stats_solwriter_numInpVars = allAssigned;
	stats_solwriter_numAllVars = allAssigned;
	#endif
}

//Printing the result UNSAT.
void solwriter_dimacscnf_printUNSAT(){
	#ifndef SILENT
	printf("s UNSATISFIABLE\n");
	#endif
}
#else
//Printing the result UNKNOWN.
void solwriter_dimacscnf_printUNKNOWN(){}

//Printing the result SAT.
void solwriter_dimacscnf_printSAT(){
	int32_t i;
	variable* v;

	for (i = 1; i <= f.n_initial; ++i){
		v = main_varData + i;
		if (IS_VAR_UNASSIGNED(v)){
			continue;
		} else {
			if (IS_VAR_TRUE(v)){
				dimetheus_ifc_assA[i] = 1;
			} else if (IS_VAR_FALSE(v)) {
				dimetheus_ifc_assA[i] = 0;
			} else {
				dimetheus_ifc_assA[i] = 2;
			}
		}
	}
}

//Printing the result UNSAT.
void solwriter_dimacscnf_printUNSAT(){}
#endif
