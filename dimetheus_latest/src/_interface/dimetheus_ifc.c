/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "../dimetheus.h"

#ifdef COMPILEASLIBRARY
int32_t dimetheus_handleSignal(int signal){
	main_returnCode = SIGNAL;
	main_handleresult();
	main_shutdown();
	return main_returnCode;
}

void dimetheus_getVersion(int32_t* v_ma, int32_t* v_mi, int32_t* v_re){
	*v_ma = VERSION_MA;
	*v_mi = VERSION_MI;
	*v_re = VERSION_RE;
}

char* dimetheus_getLicenseHash(){
	return DIMETHEUS_LICENSE_HASH;
}


int32_t dimetheus_solve_dimacscnf(int32_t argc, char** argv, int32_t n, int32_t m, int32_t** clsA, int32_t* ass){

	main_reset();

	if (argc < 1)		{printf("c IFC-ERROR. The number of parameters must be at least 1.\n");return ERROR;}
	if (argv == NULL)	{printf("c IFC-ERROR. The argv array must contain at least one element.\n");return ERROR;}
	if (n <= 0)			{printf("c IFC-ERROR. The number of variables in the formula must be larger 0.\n");return ERROR;}
	if (m <= 0)			{printf("c IFC-ERROR. The number of clauses in the formula must be larger 0.\n");return ERROR;}
	if (clsA == NULL)	{printf("c IFC-ERROR. The clause definition array must not be NULL.\n");return ERROR;}
	if (ass == NULL)	{printf("c IFC-ERROR. The assignment array must not be NULL.\n");return ERROR;}

	dimetheus_ifc_n = n;
	dimetheus_ifc_m = m;
	dimetheus_ifc_clsA = clsA;
	dimetheus_ifc_assA = ass;

	if (main_returnCode == UNKNOWN) main_params(argc,argv);

	if (param_startupInputFormat == STARTUP_INPUTFORMAT_UNKNOWN){
		param_startupInputFormat = STARTUP_INPUTFORMAT_DIMACSCNF;
	} else if (param_startupInputFormat != STARTUP_INPUTFORMAT_DIMACSCNF){
		printf("c IFC-ERROR. Parameter startupInputFormat must be %d (STARTUP_INPUTFORMAT_DIMACSCNF) when calling "
			   "the dimetheus_solve_dimacscnf(...) interface method, or it must not be provided at all.\n",
			   STARTUP_INPUTFORMAT_DIMACSCNF);return ERROR;
	}

	if (main_returnCode == UNKNOWN) main_guidance(argc,argv);

	if (main_returnCode == UNKNOWN){
		main_startup();

		if (main_returnCode == UNKNOWN)	main_classify();
		if (main_returnCode == UNKNOWN)	main_adapt();
		if (main_returnCode == UNKNOWN)	main_preprocess();
		if (main_returnCode == UNKNOWN) main_search();
	}

	main_handleresult();
	main_shutdown();

	return main_returnCode;
}

int32_t dimetheus_solve_aplefoo(int32_t argc, char** argv, char* aplefooFormula, char** aplefooSolution){

	main_reset();
	if (argc < 1)				{printf("c IFC-ERROR. The number of parameters must be at least 1.\n");return ERROR;}
	if (argv == NULL)			{printf("c IFC-ERROR. The argv array must contain at least one element.\n");return ERROR;}
	if (aplefooFormula == NULL) {printf("c IFC-ERROR. The aplefooFormula must not be NULL.\n");return ERROR;}
	if (aplefooSolution == NULL){printf("c IFC-ERROR. The aplefooSolution must not be NULL.\n"); return ERROR;}

	dimetheus_ifc_aplefooFormula = aplefooFormula;
	dimetheus_ifc_aplefooSolution = aplefooSolution;

	if (main_returnCode == UNKNOWN) main_params(argc,argv);

	if (param_startupInputFormat == STARTUP_INPUTFORMAT_UNKNOWN){
		param_startupInputFormat = STARTUP_INPUTFORMAT_APLEFOO;
	} else if (param_startupInputFormat != STARTUP_INPUTFORMAT_APLEFOO){
		printf("c IFC-ERROR. Parameter startupInputFormat must be %d (STARTUP_INPUTFORMAT_APLEFOO) when calling "
			   "the dimetheus_solve_aplefoo(...) interface method, or it must not be provided at all.\n",
			   STARTUP_INPUTFORMAT_APLEFOO);return ERROR;
	}

	if (main_returnCode == UNKNOWN) main_guidance(argc,argv);

	if (main_returnCode == UNKNOWN){
		main_startup();

		if (main_returnCode == UNKNOWN)	main_classify();
		if (main_returnCode == UNKNOWN)	main_adapt();
		if (main_returnCode == UNKNOWN)	main_preprocess();
		if (main_returnCode == UNKNOWN) main_search();
	}

	main_handleresult();
	main_shutdown();

	return main_returnCode;
}
#endif
