/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "paramgraph.h"
#ifdef SPECS_PARAMGRAPH

void params_intern_appendCdclCAR_unconditional(uint32_t CDCLCARONID, char* parName){
	//FIRSTUIP
	params_intern_appendANode(CDCLCARONID, parName, SPECS_DTYPE_C, CDCL_CONFLICTANALYSISRULE_FIRSTUIP, CDCL_CONFLICTANALYSISRULE_FIRSTUIP);
	//LASTUIP
	params_intern_appendANode(CDCLCARONID, parName, SPECS_DTYPE_C, CDCL_CONFLICTANALYSISRULE_LASTUIP, CDCL_CONFLICTANALYSISRULE_LASTUIP);
	//ALLUIP
	params_intern_appendANode(CDCLCARONID, parName, SPECS_DTYPE_C, CDCL_CONFLICTANALYSISRULE_ALLUIP, CDCL_CONFLICTANALYSISRULE_ALLUIP);
}

void params_intern_appendCdclSLR_unconditional(uint32_t CDCLSLRONID, char* parName){
	//LOCAL
	params_intern_appendANode(CDCLSLRONID, parName, SPECS_DTYPE_C, CDCL_STRLEARNEDRULE_LOCAL, CDCL_STRLEARNEDRULE_LOCAL);
	//LOCALREC
	params_intern_appendANode(CDCLSLRONID, parName, SPECS_DTYPE_C, CDCL_STRLEARNEDRULE_LOCALREC, CDCL_STRLEARNEDRULE_LOCALREC);
}

void params_intern_appendCdclSOR_unconditional(uint32_t CDCLSORONID, char* parName){
	//NULL
	params_intern_appendANode(CDCLSORONID, parName, SPECS_DTYPE_C, CDCL_STROTHERRULE_NULL, CDCL_STROTHERRULE_NULL);
	//WTWO
	params_intern_appendANode(CDCLSORONID, parName, SPECS_DTYPE_C, CDCL_STROTHERRULE_WTWO, CDCL_STROTHERRULE_WTWO);
}

void params_intern_appendCdclRR_unconditional(uint32_t CDCLRRONID, char* parName){
	uint32_t CDCLRRANID;
	//NULL
	params_intern_appendANode(CDCLRRONID, parName, SPECS_DTYPE_C, CDCL_RESTARTRULE_NULL, CDCL_RESTARTRULE_NULL);
	//LUBY
	params_intern_appendANode(CDCLRRONID, parName, SPECS_DTYPE_C, CDCL_RESTARTRULE_LUBY, CDCL_RESTARTRULE_LUBY);
	CDCLRRANID = currNID-1;
	params_intern_appendONode(CDCLRRANID, "cdclRestartLubyUnit");
		params_intern_appendANode(currNID-1, "cdclRestartLubyUnit", SPECS_DTYPE_I, PARAM_FUZZMIN_CDCLRESTARTLUBYUNIT, PARAM_FUZZMAX_CDCLRESTARTLUBYUNIT);
	//LUBY AGILITY
	params_intern_appendANode(CDCLRRONID, parName, SPECS_DTYPE_C, CDCL_RESTARTRULE_LUBYAGILITY, CDCL_RESTARTRULE_LUBYAGILITY);
	CDCLRRANID = currNID-1;
	params_intern_appendONode(CDCLRRANID, "cdclRestartLubyUnit");
		params_intern_appendANode(currNID-1, "cdclRestartLubyUnit", SPECS_DTYPE_I, PARAM_FUZZMIN_CDCLRESTARTLUBYUNIT, PARAM_FUZZMAX_CDCLRESTARTLUBYUNIT);
	params_intern_appendONode(CDCLRRANID, "cdclRestartVarAgiIncBump");
		params_intern_appendANode(currNID-1, "cdclRestartVarAgiIncBump", SPECS_DTYPE_R, PARAM_FUZZMIN_CDCLRESTARTVARAGIINCBUMP, PARAM_FUZZMAX_CDCLRESTARTVARAGIINCBUMP);
	params_intern_appendONode(CDCLRRANID, "cdclRestartMaxAgiFrac");
		params_intern_appendANode(currNID-1, "cdclRestartMaxAgiFrac", SPECS_DTYPE_R, PARAM_FUZZMIN_CDCLRESTARTMAXAGIFRAC, PARAM_FUZZMAX_CDCLRESTARTMAXAGIFRAC);
	//LUBY STAG
	params_intern_appendANode(CDCLRRONID, parName, SPECS_DTYPE_C, CDCL_RESTARTRULE_LUBYSTAG, CDCL_RESTARTRULE_LUBYSTAG);
	CDCLRRANID = currNID-1;
	params_intern_appendONode(CDCLRRANID, "cdclRestartLubyUnit");
		params_intern_appendANode(currNID-1, "cdclRestartLubyUnit", SPECS_DTYPE_I, PARAM_FUZZMIN_CDCLRESTARTLUBYUNIT, PARAM_FUZZMAX_CDCLRESTARTLUBYUNIT);
	params_intern_appendONode(CDCLRRANID, "cdclRestartStagMaxFrac");
		params_intern_appendANode(currNID-1, "cdclRestartStagMaxFrac", SPECS_DTYPE_R, PARAM_FUZZMIN_CDCLRESTARTSTAGMAXFRAC, PARAM_FUZZMAX_CDCLRESTARTSTAGMAXFRAC);
	//INNEROUTER
	params_intern_appendANode(CDCLRRONID, parName, SPECS_DTYPE_C, CDCL_RESTARTRULE_INNEROUTER, CDCL_RESTARTRULE_INNEROUTER);
	CDCLRRANID = currNID-1;
	params_intern_appendONode(CDCLRRANID, "cdclRestartInnerouterInit");
		params_intern_appendANode(currNID-1, "cdclRestartInnerouterInit", SPECS_DTYPE_I, PARAM_FUZZMIN_CDCLRESTARTINNEROUTERINIT, PARAM_FUZZMAX_CDCLRESTARTINNEROUTERINIT);
	params_intern_appendONode(CDCLRRANID, "cdclRestartInnerouterInc");
		params_intern_appendANode(currNID-1, "cdclRestartInnerouterInc", SPECS_DTYPE_R, PARAM_FUZZMIN_CDCLRESTARTINNEROUTERINC, PARAM_FUZZMAX_CDCLRESTARTINNEROUTERINC);
	//INNEROUTER AGILITY
	params_intern_appendANode(CDCLRRONID, parName, SPECS_DTYPE_C, CDCL_RESTARTRULE_INNEROUTERAGILITY, CDCL_RESTARTRULE_INNEROUTERAGILITY);
	CDCLRRANID = currNID-1;
	params_intern_appendONode(CDCLRRANID, "cdclRestartInnerouterInit");
		params_intern_appendANode(currNID-1, "cdclRestartInnerouterInit", SPECS_DTYPE_I, PARAM_FUZZMIN_CDCLRESTARTINNEROUTERINIT, PARAM_FUZZMAX_CDCLRESTARTINNEROUTERINIT);
	params_intern_appendONode(CDCLRRANID, "cdclRestartInnerouterInc");
		params_intern_appendANode(currNID-1, "cdclRestartInnerouterInc", SPECS_DTYPE_R, PARAM_FUZZMIN_CDCLRESTARTINNEROUTERINC, PARAM_FUZZMAX_CDCLRESTARTINNEROUTERINC);
	params_intern_appendONode(CDCLRRANID, "cdclRestartVarAgiIncBump");
		params_intern_appendANode(currNID-1, "cdclRestartVarAgiIncBump", SPECS_DTYPE_R, PARAM_FUZZMIN_CDCLRESTARTVARAGIINCBUMP, PARAM_FUZZMAX_CDCLRESTARTVARAGIINCBUMP);
	params_intern_appendONode(CDCLRRANID, "cdclRestartMaxAgiFrac");
		params_intern_appendANode(currNID-1, "cdclRestartMaxAgiFrac", SPECS_DTYPE_R, PARAM_FUZZMIN_CDCLRESTARTMAXAGIFRAC, PARAM_FUZZMAX_CDCLRESTARTMAXAGIFRAC);
	params_intern_appendONode(CDCLRRANID, "cdclRestartMaxAgiOutFrac");
		params_intern_appendANode(currNID-1, "cdclRestartMaxAgiOutFrac", SPECS_DTYPE_R, PARAM_FUZZMIN_CDCLRESTARTMAXAGIOUTFRAC, PARAM_FUZZMAX_CDCLRESTARTMAXAGIOUTFRAC);
	//INNEROUTER STAG
	params_intern_appendANode(CDCLRRONID, parName, SPECS_DTYPE_C, CDCL_RESTARTRULE_INNEROUTERSTAG, CDCL_RESTARTRULE_INNEROUTERSTAG);
	CDCLRRANID = currNID-1;
	params_intern_appendONode(CDCLRRANID, "cdclRestartInnerouterInit");
		params_intern_appendANode(currNID-1, "cdclRestartInnerouterInit", SPECS_DTYPE_I, PARAM_FUZZMIN_CDCLRESTARTINNEROUTERINIT, PARAM_FUZZMAX_CDCLRESTARTINNEROUTERINIT);
	params_intern_appendONode(CDCLRRANID, "cdclRestartInnerouterInc");
		params_intern_appendANode(currNID-1, "cdclRestartInnerouterInc", SPECS_DTYPE_R, PARAM_FUZZMIN_CDCLRESTARTINNEROUTERINC, PARAM_FUZZMAX_CDCLRESTARTINNEROUTERINC);
	params_intern_appendONode(CDCLRRANID, "cdclRestartStagMaxFrac");
		params_intern_appendANode(currNID-1, "cdclRestartStagMaxFrac", SPECS_DTYPE_R, PARAM_FUZZMIN_CDCLRESTARTSTAGMAXFRAC, PARAM_FUZZMAX_CDCLRESTARTSTAGMAXFRAC);
}

void params_intern_appendCdclMR_unconditional(uint32_t CDCLMRONID, char* parName){
	uint32_t CDCLMRANID;
	//NULL
	params_intern_appendANode(CDCLMRONID, parName, SPECS_DTYPE_C, CDCL_MAINTENANCERULE_NULL, CDCL_MAINTENANCERULE_NULL);
	//INACTIVITY
	params_intern_appendANode(CDCLMRONID, parName, SPECS_DTYPE_C, CDCL_MAINTENANCERULE_INACTIVITY, CDCL_MAINTENANCERULE_INACTIVITY);
	CDCLMRANID = currNID-1;
	params_intern_appendONode(CDCLMRANID, "cdclMaintenanceInitial");
		params_intern_appendANode(currNID-1, "cdclMaintenanceInitial", SPECS_DTYPE_R, PARAM_FUZZMIN_CDCLMAINTENANCEINITIAL, PARAM_FUZZMAX_CDCLMAINTENANCEINITIAL);
	params_intern_appendONode(CDCLMRANID, "cdclMaintenanceIncrease");
		params_intern_appendANode(currNID-1, "cdclMaintenanceIncrease", SPECS_DTYPE_R, PARAM_FUZZMIN_CDCLMAINTENANCEINCREASE, PARAM_FUZZMAX_CDCLMAINTENANCEINCREASE);
	params_intern_appendONode(CDCLMRANID, "cdclMaintenanceActIncBump");
		params_intern_appendANode(currNID-1, "cdclMaintenanceActIncBump", SPECS_DTYPE_R, PARAM_FUZZMIN_CDCLMAINTENANCEACTINCBUMP, PARAM_FUZZMAX_CDCLMAINTENANCEACTINCBUMP);
	//LBD
	params_intern_appendANode(CDCLMRONID, parName, SPECS_DTYPE_C, CDCL_MAINTENANCERULE_LBD, CDCL_MAINTENANCERULE_LBD);
	CDCLMRANID = currNID-1;
	params_intern_appendONode(CDCLMRANID, "cdclMaintenanceBase");
		params_intern_appendANode(currNID-1, "cdclMaintenanceBase", SPECS_DTYPE_I, PARAM_FUZZMIN_CDCLMAINTENANCEBASE, PARAM_FUZZMAX_CDCLMAINTENANCEBASE);
	params_intern_appendONode(CDCLMRANID, "cdclMaintenanceBoost");
		params_intern_appendANode(currNID-1, "cdclMaintenanceBoost", SPECS_DTYPE_I, PARAM_FUZZMIN_CDCLMAINTENANCEBOOST, PARAM_FUZZMAX_CDCLMAINTENANCEBOOST);
	//LBDINACT
	params_intern_appendANode(CDCLMRONID, parName, SPECS_DTYPE_C, CDCL_MAINTENANCERULE_LBDINACT, CDCL_MAINTENANCERULE_LBDINACT);
	CDCLMRANID = currNID-1;
	params_intern_appendONode(CDCLMRANID, "cdclMaintenanceBase");
		params_intern_appendANode(currNID-1, "cdclMaintenanceBase", SPECS_DTYPE_I, PARAM_FUZZMIN_CDCLMAINTENANCEBASE, PARAM_FUZZMAX_CDCLMAINTENANCEBASE);
	params_intern_appendONode(CDCLMRANID, "cdclMaintenanceBoost");
		params_intern_appendANode(currNID-1, "cdclMaintenanceBoost", SPECS_DTYPE_I, PARAM_FUZZMIN_CDCLMAINTENANCEBOOST, PARAM_FUZZMAX_CDCLMAINTENANCEBOOST);
	params_intern_appendONode(CDCLMRANID, "cdclMaintenanceMinDev");
		params_intern_appendANode(currNID-1, "cdclMaintenanceMinDev", SPECS_DTYPE_R, PARAM_FUZZMIN_CDCLMAINTENANCEMINDEV, PARAM_FUZZMAX_CDCLMAINTENANCEMINDEV);
	params_intern_appendONode(CDCLMRANID, "cdclMaintenanceMaxAvg");
		params_intern_appendANode(currNID-1, "cdclMaintenanceMaxAvg", SPECS_DTYPE_R, PARAM_FUZZMIN_CDCLMAINTENANCEMAXAVG, PARAM_FUZZMAX_CDCLMAINTENANCEMAXAVG);
	params_intern_appendONode(CDCLMRANID, "cdclMaintenanceActIncBump");
		params_intern_appendANode(currNID-1, "cdclMaintenanceActIncBump", SPECS_DTYPE_R, PARAM_FUZZMIN_CDCLMAINTENANCEACTINCBUMP, PARAM_FUZZMAX_CDCLMAINTENANCEACTINCBUMP);
	//LBDINACTLUBY
	params_intern_appendANode(CDCLMRONID, parName, SPECS_DTYPE_C, CDCL_MAINTENANCERULE_LBDINACTLUBY, CDCL_MAINTENANCERULE_LBDINACTLUBY);
	CDCLMRANID = currNID-1;
	params_intern_appendONode(CDCLMRANID, "cdclMaintenanceInitial");
		params_intern_appendANode(currNID-1, "cdclMaintenanceInitial", SPECS_DTYPE_R, PARAM_FUZZMIN_CDCLMAINTENANCEINITIAL, PARAM_FUZZMAX_CDCLMAINTENANCEINITIAL);
	params_intern_appendONode(CDCLMRANID, "cdclMaintenanceIncrease");
		params_intern_appendANode(currNID-1, "cdclMaintenanceIncrease", SPECS_DTYPE_R, PARAM_FUZZMIN_CDCLMAINTENANCEINCREASE, PARAM_FUZZMAX_CDCLMAINTENANCEINCREASE);
	params_intern_appendONode(CDCLMRANID, "cdclMaintenanceActIncBump");
		params_intern_appendANode(currNID-1, "cdclMaintenanceActIncBump", SPECS_DTYPE_R, PARAM_FUZZMIN_CDCLMAINTENANCEACTINCBUMP, PARAM_FUZZMAX_CDCLMAINTENANCEACTINCBUMP);
	params_intern_appendONode(CDCLMRANID, "cdclMaintenanceMinDev");
		params_intern_appendANode(currNID-1, "cdclMaintenanceMinDev", SPECS_DTYPE_R, PARAM_FUZZMIN_CDCLMAINTENANCEMINDEV, PARAM_FUZZMAX_CDCLMAINTENANCEMINDEV);
	params_intern_appendONode(CDCLMRANID, "cdclMaintenanceMaxAvg");
		params_intern_appendANode(currNID-1, "cdclMaintenanceMaxAvg", SPECS_DTYPE_R, PARAM_FUZZMIN_CDCLMAINTENANCEMAXAVG, PARAM_FUZZMAX_CDCLMAINTENANCEMAXAVG);
}

void params_intern_appendCdclIR_unconditional(uint32_t CDCLIRONID, char* parName){
	uint32_t CDCLIRANID;
	//NULL
	params_intern_appendANode(CDCLIRONID, parName, SPECS_DTYPE_C, CDCL_INPROCESSINGRULE_NULL, CDCL_INPROCESSINGRULE_NULL);
	//NULL
	params_intern_appendANode(CDCLIRONID, parName, SPECS_DTYPE_C, CDCL_INPROCESSINGRULE_REDUCEONLY, CDCL_INPROCESSINGRULE_REDUCEONLY);
	//SIMPLE
	params_intern_appendANode(CDCLIRONID, parName, SPECS_DTYPE_C, CDCL_INPROCESSINGRULE_SIMPLE, CDCL_INPROCESSINGRULE_SIMPLE);
	CDCLIRANID = currNID-1;
	params_intern_appendONode(CDCLIRANID, "cdclIRMinConfDistance");
		params_intern_appendANode(currNID-1, "cdclIRMinConfDistance", SPECS_DTYPE_R, PARAM_FUZZMIN_CDCLIRMINCONFDISTANCE, PARAM_FUZZMAX_CDCLIRMINCONFDISTANCE);
	params_intern_appendONode(CDCLIRANID, "cdclIRUNHRounds");
		params_intern_appendANode(currNID-1, "cdclIRUNHRounds", SPECS_DTYPE_I, PARAM_FUZZMIN_CDCLIRUNHROUNDS, PARAM_FUZZMAX_CDCLIRUNHROUNDS);
	params_intern_appendONode(CDCLIRANID, "cdclIRTERNARYLitMax");
		params_intern_appendANode(currNID-1, "cdclIRTERNARYLitMax", SPECS_DTYPE_I, PARAM_FUZZMIN_CDCLIRTERNARYLITMAX, PARAM_FUZZMAX_CDCLIRTERNARYLITMAX);
	params_intern_appendONode(CDCLIRANID, "cdclIRTERNARYMax");
		params_intern_appendANode(currNID-1, "cdclIRTERNARYMax", SPECS_DTYPE_I, PARAM_FUZZMIN_CDCLIRTERNARYMAX, PARAM_FUZZMAX_CDCLIRTERNARYMAX);
	//FULL
	params_intern_appendANode(CDCLIRONID, parName, SPECS_DTYPE_C, CDCL_INPROCESSINGRULE_FULL, CDCL_INPROCESSINGRULE_FULL);
	CDCLIRANID = currNID-1;
	params_intern_appendONode(CDCLIRANID, "cdclIRMinConfDistance");
		params_intern_appendANode(currNID-1, "cdclIRMinConfDistance", SPECS_DTYPE_R, PARAM_FUZZMIN_CDCLIRMINCONFDISTANCE, PARAM_FUZZMAX_CDCLIRMINCONFDISTANCE);
	params_intern_appendONode(CDCLIRANID, "cdclIRUNHRounds");
		params_intern_appendANode(currNID-1, "cdclIRUNHRounds", SPECS_DTYPE_I, PARAM_FUZZMIN_CDCLIRUNHROUNDS, PARAM_FUZZMAX_CDCLIRUNHROUNDS);
	params_intern_appendONode(CDCLIRANID, "cdclIRTERNARYLitMax");
		params_intern_appendANode(currNID-1, "cdclIRTERNARYLitMax", SPECS_DTYPE_I, PARAM_FUZZMIN_CDCLIRTERNARYLITMAX, PARAM_FUZZMAX_CDCLIRTERNARYLITMAX);
	params_intern_appendONode(CDCLIRANID, "cdclIRTERNARYMax");
		params_intern_appendANode(currNID-1, "cdclIRTERNARYMax", SPECS_DTYPE_I, PARAM_FUZZMIN_CDCLIRTERNARYMAX, PARAM_FUZZMAX_CDCLIRTERNARYMAX);
	params_intern_appendONode(CDCLIRANID, "cdclIRGEMaxAONN");
		params_intern_appendANode(currNID-1, "cdclIRGEMaxAONN", SPECS_DTYPE_I, PARAM_FUZZMIN_CDCLIRGEMAXAONN, PARAM_FUZZMAX_CDCLIRGEMAXAONN);
	params_intern_appendONode(CDCLIRANID, "cdclIRGEMaxXOR");
		params_intern_appendANode(currNID-1, "cdclIRGEMaxXOR", SPECS_DTYPE_I, PARAM_FUZZMIN_CDCLIRGEMAXXOR, PARAM_FUZZMAX_CDCLIRGEMAXXOR);
	params_intern_appendONode(CDCLIRANID, "cdclIRStrMaxSize");
		params_intern_appendANode(currNID-1, "cdclIRStrMaxSize", SPECS_DTYPE_I, PARAM_FUZZMIN_CDCLIRSTRMAXSIZE, PARAM_FUZZMAX_CDCLIRSTRMAXSIZE);
	params_intern_appendONode(CDCLIRANID, "cdclIRNIVERMax");
		params_intern_appendANode(currNID-1, "cdclIRNIVERMax", SPECS_DTYPE_I, PARAM_FUZZMIN_CDCLIRNIVERMAX, PARAM_FUZZMAX_CDCLIRNIVERMAX);

}

void params_intern_appendCdclSVR_CDCL(uint32_t GUIDEONID, char* parName){
	params_intern_appendONode(GUIDEONID, "cdclSelectVarActIncBump");
		params_intern_appendANode(currNID-1, "cdclSelectVarActIncBump", SPECS_DTYPE_R, PARAM_FUZZMIN_CDCLSELECTVARACTINCBUMP, PARAM_FUZZMAX_CDCLSELECTVARACTINCBUMP);
	params_intern_appendONode(GUIDEONID, "cdclSelectVarRandProb");
		params_intern_appendANode(currNID-1, "cdclSelectVarRandProb", SPECS_DTYPE_R, PARAM_FUZZMIN_CDCLSELECTVARRANDPROB, PARAM_FUZZMAX_CDCLSELECTVARRANDPROB);
}

void params_intern_appendCdclSVAI_CDCL(uint32_t CDCLSVAIONID, char* parName){
	//OCCNUM
	params_intern_appendANode(CDCLSVAIONID, parName, SPECS_DTYPE_C, CDCL_SELECTVARRULE_VARACTINIT_OCCNUM, CDCL_SELECTVARRULE_VARACTINIT_OCCNUM);
	//JERESLOWWANG
	params_intern_appendANode(CDCLSVAIONID, parName, SPECS_DTYPE_C, CDCL_SELECTVARRULE_VARACTINIT_JERESLOWWANG, CDCL_SELECTVARRULE_VARACTINIT_JERESLOWWANG);
	//BINOCC
	params_intern_appendANode(CDCLSVAIONID, parName, SPECS_DTYPE_C, CDCL_SELECTVARRULE_VARACTINIT_BINOCC, CDCL_SELECTVARRULE_VARACTINIT_BINOCC);
}

void params_intern_appendCdclSDR_CDCL(uint32_t CDCLSDRONID, char* parName){
	uint32_t CDCLSDRANID;
	//PHASE SAVING
	params_intern_appendANode(CDCLSDRONID, parName, SPECS_DTYPE_C, CDCL_SELECTDIRRULE_PHASESAVING, CDCL_SELECTDIRRULE_PHASESAVING);
	//FAIL BINARY
	params_intern_appendANode(CDCLSDRONID, parName, SPECS_DTYPE_C, CDCL_SELECTDIRRULE_FAILBINARY, CDCL_SELECTDIRRULE_FAILBINARY);
	//FAIL BINARY
	params_intern_appendANode(CDCLSDRONID, parName, SPECS_DTYPE_C, CDCL_SELECTDIRRULE_PHASEFLIP, CDCL_SELECTDIRRULE_PHASEFLIP);
	CDCLSDRANID = currNID-1;
	params_intern_appendONode(CDCLSDRANID, "cdclSelectDirMinFlipDist");
		params_intern_appendANode(currNID-1, "cdclSelectDirMinFlipDist", SPECS_DTYPE_I, PARAM_FUZZMIN_CDCLSELECTDIRMINFLIPDIST, PARAM_FUZZMAX_CDCLSELECTDIRMINFLIPDIST);
}

void params_intern_appendCdclSDDI_CDCL(uint32_t CDCLSDDIONID, char* parName){
	//ZEROFIRST
	params_intern_appendANode(CDCLSDDIONID, parName, SPECS_DTYPE_C, CDCL_SELECTDIRRULE_DIRINIT_ZEROFIRST, CDCL_SELECTDIRRULE_DIRINIT_ZEROFIRST);
	//ONEFIRST
	params_intern_appendANode(CDCLSDDIONID, parName, SPECS_DTYPE_C, CDCL_SELECTDIRRULE_DIRINIT_ONEFIRST, CDCL_SELECTDIRRULE_DIRINIT_ONEFIRST);
	//MAXOCC
	params_intern_appendANode(CDCLSDDIONID, parName, SPECS_DTYPE_C, CDCL_SELECTDIRRULE_DIRINIT_MAXOCC, CDCL_SELECTDIRRULE_DIRINIT_MAXOCC);
	//JERESLOWWANG
	params_intern_appendANode(CDCLSDDIONID, parName, SPECS_DTYPE_C, CDCL_SELECTDIRRULE_DIRINIT_JERESLOWWANG, CDCL_SELECTDIRRULE_DIRINIT_JERESLOWWANG);
	//MAXOCCFAIL
	params_intern_appendANode(CDCLSDDIONID, parName, SPECS_DTYPE_C, CDCL_SELECTDIRRULE_DIRINIT_MAXOCCFAIL, CDCL_SELECTDIRRULE_DIRINIT_MAXOCCFAIL);
	//JERESLOWWANGFAIL
	params_intern_appendANode(CDCLSDDIONID, parName, SPECS_DTYPE_C, CDCL_SELECTDIRRULE_DIRINIT_JERESLOWWANGFAIL, CDCL_SELECTDIRRULE_DIRINIT_JERESLOWWANGFAIL);
	//BIFAIL
	params_intern_appendANode(CDCLSDDIONID, parName, SPECS_DTYPE_C, CDCL_SELECTDIRRULE_DIRINIT_BINFAIL, CDCL_SELECTDIRRULE_DIRINIT_BINFAIL);
}

void params_intern_appendCdclSVR_PMPCDCL(uint32_t GUIDEONID, char* parName){
	params_intern_appendONode(GUIDEONID, "cdclSelectVarActIncBump");
		params_intern_appendANode(currNID-1, "cdclSelectVarActIncBump", SPECS_DTYPE_R, PARAM_FUZZMIN_CDCLSELECTVARACTINCBUMP, PARAM_FUZZMAX_CDCLSELECTVARACTINCBUMP);
}

void params_intern_appendCdclSVR_HYBRIDMSC(uint32_t GUIDEONID, char* parName){
	params_intern_appendONode(GUIDEONID, "cdclSelectVarActIncBump");
		params_intern_appendANode(currNID-1, "cdclSelectVarActIncBump", SPECS_DTYPE_R, PARAM_FUZZMIN_CDCLSELECTVARACTINCBUMP, PARAM_FUZZMAX_CDCLSELECTVARACTINCBUMP);
}

void params_intern_appendCdclForGuideCDCL(uint32_t GUIDEONID){
	uint32_t ONID;

	params_intern_appendCdclSVR_CDCL(GUIDEONID, "cdclSelectVarRule");

	params_intern_appendONode(GUIDEONID, "cdclSelectVarActInit");
	ONID = currNID-1;
	params_intern_appendCdclSVAI_CDCL(ONID, "cdclSelectVarActInit");

	params_intern_appendONode(GUIDEONID, "cdclSelectDirRule");
	ONID = currNID-1;
	params_intern_appendCdclSDR_CDCL(ONID, "cdclSelectDirRule");

	params_intern_appendONode(GUIDEONID, "cdclSelectDirDirInit");
	ONID = currNID-1;
	params_intern_appendCdclSDDI_CDCL(ONID, "cdclSelectDirDirInit");

	params_intern_appendONode(GUIDEONID, "cdclConflictAnalysisRule");
	ONID = currNID-1;
	params_intern_appendCdclCAR_unconditional(ONID, "cdclConflictAnalysisRule");

	params_intern_appendONode(GUIDEONID, "cdclStrLearnedRule");
	ONID = currNID-1;
	params_intern_appendCdclSLR_unconditional(ONID, "cdclStrLearnedRule");

	params_intern_appendONode(GUIDEONID, "cdclStrOtherRule");
	ONID = currNID-1;
	params_intern_appendCdclSOR_unconditional(ONID, "cdclStrOtherRule");

	params_intern_appendONode(GUIDEONID, "cdclRestartRule");
	ONID = currNID-1;
	params_intern_appendCdclRR_unconditional(ONID, "cdclRestartRule");

	params_intern_appendONode(GUIDEONID, "cdclMaintenanceRule");
	ONID = currNID-1;
	params_intern_appendCdclMR_unconditional(ONID, "cdclMaintenanceRule");

	params_intern_appendONode(GUIDEONID, "cdclInprocessingRule");
	ONID = currNID-1;
	params_intern_appendCdclIR_unconditional(ONID, "cdclInprocessingRule");
}

void params_intern_appendCdclForGuidePMPCDCL(uint32_t GUIDEONID){
	uint32_t ONID;

	params_intern_appendCdclSVR_PMPCDCL(GUIDEONID, "cdclSelectVarRule");

	params_intern_appendONode(GUIDEONID, "cdclConflictAnalysisRule");
	ONID = currNID-1;
	params_intern_appendCdclCAR_unconditional(ONID, "cdclConflictAnalysisRule");

	params_intern_appendONode(GUIDEONID, "cdclStrLearnedRule");
	ONID = currNID-1;
	params_intern_appendCdclSLR_unconditional(ONID, "cdclStrLearnedRule");

	params_intern_appendONode(GUIDEONID, "cdclStrOtherRule");
	ONID = currNID-1;
	params_intern_appendCdclSOR_unconditional(ONID, "cdclStrOtherRule");

	params_intern_appendONode(GUIDEONID, "cdclRestartRule");
	ONID = currNID-1;
	params_intern_appendCdclRR_unconditional(ONID, "cdclRestartRule");

	params_intern_appendONode(GUIDEONID, "cdclMaintenanceRule");
	ONID = currNID-1;
	params_intern_appendCdclMR_unconditional(ONID, "cdclMaintenanceRule");

	params_intern_appendONode(GUIDEONID, "cdclInprocessingRule");
	ONID = currNID-1;
	params_intern_appendCdclIR_unconditional(ONID, "cdclInprocessingRule");
}

void params_intern_appendCdclForGuideHYBRIDMSC(uint32_t GUIDEONID){
	uint32_t ONID;

	params_intern_appendCdclSVR_HYBRIDMSC(GUIDEONID, "cdclSelectVarRule");

	params_intern_appendONode(GUIDEONID, "cdclConflictAnalysisRule");
	ONID = currNID-1;
	params_intern_appendCdclCAR_unconditional(ONID, "cdclConflictAnalysisRule");

	params_intern_appendONode(GUIDEONID, "cdclStrLearnedRule");
	ONID = currNID-1;
	params_intern_appendCdclSLR_unconditional(ONID, "cdclStrLearnedRule");

	params_intern_appendONode(GUIDEONID, "cdclStrOtherRule");
	ONID = currNID-1;
	params_intern_appendCdclSOR_unconditional(ONID, "cdclStrOtherRule");

	params_intern_appendONode(GUIDEONID, "cdclRestartRule");
	ONID = currNID-1;
	params_intern_appendCdclRR_unconditional(ONID, "cdclRestartRule");

	params_intern_appendONode(GUIDEONID, "cdclMaintenanceRule");
	ONID = currNID-1;
	params_intern_appendCdclMR_unconditional(ONID, "cdclMaintenanceRule");

	params_intern_appendONode(GUIDEONID, "cdclInprocessingRule");
	ONID = currNID-1;
	params_intern_appendCdclIR_unconditional(ONID, "cdclInprocessingRule");
}

#endif
