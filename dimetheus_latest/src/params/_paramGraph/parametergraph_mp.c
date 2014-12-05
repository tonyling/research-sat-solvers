/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "paramgraph.h"
#ifdef SPECS_PARAMGRAPH

void params_intern_appendMpUR_unconditional(uint32_t GUIDEONID, char* parName){
	params_intern_appendONode(GUIDEONID, "mpRho");
		params_intern_appendANode(currNID-1, "mpRho", SPECS_DTYPE_R, PARAM_FUZZMIN_MPRHO, PARAM_FUZZMAX_MPRHO);
	params_intern_appendONode(GUIDEONID, "mpSigma");
		params_intern_appendANode(currNID-1, "mpSigma", SPECS_DTYPE_R, PARAM_FUZZMIN_MPSIGMA, PARAM_FUZZMAX_MPSIGMA);
	params_intern_appendONode(GUIDEONID, "mpPhi");
		params_intern_appendANode(currNID-1, "mpPhi", SPECS_DTYPE_R, PARAM_FUZZMIN_MPPHI, PARAM_FUZZMAX_MPPHI);
	params_intern_appendONode(GUIDEONID, "mpUpsilon");
		params_intern_appendANode(currNID-1, "mpUpsilon", SPECS_DTYPE_R, PARAM_FUZZMIN_MPUPSILON, PARAM_FUZZMAX_MPUPSILON);
	params_intern_appendONode(GUIDEONID, "mpMaxConvergenceDiff");
		params_intern_appendANode(currNID-1, "mpMaxConvergenceDiff", SPECS_DTYPE_R, PARAM_FUZZMIN_MPMAXCONVERGENCEDIFF, PARAM_FUZZMAX_MPMAXCONVERGENCEDIFF);

}

void params_intern_appendMpForGuidePMPCDCL(uint32_t GUIDEONID){
	params_intern_appendMpUR_unconditional(GUIDEONID, "mpUpdateRule");
}

void params_intern_appendMpForGuidePMPSLS(uint32_t GUIDEONID){
	params_intern_appendMpUR_unconditional(GUIDEONID, "mpUpdateRule");

}

void params_intern_appendMpForGuideHYBRIDMSC(uint32_t GUIDEONID){
	params_intern_appendMpUR_unconditional(GUIDEONID, "mpUpdateRule");

}

void params_intern_appendMpForGuideEMP(uint32_t GUIDEONID){
	params_intern_appendMpUR_unconditional(GUIDEONID, "mpUpdateRule");

}

#endif
