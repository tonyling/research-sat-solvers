/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */


#include "or_aplefoo.h"

void solwriter_aplefoo_resetPlugin(){
	#ifdef VERBOSE_SOLWRITER
	printf("c       SOLWRITER: Resetting APLEFOO plug-in...\n");
	#endif
	//Currently, there is nothing to reset.
}

void solwriter_aplefoo_initPlugin(){
	#ifdef VERBOSE_SOLWRITER
	printf("c       SOLWRITER: Initializing APLEFOO plug-in...\n");
	#endif
	//Currently, there is nothing to initialize.
}

void solwriter_aplefoo_disposePlugin(){
	#ifdef VERBOSE_SOLWRITER
	printf("c       SOLWRITER: Disposing APLEFOO plug-in...\n");
	#endif
	//Currently, there is nothing to dispose.
}

#ifndef COMPILEASLIBRARY
//Printing the result UNKNOWN.
void solwriter_aplefoo_printUNKNOWN(){
	#ifndef SILENT
	printf("s UNKNOWN\n");
	#endif
}

//Printing the result SAT.
void solwriter_aplefoo_printSAT(){
	#ifndef SILENT
	printf("s SATISFIABLE\n");
	#endif
	#ifdef COLLECTSTATS
	float_ty posAssignedInp = 0, negAssignedInp = 0, allAssignedInp = 0;
	float_ty posAssignedAux = 0, negAssignedAux = 0, allAssignedAux = 0;
	float_ty posAssignedAll = 0, negAssignedAll = 0, allAssignedAll = 0;
	uint32_t numAuxVars = 0, numInpVars = 0, numAllVars = 0;
	#endif
	variable *v;
	int32_t i;

	//We check if we have variable names. If not, we cannot provide a solution!
	if (main_varNames == NULL){
		printf("c ERROR. SOLWRITER cannot provide a solution if the variable array is NULL.\n");
		solwriter_aplefoo_printUNKNOWN();
		solwriter_returnCode = SOLWRITER_ERROR;
		return;
	}

	if (param_solwriterPrintAssignment){
		for (i = 1; i < f.n_initial+1; ++i){
			//First, we check if the variable is an original input variable -- if so, it cannot start with "__dimetheus".
			if (strncmp(main_varNames[i], "__dimetheus", 11) == 0){
				//It is an auxiliary variable or something not part of the input. We ignore this.
			} else {
				//It is an original input variable. We output an assignment, if the variable IS assigned.
				v = main_varData + i;
				if (IS_VAR_UNASSIGNED(v)){
					continue;
				} else {
					if (IS_VAR_TRUE(v)){
						printf("v %s %d\n", main_varNames[i], 1);
					} else {
						printf("v %s %d\n", main_varNames[i], 0);
					}
				}
			}
		}
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
			++numAllVars;
			if (strncmp(main_varNames[i], "__dimetheus", 11) == 0){
				++numAuxVars;
				//It is an auxiliary variable or something not part of the input.
				if (IS_VAR_TRUE(v)){
					++posAssignedAux;
					++posAssignedAll;
				} else {
					++negAssignedAux;
					++negAssignedAll;
				}
			} else {
				++numInpVars;
				//It is an input variable.
				if (IS_VAR_TRUE(v)){
					++posAssignedInp;
					++posAssignedAll;
				} else {
					++negAssignedInp;
					++negAssignedAll;
				}
			}
		}

	}
	stats_solwriter_numAuxVars = numAuxVars;
	stats_solwriter_numInpVars = numInpVars;
	stats_solwriter_numAllVars = numAllVars;

	stats_solwriter_numPosVars = posAssignedAll;
	stats_solwriter_numNegVars = negAssignedAll;
	stats_solwriter_numUnaVars = f.n_initial - stats_solwriter_numPosVars - stats_solwriter_numNegVars;

	allAssignedAux = posAssignedAux + negAssignedAux;
	if (allAssignedAux > ZERO){
		stats_solwriter_assignmentBiasAux = (posAssignedAux - negAssignedAux) / allAssignedAux;
	}
	allAssignedInp = posAssignedInp + negAssignedInp;
	if (allAssignedInp > ZERO){
		stats_solwriter_assignmentBiasInp = (posAssignedInp - negAssignedInp) / allAssignedInp;
	}
	allAssignedAll = posAssignedAll + negAssignedAll;
	if (allAssignedAll > ZERO){
		stats_solwriter_assignmentBiasAll = (posAssignedAll - negAssignedAll) / allAssignedAll;
	}
	#endif
}

//Printing the result UNSAT.
void solwriter_aplefoo_printUNSAT(){
	#ifndef SILENT
	printf("s UNSATISFIABLE\n");
	#endif
}
#else
//Printing the result UNKNOWN.
void solwriter_aplefoo_printUNKNOWN(){}

//Printing the result SAT.
void solwriter_aplefoo_printSAT(){
	variable *v;
	int32_t i;

	//We check if we have variable names. If not, we cannot provide a solution!
	if (main_varNames == NULL){
		printf("c ERROR. SOLWRITER cannot provide a solution if the variable array is NULL.\n");
		solwriter_aplefoo_printUNKNOWN();
		solwriter_returnCode = SOLWRITER_ERROR;
		return;
	}

	for (i = 1; i < f.n_initial+1; ++i){
		//First, we check if the variable is an original input variable -- if so, it cannot start with "__dimetheus".
		if (strncmp(main_varNames[i], "__dimetheus", 11) == 0){
			//It is an auxiliary variable or something not part of the input. We ignore this.
		} else {
			//It is an original input variable. We copy the variable name and assignment into a string for this variable.
			dimetheus_ifc_aplefooSolution[i] = malloc(sizeof(char) * (strlen(main_varNames[i]) + 3));
			v = main_varData + i;
			if (IS_VAR_UNASSIGNED(v)){
				sprintf(dimetheus_ifc_aplefooSolution[i], "%s %d", main_varNames[i], 2);
			} else {
				if (IS_VAR_TRUE(v)){
					sprintf(dimetheus_ifc_aplefooSolution[i], "%s %d", main_varNames[i], 1);
				} else {
					sprintf(dimetheus_ifc_aplefooSolution[i], "%s %d", main_varNames[i], 0);
				}
			}
		}
	}
}

//Printing the result UNSAT.
void solwriter_aplefoo_printUNSAT(){}
#endif
