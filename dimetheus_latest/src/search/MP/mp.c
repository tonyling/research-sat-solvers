/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "mp.h"

void mp_intern_retainMsgs(){
	//This method will walk through all the clauses of the global formula, check which of them was used in the iteration,
	//and copy all the messages locally known to the global messages array. This way we retain the last set of messages.
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	#endif
	#ifdef VERBOSE_MP
	printf("c     MP:   Retaining clause messages.\n");
	#endif
	mp_clauses_retainMsgs();
	#ifdef COLLECTSTATS
	stats_mp_time_retaining += (STATS_GET_CURRENT_TIME_IN_SEC() - time_start);
	#endif
}

void mp_intern_retainBiases(){
	//This method will walk through all the variables of the global formula, and update the variables activity to the bias of
	//the variable after MP converged.
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	#endif
	#ifdef VERBOSE_MP
	printf("c     MP:   Retaining variable biases.\n");
	#endif
	mp_variables_retainBiases();
	#ifdef COLLECTSTATS
	stats_mp_time_retaining += (STATS_GET_CURRENT_TIME_IN_SEC() - time_start);
	#endif
}

void mp_extern_printVersion(){
	printf("[Search          ] [Message Passing                ] :: %3d.%-4d :: %s",
			MP_VERSION_MA, MP_VERSION_MI, MP_VERSION_NAME);
}

void mp_extern_iterate(uint32_t prepareAnew){
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_mp_component_totalCalls;
	#endif
	float_ty minMessageDiff = 1.0;
	uint32_t numIterations = 0, numNonImpIterations = 0;
	mp_returnCode = MP_UNKNOWN;
	//First we prepare the call to the message passing component.
	if (prepareAnew){
		#ifdef VERBOSE_MP
		printf("c     MP: Preparing call...\n");
		printf("c     MP:   Data-structures...\n");
		#endif
		//Prepare the snapshot view of the formula for the MP module.
		mp_variables_prepareCall();
		mp_literals_prepareCall();
		mp_clauses_prepareCall();

		#ifdef VERBOSE_MP
		printf("c     MP:     Variables: %u, Clauses %u\n", mp_varNum, mp_clsUsed);
		printf("c     MP:   Plug-ins...\n");
		#endif
		//Prepare the call to all plug-ins.
		mp_updateRule_prepare();

		//Prepare local parameters.
		perm_LFSR_init(mp_clsUsed);
		#ifdef VERBOSE_MP
		printf("c     MP:   Message passing configuration:\n");
		printf("c     MP:     ");perm_LFSR_printConfig_inline();printf("\n");
		#endif
	}

	#ifdef COLLECTSTATS
	stats_mp_time_init += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	#endif

	//Second, we perform the message passing.
	#ifdef VERBOSE_MP
	printf("c     MP: Iterating (mp_maxMessageDiff): ");
	#endif
	do {
		//A single message passing round (called an iteration) updates all messages and and checks whether the maximum number
		//of iterations is reached. If so, no convergence occurred. Non-convergence might also occur, if too many non-improving
		//iterations were performed. In case non of these situations occur, then the maximum difference between a new and an
		//old message we found during all updates was small enough such that we can assume our message passing converged
		//against a stable configuration of the messages (called an equilibrium).
		mp_maxMessageDiff = 0.0f;
		mp_updateRule_msgUpdate();
		if (mp_returnCode == MP_ERROR) {
			#ifdef COLLECTSTATS
			++stats_mp_return_ERROR;
			#endif
			return;
		}
		//After the iteration, we increase its value.
		++numIterations;
		//We now check if the maximum message difference encountered in the last round improved the situation.
		if (mp_maxMessageDiff < minMessageDiff){
			//Yes, the new message difference is even smaller than the smallest encountered so far. We make it the new
			//smallest value and reset the counter for non-improving iterations.
			minMessageDiff = mp_maxMessageDiff;
			numNonImpIterations = 0;
		} else {
			//No, the new message difference is not smaller than the smallest encountered so far. We increase the counter
			//for non-improving iterations.
			++numNonImpIterations;
		}
		#ifdef VERBOSE_MP
		printf("%1.3f", mp_maxMessageDiff);
		if (minMessageDiff == mp_maxMessageDiff){
			//The last iteration improved the situation.
			printf("+ ");
		} else {
			//The last iteration did not improve the situation.
			printf("- ");
		}
		if (numIterations % 10U == 0U)
			printf("  (%d iterations)\nc     MP:                                ", numIterations);
		fflush(stdout);
		#endif
	} while (//Continue, if...
			(mp_maxMessageDiff > param_mpMaxConvergenceDiff)		//... no equilibrium has been reached, and
			&& (numIterations < param_mpMaxNumIterations)			//... the maximum number of iterations is not reached, and
			&& (numNonImpIterations < param_mpMaxNonImpIterations)	//... not too many non-improving iterations were seen.
			);
	#ifdef VERBOSE_MP
	printf("%d iterations.\n", numIterations);
	#endif

	#ifdef COLLECTSTATS
	stats_mp_passing_numIterations += numIterations;
	stats_mp_time_iterating += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif

	//Third, we evaluate the result.
	if (mp_maxMessageDiff <= param_mpMaxConvergenceDiff){
		//Message passing did converge.
		#ifdef VERBOSE_MP
		printf("c     MP:   Converged (maximum message difference was %f < %f).\n",
				mp_maxMessageDiff, param_mpMaxConvergenceDiff);
		#endif
		mp_returnCode = MP_CONVERGED;
		#ifdef COLLECTSTATS
		++stats_mp_return_CONVERGED;
		#endif
	} else {
		//No convergence.
		#ifdef VERBOSE_MP
		printf("c     MP:   UNCONVERGED (maximum message difference %f, non-improving iterations %d).\n",
				mp_maxMessageDiff, numNonImpIterations);
		#endif
		mp_returnCode = MP_UNCONVERGED;
		#ifdef COLLECTSTATS
		++stats_mp_return_UNCONVERGED;
		#endif
	}
	#ifdef COLLECTSTATS
	stats_mp_time_total += (STATS_GET_CURRENT_TIME_IN_SEC() - time_start);
	#endif
}

void mp_extern_computeBiases(){
	//We compute the global magnetization and biases to detect if the state is paramagnetic or not.
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_mp_component_totalCalls;
	#endif
	#ifdef VERBOSE_MP
	printf("c     MP: Computing variable magnetization, variable biases, global magnetization...\n");
	#endif
	mp_updateRule_computeBias();
	#ifdef COLLECTSTATS
	stats_mp_time_biasCompute += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
	if (mp_globalMagnetization < param_mpMaxMagnetization){
		//The convergence of the message passing lead us to a paramagnetic state.
		#ifdef VERBOSE_MP
		printf("c     MP:   PARAMAGNETIC (global magnetization is %f < %f).\n",
				mp_globalMagnetization, param_mpMaxMagnetization);
		#endif
		mp_returnCode = MP_CONVERGED_PARAMAG;
		#ifdef COLLECTSTATS
		++stats_mp_return_PARAMAG;
		#endif
	} else {
		//The convergence of the message passing lead us to a non-paramagnetic state.
		#ifdef VERBOSE_MP
		printf("c     MP:   Not paramagnetic (global magnetization is %f >= %f).\n",
				mp_globalMagnetization, param_mpMaxMagnetization);
		#endif
		mp_returnCode = MP_CONVERGED_NONPARA;
		#ifdef COLLECTSTATS
		++stats_mp_return_NONPARA;
		#endif
		mp_intern_retainMsgs();
		mp_intern_retainBiases();
	}
	#ifdef COLLECTSTATS
	stats_mp_time_total += (STATS_GET_CURRENT_TIME_IN_SEC() - time_start);
	#endif
}

void mp_resetModule(){
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_mp_component_totalCalls;
	#endif
	#ifdef VERBOSE_MP
	printf("c     MP: Component reset... \n");
	#endif
	mp_returnCode 			= MP_UNKNOWN;
	mp_clauseDeltas 		= NULL;
	mp_maxMessageDiff		= 0;
	mp_globalMagnetization	= 0;

	mp_variables_reset();
	mp_literals_reset();
	mp_clauses_reset();

	mp_updateRules_resetPlugin();
	#ifdef COLLECTSTATS
	stats_mp_time_total += (STATS_GET_CURRENT_TIME_IN_SEC() - time_start);
	#endif
}

void mp_initModule(){
	//We allocate an array to store all factors that participate in the creation of a message. In the worst case, this
	//array has size n (maximum size of a clause).
	#ifdef VERBOSE_MP
	printf("c     MP: Component initialize... \n");
	#endif
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_mp_component_totalCalls;
	#endif
	mp_returnCode = MP_UNKNOWN;

	//Initialize MP module data-structures.
	mp_variables_init(); if (mp_returnCode != MP_UNKNOWN) return;
	mp_literals_init(); if (mp_returnCode != MP_UNKNOWN) return;
	mp_clauses_init(); if (mp_returnCode != MP_UNKNOWN) return;

	//Initialize plug-ins.
	mp_updateRules_initPlugin(); if (mp_returnCode != MP_UNKNOWN) return;

	#ifdef COLLECTSTATS
	stats_mp_time_init += (STATS_GET_CURRENT_TIME_IN_SEC() - time_start);
	stats_mp_time_total += (STATS_GET_CURRENT_TIME_IN_SEC() - time_start);
	#endif
}

void mp_disposeModule(){
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_mp_component_totalCalls;
	#endif
	#ifdef VERBOSE_MP
	printf("c     MP: Component dispose... \n");
	#endif
	if (mp_clauseDeltas != NULL){
		free(mp_clauseDeltas);
		mp_clauseDeltas = NULL;
	}

	//Dispose MP module data-structures.
	mp_variables_dispose();
	mp_literals_dispose();
	mp_clauses_dispose();

	//Dispose plug-ins.
	mp_updateRules_disposePlugin();
	#ifdef COLLECTSTATS
	stats_mp_time_total += (STATS_GET_CURRENT_TIME_IN_SEC() - time_start);
	#endif
}

