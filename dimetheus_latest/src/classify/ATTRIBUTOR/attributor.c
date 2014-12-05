/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "attributor.h"

void attributor_extern_printVersion(){
	printf("[Classify        ] [Attribute Computer             ] :: %3d.%-4d :: %s",
			ATTRIBUTOR_VERSION_MA, ATTRIBUTOR_VERSION_MI, ATTRIBUTOR_VERSION_NAME);
}

void attributor_resetModule(){
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_attributor_component_totalCalls;
	#endif
	#ifdef VERBOSE_ATTRIBUTOR
	printf("c     ATTRIBUTOR: Component reset... \n");
	#endif
	attributor_returnCode = ATTRIBUTOR_UNKNOWN;
	attributor_prioQa = NULL;
	attributor_prioQa_size = 0;
	attributor_prioQa_leaf = 0;
	attributor_prioQb = NULL;
	attributor_prioQb_size = 0;
	attributor_prioQb_leaf = 0;

	attributor_graph = NULL;

	attributor_ELS_classes = NULL;
	attributor_ELS_classes_avail = 0;
	attributor_ELS_classes_used = 0;
	attributor_ELS_classes_memUsed = NULL;

	attributor_nrdfsStack = NULL;
	attributor_nrdfsStack_size = 0;
	attributor_nrdfsStack_leaf = 0;
	#ifdef COLLECTSTATS
	stats_attributor_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}

void attributor_initModule(){
	#ifdef VERBOSE_ATTRIBUTOR
	printf("c     ATTRIBUTOR: Component initialize... \n");
	#endif
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_attributor_component_totalCalls;
	#endif
	uint32_t i,num;

	attributor_returnCode = ATTRIBUTOR_UNKNOWN;

	attributor_prioQa = malloc(sizeof(float_ty)*(BLOCKSIZE + 2));
	if (attributor_prioQa == NULL){
		printf("c ERROR. Could not allocate the priority queue for the ATTRIBUTOR module.\n");
		attributor_returnCode = ATTRIBUTOR_ERROR;
		return;
	}
	attributor_prioQa_size = BLOCKSIZE;
	attributor_prioQa_leaf = 1;

	attributor_prioQb = malloc(sizeof(float_ty)*(BLOCKSIZE + 2));
	if (attributor_prioQb == NULL){
		printf("c ERROR. Could not allocate the priority queue for the ATTRIBUTOR module.\n");
		attributor_returnCode = ATTRIBUTOR_ERROR;
		return;
	}
	attributor_prioQb_size = BLOCKSIZE;
	attributor_prioQb_leaf = 1;

	//Allocate a minimal graph that can hold all clauses and all variables.
	attributor_graph = malloc(sizeof(attributor_adjacencyListSet));
	if (attributor_graph == NULL){
		printf("c ERROR. Could not allocate the graph for the ATTRIBUTOR module.\n");
		attributor_returnCode = ATTRIBUTOR_ERROR;
		return;
	}
	//Allocate an amount of adjacency lists for the graph such that it can hold all variable or clause indexes.
	if (f.m_eo_used > f.n_initial) {
		num = f.m_eo_used + 1;
	} else {
		num = f.n_initial + 1;
	}
	attributor_graph->lists = NULL;
	attributor_graph->lists = malloc(sizeof(attributor_adjacencyList*) * num);
	if (attributor_graph->lists == NULL){
		printf("c ERROR. Could not allocate the graph list set for the ATTRIBUTOR module.\n");
		attributor_returnCode = ATTRIBUTOR_ERROR;
		return;
	}
	attributor_graph->numListsAvail = num;
	attributor_graph->numListsUsed = 0;
	//Initialize the list to small size.
	for (i = 0; i < num; ++i){
		ATTRIBUTOR_GRAPH_ADD_LIST(BLOCKSIZE);
	}

	attributor_ELS_classes_avail = BLOCKSIZE;
	attributor_ELS_classes_used = 0;

	attributor_ELS_classes = malloc(sizeof(literal**) * attributor_ELS_classes_avail);
	if (attributor_ELS_classes == NULL){
		printf("c ERROR. Failed to initialize the ELS classes array. Out of memory?\n");
		attributor_returnCode = ATTRIBUTOR_ERROR;
		return;
	}

	attributor_ELS_classes_memUsed = malloc(sizeof(uint32_t) * attributor_ELS_classes_avail);
	if (attributor_ELS_classes_memUsed == NULL){
		printf("c ERROR. Failed to initialize the ELS memory usage array. Out of memory?\n");
		attributor_returnCode = ATTRIBUTOR_ERROR;
		return;
	}

	attributor_ELS_classes_memAvail = malloc(sizeof(uint32_t) * attributor_ELS_classes_avail);
	if (attributor_ELS_classes_memAvail == NULL){
		printf("c ERROR. Failed to initialize the ELS memory availability array. Out of memory?\n");
		attributor_returnCode = ATTRIBUTOR_ERROR;
		return;
	}

	for (i = 0; i < attributor_ELS_classes_avail; ++i){
		if (attributor_returnCode == ATTRIBUTOR_UNKNOWN){
			attributor_ELS_classes[i] = NULL;
			attributor_ELS_classes[i] = malloc(sizeof(literal*) * BLOCKSIZE);
			if (attributor_ELS_classes[i] == NULL){
				attributor_returnCode = ATTRIBUTOR_ERROR;
				attributor_ELS_classes_memUsed[i] = 0;
				attributor_ELS_classes_memAvail[i] = 0;
			} else {
				attributor_ELS_classes_memUsed[i] = 0;
				attributor_ELS_classes_memAvail[i] = BLOCKSIZE;
			}
		} else {
			attributor_ELS_classes[i] = NULL;
			attributor_ELS_classes_memUsed[i] = 0;
			attributor_ELS_classes_memAvail[i] = 0;
		}
	}

	attributor_nrdfsStack_size = BLOCKSIZE;
	attributor_nrdfsStack = malloc(sizeof(nrdfsStackNode*)*attributor_nrdfsStack_size);
	if (attributor_nrdfsStack == NULL){
		printf("c ERROR. Failed to initialize the NRDFSStack array. Out of memory?\n");
		attributor_returnCode = ATTRIBUTOR_ERROR;
		return;
	}
	attributor_nrdfsStack_leaf = 0;

	#ifdef COLLECTSTATS
	stats_attributor_time_init += (STATS_GET_CURRENT_TIME_IN_SEC() - time_start);
	stats_attributor_time_total += (STATS_GET_CURRENT_TIME_IN_SEC() - time_start);
	#endif
}

void attributor_disposeModule(){
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_attributor_component_totalCalls;
	#endif
	#ifdef VERBOSE_CLASS
	printf("c     ATTRIBUTOR: Component dispose...\n");
	#endif

	uint32_t i;

	attributor_returnCode = ATTRIBUTOR_UNKNOWN;

	if (attributor_prioQa != NULL){
		free(attributor_prioQa);
		attributor_prioQa = NULL;
	}
	attributor_prioQa_size = 0;
	attributor_prioQa_leaf = 0;

	if (attributor_prioQb != NULL){
		free(attributor_prioQb);
		attributor_prioQb = NULL;
	}
	attributor_prioQb_size = 0;
	attributor_prioQb_leaf = 0;

	//Check if a graph is allocated.
	if (attributor_graph != NULL){
		//Check if the graph lists are allocated.
		if (attributor_graph->lists != NULL){
			//Free each element in the graph list.
			for (i = 0; i < attributor_graph->numListsAvail; ++i){
				//Check if the list element is set.
				if (attributor_graph->lists[i] != NULL){
					//Check if the element has an allocated element list.
					if (attributor_graph->lists[i]->elts != NULL){
						//Free the element list.
						free(attributor_graph->lists[i]->elts);
						attributor_graph->lists[i]->elts = NULL;
					}
					//Free the element.
					free(attributor_graph->lists[i]);
					attributor_graph->lists[i] = NULL;
				}
			}
			//Free the graph list itself.
			free(attributor_graph->lists);
			attributor_graph->lists = NULL;
		}
		//Free the graph.
		free(attributor_graph);
		attributor_graph = NULL;
	}

	if (attributor_ELS_classes != NULL){
		for (i = 0; i < attributor_ELS_classes_avail; ++i){
			if (attributor_ELS_classes[i] != NULL){
				free(attributor_ELS_classes[i]);
				attributor_ELS_classes[i] = NULL;
			}
		}
		free(attributor_ELS_classes);
		attributor_ELS_classes = NULL;
	}
	attributor_ELS_classes_avail = 0;
	attributor_ELS_classes_used = 0;

	if (attributor_ELS_classes_memUsed != NULL){
		free(attributor_ELS_classes_memUsed);
		attributor_ELS_classes_memUsed = NULL;
	}
	if (attributor_ELS_classes_memAvail != NULL){
		free(attributor_ELS_classes_memAvail);
		attributor_ELS_classes_memAvail = NULL;
	}

	if (attributor_nrdfsStack != NULL){
		free(attributor_nrdfsStack);
		attributor_nrdfsStack = NULL;
	}
	attributor_nrdfsStack_leaf = 0;
	attributor_nrdfsStack_size = 0;

	#ifdef COLLECTSTATS
	stats_attributor_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}

void attributor_extern_computeAttributes(){
	//Computes all attributes of the formula that ATTRIBUTOR knows about.
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_attributor_component_totalCalls;
	#endif

	#ifdef VERBOSE_ATTRIBUTOR
	printf("c     ATTRIBUTOR: Computing attributes...\n");
	printf("c     ATTRIBUTOR: Domain is %d (", param_classifyInputDomain); classify_extern_printInputDomainName(param_classifyInputDomain); printf(")\n");
	#endif

	if (param_classifyInputDomain == CLASSIFY_INPUTDOMAIN_GENERIC){
		attributor_intern_computeGenericAttributes();
	} else if (param_classifyInputDomain == CLASSIFY_INPUTDOMAIN_RANDOM){
		attributor_intern_computeRandomAttributes();
	} else if (param_classifyInputDomain == CLASSIFY_INPUTDOMAIN_CRAFTED){
		attributor_intern_computeCraftedAttributes();
	} else if (param_classifyInputDomain == CLASSIFY_INPUTDOMAIN_APPLICATION){
		attributor_intern_computeApplicationAttributes();
	} else {
		printf("c ERROR. ATTRIBUTOR does not know domain %d. Attribute computation therefore failed.\n", param_classifyInputDomain);
		attributor_returnCode = ATTRIBUTOR_ERROR;
		return;
	}


	#ifdef VERBOSE_ATTRIBUTOR
	printf("c     ATTRIBUTOR: Done with computing attributes.\n");
	#endif

	#ifdef COLLECTSTATS
	stats_attributor_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}

void attributor_intern_computeGenericAttributes(){
	//BASIC ATTRIBUTES
	attributor_intern_compute_BASIC_allAttributes();
	if (attributor_returnCode != ATTRIBUTOR_UNKNOWN) return;

	//VARIABLE-CLAUSE GRAPH ATTRIBUTES
	attributor_intern_compute_VCG_allAttributes();
	if (attributor_returnCode != ATTRIBUTOR_UNKNOWN) return;

	//VARIABLE GRAPH
	attributor_intern_compute_VG_allAttributes();
	if (attributor_returnCode != ATTRIBUTOR_UNKNOWN) return;

	//CLAUSE GRAPH
	attributor_intern_compute_CG_allAttributes();
	if (attributor_returnCode != ATTRIBUTOR_UNKNOWN) return;

	//BALANCE
	attributor_intern_compute_B_allAttributes();
	if (attributor_returnCode != ATTRIBUTOR_UNKNOWN) return;

	//HORN PROXIMITY
	attributor_intern_compute_HP_allAttributes();
	if (attributor_returnCode != ATTRIBUTOR_UNKNOWN) return;

	//BINARY IMPLICATION GRAPH
	attributor_intern_compute_BIG_allAttributes();
	if (attributor_returnCode != ATTRIBUTOR_UNKNOWN) return;

	//UP
	attributor_intern_compute_UP_allAttributes();
	if (attributor_returnCode != ATTRIBUTOR_UNKNOWN) return;

	//RW
	attributor_intern_compute_RW_allAttributes();
	if (attributor_returnCode != ATTRIBUTOR_UNKNOWN) return;

	//ELS
	attributor_intern_compute_ELS_allAttributes();
	if (attributor_returnCode != ATTRIBUTOR_UNKNOWN) return;

	MAIN_SET_FATT_ISSET(1);
}

void attributor_intern_computeRandomAttributes(){
	//BASIC ATTRIBUTES
	attributor_intern_compute_BASIC_allAttributes();
	if (attributor_returnCode != ATTRIBUTOR_UNKNOWN) return;

	//VARIABLE-CLAUSE GRAPH ATTRIBUTES
	attributor_intern_compute_VCG_allAttributes();
	if (attributor_returnCode != ATTRIBUTOR_UNKNOWN) return;

	MAIN_SET_FATT_ISSET(1);
}

void attributor_intern_computeCraftedAttributes(){
	attributor_intern_computeGenericAttributes();
}

void attributor_intern_computeApplicationAttributes(){
	attributor_intern_computeGenericAttributes();
}

void attributor_intern_prioQa_sinkRoot(){
	//To sink down the root element as far as possible.
	if (attributor_prioQa_leaf == 1) return;

	float_ty leftChild, rightChild, rootElt = attributor_prioQa[1];
	uint32_t leftChildPos, rightChildPos, rootPos = 1;

	while(1){
		leftChildPos = rootPos << 1;
		rightChildPos = (rootPos << 1)+1;

		//No left child means no right child either. Element already at bottom of the heap. We cannot push further down.
		if (leftChildPos >= attributor_prioQa_leaf) break;

		//No right child means we have to check for a down move with the lift child only.
		if (rightChildPos >= attributor_prioQa_leaf) {
			leftChild = attributor_prioQa[leftChildPos];

			//We are done if the left child is smaller or equal than the element we rotate.
			if (leftChild <= rootElt) break;

			//We are not done yet. We first lift the child and then push down the root element.
			attributor_prioQa[rootPos] = leftChild;
			attributor_prioQa[leftChildPos] = rootElt;
			rootPos = leftChildPos;
		} else {
			//We have both a left and a right child and must first determine which one is larger.
			leftChild = attributor_prioQa[leftChildPos];
			rightChild = attributor_prioQa[rightChildPos];

			if (leftChild < rightChild){
				//We compare against the right child. We override the left child with it.
				leftChild = rightChild;
				leftChildPos = rightChildPos;
			}//Otherwise we leave it as it is and compare against the original left child. Do not get confused here, below,
			//leftChild could be the right child. We just use the name here to spare an additional else statement.

			//We are done if the left child is not larger than the root element.
			if (leftChild <= rootElt) break;

			//We are not done yet. We first lift the child and then push down the variable.
			attributor_prioQa[rootPos] = leftChild;
			attributor_prioQa[leftChildPos] = rootElt;
			rootPos = leftChildPos;
		}
	}
}

void attributor_intern_prioQa_raiseLeaf(){
	//To raise the leaf as far as possible.

	if (attributor_prioQa_leaf < 3) return;

	//We first get the current position of the leaf element and the element itself.
	float_ty leaf = attributor_prioQa[attributor_prioQa_leaf-1], parent;
	uint32_t leafPos = attributor_prioQa_leaf-1, parentPos;

	while(1){
		//We can stop right away if the leaf is at the top position.
		if (leafPos == 1) break;

		//We move the element upwards by exchanging it with its parent as long as the parent is smaller.
		parentPos = leafPos >> 1;
		parent = attributor_prioQa[parentPos];

		//If the parent is larger or equal to the leaf element we can stop.
		if (parent >= leaf) break;

		//Otherwise, we must exchange the element and its parent.
		attributor_prioQa[leafPos] = parent;
		attributor_prioQa[parentPos] = leaf;
		leafPos = parentPos;
	}
}

void attributor_intern_prioQa_add(float_ty elt){
	//Add an element and raise it as far as possible.
	attributor_prioQa[attributor_prioQa_leaf++] = elt;
	attributor_intern_prioQa_raiseLeaf();
}

void attributor_intern_prioQa_flush(){
	//To flush the priority queue.
	attributor_prioQa_leaf = 1;
}

void attributor_intern_prioQa_resize(uint32_t size){
	//To re-size the priority to the given value. The leaf will be reset.
	attributor_prioQa = realloc(attributor_prioQa, sizeof(float_ty)*(size+2));
	attributor_prioQa_leaf = 1;
}

float_ty attributor_intern_prioQa_dequeueBest(){
	//To get the element with most priority (largest value).
	if (attributor_prioQa_leaf == 1) return ZERO;

	//If there is only one element left we pick it but do not shuffle around a replacement.
	if (attributor_prioQa_leaf == 2){
		--attributor_prioQa_leaf;
		return attributor_prioQa[1];
	}

	//If there is more than one element left we pick the topmost element.
	float_ty best = attributor_prioQa[1];

	//We override the first position with the last element in the queue and push it down afterwards.
	attributor_prioQa[1] = attributor_prioQa[--attributor_prioQa_leaf];
	attributor_intern_prioQa_sinkRoot();

	//The queue is again up to date and we return the best one we have dequeued.
	return best;
}

void attributor_intern_prioQb_sinkRoot(){
	//To sink down the root element as far as possible.
	if (attributor_prioQb_leaf == 1) return;

	float_ty leftChild, rightChild, rootElt = attributor_prioQb[1];
	uint32_t leftChildPos, rightChildPos, rootPos = 1;

	while(1){
		leftChildPos = rootPos << 1;
		rightChildPos = (rootPos << 1)+1;

		//No left child means no right child either. Element already at bottom of the heap. We cannot push further down.
		if (leftChildPos >= attributor_prioQb_leaf) break;

		//No right child means we have to check for a down move with the lift child only.
		if (rightChildPos >= attributor_prioQb_leaf) {
			leftChild = attributor_prioQb[leftChildPos];

			//We are done if the left child is smaller or equal than the element we rotate.
			if (leftChild <= rootElt) break;

			//We are not done yet. We first lift the child and then push down the root element.
			attributor_prioQb[rootPos] = leftChild;
			attributor_prioQb[leftChildPos] = rootElt;
			rootPos = leftChildPos;
		} else {
			//We have both a left and a right child and must first determine which one is larger.
			leftChild = attributor_prioQb[leftChildPos];
			rightChild = attributor_prioQb[rightChildPos];

			if (leftChild < rightChild){
				//We compare against the right child. We override the left child with it.
				leftChild = rightChild;
				leftChildPos = rightChildPos;
			}//Otherwise we leave it as it is and compare against the original left child. Do not get confused here, below,
			//leftChild could be the right child. We just use the name here to spare an additional else statement.

			//We are done if the left child is not larger than the root element.
			if (leftChild <= rootElt) break;

			//We are not done yet. We first lift the child and then push down the variable.
			attributor_prioQb[rootPos] = leftChild;
			attributor_prioQb[leftChildPos] = rootElt;
			rootPos = leftChildPos;
		}
	}
}

void attributor_intern_prioQb_raiseLeaf(){
	//To raise the leaf as far as possible.

	if (attributor_prioQb_leaf < 3) return;

	//We first get the current position of the leaf element and the element itself.
	float_ty leaf = attributor_prioQb[attributor_prioQb_leaf-1], parent;
	uint32_t leafPos = attributor_prioQb_leaf-1, parentPos;

	while(1){
		//We can stop right away if the leaf is at the top position.
		if (leafPos == 1) break;

		//We move the element upwards by exchanging it with its parent as long as the parent is smaller.
		parentPos = leafPos >> 1;
		parent = attributor_prioQb[parentPos];

		//If the parent is larger or equal to the leaf element we can stop.
		if (parent >= leaf) break;

		//Otherwise, we must exchange the element and its parent.
		attributor_prioQb[leafPos] = parent;
		attributor_prioQb[parentPos] = leaf;
		leafPos = parentPos;
	}
}

void attributor_intern_prioQb_add(float_ty elt){
	//Add an element and raise it as far as possible.
	attributor_prioQb[attributor_prioQb_leaf++] = elt;
	attributor_intern_prioQb_raiseLeaf();
}

void attributor_intern_prioQb_flush(){
	//To flush the priority queue.
	attributor_prioQb_leaf = 1;
}

void attributor_intern_prioQb_resize(uint32_t size){
	//To re-size the priority to the given value. The leaf will be reset.
	attributor_prioQb = realloc(attributor_prioQb, sizeof(float_ty)*(size+2));
	attributor_prioQb_leaf = 1;
}

float_ty attributor_intern_prioQb_dequeueBest(){
	//To get the element with most priority (largest value).
	if (attributor_prioQb_leaf == 1) return ZERO;

	//If there is only one element left we pick it but do not shuffle around a replacement.
	if (attributor_prioQb_leaf == 2){
		--attributor_prioQb_leaf;
		return attributor_prioQb[1];
	}

	//If there is more than one element left we pick the topmost element.
	float_ty best = attributor_prioQb[1];

	//We override the first position with the last element in the queue and push it down afterwards.
	attributor_prioQb[1] = attributor_prioQb[--attributor_prioQb_leaf];
	attributor_intern_prioQb_sinkRoot();

	//The queue is again up to date and we return the best one we have dequeued.
	return best;
}

void attributor_intern_prioQa_computeDescription(float_ty mean, float_ty* median, float_ty* stddev,
		float_ty* sent, float_ty* rent, float_ty* ment,
		float_ty* pgfa, float_ty* pgfb, float_ty* pgfc){
	//This computes for the current priority queue (with the given mean value) the median, standard deviation, and entropy
	//(Shannon, Renyi, and Minimum). Furthermore, it computes the probability generating function values A, B, and C.
	//It ERASES the priority queue doing that.
	float_ty countedElt = ZERO, oldElt = ZERO, elt, numElt = attributor_prioQa_leaf - 1;
	uint32_t pos = 0, medianPos = 0;

	//We initialize the median, standard deviation, and the entropy.
	*median = ZERO;
	*stddev = ZERO;
	*sent = ZERO;
	*rent = ZERO;
	*ment = ZERO;
	*pgfa = ZERO;
	*pgfb = ZERO;
	*pgfc = ZERO;

	//In case the queue is empty, we can return right away. Otherwise we initialize the median position.
	if (numElt == 0){
		return;
	} else if (numElt == 1) {
		medianPos = 1;
	} else {
		medianPos = attributor_prioQa_leaf >> 1;
	}

	//In the beginning, the only element we have is the first element.
	oldElt = attributor_prioQa[1];

	while (attributor_prioQa_leaf > 1){
		++pos;
		elt = attributor_intern_prioQa_dequeueBest();
		//Check if the element is the median.
		if (pos == medianPos) {
			//It is, update the median.
			*median = elt;
		}

		//Update the standard deviation information sum.
		*stddev += pow(elt - mean, TWO);

		//Update the entropy information. We check if we are working on the last element already.
		if (attributor_prioQa_leaf == 1){
			//Yes, it is the last element. We check if the old element and the current one match.
			if (oldElt == elt){
				//Yes, they match, we count the current element in the old sequence.
				++countedElt;
				//We update the entropy information based on the last count, and compute the PGF values.
				countedElt = countedElt / numElt;
				if (countedElt > *ment) *ment = countedElt;
				if (countedElt > ZERO){
					*sent += countedElt * log2(countedElt);
					*rent += countedElt*countedElt;
					*pgfa += countedElt * pow(param_classifyPGFA,oldElt);
					*pgfb += countedElt * pow(param_classifyPGFB,oldElt);
					*pgfc += countedElt * pow(param_classifyPGFC,oldElt);
				}
			} else {
				//No, the element opens up a new sequence and therefore stands alone.
				//We update the entropy information based on the last count, and compute the PGF values.
				countedElt = countedElt / numElt;
				//And we add the entropy information for the last element, and compute its PGF value.
				if (countedElt > *ment) *ment = countedElt;
				if (countedElt > ZERO){
					*sent += (countedElt) * log2(countedElt);
					*rent += countedElt*countedElt;
					*pgfa += countedElt * pow(param_classifyPGFA,oldElt);
					*pgfb += countedElt * pow(param_classifyPGFB,oldElt);
					*pgfc += countedElt * pow(param_classifyPGFC,oldElt);
					countedElt = ONE / numElt;
					if (countedElt > ZERO){
						*sent += countedElt * log2(countedElt);
						*rent += countedElt*countedElt;
						*pgfa += countedElt * pow(param_classifyPGFA,oldElt);
						*pgfb += countedElt * pow(param_classifyPGFB,oldElt);
						*pgfc += countedElt * pow(param_classifyPGFC,oldElt);
					}
				}
			}
		} else {
			//No, it is not the last one. We check if the old element and the current one match.
			if (oldElt == elt){
				//Yes, they match. We just increase the count.
				++countedElt;
			} else {
				//No, they do not match. We must now update the entropy information and PGF values for the sequence and
				//open up a new one.
				countedElt = countedElt / numElt;
				if (countedElt > *ment) *ment = countedElt;
				if (countedElt > ZERO){
					*sent += countedElt * log2(countedElt);
					*rent += countedElt*countedElt;
					*pgfa += countedElt * pow(param_classifyPGFA,oldElt);
					*pgfb += countedElt * pow(param_classifyPGFB,oldElt);
					*pgfc += countedElt * pow(param_classifyPGFC,oldElt);
				}
				//Start a new sequence with the just read element.
				oldElt = elt;
				countedElt = ONE;
			}
		}
	}

	//Finalize the standard deviation information.
	*stddev = sqrt(*stddev / numElt);

	//Finalize the entropy information.
	//Shannon.
	*sent = fabs(*sent);
	//Renyi.
	if (*rent > ZERO){
		*rent = fabs(log2(*rent));
	}
	//Minimum.
	if (*ment > ZERO){
		*ment = fabs(log2(*ment));
	}
}

void attributor_intern_prioQa_computeQuantiles(float_ty total, float_ty* q05, float_ty* q10, float_ty* q25,
		float_ty* q50, float_ty* q75, float_ty* q90, float_ty* q95){
	//Computes the quantiles for 5, 10, 25, 50, 75, 90, and 95 percent for the first queue. The very first value is the total
	//mass collected in the queue. We return right away if the total mass in the first priority queue is empty. This operation
	//ERASES the priority queue doing that.

	float_ty currentMass = ZERO, currentTarget = 0.05*total, numElt = attributor_prioQa_leaf-1, eltNum = ZERO, eps = 0.001*total;

	if (eps == ZERO || total == ZERO || numElt == ZERO){
		*q05 = ZERO;
		*q10 = ZERO;
		*q25 = ZERO;
		*q50 = ZERO;
		*q75 = ZERO;
		*q90 = ZERO;
		*q95 = ZERO;
		//Just to be sure, we flush the remaining queue.
		attributor_intern_prioQa_flush();
		return;
	}

	//Collect the quantile information.
	while (attributor_prioQa_leaf > 1 && currentTarget < total){
		//Grab the next value and add it to the current mass.
		currentMass += attributor_intern_prioQa_dequeueBest();
		++eltNum;
		//Check if the collected mass now exceeds the current Target.
		while (currentMass >= currentTarget){
			//Yes it does. It is time to update the quantile value.
			if (currentTarget < 0.05*total + eps){
				*q05 = eltNum / numElt;
				currentTarget = 0.10*total;
			} else if (0.10*total - eps < currentTarget && currentTarget < 0.10*total + eps){
				*q10 = eltNum / numElt;
				currentTarget = 0.25*total;
			} else if (0.25*total - eps < currentTarget && currentTarget < 0.25*total + eps){
				*q25 = eltNum / numElt;
				currentTarget = 0.50*total;
			} else if (0.50*total - eps < currentTarget && currentTarget < 0.50*total + eps){
				*q50 = eltNum / numElt;
				currentTarget = 0.75*total;
			} else if (0.75*total - eps < currentTarget && currentTarget < 0.75*total + eps){
				*q75 = eltNum / numElt;
				currentTarget = 0.90*total;
			} else if (0.90*total - eps < currentTarget && currentTarget < 0.90*total + eps){
				*q90 = eltNum / numElt;
				currentTarget = 0.95*total;
			} else if (0.95*total - eps < currentTarget){
				*q95 = eltNum / numElt;
				currentTarget = total;
				break;
			}
		}
	}

	//Just to be sure, we flush the remaining queue.
	attributor_intern_prioQa_flush();
}

void attributor_intern_prioQb_computeDescription(float_ty mean, float_ty* median, float_ty* stddev,
		float_ty* sent, float_ty* rent, float_ty* ment,
		float_ty* pgfa, float_ty* pgfb, float_ty* pgfc){
	//This computes for the current priority queue (with the given mean value) the median, standard deviation, and entropy
	//(Shannon, Renyi, and Minimum). Furthermore, it computes the probability generating function values A, B, and C.
	//It ERASES the priority queue doing that.
	float_ty countedElt = ZERO, oldElt = ZERO, elt, numElt = attributor_prioQb_leaf - 1;
	uint32_t pos = 0, medianPos = 0;

	//We initialize the median, standard deviation, and the entropy.
	*median = ZERO;
	*stddev = ZERO;
	*sent = ZERO;
	*rent = ZERO;
	*ment = ZERO;
	*pgfa = ZERO;
	*pgfb = ZERO;
	*pgfc = ZERO;

	//In case the queue is empty, we can return right away. Otherwise we initialize the median position.
	if (numElt == 0){
		return;
	} else if (numElt == 1) {
		medianPos = 1;
	} else {
		medianPos = attributor_prioQb_leaf >> 1;
	}

	//In the beginning, the only element we have is the first element.
	oldElt = attributor_prioQb[1];

	while (attributor_prioQb_leaf > 1){
		++pos;
		elt = attributor_intern_prioQb_dequeueBest();
		//Check if the element is the median.
		if (pos == medianPos) {
			//It is, update the median.
			*median = elt;
		}

		//Update the standard deviation information sum.
		*stddev += pow(elt - mean, TWO);

		//Update the entropy information. We check if we are working on the last element already.
		if (attributor_prioQb_leaf == 1){
			//Yes, it is the last element. We check if the old element and the current one match.
			if (oldElt == elt){
				//Yes, they match, we count the current element in the old sequence.
				++countedElt;
				//We update the entropy information based on the last count, and compute the PGF values.
				countedElt = countedElt / numElt;
				if (countedElt > *ment) *ment = countedElt;
				if (countedElt > ZERO){
					*sent += countedElt * log2(countedElt);
					*rent += countedElt*countedElt;
					*pgfa += countedElt * pow(param_classifyPGFA,oldElt);
					*pgfb += countedElt * pow(param_classifyPGFB,oldElt);
					*pgfc += countedElt * pow(param_classifyPGFC,oldElt);
				}
			} else {
				//No, the element opens up a new sequence and therefore stands alone.
				//We update the entropy information based on the last count, and compute the PGF values.
				countedElt = countedElt / numElt;
				//And we add the entropy information for the last element, and compute its PGF value.
				if (countedElt > *ment) *ment = countedElt;
				if (countedElt > ZERO){
					*sent += (countedElt) * log2(countedElt);
					*rent += countedElt*countedElt;
					*pgfa += countedElt * pow(param_classifyPGFA,oldElt);
					*pgfb += countedElt * pow(param_classifyPGFB,oldElt);
					*pgfc += countedElt * pow(param_classifyPGFC,oldElt);
					countedElt = ONE / numElt;
					if (countedElt > ZERO){
						*sent += countedElt * log2(countedElt);
						*rent += countedElt*countedElt;
						*pgfa += countedElt * pow(param_classifyPGFA,oldElt);
						*pgfb += countedElt * pow(param_classifyPGFB,oldElt);
						*pgfc += countedElt * pow(param_classifyPGFC,oldElt);
					}
				}
			}
		} else {
			//No, it is not the last one. We check if the old element and the current one match.
			if (oldElt == elt){
				//Yes, they match. We just increase the count.
				++countedElt;
			} else {
				//No, they do not match. We must now update the entropy information and PGF values for the sequence and
				//open up a new one.
				countedElt = countedElt / numElt;
				if (countedElt > *ment) *ment = countedElt;
				if (countedElt > ZERO){
					*sent += countedElt * log2(countedElt);
					*rent += countedElt*countedElt;
					*pgfa += countedElt * pow(param_classifyPGFA,oldElt);
					*pgfb += countedElt * pow(param_classifyPGFB,oldElt);
					*pgfc += countedElt * pow(param_classifyPGFC,oldElt);
				}
				//Start a new sequence with the just read element.
				oldElt = elt;
				countedElt = ONE;
			}
		}
	}

	//Finalize the standard deviation information.
	*stddev = sqrt(*stddev / numElt);

	//Finalize the entropy information.
	//Shannon.
	*sent = fabs(*sent);
	//Renyi.
	if (*rent > ZERO){
		*rent = fabs(log2(*rent));
	}
	//Minimum.
	if (*ment > ZERO){
		*ment = fabs(log2(*ment));
	}
}

void attributor_intern_prioQb_computeQuantiles(float_ty total, float_ty* q05, float_ty* q10, float_ty* q25,
		float_ty* q50, float_ty* q75, float_ty* q90, float_ty* q95){
	//Computes the quantiles for 5, 10, 25, 50, 75, 90, and 95 percent for the second queue. The very first value is the total
	//mass collected in the queue. We return right away if the total mass in the second priority queue is empty. This operation
	//ERASES the priority queue doing that.

	float_ty currentMass = ZERO, currentTarget = 0.05*total, numElt = attributor_prioQb_leaf-1, eltNum = ZERO, eps = 0.001*total;

	if (eps == ZERO || total == ZERO || numElt == ZERO){
		*q05 = ZERO;
		*q10 = ZERO;
		*q25 = ZERO;
		*q50 = ZERO;
		*q75 = ZERO;
		*q90 = ZERO;
		*q95 = ZERO;
		//Just to be sure, we flush the remaining queue.
		attributor_intern_prioQb_flush();
		return;
	}

	//Collect the quantile information.
	while (attributor_prioQb_leaf > 1 && currentTarget < total){
		//Grab the next value and add it to the current mass.
		currentMass += attributor_intern_prioQb_dequeueBest();
		++eltNum;
		//Check if the collected mass now exceeds the current Target.
		while (currentMass >= currentTarget){
			//Yes it does. It is time to update the quantile value.
			if (currentTarget < 0.05*total + eps){
				*q05 = eltNum / numElt;
				currentTarget = 0.10*total;
			} else if (0.10*total - eps < currentTarget && currentTarget < 0.10*total + eps){
				*q10 = eltNum / numElt;
				currentTarget = 0.25*total;
			} else if (0.25*total - eps < currentTarget && currentTarget < 0.25*total + eps){
				*q25 = eltNum / numElt;
				currentTarget = 0.50*total;
			} else if (0.50*total - eps < currentTarget && currentTarget < 0.50*total + eps){
				*q50 = eltNum / numElt;
				currentTarget = 0.75*total;
			} else if (0.75*total - eps < currentTarget && currentTarget < 0.75*total + eps){
				*q75 = eltNum / numElt;
				currentTarget = 0.90*total;
			} else if (0.90*total - eps < currentTarget && currentTarget < 0.90*total + eps){
				*q90 = eltNum / numElt;
				currentTarget = 0.95*total;
			} else if (0.95*total - eps < currentTarget){
				*q95 = eltNum / numElt;
				currentTarget = total;
				break;
			}
		}
	}

	//Just to be sure, we flush the remaining queue.
	attributor_intern_prioQb_flush();
}

void attributor_intern_nrdfsStack_push(nrdfsStackNode* aStackNode){
	//To push a new element on the stack. May increase the stack-size.
	attributor_nrdfsStack[attributor_nrdfsStack_leaf++] = aStackNode;
	//We might have to increase the stack size.
	if (attributor_nrdfsStack_leaf >= attributor_nrdfsStack_size){
		attributor_nrdfsStack_size += 1024*BLOCKSIZE;
		nrdfsStackNode** temp = realloc(attributor_nrdfsStack, sizeof(nrdfsStackNode*)*attributor_nrdfsStack_size);
		if (temp == NULL){
			printf("c ERROR. Extending the attributor_nrdfsStack failed in ATTRIBUTOR. Out of memory?");fflush(stdout);
			attributor_returnCode = ATTRIBUTOR_ERROR;
			exit(ERROR);
		} else {
			attributor_nrdfsStack = temp;
		}
	}
}

nrdfsStackNode* attributor_intern_nrdfsStack_pop(){
	//To read and remove the topmost element from the stack. It provides the three integers node, depth, parent.
	//A single element on this stack has the form [node, depth, parent].
	if (attributor_nrdfsStack_leaf == 0){
		return NULL;
	} else {
		--attributor_nrdfsStack_leaf;
		return attributor_nrdfsStack[attributor_nrdfsStack_leaf];
	}
}

nrdfsStackNode* attributor_intern_nrdfsStack_read(){
	//To just read the topmost element from the stack. It provides the three integers node, depth, parent. A single element
	//on this stack has the form [node, depth, parent].
	if (attributor_nrdfsStack_leaf == 0){
		return NULL;
	} else {
		return attributor_nrdfsStack[attributor_nrdfsStack_leaf-1];
	}
}

void attributor_intern_nrdfsStack_remove(){
	//To just remove the top element from the stack.
	if (attributor_nrdfsStack_leaf > 0) --attributor_nrdfsStack_leaf;
}

void attributor_intern_nrdfsStack_resize(uint32_t size){
	//To resize the stack to the given size (will be increased by 1).
	attributor_nrdfsStack_size = size+1;
	nrdfsStackNode** temp = realloc(attributor_nrdfsStack, sizeof(nrdfsStackNode*)*attributor_nrdfsStack_size);
	if (temp == NULL){
		printf("c ERROR. Resizing the attributor_nrdfsStack failed in ATTRIBUTOR. Out of memory?");fflush(stdout);
		attributor_returnCode = ATTRIBUTOR_ERROR;
		exit(ERROR);
	} else {
		attributor_nrdfsStack = temp;
		attributor_nrdfsStack_leaf = 0;
	}
}

void attributor_intern_nrdfsStack_flush(){
	//To flush all elements from the stack (just resets the leaf to 0).
	attributor_nrdfsStack_leaf = 0;
}

void attributor_intern_compute_BASIC_allAttributes(){
	//Computes the basic attributes of the formula.
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	#endif

	ATTRIBUTOR_SET_START_TIME_IN_SEC();

	float_ty initDis = ZERO, initAss = ZERO, initPurePos = ZERO, initPureNeg = ZERO, r = ZERO;
	int32_t i;

	MAIN_SET_FATT_N(f.n_initial);
	MAIN_SET_FATT_M(f.m_initial);

	r = ((float_ty)f.m_initial) / ((float_ty)f.n_initial);
	MAIN_SET_FATT_R(r);


	for (i = 1; i < f.n_initial+1; ++i){
		//Check if the variable is still unassigned.
		if (!IS_VAR_UNASSIGNED((main_varData + i))){
			//The variable is assigned.
			++initAss;
		} else {
			//The variable is unassigned. Check if the variable is disabled.
			if (IS_VAR_DISABLED((main_varData + i))){
				//The variable is disabled.
				++initDis;
			} else {
				//The variable is enabled. Check if the variable is pure in exactly one direction.
				if (GET_LIT_NUMOCCS_USED((main_litData + i)) > 0 && GET_LIT_NUMOCCS_USED((main_litData - i)) == 0){
					//It is pure in the positive direction.
					++initPurePos;
					classify_intern_simplify_addLitToEnforce(i);
				} else if (GET_LIT_NUMOCCS_USED((main_litData - i)) > 0 && GET_LIT_NUMOCCS_USED((main_litData + i)) == 0){
					//It is pure in the positive direction.
					++initPureNeg;
					classify_intern_simplify_addLitToEnforce(-i);
				}
			}
		}
		if (ATTRIBUTOR_IS_TIME_ELAPSED()){
			#ifdef VERBOSE_ATTRIBUTOR
			printf("c     ATTRIBUTOR:   TIMEOUT BASIC.\n");
			#endif
			#ifdef COLLECTSTATS
			++stats_attributor_numTimeouts;
			#endif
			break;
		}
	}

	MAIN_SET_FATT_INITASS(initAss);
	MAIN_SET_FATT_INITDIS(initDis);
	MAIN_SET_FATT_INITPUREPOS(initPurePos);
	MAIN_SET_FATT_INITPURENEG(initPureNeg);

	#ifdef VERBOSE_ATTRIBUTOR
	printf("c     ATTRIBUTOR:   N[%f] M[%f] R[%f] INITASS[%f] INITDIS[%f] INITPUREPOS[%f] INITPURENEG[%f]\n", MAIN_GET_FATT_N(),
			MAIN_GET_FATT_M(), MAIN_GET_FATT_R(), MAIN_GET_FATT_INITASS(), MAIN_GET_FATT_INITDIS(),
			MAIN_GET_FATT_INITPUREPOS(), MAIN_GET_FATT_INITPURENEG());
	fflush(stdout);
	#endif
	#ifdef COLLECTSTATS
	stats_attributor_time_BASIC += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}

void attributor_intern_compute_VCG_allAttributes(){
	//Computes the variable-clause graph variable and clause node degrees, their minimum, maximum, mean, median, standard
	//deviation and entropy. Furthermore, it checks if the formula may be a random one.

	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	#endif

	ATTRIBUTOR_SET_START_TIME_IN_SEC();

	variable *v;
	clause *c;
	literal *lPos, *lNeg;
	float_ty numElt = ZERO;
	float_ty vDegree, vDegreeMin = UINT32_MAX, vDegreeMax = ZERO, vDegreeMean = ZERO,
	         vDegreeMedian = ZERO, vDegreeStdDev = ZERO, vDegreeSent = ZERO, vDegreeRent = ZERO, vDegreeMent = ZERO,
	         vDegreePGFA = ZERO, vDegreePGFB = ZERO, vDegreePGFC = ZERO;
	float_ty cDegree, cDegreeMin = UINT32_MAX, cDegreeMax = ZERO, cDegreeMean = ZERO,
	         cDegreeMedian = ZERO, cDegreeStdDev = ZERO, cDegreeSent = ZERO, cDegreeRent = ZERO, cDegreeMent = ZERO,
	         cDegreePGFA = ZERO, cDegreePGFB = ZERO, cDegreePGFC = ZERO;
	float_ty trueRatio = ZERO;
	int32_t vNum, cNum;

	//First, we fill the priority queue containing all variable node degrees (highest count first). While we are at it, we
	//store the minimum, maximum, and the mean.
	attributor_intern_prioQa_resize(f.n_initial);
	for (vNum = 1; vNum < f.n_initial+1; ++vNum){
		if (ATTRIBUTOR_IS_TIME_ELAPSED()){
			#ifdef VERBOSE_ATTRIBUTOR
			printf("c     ATTRIBUTOR:   TIMEOUT VCG-V.\n");
			#endif
			#ifdef COLLECTSTATS
			++stats_attributor_numTimeouts;
			#endif
			break;
		}
		v = main_varData + vNum;
		//We ignore all variables that are not enabled anymore.
		if (IS_VAR_DISABLED(v)) continue;
		//We count the degree for the variable by summing up its positive and negative literal count.
		lPos = main_litData + vNum;
		lNeg = main_litData - vNum;
		vDegree = GET_LIT_NUMOCCS_USED(lPos) + GET_LIT_NUMOCCS_USED(lNeg);
		attributor_intern_prioQa_add(vDegree);
		++numElt;
		//We store the current minimum, maximum and mean value.
		vDegreeMean += vDegree;
		if (vDegree < vDegreeMin) vDegreeMin = vDegree;
		if (vDegree > vDegreeMax) vDegreeMax = vDegree;
	}

	//Then, we compute the median, standard deviation and entropy in case the queue is not empty.
	if (numElt > ZERO) {
		vDegreeMean = vDegreeMean / numElt;
		attributor_intern_prioQa_computeDescription(vDegreeMean,&vDegreeMedian,&vDegreeStdDev,
				&vDegreeSent,&vDegreeRent,&vDegreeMent,
				&vDegreePGFA,&vDegreePGFB,&vDegreePGFC);
		MAIN_SET_FATT_VCG_VNODEDEGREE_MIN(vDegreeMin);
		MAIN_SET_FATT_VCG_VNODEDEGREE_MAX(vDegreeMax);
		MAIN_SET_FATT_VCG_VNODEDEGREE_MEAN(vDegreeMean);
		MAIN_SET_FATT_VCG_VNODEDEGREE_MEDIAN(vDegreeMedian);
		MAIN_SET_FATT_VCG_VNODEDEGREE_STDDEV(vDegreeStdDev);
		MAIN_SET_FATT_VCG_VNODEDEGREE_SENT(vDegreeSent);
		MAIN_SET_FATT_VCG_VNODEDEGREE_RENT(vDegreeRent);
		MAIN_SET_FATT_VCG_VNODEDEGREE_MENT(vDegreeMent);
		MAIN_SET_FATT_VCG_VNODEDEGREE_PGFA(vDegreePGFA);
		MAIN_SET_FATT_VCG_VNODEDEGREE_PGFB(vDegreePGFB);
		MAIN_SET_FATT_VCG_VNODEDEGREE_PGFC(vDegreePGFC);
	}
	//Clean up.
	attributor_intern_prioQa_resize(BLOCKSIZE);

	//Second, we fill the priority queue containing all the clause node degrees (highest count first). While we are at it, we
	//store the minimum, maximum, and the mean.
	if (!ATTRIBUTOR_IS_TIME_ELAPSED()){
		attributor_intern_prioQa_resize(f.m_eo_used);
		for (cNum = 0; cNum < f.m_eo_used; ++cNum){
			if (ATTRIBUTOR_IS_TIME_ELAPSED()){
				#ifdef VERBOSE_ATTRIBUTOR
				printf("c     ATTRIBUTOR:   TIMEOUT VCG-C.\n");
				#endif
				#ifdef COLLECTSTATS
				++stats_attributor_numTimeouts;
				#endif
				break;
			}
			c = f.clauses_eo[cNum];
			//The clause degree is its size.
			cDegree = GET_CLS_SIZE(c);
			attributor_intern_prioQa_add(cDegree);
			++numElt;
			//We store the current minimum, maximum and mean value.
			cDegreeMean += cDegree;
			if (cDegree < cDegreeMin) cDegreeMin = cDegree;
			if (cDegree > cDegreeMax) cDegreeMax = cDegree;
		}

		//Then, we compute the median, standard deviation and entropy in case the queue is not empty.
		if (numElt > ZERO) {
			cDegreeMean = cDegreeMean / numElt;
			attributor_intern_prioQa_computeDescription(cDegreeMean,&cDegreeMedian,&cDegreeStdDev,
					&cDegreeSent,&cDegreeRent,&cDegreeMent,
					&cDegreePGFA,&cDegreePGFB,&cDegreePGFC);
			MAIN_SET_FATT_VCG_CNODEDEGREE_MIN(cDegreeMin);
			MAIN_SET_FATT_VCG_CNODEDEGREE_MAX(cDegreeMax);
			MAIN_SET_FATT_VCG_CNODEDEGREE_MEAN(cDegreeMean);
			MAIN_SET_FATT_VCG_CNODEDEGREE_MEDIAN(cDegreeMedian);
			MAIN_SET_FATT_VCG_CNODEDEGREE_STDDEV(cDegreeStdDev);
			MAIN_SET_FATT_VCG_CNODEDEGREE_SENT(cDegreeSent);
			MAIN_SET_FATT_VCG_CNODEDEGREE_RENT(cDegreeRent);
			MAIN_SET_FATT_VCG_CNODEDEGREE_MENT(cDegreeMent);
			MAIN_SET_FATT_VCG_CNODEDEGREE_PGFA(cDegreePGFA);
			MAIN_SET_FATT_VCG_CNODEDEGREE_PGFB(cDegreePGFB);
			MAIN_SET_FATT_VCG_CNODEDEGREE_PGFC(cDegreePGFC);
		}
		//Clean up.
		attributor_intern_prioQa_resize(BLOCKSIZE);
	}

	//Finally, we check if the formula may be random using a few simple checks.
	if (f.n_initial > 0){
		trueRatio = ((float_ty)f.m_initial) / ((float_ty)f.n_initial);
		if (MAIN_GET_FATT_VCG_CNODEDEGREE_MIN() == 3
				&& MAIN_GET_FATT_VCG_CNODEDEGREE_MAX() == 3
				&& trueRatio > 3.5 && trueRatio < 4.3){
			//May be a random 3-SAT formula.
			MAIN_SET_FATT_VCG_MAYBERAND(3.0);
		} else if (MAIN_GET_FATT_VCG_CNODEDEGREE_MIN() == 4
				&& MAIN_GET_FATT_VCG_CNODEDEGREE_MAX() == 4
				&& trueRatio > 6.5 && trueRatio < 10.0){
			//May be a random 4-SAT formula.
			MAIN_SET_FATT_VCG_MAYBERAND(4.0);
		} else if (MAIN_GET_FATT_VCG_CNODEDEGREE_MIN() == 5
				&& MAIN_GET_FATT_VCG_CNODEDEGREE_MAX() == 5
				&& trueRatio > 14.0 && trueRatio < 21.5){
			//May be a random 5-SAT formula.
			MAIN_SET_FATT_VCG_MAYBERAND(5.0);
		} else if (MAIN_GET_FATT_VCG_CNODEDEGREE_MIN() == 6
				&& MAIN_GET_FATT_VCG_CNODEDEGREE_MAX() == 6
				&& trueRatio > 29.0 && trueRatio < 44.0){
			//May be a random 6-SAT formula.
			MAIN_SET_FATT_VCG_MAYBERAND(6.0);
		} else if (MAIN_GET_FATT_VCG_CNODEDEGREE_MIN() == 7
				&& MAIN_GET_FATT_VCG_CNODEDEGREE_MAX() == 7
				&& trueRatio > 59.0 && trueRatio < 88.5){
			//May be a random 7-SAT formula.
			MAIN_SET_FATT_VCG_MAYBERAND(7.0);
		} else if (MAIN_GET_FATT_VCG_CNODEDEGREE_MIN() == 2
				&& MAIN_GET_FATT_VCG_CNODEDEGREE_MAX() == 3
				&& trueRatio > 2.69 && trueRatio < 4.0){
			//May be a random 2+p-SAT formula.
			MAIN_SET_FATT_VCG_MAYBERAND(2.5);
		} else if (MAIN_GET_FATT_VCG_CNODEDEGREE_MIN() == 3
				&& MAIN_GET_FATT_VCG_CNODEDEGREE_MAX() == 4
				&& trueRatio > 5.0 && trueRatio < 8.0){
			//May be a random 3+p-SAT formula.
			MAIN_SET_FATT_VCG_MAYBERAND(3.5);
		} else if (MAIN_GET_FATT_VCG_CNODEDEGREE_MIN() == 4
				&& MAIN_GET_FATT_VCG_CNODEDEGREE_MAX() == 5
				&& trueRatio > 12.0 && trueRatio < 19.0){
			//May be a random 4+p-SAT formula.
			MAIN_SET_FATT_VCG_MAYBERAND(4.5);
		} else if (MAIN_GET_FATT_VCG_CNODEDEGREE_MIN() == 5
				&& MAIN_GET_FATT_VCG_CNODEDEGREE_MAX() == 6
				&& trueRatio > 24.0 && trueRatio < 40.0){
			//May be a random 5+p-SAT formula.
			MAIN_SET_FATT_VCG_MAYBERAND(5.5);
		} else if (MAIN_GET_FATT_VCG_CNODEDEGREE_MIN() == 6
				&& MAIN_GET_FATT_VCG_CNODEDEGREE_MAX() == 7
				&& trueRatio > 50.0 && trueRatio < 80.0){
			//May be a random 6+p-SAT formula.
			MAIN_SET_FATT_VCG_MAYBERAND(6.5);
		}
	}

	#ifdef VERBOSE_ATTRIBUTOR
	printf("c     ATTRIBUTOR:   VCG_VNODEDEGREE[MIN %f MAX %f MEAN %f MEDIAN %f SENT %f RENT %f MENT %f STDDEV %f PGFA %f PGFB %f PGFC %f]\n",
			MAIN_GET_FATT_VCG_VNODEDEGREE_MIN(), MAIN_GET_FATT_VCG_VNODEDEGREE_MAX(), MAIN_GET_FATT_VCG_VNODEDEGREE_MEAN(),
			MAIN_GET_FATT_VCG_VNODEDEGREE_MEDIAN(),	MAIN_GET_FATT_VCG_VNODEDEGREE_SENT(), MAIN_GET_FATT_VCG_VNODEDEGREE_RENT(),
			MAIN_GET_FATT_VCG_VNODEDEGREE_MENT(), MAIN_GET_FATT_VCG_VNODEDEGREE_STDDEV(), MAIN_GET_FATT_VCG_VNODEDEGREE_PGFA(),
			MAIN_GET_FATT_VCG_VNODEDEGREE_PGFB(),MAIN_GET_FATT_VCG_VNODEDEGREE_PGFC());
	printf("c     ATTRIBUTOR:   VCG_CNODEDEGREE[MIN %f MAX %f MEAN %f MEDIAN %f SENT %f RENT %f MENT %f STDDEV %f PGFA %f PGFB %f PGFC %f]\n",
			MAIN_GET_FATT_VCG_CNODEDEGREE_MIN(), MAIN_GET_FATT_VCG_CNODEDEGREE_MAX(), MAIN_GET_FATT_VCG_CNODEDEGREE_MEAN(),
			MAIN_GET_FATT_VCG_CNODEDEGREE_MEDIAN(),	MAIN_GET_FATT_VCG_CNODEDEGREE_SENT(), MAIN_GET_FATT_VCG_CNODEDEGREE_RENT(),
			MAIN_GET_FATT_VCG_CNODEDEGREE_MENT(), MAIN_GET_FATT_VCG_CNODEDEGREE_STDDEV(), MAIN_GET_FATT_VCG_CNODEDEGREE_PGFA(),
			MAIN_GET_FATT_VCG_CNODEDEGREE_PGFB(),MAIN_GET_FATT_VCG_CNODEDEGREE_PGFC());
	printf("c     ATTRIBUTOR:   VCG_MAYBERAND[%f]\n", MAIN_GET_FATT_VCG_MAYBERAND());
	fflush(stdout);
	#endif
	#ifdef COLLECTSTATS
	stats_attributor_time_VCG += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}

void attributor_intern_compute_VG_components(int32_t numElt){
	//We determine the number of components and the largest component diameter using BFS.
	if (numElt == 0){
		return;
	}

	variable *v;
	literal *l, *adjLit;
	int32_t i;
	uint32_t distance, lSpos, id, node;
	float_ty qNumElt = ZERO, vgNumComp,
			vgCompDiamMin = UINT32_MAX, vgCompDiamMax = ZERO, vgCompDiamMean = ZERO,
			vgCompDiamMed = ZERO, vgCompDiamStdDev = ZERO, vgCompDiamSent = ZERO, vgCompDiamRent = ZERO, vgCompDiamMent = ZERO,
			vgCompDiamPGFA = ZERO, vgCompDiamPGFB = ZERO, vgCompDiamPGFC = ZERO,
			vgCompSizeMin = UINT32_MAX, vgCompSizeMax = ZERO, vgCompSizeMean = ZERO,
			vgCompSizeMed = ZERO, vgCompSizeStdDev = ZERO, vgCompSizeSent = ZERO, vgCompSizeRent = ZERO, vgCompSizeMent = ZERO,
			vgCompSizePGFA = ZERO, vgCompSizePGFB = ZERO, vgCompSizePGFC = ZERO;

	//Initialize stacks and return right away if nothing is to be computed. The priority queues will have size numElt/2,
	//because that is the maximum number of components that our formula can have. A variable that appears in the formula
	//cannot be a component by itself (would be a single unit clause, but they have been removed already).
	vS_flush();
	lS_flush();
	attributor_intern_prioQa_resize(numElt/2);
	attributor_intern_prioQb_resize(numElt/2);

	vgNumComp = ZERO;

	//We grab all the nodes in the graph that have a non-empty adjacency list.
	for (i = 0; i < ATTRIBUTOR_GRAPH_GET_NUMLISTS_USED(); ++i){
		if (ATTRIBUTOR_GRAPH_GET_LIST_NUM_NUMUSED(i) > 0) {
			v = main_varData + i;
			l = main_litData + i;
			SET_LIT_S_POS(l, 0);
			SET_LIT_DSC(l, UINT32_MAX);
			SET_VAR_S_POS(v, 0);
			vS_push(v);
		}
	}

	//We start by constructing a component. The component will be stored in the literal stack.
	while (vS_leaf > 1){
		if (ATTRIBUTOR_IS_TIME_ELAPSED()){
			#ifdef VERBOSE_ATTRIBUTOR
			printf("c     ATTRIBUTOR:   TIMEOUT VG-COMP.\n");
			#endif
			#ifdef COLLECTSTATS
			++stats_attributor_numTimeouts;
			#endif
			break;
		}
		//We start constructing the component by adding the root element (popping the last element from the variable stack)
		//to the literal stack. The literal stack will be the component in the end.
		v = vS_pop();
		vgNumComp = vgNumComp + ONE;
		distance = 0;
		l = main_litData + GET_VAR_ID(v);
		SET_LIT_DSC(l, distance);
		lS_flush();
		lS_push(l);
		lSpos = 1;

		//We will now move through the literal stack starting at position one and add all elements in the adjacency list for
		//this literal -- if the literal is not already in the literal stack.
		while (lSpos < lS_leaf){
			//Get the id of the element for this distance.
			l = lS_readAt(lSpos);
			++lSpos;
			id = GET_LIT_NUM_FROM_PTR(l);
			//Check if this literal belongs to a new, farther distance.
			if (GET_LIT_DSC(l) > distance){
				//That is the case, we now work on a block of nodes that is farther away. We increase the distance by one.
				++distance;
			}
			for (i = 0; i < ATTRIBUTOR_GRAPH_GET_LIST_NUM_NUMUSED(id); ++i){
				//Get the adjacent variable.
				node = ATTRIBUTOR_GRAPH_GET_LIST_NUM_ELTNUM(id, i);
				adjLit = main_litData + node;
				//Check if the adjacent variable has been found already.
				if (GET_LIT_S_POS(adjLit) == 0){
					//No, it is not in the stack, therefore we must add it now (with increased distance).
					SET_LIT_DSC(adjLit, distance+1);
					lS_push(adjLit);
					//The corresponding variable is part of the current component and can be removed from the variable stack.
					vS_delete(main_varData + node);
				}
			}
		}

		//We have now in the literal stack all the positive literals of the variables that belong to the current component.
		//The last element in this stack has the farthest distance to the initial root element. We will now pick this element
		//as the new root and restart from scratch.
		l = lS_pop();
		lS_flush();
		distance = 0;
		SET_LIT_DSC(l, distance);
		lS_push(l);
		lSpos = 1;

		while (lSpos < lS_leaf){
			//Get the id of the element for this distance.
			l = lS_readAt(lSpos);
			++lSpos;
			id = GET_LIT_NUM_FROM_PTR(l);
			//Check if this literal belongs to a new, farther distance.
			if (GET_LIT_DSC(l) > distance){
				//That is the case, we now work on a block of nodes that is farther away. We increase the distance by one.
				++distance;
			}
			for (i = 0; i < ATTRIBUTOR_GRAPH_GET_LIST_NUM_NUMUSED(id); ++i){
				//Get the adjacent variable.
				node = ATTRIBUTOR_GRAPH_GET_LIST_NUM_ELTNUM(id, i);
				adjLit = main_litData + node;
				//Check if the adjacent variable has been found already.
				if (GET_LIT_S_POS(adjLit) == 0){
					//No, it is not in the stack, therefore we must add it now (with increased distance).
					SET_LIT_DSC(adjLit, distance+1);
					lS_push(adjLit);
				}
			}
		}

		//We have now in the literal stack the component, the last element in it has the farthest distance from the second
		//root try. This gives us the diameter of the component. The component size is lSpos - 1.
		//Update the component size information.
		--lSpos;
		attributor_intern_prioQa_add(lSpos);
		if (lSpos < vgCompSizeMin) vgCompSizeMin = lSpos;
		if (lSpos > vgCompSizeMax) vgCompSizeMax = lSpos;
		vgCompSizeMean += lSpos;
		//Update the component diameter information.
		l = lS_pop();
		distance = GET_LIT_DSC(l);
		attributor_intern_prioQb_add(distance);
		if (distance < vgCompDiamMin) vgCompDiamMin = distance;
		if (distance > vgCompDiamMax) vgCompDiamMax = distance;
		vgCompDiamMean += distance;

		//Update the number of elements in the queues.
		++qNumElt;
	}

	//Update the number of components.
	MAIN_SET_FATT_VG_NUMCOMP(vgNumComp);

	//Update the information for the component sizes.
	if (qNumElt > ZERO) {
		vgCompSizeMean = vgCompSizeMean / qNumElt;
		attributor_intern_prioQa_computeDescription(vgCompSizeMean,&vgCompSizeMed,&vgCompSizeStdDev,
				&vgCompSizeSent,&vgCompSizeRent,&vgCompSizeMent,
				&vgCompSizePGFA,&vgCompSizePGFB,&vgCompSizePGFC);
		MAIN_SET_FATT_VG_VCOMPSIZES_MIN(vgCompSizeMin);
		MAIN_SET_FATT_VG_VCOMPSIZES_MAX(vgCompSizeMax);
		MAIN_SET_FATT_VG_VCOMPSIZES_MEAN(vgCompSizeMean);
		MAIN_SET_FATT_VG_VCOMPSIZES_MEDIAN(vgCompSizeMed);
		MAIN_SET_FATT_VG_VCOMPSIZES_STDDEV(vgCompSizeStdDev);
		MAIN_SET_FATT_VG_VCOMPSIZES_SENT(vgCompSizeSent);
		MAIN_SET_FATT_VG_VCOMPSIZES_RENT(vgCompSizeRent);
		MAIN_SET_FATT_VG_VCOMPSIZES_MENT(vgCompSizeMent);
		MAIN_SET_FATT_VG_VCOMPSIZES_PGFA(vgCompSizePGFA);
		MAIN_SET_FATT_VG_VCOMPSIZES_PGFB(vgCompSizePGFB);
		MAIN_SET_FATT_VG_VCOMPSIZES_PGFC(vgCompSizePGFC);
	}

	//Update the information for the component diameters.
	if (qNumElt > ZERO) {
		vgCompDiamMean = vgCompDiamMean / qNumElt;
		attributor_intern_prioQb_computeDescription(vgCompDiamMean,&vgCompDiamMed,&vgCompDiamStdDev,
				&vgCompDiamSent,&vgCompDiamRent,&vgCompDiamMent,
				&vgCompDiamPGFA,&vgCompDiamPGFB,&vgCompDiamPGFC);
		MAIN_SET_FATT_VG_VCOMPDIAMS_MIN(vgCompDiamMin);
		MAIN_SET_FATT_VG_VCOMPDIAMS_MAX(vgCompDiamMax);
		MAIN_SET_FATT_VG_VCOMPDIAMS_MEAN(vgCompDiamMean);
		MAIN_SET_FATT_VG_VCOMPDIAMS_MEDIAN(vgCompDiamMed);
		MAIN_SET_FATT_VG_VCOMPDIAMS_STDDEV(vgCompDiamStdDev);
		MAIN_SET_FATT_VG_VCOMPDIAMS_SENT(vgCompDiamSent);
		MAIN_SET_FATT_VG_VCOMPDIAMS_RENT(vgCompDiamRent);
		MAIN_SET_FATT_VG_VCOMPDIAMS_MENT(vgCompDiamMent);
		MAIN_SET_FATT_VG_VCOMPDIAMS_PGFA(vgCompDiamPGFA);
		MAIN_SET_FATT_VG_VCOMPDIAMS_PGFB(vgCompDiamPGFB);
		MAIN_SET_FATT_VG_VCOMPDIAMS_PGFC(vgCompDiamPGFC);
	}

	//Finally, clean up the queues and stacks that have been used.
	vS_flush();
	lS_flush();
	attributor_intern_prioQa_resize(BLOCKSIZE);
	attributor_intern_prioQb_resize(BLOCKSIZE);
}

float_ty attributor_intern_compute_VG_articulationPoints(int32_t numElt){
	//This method computes the articulation points of the graph assuming that all the elements in the graph are variable IDs.
	if (numElt == ZERO){
		return ZERO;
	}

	float_ty result = ZERO;
	uint32_t vNum, childNode, rootChildren;
	nrdfsStackNode *sn;

	//First, we create an array containing all our variables as stack-nodes. We reset them all.
	nrdfsStackNode* theNodes = NULL;
	theNodes = malloc(sizeof(nrdfsStackNode)*(f.n_initial+2));
	if (theNodes == NULL){
		printf("c ERROR. The theNodes in the VG articulation point computation could not be allocated. Out of memory?\n");
		attributor_returnCode = ATTRIBUTOR_ERROR;
		return ZERO;
	}
	for (vNum = 0; vNum < f.n_initial+2; ++vNum){
		//We reset all its data.
		theNodes[vNum].numChildren = INT32_MAX;
		theNodes[vNum].node = vNum;
		theNodes[vNum].parent = INT32_MAX;
		theNodes[vNum].depth = INT32_MAX;
		theNodes[vNum].reach = INT32_MAX;
	}

	//Second, we resize the stack that simulates the depth first search. We use half the number of variables as a wild guess.
	attributor_intern_nrdfsStack_resize((uint32_t)(numElt/TWO));

	//Now that we have prepared everything, we can start our depth first search for each component root.
	for (vNum = 1; vNum < f.n_initial+1; ++vNum){
		if (ATTRIBUTOR_IS_TIME_ELAPSED()){
			#ifdef VERBOSE_ATTRIBUTOR
			printf("c     ATTRIBUTOR:   TIMEOUT VG-AP.\n");
			#endif
			#ifdef COLLECTSTATS
			++stats_attributor_numTimeouts;
			#endif
			break;
		}

		if (IS_VAR_DISABLED((main_varData + vNum)) || ATTRIBUTOR_GRAPH_GET_LIST_NUM_NUMUSED(vNum) == 0) continue;

		//Check if the current node was never checked. If so, it is a new root.
		if (theNodes[vNum].numChildren == INT32_MAX){
			//Yes, we construct a new root node here and perform the non-recursive depth first search on it.
			theNodes[vNum].depth = 0;
			attributor_intern_nrdfsStack_push(theNodes + vNum);
			rootChildren = 0;
			//We now perform the non-recursive depth first search.
			while (attributor_nrdfsStack_leaf > 0){
				sn = attributor_intern_nrdfsStack_read();
				//We separate into three cases.
				if (sn->numChildren == INT32_MAX){
					//1. We work on this node for the very first time.
					//Update the nodes depth and reach information.
					theNodes[sn->node].depth = sn->depth;
					sn->reach = sn->depth;
					//Update the nodes child information.
					sn->numChildren = ATTRIBUTOR_GRAPH_GET_LIST_NUM_NUMUSED(sn->node);
				} else if (sn->numChildren > 0){
					//2. We are still working through the children of this node. Grab the last children in the nodes queue.
					//and remove it.
					childNode = ATTRIBUTOR_GRAPH_GET_LIST_NUM_ELTNUM(sn->node, --sn->numChildren);
					//Check if this child is the parent node of sn. If so, ignore it.
					if (childNode != sn->parent) {
						//Check if we have already visited this child.
						if (theNodes[childNode].depth != INT32_MAX){
							//Yes, we have already visited this child. We update the reach information of the node.
							if (theNodes[childNode].depth < sn->reach){
								sn->reach = theNodes[childNode].depth;
							}
						} else {
							//No, we must visit the child first. We put it on the stack.
							theNodes[childNode].parent = sn->node;
							theNodes[childNode].depth = sn->depth+1;

							//If we are working on the root node, we must keep track of the number of its children.
							if (attributor_nrdfsStack_leaf == 1){
								++rootChildren;
							}

							attributor_intern_nrdfsStack_push(theNodes + childNode);
						}
					}
				} else {
					//3. We are finishing this node.
					if (theNodes[sn->node].depth > 1){
						if (sn->reach >= theNodes[sn->parent].depth) {
							++result;
						}
						if (sn->reach < theNodes[sn->parent].reach){
							theNodes[sn->parent].reach = sn->reach;
						}
					}
					attributor_intern_nrdfsStack_remove();
				}
			}
			//Finally, if the root had more than one child, we must mark it as an articulation point as well.
			if (rootChildren > 1){
				++result;
			}
		}
	}

	//Finally, we clean up all the temporarily allocated data.
	free(theNodes);
	attributor_intern_nrdfsStack_resize(BLOCKSIZE);
	return result;
}

void attributor_intern_compute_VG_allAttributes(){
	//Computes the variable graph attributes. This includes the number of components of the graph, the largest component
	//diameter, the number of components of the graph as well as the minimum, maximum, mean, median, standard deviation,
	//and entropy of the variable node degrees.

	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	#endif

	variable *v;
	literal *lPos, *lNeg, *cLit;
	clause *c;
	float_ty estAvgNDeg, estAvgNDegMin = UINT32_MAX, estAvgNDegMax = ZERO, estAvgNDegMean = ZERO, numElt = ZERO;
	float_ty estAvgNDegMedian = ZERO, estAvgNDegStdDev = ZERO, estAvgNDegSent = ZERO, estAvgNDegRent = ZERO,
			estAvgNDegMent = ZERO, estAvgNDegPGFA = ZERO, estAvgNDegPGFB = ZERO, estAvgNDegPGFC = ZERO;
	float_ty totalOccs;
	float_ty vDegree, vDegreeMin = UINT32_MAX, vDegreeMax = ZERO, vDegreeMean = ZERO;
	float_ty vDegreeMedian = ZERO, vDegreeStdDev = ZERO, vDegreeSent = ZERO, vDegreeRent = ZERO,
			vDegreeMent = ZERO, vDegreePGFA = ZERO, vDegreePGFB = ZERO, vDegreePGFC = ZERO;
	int32_t i, j, vNum, litNum;

	//First, we compute the estimated average node degree per variable. For that, we walk through all the clauses of the
	//formula and add the clause size minus one to the variables contained in the clause. In the end, this is divided by the
	//number of occurrences for the variable. We will use the variable stack position for this. This is dangerous as it might
	//brake the variable stack, but we will make sure in the end that this queue is reset.
	for (i = 1; i < f.n_initial+1; ++i){
		SET_VAR_S_POS((main_varData + i), 0);
	}

	//We now check out the clauses.
	for (i = 0; i < f.m_eo_used; ++i){
		if (ATTRIBUTOR_IS_TIME_ELAPSED()){
			#ifdef VERBOSE_ATTRIBUTOR
			printf("c     ATTRIBUTOR:   TIMEOUT VG-EC.\n");
			#endif
			#ifdef COLLECTSTATS
			++stats_attributor_numTimeouts;
			#endif
			break;
		}
		c = f.clauses_eo[i];
		for (j = 0; j < GET_CLS_SIZE(c); ++j){
			v = main_varData + abs(GET_CLS_LITNUM(c,j));
			SET_VAR_S_POS(v, (GET_VAR_S_POS(v) + GET_CLS_SIZE(c) - 1));
		}
	}
	//We have now computed the estimated node degree. We will now divide them by the number of occurrences for each variable
	//and push them into the queue for data computation.
	if (!ATTRIBUTOR_IS_TIME_ELAPSED()){
		attributor_intern_prioQa_resize(f.n_initial);
		for (i = 1; i < f.n_initial+1; ++i){
			if (ATTRIBUTOR_IS_TIME_ELAPSED()){
				#ifdef VERBOSE_ATTRIBUTOR
				printf("c     ATTRIBUTOR:   TIMEOUT VG-EV.\n");
				#endif
				#ifdef COLLECTSTATS
				++stats_attributor_numTimeouts;
				#endif
				break;
			}
			//Check how many occurrences the variable has. Do not add zero values to the array (useless information).
			totalOccs = GET_LIT_NUMOCCS_USED((main_litData + i)) + GET_LIT_NUMOCCS_USED((main_litData - i));
			if (totalOccs > 0){
				++numElt;
				estAvgNDeg = GET_VAR_S_POS((main_varData + i)) / totalOccs;
				if (estAvgNDeg < estAvgNDegMin) estAvgNDegMin = estAvgNDeg;
				if (estAvgNDeg > estAvgNDegMax) estAvgNDegMax = estAvgNDeg;
				estAvgNDegMean += estAvgNDeg;
				attributor_intern_prioQa_add(estAvgNDeg);
			}
		}

		//Compute the estimated average node degree information.
		if (numElt > ZERO) {
			estAvgNDegMean = estAvgNDegMean / numElt;
			attributor_intern_prioQa_computeDescription(estAvgNDegMean,&estAvgNDegMedian,&estAvgNDegStdDev,
					&estAvgNDegSent,&estAvgNDegRent,&estAvgNDegMent,
					&estAvgNDegPGFA,&estAvgNDegPGFB,&estAvgNDegPGFC);
			//Write the degree information.
			MAIN_SET_FATT_VG_ESTAVGNDEG_MIN(estAvgNDegMin);
			MAIN_SET_FATT_VG_ESTAVGNDEG_MAX(estAvgNDegMax);
			MAIN_SET_FATT_VG_ESTAVGNDEG_MEAN(estAvgNDegMean);
			MAIN_SET_FATT_VG_ESTAVGNDEG_MEDIAN(estAvgNDegMedian);
			MAIN_SET_FATT_VG_ESTAVGNDEG_STDDEV(estAvgNDegStdDev);
			MAIN_SET_FATT_VG_ESTAVGNDEG_SENT(estAvgNDegSent);
			MAIN_SET_FATT_VG_ESTAVGNDEG_RENT(estAvgNDegRent);
			MAIN_SET_FATT_VG_ESTAVGNDEG_MENT(estAvgNDegMent);
			MAIN_SET_FATT_VG_ESTAVGNDEG_PGFA(estAvgNDegPGFA);
			MAIN_SET_FATT_VG_ESTAVGNDEG_PGFB(estAvgNDegPGFB);
			MAIN_SET_FATT_VG_ESTAVGNDEG_PGFC(estAvgNDegPGFC);
		}

		//Reset the queue.
		attributor_intern_prioQa_resize(BLOCKSIZE);
	}

	//Reset the variable stack positions and the variable stack.
	for (i = 1; i < f.n_initial+1; ++i){
		SET_VAR_S_POS((main_varData + i), 0);
	}
	vS_leaf = 1;
	numElt = 0;

	//Then, we create the variable graph in form of an adjacency list. The graph has been allocated to contain enough lists
	//so all we need to do here is to fill the lists. Each adjacency list belongs to a single variable and holds all
	//immediate neighbors of this variable (all variables that this specific variable appears in a clause with - ignoring
	//itself).
	if (f.n_initial <= param_classifyMaxVGSize && !ATTRIBUTOR_IS_TIME_ELAPSED()){
		ATTRIBUTOR_GRAHP_ENSURE_LIST_SIZE( f.n_initial );
		ATTRIBUTOR_GRAPH_RESET_ALL_LISTS( i );
		attributor_intern_prioQa_resize(f.n_initial);
		for (vNum = 1; vNum < f.n_initial+1; ++vNum){
			if (ATTRIBUTOR_IS_TIME_ELAPSED()){
				#ifdef VERBOSE_ATTRIBUTOR
				printf("c     ATTRIBUTOR:   TIMEOUT VG-G.\n");
				#endif
				#ifdef COLLECTSTATS
				++stats_attributor_numTimeouts;
				#endif
				break;
			}
			//Grab the new variable and the corresponding literals. Ignore variables that are disabled.
			v = main_varData + vNum;
			if (IS_VAR_DISABLED(v)) continue;
			lPos = main_litData + vNum;
			lNeg = main_litData - vNum;
			//Walk through the occurrence lists of the literals and grab the clauses. Each newly encountered variable in the
			//clause is being stamped and added to the current adjacency list. We ignore the variable v itself.
			++main_litStamp;
			//Check all the positive occurrences of vNum.
			for (i = 0; i < GET_LIT_NUMOCCS_USED(lPos); ++i){
				c = GET_LIT_OCCNUM(lPos, i);
				//Check all the contained literals -- use the abstract only. If the literal is not stamped, we have found a
				//new adjacency. The abstract of the literal number is being added to the current list of v.
				for (j = 0; j < GET_CLS_SIZE(c); ++j){
					litNum = abs(GET_CLS_LITNUM(c, j));
					//Ignore the literal that corresponds to the current variable v.
					if (litNum == vNum) continue;
					//It is a different literal. We check if the cLit is already stamped. If so, we have seen it before and can
					//continue. Since we always stamp the positive literal, it suffices to check the positive literal stamp.
					cLit = main_litData + litNum;
					if (GET_LIT_STAMP(cLit) == main_litStamp) continue;
					//The literal is not stamped. We can now add the litNum into the adjacency list of vNum and stamp it.
					ATTRIBUTOR_GRAPH_ADD_LIST_NUM_ELT(vNum, litNum);
					SET_LIT_STAMP(cLit, main_litStamp);
				}
			}
			//Check all the negative occurrences of vNum.
			for (i = 0; i < GET_LIT_NUMOCCS_USED(lNeg); ++i){
				c = GET_LIT_OCCNUM(lNeg, i);
				//Check all the contained literals -- use the abstract only. If the literal is not stamped, we have found a
				//new adjacency. The abstract of the literal number is being added to the current list of v.
				for (j = 0; j < GET_CLS_SIZE(c); ++j){
					litNum = abs(GET_CLS_LITNUM(c, j));
					//Ignore the literal that corresponds to the current variable v.
					if (litNum == vNum) continue;
					//It is a different literal. We check if the cLit is already stamped. If so, we have seen it before and can
					//continue. Since we always stamp the positive literal, it suffices to check the positive literal stamp.
					cLit = main_litData + litNum;
					if (GET_LIT_STAMP(cLit) == main_litStamp) continue;
					//The literal is not stamped. We can now add the litNum into the adjacency list of vNum and stamp it.
					ATTRIBUTOR_GRAPH_ADD_LIST_NUM_ELT(vNum, litNum);
					SET_LIT_STAMP(cLit, main_litStamp);
				}
			}

			//After collecting all the adjacent variables, we use their count as degree for variable v. We update the minimum,
			//maximum and mean information, and add the degree to the priority queue.
			vDegree = ATTRIBUTOR_GRAPH_GET_LIST_NUM_NUMUSED(vNum);
			attributor_intern_prioQa_add(vDegree);
			++numElt;
			vDegreeMean += vDegree;
			if (vDegree < vDegreeMin) vDegreeMin = vDegree;
			if (vDegree > vDegreeMax) vDegreeMax = vDegree;

		}
		++main_litStamp;

		//Finalize the exact degree information.
		if (numElt > ZERO) {
			vDegreeMean = vDegreeMean / numElt;
			attributor_intern_prioQa_computeDescription(vDegreeMean,&vDegreeMedian,&vDegreeStdDev,
					&vDegreeSent,&vDegreeRent,&vDegreeMent,
					&vDegreePGFA,&vDegreePGFB,&vDegreePGFC);
			//Write the degree information.
			MAIN_SET_FATT_VG_VNODEDEGREE_MIN(vDegreeMin);
			MAIN_SET_FATT_VG_VNODEDEGREE_MAX(vDegreeMax);
			MAIN_SET_FATT_VG_VNODEDEGREE_MEAN(vDegreeMean);
			MAIN_SET_FATT_VG_VNODEDEGREE_MEDIAN(vDegreeMedian);
			MAIN_SET_FATT_VG_VNODEDEGREE_STDDEV(vDegreeStdDev);
			MAIN_SET_FATT_VG_VNODEDEGREE_SENT(vDegreeSent);
			MAIN_SET_FATT_VG_VNODEDEGREE_RENT(vDegreeRent);
			MAIN_SET_FATT_VG_VNODEDEGREE_MENT(vDegreeMent);
			MAIN_SET_FATT_VG_VNODEDEGREE_PGFA(vDegreePGFA);
			MAIN_SET_FATT_VG_VNODEDEGREE_PGFB(vDegreePGFB);
			MAIN_SET_FATT_VG_VNODEDEGREE_PGFC(vDegreePGFC);
		}
		attributor_intern_prioQa_resize(BLOCKSIZE);

		//Based on the constructed variable graph, we compute the attributes of the components.
		if (!ATTRIBUTOR_IS_TIME_ELAPSED()){
			attributor_intern_compute_VG_components((int32_t)numElt);
		}
		if (!ATTRIBUTOR_IS_TIME_ELAPSED()){
			float_ty numAP = attributor_intern_compute_VG_articulationPoints((int32_t)numElt);
			MAIN_SET_FATT_VG_NUMAP(numAP);
		}
	}
	#ifdef COLLECTSTATS
	else {
		if (f.n_initial > param_classifyMaxVGSize) ++stats_attributor_numSkipped;
	}
	#endif

	#ifdef VERBOSE_ATTRIBUTOR
	printf("c     ATTRIBUTOR:   VG_ESTAVGNDEG[MIN %f MAX %f MEAN %f MEDIAN %f SENT %f RENT %f MENT %f STDDEV %f PGFA %f PGFB %f PGFC %f]\n",
			MAIN_GET_FATT_VG_ESTAVGNDEG_MIN(),MAIN_GET_FATT_VG_ESTAVGNDEG_MAX(),MAIN_GET_FATT_VG_ESTAVGNDEG_MEAN(),
			MAIN_GET_FATT_VG_ESTAVGNDEG_MEDIAN(),MAIN_GET_FATT_VG_ESTAVGNDEG_SENT(),MAIN_GET_FATT_VG_ESTAVGNDEG_RENT(),
			MAIN_GET_FATT_VG_ESTAVGNDEG_MENT(),MAIN_GET_FATT_VG_ESTAVGNDEG_STDDEV(),MAIN_GET_FATT_VG_ESTAVGNDEG_PGFA(),
			MAIN_GET_FATT_VG_ESTAVGNDEG_PGFB(),MAIN_GET_FATT_VG_ESTAVGNDEG_PGFC());
	printf("c     ATTRIBUTOR:   VG_VNODEDEGREE[MIN %f MAX %f MEAN %f MEDIAN %f SENT %f RENT %f MENT %f STDDEV %f PGFA %f PGFB %f PGFC %f]\n",
			MAIN_GET_FATT_VG_VNODEDEGREE_MIN(),MAIN_GET_FATT_VG_VNODEDEGREE_MAX(),MAIN_GET_FATT_VG_VNODEDEGREE_MEAN(),
			MAIN_GET_FATT_VG_VNODEDEGREE_MEDIAN(),MAIN_GET_FATT_VG_VNODEDEGREE_SENT(),MAIN_GET_FATT_VG_VNODEDEGREE_RENT(),
			MAIN_GET_FATT_VG_VNODEDEGREE_MENT(),MAIN_GET_FATT_VG_VNODEDEGREE_STDDEV(),MAIN_GET_FATT_VG_VNODEDEGREE_PGFA(),
			MAIN_GET_FATT_VG_VNODEDEGREE_PGFB(),MAIN_GET_FATT_VG_VNODEDEGREE_PGFC());
	printf("c     ATTRIBUTOR:   VG_NUMCOMP[%f]\n", MAIN_GET_FATT_VG_NUMCOMP());
	printf("c     ATTRIBUTOR:   VG_VCOMPSIZES[MIN %f MAX %f MEAN %f MEDIAN %f SENT %f RENT %f MENT %f STDDEV %f PGFA %f PGFB %f PGFC %f]\n",
			MAIN_GET_FATT_VG_VCOMPSIZES_MIN(),MAIN_GET_FATT_VG_VCOMPSIZES_MAX(),MAIN_GET_FATT_VG_VCOMPSIZES_MEAN(),
			MAIN_GET_FATT_VG_VCOMPSIZES_MEDIAN(),MAIN_GET_FATT_VG_VCOMPSIZES_SENT(),MAIN_GET_FATT_VG_VCOMPSIZES_RENT(),
			MAIN_GET_FATT_VG_VCOMPSIZES_MENT(),MAIN_GET_FATT_VG_VCOMPSIZES_STDDEV(),MAIN_GET_FATT_VG_VCOMPSIZES_PGFA(),
			MAIN_GET_FATT_VG_VCOMPSIZES_PGFB(),MAIN_GET_FATT_VG_VCOMPSIZES_PGFC());
	printf("c     ATTRIBUTOR:   VG_VCOMPDIAMS[MIN %f MAX %f MEAN %f MEDIAN %f SENT %f RENT %f MENT %f STDDEV %f PGFA %f PGFB %f PGFC %f]\n",
			MAIN_GET_FATT_VG_VCOMPDIAMS_MIN(),MAIN_GET_FATT_VG_VCOMPDIAMS_MAX(),MAIN_GET_FATT_VG_VCOMPDIAMS_MEAN(),
			MAIN_GET_FATT_VG_VCOMPDIAMS_MEDIAN(),MAIN_GET_FATT_VG_VCOMPDIAMS_SENT(),MAIN_GET_FATT_VG_VCOMPDIAMS_RENT(),
			MAIN_GET_FATT_VG_VCOMPDIAMS_MENT(),MAIN_GET_FATT_VG_VCOMPDIAMS_STDDEV(),MAIN_GET_FATT_VG_VCOMPDIAMS_PGFA(),
			MAIN_GET_FATT_VG_VCOMPDIAMS_PGFB(),MAIN_GET_FATT_VG_VCOMPDIAMS_PGFC());
	printf("c     ATTRIBUTOR:   VG_NUMAP[%f]\n", MAIN_GET_FATT_VG_NUMAP());
	fflush(stdout);
	#endif

	#ifdef COLLECTSTATS
	stats_attributor_time_VG += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}

void attributor_intern_compute_CG_allAttributes(){
	//Computes the clause graph attributes. This includes the minimum, maximum, mean, median, standard deviation, and entropy
	//and of the clause node degrees. Furthermore, it computes the average node degree per literal of all the clause nodes.

	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	#endif

	ATTRIBUTOR_SET_START_TIME_IN_SEC();

	clause *c;
	literal *cLit;
	float_ty numElt = ZERO;
	float_ty cDegree, cDegreeMin = UINT32_MAX, cDegreeMax = ZERO, cDegreeMean = ZERO, cDegreeMedian = ZERO,
			 cDegreeStdDev = ZERO, cDegreeSent = ZERO, cDegreeRent = ZERO, cDegreeMent = ZERO,
			 cDegreePGFA = ZERO, cDegreePGFB = ZERO, cDegreePGFC = ZERO;
	float_ty avgDegPerLit, avgDegPerLitMin = UINT32_MAX, avgDegPerLitMax = ZERO, avgDegPerLitMean = ZERO,
			 avgDegPerLitMedian = ZERO, avgDegPerLitStdDev = ZERO, avgDegPerLitSent = ZERO, avgDegPerLitRent = ZERO,
			 avgDegPerLitMent = ZERO, avgDegPerLitPGFA = ZERO, avgDegPerLitPGFB = ZERO, avgDegPerLitPGFC = ZERO;
	uint32_t cNum;
	int32_t i;

	if (f.m_initial <= param_classifyMaxCGSize){
		attributor_intern_prioQa_resize(f.m_eo_used);
		attributor_intern_prioQb_resize(f.m_eo_used);
		//attributor_intern_prioQb_resize(f.m_eo_used);
		for (cNum = 0; cNum < f.m_eo_used; ++cNum){
			if (ATTRIBUTOR_IS_TIME_ELAPSED()){
				#ifdef VERBOSE_ATTRIBUTOR
				printf("c     ATTRIBUTOR:   TIMEOUT CG-D.\n");
				#endif
				#ifdef COLLECTSTATS
				++stats_attributor_numTimeouts;
				#endif
				break;
			}
			//Grab the next clause.
			c = f.clauses_eo[cNum];
			cDegree = ZERO;
			avgDegPerLit = ZERO;
			for (i = 0; i < GET_CLS_SIZE(c); ++i){
				cLit = main_litData + GET_CLS_LITNUM(c,i);
				cDegree += GET_LIT_NUMOCCS_USED(cLit);
				if (GET_LIT_NUMOCCS_USED(cLit) > 0){
					cDegree -= 1;
				}
				cLit = main_litData - GET_CLS_LITNUM(c,i);
				cDegree += GET_LIT_NUMOCCS_USED(cLit);
				if (GET_LIT_NUMOCCS_USED(cLit) > 0){
					cDegree -= 1;
				}
			}
			++numElt;

			//Add the degree information for this clause.
			if (cDegree < cDegreeMin) cDegreeMin = cDegree;
			if (cDegree > cDegreeMax) cDegreeMax = cDegree;
			cDegreeMean += cDegree;
			attributor_intern_prioQa_add(cDegree);

			//Add the cCliqueyness information for this clause.
			avgDegPerLit = cDegree / (float_ty)GET_CLS_SIZE(c);
			if (avgDegPerLit < avgDegPerLitMin) avgDegPerLitMin = avgDegPerLit;
			if (avgDegPerLit > avgDegPerLitMax) avgDegPerLitMax = avgDegPerLit;
			avgDegPerLitMean += avgDegPerLit;
			attributor_intern_prioQb_add(avgDegPerLit);
		}

		//We can now compute the information regarding the node degree.
		if (numElt > ZERO) {
			cDegreeMean = cDegreeMean / numElt;
			attributor_intern_prioQa_computeDescription(cDegreeMean,&cDegreeMedian,&cDegreeStdDev,
					&cDegreeSent,&cDegreeRent,&cDegreeMent,
					&cDegreePGFA,&cDegreePGFB,&cDegreePGFC);
			//Write the information.
			MAIN_SET_FATT_CG_CNODEDEGREE_MIN(cDegreeMin);
			MAIN_SET_FATT_CG_CNODEDEGREE_MAX(cDegreeMax);
			MAIN_SET_FATT_CG_CNODEDEGREE_MEAN(cDegreeMean);
			MAIN_SET_FATT_CG_CNODEDEGREE_MEDIAN(cDegreeMedian);
			MAIN_SET_FATT_CG_CNODEDEGREE_STDDEV(cDegreeStdDev);
			MAIN_SET_FATT_CG_CNODEDEGREE_SENT(cDegreeSent);
			MAIN_SET_FATT_CG_CNODEDEGREE_RENT(cDegreeRent);
			MAIN_SET_FATT_CG_CNODEDEGREE_MENT(cDegreeMent);
			MAIN_SET_FATT_CG_CNODEDEGREE_PGFA(cDegreePGFA);
			MAIN_SET_FATT_CG_CNODEDEGREE_PGFB(cDegreePGFB);
			MAIN_SET_FATT_CG_CNODEDEGREE_PGFC(cDegreePGFC);
		}
		attributor_intern_prioQa_resize(BLOCKSIZE);

		//Furthermore, we can update the average node degree per literal information for the clauses.
		if (numElt > ZERO) {
			avgDegPerLitMean = avgDegPerLitMean / numElt;
			attributor_intern_prioQb_computeDescription(avgDegPerLitMean,&avgDegPerLitMedian,&avgDegPerLitStdDev,
					&avgDegPerLitSent,&avgDegPerLitRent,&avgDegPerLitMent,
					&avgDegPerLitPGFA,&avgDegPerLitPGFB,&avgDegPerLitPGFC);
			//Write the information.
			MAIN_SET_FATT_CG_AVGDEGPERLIT_MIN(avgDegPerLitMin);
			MAIN_SET_FATT_CG_AVGDEGPERLIT_MAX(avgDegPerLitMax);
			MAIN_SET_FATT_CG_AVGDEGPERLIT_MEAN(avgDegPerLitMean);
			MAIN_SET_FATT_CG_AVGDEGPERLIT_MEDIAN(avgDegPerLitMedian);
			MAIN_SET_FATT_CG_AVGDEGPERLIT_STDDEV(avgDegPerLitStdDev);
			MAIN_SET_FATT_CG_AVGDEGPERLIT_SENT(avgDegPerLitSent);
			MAIN_SET_FATT_CG_AVGDEGPERLIT_RENT(avgDegPerLitRent);
			MAIN_SET_FATT_CG_AVGDEGPERLIT_MENT(avgDegPerLitMent);
			MAIN_SET_FATT_CG_AVGDEGPERLIT_PGFA(avgDegPerLitPGFA);
			MAIN_SET_FATT_CG_AVGDEGPERLIT_PGFB(avgDegPerLitPGFB);
			MAIN_SET_FATT_CG_AVGDEGPERLIT_PGFC(avgDegPerLitPGFC);
		}
		attributor_intern_prioQb_resize(BLOCKSIZE);
	}
	#ifdef COLLECTSTATS
	else {
		++stats_attributor_numSkipped;
	}
	#endif

	#ifdef VERBOSE_ATTRIBUTOR
	printf("c     ATTRIBUTOR:   CG_CNODEDEGREE[MIN %f MAX %f MEAN %f MEDIAN %f SENT %f RENT %f MENT %f STDDEV %f PGFA %f PGFB %f PGFC %f]\n",
			MAIN_GET_FATT_CG_CNODEDEGREE_MIN(),MAIN_GET_FATT_CG_CNODEDEGREE_MAX(),MAIN_GET_FATT_CG_CNODEDEGREE_MEAN(),
			MAIN_GET_FATT_CG_CNODEDEGREE_MEDIAN(),MAIN_GET_FATT_CG_CNODEDEGREE_SENT(),MAIN_GET_FATT_CG_CNODEDEGREE_RENT(),
			MAIN_GET_FATT_CG_CNODEDEGREE_MENT(),MAIN_GET_FATT_CG_CNODEDEGREE_STDDEV(),MAIN_GET_FATT_CG_CNODEDEGREE_PGFA(),
			MAIN_GET_FATT_CG_CNODEDEGREE_PGFB(),MAIN_GET_FATT_CG_CNODEDEGREE_PGFC());
	printf("c     ATTRIBUTOR:   CG_AVGDEGPERLIT[MIN %f MAX %f MEAN %f MEDIAN %f SENT %f RENT %f MENT %f STDDEV %f PGFA %f PGFB %f PGFC %f]\n",
			MAIN_GET_FATT_CG_AVGDEGPERLIT_MIN(),MAIN_GET_FATT_CG_AVGDEGPERLIT_MAX(),MAIN_GET_FATT_CG_AVGDEGPERLIT_MEAN(),
			MAIN_GET_FATT_CG_AVGDEGPERLIT_MEDIAN(),MAIN_GET_FATT_CG_AVGDEGPERLIT_SENT(),MAIN_GET_FATT_CG_AVGDEGPERLIT_RENT(),
			MAIN_GET_FATT_CG_AVGDEGPERLIT_MENT(),MAIN_GET_FATT_CG_AVGDEGPERLIT_STDDEV(),MAIN_GET_FATT_CG_AVGDEGPERLIT_PGFA(),
			MAIN_GET_FATT_CG_AVGDEGPERLIT_PGFB(),MAIN_GET_FATT_CG_AVGDEGPERLIT_PGFC());
	fflush(stdout);
	#endif

	#ifdef COLLECTSTATS
	stats_attributor_time_CG += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}

void attributor_intern_compute_B_allAttributes(){
	//Computes the balance attributes, the ratio of positive to negative literal occurrences for all variables, the ratio
	//of positive to negative literal occurrences in each clause and the fraction of clauses of specific sizes.

	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	#endif

	ATTRIBUTOR_SET_START_TIME_IN_SEC();

	variable *v;
	literal *lPos, *lNeg;
	clause *c;
	float_ty posOcc, negOcc, numElt = ZERO;
	float_ty vLitBalance, vLitBalanceMin = UINT32_MAX, vLitBalanceMax = ZERO, vLitBalanceMean = ZERO, vLitBalanceMedian = ZERO,
			 vLitBalanceStdDev = ZERO, vLitBalanceSent = ZERO, vLitBalanceRent = ZERO, vLitBalanceMent = ZERO,
			 vLitBalancePGFA = ZERO, vLitBalancePGFB = ZERO, vLitBalancePGFC = ZERO;
	float_ty cLitBalance, cLitBalanceMin = UINT32_MAX, cLitBalanceMax = ZERO, cLitBalanceMean = ZERO, cLitBalanceMedian = ZERO,
			 cLitBalanceStdDev = ZERO, cLitBalanceSent = ZERO, cLitBalanceRent = ZERO, cLitBalanceMent = ZERO,
			 cLitBalancePGFA = ZERO, cLitBalancePGFB = ZERO, cLitBalancePGFC = ZERO;
	float_ty cSize2 = ZERO, cSize3 = ZERO, cSize4 = ZERO, cSize5 = ZERO, cSize6 = ZERO, cSize7 = ZERO;
	int32_t id, litNum;

	//First, we compute the balance attributes for the variables. We check all variables and see how many occurrences are
	//positive. Ignore variables that are disabled.
	attributor_intern_prioQa_resize(f.n_initial);
	for (id = 1; id < f.n_initial+1; ++id){
		if (ATTRIBUTOR_IS_TIME_ELAPSED()){
			#ifdef VERBOSE_ATTRIBUTOR
			printf("c     ATTRIBUTOR:   TIMEOUT B-V.\n");
			#endif
			#ifdef COLLECTSTATS
			++stats_attributor_numTimeouts;
			#endif
			break;
		}
		v = main_varData + id;
		if (IS_VAR_DISABLED(v)) continue;
		//Grab the corresponding literals.
		lPos = main_litData + id;
		lNeg = main_litData - id;
		posOcc = GET_LIT_NUMOCCS_USED(lPos);
		negOcc = GET_LIT_NUMOCCS_USED(lNeg);
		if (posOcc + negOcc == ZERO){
			vLitBalance = ZERO;
		} else {
			vLitBalance = posOcc / (posOcc + negOcc);
		}
		//Update the literal balance for the variable.
		if (vLitBalance < vLitBalanceMin) vLitBalanceMin = vLitBalance;
		if (vLitBalance > vLitBalanceMax) vLitBalanceMax = vLitBalance;
		vLitBalanceMean += vLitBalance;
		attributor_intern_prioQa_add(vLitBalance);
		++numElt;
	}

	//We have computed all the variable balances. We update the balance information.
	if (numElt > ZERO){
		vLitBalanceMean = vLitBalanceMean / numElt;
		attributor_intern_prioQa_computeDescription(vLitBalanceMean,&vLitBalanceMedian,&vLitBalanceStdDev,
				&vLitBalanceSent,&vLitBalanceRent,&vLitBalanceMent,
				&vLitBalancePGFA,&vLitBalancePGFB,&vLitBalancePGFC);
		//Write the information.
		MAIN_SET_FATT_B_VLITBALANCE_MIN(vLitBalanceMin);
		MAIN_SET_FATT_B_VLITBALANCE_MAX(vLitBalanceMax);
		MAIN_SET_FATT_B_VLITBALANCE_MEAN(vLitBalanceMean);
		MAIN_SET_FATT_B_VLITBALANCE_MEDIAN(vLitBalanceMedian);
		MAIN_SET_FATT_B_VLITBALANCE_STDDEV(vLitBalanceStdDev);
		MAIN_SET_FATT_B_VLITBALANCE_SENT(vLitBalanceSent);
		MAIN_SET_FATT_B_VLITBALANCE_RENT(vLitBalanceRent);
		MAIN_SET_FATT_B_VLITBALANCE_MENT(vLitBalanceMent);
		MAIN_SET_FATT_B_VLITBALANCE_PGFA(vLitBalancePGFA);
		MAIN_SET_FATT_B_VLITBALANCE_PGFB(vLitBalancePGFB);
		MAIN_SET_FATT_B_VLITBALANCE_PGFC(vLitBalancePGFC);
	}
	//Clean up.
	attributor_intern_prioQa_resize(BLOCKSIZE);

	//Second, we compute the balance attribute for all clauses. We check all clauses and see how many literals are positive
	//and how many are negative.
	if (!ATTRIBUTOR_IS_TIME_ELAPSED()){
		attributor_intern_prioQa_resize(f.m_eo_used);
		numElt = ZERO;
		for (id = 0; id < f.m_eo_used; ++id){
			if (ATTRIBUTOR_IS_TIME_ELAPSED()){
				#ifdef VERBOSE_ATTRIBUTOR
				printf("c     ATTRIBUTOR:   TIMEOUT B-C.\n");
				#endif
				#ifdef COLLECTSTATS
				++stats_attributor_numTimeouts;
				#endif
				break;
			}
			c = f.clauses_eo[id];
			//Reset the counters and store the size information of the clause.
			posOcc = ZERO;
			negOcc = ZERO;
			if (GET_CLS_SIZE(c) == 2){
				++cSize2;
			} else if (GET_CLS_SIZE(c) == 3){
				++cSize3;
			} else if (GET_CLS_SIZE(c) == 4){
				++cSize4;
			} else if (GET_CLS_SIZE(c) == 5){
				++cSize5;
			} else if (GET_CLS_SIZE(c) == 6){
				++cSize6;
			} else if (GET_CLS_SIZE(c) == 7){
				++cSize7;
			}

			//Count the literals.
			for (litNum = 0; litNum < GET_CLS_SIZE(c); ++litNum){
				if (GET_CLS_LITNUM(c, litNum) < 0){
					++negOcc;
				} else {
					++posOcc;
				}
			}

			//Compute the balance value and add it to the priority queue.
			if (posOcc + negOcc == ZERO){
				cLitBalance = ZERO;
			} else {
				cLitBalance = posOcc / (posOcc + negOcc);
			}
			if (cLitBalance < cLitBalanceMin) cLitBalanceMin = cLitBalance;
			if (cLitBalance > cLitBalanceMax) cLitBalanceMax = cLitBalance;
			cLitBalanceMean += cLitBalance;
			attributor_intern_prioQa_add(cLitBalance);
			++numElt;
		}

		//We have computed all the clause balances and size information. We update the balance information.
		if (numElt > ZERO){
			cSize2 = cSize2 / numElt;
			cSize3 = cSize3 / numElt;
			cSize4 = cSize4 / numElt;
			cSize5 = cSize5 / numElt;
			cSize6 = cSize6 / numElt;
			cSize7 = cSize7 / numElt;
			cLitBalanceMean = cLitBalanceMean / numElt;
			attributor_intern_prioQa_computeDescription(cLitBalanceMean,&cLitBalanceMedian,&cLitBalanceStdDev,
					&cLitBalanceSent,&cLitBalanceRent,&cLitBalanceMent,
					&cLitBalancePGFA,&cLitBalancePGFB,&cLitBalancePGFC);
			//Write the information.
			MAIN_SET_FATT_B_CSIZE2(cSize2);
			MAIN_SET_FATT_B_CSIZE3(cSize3);
			MAIN_SET_FATT_B_CSIZE4(cSize4);
			MAIN_SET_FATT_B_CSIZE5(cSize5);
			MAIN_SET_FATT_B_CSIZE6(cSize6);
			MAIN_SET_FATT_B_CSIZE7(cSize7);
			MAIN_SET_FATT_B_CLITBALANCE_MIN(cLitBalanceMin);
			MAIN_SET_FATT_B_CLITBALANCE_MAX(cLitBalanceMax);
			MAIN_SET_FATT_B_CLITBALANCE_MEAN(cLitBalanceMean);
			MAIN_SET_FATT_B_CLITBALANCE_MEDIAN(cLitBalanceMedian);
			MAIN_SET_FATT_B_CLITBALANCE_STDDEV(cLitBalanceStdDev);
			MAIN_SET_FATT_B_CLITBALANCE_SENT(cLitBalanceSent);
			MAIN_SET_FATT_B_CLITBALANCE_RENT(cLitBalanceRent);
			MAIN_SET_FATT_B_CLITBALANCE_MENT(cLitBalanceMent);
			MAIN_SET_FATT_B_CLITBALANCE_PGFA(cLitBalancePGFA);
			MAIN_SET_FATT_B_CLITBALANCE_PGFB(cLitBalancePGFB);
			MAIN_SET_FATT_B_CLITBALANCE_PGFC(cLitBalancePGFC);
		}

		//Clean up.
		attributor_intern_prioQa_resize(BLOCKSIZE);
	}

	#ifdef VERBOSE_ATTRIBUTOR
	printf("c     ATTRIBUTOR:   B_VLITBALANCE[MIN %f MAX %f MEAN %f MEDIAN %f SENT %f RENT %f MENT %f STDDEV %f PGFA %f PGFB %f PGFC %f]\n",
			MAIN_GET_FATT_B_VLITBALANCE_MIN(),MAIN_GET_FATT_B_VLITBALANCE_MAX(),MAIN_GET_FATT_B_VLITBALANCE_MEAN(),
			MAIN_GET_FATT_B_VLITBALANCE_MEDIAN(),MAIN_GET_FATT_B_VLITBALANCE_SENT(),MAIN_GET_FATT_B_VLITBALANCE_RENT(),
			MAIN_GET_FATT_B_VLITBALANCE_MENT(),MAIN_GET_FATT_B_VLITBALANCE_STDDEV(),MAIN_GET_FATT_B_VLITBALANCE_PGFA(),
			MAIN_GET_FATT_B_VLITBALANCE_PGFB(),MAIN_GET_FATT_B_VLITBALANCE_PGFC());
	printf("c     ATTRIBUTOR:   B_CLITBALANCE[MIN %f MAX %f MEAN %f MEDIAN %f SENT %f RENT %f MENT %f STDDEV %f PGFA %f PGFB %f PGFC %f]\n",
			MAIN_GET_FATT_B_CLITBALANCE_MIN(),MAIN_GET_FATT_B_CLITBALANCE_MAX(),MAIN_GET_FATT_B_CLITBALANCE_MEAN(),
			MAIN_GET_FATT_B_CLITBALANCE_MEDIAN(),MAIN_GET_FATT_B_CLITBALANCE_SENT(),MAIN_GET_FATT_B_CLITBALANCE_RENT(),
			MAIN_GET_FATT_B_CLITBALANCE_MENT(),MAIN_GET_FATT_B_CLITBALANCE_STDDEV(),MAIN_GET_FATT_B_CLITBALANCE_PGFA(),
			MAIN_GET_FATT_B_CLITBALANCE_PGFB(),MAIN_GET_FATT_B_CLITBALANCE_PGFC());
	printf("c     ATTRIBUTOR:   B_CSIZE2[%f] B_CSIZE3[%f] B_CSIZE4[%f] B_CSIZE5[%f] B_CSIZE6[%f] B_CSIZE7[%f]\n",
			MAIN_GET_FATT_B_CSIZE2(),MAIN_GET_FATT_B_CSIZE3(),MAIN_GET_FATT_B_CSIZE4(),MAIN_GET_FATT_B_CSIZE5(),
			MAIN_GET_FATT_B_CSIZE6(),MAIN_GET_FATT_B_CSIZE7());
	fflush(stdout);
	#endif

	#ifdef COLLECTSTATS
	stats_attributor_time_B += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}

void attributor_intern_compute_HP_allAttributes(){
	//Computes the Horn proximity attributes. It computes the fraction of clauses that are Horn clauses. It computes the
	//fraction of variable occurrences in Horn clauses (minimum, maximum, etc.), and it computes the distance of the clauses
	//to be horn clauses (minimum, maximum, etc.).

	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	#endif

	ATTRIBUTOR_SET_START_TIME_IN_SEC();

	clause *c;
	float_ty hornFrac = ZERO, numElt = ZERO, posOcc, negOcc;
	float_ty vHornFrac, vHornFracMin = UINT32_MAX, vHornFracMax = ZERO, vHornFracMean = ZERO, vHornFracMedian = ZERO,
			 vHornFracStdDev = ZERO, vHornFracSent = ZERO, vHornFracRent = ZERO, vHornFracMent = ZERO,
			 vHornFracPGFA = ZERO, vHornFracPGFB = ZERO, vHornFracPGFC = ZERO;
	float_ty cHornDist, cHornDistMin = UINT32_MAX, cHornDistMax = ZERO, cHornDistMean = ZERO, cHornDistMedian = ZERO,
			 cHornDistStdDev = ZERO, cHornDistSent = ZERO, cHornDistRent = ZERO, cHornDistMent = ZERO,
			 cHornDistPGFA = ZERO, cHornDistPGFB = ZERO, cHornDistPGFC = ZERO;
	int32_t litNum, cNum, vNum;

	//We will use the literal stamp memory position to count how often a literal was part of a horn clause.
	for (vNum = 0; vNum < f.n_initial+1; ++vNum){
		SET_LIT_NUM_STAMP(vNum, 0);
		SET_LIT_NUM_STAMP(-vNum, 0);
	}

	//First, we iterate over all clauses and check if they are horn (contain at most one positive literal). We also check the
	//clauses distance to a horn clause.
	attributor_intern_prioQa_resize(f.m_eo_used);
	for (cNum = 0; cNum < f.m_eo_used; ++cNum){
		if (ATTRIBUTOR_IS_TIME_ELAPSED()){
			#ifdef VERBOSE_ATTRIBUTOR
			printf("c     ATTRIBUTOR:   TIMEOUT HP-C.\n");
			#endif
			#ifdef COLLECTSTATS
			++stats_attributor_numTimeouts;
			#endif
			break;
		}
		c = f.clauses_eo[cNum];
		posOcc = ZERO;
		negOcc = ZERO;
		//Count the positive and negative literals in the clause.
		for (litNum = 0; litNum < GET_CLS_SIZE(c); ++litNum){
			if (GET_CLS_LITNUM(c, litNum) < 0){
				++negOcc;
			} else {
				++posOcc;
			}
		}
		//Check if it is a Horn clause. Update the clauses distance to being a Horn clause and count the occurrences.
		if (posOcc < TWO){
			//Yes, it is a Horn clause. For this clause, we count that it is horn and increase the counter for the literals.
			++hornFrac;
			for (litNum = 0; litNum < GET_CLS_SIZE(c); ++litNum){
				SET_LIT_NUM_STAMP(GET_CLS_LITNUM(c,litNum), GET_LIT_NUM_STAMP(GET_CLS_LITNUM(c,litNum)) + 1);
			}
			cHornDist = ZERO;
		} else {
			//No, it is no horn. We compute the distance to a Horn clause by counting how many positive literals the clause
			//has. From that number we remove one (one positive literal would be OK). We divide this number by the size of the
			//clause minus one (one positive literal might be removed). Then, the result is exactly 1.0 if all literals in the
			//clause are positive.
			cHornDist = (posOcc-ONE) / (((float_ty) GET_CLS_SIZE(c)) - ONE);
		}
		//Update the information for the clause distance and the priority queue.
		if (cHornDist < cHornDistMin) cHornDistMin = cHornDist;
		if (cHornDist > cHornDistMax) cHornDistMax = cHornDist;
		cHornDistMean += cHornDist;
		attributor_intern_prioQa_add(cHornDist);
		++numElt;
	}

	//We have now computed the distances for all clauses to be horn and how many of them are horn. We update the values.
	if (numElt > ZERO){
		hornFrac = hornFrac / numElt;
		cHornDistMean = cHornDistMean / numElt;
		attributor_intern_prioQa_computeDescription(cHornDistMean,&cHornDistMedian,&cHornDistStdDev,
				&cHornDistSent,&cHornDistRent,&cHornDistMent,
				&cHornDistPGFA,&cHornDistPGFB,&cHornDistPGFC);
		//Write the information.
		MAIN_SET_FATT_HP_HORNFRAC(hornFrac);
		MAIN_SET_FATT_HP_CHORNDIST_MIN(cHornDistMin);
		MAIN_SET_FATT_HP_CHORNDIST_MAX(cHornDistMax);
		MAIN_SET_FATT_HP_CHORNDIST_MEAN(cHornDistMean);
		MAIN_SET_FATT_HP_CHORNDIST_MEDIAN(cHornDistMedian);
		MAIN_SET_FATT_HP_CHORNDIST_STDDEV(cHornDistStdDev);
		MAIN_SET_FATT_HP_CHORNDIST_SENT(cHornDistSent);
		MAIN_SET_FATT_HP_CHORNDIST_RENT(cHornDistRent);
		MAIN_SET_FATT_HP_CHORNDIST_MENT(cHornDistMent);
		MAIN_SET_FATT_HP_CHORNDIST_PGFA(cHornDistPGFA);
		MAIN_SET_FATT_HP_CHORNDIST_PGFB(cHornDistPGFB);
		MAIN_SET_FATT_HP_CHORNDIST_PGFC(cHornDistPGFC);
	}
	//Clean up.
	attributor_intern_prioQa_resize(BLOCKSIZE);

	//Second, based on the counters in the literal stamp memory position, we check how often a variable appears in a horn
	//formula (ratio of it). We count the sum of Horn occurrences in posOcc, and the total occurrence in negOcc.

	numElt = ZERO;
	attributor_intern_prioQa_resize(f.n_initial);
	for (vNum = 1; vNum < f.n_initial +1; ++vNum){
		if (IS_VAR_DISABLED( (main_varData + vNum))) continue;

		negOcc = GET_LIT_NUM_NUMOCCS_USED(vNum) + GET_LIT_NUM_NUMOCCS_USED(-vNum);

		//Compute the fraction of variable occurrences in Horn clauses.
		if (negOcc == ZERO){
			vHornFrac = ZERO;
		} else {
			posOcc = GET_LIT_NUM_STAMP(vNum) + GET_LIT_NUM_STAMP(-vNum);
			vHornFrac = posOcc / negOcc;
		}

		//Update the Horn information for the variable and add it to the queue.
		if (vHornFrac < vHornFracMin) vHornFracMin = vHornFrac;
		if (vHornFrac > vHornFracMax) vHornFracMax = vHornFrac;
		vHornFracMean += vHornFrac;
		attributor_intern_prioQa_add(vHornFrac);
		++numElt;
	}
	//We have computed the fraction of Horn occurrences for all variables. We update the information.
	if (numElt > ZERO){
		vHornFracMean = vHornFracMean / numElt;
		attributor_intern_prioQa_computeDescription(vHornFracMean,&vHornFracMedian,&vHornFracStdDev,
				&vHornFracSent,&vHornFracRent,&vHornFracMent,
				&vHornFracPGFA,&vHornFracPGFB,&vHornFracPGFC);
		//Write the information.
		MAIN_SET_FATT_HP_VHORNFRAC_MIN(vHornFracMin);
		MAIN_SET_FATT_HP_VHORNFRAC_MAX(vHornFracMax);
		MAIN_SET_FATT_HP_VHORNFRAC_MEAN(vHornFracMean);
		MAIN_SET_FATT_HP_VHORNFRAC_MEDIAN(vHornFracMedian);
		MAIN_SET_FATT_HP_VHORNFRAC_STDDEV(vHornFracStdDev);
		MAIN_SET_FATT_HP_VHORNFRAC_SENT(vHornFracSent);
		MAIN_SET_FATT_HP_VHORNFRAC_RENT(vHornFracRent);
		MAIN_SET_FATT_HP_VHORNFRAC_MENT(vHornFracMent);
		MAIN_SET_FATT_HP_VHORNFRAC_PGFA(vHornFracPGFA);
		MAIN_SET_FATT_HP_VHORNFRAC_PGFB(vHornFracPGFB);
		MAIN_SET_FATT_HP_VHORNFRAC_PGFC(vHornFracPGFC);
	}

	//Clean up. Set the literal stamp to something big here.
	attributor_intern_prioQa_resize(BLOCKSIZE);
	main_litStamp = 2*f.m_eo_used + 2;

	#ifdef VERBOSE_ATTRIBUTOR
	printf("c     ATTRIBUTOR:   HP_HORNFRAC[%f]\n", MAIN_GET_FATT_HP_HORNFRAC());
	printf("c     ATTRIBUTOR:   HP_VHORNFRAC[MIN %f MAX %f MEAN %f MEDIAN %f SENT %f RENT %f MENT %f STDDEV %f PGFA %f PGFB %f PGFC %f]\n",
			MAIN_GET_FATT_HP_VHORNFRAC_MIN(),MAIN_GET_FATT_HP_VHORNFRAC_MAX(),MAIN_GET_FATT_HP_VHORNFRAC_MEAN(),
			MAIN_GET_FATT_HP_VHORNFRAC_MEDIAN(),MAIN_GET_FATT_HP_VHORNFRAC_SENT(),MAIN_GET_FATT_HP_VHORNFRAC_RENT(),
			MAIN_GET_FATT_HP_VHORNFRAC_MENT(),MAIN_GET_FATT_HP_VHORNFRAC_STDDEV(),MAIN_GET_FATT_HP_VHORNFRAC_PGFA(),
			MAIN_GET_FATT_HP_VHORNFRAC_PGFB(),MAIN_GET_FATT_HP_VHORNFRAC_PGFC());
	printf("c     ATTRIBUTOR:   HP_CHORNDIST[MIN %f MAX %f MEAN %f MEDIAN %f SENT %f RENT %f MENT %f STDDEV %f PGFA %f PGFB %f PGFC %f]\n",
			MAIN_GET_FATT_HP_CHORNDIST_MIN(),MAIN_GET_FATT_HP_CHORNDIST_MAX(),MAIN_GET_FATT_HP_CHORNDIST_MEAN(),
			MAIN_GET_FATT_HP_CHORNDIST_MEDIAN(),MAIN_GET_FATT_HP_CHORNDIST_SENT(),MAIN_GET_FATT_HP_CHORNDIST_RENT(),
			MAIN_GET_FATT_HP_CHORNDIST_MENT(),MAIN_GET_FATT_HP_CHORNDIST_STDDEV(),MAIN_GET_FATT_HP_CHORNDIST_PGFA(),
			MAIN_GET_FATT_HP_CHORNDIST_PGFB(),MAIN_GET_FATT_HP_CHORNDIST_PGFC());
	fflush(stdout);
	#endif

	#ifdef COLLECTSTATS
	stats_attributor_time_HP += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}

void attributor_intern_compute_BIG_components(int32_t numElt){
	//We determine the number of components and the largest component diameter using BFS.
	if (numElt == 0){
		return;
	}

	variable *v;
	literal *l, *adjLit;
	int32_t i;
	uint32_t distance, lSpos, id, node;
	float_ty qNumElt = ZERO, bigNumComp,
			bigCompDiamMin = UINT32_MAX, bigCompDiamMax = ZERO, bigCompDiamMean = ZERO,
			bigCompDiamMed = ZERO, bigCompDiamStdDev = ZERO, bigCompDiamSent = ZERO, bigCompDiamRent = ZERO,
			bigCompDiamMent = ZERO, bigCompDiamPGFA = ZERO, bigCompDiamPGFB = ZERO, bigCompDiamPGFC = ZERO,
			bigCompSizeMin = UINT32_MAX, bigCompSizeMax = ZERO, bigCompSizeMean = ZERO,
			bigCompSizeMed = ZERO, bigCompSizeStdDev = ZERO, bigCompSizeSent = ZERO, bigCompSizeRent = ZERO,
			bigCompSizeMent = ZERO, bigCompSizePGFA = ZERO, bigCompSizePGFB = ZERO, bigCompSizePGFC = ZERO;

	//Initialize stacks and return right away if nothing is to be computed. The priority queues will have size numElt/2,
	//because that is the maximum number of components that our formula can have. A variable that appears in the formula
	//cannot be a component by itself (would be a single unit clause, but they have been removed already).
	vS_flush();
	lS_flush();
	attributor_intern_prioQa_resize(numElt/2);
	attributor_intern_prioQb_resize(numElt/2);

	bigNumComp = ZERO;

	//We grab all the nodes in the graph that have a non-empty adjacency list.
	for (i = 0; i < ATTRIBUTOR_GRAPH_GET_NUMLISTS_USED(); ++i){
		if (ATTRIBUTOR_GRAPH_GET_LIST_NUM_NUMUSED(i) > 0) {
			v = main_varData + i;
			l = main_litData + i;
			SET_LIT_S_POS(l, 0);
			SET_LIT_DSC(l, UINT32_MAX);
			SET_VAR_S_POS(v, 0);
			vS_push(v);
		}
	}

	//We start by constructing a component. The component will be stored in the literal stack.
	while (vS_leaf > 1){
		if (ATTRIBUTOR_IS_TIME_ELAPSED()){
			#ifdef VERBOSE_ATTRIBUTOR
			printf("c     ATTRIBUTOR:   TIMEOUT BIG-C.\n");
			#endif
			#ifdef COLLECTSTATS
			++stats_attributor_numTimeouts;
			#endif
			break;
		}
		//We start constructing the component by adding the root element (popping the last element from the variable stack)
		//to the literal stack. The literal stack will be the component in the end.
		v = vS_pop();
		bigNumComp = bigNumComp + ONE;
		distance = 0;
		l = main_litData + GET_VAR_ID(v);
		SET_LIT_DSC(l, distance);
		lS_flush();
		lS_push(l);
		lSpos = 1;

		//We will now move through the literal stack starting at position one and add all elements in the adjacency list for
		//this literal -- if the literal is not already in the literal stack.
		while (lSpos < lS_leaf){
			//Get the id of the element for this distance.
			l = lS_readAt(lSpos);
			++lSpos;
			id = GET_LIT_NUM_FROM_PTR(l);
			//Check if this literal belongs to a new, farther distance.
			if (GET_LIT_DSC(l) > distance){
				//That is the case, we now work on a block of nodes that is farther away. We increase the distance by one.
				++distance;
			}
			for (i = 0; i < ATTRIBUTOR_GRAPH_GET_LIST_NUM_NUMUSED(id); ++i){
				//Get the adjacent variable.
				node = ATTRIBUTOR_GRAPH_GET_LIST_NUM_ELTNUM(id, i);
				adjLit = main_litData + node;
				//Check if the adjacent variable has been found already.
				if (GET_LIT_S_POS(adjLit) == 0){
					//No, it is not in the stack, therefore we must add it now (with increased distance).
					SET_LIT_DSC(adjLit, distance+1);
					lS_push(adjLit);
					//The corresponding variable is part of the current component and can be removed from the variable stack.
					vS_delete(main_varData + node);
				}
			}
		}

		//We have now in the literal stack all the positive literals of the variables that belong to the current component.
		//The last element in this stack has the farthest distance to the initial root element. We will now pick this element
		//as the new root and restart from scratch.
		l = lS_pop();
		lS_flush();
		distance = 0;
		SET_LIT_DSC(l, distance);
		lS_push(l);
		lSpos = 1;

		while (lSpos < lS_leaf){
			//Get the id of the element for this distance.
			l = lS_readAt(lSpos);
			++lSpos;
			id = GET_LIT_NUM_FROM_PTR(l);
			//Check if this literal belongs to a new, farther distance.
			if (GET_LIT_DSC(l) > distance){
				//That is the case, we now work on a block of nodes that is farther away. We increase the distance by one.
				++distance;
			}
			for (i = 0; i < ATTRIBUTOR_GRAPH_GET_LIST_NUM_NUMUSED(id); ++i){
				//Get the adjacent variable.
				node = ATTRIBUTOR_GRAPH_GET_LIST_NUM_ELTNUM(id, i);
				adjLit = main_litData + node;
				//Check if the adjacent variable has been found already.
				if (GET_LIT_S_POS(adjLit) == 0){
					//No, it is not in the stack, therefore we must add it now (with increased distance).
					SET_LIT_DSC(adjLit, distance+1);
					lS_push(adjLit);
				}
			}
		}

		//We have now in the literal stack the component, the last element in it has the farthest distance from the second
		//root try. This gives us the diameter of the component. The component size is lSpos - 1.
		//Update the component size information.
		--lSpos;
		attributor_intern_prioQa_add(lSpos);
		if (lSpos < bigCompSizeMin) bigCompSizeMin = lSpos;
		if (lSpos > bigCompSizeMax) bigCompSizeMax = lSpos;
		bigCompSizeMean += lSpos;
		//Update the component diameter information.
		l = lS_pop();
		distance = GET_LIT_DSC(l);
		attributor_intern_prioQb_add(distance);
		if (distance < bigCompDiamMin) bigCompDiamMin = distance;
		if (distance > bigCompDiamMax) bigCompDiamMax = distance;
		bigCompDiamMean += distance;

		//Update the number of elements in the queues.
		++qNumElt;
	}

	//Update the number of components.
	MAIN_SET_FATT_BIG_NUMCOMP(bigNumComp);

	//Update the information for the component sizes.
	if (qNumElt > ZERO) {
		bigCompSizeMean = bigCompSizeMean / qNumElt;
		attributor_intern_prioQa_computeDescription(bigCompSizeMean,&bigCompSizeMed,&bigCompSizeStdDev,
				&bigCompSizeSent,&bigCompSizeRent,&bigCompSizeMent,
				&bigCompSizePGFA,&bigCompSizePGFB,&bigCompSizePGFC);
		MAIN_SET_FATT_BIG_VCOMPSIZES_MIN(bigCompSizeMin);
		MAIN_SET_FATT_BIG_VCOMPSIZES_MAX(bigCompSizeMax);
		MAIN_SET_FATT_BIG_VCOMPSIZES_MEAN(bigCompSizeMean);
		MAIN_SET_FATT_BIG_VCOMPSIZES_MEDIAN(bigCompSizeMed);
		MAIN_SET_FATT_BIG_VCOMPSIZES_STDDEV(bigCompSizeStdDev);
		MAIN_SET_FATT_BIG_VCOMPSIZES_SENT(bigCompSizeSent);
		MAIN_SET_FATT_BIG_VCOMPSIZES_RENT(bigCompSizeRent);
		MAIN_SET_FATT_BIG_VCOMPSIZES_MENT(bigCompSizeMent);
		MAIN_SET_FATT_BIG_VCOMPSIZES_PGFA(bigCompSizePGFA);
		MAIN_SET_FATT_BIG_VCOMPSIZES_PGFB(bigCompSizePGFB);
		MAIN_SET_FATT_BIG_VCOMPSIZES_PGFC(bigCompSizePGFC);
	}

	//Update the information for the component diameters.
	if (qNumElt > ZERO) {
		bigCompDiamMean = bigCompDiamMean / qNumElt;
		attributor_intern_prioQb_computeDescription(bigCompDiamMean,&bigCompDiamMed,&bigCompDiamStdDev,
				&bigCompDiamSent,&bigCompDiamRent,&bigCompDiamMent,
				&bigCompDiamPGFA,&bigCompDiamPGFB,&bigCompDiamPGFC);
		MAIN_SET_FATT_BIG_VCOMPDIAMS_MIN(bigCompDiamMin);
		MAIN_SET_FATT_BIG_VCOMPDIAMS_MAX(bigCompDiamMax);
		MAIN_SET_FATT_BIG_VCOMPDIAMS_MEAN(bigCompDiamMean);
		MAIN_SET_FATT_BIG_VCOMPDIAMS_MEDIAN(bigCompDiamMed);
		MAIN_SET_FATT_BIG_VCOMPDIAMS_STDDEV(bigCompDiamStdDev);
		MAIN_SET_FATT_BIG_VCOMPDIAMS_SENT(bigCompDiamSent);
		MAIN_SET_FATT_BIG_VCOMPDIAMS_RENT(bigCompDiamRent);
		MAIN_SET_FATT_BIG_VCOMPDIAMS_MENT(bigCompDiamMent);
		MAIN_SET_FATT_BIG_VCOMPDIAMS_PGFA(bigCompDiamPGFA);
		MAIN_SET_FATT_BIG_VCOMPDIAMS_PGFB(bigCompDiamPGFB);
		MAIN_SET_FATT_BIG_VCOMPDIAMS_PGFC(bigCompDiamPGFC);
	}

	//Finally, clean up the queues and stacks that have been used.
	vS_flush();
	lS_flush();
	attributor_intern_prioQa_resize(BLOCKSIZE);
	attributor_intern_prioQb_resize(BLOCKSIZE);
}

float_ty attributor_intern_compute_BIG_articulationPoints(int32_t numElt){
	//This method computes the articulation points of the graph assuming that all the elements in the graph are variable IDs.
	if (numElt == ZERO){
		return ZERO;
	}

	float_ty result = ZERO;
	uint32_t vNum, childNode, rootChildren;
	nrdfsStackNode *sn;

	//First, we create an array containing all our variables as stack-nodes. We reset them all.
	nrdfsStackNode* theNodes = NULL;
	theNodes = malloc(sizeof(nrdfsStackNode)*(f.n_initial+2));
	if (theNodes == NULL){
		printf("c ERROR. The theNodes in the BIG articulation point computation could not be allocated. Out of memory?\n");
		attributor_returnCode = ATTRIBUTOR_ERROR;
		return ZERO;
	}
	for (vNum = 0; vNum < f.n_initial+2; ++vNum){
		//We reset all its data.
		theNodes[vNum].numChildren = INT32_MAX;
		theNodes[vNum].node = vNum;
		theNodes[vNum].parent = INT32_MAX;
		theNodes[vNum].depth = INT32_MAX;
		theNodes[vNum].reach = INT32_MAX;
	}

	//Second, we resize the stack that simulates the depth first search. We use half the number of variables as a wild guess.
	attributor_intern_nrdfsStack_resize((uint32_t)(numElt/TWO));


	//Now that we have prepared everything, we can start our depth first search for each component root.
	for (vNum = 1; vNum < f.n_initial+1; ++vNum){
		if (ATTRIBUTOR_IS_TIME_ELAPSED()){
			#ifdef VERBOSE_ATTRIBUTOR
			printf("c     ATTRIBUTOR:   TIMEOUT BIG-AP.\n");
			#endif
			#ifdef COLLECTSTATS
			++stats_attributor_numTimeouts;
			#endif
			break;
		}

		if (IS_VAR_DISABLED((main_varData + vNum)) || ATTRIBUTOR_GRAPH_GET_LIST_NUM_NUMUSED(vNum) == 0) continue;

		//Check if the current node was never checked. If so, it is a new root.
		if (theNodes[vNum].numChildren == INT32_MAX){
			//Yes, we construct a new root node here and perform the non-recursive depth first search on it.
			theNodes[vNum].depth = 0;
			attributor_intern_nrdfsStack_push(theNodes + vNum);
			rootChildren = 0;
			//We now perform the non-recursive depth first search.
			while (attributor_nrdfsStack_leaf > 0){
				sn = attributor_intern_nrdfsStack_read();
				//We separate into three cases.
				if (sn->numChildren == INT32_MAX){
					//1. We work on this node for the very first time.
					//Update the nodes depth and reach information.
					theNodes[sn->node].depth = sn->depth;
					sn->reach = sn->depth;
					//Update the nodes child information.
					sn->numChildren = ATTRIBUTOR_GRAPH_GET_LIST_NUM_NUMUSED(sn->node);
				} else if (sn->numChildren > 0){
					//2. We are still working through the children of this node. Grab the last children in the nodes queue.
					//and remove it.
					childNode = ATTRIBUTOR_GRAPH_GET_LIST_NUM_ELTNUM(sn->node, --sn->numChildren);
					//Check if this child is the parent node of sn. If so, ignore it.
					if (childNode != sn->parent) {
						//Check if we have already visited this child.
						if (theNodes[childNode].depth != INT32_MAX){
							//Yes, we have already visited this child. We update the reach information of the node.
							if (theNodes[childNode].depth < sn->reach){
								sn->reach = theNodes[childNode].depth;
							}
						} else {
							//No, we must visit the child first. We put it on the stack.
							theNodes[childNode].parent = sn->node;
							theNodes[childNode].depth = sn->depth+1;

							//If we are working on the root node, we must keep track of the number of its children.
							if (attributor_nrdfsStack_leaf == 1){
								++rootChildren;
							}

							attributor_intern_nrdfsStack_push(theNodes + childNode);
						}
					}
				} else {
					//3. We are finishing this node.
					if (theNodes[sn->node].depth > 1){
						if (sn->reach >= theNodes[sn->parent].depth) {
							++result;
						}
						if (sn->reach < theNodes[sn->parent].reach){
							theNodes[sn->parent].reach = sn->reach;
						}
					}
					attributor_intern_nrdfsStack_remove();
				}
			}
			//Finally, if the root had more than one child, we must mark it as an articulation point as well.
			if (rootChildren > 1){
				++result;
			}
		}
	}

	//Finally, we clean up all the temporarily allocated data.
	free(theNodes);
	attributor_intern_nrdfsStack_resize(BLOCKSIZE);
	return result;
}

void attributor_intern_compute_BIG_allAttributes(){
	//Computes the binary implication graph attributes. This includes the variable node degree for all variables, the number
	//of components of the BIG, as well as the sizes of the components and their diameters.
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	#endif

	ATTRIBUTOR_SET_START_TIME_IN_SEC();

	literal *l;
	float_ty vFrac = ZERO;
	float_ty vDegree, vDegreeMin = UINT32_MAX, vDegreeMax = ZERO, vDegreeMean = ZERO, numElt = ZERO;
	float_ty vDegreeMedian = ZERO, vDegreeStdDev = ZERO, vDegreeSent = ZERO, vDegreeRent = ZERO, vDegreeMent = ZERO,
			vDegreePGFA = ZERO, vDegreePGFB = ZERO, vDegreePGFC = ZERO;
	int32_t i, vNum, impID;

	//First, we will create the BIG using the implication lists of the literals and the literal stamps. Each literal will be
	//transformed into a variable. Each variable will be present only once.
	ATTRIBUTOR_GRAHP_ENSURE_LIST_SIZE( f.n_initial);
	ATTRIBUTOR_GRAPH_RESET_ALL_LISTS( i );
	attributor_intern_prioQa_resize(f.n_initial);
	for (vNum = 1; vNum < f.n_initial+1; ++vNum){
		if (ATTRIBUTOR_IS_TIME_ELAPSED()){
			#ifdef VERBOSE_ATTRIBUTOR
			printf("c     ATTRIBUTOR:   TIMEOUT BIG-V.\n");
			#endif
			#ifdef COLLECTSTATS
			++stats_attributor_numTimeouts;
			#endif
			break;
		}
		//Ignore disabled variables.
		if (IS_VAR_DISABLED((main_varData + vNum))) continue;
		//We collect all the literals as IDs that are implied by the positive literal of vNum. We stamp all the literal
		//abstracts with the new stamp in order to detect that the literal has been selected.
		++main_litStamp;
		//Check the positive literals implication list.
		l = main_litData + vNum;
		for (i = 0; i < GET_LIT_NUMIMP_USED(l); i+=2){
			//Grab the implied literal abstract and check if it has been collected already. If so, ignore it.
			impID = abs(GET_LIT_IMPNUM(l, i));
			if (GET_LIT_NUM_STAMP(impID) == main_litStamp) continue;
			//The literal has not yet been stamped. We collect it.
			ATTRIBUTOR_GRAPH_ADD_LIST_NUM_ELT(vNum, impID);
			SET_LIT_NUM_STAMP(impID, main_litStamp);
		}
		//Check the negative literals implication list.
		l = main_litData - vNum;
		for (i = 0; i < GET_LIT_NUMIMP_USED(l); i+=2){
			//Grab the implied literal abstract and check if it has been collected already. If so, ignore it.
			impID = abs(GET_LIT_IMPNUM(l,i));
			if (GET_LIT_NUM_STAMP(impID) == main_litStamp) continue;
			//The literal has not yet been stamped. We collect it.
			ATTRIBUTOR_GRAPH_ADD_LIST_NUM_ELT(vNum, impID);
			SET_LIT_NUM_STAMP(impID, main_litStamp);
		}
		//We have now collected all implications that vNum has as either positive or negative literal. We therefore have the
		//degree for vNum available now and add it to the priority queue and update some information.
		vDegree = ATTRIBUTOR_GRAPH_GET_LIST_NUM_NUMUSED(vNum);
		if (vDegree > ZERO) ++vFrac;
		if (vDegree < vDegreeMin) vDegreeMin = vDegree;
		if (vDegree > vDegreeMax) vDegreeMax = vDegree;
		vDegreeMean += vDegree;
		attributor_intern_prioQa_add(vDegree);
		++numElt;
	}
	//We have now created the binary implication graph for the variables. We will now update the information regarding it.
	vFrac = vFrac / ((float_ty) f.n_initial);
	MAIN_SET_FATT_BIG_VFRAC(vFrac);
	if (numElt > ZERO){
		vDegreeMean = vDegreeMean / numElt;
		attributor_intern_prioQa_computeDescription(vDegreeMean,&vDegreeMedian,&vDegreeStdDev,
				&vDegreeSent,&vDegreeRent,&vDegreeMent,
				&vDegreePGFA,&vDegreePGFB,&vDegreePGFC);
		//Write the degree information.
		MAIN_SET_FATT_BIG_VNODEDEGREE_MIN(vDegreeMin);
		MAIN_SET_FATT_BIG_VNODEDEGREE_MAX(vDegreeMax);
		MAIN_SET_FATT_BIG_VNODEDEGREE_MEAN(vDegreeMean);
		MAIN_SET_FATT_BIG_VNODEDEGREE_MEDIAN(vDegreeMedian);
		MAIN_SET_FATT_BIG_VNODEDEGREE_STDDEV(vDegreeStdDev);
		MAIN_SET_FATT_BIG_VNODEDEGREE_SENT(vDegreeSent);
		MAIN_SET_FATT_BIG_VNODEDEGREE_RENT(vDegreeRent);
		MAIN_SET_FATT_BIG_VNODEDEGREE_MENT(vDegreeMent);
		MAIN_SET_FATT_BIG_VNODEDEGREE_PGFA(vDegreePGFA);
		MAIN_SET_FATT_BIG_VNODEDEGREE_PGFB(vDegreePGFB);
		MAIN_SET_FATT_BIG_VNODEDEGREE_PGFC(vDegreePGFC);
	}

	//We must now compute the number of components and the diameters of the components of the BIG.

	//Clean up.
	++main_litStamp;
	attributor_intern_prioQa_resize(BLOCKSIZE);

	if (!ATTRIBUTOR_IS_TIME_ELAPSED()){
		//Based on the constructed binary implication graph, we compute the attributes of the components.
		attributor_intern_compute_BIG_components((int32_t)numElt);
	}

	if (!ATTRIBUTOR_IS_TIME_ELAPSED()){
		//Finally, we can compute the number of articulation points for the binary implication graph.
		float_ty numAP = attributor_intern_compute_BIG_articulationPoints((int32_t)numElt);
		MAIN_SET_FATT_BIG_NUMAP(numAP);
	}

	#ifdef VERBOSE_ATTRIBUTOR
	printf("c     ATTRIBUTOR:   BIG_VNODEDEGREE[MIN %f MAX %f MEAN %f MEDIAN %f SENT %f RENT %f MENT %f STDDEV %f PGFA %f PGFB %f PGFC %f]\n",
			MAIN_GET_FATT_BIG_VNODEDEGREE_MIN(),MAIN_GET_FATT_BIG_VNODEDEGREE_MAX(),MAIN_GET_FATT_BIG_VNODEDEGREE_MEAN(),
			MAIN_GET_FATT_BIG_VNODEDEGREE_MEDIAN(),MAIN_GET_FATT_BIG_VNODEDEGREE_SENT(),MAIN_GET_FATT_BIG_VNODEDEGREE_RENT(),
			MAIN_GET_FATT_BIG_VNODEDEGREE_MENT(),MAIN_GET_FATT_BIG_VNODEDEGREE_STDDEV(),MAIN_GET_FATT_BIG_VNODEDEGREE_PGFA(),
			MAIN_GET_FATT_BIG_VNODEDEGREE_PGFB(),MAIN_GET_FATT_BIG_VNODEDEGREE_PGFC());
	printf("c     ATTRIBUTOR:   BIG_VFRAC[%f]\n", MAIN_GET_FATT_BIG_VFRAC());
	printf("c     ATTRIBUTOR:   BIG_NUMCOMP[%f]\n", MAIN_GET_FATT_BIG_NUMCOMP());
	printf("c     ATTRIBUTOR:   BIG_VCOMPSIZES[MIN %f MAX %f MEAN %f MEDIAN %f SENT %f RENT %f MENT %f STDDEV %f PGFA %f PGFB %f PGFC %f]\n",
			MAIN_GET_FATT_BIG_VCOMPSIZES_MIN(),MAIN_GET_FATT_BIG_VCOMPSIZES_MAX(),MAIN_GET_FATT_BIG_VCOMPSIZES_MEAN(),
			MAIN_GET_FATT_BIG_VCOMPSIZES_MEDIAN(),MAIN_GET_FATT_BIG_VCOMPSIZES_SENT(),MAIN_GET_FATT_BIG_VCOMPSIZES_RENT(),
			MAIN_GET_FATT_BIG_VCOMPSIZES_MENT(),MAIN_GET_FATT_BIG_VCOMPSIZES_STDDEV(),MAIN_GET_FATT_BIG_VCOMPSIZES_PGFA(),
			MAIN_GET_FATT_BIG_VCOMPSIZES_PGFB(),MAIN_GET_FATT_BIG_VCOMPSIZES_PGFC());
	printf("c     ATTRIBUTOR:   BIG_VCOMPDIAMS[MIN %f MAX %f MEAN %f MEDIAN %f SENT %f RENT %f MENT %f STDDEV %f PGFA %f PGFB %f PGFC %f]\n",
			MAIN_GET_FATT_BIG_VCOMPDIAMS_MIN(),MAIN_GET_FATT_BIG_VCOMPDIAMS_MAX(),MAIN_GET_FATT_BIG_VCOMPDIAMS_MEAN(),
			MAIN_GET_FATT_BIG_VCOMPDIAMS_MEDIAN(),MAIN_GET_FATT_BIG_VCOMPDIAMS_SENT(),MAIN_GET_FATT_BIG_VCOMPDIAMS_RENT(),
			MAIN_GET_FATT_BIG_VCOMPDIAMS_MENT(),MAIN_GET_FATT_BIG_VCOMPDIAMS_STDDEV(),MAIN_GET_FATT_BIG_VCOMPDIAMS_PGFA(),
			MAIN_GET_FATT_BIG_VCOMPDIAMS_PGFB(),MAIN_GET_FATT_BIG_VCOMPDIAMS_PGFC());
	printf("c     ATTRIBUTOR:   BIG_NUMAP[%f]\n", MAIN_GET_FATT_BIG_NUMAP());
	fflush(stdout);
	#endif
	#ifdef COLLECTSTATS
	stats_attributor_time_BIG += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}

void attributor_intern_compute_UP_allAttributes(){
	//Computes the UP attributes. This includes the fraction of literals that, when satisfied, make UP run into a conflict.
	//It includes the count of additional unit propagations when satisfying a literal.

	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	#endif

	ATTRIBUTOR_SET_START_TIME_IN_SEC();

	variable *v;
	literal *lit;
	clause *c;
	float_ty posLitConfFrac = ZERO, negLitConfFrac = ZERO, numEltA = ZERO, numEltB = ZERO;
	float_ty posUPCount, posUPCountMin = UINT32_MAX, posUPCountMax = ZERO, posUPCountMean = ZERO,
			 posUPCountMedian = ZERO, posUPCountStdDev = ZERO, posUPCountSent = ZERO, posUPCountRent = ZERO,
			 posUPCountMent = ZERO, posUPCountPGFA = ZERO, posUPCountPGFB = ZERO, posUPCountPGFC = ZERO;
	float_ty negUPCount, negUPCountMin = UINT32_MAX, negUPCountMax = ZERO, negUPCountMean = ZERO,
			 negUPCountMedian = ZERO, negUPCountStdDev = ZERO, negUPCountSent = ZERO, negUPCountRent = ZERO,
			 negUPCountMent = ZERO, negUPCountPGFA = ZERO, negUPCountPGFB = ZERO, negUPCountPGFC = ZERO;
	int32_t vNum, numPropagated;

	//First, we propagate all literals with non-empty implication lists.
	if (f.n_initial <= param_classifyMaxUPn){
		attributor_intern_prioQa_resize(f.n_initial);
		attributor_intern_prioQb_resize(f.n_initial);
		for (vNum = 1; vNum < f.n_initial+1; ++vNum){
			if (ATTRIBUTOR_IS_TIME_ELAPSED()){
				#ifdef VERBOSE_ATTRIBUTOR
				printf("c     ATTRIBUTOR:   TIMEOUT UP-V.\n");
				#endif
				#ifdef COLLECTSTATS
				++stats_attributor_numTimeouts;
				#endif
				break;
			}
			//Grab the variable that can be propagated. Ignore it if it is already disabled.
			v = main_varData + vNum;
			if (IS_VAR_DISABLED(v) || !IS_VAR_UNASSIGNED(v)) continue;
			//Check if the corresponding positive literal has a non-empty implication list.
			lit = main_litData + vNum;
			if (GET_LIT_NUMIMP_USED(lit) > 0 && GET_LIT_NUMIMP_USED(lit) < param_classifyMaxUPImps){
				//Yes, it has a non-empty implication list. We can propagate it temporarily.
				ADD_LIT_TO_SIMPLEUPQUEUE_PTR(lit);
				c = main_simpleUP_propagate_temporary(&numPropagated, 2, 0, 0, NULL);
				//Update the information on how many literals where propagated and if a conflict occurred.
				if (c != NULL){
					//A conflict occurred, we increase the posLitConfFrac counter.
					++posLitConfFrac;
					//If the positive literal propagation results in a conflict, vNum is a failed literal. Propagate -vNum.
					classify_intern_simplify_addLitToEnforce(-vNum);
				}
				posUPCount = numPropagated;
				if (posUPCount < posUPCountMin) posUPCountMin = posUPCount;
				if (posUPCount > posUPCountMax) posUPCountMax = posUPCount;
				posUPCountMean += posUPCount;
				attributor_intern_prioQa_add(posUPCount);
				++numEltA;
			}

			//Check if the corresponding negative literal has a non-empty implication list.
			lit = main_litData - vNum;
			if (GET_LIT_NUMIMP_USED(lit) > 0 && GET_LIT_NUMIMP_USED(lit) < param_classifyMaxUPImps){
				//Yes, it has a non-empty implication list. We can propagate it temporarily.
				ADD_LIT_TO_SIMPLEUPQUEUE_PTR(lit);
				c = main_simpleUP_propagate_temporary(&numPropagated, 2, 0, 0, NULL);
				//Update the information on how many literals where propagated and if a conflict occurred.
				if (c != NULL){
					//A conflict occurred, we increase the negLitConfFrac counter.
					++negLitConfFrac;
					//If the negative literal propagation results in a conflict, -vNum is a failed literal. Propagate vNum.
					classify_intern_simplify_addLitToEnforce(vNum);
				}
				negUPCount = numPropagated;
				if (negUPCount < negUPCountMin) negUPCountMin = negUPCount;
				if (negUPCount > negUPCountMax) negUPCountMax = negUPCount;
				negUPCountMean += negUPCount;
				attributor_intern_prioQb_add(negUPCount);
				++numEltB;
			}
		}

		//We have now computed how many literals can be propagated when satisfying a positive literal.
		if (numEltA > ZERO){
			posLitConfFrac = posLitConfFrac / numEltA;
			posUPCountMean = posUPCountMean / numEltA;
			attributor_intern_prioQa_computeDescription(posUPCountMean,&posUPCountMedian,&posUPCountStdDev,
					&posUPCountSent,&posUPCountRent,&posUPCountMent,
					&posUPCountPGFA,&posUPCountPGFB,&posUPCountPGFC);
			MAIN_SET_FATT_UP_POSLITCONFFRAC(posLitConfFrac);
			MAIN_SET_FATT_UP_POSLITUPCOUNT_MIN(posUPCountMin);
			MAIN_SET_FATT_UP_POSLITUPCOUNT_MAX(posUPCountMax);
			MAIN_SET_FATT_UP_POSLITUPCOUNT_MEAN(posUPCountMean);
			MAIN_SET_FATT_UP_POSLITUPCOUNT_MEDIAN(posUPCountMedian);
			MAIN_SET_FATT_UP_POSLITUPCOUNT_STDDEV(posUPCountStdDev);
			MAIN_SET_FATT_UP_POSLITUPCOUNT_SENT(posUPCountSent);
			MAIN_SET_FATT_UP_POSLITUPCOUNT_RENT(posUPCountRent);
			MAIN_SET_FATT_UP_POSLITUPCOUNT_MENT(posUPCountMent);
			MAIN_SET_FATT_UP_POSLITUPCOUNT_PGFA(posUPCountPGFA);
			MAIN_SET_FATT_UP_POSLITUPCOUNT_PGFB(posUPCountPGFB);
			MAIN_SET_FATT_UP_POSLITUPCOUNT_PGFC(posUPCountPGFC);
		}

		//We have now computed how many literals can be propagated when satisfying a negative literal.
		if (numEltB > ZERO){
			negLitConfFrac = negLitConfFrac / numEltB;
			negUPCountMean = negUPCountMean / numEltB;
			attributor_intern_prioQb_computeDescription(negUPCountMean,&negUPCountMedian,&negUPCountStdDev,
					&negUPCountSent,&negUPCountRent,&negUPCountMent,
					&negUPCountPGFA,&negUPCountPGFB,&negUPCountPGFC);
			MAIN_SET_FATT_UP_NEGLITCONFFRAC(negLitConfFrac);
			MAIN_SET_FATT_UP_NEGLITUPCOUNT_MIN(negUPCountMin);
			MAIN_SET_FATT_UP_NEGLITUPCOUNT_MAX(negUPCountMax);
			MAIN_SET_FATT_UP_NEGLITUPCOUNT_MEAN(negUPCountMean);
			MAIN_SET_FATT_UP_NEGLITUPCOUNT_MEDIAN(negUPCountMedian);
			MAIN_SET_FATT_UP_NEGLITUPCOUNT_STDDEV(negUPCountStdDev);
			MAIN_SET_FATT_UP_NEGLITUPCOUNT_SENT(negUPCountSent);
			MAIN_SET_FATT_UP_NEGLITUPCOUNT_RENT(negUPCountRent);
			MAIN_SET_FATT_UP_NEGLITUPCOUNT_MENT(negUPCountMent);
			MAIN_SET_FATT_UP_NEGLITUPCOUNT_PGFA(negUPCountPGFA);
			MAIN_SET_FATT_UP_NEGLITUPCOUNT_PGFB(negUPCountPGFB);
			MAIN_SET_FATT_UP_NEGLITUPCOUNT_PGFC(negUPCountPGFC);
		}

		//Clean up.
		attributor_intern_prioQa_resize(BLOCKSIZE);
		attributor_intern_prioQb_resize(BLOCKSIZE);
	}
	#ifdef COLLECTSTATS
	else {
		++stats_attributor_numSkipped;
	}
	#endif

	#ifdef VERBOSE_ATTRIBUTOR
	printf("c     ATTRIBUTOR:   UP_POSLITCONFFRAC[%f] UP_NEGLITCONFFRAC[%f]\n",
			MAIN_GET_FATT_UP_POSLITCONFFRAC(), MAIN_GET_FATT_UP_NEGLITCONFFRAC());
	printf("c     ATTRIBUTOR:   UP_POSLITUPCOUNT[MIN %f MAX %f MEAN %f MEDIAN %f SENT %f RENT %f MENT %f STDDEV %f PGFA %f PGFB %f PGFC %f]\n",
			MAIN_GET_FATT_UP_POSLITUPCOUNT_MIN(),MAIN_GET_FATT_UP_POSLITUPCOUNT_MAX(),MAIN_GET_FATT_UP_POSLITUPCOUNT_MEAN(),
			MAIN_GET_FATT_UP_POSLITUPCOUNT_MEDIAN(),MAIN_GET_FATT_UP_POSLITUPCOUNT_SENT(),
			MAIN_GET_FATT_UP_POSLITUPCOUNT_RENT(),MAIN_GET_FATT_UP_POSLITUPCOUNT_MENT(),MAIN_GET_FATT_UP_POSLITUPCOUNT_STDDEV(),
			MAIN_GET_FATT_UP_POSLITUPCOUNT_PGFA(),MAIN_GET_FATT_UP_POSLITUPCOUNT_PGFB(),MAIN_GET_FATT_UP_POSLITUPCOUNT_PGFC());
	printf("c     ATTRIBUTOR:   UP_NEGLITUPCOUNT[MIN %f MAX %f MEAN %f MEDIAN %f SENT %f RENT %f MENT %f STDDEV %f PGFA %f PGFB %f PGFC %f]\n",
			MAIN_GET_FATT_UP_NEGLITUPCOUNT_MIN(),MAIN_GET_FATT_UP_NEGLITUPCOUNT_MAX(),MAIN_GET_FATT_UP_NEGLITUPCOUNT_MEAN(),
			MAIN_GET_FATT_UP_NEGLITUPCOUNT_MEDIAN(),MAIN_GET_FATT_UP_NEGLITUPCOUNT_SENT(),
			MAIN_GET_FATT_UP_NEGLITUPCOUNT_RENT(),MAIN_GET_FATT_UP_NEGLITUPCOUNT_MENT(),MAIN_GET_FATT_UP_NEGLITUPCOUNT_STDDEV(),
			MAIN_GET_FATT_UP_NEGLITUPCOUNT_PGFA(),MAIN_GET_FATT_UP_NEGLITUPCOUNT_PGFB(),MAIN_GET_FATT_UP_NEGLITUPCOUNT_PGFC());
	fflush(stdout);
	#endif

	#ifdef COLLECTSTATS
	stats_attributor_time_UP += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}

void attributor_intern_compute_RW_allAttributes(){
	//Computes the recursive weights for all literals and the total weight for the variables according to the recursive weight
	//heuristic by Mjinders et. al. It computes the quantiles of the variable weight distribution and the basic information
	//of the weight distribution itself.

	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	#endif

	ATTRIBUTOR_SET_START_TIME_IN_SEC();

	literal *l;
	clause *c;
	float_ty clsResult, fValA, fValB, numElt = ZERO;
	float_ty vWeightTotal = ZERO, vWeight, vWeightMin = UINT32_MAX, vWeightMax = ZERO, vWeightMean = ZERO,
			 vWeightMedian = ZERO, vWeightStdDev = ZERO, vWeightSent = ZERO, vWeightRent = ZERO, vWeightMent = ZERO,
			 vWeightPGFA = ZERO, vWeightPGFB = ZERO, vWeightPGFC = ZERO;
	float_ty q05 = ZERO, q10 = ZERO, q25 = ZERO, q50 = ZERO, q75 = ZERO, q90 = ZERO, q95 = ZERO;
	int32_t i,j, vNum, maxCSize;

	//We initialize two float arrays to host the literal scores. We then use two pointers to switch between them to distinct
	//between the values of the current iteration and the previous one. In each iteration that has an odd number, the rw_a
	//array will be the previous one, and the rw_b array will be the current one. In iterations with even numbers, it is the
	//other way around.
	float_ty* rw_a = NULL;
	float_ty* rw_b = NULL;
	float_ty* rw_curr = NULL;
	float_ty* rw_prev = NULL;

	rw_a = malloc(sizeof(float_ty) * (2*f.n_initial+1));
	if (rw_a == NULL){
		printf("c ERROR. Could not allocate the rw_a array in ATTRIBUTOR. Out of memory?\n");
		attributor_returnCode = ATTRIBUTOR_ERROR;
		return;
	}

	rw_b = malloc(sizeof(float_ty) * (2*f.n_initial+1));
	if (rw_b == NULL){
		printf("c ERROR. Could not allocate the rw_b array in ATTRIBUTOR. Out of memory?\n");
		attributor_returnCode = ATTRIBUTOR_ERROR;
		return;
	}

	//We initialize the rw_a array to contain all ones.
	for (i = 0; i < 2*f.n_initial+1; ++i){
		rw_a[i] = ONE;
	}

	//Additionally, we need an array to store scores based on the size of the clause. We use the value in the position for
	//MAIN_GET_FATT_VCG_CNODEDEGREE_MAX in order to check on the maximum clause size. In case this value is zero, we do the
	//fall-back and use f.n_initial which is certainly way too large but will still work. In most cases, this will result in
	//the smallest array possible (if the VCG values have been previously computed -- and they should).
	maxCSize = (int32_t)MAIN_GET_FATT_VCG_CNODEDEGREE_MAX();
	if (maxCSize == 0 || maxCSize > 7){
		maxCSize = 7;
	}
	float_ty* rw_l = NULL;
	rw_l = malloc(sizeof(float_ty) * (maxCSize+1));
	if (rw_l == NULL) {
		printf("c ERROR. Could not allocate the rw_l array in ATTRIBUTOR. Out of memory?\n");
		attributor_returnCode = ATTRIBUTOR_ERROR;
		return;
	}

	//We furthermore need the rw_s, that will be used for scaling.
	float_ty rw_s;

	//We start in iteration 1.
	uint32_t i_it = 1;

	//We will now compute the recursive weight scores.
	while (i_it <= 5){
		if (ATTRIBUTOR_IS_TIME_ELAPSED()){
			#ifdef VERBOSE_ATTRIBUTOR
			printf("c     ATTRIBUTOR:   TIMEOUT RW-I.\n");
			#endif
			#ifdef COLLECTSTATS
			++stats_attributor_numTimeouts;
			#endif
			break;
		}
		//Check which array is the current and which is the previous one.
		if (i_it % 2 == 1){
			//Odd cycle means the rw_a array is the previous one.
			rw_prev = rw_a + f.n_initial;	//These values are the given ones.
			rw_curr = rw_b + f.n_initial;	//These values are about to be computed.
		} else {
			//Even cycle means the rw_b array is the previous one.
			rw_prev = rw_b + f.n_initial;	//These values are the given ones.
			rw_curr = rw_a + f.n_initial;	//These values are about to be computed.
		}
		//We compute the rw_s value for the current round.
		rw_s = ZERO;
		for (vNum = 1; vNum < f.n_initial+1; ++vNum){
			rw_s += rw_prev[vNum] + rw_prev[-vNum];
		}
		rw_s = rw_s / ((float_ty)(2*f.n_initial));

		//Based on the rw_s and the previous scores in rw_prev[lit], we can compute the current scores in rw_curr[lit].
		for (vNum = 1; vNum < f.n_initial+1; ++vNum){
			//For each variable, we compute for both its literals the following stuff.
			//First, we work on the positive literal. Reset the scores for the clause size value.
			for (i = 1; i < maxCSize+1; ++i){
				rw_l[i] = ZERO;
			}
			l = main_litData + vNum;
			//For all occurrences of literal l (equals vNum now).
			for (i = 0; i < GET_LIT_NUMOCCS_USED(l); ++i){
				//Grab the clause and reset its clause score.
				c = GET_LIT_OCCNUM(l, i);
				if (GET_CLS_SIZE(c) < 8){
					clsResult = ONE;
					//For all literals in the clause.
					for (j = 0; j < GET_CLS_SIZE(c); ++j){
						//Ignore the literal we are working on.
						if (GET_CLS_LITNUM(c,j) == vNum) continue;
						clsResult *= rw_prev[-GET_CLS_LITNUM(c,j)];
					}
					//We have now computed the score for the clause. We add it to the score of all clauses with that same size.
					rw_l[GET_CLS_SIZE(c)] += clsResult;
				}
			}

			//We have now computed the score of literal vNum for all the clause sizes. We will now normalize the score for each
			//clause size. The normalization factor for the score of clauses of size i is maxCSize^(maxCSize-i)/rw_s^(i-1).
			//We then add the normalized score for the specific length to the current score of the literal.
			rw_curr[vNum] = ZERO;
			for (i = 1; i < maxCSize+1; ++i){
				fValA = pow((float_ty)maxCSize, (float_ty)(maxCSize - i));
				fValB = pow(rw_s, (float_ty)(i-1));
				//Check if a division gives a problem because of small numbers.
				if (fValB > ZERO){
					//No problem, fValB is large enough.
					rw_l[i] *= (fValA / fValB);
				} else {
					//Thats bad, fValB is so small that it appears as zero. We simply divide rw_l[i] with a small float.
					rw_l[i] *= (fValA / 0.00001);
				}
				rw_curr[vNum] += rw_l[i];
			}
			//We are now done with the positive literal vNum.

			//Second, we work on the negative literal. Reset the scores for the clause size values.
			for (i = 0; i < maxCSize+1; ++i){
				rw_l[i] = ZERO;
			}
			l = main_litData - vNum;
			//For all occurrences of literal l (equals -vNum now).
			for (i = 0; i < GET_LIT_NUMOCCS_USED(l); ++i){
				//Grab the clause and reset its clause score.
				c = GET_LIT_OCCNUM(l, i);
				clsResult = ONE;
				if (GET_CLS_SIZE(c) < 8){
					//For all literals in the clause.
					for (j = 0; j < GET_CLS_SIZE(c); ++j){
						//Ignore the literal we are working on.
						if (GET_CLS_LITNUM(c,j) == -vNum) continue;
						clsResult *= rw_prev[-GET_CLS_LITNUM(c,j)];
					}
					//We have now computed the score for the clause. We add it to the score of all clauses with that same size.
					rw_l[GET_CLS_SIZE(c)] += clsResult;
				}
			}
			//We have now computed the score of literal -vNum for all the clause sizes. We will now normalize the score for
			//each clause size. The normalization factor for the score of clauses of size i is maxCSize^(maxCSize-i)/rw_s^(i-1).
			//We then add the normalized score for the specific length to the current score of the literal.
			rw_curr[-vNum] = ZERO;
			for (i = 1; i < maxCSize+1; ++i){
				fValA = pow((float_ty)maxCSize, (float_ty)(maxCSize - i));
				fValB = pow(rw_s, (float_ty)(i-1));
				//Check if a division gives a problem because of small numbers.
				if (fValB > ZERO){
					//No problem, fValB is large enough.
					rw_l[i] *= (fValA / fValB);
				} else {
					//Thats bad, fValB is so small that it appears as zero. We simply divide rw_l[i] with a small float.
					rw_l[i] *= (fValA / 0.00001);
				}
				rw_curr[-vNum] += rw_l[i];
			}
			//We are now done with the negative literal -vNum.

			//We are now done with the variable in this iteration. We go on with the next.
		}
		//Go on with the next iteration.
		++i_it;
	}

	//We have now computed the literal scores of of all the literals. We will now first compute the quantiles based on the
	//weight of every variable.
	attributor_intern_prioQa_resize(f.n_initial);
	numElt = ZERO;
	for (vNum = 1; vNum < f.n_initial+1; ++vNum){
		//Grab the variable weight.
		vWeight = rw_curr[vNum] + rw_curr[-vNum];
		attributor_intern_prioQa_add(vWeight);
		vWeightTotal += vWeight;
		++numElt;
	}

	//We have now computed the weights for all variables. We will use the first priority queue to compute the quantile values
	//and the second priority queue to compute the distribution information and the entropy.
	if (numElt > ZERO){
		attributor_intern_prioQa_computeQuantiles(vWeightTotal,&q05,&q10,&q25,&q50,&q75,&q90,&q95);
		MAIN_SET_FATT_RW_Q05(q05);
		MAIN_SET_FATT_RW_Q10(q10);
		MAIN_SET_FATT_RW_Q25(q25);
		MAIN_SET_FATT_RW_Q50(q50);
		MAIN_SET_FATT_RW_Q75(q75);
		MAIN_SET_FATT_RW_Q90(q90);
		MAIN_SET_FATT_RW_Q95(q95);
	}

	//We will again use the first priority queue to compute the weight mean and median and standard deviation information.
	numElt = ZERO;
	for (vNum = 1; vNum < f.n_initial+1; ++vNum){
		//Grab the variable weight.
		vWeight = rw_curr[vNum] + rw_curr[-vNum];
		if (vWeight < vWeightMin) vWeightMin = vWeight;
		if (vWeight > vWeightMax) vWeightMax = vWeight;
		attributor_intern_prioQa_add(vWeight);
		vWeightMean += vWeight;
		++numElt;
	}

	if (numElt > ZERO){
		vWeightMean = vWeightMean / numElt;
		attributor_intern_prioQa_computeDescription(vWeightMean,&vWeightMedian,&vWeightStdDev,
				&vWeightSent,&vWeightRent,&vWeightMent,
				&vWeightPGFA,&vWeightPGFB,&vWeightPGFC);
		MAIN_SET_FATT_RW_VWEIGHTS_MIN(vWeightMin);
		MAIN_SET_FATT_RW_VWEIGHTS_MAX(vWeightMax);
		MAIN_SET_FATT_RW_VWEIGHTS_MEAN(vWeightMean);
		MAIN_SET_FATT_RW_VWEIGHTS_MEDIAN(vWeightMedian);
		MAIN_SET_FATT_RW_VWEIGHTS_STDDEV(vWeightStdDev);
		MAIN_SET_FATT_RW_VWEIGHTS_SENT(vWeightSent);
		MAIN_SET_FATT_RW_VWEIGHTS_RENT(vWeightRent);
		MAIN_SET_FATT_RW_VWEIGHTS_MENT(vWeightMent);
		MAIN_SET_FATT_RW_VWEIGHTS_PGFA(vWeightPGFA);
		MAIN_SET_FATT_RW_VWEIGHTS_PGFB(vWeightPGFB);
		MAIN_SET_FATT_RW_VWEIGHTS_PGFC(vWeightPGFC);
	}

	//Clean up.
	free(rw_a);
	free(rw_b);
	free(rw_l);
	attributor_intern_prioQa_resize(BLOCKSIZE);

	#ifdef VERBOSE_ATTRIBUTOR
	printf("c     ATTRIBUTOR:   RW_Q[%f %f %f %f %f %f %f] \n",
			MAIN_GET_FATT_RW_Q05(), MAIN_GET_FATT_RW_Q10(), MAIN_GET_FATT_RW_Q25(), MAIN_GET_FATT_RW_Q50(),
			MAIN_GET_FATT_RW_Q75(),	MAIN_GET_FATT_RW_Q90(), MAIN_GET_FATT_RW_Q95());
	printf("c     ATTRIBUTOR:   RW_VWEIGHTS[MIN %f MAX %f MEAN %f MEDIAN %f SENT %f RENT %f MENT %f STDDEV %f PGFA %f PGFB %f PGFC %f]\n",
			MAIN_GET_FATT_RW_VWEIGHTS_MIN(),MAIN_GET_FATT_RW_VWEIGHTS_MAX(),MAIN_GET_FATT_RW_VWEIGHTS_MEAN(),
			MAIN_GET_FATT_RW_VWEIGHTS_MEDIAN(),MAIN_GET_FATT_RW_VWEIGHTS_SENT(),MAIN_GET_FATT_RW_VWEIGHTS_RENT(),
			MAIN_GET_FATT_RW_VWEIGHTS_MENT(),MAIN_GET_FATT_RW_VWEIGHTS_STDDEV(),MAIN_GET_FATT_RW_VWEIGHTS_PGFA(),
			MAIN_GET_FATT_RW_VWEIGHTS_PGFB(),MAIN_GET_FATT_RW_VWEIGHTS_PGFC());
	fflush(stdout);
	#endif

	#ifdef COLLECTSTATS
	stats_attributor_time_RW += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}

uint32_t attributor_intern_compute_ELS_strongConnect(literal* l, uint32_t index){
	literal *impLit;
	uint32_t i;

	SET_LIT_ELS_INDEX(l,index);
	SET_LIT_ELS_LOWLINK(l, index);
	++index;
	lS_push(l);
	SET_LIT_ELS_ISINS(l, 1U);

	for (i= 0; i < GET_LIT_NUMIMP_USED(l); i+=2){
		impLit = main_litData + GET_LIT_IMPNUM(l,i);
		if (GET_LIT_ELS_INDEX(impLit) == 0U){
			//Not yet marked. Recurse on implied literal.
			index = attributor_intern_compute_ELS_strongConnect(impLit, index);
			if (GET_LIT_ELS_LOWLINK(impLit) < GET_LIT_ELS_LOWLINK(l)){
				SET_LIT_ELS_LOWLINK(l, GET_LIT_ELS_LOWLINK(impLit));
			}
		} else if (GET_LIT_ELS_ISINS(impLit)){
			//Already in stack -- implied literal is in the current SCC.
			if (GET_LIT_ELS_INDEX(impLit) < GET_LIT_ELS_LOWLINK(l)){
				SET_LIT_ELS_LOWLINK(l, GET_LIT_ELS_INDEX(impLit));
			}
		}
	}

	//If l is a root, then pop the stack and generate an SCC class.
	if (GET_LIT_ELS_LOWLINK(l) == GET_LIT_ELS_INDEX(l)){
		//Allocate new equivalence class if necessary.
		ATTRIBUTOR_ENLARGE_ELS_CLASSES_IF_NECESSARY(i);
		do {
			impLit = lS_pop();
			SET_LIT_ELS_ISINS(impLit, 0U);

			if (GET_LIT_STAMP(impLit) != main_litStamp){
				ATTRIBUTOR_ADD_LIT_TO_CURRENT_ELS_CLASS(impLit);
				SET_LIT_STAMP(impLit, main_litStamp);
				SET_LIT_STAMP(GET_LIT_OPPOSITE(impLit), main_litStamp);
			}
		} while (impLit != l);
		//If the class now contains more than one element it contains useful information. If not, we can reset the class
		//as it contains only one element.
		if (attributor_ELS_classes_memUsed[attributor_ELS_classes_used] > 1U){
			//Yes, some literals in this class are defined to be equivalent.
			++attributor_ELS_classes_used;
		} else {
			//No, we can revert the class as it would contain only one element.
			attributor_ELS_classes_memUsed[attributor_ELS_classes_used] = 0;
		}
	}

	return index;
}

void attributor_intern_compute_ELS_allAttributes(){
	//Computes the equivalent literal information. How many equivalence classes do we have, and what is the minimum, maximum
	//mean and average size, and for all of that the standard deviation and entropy of the values.
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	#endif

	ATTRIBUTOR_SET_START_TIME_IN_SEC();

	literal *l;
	variable *v;
	float_ty numElt = ZERO;
	float_ty elsNumClasses = ZERO;
	float_ty elsSize, elsSizeMin = UINT32_MAX, elsSizeMax = ZERO, elsSizeMean = ZERO,
			 elsSizeMedian = ZERO, elsSizeStdDev = ZERO, elsSizeSent = ZERO, elsSizeRent = ZERO, elsSizeMent = ZERO,
			 elsSizePGFA = ZERO, elsSizePGFB = ZERO, elsSizePGFC = ZERO;
	uint32_t index = 1;
	int32_t i;

	//Initialize the literal stack to be empty.
	lS_flush();

	//We prepare the literal stamp to present classes only once.
	++main_litStamp;

	//Initialize the classes information.
	attributor_ELS_classes_used = 0;
	for (i = 0; i < attributor_ELS_classes_avail; ++i){
		attributor_ELS_classes_memUsed[i] = 0;
	}

	//Prepare all literals.
	for(i = 1; i < f.n_initial+1; ++i){
		l = main_litData + i;
		SET_LIT_ELS_INDEX(l, 0);
		SET_LIT_ELS_LOWLINK(l, 0);
		SET_LIT_ELS_ISINS(l, 0);
		l = main_litData - i;
		SET_LIT_ELS_INDEX(l, 0);
		SET_LIT_ELS_LOWLINK(l, 0);
		SET_LIT_ELS_ISINS(l, 0);
	}

	for (i = 1; i < f.n_initial+1; ++i){
		if (ATTRIBUTOR_IS_TIME_ELAPSED()){
			#ifdef VERBOSE_ATTRIBUTOR
			printf("c     ATTRIBUTOR:   TIMEOUT ELS-V.\n");
			#endif
			#ifdef COLLECTSTATS
			++stats_attributor_numTimeouts;
			#endif
			break;
		}
		//Ignore variables that are already disabled.
		v = main_varData + i;
		if (IS_VAR_DISABLED(v) || !IS_VAR_UNASSIGNED(v)) continue;
		//The variable is still good to go.
		l = main_litData + i;
		//Check if the literal has been checked by strong-connect.
		if (GET_LIT_ELS_INDEX(l) == 0){
			index = attributor_intern_compute_ELS_strongConnect(l, index);
		}
		l = GET_LIT_OPPOSITE(l);
		if (GET_LIT_ELS_INDEX(l) == 0){
			index = attributor_intern_compute_ELS_strongConnect(l, index);
		}
	}

	//We have now computed all the equivalence classes. We add their sizes to the priority queue and then compute the
	//values for the attributes.
	attributor_intern_prioQa_resize(attributor_ELS_classes_used);
	elsNumClasses = attributor_ELS_classes_used;
	if (elsNumClasses > ZERO){
		for (i = 0; i < attributor_ELS_classes_used; ++i){
			elsSize = attributor_ELS_classes_memUsed[i];
			if (elsSize < elsSizeMin) elsSizeMin = elsSize;
			if (elsSize > elsSizeMax) elsSizeMax = elsSize;
			elsSizeMean += elsSize;
			attributor_intern_prioQa_add(elsSize);
			++numElt;
		}
		elsSizeMean = elsSizeMean / numElt;
		attributor_intern_prioQa_computeDescription(elsSizeMean,&elsSizeMedian,&elsSizeStdDev,
				&elsSizeSent,&elsSizeRent,&elsSizeMent,
				&elsSizePGFA,&elsSizePGFB,&elsSizePGFC);
		MAIN_SET_FATT_ELS_NUMC(elsNumClasses);
		MAIN_SET_FATT_ELS_SIZES_MIN(elsSizeMin);
		MAIN_SET_FATT_ELS_SIZES_MAX(elsSizeMax);
		MAIN_SET_FATT_ELS_SIZES_MEAN(elsSizeMean);
		MAIN_SET_FATT_ELS_SIZES_MEDIAN(elsSizeMedian);
		MAIN_SET_FATT_ELS_SIZES_STDDEV(elsSizeStdDev);
		MAIN_SET_FATT_ELS_SIZES_SENT(elsSizeSent);
		MAIN_SET_FATT_ELS_SIZES_RENT(elsSizeRent);
		MAIN_SET_FATT_ELS_SIZES_MENT(elsSizeMent);
		MAIN_SET_FATT_ELS_SIZES_PGFA(elsSizePGFA);
		MAIN_SET_FATT_ELS_SIZES_PGFB(elsSizePGFB);
		MAIN_SET_FATT_ELS_SIZES_PGFC(elsSizePGFC);
	}


	//Finally, clean up again.
	attributor_intern_prioQa_resize(BLOCKSIZE);
	lS_flush();
	++main_litStamp;

	#ifdef VERBOSE_ATTRIBUTOR
	printf("c     ATTRIBUTOR:   ELS_NUMC[%f]\n", MAIN_GET_FATT_ELS_NUMC());
	printf("c     ATTRIBUTOR:   ELS_SIZES[MIN %f MAX %f MEAN %f MEDIAN %f SENT %f RENT %f MENT %f STDDEV %f PGFA %f PGFB %f PGFC %f]\n",
			MAIN_GET_FATT_ELS_SIZES_MIN(),MAIN_GET_FATT_ELS_SIZES_MAX(),MAIN_GET_FATT_ELS_SIZES_MEAN(),
			MAIN_GET_FATT_ELS_SIZES_MEDIAN(),MAIN_GET_FATT_ELS_SIZES_SENT(),MAIN_GET_FATT_ELS_SIZES_RENT(),
			MAIN_GET_FATT_ELS_SIZES_MENT(),MAIN_GET_FATT_ELS_SIZES_STDDEV(),MAIN_GET_FATT_ELS_SIZES_PGFA(),
			MAIN_GET_FATT_ELS_SIZES_PGFB(),MAIN_GET_FATT_ELS_SIZES_PGFC());

	fflush(stdout);
	#endif
	#ifdef COLLECTSTATS
	stats_attributor_time_ELS += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}
