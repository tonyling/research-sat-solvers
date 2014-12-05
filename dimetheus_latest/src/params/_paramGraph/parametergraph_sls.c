/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "paramgraph.h"
#ifdef SPECS_PARAMGRAPH

void params_intern_appendSlsPAFVR_unconditional(uint32_t PAFVRONID, char* parName){
	uint32_t PAFVRANID;

	//PROBSAT PB
	params_intern_appendANode(PAFVRONID, parName, SPECS_DTYPE_C, SLS_PICKANDFLIPVARRULE_PROBSATPB, SLS_PICKANDFLIPVARRULE_PROBSATPB);
	PAFVRANID = currNID-1;
	params_intern_appendONode(PAFVRANID, "slsProbsatCB");
		params_intern_appendANode(currNID-1, "slsProbsatCB", SPECS_DTYPE_R, PARAM_FUZZMIN_SLSPROBSATCB, PARAM_FUZZMAX_SLSPROBSATCB);
	params_intern_appendONode(PAFVRANID, "slsProbsatCBShift");
			params_intern_appendANode(currNID-1, "slsProbsatCBShift", SPECS_DTYPE_R, PARAM_FUZZMIN_SLSPROBSATCBSHIFT, PARAM_FUZZMAX_SLSPROBSATCBSHIFT);

	//PROBSAT EB
	params_intern_appendANode(PAFVRONID, parName, SPECS_DTYPE_C, SLS_PICKANDFLIPVARRULE_PROBSATEB, SLS_PICKANDFLIPVARRULE_PROBSATEB);
	PAFVRANID = currNID-1;
	params_intern_appendONode(PAFVRANID, "slsProbsatCB");
		params_intern_appendANode(currNID-1, "slsProbsatCB", SPECS_DTYPE_R, PARAM_FUZZMIN_SLSPROBSATCB, PARAM_FUZZMAX_SLSPROBSATCB);
}

void params_intern_appendSlsTRR_SLS(uint32_t TRRONID, char* parName){
	uint32_t TRRANID;

	//SINGLE CONST
	params_intern_appendANode(TRRONID, parName, SPECS_DTYPE_C, SLS_TRYRESTARTRULE_CONST, SLS_TRYRESTARTRULE_CONST);
	TRRANID = currNID-1;
	params_intern_appendONode(TRRANID, "slsMaxFlipsA");
		params_intern_appendANode(currNID-1, "slsMaxFlipsA", SPECS_DTYPE_R, PARAM_FUZZMIN_SLSMAXFLIPSA, PARAM_FUZZMAX_SLSMAXFLIPSA);

	//SINGLE LINEAR
	params_intern_appendANode(TRRONID, parName, SPECS_DTYPE_C, SLS_TRYRESTARTRULE_LINEAR, SLS_TRYRESTARTRULE_LINEAR);
	TRRANID = currNID-1;
	params_intern_appendONode(TRRANID, "slsMaxFlipsA");
		params_intern_appendANode(currNID-1, "slsMaxFlipsA", SPECS_DTYPE_R, PARAM_FUZZMIN_SLSMAXFLIPSA, PARAM_FUZZMAX_SLSMAXFLIPSA);

	//SINGLE SQRT
	params_intern_appendANode(TRRONID, parName, SPECS_DTYPE_C, SLS_TRYRESTARTRULE_SQRT, SLS_TRYRESTARTRULE_SQRT);
	TRRANID = currNID-1;
	params_intern_appendONode(TRRANID, "slsMaxFlipsA");
		params_intern_appendANode(currNID-1, "slsMaxFlipsA", SPECS_DTYPE_R, PARAM_FUZZMIN_SLSMAXFLIPSA, PARAM_FUZZMAX_SLSMAXFLIPSA);
	params_intern_appendONode(TRRANID, "slsMaxFlipsB");
		params_intern_appendANode(currNID-1, "slsMaxFlipsB", SPECS_DTYPE_R, PARAM_FUZZMIN_SLSMAXFLIPSB, PARAM_FUZZMAX_SLSMAXFLIPSB);

	//SINGLE EXP
	params_intern_appendANode(TRRONID, parName, SPECS_DTYPE_C, SLS_TRYRESTARTRULE_EXP, SLS_TRYRESTARTRULE_EXP);
	TRRANID = currNID-1;
	params_intern_appendONode(TRRANID, "slsMaxFlipsA");
		params_intern_appendANode(currNID-1, "slsMaxFlipsA", SPECS_DTYPE_R, PARAM_FUZZMIN_SLSMAXFLIPSA, PARAM_FUZZMAX_SLSMAXFLIPSA);
	params_intern_appendONode(TRRANID, "slsMaxFlipsB");
		params_intern_appendANode(currNID-1, "slsMaxFlipsB", SPECS_DTYPE_R, PARAM_FUZZMIN_SLSMAXFLIPSB, PARAM_FUZZMAX_SLSMAXFLIPSB);
}

void params_intern_appendSlsTRR_PMPSLS(uint32_t TRRONID, char* parName){
	uint32_t TRRANID;

	//SINGLE CONST
	params_intern_appendANode(TRRONID, parName, SPECS_DTYPE_C, SLS_TRYRESTARTRULE_CONST, SLS_TRYRESTARTRULE_CONST);
	TRRANID = currNID-1;
	params_intern_appendONode(TRRANID, "slsMaxTries");
		params_intern_appendANode(currNID-1, "slsMaxTries", SPECS_DTYPE_I, PARAM_FUZZMIN_SLSMAXTRIES, PARAM_FUZZMAX_SLSMAXTRIES);
	params_intern_appendONode(TRRANID, "slsMaxFlipsA");
		params_intern_appendANode(currNID-1, "slsMaxFlipsA", SPECS_DTYPE_R, PARAM_FUZZMIN_SLSMAXFLIPSA, PARAM_FUZZMAX_SLSMAXFLIPSA);

	//SINGLE LINEAR
	params_intern_appendANode(TRRONID, parName, SPECS_DTYPE_C, SLS_TRYRESTARTRULE_LINEAR, SLS_TRYRESTARTRULE_LINEAR);
	TRRANID = currNID-1;
	params_intern_appendONode(TRRANID, "slsMaxTries");
		params_intern_appendANode(currNID-1, "slsMaxTries", SPECS_DTYPE_I, PARAM_FUZZMIN_SLSMAXTRIES, PARAM_FUZZMAX_SLSMAXTRIES);
	params_intern_appendONode(TRRANID, "slsMaxFlipsA");
		params_intern_appendANode(currNID-1, "slsMaxFlipsA", SPECS_DTYPE_R, PARAM_FUZZMIN_SLSMAXFLIPSA, PARAM_FUZZMAX_SLSMAXFLIPSA);

	//SINGLE SQRT
	params_intern_appendANode(TRRONID, parName, SPECS_DTYPE_C, SLS_TRYRESTARTRULE_SQRT, SLS_TRYRESTARTRULE_SQRT);
	TRRANID = currNID-1;
	params_intern_appendONode(TRRANID, "slsMaxTries");
		params_intern_appendANode(currNID-1, "slsMaxTries", SPECS_DTYPE_I, PARAM_FUZZMIN_SLSMAXTRIES, PARAM_FUZZMAX_SLSMAXTRIES);
	params_intern_appendONode(TRRANID, "slsMaxFlipsA");
		params_intern_appendANode(currNID-1, "slsMaxFlipsA", SPECS_DTYPE_R, PARAM_FUZZMIN_SLSMAXFLIPSA, PARAM_FUZZMAX_SLSMAXFLIPSA);
	params_intern_appendONode(TRRANID, "slsMaxFlipsB");
		params_intern_appendANode(currNID-1, "slsMaxFlipsB", SPECS_DTYPE_R, PARAM_FUZZMIN_SLSMAXFLIPSB, PARAM_FUZZMAX_SLSMAXFLIPSB);

	//SINGLE EXP
	params_intern_appendANode(TRRONID, parName, SPECS_DTYPE_C, SLS_TRYRESTARTRULE_EXP, SLS_TRYRESTARTRULE_EXP);
	TRRANID = currNID-1;
	params_intern_appendONode(TRRANID, "slsMaxTries");
		params_intern_appendANode(currNID-1, "slsMaxTries", SPECS_DTYPE_I, PARAM_FUZZMIN_SLSMAXTRIES, PARAM_FUZZMAX_SLSMAXTRIES);
	params_intern_appendONode(TRRANID, "slsMaxFlipsA");
		params_intern_appendANode(currNID-1, "slsMaxFlipsA", SPECS_DTYPE_R, PARAM_FUZZMIN_SLSMAXFLIPSA, PARAM_FUZZMAX_SLSMAXFLIPSA);
	params_intern_appendONode(TRRANID, "slsMaxFlipsB");
		params_intern_appendANode(currNID-1, "slsMaxFlipsB", SPECS_DTYPE_R, PARAM_FUZZMIN_SLSMAXFLIPSB, PARAM_FUZZMAX_SLSMAXFLIPSB);
}

void params_intern_appendSlsTRR_HYBRIDMSC(uint32_t TRRONID, char* parName){
	uint32_t TRRANID;

	//SINGLE CONST
	params_intern_appendANode(TRRONID, parName, SPECS_DTYPE_C, SLS_TRYRESTARTRULE_CONST, SLS_TRYRESTARTRULE_CONST);
	TRRANID = currNID-1;
	params_intern_appendONode(TRRANID, "slsMaxTries");
		params_intern_appendANode(currNID-1, "slsMaxTries", SPECS_DTYPE_I, PARAM_FUZZMIN_SLSMAXTRIES, PARAM_FUZZMAX_SLSMAXTRIES);
	params_intern_appendONode(TRRANID, "slsMaxFlipsA");
		params_intern_appendANode(currNID-1, "slsMaxFlipsA", SPECS_DTYPE_R, PARAM_FUZZMIN_SLSMAXFLIPSA, PARAM_FUZZMAX_SLSMAXFLIPSA);

	//SINGLE LINEAR
	params_intern_appendANode(TRRONID, parName, SPECS_DTYPE_C, SLS_TRYRESTARTRULE_LINEAR, SLS_TRYRESTARTRULE_LINEAR);
	TRRANID = currNID-1;
	params_intern_appendONode(TRRANID, "slsMaxTries");
		params_intern_appendANode(currNID-1, "slsMaxTries", SPECS_DTYPE_I, PARAM_FUZZMIN_SLSMAXTRIES, PARAM_FUZZMAX_SLSMAXTRIES);
	params_intern_appendONode(TRRANID, "slsMaxFlipsA");
		params_intern_appendANode(currNID-1, "slsMaxFlipsA", SPECS_DTYPE_R, PARAM_FUZZMIN_SLSMAXFLIPSA, PARAM_FUZZMAX_SLSMAXFLIPSA);

	//SINGLE SQRT
	params_intern_appendANode(TRRONID, parName, SPECS_DTYPE_C, SLS_TRYRESTARTRULE_SQRT, SLS_TRYRESTARTRULE_SQRT);
	TRRANID = currNID-1;
	params_intern_appendONode(TRRANID, "slsMaxTries");
		params_intern_appendANode(currNID-1, "slsMaxTries", SPECS_DTYPE_I, PARAM_FUZZMIN_SLSMAXTRIES, PARAM_FUZZMAX_SLSMAXTRIES);
	params_intern_appendONode(TRRANID, "slsMaxFlipsA");
		params_intern_appendANode(currNID-1, "slsMaxFlipsA", SPECS_DTYPE_R, PARAM_FUZZMIN_SLSMAXFLIPSA, PARAM_FUZZMAX_SLSMAXFLIPSA);
	params_intern_appendONode(TRRANID, "slsMaxFlipsB");
		params_intern_appendANode(currNID-1, "slsMaxFlipsB", SPECS_DTYPE_R, PARAM_FUZZMIN_SLSMAXFLIPSB, PARAM_FUZZMAX_SLSMAXFLIPSB);

	//SINGLE EXP
	params_intern_appendANode(TRRONID, parName, SPECS_DTYPE_C, SLS_TRYRESTARTRULE_EXP, SLS_TRYRESTARTRULE_EXP);
	TRRANID = currNID-1;
	params_intern_appendONode(TRRANID, "slsMaxTries");
		params_intern_appendANode(currNID-1, "slsMaxTries", SPECS_DTYPE_I, PARAM_FUZZMIN_SLSMAXTRIES, PARAM_FUZZMAX_SLSMAXTRIES);
	params_intern_appendONode(TRRANID, "slsMaxFlipsA");
		params_intern_appendANode(currNID-1, "slsMaxFlipsA", SPECS_DTYPE_R, PARAM_FUZZMIN_SLSMAXFLIPSA, PARAM_FUZZMAX_SLSMAXFLIPSA);
	params_intern_appendONode(TRRANID, "slsMaxFlipsB");
		params_intern_appendANode(currNID-1, "slsMaxFlipsB", SPECS_DTYPE_R, PARAM_FUZZMIN_SLSMAXFLIPSB, PARAM_FUZZMIN_SLSMAXFLIPSB);
}

void params_intern_appendSlsTRR_EMP(uint32_t TRRONID, char* parName){
	uint32_t TRRANID;

	//SINGLE CONST
	params_intern_appendANode(TRRONID, parName, SPECS_DTYPE_C, SLS_TRYRESTARTRULE_CONST, SLS_TRYRESTARTRULE_CONST);
	TRRANID = currNID-1;
	params_intern_appendONode(TRRANID, "slsMaxTries");
		params_intern_appendANode(currNID-1, "slsMaxTries", SPECS_DTYPE_I, PARAM_FUZZMIN_SLSMAXTRIES, PARAM_FUZZMAX_SLSMAXTRIES);
	params_intern_appendONode(TRRANID, "slsMaxFlipsA");
		params_intern_appendANode(currNID-1, "slsMaxFlipsA", SPECS_DTYPE_R, PARAM_FUZZMIN_SLSMAXFLIPSA, PARAM_FUZZMAX_SLSMAXFLIPSA);

	//SINGLE LINEAR
	params_intern_appendANode(TRRONID, parName, SPECS_DTYPE_C, SLS_TRYRESTARTRULE_LINEAR, SLS_TRYRESTARTRULE_LINEAR);
	TRRANID = currNID-1;
	params_intern_appendONode(TRRANID, "slsMaxTries");
		params_intern_appendANode(currNID-1, "slsMaxTries", SPECS_DTYPE_I, PARAM_FUZZMIN_SLSMAXTRIES, PARAM_FUZZMAX_SLSMAXTRIES);
	params_intern_appendONode(TRRANID, "slsMaxFlipsA");
		params_intern_appendANode(currNID-1, "slsMaxFlipsA", SPECS_DTYPE_R, PARAM_FUZZMIN_SLSMAXFLIPSA, PARAM_FUZZMAX_SLSMAXFLIPSA);

	//SINGLE SQRT
	params_intern_appendANode(TRRONID, parName, SPECS_DTYPE_C, SLS_TRYRESTARTRULE_SQRT, SLS_TRYRESTARTRULE_SQRT);
	TRRANID = currNID-1;
	params_intern_appendONode(TRRANID, "slsMaxTries");
		params_intern_appendANode(currNID-1, "slsMaxTries", SPECS_DTYPE_I, PARAM_FUZZMIN_SLSMAXTRIES, PARAM_FUZZMAX_SLSMAXTRIES);
	params_intern_appendONode(TRRANID, "slsMaxFlipsA");
		params_intern_appendANode(currNID-1, "slsMaxFlipsA", SPECS_DTYPE_R, PARAM_FUZZMIN_SLSMAXFLIPSA, PARAM_FUZZMAX_SLSMAXFLIPSA);
	params_intern_appendONode(TRRANID, "slsMaxFlipsB");
		params_intern_appendANode(currNID-1, "slsMaxFlipsB", SPECS_DTYPE_R, PARAM_FUZZMIN_SLSMAXFLIPSB, PARAM_FUZZMAX_SLSMAXFLIPSB);

	//SINGLE EXP
	params_intern_appendANode(TRRONID, parName, SPECS_DTYPE_C, SLS_TRYRESTARTRULE_EXP, SLS_TRYRESTARTRULE_EXP);
	TRRANID = currNID-1;
	params_intern_appendONode(TRRANID, "slsMaxTries");
		params_intern_appendANode(currNID-1, "slsMaxTries", SPECS_DTYPE_I, PARAM_FUZZMIN_SLSMAXTRIES, PARAM_FUZZMAX_SLSMAXTRIES);
	params_intern_appendONode(TRRANID, "slsMaxFlipsA");
		params_intern_appendANode(currNID-1, "slsMaxFlipsA", SPECS_DTYPE_R, PARAM_FUZZMIN_SLSMAXFLIPSA, PARAM_FUZZMAX_SLSMAXFLIPSA);
	params_intern_appendONode(TRRANID, "slsMaxFlipsB");
		params_intern_appendANode(currNID-1, "slsMaxFlipsB", SPECS_DTYPE_R, PARAM_FUZZMIN_SLSMAXFLIPSB, PARAM_FUZZMAX_SLSMAXFLIPSB);

}

void params_intern_appendSlsForGuideSLS(uint32_t GUIDEONID){
	uint32_t ONID;

	params_intern_appendONode(GUIDEONID, "slsPickAndFlipVarRule");
	ONID = currNID-1;
	params_intern_appendSlsPAFVR_unconditional(ONID, "slsPickAndFlipVarRule");

	params_intern_appendONode(GUIDEONID, "slsTryRestartRule");
	ONID = currNID-1;
	params_intern_appendSlsTRR_SLS(ONID, "slsTryRestartRule");
}

void params_intern_appendSlsForGuidePMPSLS(uint32_t GUIDEONID){
	uint32_t ONID;

	params_intern_appendONode(GUIDEONID, "slsPickAndFlipVarRule");
	ONID = currNID-1;
	params_intern_appendSlsPAFVR_unconditional(ONID, "slsPickAndFlipVarRule");

	params_intern_appendONode(GUIDEONID, "slsTryRestartRule");
	ONID = currNID-1;
	params_intern_appendSlsTRR_PMPSLS(ONID, "slsTryRestartRule");
}

void params_intern_appendSlsForGuideHYBRIDMSC(uint32_t GUIDEONID){
	uint32_t ONID;

	params_intern_appendONode(GUIDEONID, "slsPickAndFlipVarRule");
	ONID = currNID-1;
	params_intern_appendSlsPAFVR_unconditional(ONID, "slsPickAndFlipVarRule");

	params_intern_appendONode(GUIDEONID, "slsTryRestartRule");
	ONID = currNID-1;
	params_intern_appendSlsTRR_HYBRIDMSC(ONID, "slsTryRestartRule");
}

void params_intern_appendSlsForGuideEMP(uint32_t GUIDEONID){
	uint32_t ONID;

	params_intern_appendONode(GUIDEONID, "slsPickAndFlipVarRule");
	ONID = currNID-1;
	params_intern_appendSlsPAFVR_unconditional(ONID, "slsPickAndFlipVarRule");

	params_intern_appendONode(GUIDEONID, "slsTryRestartRule");
	ONID = currNID-1;
	params_intern_appendSlsTRR_EMP(ONID, "slsTryRestartRule");
}

#endif
