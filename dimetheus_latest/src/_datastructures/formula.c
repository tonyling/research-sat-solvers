/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "../dimetheus.h"

void formula_reset(){
	main_litStamp = 1;
	f.att = NULL;
	f.vars = NULL;
	f.vars_e = NULL;
	f.vars_d = NULL;

	f.lits = NULL;

	f.clauses_eo = NULL;
	f.clauses_do = NULL;
	f.clauses_el = NULL;
	f.clauses_dl = NULL;
	f.clauses_rem = NULL;

	f.n_initial = 0;
	f.n_afterPrep = 0;
	f.n_vars_e_used = 0;
	f.n_vars_d_used = 0;

	f.m_initial = 0;

	f.m_eo_used = 0;
	f.m_eo_avail = 0;

	f.m_do_used = 0;
	f.m_do_avail = 0;

	f.m_el_used = 0;
	f.m_el_avail = 0;

	f.m_dl_used = 0;
	f.m_dl_avail = 0;

	f.m_rem_used = 0;
	f.m_rem_avail = 0;
}

uint32_t formula_init(){
	int32_t i;

	//Allocate memory for the formula attributes.
	MAIN_FATT_INIT();

	//Enabled original clauses. We give one extra address such that the array is not automatically increased when the last
	//original clause gets added to the array (see the end of method addOriginalClause).
	f.clauses_eo = malloc(sizeof(clause*)*(f.m_initial+1));
	if (f.clauses_eo == NULL){
		printf("c ERROR. Unable to allocate the memory for the original enabled clause pointers.\n");
		return 0;
	}
	for (i=0; i < (f.m_initial+1); ++i){
		f.clauses_eo[i] = NULL;
	}
	f.m_eo_avail = f.m_initial+1;

	//Disabled original clauses. We add one extra address field here as well.
	f.clauses_do = malloc(sizeof(clause*)*(f.m_initial+1));
	if (f.clauses_do == NULL){
		printf("c ERROR. Unable to allocate the memory for the original disabled clause pointers.\n");
		return 0;
	}
	for (i=0; i < (f.m_initial+1); ++i){
		f.clauses_do[i] = NULL;
	}
	f.m_do_avail = f.m_initial+1;

	//Enabled learned clauses.
	f.clauses_el = malloc(sizeof(clause*)*BLOCKSIZE);
	if (f.clauses_el == NULL){
		printf("c ERROR. Unable to allocate the memory for the learned enabled clause pointers.\n");
		return 0;
	}
	for (i=0; i < BLOCKSIZE; ++i){
		f.clauses_el[i] = NULL;
	}
	f.m_el_avail = BLOCKSIZE;

	//Disabled learned clauses.
	f.clauses_dl = malloc(sizeof(clause*)*BLOCKSIZE);
	if (f.clauses_dl == NULL){
		printf("c ERROR. Unable to allocate the memory for the learned disabled clause pointers.\n");
		return 0;
	}
	for (i=0; i < BLOCKSIZE; ++i){
		f.clauses_dl[i] = NULL;
	}
	f.m_dl_avail = BLOCKSIZE;

	//Removed clauses.
	f.clauses_rem = malloc(sizeof(remClause*)*BLOCKSIZE);
	if (f.clauses_rem == NULL){
		printf("c ERROR. Unable to allocate the memory for the removed clause pointers.\n");
		return 0;
	}
	for (i=0; i < BLOCKSIZE; ++i){
		f.clauses_rem[i] = NULL;
	}
	f.m_rem_avail = BLOCKSIZE;

	return 1;
}

void formula_dispose(){
	int32_t i;

	MAIN_FATT_DISPOSE();

	if (f.clauses_eo != NULL){
		for (i = 0; i < f.m_eo_used; ++i){
			if (f.clauses_eo[i] != NULL){
				if (f.clauses_eo[i]->lits != NULL){
					free(f.clauses_eo[i]->lits);
					f.clauses_eo[i]->lits = NULL;
				}
				if (f.clauses_eo[i]->msgs != NULL){
					free(f.clauses_eo[i]->msgs);
					f.clauses_eo[i]->msgs = NULL;
				}
				if (f.clauses_eo[i]->occPos != NULL){
					free(f.clauses_eo[i]->occPos);
					f.clauses_eo[i]->occPos = NULL;
				}
				free(f.clauses_eo[i]);
			}
		}
		free(f.clauses_eo);
		f.clauses_eo = NULL;
	}

	if (f.clauses_do != NULL){
		for (i = 0; i < f.m_do_used; ++i){
			if (f.clauses_do[i] != NULL){
				if (f.clauses_do[i]->lits != NULL){
					free(f.clauses_do[i]->lits);
					f.clauses_do[i]->lits = NULL;
				}
				if (f.clauses_do[i]->msgs != NULL){
					free(f.clauses_do[i]->msgs);
					f.clauses_do[i]->msgs = NULL;
				}
				if (f.clauses_do[i]->occPos != NULL){
					free(f.clauses_do[i]->occPos);
					f.clauses_do[i]->occPos = NULL;
				}
				free(f.clauses_do[i]);
			}
		}
		free(f.clauses_do);
		f.clauses_do = NULL;
	}

	if (f.clauses_el != NULL){
		for (i = 0; i < f.m_el_used; ++i){
			if (f.clauses_el[i] != NULL){
				if (f.clauses_el[i]->lits != NULL){
					free(f.clauses_el[i]->lits);
					f.clauses_el[i]->lits = NULL;
				}
				if (f.clauses_el[i]->msgs != NULL){
					free(f.clauses_el[i]->msgs);
					f.clauses_el[i]->msgs = NULL;
				}
				if (f.clauses_el[i]->occPos != NULL){
					free(f.clauses_el[i]->occPos);
					f.clauses_el[i]->occPos = NULL;
				}
				free(f.clauses_el[i]);
			}
		}
		free(f.clauses_el);
		f.clauses_el = NULL;
	}

	if (f.clauses_dl != NULL){
		for (i = 0; i < f.m_dl_used; ++i){
			if (f.clauses_dl[i] != NULL){
				if (f.clauses_dl[i]->lits != NULL){
					free(f.clauses_dl[i]->lits);
					f.clauses_dl[i]->lits = NULL;
				}
				if (f.clauses_dl[i]->msgs != NULL){
					free(f.clauses_dl[i]->msgs);
					f.clauses_dl[i]->msgs = NULL;
				}
				if (f.clauses_dl[i]->occPos != NULL){
					free(f.clauses_dl[i]->occPos);
					f.clauses_dl[i]->occPos = NULL;
				}
				free(f.clauses_dl[i]);
			}
		}
		free(f.clauses_dl);
		f.clauses_dl = NULL;
	}

	if (f.clauses_rem != NULL){
		for (i = 0; i < f.m_rem_used; ++i){
			if (f.clauses_rem[i] != NULL){
				if (f.clauses_rem[i]->lits != NULL){
					free(f.clauses_rem[i]->lits);
					f.clauses_rem[i]->lits = NULL;
				}
				if (f.clauses_rem[i]->coveredLits != NULL){
					free(f.clauses_rem[i]->coveredLits);
					f.clauses_rem[i]->coveredLits = NULL;
				}
				free(f.clauses_rem[i]);
			}
		}
		free(f.clauses_rem);
		f.clauses_rem = NULL;
	}
}
