/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */


#include "load_aplefoo.h"

void loader_loadClauses_aplefoo_resetPlugin(){
	#ifdef VERBOSE_LOADER
	printf("c       LOADER: Resetting APLEFOO plug-in...\n");
	#endif
	//Currently, there is nothing to reset.
}

void loader_loadClauses_aplefoo_initPlugin(){
	#ifdef VERBOSE_LOADER
	printf("c       LOADER: Initializing APLEFOO plug-in...\n");
	#endif
	//Currently, there is nothing to initialize.
}

void loader_loadClauses_aplefoo_disposePlugin(){
	#ifdef VERBOSE_LOADER
	printf("c       LOADER: Disposing APLEFOO plug-in...\n");
	#endif
	//Currently, there is nothing to dispose.
}

void loader_loadClauses_aplefoo(){
	//This method is used to initialize the clauses of a formula that is given in a text file.
	int32_t i,k,lit,isTaut,toRead = f.m_initial;

	#ifdef VERBOSE_LOADER
	printf("c     LOADER:   Reading clauses from startup array.\n");fflush(stdout);
	#endif
	for (i = 0; i < toRead; ++i) {
		k = 0; ++main_litStamp; isTaut = 0;
		do {
			lit = startup_cnfClauses[i][k];

			if (abs(lit) > f.n_initial){
				printf("c ERROR. The variable index is exceeded. Read literal %d, maximum in specification is %d.\n",
						lit,f.n_initial);
				loader_returnCode = LOADER_BADFORMULA;
				break;
			}

			if (lit == 0) {
				break;
			}

			if (main_litData[-lit].stamp == main_litStamp){
				//The clause we just read is tautological.
				++k;
				main_litData[lit].stamp = main_litStamp;
				++isTaut;
			}

			if (main_litData[lit].stamp != main_litStamp){
				//The literal we just read is not yet in the clause.
				++k;
				main_litData[lit].stamp = main_litStamp;
			}
		} while (1);

		if (loader_returnCode != LOADER_UNKNOWN) break;

		//We check if the just read clause is the empty clause.
		if(startup_cnfClauses[i][0] == 0){
			//If the clause is empty, the formula is unsatisfiable by definition.
			#ifdef VERBOSE_LOADER
			printf("c     LOADER:   Clause %d is empty.\n", i);
			#endif
			loader_returnCode = LOADER_UNSAT;
			break;
		}

		//Then we check if the just read clause was a unit clause. If so, we add its implication to the UP stack. If not, we
		//check if the clause is tautological, if so, we ignore it right away. If not, we add it to the clause database.
		if (startup_cnfClauses[i][1] == 0){
			//If the clause is a unit clause, we store its only literal into the temporary array.
			lit = startup_cnfClauses[i][0];
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
				clauses_addOrig(startup_cnfClauses[i], k);
				#ifdef COLLECTSTATS
				stats_prep_clauseCoverRateOrig += pow(2, -k);
				#endif
			} else {
				#ifdef VERBOSE_LOADER
				printf("c     LOADER:   Clause %d is tautological and is therefore ignored.\n", i);
				#endif
			}
		}

		//After we transferred the clause from the startup_cnfClauses array, we can free the memory consumed by it.
		free(startup_cnfClauses[i]);
		startup_cnfClauses[i] = NULL;
	}
}
