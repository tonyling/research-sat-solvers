/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "load_dimacscnf.h"

void loader_loadClauses_dimacscnf_resetPlugin(){
	#ifdef VERBOSE_LOADER
	printf("c       LOADER: Resetting DIMACS CNF plug-in...\n");
	#endif
	//Currently, there is nothing to reset.
}

void loader_loadClauses_dimacscnf_initPlugin(){
	#ifdef VERBOSE_LOADER
	printf("c       LOADER: Initializing DIMACS CNF plug-in...\n");
	#endif
	//Currently, there is nothing to initialize.
}

void loader_loadClauses_dimacscnf_disposePlugin(){
	#ifdef VERBOSE_LOADER
	printf("c       LOADER: Disposing DIMACS CNF plug-in...\n");
	#endif
	//Currently, there is nothing to dispose.
}

#ifndef COMPILEASLIBRARY
void loader_loadClauses_dimacscnf(){
	//This method is used to initialize the clauses of a formula that is given in a text file.
	int32_t i,k,lit,isTaut,chr,chr2,toRead = f.m_initial;
	#ifdef VERBOSE_LOADER
	printf("c     LOADER:   Initializing a temporary array for storing clauses.\n");fflush(stdout);
	#endif

	int32_t* temp_array = NULL;
	temp_array = malloc(sizeof(int32_t) * (f.n_initial+1));
	if (temp_array == NULL){
		printf("c ERROR. The allocation of the temporary array to read clauses failed. Out of memory?\n");
		loader_returnCode = LOADER_ERROR;
		return;
	}

	#ifdef VERBOSE_LOADER
	printf("c     LOADER:   Reading clauses from file.\n");fflush(stdout);
	#endif
	for (i = 0; i < toRead; ++i) {
		k = 0; ++main_litStamp; isTaut = 0;
		temp_array[0] = 0;

		chr = fgetc(param_formula);
		//Check if the line is a comment line, a p line, or an empty line.
		if (chr == 'c' || chr == 'p' || chr == '\n') {
			//Yes, so we skip the line.
			#ifdef VERBOSE_LOADER
			printf("c     LOADER:   Ignoring comment/definition/empty line while reading clause definitions.\n");
			#endif
			while ((chr2 = fgetc(param_formula)) != EOF && chr2 != '\n');
			++toRead;
			continue;
		} else {
			//No, we can revert back to the beginning of the line.
			ungetc(chr,param_formula);
		}

		//We read a clause from hard-drive.
		do {
			if (fscanf(param_formula, "%d ", &lit) != 1) {
				printf("c ERROR. Reading literal %d in clause %d failed. Bad input file?\n", lit, i);
				loader_returnCode = LOADER_BADFORMULA;
				break;
			}

			if (abs(lit) > f.n_initial){
				printf("c ERROR. The variable index is exceeded. Read literal %d, maximum in p line is %d.\n",lit,f.n_initial);
				loader_returnCode = LOADER_BADFORMULA;
				break;
			}

			if (lit == 0) {
				temp_array[k] = 0;
				break;
			}

			if (main_litData[-lit].stamp == main_litStamp){
				//The clause we just read is tautological.
				temp_array[k++] = lit;
				main_litData[lit].stamp = main_litStamp;
				++isTaut;
			}

			if (main_litData[lit].stamp != main_litStamp){
				//The literal we just read is not yet in the clause.
				temp_array[k++] = lit;
				main_litData[lit].stamp = main_litStamp;
			}
		} while (1);

		if (loader_returnCode != LOADER_UNKNOWN) break;

		//We check if the just read clause is the empty clause.
		if(temp_array[0] == 0){
			//If the clause is empty, the formula is unsatisfiable by definition.
			#ifdef VERBOSE_LOADER
			printf("c     LOADER:   Clause %d is empty.\n", i);
			#endif
			loader_returnCode = LOADER_UNSAT;
			break;
		}

		//Then we check if the just read clause was a unit clause. If so, we add its implication to the UP stack. If not, we
		//check if the clause is tautological, if so, we ignore it right away. If not, we add it to the clause database.
		if (temp_array[1] == 0){
			//If the clause is a unit clause, we store its only literal into the temporary array.
			lit = temp_array[0];
			if (IS_OPPOSITE_LIT_IN_SIMPLEUPQUEUE(lit)){
				//We have already stamped the opposite literal, and as such, it must already be in the queue. Conflict.
				#ifdef VERBOSE_LOADER
				printf("c     LOADER:   Found unit clause enforcing %d, but the opposite is already enforced.\n", lit);
				#endif
				loader_returnCode = LOADER_UNSAT;
				break;
			} else if (!IS_LIT_IN_SIMPLEUPQUEUE(lit)){
				//The element is not yet in the UP queue. We put it there and stamp it.
				ADD_LIT_TO_SIMPLEUPQUEUE(lit);
			} //Else, the literal is already enqueued.
		} else {
			//We now add the read clause if and only if it is no tautology.
			if (isTaut == 0) {
				//The clause must not be unit in this call. Lucky us, we took care of this before.
				clauses_addOrig(temp_array, k);
				#ifdef COLLECTSTATS
				stats_prep_clauseCoverRateOrig += pow(2, -k);
				#endif
			} else {
				#ifdef VERBOSE_LOADER
				printf("c     LOADER:   Clause %d is tautological and is therefore ignored.\n", i);
				#endif
			}
		}
	}

	if (temp_array != NULL){
		free(temp_array);
	}

	rewind(param_formula);
}
#else
void loader_loadClauses_dimacscnf(){
	//This method is used to initialize the clauses of a formula that is given as two dimensional integer array.
	int32_t k, cNum, lit, isTaut, toRead = f.m_initial;

	int32_t* temp_array = NULL;
	temp_array = malloc(sizeof(int32_t) * (f.n_initial+1));
	if (temp_array == NULL){
		printf("c ERROR. The allocation of the temporary array to read clauses failed. Out of memory?\n");
		loader_returnCode = LOADER_ERROR;
		return;
	}

	for (cNum = 0; cNum < toRead; ++cNum){
		k = 0; ++main_litStamp; isTaut = 0;
		temp_array[0] = 0;
		//We read a clause from the array.
		do {
			lit = dimetheus_ifc_clsA[cNum][k];
			if (abs(lit) > f.n_initial){
				printf("c IFC-ERROR. The literal %d in clause %d is %d. Bad input array?\n", k, cNum, lit);
				loader_returnCode = LOADER_BADFORMULA;
				break;
			}
			if (lit == 0){
				temp_array[k] = 0;
				break;
			}

			if (main_litData[-lit].stamp == main_litStamp){
				//The clause we just read is tautological.
				temp_array[k++] = lit;
				main_litData[lit].stamp = main_litStamp;
				++isTaut;
			}

			if (main_litData[lit].stamp != main_litStamp){
				//The literal we just read is not yet in the clause.
				temp_array[k++] = lit;
				main_litData[lit].stamp = main_litStamp;
			}
		} while (1);

		if (loader_returnCode != LOADER_UNKNOWN) break;

		//We check if the just read clause is the empty clause.
		if(temp_array[0] == 0){
			//If the clause is empty, the formula is unsatisfiable by definition.
			loader_returnCode = LOADER_UNSAT;
			break;
		}

		//Then we check if the just read clause was a unit clause. If so, we add its implication to the UP stack. If not, we
		//check if the clause is tautological, if so, we ignore it right away. If not, we add it to the clause database.
		if (temp_array[1] == 0){
			//If the clause is a unit clause, we store its only literal into the temporary array.
			lit = temp_array[0];
			if (IS_OPPOSITE_LIT_IN_SIMPLEUPQUEUE(lit)){
				//We have already stamped the opposite literal, and as such, it must already be in the queue. Conflict.
				loader_returnCode = LOADER_UNSAT;
				break;
			} else if (!IS_LIT_IN_SIMPLEUPQUEUE(lit)){
				//The element is not yet in the UP queue. We put it there and stamp it.
				ADD_LIT_TO_SIMPLEUPQUEUE(lit);
			} //Else, the literal is already enqueued.
		} else {
			//We now add the read clause if and only if it is no tautology.
			if (isTaut == 0) {
				//The clause must not be unit in this call. Lucky us, we took care of this before.
				clauses_addOrig(temp_array, k);
			}
		}
	}

	if (temp_array != NULL){
		free(temp_array);
	}
}
#endif
