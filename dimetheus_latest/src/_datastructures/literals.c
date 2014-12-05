/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "../dimetheus.h"

void literals_reset(){
	main_litData = NULL;
	f.lits = NULL;
}

uint32_t literals_init(){
	int32_t i, success = 1;

	f.lits = malloc(sizeof(literal)*(2*f.n_initial+1));
	if (f.lits == NULL){
		printf("c ERROR. Unable to allocate the literal data array. Out of Memory?\n");
		return 0;
	}
	main_litData = f.lits;
	main_litData += f.n_initial;

	for (i = -f.n_initial; i<=((int32_t)f.n_initial); ++i){
		main_litData[i].stamp = 0;
		main_litData[i].upStamp = 0;
		main_litData[i].lQPos = 0;
		main_litData[i].lQScore = 0;
		main_litData[i].lSPos = 0;
		main_litData[i].dsc = 0;
		main_litData[i].fin = 0;
		main_litData[i].obs = 0;
		main_litData[i].prt = NULL;
		main_litData[i].root = NULL;

		main_litData[i].numOccsAvail = BLOCKSIZE;
		main_litData[i].numOccsUsed = 0;
		main_litData[i].occs = NULL;
		main_litData[i].occs = malloc(sizeof(clause*)*main_litData[i].numOccsAvail);
		if (main_litData[i].occs == NULL){
			success = 0;
		}
		main_litData[i].occLitPos = NULL;
		main_litData[i].occLitPos = malloc(sizeof(uint32_t)*main_litData[i].numOccsAvail);
		if (main_litData[i].occLitPos == NULL){
			success = 0;
		}

		main_litData[i].numw1Avail = BLOCKSIZE;
		main_litData[i].numw1Used = 0;
		main_litData[i].w1 = NULL;
		main_litData[i].w1 = malloc(sizeof(clause*)*main_litData[i].numw1Avail);
		if (main_litData[i].w1 == NULL){
			success = 0;
		}

		main_litData[i].numw2Avail = BLOCKSIZE;
		main_litData[i].numw2Used = 0;
		main_litData[i].w2 = NULL;
		main_litData[i].w2 = malloc(sizeof(clause*)*main_litData[i].numw2Avail);
		if (main_litData[i].w2 == NULL){
			success = 0;
		}

		main_litData[i].numImpAvail = 2*BLOCKSIZE;
		main_litData[i].numImpUsed = 0;
		main_litData[i].implies = NULL;
		main_litData[i].implies = malloc(sizeof(clause*)*main_litData[i].numImpAvail);
		if (main_litData[i].implies == NULL){
			success = 0;
		}
	}
	if (success) {
		return 1;
	} else {
		return 0;
	}
}

void literals_dispose(){
	int32_t i;
	if (f.lits != NULL){
		for (i = -f.n_initial; i<=((int32_t)f.n_initial); ++i){
			if (main_litData[i].occs != NULL){
				free(main_litData[i].occs);
				main_litData[i].occs = NULL;
			}
			if (main_litData[i].occLitPos != NULL){
				free(main_litData[i].occLitPos);
				main_litData[i].occLitPos = NULL;
			}
			if (main_litData[i].w1 != NULL){
				free(main_litData[i].w1);
				main_litData[i].w1 = NULL;
			}
			if (main_litData[i].w2 != NULL){
				free(main_litData[i].w2);
				main_litData[i].w2 = NULL;
			}
			if (main_litData[i].implies != NULL){
				free(main_litData[i].implies);
				main_litData[i].implies = NULL;
			}
		}
		main_litData -= f.n_initial;
		free(main_litData);
		main_litData = NULL;
	}
}

#ifdef VERBOSE
void literals_printLiteralImplications(literal *l){
	uint32_t i;
	for (i = 0; i < l->numImpUsed; i += 2){
		printf("%d %p ", (int32_t)(addr_t)(l->implies[ i ]), (void *)(l->implies[ i +1]));
	}
}

void literals_printLiteralOccs(literal *l){
	uint32_t i;
	for (i = 0; i < l->numOccsUsed; ++i){
		printf("%p ", (void*) l->occs[ i ]);
	}
}

void literals_printLiteralOccLitPos(literal *l){
	uint32_t i;
	for (i = 0; i < l->numOccsUsed; ++i){
		printf("%u ", l->occLitPos[ i ]);
	}
}

void literals_printLiteralW1List(literal *l){
	uint32_t i;
	for (i = 0; i < l->numw1Used; ++i){
		printf("%p ", (void*) l->w1[ i ]);
	}
}

void literals_printLiteralW2List(literal *l){
	uint32_t i;
	for (i = 0; i < l->numw2Used; ++i){
		printf("%p ", (void*) l->w2[ i ]);
	}
}

void literals_printLiteral(literal* l){
	literal *ll, *lll;
	printf("c LIT %p(%6d) ", (void*)l, (int32_t)(l - main_litData));
	printf("STAMP:%10d\tQPOS:%5d\tQSCORE:%d\tIMP(U/A/LIST):%d/%d/", l->stamp,l->lQPos,l->lQScore,l->numImpUsed,l->numImpAvail);
	literals_printLiteralImplications(l);
	printf("\tOCC(U/A/LIST):%d/%d/", l->numOccsUsed, l->numOccsAvail);
	literals_printLiteralOccs(l);
	printf("\tOCCLITPOS(U/A/LIST):%d/%d/", l->numOccsUsed, l->numOccsAvail);
	literals_printLiteralOccLitPos(l);
	printf("\tW1LIST(U/A/LIST):%d/%d/", l->numw1Used, l->numw1Avail);
	literals_printLiteralW1List(l);
	printf("\tW2LIST(U/A/LIST):%d/%d/", l->numw2Used, l->numw2Avail);
	literals_printLiteralW2List(l);
	ll = GET_LIT_ROOT(l);
	lll = GET_LIT_PRT(l);
	printf("\tDSC:%d\tOBS:%d\tFIN:%d\tPRT:%d\tROOT:%d", GET_LIT_DSC(l), GET_LIT_OBS(l), GET_LIT_FIN(l),
			GET_LIT_NUM_FROM_PTR(lll), GET_LIT_NUM_FROM_PTR(ll));
	printf("\n");
}
#endif
