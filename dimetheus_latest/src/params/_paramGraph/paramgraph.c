/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "paramgraph.h"
#ifdef SPECS_PARAMGRAPH
void params_intern_writeXMLHeader(){
	fprintf(param_specsGraphOutput, "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standalone=\"yes\"?>\n");
	fprintf(param_specsGraphOutput, "<parameterspace>\n");
	fprintf(param_specsGraphOutput, "    <startNode>%d</startNode>\n", SPECS_ROOTNID);
	fprintf(param_specsGraphOutput, "    <nodes "SPECS_XMLNSXSI SPECS_XSITYPE_AND">\n");
	fprintf(param_specsGraphOutput, "        <id>%d</id>\n", SPECS_ROOTNID);
	fprintf(param_specsGraphOutput, "    </nodes>\n");
}

void params_intern_writeNode_param(uint32_t type, uint32_t id, char* parName){
	//First, we write the node tag.
	fprintf(param_specsGraphOutput, "    <nodes " SPECS_XMLNSXSI);
	//Determine the type of node we are working on.
	if (type == SPECS_NTYPE_A){
		//It is an AND node.
		fprintf(param_specsGraphOutput, SPECS_XSITYPE_AND);
	} else if (type == SPECS_NTYPE_O){
		//It is an OR node.
		fprintf(param_specsGraphOutput, SPECS_XSITYPE_OR);
	}
	//We now finish the node tag.
	fprintf(param_specsGraphOutput, ">\n");

	//We then write the ID of the node.
	fprintf(param_specsGraphOutput, "        <id>%d</id>\n", id);

	//We then write the parameter name (abbreviated, without param_ in the beginning).
	fprintf(param_specsGraphOutput, "        <parameter>%s</parameter>\n", parName);

	//We close the node.
	fprintf(param_specsGraphOutput, "    </nodes>\n");
}

void params_intern_writeNode_domain(uint32_t type, uint32_t id, char* parName, uint32_t domType, float_ty low, float_ty high){
	//First, we write the node tag.
	fprintf(param_specsGraphOutput, "    <nodes " SPECS_XMLNSXSI);
	//Determine the type of node we are working on.
	if (type == SPECS_NTYPE_A){
		//It is an AND node.
		fprintf(param_specsGraphOutput, SPECS_XSITYPE_AND);
	} else if (type == SPECS_NTYPE_O){
		//It is an OR node.
		fprintf(param_specsGraphOutput, SPECS_XSITYPE_OR);
	}
	//We now finish the node tag.
	fprintf(param_specsGraphOutput, ">\n");

	//We then write the ID of the node.
	fprintf(param_specsGraphOutput, "        <id>%d</id>\n", id);

	//We then write the parameter name (abbreviated, without param_ in the beginning).
	fprintf(param_specsGraphOutput, "        <parameter>%s</parameter>\n", parName);

	//We then write the domain information.
	fprintf(param_specsGraphOutput, "        <domain ");
	if (domType == SPECS_DTYPE_I){
		//It is an integer node.
		fprintf(param_specsGraphOutput, SPECS_XSITYPE_INTEGER ">\n");
		fprintf(param_specsGraphOutput, "            <high>%d</high>\n", (int32_t)high);
		fprintf(param_specsGraphOutput, "            <low>%d</low>\n", (int32_t)low);
	} else if (domType  == SPECS_DTYPE_R){
		//It is a real node.
		fprintf(param_specsGraphOutput, SPECS_XSITYPE_REAL ">\n");
		fprintf(param_specsGraphOutput, "            <high>%f</high>\n",high);
		fprintf(param_specsGraphOutput, "            <low>%f</low>\n",low);
	} else if (domType == SPECS_DTYPE_C){
		fprintf(param_specsGraphOutput, SPECS_XSITYPE_CAT ">\n");
		uint32_t category;
		for (category = low; category <= high; ++category){
			fprintf(param_specsGraphOutput, "            <categories>%d</categories>\n",category);
		}
	}
	//We finish the domain information.
	fprintf(param_specsGraphOutput, "        </domain>\n");

	//We close the node.
	fprintf(param_specsGraphOutput, "    </nodes>\n");
}

void params_intern_writeParametersTag(uint32_t domType, float_ty low, float_ty high, char* parName){
	fprintf(param_specsGraphOutput, "    <parameters>\n");
	//Determine the type of node we are working on.
	fprintf(param_specsGraphOutput, "        <domain " SPECS_XMLNSXSI);
	if (domType == SPECS_DTYPE_I){
		//It is an integer node.
		fprintf(param_specsGraphOutput, SPECS_XSITYPE_INTEGER ">\n");
		fprintf(param_specsGraphOutput, "            <high>%d</high>\n", (int32_t)high);
		fprintf(param_specsGraphOutput, "            <low>%d</low>\n", (int32_t)low);
	} else if (domType  == SPECS_DTYPE_R){
		//It is a real node.
		fprintf(param_specsGraphOutput, SPECS_XSITYPE_REAL ">\n");
		fprintf(param_specsGraphOutput, "            <high>%f</high>\n",high);
		fprintf(param_specsGraphOutput, "            <low>%f</low>\n",low);
	} else if (domType == SPECS_DTYPE_C){
		fprintf(param_specsGraphOutput, SPECS_XSITYPE_CAT ">\n");
		uint32_t category;
		for (category = low; category <= high; ++category){
			fprintf(param_specsGraphOutput, "            <categories>%d</categories>\n",category);
		}
	}
	//We now finish the domain tag.
	fprintf(param_specsGraphOutput, "        </domain>\n");

	//Write the name tag.
	fprintf(param_specsGraphOutput, "        <name>%s</name>\n", parName);

	fprintf(param_specsGraphOutput, "    </parameters>\n");
}

void params_intern_writeEdgeTag(uint32_t sourceID, uint32_t targetID){
	fprintf(param_specsGraphOutput, "    <edges>\n");
	fprintf(param_specsGraphOutput, "      <group>0</group>\n");
	fprintf(param_specsGraphOutput, "      <source>%d</source>\n", sourceID);
	fprintf(param_specsGraphOutput, "      <target>%d</target>\n", targetID);
	fprintf(param_specsGraphOutput, "    </edges>\n");
}

void params_intern_appendONode(uint32_t parID, char* parName){
	params_intern_writeNode_param(SPECS_NTYPE_O, currNID, parName);
	params_intern_writeEdgeTag(parID, currNID);
	++currNID;
}

void params_intern_appendANode(uint32_t parID, char* parName, uint32_t dType, float_ty low, float_ty high){
	params_intern_writeNode_domain(SPECS_NTYPE_A, currNID, parName, dType, low, high);
	params_intern_writeEdgeTag(parID, currNID);
	++currNID;
}

//PREPROCESSING TREES
void params_intern_appendPrepStrategyNULL(uint32_t PSANID){
	//PREP_STRATEGY_NULL
	params_intern_appendANode(PSANID, "prepStrategy", SPECS_DTYPE_C, PREP_STRATEGY_NULL, PREP_STRATEGY_NULL);
}

void params_intern_appendPrepStrategySIMPLE(uint32_t PSANID){
	uint32_t PSONID;
	//PREP_STRATEGY_SIMPLE
	params_intern_appendANode(PSANID, "prepStrategy", SPECS_DTYPE_C, PREP_STRATEGY_SIMPLE, PREP_STRATEGY_SIMPLE);
	PSONID = currNID-1;
		params_intern_appendONode(PSONID, "prepUNHRounds");
			params_intern_appendANode(currNID-1, "prepUNHRounds", SPECS_DTYPE_I, PARAM_FUZZMIN_PREPUNHROUNDS, PARAM_FUZZMAX_PREPUNHROUNDS);
}

void params_intern_appendPrepStrategySTRONG(uint32_t PSANID){
	uint32_t PSONID;
	//PREP_STRATEGY_STRONG
	params_intern_appendANode(PSANID, "prepStrategy", SPECS_DTYPE_C, PREP_STRATEGY_STRONG, PREP_STRATEGY_STRONG);
	PSONID = currNID-1;
		params_intern_appendONode(PSONID, "prepTERNARYLitMax");
			params_intern_appendANode(currNID-1, "prepTERNARYLitMax", SPECS_DTYPE_I, PARAM_FUZZMIN_PREPTERNARYLITMAX, PARAM_FUZZMAX_PREPTERNARYLITMAX);
		params_intern_appendONode(PSONID, "prepTERNARYMax");
			params_intern_appendANode(currNID-1, "prepTERNARYMax", SPECS_DTYPE_I, PARAM_FUZZMIN_PREPTERNARYMAX, PARAM_FUZZMAX_PREPTERNARYMAX);
		params_intern_appendONode(PSONID, "prepNIVERMax");
			params_intern_appendANode(currNID-1, "prepNIVERMax", SPECS_DTYPE_I, PARAM_FUZZMIN_PREPNIVERMAX, PARAM_FUZZMAX_PREPNIVERMAX);
		params_intern_appendONode(PSONID, "prepNHBRMin");
			params_intern_appendANode(currNID-1, "prepNHBRMin", SPECS_DTYPE_I, PARAM_FUZZMIN_PREPNHBRMIN, PARAM_FUZZMAX_PREPNHBRMIN);
		params_intern_appendONode(PSONID, "prepUNHRounds");
			params_intern_appendANode(currNID-1, "prepUNHRounds", SPECS_DTYPE_I, PARAM_FUZZMIN_PREPUNHROUNDS, PARAM_FUZZMAX_PREPUNHROUNDS);
}

void params_intern_appendPrepStrategyFULL(uint32_t PSANID){
	uint32_t PSONID;
	//PREP_STRATEGY_FULL
	params_intern_appendANode(PSANID, "prepStrategy", SPECS_DTYPE_C, PREP_STRATEGY_FULL, PREP_STRATEGY_FULL);
	PSONID = currNID-1;
		params_intern_appendONode(PSONID, "prepTERNARYLitMax");
			params_intern_appendANode(currNID-1, "prepTERNARYLitMax", SPECS_DTYPE_I, PARAM_FUZZMIN_PREPTERNARYLITMAX, PARAM_FUZZMAX_PREPTERNARYLITMAX);
		params_intern_appendONode(PSONID, "prepTERNARYMax");
			params_intern_appendANode(currNID-1, "prepTERNARYMax", SPECS_DTYPE_I, PARAM_FUZZMIN_PREPTERNARYMAX, PARAM_FUZZMAX_PREPTERNARYMAX);
		params_intern_appendONode(PSONID, "prepNIVERMax");
			params_intern_appendANode(currNID-1, "prepNIVERMax", SPECS_DTYPE_I, PARAM_FUZZMIN_PREPNIVERMAX, PARAM_FUZZMAX_PREPNIVERMAX);
		params_intern_appendONode(PSONID, "prepNHBRMin");
			params_intern_appendANode(currNID-1, "prepNHBRMin", SPECS_DTYPE_I, PARAM_FUZZMIN_PREPNHBRMIN, PARAM_FUZZMAX_PREPNHBRMIN);
		params_intern_appendONode(PSONID, "prepUNHRounds");
			params_intern_appendANode(currNID-1, "prepUNHRounds", SPECS_DTYPE_I, PARAM_FUZZMIN_PREPUNHROUNDS, PARAM_FUZZMAX_PREPUNHROUNDS);
		params_intern_appendONode(PSONID, "prepGEMaxAONN");
			params_intern_appendANode(currNID-1, "prepGEMaxAONN", SPECS_DTYPE_I, PARAM_FUZZMIN_PREPGEMAXAONN, PARAM_FUZZMAX_PREPGEMAXAONN);
		params_intern_appendONode(PSONID, "prepGEMaxXOR");
			params_intern_appendANode(currNID-1, "prepGEMaxXOR", SPECS_DTYPE_I, PARAM_FUZZMIN_PREPGEMAXXOR, PARAM_FUZZMAX_PREPGEMAXXOR);
		params_intern_appendONode(PSONID, "prepACCESaturate");
			params_intern_appendANode(currNID-1, "prepACCESaturate", SPECS_DTYPE_I, PARAM_FUZZMIN_PREPACCESATURATE, PARAM_FUZZMAX_PREPACCESATURATE);
		params_intern_appendONode(PSONID, "prepACCEMax");
			params_intern_appendANode(currNID-1, "prepACCEMax", SPECS_DTYPE_I, PARAM_FUZZMIN_PREPACCEMAX, PARAM_FUZZMAX_PREPACCEMAX);
		params_intern_appendONode(PSONID, "prepACCEMinClsSize");
			params_intern_appendANode(currNID-1, "prepACCEMinClsSize", SPECS_DTYPE_I, PARAM_FUZZMIN_PREPACCEMINCLSSIZE, PARAM_FUZZMAX_PREPACCEMINCLSSIZE);
		params_intern_appendONode(PSONID, "prepReviveCls");
			params_intern_appendANode(currNID-1, "prepReviveCls", SPECS_DTYPE_I, PARAM_FUZZMIN_PREPREVIVECLS, PARAM_FUZZMAX_PREPREVIVECLS);
}

void params_intern_appendPrepStrategyRAND(uint32_t PSANID){
	//PREP_STRATEGY_RAND
	params_intern_appendANode(PSANID, "prepStrategy", SPECS_DTYPE_C, PREP_STRATEGY_RAND, PREP_STRATEGY_RAND);
}


//INPROCESSING TREES
void params_intern_appendInpStrategyNULL(uint32_t INPANID){
	params_intern_appendANode(INPANID, "inpStrategy", SPECS_DTYPE_C, INP_STRATEGY_NULL, INP_STRATEGY_NULL);
}

void params_intern_appendInpStrategyREDUCEONLY(uint32_t INPANID){
	params_intern_appendANode(INPANID, "inpStrategy", SPECS_DTYPE_C, INP_STRATEGY_REDUCEONLY, INP_STRATEGY_REDUCEONLY);
}

void params_intern_appendInpStrategySIMPLE(uint32_t INPANID){
	uint32_t INPONID;
	params_intern_appendANode(INPANID, "inpStrategy", SPECS_DTYPE_C, INP_STRATEGY_SIMPLE, INP_STRATEGY_SIMPLE);
	INPONID = currNID-1;
	params_intern_appendONode(INPONID, "inpTernaryLitMax");
		params_intern_appendANode(currNID-1, "inpTernaryLitMax", SPECS_DTYPE_I, PARAM_FUZZMIN_INPTERNARYLITMAX, PARAM_FUZZMAX_INPTERNARYLITMAX);
	params_intern_appendONode(INPONID, "inpTernaryMax");
		params_intern_appendANode(currNID-1, "inpTernaryMax", SPECS_DTYPE_I, PARAM_FUZZMIN_INPTERNARYMAX, PARAM_FUZZMAX_INPTERNARYMAX);
	params_intern_appendONode(INPONID, "inpUNHRounds");
		params_intern_appendANode(currNID-1, "inpUNHRounds", SPECS_DTYPE_I, PARAM_FUZZMIN_INPUNHROUNDS, PARAM_FUZZMAX_INPUNHROUNDS);
}

void params_intern_appendInpStrategyFULL(uint32_t INPANID){
	uint32_t INPONID;
	params_intern_appendANode(INPANID, "inpStrategy", SPECS_DTYPE_C, INP_STRATEGY_FULL, INP_STRATEGY_FULL);
	INPONID = currNID-1;
	params_intern_appendONode(INPONID, "inpTernaryLitMax");
		params_intern_appendANode(currNID-1, "inpTernaryLitMax", SPECS_DTYPE_I, PARAM_FUZZMIN_INPTERNARYLITMAX, PARAM_FUZZMAX_INPTERNARYLITMAX);
	params_intern_appendONode(INPONID, "inpTernaryMax");
		params_intern_appendANode(currNID-1, "inpTernaryMax", SPECS_DTYPE_I, PARAM_FUZZMIN_INPTERNARYMAX, PARAM_FUZZMAX_INPTERNARYMAX);
	params_intern_appendONode(INPONID, "inpUNHRounds");
		params_intern_appendANode(currNID-1, "inpUNHRounds", SPECS_DTYPE_I, PARAM_FUZZMIN_INPUNHROUNDS, PARAM_FUZZMAX_INPUNHROUNDS);
	params_intern_appendONode(INPONID, "inpGEMaxAONN");
		params_intern_appendANode(currNID-1, "inpGEMaxAONN", SPECS_DTYPE_I, PARAM_FUZZMIN_INPGEMAXAONN, PARAM_FUZZMAX_INPGEMAXAONN);
	params_intern_appendONode(INPONID, "inpGEMaxXOR");
		params_intern_appendANode(currNID-1, "inpGEMaxXOR", SPECS_DTYPE_I, PARAM_FUZZMIN_INPGEMAXXOR, PARAM_FUZZMAX_INPGEMAXXOR);
	params_intern_appendONode(INPONID, "inpStrMaxSize");
		params_intern_appendANode(currNID-1, "inpStrMaxSize", SPECS_DTYPE_I, PARAM_FUZZMIN_INPSTRMAXSIZE, PARAM_FUZZMAX_INPSTRMAXSIZE);
	params_intern_appendONode(INPONID, "inpNiverMax");
		params_intern_appendANode(currNID-1, "inpNiverMax", SPECS_DTYPE_I, PARAM_FUZZMIN_INPNIVERMAX, PARAM_FUZZMAX_INPNIVERMAX);
}

void params_intern_appendInpStrategyRAND(uint32_t INPANID){
	params_intern_appendANode(INPANID, "inpStrategy", SPECS_DTYPE_C, INP_STRATEGY_RAND, INP_STRATEGY_RAND);
}

//GUIDE BASED MODULE TREES
//MP
void params_intern_appendMpForGuidePMPCDCL(uint32_t);
void params_intern_appendMpForGuidePMPSLS(uint32_t);
void params_intern_appendMpForGuideHYBRIDMSC(uint32_t);
void params_intern_appendMpForGuideEMP(uint32_t);
//SLS
void params_intern_appendSlsForGuideSLS(uint32_t);
void params_intern_appendSlsForGuidePMPSLS(uint32_t);
void params_intern_appendSlsForGuideHYBRIDMSC(uint32_t);
void params_intern_appendSlsForGuideEMP(uint32_t);
//CDCL
void params_intern_appendCdclForGuideCDCL(uint32_t);
void params_intern_appendCdclForGuidePMPCDCL(uint32_t);
void params_intern_appendCdclForGuideHYBRIDMSC(uint32_t);


//SEARCH STRATEGY TREES
void params_intern_appendSearchStrategyCDCL(uint32_t GUIDEONID){
	params_intern_appendCdclForGuideCDCL(GUIDEONID);
}

void params_intern_appendSearchStrategySLS(uint32_t GUIDEONID){
	params_intern_appendSlsForGuideSLS(GUIDEONID);

}

void params_intern_appendSearchStrategyPMPCDCL(uint32_t GUIDEONID){
	params_intern_appendONode(GUIDEONID, "searchMIDBlockSize");
		params_intern_appendANode(currNID-1, "searchMIDBlockSize", SPECS_DTYPE_R, PARAM_FUZZMIN_SEARCHMIDBLOCKSIZE, PARAM_FUZZMAX_SEARCHMIDBLOCKSIZE);
	params_intern_appendONode(GUIDEONID, "searchImpactMaxFactor");
		params_intern_appendANode(currNID-1, "searchImpactMaxFactor", SPECS_DTYPE_R, PARAM_FUZZMIN_SEARCHIMPACTMAXFACTOR, PARAM_FUZZMAX_SEARCHIMPACTMAXFACTOR);
	params_intern_appendONode(GUIDEONID, "searchMinClsImpact");
		params_intern_appendANode(currNID-1, "searchMinClsImpact", SPECS_DTYPE_R, PARAM_FUZZMIN_SEARCHMINCLSIMPACT, PARAM_FUZZMAX_SEARCHMINCLSIMPACT);
	params_intern_appendMpForGuidePMPCDCL(GUIDEONID);
	params_intern_appendCdclForGuidePMPCDCL(GUIDEONID);
}

void params_intern_appendSearchStrategyPMPSLS(uint32_t GUIDEONID){
	params_intern_appendONode(GUIDEONID, "searchMIDBlockSize");
		params_intern_appendANode(currNID-1, "searchMIDBlockSize", SPECS_DTYPE_R, PARAM_FUZZMIN_SEARCHMIDBLOCKSIZE, PARAM_FUZZMAX_SEARCHMIDBLOCKSIZE);
	params_intern_appendONode(GUIDEONID, "searchMaxMagForSLS");
		params_intern_appendANode(currNID-1, "searchMaxMagForSLS", SPECS_DTYPE_R, PARAM_FUZZMIN_SEARCHMAXMAGFORSLS, PARAM_FUZZMAX_SEARCHMAXMAGFORSLS);
	params_intern_appendMpForGuidePMPSLS(GUIDEONID);
	params_intern_appendSlsForGuidePMPSLS(GUIDEONID);

}

void params_intern_appendSearchStrategyHYBRIDMSC(uint32_t GUIDEONID){
	params_intern_appendONode(GUIDEONID, "searchMIDBlockSize");
		params_intern_appendANode(currNID-1, "searchMIDBlockSize", SPECS_DTYPE_R, PARAM_FUZZMIN_SEARCHMIDBLOCKSIZE, PARAM_FUZZMAX_SEARCHMIDBLOCKSIZE);
	params_intern_appendONode(GUIDEONID, "searchImpactMaxFactor");
		params_intern_appendANode(currNID-1, "searchImpactMaxFactor", SPECS_DTYPE_R, PARAM_FUZZMIN_SEARCHIMPACTMAXFACTOR, PARAM_FUZZMAX_SEARCHIMPACTMAXFACTOR);
	params_intern_appendONode(GUIDEONID, "searchMinClsImpact");
		params_intern_appendANode(currNID-1, "searchMinClsImpact", SPECS_DTYPE_R, PARAM_FUZZMIN_SEARCHMINCLSIMPACT, PARAM_FUZZMAX_SEARCHMINCLSIMPACT);
	params_intern_appendONode(GUIDEONID, "searchRetreatFact");
		params_intern_appendANode(currNID-1, "searchRetreatFact", SPECS_DTYPE_R, PARAM_FUZZMIN_SEARCHRETREATFACT, PARAM_FUZZMAX_SEARCHRETREATFACT);
	params_intern_appendONode(GUIDEONID, "searchMaxMagForSLS");
		params_intern_appendANode(currNID-1, "searchMaxMagForSLS", SPECS_DTYPE_R, PARAM_FUZZMIN_SEARCHMAXMAGFORSLS, PARAM_FUZZMAX_SEARCHMAXMAGFORSLS);
	params_intern_appendMpForGuideHYBRIDMSC(GUIDEONID);
	params_intern_appendSlsForGuideHYBRIDMSC(GUIDEONID);
	params_intern_appendCdclForGuideHYBRIDMSC(GUIDEONID);

}

void params_intern_appendSearchStrategyEMP(uint32_t GUIDEONID){
	params_intern_appendONode(GUIDEONID, "searchMPWeightModSAT");
		params_intern_appendANode(currNID-1, "searchMPWeightModSAT", SPECS_DTYPE_R, PARAM_FUZZMIN_SEARCHMPWEIGHTMODSAT, PARAM_FUZZMAX_SEARCHMPWEIGHTMODUNS);
	params_intern_appendONode(GUIDEONID, "searchMPWeightModUNS");
		params_intern_appendANode(currNID-1, "searchMPWeightModUNS", SPECS_DTYPE_R, PARAM_FUZZMIN_SEARCHMPWEIGHTMODSAT, PARAM_FUZZMAX_SEARCHMPWEIGHTMODUNS);
	params_intern_appendMpForGuideEMP(GUIDEONID);
	params_intern_appendSlsForGuideEMP(GUIDEONID);

}

//GUIDE TREES
void params_intern_appendGuideCDCL(uint32_t GUIDEANID){
	uint32_t GUIDEONID;
	params_intern_appendANode(GUIDEANID, "guide", SPECS_DTYPE_C, GUIDE_CDCL, GUIDE_CDCL);
	GUIDEONID = currNID-1;
	//INP_STRATEGY_NULL
	//No inprocessing allowed, therefore no inprocessing node.
	//Append all guide specific nodes.
	params_intern_appendSearchStrategyCDCL(GUIDEONID);

}

void params_intern_appendGuideSLS(uint32_t GUIDEANID){
	uint32_t GUIDEONID;
	params_intern_appendANode(GUIDEANID, "guide", SPECS_DTYPE_C, GUIDE_SLS, GUIDE_SLS);
	GUIDEONID = currNID-1;
	//INP_STRATEGY_NULL
	//No inprocessing allowed, therefore no inprocessing node.
	//Append all guide specific nodes.
	params_intern_appendSearchStrategySLS(GUIDEONID);

}

void params_intern_appendGuidePMPCDCL(uint32_t GUIDEANID){
	uint32_t GUIDEONID, INPANID;
	params_intern_appendANode(GUIDEANID, "guide", SPECS_DTYPE_C, GUIDE_PMPCDCL, GUIDE_PMPCDCL);
	GUIDEONID = currNID-1;
	//INP_STRATEGY_NULL
	params_intern_appendONode(GUIDEONID, "inpStrategy");
	INPANID = currNID-1;
	params_intern_appendInpStrategyNULL(INPANID);
	params_intern_appendInpStrategyREDUCEONLY(INPANID);
	params_intern_appendInpStrategySIMPLE(INPANID);
	params_intern_appendInpStrategyFULL(INPANID);
	params_intern_appendInpStrategyRAND(INPANID);
	//Append all guide specific nodes.
	params_intern_appendSearchStrategyPMPCDCL(GUIDEONID);
}

void params_intern_appendGuidePMPSLS(uint32_t GUIDEANID){
	uint32_t GUIDEONID;
	params_intern_appendANode(GUIDEANID, "guide", SPECS_DTYPE_C, GUIDE_PMPSLS, GUIDE_PMPSLS);
	GUIDEONID = currNID-1;
	//INP_STRATEGY_NULL
	//No inprocessing allowed, therefore no inprocessing node.
	//Append all guide specific nodes.
	params_intern_appendSearchStrategyPMPSLS(GUIDEONID);

}

void params_intern_appendGuideHYBRIDMSC(uint32_t GUIDEANID){
	uint32_t GUIDEONID, INPANID;
	params_intern_appendANode(GUIDEANID, "guide", SPECS_DTYPE_C, GUIDE_HYBRIDMSC, GUIDE_HYBRIDMSC);
	GUIDEONID = currNID-1;
	//INP_STRATEGY_NULL
	params_intern_appendONode(GUIDEONID, "inpStrategy");
	INPANID = currNID-1;
	params_intern_appendInpStrategyNULL(INPANID);
	params_intern_appendInpStrategyREDUCEONLY(INPANID);
	params_intern_appendInpStrategySIMPLE(INPANID);
	params_intern_appendInpStrategyFULL(INPANID);
	params_intern_appendInpStrategyRAND(INPANID);
	//Append all guide specific nodes.
	params_intern_appendSearchStrategyHYBRIDMSC(GUIDEONID);
}

void params_intern_appendGuideEMP(uint32_t GUIDEANID){
	uint32_t GUIDEONID;
	params_intern_appendANode(GUIDEANID, "guide", SPECS_DTYPE_C, GUIDE_EMP, GUIDE_EMP);
	GUIDEONID = currNID-1;
	//INP_STRATEGY_NULL
	//No inprocessing allowed, therefore no inprocessing node.
	//Append all guide specific nodes.
	params_intern_appendSearchStrategyEMP(GUIDEONID);

}


//TOTAL TREE CREATION METHOD
void params_intern_writeNodesTags(){
	uint32_t PSANID, GUIDEANID;

	//Classification strategy, turn off classification.
	params_intern_appendONode(SPECS_ROOTNID, "classifyStrategy");
		params_intern_appendANode(currNID-1, "classifyStrategy", SPECS_DTYPE_C, CLASSIFY_STRATEGY_NULL, CLASSIFY_STRATEGY_NULL);

	//Adaptation strategy, turn off adaptation.
	params_intern_appendONode(SPECS_ROOTNID, "adaptStrategy");
		params_intern_appendANode(currNID-1, "adaptStrategy", SPECS_DTYPE_C, ADAPT_STRATEGY_NULL, ADAPT_STRATEGY_NULL);

	//Preprocessing strategy, provide all allowed.
	params_intern_appendONode(SPECS_ROOTNID, "prepStrategy");
	PSANID = currNID-1;
	params_intern_appendPrepStrategyNULL(PSANID);
	params_intern_appendPrepStrategySIMPLE(PSANID);
	params_intern_appendPrepStrategySTRONG(PSANID);
	params_intern_appendPrepStrategyFULL(PSANID);
	params_intern_appendPrepStrategyRAND(PSANID);

	//Guides, provide all allowed.
	params_intern_appendONode(SPECS_ROOTNID, "guide");
	GUIDEANID = currNID-1;
	params_intern_appendGuideCDCL(GUIDEANID);
	params_intern_appendGuideSLS(GUIDEANID);
	params_intern_appendGuidePMPCDCL(GUIDEANID);
	params_intern_appendGuidePMPSLS(GUIDEANID);
	params_intern_appendGuideHYBRIDMSC(GUIDEANID);
	params_intern_appendGuideEMP(GUIDEANID);
}

//TOTAL PARAMETER CREATION METHOD
void params_intern_writeParametersTags(){
	//Just a boring collection of all parameters with their domain type and high/low bounds.
	//STRATEGIES
	params_intern_writeParametersTag(SPECS_DTYPE_C, PARAM_FUZZMIN_CLASSIFYSTRATEGY, PARAM_FUZZMAX_CLASSIFYSTRATEGY, "classifyStrategy");
	params_intern_writeParametersTag(SPECS_DTYPE_C, PARAM_FUZZMIN_ADAPTSTRATEGY, PARAM_FUZZMAX_ADAPTSTRATEGY, "adaptStrategy");
	params_intern_writeParametersTag(SPECS_DTYPE_C, PARAM_FUZZMIN_PREPSTRATEGY, PARAM_FUZZMAX_PREPSTRATEGY, "prepStrategy");
	params_intern_writeParametersTag(SPECS_DTYPE_C, PARAM_FUZZMIN_INPSTRATEGY, PARAM_FUZZMAX_INPSTRATEGY, "inpStrategy");

	//GUIDE
	params_intern_writeParametersTag(SPECS_DTYPE_C, PARAM_FUZZMIN_GUIDE, PARAM_FUZZMAX_GUIDE, "guide");

	//PARAMETERS
	//Preprocessing
	params_intern_writeParametersTag(SPECS_DTYPE_I, PARAM_FUZZMIN_PREPTERNARYLITMAX, PARAM_FUZZMAX_PREPTERNARYLITMAX, "prepTERNARYLitMax");
	params_intern_writeParametersTag(SPECS_DTYPE_I, PARAM_FUZZMIN_PREPTERNARYMAX, PARAM_FUZZMAX_PREPTERNARYMAX, "prepTERNARYMax");
	params_intern_writeParametersTag(SPECS_DTYPE_I, PARAM_FUZZMIN_PREPNIVERMAX, PARAM_FUZZMAX_PREPNIVERMAX, "prepNIVERMax");
	params_intern_writeParametersTag(SPECS_DTYPE_I, PARAM_FUZZMIN_PREPNHBRMIN, PARAM_FUZZMAX_PREPNHBRMIN, "prepNHBRMin");
	params_intern_writeParametersTag(SPECS_DTYPE_I, PARAM_FUZZMIN_PREPUNHROUNDS, PARAM_FUZZMAX_PREPUNHROUNDS, "prepUNHRounds");
	params_intern_writeParametersTag(SPECS_DTYPE_I, PARAM_FUZZMIN_PREPGEMAXAONN, PARAM_FUZZMAX_PREPGEMAXAONN, "prepGEMaxAONN");
	params_intern_writeParametersTag(SPECS_DTYPE_I, PARAM_FUZZMIN_PREPGEMAXXOR, PARAM_FUZZMAX_PREPGEMAXXOR, "prepGEMaxXOR");
	params_intern_writeParametersTag(SPECS_DTYPE_I, PARAM_FUZZMIN_PREPACCESATURATE, PARAM_FUZZMAX_PREPACCESATURATE, "prepACCESaturate");
	params_intern_writeParametersTag(SPECS_DTYPE_I, PARAM_FUZZMIN_PREPACCEMAX, PARAM_FUZZMAX_PREPACCEMAX, "prepACCEMax");
	params_intern_writeParametersTag(SPECS_DTYPE_I, PARAM_FUZZMIN_PREPACCEMINCLSSIZE, PARAM_FUZZMAX_PREPACCEMINCLSSIZE, "prepACCEMinClsSize");
	params_intern_writeParametersTag(SPECS_DTYPE_I, PARAM_FUZZMIN_PREPREVIVECLS, PARAM_FUZZMAX_PREPREVIVECLS, "prepReviveCls");

	//Inprocessing
	params_intern_writeParametersTag(SPECS_DTYPE_I, PARAM_FUZZMIN_INPTERNARYLITMAX, PARAM_FUZZMAX_INPTERNARYLITMAX, "inpTernaryLitMax");
	params_intern_writeParametersTag(SPECS_DTYPE_I, PARAM_FUZZMIN_INPTERNARYMAX, PARAM_FUZZMAX_INPTERNARYMAX, "inpTernaryMax");
	params_intern_writeParametersTag(SPECS_DTYPE_I, PARAM_FUZZMIN_INPUNHROUNDS, PARAM_FUZZMAX_INPUNHROUNDS, "inpUNHRounds");
	params_intern_writeParametersTag(SPECS_DTYPE_I, PARAM_FUZZMIN_INPGEMAXAONN, PARAM_FUZZMAX_INPGEMAXAONN, "inpGEMaxAONN");
	params_intern_writeParametersTag(SPECS_DTYPE_I, PARAM_FUZZMIN_INPGEMAXXOR, PARAM_FUZZMAX_INPGEMAXXOR, "inpGEMaxXOR");
	params_intern_writeParametersTag(SPECS_DTYPE_I, PARAM_FUZZMIN_INPSTRMAXSIZE, PARAM_FUZZMAX_INPSTRMAXSIZE, "inpStrMaxSize");
	params_intern_writeParametersTag(SPECS_DTYPE_I, PARAM_FUZZMIN_INPNIVERMAX, PARAM_FUZZMAX_INPNIVERMAX, "inpNiverMax");

	//Search
	params_intern_writeParametersTag(SPECS_DTYPE_R, PARAM_FUZZMIN_SEARCHMIDBLOCKSIZE, PARAM_FUZZMAX_SEARCHMIDBLOCKSIZE, "searchMIDBlockSize");
	params_intern_writeParametersTag(SPECS_DTYPE_R, PARAM_FUZZMIN_SEARCHIMPACTMAXFACTOR, PARAM_FUZZMAX_SEARCHIMPACTMAXFACTOR, "searchImpactMaxFactor");
	params_intern_writeParametersTag(SPECS_DTYPE_R, PARAM_FUZZMIN_SEARCHMINCLSIMPACT, PARAM_FUZZMAX_SEARCHMINCLSIMPACT, "searchMinClsImpact");
	params_intern_writeParametersTag(SPECS_DTYPE_R, PARAM_FUZZMIN_SEARCHMAXMAGFORSLS, PARAM_FUZZMAX_SEARCHMAXMAGFORSLS, "searchMaxMagForSLS");
	params_intern_writeParametersTag(SPECS_DTYPE_R, PARAM_FUZZMIN_SEARCHRETREATFACT, PARAM_FUZZMAX_SEARCHRETREATFACT, "searchRetreatFact");
	params_intern_writeParametersTag(SPECS_DTYPE_R, PARAM_FUZZMIN_SEARCHMPWEIGHTMODSAT, PARAM_FUZZMAX_SEARCHMPWEIGHTMODSAT, "searchMPWeightModSAT");
	params_intern_writeParametersTag(SPECS_DTYPE_R, PARAM_FUZZMIN_SEARCHMPWEIGHTMODUNS, PARAM_FUZZMAX_SEARCHMPWEIGHTMODUNS, "searchMPWeightModUNS");

	//MP
	params_intern_writeParametersTag(SPECS_DTYPE_R, PARAM_FUZZMIN_MPRHO, PARAM_FUZZMAX_MPRHO, "mpRho");
	params_intern_writeParametersTag(SPECS_DTYPE_R, PARAM_FUZZMIN_MPSIGMA, PARAM_FUZZMAX_MPSIGMA, "mpSigma");
	params_intern_writeParametersTag(SPECS_DTYPE_R, PARAM_FUZZMIN_MPPHI, PARAM_FUZZMAX_MPPHI, "mpPhi");
	params_intern_writeParametersTag(SPECS_DTYPE_R, PARAM_FUZZMIN_MPUPSILON, PARAM_FUZZMAX_MPUPSILON, "mpUpsilon");
	params_intern_writeParametersTag(SPECS_DTYPE_R, PARAM_FUZZMIN_MPMAXCONVERGENCEDIFF, PARAM_FUZZMAX_MPMAXCONVERGENCEDIFF, "mpMaxConvergenceDiff");

	//SLS
	params_intern_writeParametersTag(SPECS_DTYPE_C, PARAM_FUZZMIN_SLSPICKANDFLIPVARRULE, PARAM_FUZZMAX_SLSPICKANDFLIPVARRULE, "slsPickAndFlipVarRule");
	params_intern_writeParametersTag(SPECS_DTYPE_R, PARAM_FUZZMIN_SLSPROBSATCB, PARAM_FUZZMAX_SLSPROBSATCB, "slsProbsatCB");
	params_intern_writeParametersTag(SPECS_DTYPE_R, PARAM_FUZZMIN_SLSPROBSATCBSHIFT, PARAM_FUZZMAX_SLSPROBSATCBSHIFT, "slsProbsatCBShift");

	params_intern_writeParametersTag(SPECS_DTYPE_C, PARAM_FUZZMIN_SLSTRYRESTARTRULE, PARAM_FUZZMAX_SLSTRYRESTARTRULE, "slsTryRestartRule");
	params_intern_writeParametersTag(SPECS_DTYPE_I, PARAM_FUZZMIN_SLSMAXTRIES, PARAM_FUZZMAX_SLSMAXTRIES, "slsMaxTries");
	params_intern_writeParametersTag(SPECS_DTYPE_R, PARAM_FUZZMIN_SLSMAXFLIPSA, PARAM_FUZZMAX_SLSMAXFLIPSA, "slsMaxFlipsA");
	params_intern_writeParametersTag(SPECS_DTYPE_R, PARAM_FUZZMIN_SLSMAXFLIPSB, PARAM_FUZZMAX_SLSMAXFLIPSB, "slsMaxFlipsB");

	//CDCL
	params_intern_writeParametersTag(SPECS_DTYPE_R, PARAM_FUZZMIN_CDCLSELECTVARACTINCBUMP, PARAM_FUZZMAX_CDCLSELECTVARACTINCBUMP, "cdclSelectVarActIncBump");
	params_intern_writeParametersTag(SPECS_DTYPE_R, PARAM_FUZZMIN_CDCLSELECTVARRANDPROB, PARAM_FUZZMAX_CDCLSELECTVARRANDPROB, "cdclSelectVarRandProb");

	params_intern_writeParametersTag(SPECS_DTYPE_C, PARAM_FUZZMIN_CDCLSELECTVARACTINIT, PARAM_FUZZMAX_CDCLSELECTVARACTINIT, "cdclSelectVarActInit");

	params_intern_writeParametersTag(SPECS_DTYPE_C, PARAM_FUZZMIN_CDCLSELECTDIRRULE, PARAM_FUZZMAX_CDCLSELECTDIRRULE, "cdclSelectDirRule");
	params_intern_writeParametersTag(SPECS_DTYPE_I, PARAM_FUZZMIN_CDCLSELECTDIRMINFLIPDIST, PARAM_FUZZMAX_CDCLSELECTDIRMINFLIPDIST, "cdclSelectDirMinFlipDist");

	params_intern_writeParametersTag(SPECS_DTYPE_C, PARAM_FUZZMIN_CDCLSELECTDIRDIRINIT, PARAM_FUZZMAX_CDCLSELECTDIRDIRINIT, "cdclSelectDirDirInit");

	params_intern_writeParametersTag(SPECS_DTYPE_C, PARAM_FUZZMIN_CDCLCONFLICTANALYSISRULE, PARAM_FUZZMAX_CDCLCONFLICTANALYSISRULE, "cdclConflictAnalysisRule");

	params_intern_writeParametersTag(SPECS_DTYPE_C, PARAM_FUZZMIN_CDCLSTRLEARNEDRULE, PARAM_FUZZMAX_CDCLSTRLEARNEDRULE, "cdclStrLearnedRule");

	params_intern_writeParametersTag(SPECS_DTYPE_C, PARAM_FUZZMIN_CDCLSTROTHERRULE, PARAM_FUZZMAX_CDCLSTROTHERRULE, "cdclStrOtherRule");

	params_intern_writeParametersTag(SPECS_DTYPE_C, PARAM_FUZZMIN_CDCLRESTARTRULE, PARAM_FUZZMAX_CDCLRESTARTRULE, "cdclRestartRule");
	params_intern_writeParametersTag(SPECS_DTYPE_I, PARAM_FUZZMIN_CDCLRESTARTLUBYUNIT, PARAM_FUZZMAX_CDCLRESTARTLUBYUNIT, "cdclRestartLubyUnit");
	params_intern_writeParametersTag(SPECS_DTYPE_R, PARAM_FUZZMIN_CDCLRESTARTVARAGIINCBUMP, PARAM_FUZZMAX_CDCLRESTARTVARAGIINCBUMP, "cdclRestartVarAgiIncBump");
	params_intern_writeParametersTag(SPECS_DTYPE_R, PARAM_FUZZMIN_CDCLRESTARTMAXAGIFRAC, PARAM_FUZZMAX_CDCLRESTARTMAXAGIFRAC, "cdclRestartMaxAgiFrac");
	params_intern_writeParametersTag(SPECS_DTYPE_R, PARAM_FUZZMIN_CDCLRESTARTSTAGMAXFRAC, PARAM_FUZZMAX_CDCLRESTARTSTAGMAXFRAC, "cdclRestartStagMaxFrac");
	params_intern_writeParametersTag(SPECS_DTYPE_I, PARAM_FUZZMIN_CDCLRESTARTINNEROUTERINIT, PARAM_FUZZMAX_CDCLRESTARTINNEROUTERINIT, "cdclRestartInnerouterInit");
	params_intern_writeParametersTag(SPECS_DTYPE_R, PARAM_FUZZMIN_CDCLRESTARTINNEROUTERINC, PARAM_FUZZMAX_CDCLRESTARTINNEROUTERINC, "cdclRestartInnerouterInc");
	params_intern_writeParametersTag(SPECS_DTYPE_R, PARAM_FUZZMIN_CDCLRESTARTMAXAGIOUTFRAC, PARAM_FUZZMAX_CDCLRESTARTMAXAGIOUTFRAC, "cdclRestartMaxAgiOutFrac");

	params_intern_writeParametersTag(SPECS_DTYPE_C, PARAM_FUZZMIN_CDCLMAINTENANCERULE, PARAM_FUZZMAX_CDCLMAINTENANCERULE, "cdclMaintenanceRule");
	params_intern_writeParametersTag(SPECS_DTYPE_R, PARAM_FUZZMIN_CDCLMAINTENANCEINITIAL, PARAM_FUZZMAX_CDCLMAINTENANCEINITIAL, "cdclMaintenanceInitial");
	params_intern_writeParametersTag(SPECS_DTYPE_R, PARAM_FUZZMIN_CDCLMAINTENANCEINCREASE, PARAM_FUZZMAX_CDCLMAINTENANCEINCREASE, "cdclMaintenanceIncrease");
	params_intern_writeParametersTag(SPECS_DTYPE_R, PARAM_FUZZMIN_CDCLMAINTENANCEACTINCBUMP, PARAM_FUZZMAX_CDCLMAINTENANCEACTINCBUMP, "cdclMaintenanceActIncBump");
	params_intern_writeParametersTag(SPECS_DTYPE_I, PARAM_FUZZMIN_CDCLMAINTENANCEBASE, PARAM_FUZZMAX_CDCLMAINTENANCEBASE, "cdclMaintenanceBase");
	params_intern_writeParametersTag(SPECS_DTYPE_I, PARAM_FUZZMIN_CDCLMAINTENANCEBOOST, PARAM_FUZZMAX_CDCLMAINTENANCEBOOST, "cdclMaintenanceBoost");
	params_intern_writeParametersTag(SPECS_DTYPE_R, PARAM_FUZZMIN_CDCLMAINTENANCEMINDEV, PARAM_FUZZMAX_CDCLMAINTENANCEMINDEV, "cdclMaintenanceMinDev");
	params_intern_writeParametersTag(SPECS_DTYPE_R, PARAM_FUZZMIN_CDCLMAINTENANCEMAXAVG, PARAM_FUZZMAX_CDCLMAINTENANCEMAXAVG, "cdclMaintenanceMaxAvg");

	params_intern_writeParametersTag(SPECS_DTYPE_C, PARAM_FUZZMIN_CDCLINPROCESSINGRULE, PARAM_FUZZMAX_CDCLINPROCESSINGRULE, "cdclInprocessingRule");
	params_intern_writeParametersTag(SPECS_DTYPE_R, PARAM_FUZZMIN_CDCLIRMINCONFDISTANCE, PARAM_FUZZMAX_CDCLIRMINCONFDISTANCE, "cdclIRMinConfDistance");
	params_intern_writeParametersTag(SPECS_DTYPE_I, PARAM_FUZZMIN_CDCLIRUNHROUNDS, PARAM_FUZZMAX_CDCLIRUNHROUNDS, "cdclIRUNHRounds");
	params_intern_writeParametersTag(SPECS_DTYPE_I, PARAM_FUZZMIN_CDCLIRTERNARYLITMAX, PARAM_FUZZMAX_CDCLIRTERNARYLITMAX, "cdclIRTERNARYLitMax");
	params_intern_writeParametersTag(SPECS_DTYPE_I, PARAM_FUZZMIN_CDCLIRTERNARYMAX, PARAM_FUZZMAX_CDCLIRTERNARYMAX, "cdclIRTERNARYMax");
	params_intern_writeParametersTag(SPECS_DTYPE_I, PARAM_FUZZMIN_CDCLIRGEMAXAONN, PARAM_FUZZMAX_CDCLIRGEMAXAONN, "cdclIRGEMaxAONN");
	params_intern_writeParametersTag(SPECS_DTYPE_I, PARAM_FUZZMIN_CDCLIRGEMAXXOR, PARAM_FUZZMAX_CDCLIRGEMAXXOR, "cdclIRGEMaxXOR");
	params_intern_writeParametersTag(SPECS_DTYPE_I, PARAM_FUZZMIN_CDCLIRSTRMAXSIZE, PARAM_FUZZMAX_CDCLIRSTRMAXSIZE, "cdclIRStrMaxSize");
	params_intern_writeParametersTag(SPECS_DTYPE_I, PARAM_FUZZMIN_CDCLIRNIVERMAX, PARAM_FUZZMAX_CDCLIRNIVERMAX, "cdclIRNIVERMax");
}

void params_intern_writeXMLFooter(){
	fprintf(param_specsGraphOutput, "</parameterspace>\n");
}

void params_extern_writeParameterGraph(char* prefix){
	//This method creates and writes to file param_specsGraphOutput the parameter graph for EDACC of all tunable parameters.
	#ifdef VERBOSE_PARAMS
	printf("%s Checking if XML file is open and ready.\n", prefix);
	#endif
	if (param_specsGraphOutput == NULL){
		printf("%s   No filename given. I assume you do not want the graph.\n", prefix);
		return;
	}

	#ifdef VERBOSE_PARAMS
	printf("%s Writing XML header.\n", prefix);
	#endif
	params_intern_writeXMLHeader();
	currNID = SPECS_NODEFID;

	#ifdef VERBOSE_PARAMS
	printf("%s Writing <nodes> tags.\n", prefix);
	#endif
	params_intern_writeNodesTags();
	#ifdef VERBOSE_PARAMS
	printf("%s   I wrote %d nodes.\n", prefix, currNID-1);
	#endif

	#ifdef VERBOSE_PARAMS
	printf("%s Writing <parameters> tags.\n", prefix);
	#endif
	params_intern_writeParametersTags();

	#ifdef VERBOSE_PARAMS
	printf("%s Writing XML footer.\n", prefix);
	#endif
	params_intern_writeXMLFooter();

	#ifdef VERBOSE_PARAMS
	printf("%s Closing XML file.\n", prefix);
	#endif
	fclose(param_specsGraphOutput);
	param_specsGraphOutput = NULL;
}
#endif
