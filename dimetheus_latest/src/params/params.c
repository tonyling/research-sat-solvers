/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "params.h"

void params_resetPhase(){
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_params_component_totalCalls;
	#endif
	#ifdef VERBOSE_PARAMS
	printf("c   PARAMS: RESET...\n");
	#endif
	params_returnCode = PARAMS_UNKNOWN;

	//RESET ALL TO DEFAULTS
	//Main algorithm related.
	param_formula 					= NULL;
	param_seed 						= PARAM_DEFAULT_SEED;
	#ifdef SPECS_PARAMGRAPH
	//Specification related.
	param_specsGraphOutput			= NULL;
	#endif

	//RESET ALL PHASES RELATED PARAMETERS
	//Reset related.
	//Nothing yet.
	//Parameter related.
	//Nothing yet.
	//Guidance related.
	param_guide						= PARAM_DEFAULT_GUIDE;
	//Startup related.
	param_startupOutput				= NULL;
	param_startupInputFormat		= PARAM_DEFAULT_STARTUPINPUTFORMAT;
	//Classification related.
	param_classifyInputDomain		= PARAM_DEFAULT_CLASSIFYINPUTDOMAIN;
	param_classifyAttSetTime		= PARAM_DEFAULT_CLASSIFYATTSETTIME;
	param_classifyFloatPrec			= PARAM_DEFAULT_CLASSIFYFLOATPREC;
	param_classifySimplify			= PARAM_DEFAULT_CLASSIFYSIMPLIFY;
	param_classifyMaxVGSize			= PARAM_DEFAULT_CLASSIFYMAXVGSIZE;
	param_classifyMaxCGSize			= PARAM_DEFAULT_CLASSIFYMAXCGSIZE;
	param_classifyMaxUPImps			= PARAM_DEFAULT_CLASSIFYMAXUPIMPS;
	param_classifyMaxUPn			= PARAM_DEFAULT_CLASSIFYMAXUPN;
	param_classifyPGFA				= PARAM_DEFAULT_CLASSIFYPGFA;
	param_classifyPGFB				= PARAM_DEFAULT_CLASSIFYPGFB;
	param_classifyPGFC				= PARAM_DEFAULT_CLASSIFYPGFC;
	param_classifyStrategy			= PARAM_DEFAULT_CLASSIFYSTRATEGY;
	//Adaptation related.
	param_adaptStrategy				= PARAM_DEFAULT_ADAPTSTRATEGY;
	param_adaptPrepStrategy			= PARAM_DEFAULT_ADAPTPREPSTRATEGY;
	param_adaptPrepStrategyParams	= PARAM_DEFAULT_ADAPTPREPSTRATEGYPARAMS;
	param_adaptInpStrategy			= PARAM_DEFAULT_ADAPTINPSTRATEGY;
	param_adaptInpStrategyParams	= PARAM_DEFAULT_ADAPTINPSTRATEGYPARAMS;
	param_adaptSearchStrategy		= PARAM_DEFAULT_ADAPTSEARCHSTRATEGY;
	param_adaptSearchStrategyParams	= PARAM_DEFAULT_ADAPTSEARCHSTRATEGYPARAMS;
	param_adaptMpUR					= PARAM_DEFAULT_ADAPTMPUR;
	param_adaptMpURParams			= PARAM_DEFAULT_ADAPTMPURPARAMS;
	param_adaptSlsPAFVR				= PARAM_DEFAULT_ADAPTSLSPAFVR;
	param_adaptSlsPAFVRParams		= PARAM_DEFAULT_ADAPTSLSPAFVRPARAMS;
	param_adaptSlsAIR				= PARAM_DEFAULT_ADAPTSLSAIR;
	param_adaptSlsAIRParams			= PARAM_DEFAULT_ADAPTSLSAIRPARAMS;
	param_adaptSlsTRR				= PARAM_DEFAULT_ADAPTSLSTRR;
	param_adaptSlsTRRParams			= PARAM_DEFAULT_ADAPTSLSTRRPARAMS;
	param_adaptCdclSVR				= PARAM_DEFAULT_ADAPTCDCLSVR;
	param_adaptCdclSVRParams		= PARAM_DEFAULT_ADAPTCDCLSVRPARAMS;
	param_adaptCdclSVAI				= PARAM_DEFAULT_ADAPTCDCLSVAI;
	param_adaptCdclSVAIParams		= PARAM_DEFAULT_ADAPTCDCLSVAIPARAMS;
	param_adaptCdclSDR				= PARAM_DEFAULT_ADAPTCDCLSDR;
	param_adaptCdclSDRParams		= PARAM_DEFAULT_ADAPTCDCLSDRPARAMS;
	param_adaptCdclSDDI				= PARAM_DEFAULT_ADAPTCDCLSDDI;
	param_adaptCdclSDDIParams		= PARAM_DEFAULT_ADAPTCDCLSDDIPARAMS;
	param_adaptCdclCAR				= PARAM_DEFAULT_ADAPTCDCLCAR;
	param_adaptCdclCARParams		= PARAM_DEFAULT_ADAPTCDCLCARPARAMS;
	param_adaptCdclSLR				= PARAM_DEFAULT_ADAPTCDCLSLR;
	param_adaptCdclSLRParams		= PARAM_DEFAULT_ADAPTCDCLSLRPARAMS;
	param_adaptCdclSOR				= PARAM_DEFAULT_ADAPTCDCLSOR;
	param_adaptCdclSORParams		= PARAM_DEFAULT_ADAPTCDCLSORPARAMS;
	param_adaptCdclCBJLR			= PARAM_DEFAULT_ADAPTCDCLCBJLR;
	param_adaptCdclCBJLRParams		= PARAM_DEFAULT_ADAPTCDCLCBJLRPARAMS;
	param_adaptCdclRR				= PARAM_DEFAULT_ADAPTCDCLRR;
	param_adaptCdclRRParams			= PARAM_DEFAULT_ADAPTCDCLRRPARAMS;
	param_adaptCdclMR				= PARAM_DEFAULT_ADAPTCDCLMR;
	param_adaptCdclMRParams			= PARAM_DEFAULT_ADAPTCDCLMRPARAMS;
	param_adaptCdclIR				= PARAM_DEFAULT_ADAPTCDCLIR;
	param_adaptCdclIRParams			= PARAM_DEFAULT_ADAPTCDCLIRPARAMS;
	param_adaptCdclAR				= PARAM_DEFAULT_ADAPTCDCLAR;
	param_adaptCdclARParams			= PARAM_DEFAULT_ADAPTCDCLARPARAMS;
	//Preprocessor related.
	param_prepOutput				= NULL;
	param_prepReviveCls 			= PARAM_DEFAULT_PREPREVIVECLS;
	param_prepUNHRounds				= PARAM_DEFAULT_PREPUNHROUNDS;
	param_prepACCEMax 				= PARAM_DEFAULT_PREPACCEMAX;
	param_prepACCEMinClsSize		= PARAM_DEFAULT_PREPACCEMINCLSSIZE;
	param_prepACCESaturate			= PARAM_DEFAULT_PREPACCESATURATE;
	param_prepNIVERMax 				= PARAM_DEFAULT_PREPNIVERMAX;
	param_prepATEMin				= PARAM_DEFAULT_PREPATEMIN;
	param_prepGEMaxAONN				= PARAM_DEFAULT_PREPGEMAXAONN;
	param_prepGEMaxXOR				= PARAM_DEFAULT_PREPGEMAXXOR;
	param_prepTERNARYLitMax			= PARAM_DEFAULT_PREPTERNARYLITMAX;
	param_prepTERNARYMax			= PARAM_DEFAULT_PREPTERNARYMAX;
	param_prepNHBRMin				= PARAM_DEFAULT_PREPNHBRMIN;
	param_prepStrategy				= PARAM_DEFAULT_PREPSTRATEGY;
	//In-processor related.
	param_inpStrMaxSize				= PARAM_DEFAULT_INPSTRMAXSIZE;
	param_inpTernaryLitMax			= PARAM_DEFAULT_INPTERNARYLITMAX;
	param_inpTernaryMax				= PARAM_DEFAULT_INPTERNARYMAX;
	param_inpNiverMax				= PARAM_DEFAULT_INPNIVERMAX;
	param_inpGEMaxAONN				= PARAM_DEFAULT_INPGEMAXAONN;
	param_inpGEMaxXOR				= PARAM_DEFAULT_INPGEMAXXOR;
	param_inpNHBRMin				= PARAM_DEFAULT_INPNHBRMIN;
	param_inpUNHRounds				= PARAM_DEFAULT_INPUNHROUNDS;
	param_inpStrategy				= PARAM_DEFAULT_INPSTRATEGY;
	//Search related.
	param_searchMIDBlockSize		= PARAM_DEFAULT_SEARCHMIDBLOCKSIZE;
	param_searchRetreatFact			= PARAM_DEFAULT_SEARCHRETREATFACT;
	param_searchMinClsImpact		= PARAM_DEFAULT_SEARCHMINCLSIMPACT;
	param_searchImpactMaxFactor		= PARAM_DEFAULT_SEARCHIMPACTMAXFACTOR;
	param_searchMaxMagForSLS		= PARAM_DEFAULT_SEARCHMAXMAGFORSLS;
	param_searchMPWeightModSAT		= PARAM_DEFAULT_SEARCHMPWEIGHTMODSAT;
	param_searchMPWeightModUNS		= PARAM_DEFAULT_SEARCHMPWEIGHTMODUNS;
	param_searchStrategy			= PARAM_DEFAULT_SEARCHSTRATEGY;
	//Handling result related.
	//Nothing yet.
	//Shutdown related.
	//Nothing yet.

	//RESET ALL MODULE RELATED PARAMETERS
	//DETERM
	//Nothing yet.
	//SPECCER
	param_speccerNNFFirst			= PARAM_DEFAULT_SPECCERNNFFIRST;
	//LOADER
	//Nothing yet.
	//ATTRIBUTOR
	//Nothing yet.
	//RANDOMFOREST
	//Nothing yet.
	//ITEADAPTER
	//Nothing yet.
	//PRE
	//Nothing yet.
	//SATELITE
	//Nothing yet.
	//MP
	param_mpMaxNumIterations		= PARAM_DEFAULT_MPMAXNUMITERATIONS;
	param_mpMaxNonImpIterations		= PARAM_DEFAULT_MPMAXNONIMPITERATIONS;
	param_mpMaxConvergenceDiff		= PARAM_DEFAULT_MPMAXCONVERGENCEDIFF;
	param_mpWeightNoise				= PARAM_DEFAULT_MPWEIGHTNOISE;
	param_mpMaxMagnetization		= PARAM_DEFAULT_MPMAXMAGNETIZATION;
	param_mpEpsilon					= PARAM_DEFAULT_MPEPSILON;
	param_mpRho						= PARAM_DEFAULT_MPRHO;
	param_mpSigma					= PARAM_DEFAULT_MPSIGMA;
	param_mpPhi						= PARAM_DEFAULT_MPPHI;
	param_mpUpsilon					= PARAM_DEFAULT_MPUPSILON;
	param_mpUpdateRule				= PARAM_DEFAULT_MPUPDATERULE;
	//SLS
	param_slsMaxFlipsA				= PARAM_DEFAULT_SLSMAXFLIPSA;
	param_slsMaxFlipsB				= PARAM_DEFAULT_SLSMAXFLIPSB;
	param_slsMaxTries				= PARAM_DEFAULT_SLSMAXTRIES;
	param_slsNoise					= PARAM_DEFAULT_SLSNOISE;
	param_slsProbsatCB				= PARAM_DEFAULT_SLSPROBSATCB;
	param_slsProbsatCBShift			= PARAM_DEFAULT_SLSPROBSATCBSHIFT;
	param_slsPickAndFlipVarRule 	= PARAM_DEFAULT_SLSPICKANDFLIPVARRULE;
	param_slsAssInitRule 			= PARAM_DEFAULT_SLSASSINITRULE;
	param_slsTryRestartRule 		= PARAM_DEFAULT_SLSTRYRESTARTRULE;
	//CDCL
	param_cdclOutput				= NULL;
	param_cdclRestartStagMaxFrac    = PARAM_DEFAULT_CDCLRESTARTSTAGMAXFRAC;
	param_cdclRestartInnerouterInit = PARAM_DEFAULT_CDCLRESTARTINNEROUTERINIT;
	param_cdclRestartInnerouterInc  = PARAM_DEFAULT_CDCLRESTARTINNEROUTERINC;
	param_cdclRestartLubyUnit		= PARAM_DEFAULT_CDCLRESTARTLUBYUNIT;
	param_cdclRestartVarAgiIncBump	= PARAM_DEFAULT_CDCLRESTARTVARAGIINCBUMP;
	param_cdclRestartMaxAgiFrac		= PARAM_DEFAULT_CDCLRESTARTMAXAGIFRAC;
	param_cdclRestartMaxAgiOutFrac	= PARAM_DEFAULT_CDCLRESTARTMAXAGIOUTFRAC;
	param_cdclSelectVarActIncBump	= PARAM_DEFAULT_CDCLSELECTVARACTINCBUMP;
	param_cdclSelectVarActInit		= PARAM_DEFAULT_CDCLSELECTVARACTINIT;
	param_cdclSelectVarRandProb		= PARAM_DEFAULT_CDCLSELECTVARRANDPROB;
	param_cdclSelectDirMinFlipDist  = PARAM_DEFAULT_CDCLSELECTDIRMINFLIPDIST;
	param_cdclSelectDirDirInit		= PARAM_DEFAULT_CDCLSELECTDIRDIRINIT;
	param_cdclMaintenanceMinDev		= PARAM_DEFAULT_CDCLMAINTENANCEMINDEV;
	param_cdclMaintenanceMaxAvg		= PARAM_DEFAULT_CDCLMAINTENANCEMAXAVG;
	param_cdclMaintenanceActIncBump	= PARAM_DEFAULT_CDCLMAINTENANCEACTINCBUMP;
	param_cdclMaintenanceInitial    = PARAM_DEFAULT_CDCLMAINTENANCEINITIAL;
	param_cdclMaintenanceIncrease   = PARAM_DEFAULT_CDCLMAINTENANCEINCREASE;
	param_cdclMaintenanceBase		= PARAM_DEFAULT_CDCLMAINTENANCEBASE;
	param_cdclMaintenanceBoost		= PARAM_DEFAULT_CDCLMAINTENANCEBOOST;
	param_cdclIRUNHRounds			= PARAM_DEFAULT_CDCLIRUNHROUNDS;
	param_cdclIRTERNARYLitMax		= PARAM_DEFAULT_CDCLIRTERNARYLITMAX;
	param_cdclIRTERNARYMax			= PARAM_DEFAULT_CDCLIRTERNARYMAX;
	param_cdclIRNIVERMax			= PARAM_DEFAULT_CDCLIRNIVERMAX;
	param_cdclIRGEMaxAONN			= PARAM_DEFAULT_CDCLIRGEMAXAONN;
	param_cdclIRGEMaxXOR			= PARAM_DEFAULT_CDCLIRGEMAXXOR;
	param_cdclIRStrMaxSize			= PARAM_DEFAULT_CDCLIRSTRMAXSIZE;
	param_cdclIRNHBRMin				= PARAM_DEFAULT_CDCLIRNHBRMIN;
	param_cdclIRMinConfDistance		= PARAM_DEFAULT_CDCLIRMINCONFDISTANCE;
	param_cdclSelectVarRule		 	= PARAM_DEFAULT_CDCLSELECTVARRULE;
	param_cdclSelectDirRule		 	= PARAM_DEFAULT_CDCLSELECTDIRRULE;
	param_cdclConflictAnalysisRule	= PARAM_DEFAULT_CDCLCONFLICTANALYSISRULE;
	param_cdclStrLearnedRule		= PARAM_DEFAULT_CDCLSTRLEARNEDRULE;
	param_cdclStrOtherRule			= PARAM_DEFAULT_CDCLSTROTHERRULE;
	param_cdclComputeBJLRule		= PARAM_DEFAULT_CDCLCOMPUTEBJLRULE;
	param_cdclRestartRule			= PARAM_DEFAULT_CDCLRESTARTRULE;
	param_cdclMaintenanceRule		= PARAM_DEFAULT_CDCLMAINTENANCERULE;
	param_cdclInprocessingRule		= PARAM_DEFAULT_CDCLINPROCESSINGRULE;
	param_cdclAbortRule		        = PARAM_DEFAULT_CDCLABORTRULE;
	//HANDLERESULT
	param_handleresultCoarsen		= PARAM_DEFAULT_HANDLERESULTCOARSEN;
	//SOLWRITER
	param_solwriterPrintAssignment	= PARAM_DEFAULT_SOLWRITERPRINTASSIGNMENT;

	#ifdef COLLECTSTATS
	stats_params_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}

void params_initPhase(){
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_params_component_totalCalls;
	#endif
	#ifdef VERBOSE_PARAMS
	printf("c   PARAMS: INITIALIZE...\n");
	#endif
	params_returnCode = PARAMS_UNKNOWN;

	#ifdef COLLECTSTATS
	stats_params_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}

void params_disposePhase(){
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_params_component_totalCalls;
	#endif
	#ifdef VERBOSE_PARAMS
	printf("c   PARAMS: DISPOSE...\n");
	#endif
	reset_returnCode = RESET_UNKNOWN;

	#ifdef COLLECTSTATS
	stats_params_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}

void params_extern_printHelp(){
	printf("c Help:\n");
    printf("c  Usage:   ./dimetheus -param1 value1 ... -paramN valueN\n");
    printf("c  Example: ./dimetheus -formula ../cnfs/tiny.cnf -seed 101 -guide 5\n");
    printf("c\n");
    printf("c Help on GUIDES:\n");
    printf("c  If you use the -guide parameter you must carefully check below what parameters this guide ENFORCES. You are\n"
    	   "c  not allowed to set any of these values yourself. Furthermore, the guide might not enforce a specific parameter,\n"
    	   "c  but it might forbid certain settings to it. You must obey these rules as well. Furthermore, every guide tells\n"
    	   "c  you what modules it will use. Providing parameters for the other modules is not forbidden, but is useless.\n");
    printf("c\n");
    printf("c Help on ADAPTION:\n");
    printf("c  Almost any parameter of the solver can be adapted. Influencing the adaptation might be prohibited in some of\n"
    	   "c  the guides. It is important to understand that providing parameters on the command line limits the solvers\n"
    	   "c  adaptation capabilities. It will always use what the user provides (if this is allowed, that is). The best\n"
    	   "c  thing to do is to provide as few parameters as possible.\n");
    printf("c\n");
    printf("c Help on PARAMETERS:\n");
    printf("c  Below is a listing of all parameters that the solver knows about. For most of the parameters, you have some\n"
    	   "c  information given in '[]' symbols. The statement is understood as follows: [type|min:default:max].\n");
    //MAIN
    printf("c   MAIN:\n");
    printf("c    -h                      [] Print this help.\n");
    printf("c    -v                      [] Print detailed version information.\n");
    printf("c    -s                      [] Print specification information on modules.\n");
    printf("c    -formula                [char*] MANDATORY. The relative path and filename of the target DIMACS CNF formula.\n");
    printf("c    -seed                   [int|%d] "
    		"The seed for the random number generator.\n",
    		PARAM_DEFAULT_SEED);

    #ifdef SPECS_PARAMGRAPH
	//SPECS
    printf("c   SPECS:\n");
    printf("c    -specsGraphOutput        [char*]* "
    		"The relative path and filename to store the EDACC parameter graph from specifications.\n");
	#endif

    //RESET
    printf("c   RESET:\n");
    printf("c     The RESET phase currently has no parameters.\n");

    //PARAMS
    printf("c   PARAMS:\n");
    printf("c     The PARAMS phase currently has no parameters.\n");

    //GUIDANCE
    printf("c   GUIDANCE:\n");
    printf("c    -guide                  [uint|%d:%d:%d] "
    		"Uses a pre-defined set of strategies and parameters. Check below.\n",
    		GUIDE_MINVALUE, PARAM_DEFAULT_GUIDE, GUIDE_MAXVALUE);
    guidance_printHelp();

    //STARTUP
    printf("c   STARTUP:\n");
    printf("c    -startupOutput          [char*]* "
    		"The relative path and filename to store the formula in DIMACS format after loading it.\n");
    printf("c    -startupInputFormat     [uint|%d] "
    		"Enforce a specific input format. Leave at %d in order to have DETERM figure it out by itself.\n",
    		PARAM_DEFAULT_STARTUPINPUTFORMAT, PARAM_DEFAULT_STARTUPINPUTFORMAT);
    printf("c      Known input formats are:\n");
    startup_extern_printKnownFormats("c        ");
    //DETERM
    printf("c   DETERM module:\n");
    printf("c     The DETERM module currently has no parameters.\n");
    //SPECCER
    printf("c   SPECCER module:\n");
    printf("c    -speccerNNFFirst        [bool|%d:0:1] "
    		"Whether a translation to NNF is to be performed first before translating a non-CNF into a CNF.\n",
    		PARAM_DEFAULT_SPECCERNNFFIRST);
    //LOADER
    printf("c   LOADER module:\n");
    printf("c     The LOADER module currently has no parameters.\n");

    //CLASSIFY
    printf("c   CLASSIFICATION:\n");
    printf("c    -classifyInputDomain    [uint|%d] "
    		"Enforce the classification for a specific domain (e.g. random). Leave at %d to use generic classification.\n",
    		PARAM_DEFAULT_CLASSIFYINPUTDOMAIN, PARAM_DEFAULT_CLASSIFYINPUTDOMAIN);
    printf("c      Known domains are:\n");
    classify_extern_printKnownDomains("c        ");
    printf("c    -classifyAttSetTime     [float|%f] "
    		"The time in seconds that the classifier is allowed to use to compute a single attribute set (VCG, VG, CG, ...).\n",
    		PARAM_DEFAULT_CLASSIFYATTSETTIME);
    printf("c    -classifyFloatPrec      [uint|%d] "
    		"The number of digits after the comma (precision) for floating point attributes that are computed.\n",
    		PARAM_DEFAULT_CLASSIFYFLOATPREC);
    printf("c    -classifySimplify       [bool|%d] "
    		"Whether simplifications should be performed after the attributes have been computed.\n",
    		PARAM_DEFAULT_CLASSIFYSIMPLIFY);
    printf("c    -classifyMaxVGSize      [uint|%d] "
    		"The maximum number of variables that are allowed to build up a variable graph for attribute computation.\n",
    		PARAM_DEFAULT_CLASSIFYMAXVGSIZE);
    printf("c    -classifyMaxCGSize      [uint|%d] "
    		"The maximum number of clauses that are allowed to compute clause graph attributes.\n",
    		PARAM_DEFAULT_CLASSIFYMAXCGSIZE);
    printf("c    -classifyMaxUPImps      [uint|%d] "
    		"The maximum number of literal implications for which we compute the UP attributes of a literal.\n",
    		PARAM_DEFAULT_CLASSIFYMAXUPIMPS);
    printf("c    -classifyMaxUPn         [uint|%d] "
    		"The maximum number of variables for which we allow the computation of UP attributes.\n",
    		PARAM_DEFAULT_CLASSIFYMAXUPN);
    printf("c    -classifyPGFA           [float|%f] "
    		"Used as input for calculating a probability generating function value. G(a) = sum p(x) * a^x.\n",
    		PARAM_DEFAULT_CLASSIFYPGFA);
    printf("c    -classifyPGFB           [float|%f] "
    		"Used as input for calculating a probability generating function value. G(b) = sum p(x) * b^x.\n",
    		PARAM_DEFAULT_CLASSIFYPGFB);
    printf("c    -classifyPGFC           [float|%f] "
    		"Used as input for calculating a probability generating function value. G(c) = sum p(x) * c^x.\n",
    		PARAM_DEFAULT_CLASSIFYPGFC);
    printf("c    -classifyStrategy       [uint|%d] "
    		"The type of classification strategy that is being used.\n",
    		PARAM_DEFAULT_CLASSIFYSTRATEGY);
    classify_strategies_printHelp();
    //ATTRIBUTOR
    printf("c   ATTRIBUTOR module:\n");
    printf("c     The ATTRIBUTOR module currently has no parameters.\n");
    //RANDOMFOREST
    printf("c   RANDOMFOREST module:\n");
    printf("c     The RANDOMFOREST module currently has no parameters.\n");

    //ADAPT
    printf("c   ADAPTATION:\n");
    printf("c    -adaptStrategy          [bool|%d] "
    		"The type of adaption strategy that is being used.\n",
    		PARAM_DEFAULT_ADAPTSTRATEGY);
    adapt_strategies_printHelp();
    printf("c    -adaptPrepStrategy      [bool|%d] "
    		"Determines if adapting the preprocessing strategy is allowed.\n",
    		PARAM_DEFAULT_ADAPTPREPSTRATEGY);
    printf("c    -adaptPrepStrategyParams [bool|%d] "
    		"Determines if it is allowed to adapt the preprocessing strategy parameters (currently none).\n",
    		PARAM_DEFAULT_ADAPTPREPSTRATEGYPARAMS);
    printf("c    -adaptInpStrategy       [bool|%d] "
    		"Determines if adapting the in-processing strategy is allowed.\n",
    		PARAM_DEFAULT_ADAPTINPSTRATEGY);
    printf("c    -adaptInpStrategyParams [bool|%d] "
    		"Determines if it is allowed to adapt the in-processing strategy parameters (currently none).\n",
    		PARAM_DEFAULT_ADAPTINPSTRATEGYPARAMS);
    printf("c    -adaptSearchStrategy    [bool|%d] "
    		"Determines if adapting the search strategy is allowed.\n",
    		PARAM_DEFAULT_ADAPTSEARCHSTRATEGY);
    printf("c    -adaptSearchStrategyParams [bool|%d] "
    		"Determines if it is allowed to adapt the search strategy parameters (-searchMIDBlockSize).\n",
    		PARAM_DEFAULT_ADAPTSEARCHSTRATEGYPARAMS);
    printf("c    -adaptMpUR              [bool|%d] "
    		"Whether it is allowed to adapt its message passing update and bias computation rule (UR, -mpUpdateRule).\n",
    		PARAM_DEFAULT_ADAPTMPUR);
    printf("c    -adaptMpURParams        [bool|%d] "
    		"Whether it is allowed to adapt the UR parameters (-mpRho, -mpSigma).\n",
    		PARAM_DEFAULT_ADAPTMPURPARAMS);
    printf("c    -adaptSlsPAFVR          [bool|%d] "
    		"Whether it is allowed to adapt the pick and flip variable rule (PAFVR, -slsPickAndFlipVarRule).\n",
    		PARAM_DEFAULT_ADAPTSLSPAFVR);
    printf("c    -adaptSlsPAFVRParams    [bool|%d] "
    		"Whether it is allowed to adapt the PAFVR parameters (-slsNoise, -slsProbsatCB, -slsProbsatCBShift).\n",
    		PARAM_DEFAULT_ADAPTSLSPAFVRPARAMS);
    printf("c    -adaptSlsAIR            [bool|%d] "
    		"Whether it is allowed to adapt the initialize assignment rule (AIR, -slsAssInitRule).\n",
    		PARAM_DEFAULT_ADAPTSLSAIR);
    printf("c    -adaptSlsAIRParams      [bool|%d] "
    		"Whether it is allowed to adapt the AIR parameters (currently none).\n",
    		PARAM_DEFAULT_ADAPTSLSAIRPARAMS);
    printf("c    -adaptSlsTRR            [bool|%d] "
    		"Whether it is allowed to adapt the try restart rule (TRR, -slsTryRestartRule).\n",
    		PARAM_DEFAULT_ADAPTSLSTRR);
    printf("c    -adaptSlsTRRParams      [bool|%d] "
    		"Whether it is allowed to adapt the TRR parameters (currently none).\n",
    		PARAM_DEFAULT_ADAPTSLSTRRPARAMS);
    printf("c    -adaptCdclSVR           [bool|%d] "
    		"Whether it is allowed to adapt the select variable rule (SVR, -cdclSelectVarRule).\n",
    		PARAM_DEFAULT_ADAPTCDCLSVR);
    printf("c    -adaptCdclSVRParams     [bool|%d] "
    		"Whether it is allowed to adapt the SVR parameters (-cdclSelectVarActIncBump, -cdclSelectVarRandProb).\n",
    		PARAM_DEFAULT_ADAPTCDCLSVRPARAMS);
    printf("c    -adaptCdclSVAI          [bool|%d] "
    		"Whether it is allowed to adapt the select variable activity initialization (SVAI, -cdclSelectVarActInit).\n",
    		PARAM_DEFAULT_ADAPTCDCLSVAI);
    printf("c    -adaptCdclSVAIParams    [bool|%d] "
    		"Whether it is allowed to adapt the SVAI parameters (currently none).\n",
    		PARAM_DEFAULT_ADAPTCDCLSVAIPARAMS);
    printf("c    -adaptCdclSDR           [bool|%d] "
    		"Whether it is allowed to adapt the select direction rule (SDR, -cdclSelectDirRule).\n",
    		PARAM_DEFAULT_ADAPTCDCLSDR);
    printf("c    -adaptCdclSDRParams     [bool|%d] "
    		"Whether it is allowed to adapt the SDR parameters (currently none).\n",
    		PARAM_DEFAULT_ADAPTCDCLSDRPARAMS);
    printf("c    -adaptCdclSDDI          [bool|%d] "
    		"Whether it is allowed to adapt the select direction direction initialization (SDDI, -cdclSelectDirDirInit).\n",
    		PARAM_DEFAULT_ADAPTCDCLSDDI);
    printf("c    -adaptCdclSDDIParams    [bool|%d] "
    		"Whether it is allowed to adapt the SDDI parameters (currently none).\n",
    		PARAM_DEFAULT_ADAPTCDCLSDDIPARAMS);
    printf("c    -adaptCdclCAR           [bool|%d] "
    		"Whether it is allowed to adapt the conflict analysis rule (CAR, -cdclConflictAnalysisRule)\n",
    		PARAM_DEFAULT_ADAPTCDCLCAR);
    printf("c    -adaptCdclCARParams     [bool|%d] "
    		"Whether it is allowed to adapt the CAR parameters (currently none).\n",
    		PARAM_DEFAULT_ADAPTCDCLCARPARAMS);
    printf("c    -adaptCdclSLR           [bool|%d] "
    		"Whether it is allowed to adapt the strengthen learned rule (SLR, -cdclStrLearnedRule).\n",
    		PARAM_DEFAULT_ADAPTCDCLSLR);
    printf("c    -adaptCdclSLRParams     [bool|%d] "
    		"Whether it is allowed to adapt the SLR parameters (currently none).\n",
    		PARAM_DEFAULT_ADAPTCDCLSLRPARAMS);
    printf("c    -adaptCdclSOR           [bool|%d] "
    		"Whether it is allowed to adapt the strengthen other rule (SOR, -cdclStrOtherRule).\n",
    		PARAM_DEFAULT_ADAPTCDCLSOR);
    printf("c    -adaptCdclSORParams     [bool|%d] "
    		"Whether it is allowed to adapt the SOR parameters (currently none)\n",
    		PARAM_DEFAULT_ADAPTCDCLSORPARAMS);
    printf("c    -adaptCdclCBJLR         [bool|%d] "
    		"Whether it is allowed to adapt the compute back-jump level rule (CBJLR, -cdclComputeBJLRule).\n",
    		PARAM_DEFAULT_ADAPTCDCLCBJLR);
    printf("c    -adaptCdclCBJLRParams   [bool|%d] "
    		"Whether it is allowed to adapt the CBJLR parameters (currently none).\n",
    		PARAM_DEFAULT_ADAPTCDCLCBJLRPARAMS);
    printf("c    -adaptCdclRR            [bool|%d] "
    		"Whether it is allowed to adapt the restart rule (RR, -cdclRestartRule).\n",
    		PARAM_DEFAULT_ADAPTCDCLRR);
    printf("c    -adaptCdclRRParams      [bool|%d] "
    		"Whether it is allowed to adapt the RR parameters (currently none).\n",
    		PARAM_DEFAULT_ADAPTCDCLRRPARAMS);
    printf("c    -adaptCdclMR            [bool|%d] "
    		"Whether it is allowed to adapt the maintenance rule (MR, -cdclMaintenanceRule).\n",
    		PARAM_DEFAULT_ADAPTCDCLMR);
    printf("c    -adaptCdclMRParams      [bool|%d] "
    		"Whether it is allowed to adapt the MR parameters (currently none).\n",
    		PARAM_DEFAULT_ADAPTCDCLMRPARAMS);
    printf("c    -adaptCdclIR            [bool|%d] "
    		"Whether it is allowed to adapt the in-processing rule (IR, -cdclInprocessingRule).\n",
    		PARAM_DEFAULT_ADAPTCDCLIR);
    printf("c    -adaptCdclIRParams      [bool|%d] "
    		"Whether it is allowed to adapt the IR parameters (currently none).\n",
    		PARAM_DEFAULT_ADAPTCDCLIRPARAMS);
    printf("c    -adaptCdclAR            [bool|%d] "
    		"Whether it is allowed to adapt the abort rule (AR, -cdclAbortRule).\n",
    		PARAM_DEFAULT_ADAPTCDCLAR);
    printf("c    -adaptCdclARParams      [bool|%d] "
    		"Whether it is allowed to adapt the AR parameters (currently none).\n",
    		PARAM_DEFAULT_ADAPTCDCLARPARAMS);
    //ITEADAPTER
    printf("c   ITEADAPTER module:\n");
    printf("c     The ITEADAPTER module currently has no parameters.\n");

    //PREPROCESSOR
    printf("c   PREPROCESSING:\n");
    printf("c    -prepOutput             [char*]* "
    		"The relative path and filename to store the result from preprocessing.\n");
    printf("c    -prepReviveCls          [bool|%d:%d:%d] "
    		"Set this to 1 if you want to bring back specific removed clauses after preprocessing finishes.\n",
    		PARAM_HARDMIN_PREPREVIVECLS, PARAM_DEFAULT_PREPREVIVECLS, PARAM_HARDMAX_PREPREVIVECLS);
    printf("c    -prepUNHRounds          [uint|%d:%d:%d] "
    		"Set the number of rounds for Unhiding.\n",
    		PARAM_HARDMIN_PREPUNHROUNDS, PARAM_DEFAULT_PREPUNHROUNDS, PARAM_HARDMAX_PREPUNHROUNDS);
    printf("c    -prepACCEMax            [uint|%d:%d:%d] "
    		"Max literal occurrences of a literal whose clauses are checked by ACCE.\n",
    		PARAM_HARDMIN_PREPACCEMAX, PARAM_DEFAULT_PREPACCEMAX, PARAM_HARDMAX_PREPACCEMAX);
    printf("c    -prepACCEMinClsSize     [uint|%d:%d:%d] "
    		"Minimum size a clause must have before it is checked with ACCE.\n",
    		PARAM_HARDMIN_PREPACCEMINCLSSIZE, PARAM_DEFAULT_PREPACCEMINCLSSIZE, PARAM_HARDMAX_PREPACCEMINCLSSIZE);
    printf("c    -prepACCESaturate       [bool|%d:%d:%d] "
    		"Perform each clause check in ACCE only once or re-add them to the queue.\n",
    		PARAM_HARDMIN_PREPACCESATURATE, PARAM_DEFAULT_PREPACCESATURATE, PARAM_HARDMAX_PREPACCESATURATE);
    printf("c    -prepNIVERMax           [uint|%d:%d:%d] "
    		"Max literal occurrences for a variable that is checked by NIVER.\n",
    		PARAM_HARDMIN_PREPNIVERMAX, PARAM_DEFAULT_PREPNIVERMAX, PARAM_HARDMAX_PREPNIVERMAX);
    printf("c    -prepATEMin             [uint|%d:%d:%d] "
    		"Minimum size of a clause that is to be checked by separate ATE.\n",
    		PARAM_HARDMIN_PREPATEMIN, PARAM_DEFAULT_PREPATEMIN, PARAM_HARDMAX_PREPATEMIN);
    printf("c    -prepGEMaxAONN          [uint|%d:%d:%d] "
    		"Maximum number of resolvents allowed to be added when extracting an AND/OR/NAND/NOR gate in GE.\n",
    		PARAM_HARDMIN_PREPGEMAXAONN, PARAM_DEFAULT_PREPGEMAXAONN, PARAM_HARDMAX_PREPGEMAXAONN);
    printf("c    -prepGEMaxXOR           [uint|%d:%d:%d] "
    		"Maximum number of resolvents allowed to be added when extracting an XOR gate in GE.\n",
    		PARAM_HARDMIN_PREPGEMAXXOR, PARAM_DEFAULT_PREPGEMAXXOR, PARAM_HARDMAX_PREPGEMAXXOR);
    printf("c    -prepTERNARYLitMax      [uint|%d:%d:%d] "
    		"Maximum number of 3-clauses that can be learned per literal in ternary resolution per call.\n",
    		PARAM_HARDMIN_PREPTERNARYLITMAX, PARAM_DEFAULT_PREPTERNARYLITMAX, PARAM_HARDMAX_PREPTERNARYLITMAX);
    printf("c    -prepTERNARYMax         [uint|%d:%d:%d] "
    		"Maximum number of 3-clauses that can be learned in total in ternary resolution per call.\n",
    		PARAM_HARDMIN_PREPTERNARYMAX, PARAM_DEFAULT_PREPTERNARYMAX, PARAM_HARDMAX_PREPTERNARYMAX);
    printf("c    -prepNHBRMin            [uint|%d:%d:%d] "
    		"Minimum number of implications for a literal to be used in NHBR.\n",
    		PARAM_HARDMIN_PREPNHBRMIN, PARAM_DEFAULT_PREPNHBRMIN, PARAM_HARDMAX_PREPNHBRMIN);
    printf("c    -prepStrategy           [uint|%d:%d:%d] "
    		"The type of preprocessing strategy that is being used.\n",
    		PREP_STRATEGY_MINVALUE, PARAM_DEFAULT_PREPSTRATEGY, PREP_STRATEGY_MAXVALUE);
    prep_strategies_printHelp();
    printf("c   PRE module:\n");
    printf("c     The PRE module currently has no parameters.\n");
    printf("c   SATELITE module:\n");
    printf("c     The SATELITE module currently has no parameters.\n");

    //IN-PROCESSOR
    printf("c   IN-PROCESSING:\n");
    printf("c    -inpStrMaxSize          [uint|%d:%d:%d] "
    		"The maximum clause size allowed for doing strengthening with it.\n",
    		PARAM_HARDMIN_INPSTRMAXSIZE, PARAM_DEFAULT_INPSTRMAXSIZE, PARAM_HARDMAX_INPSTRMAXSIZE);
    printf("c    -inpTernaryLitMax       [uint|%d:%d:%d] "
    		"Maximum number of learned ternary clauses for a single literal in a call to TERNARY resolution.\n",
    		PARAM_HARDMIN_INPTERNARYLITMAX, PARAM_DEFAULT_INPTERNARYLITMAX, PARAM_HARDMAX_INPTERNARYLITMAX);
    printf("c    -inpTernaryMax          [uint|%d:%d:%d] "
    		"Maximum number of learned ternary clauses for all literals in a call to TERNARY resolution.\n",
    		PARAM_HARDMIN_INPTERNARYMAX, PARAM_DEFAULT_INPTERNARYMAX, PARAM_HARDMAX_INPTERNARYMAX);
    printf("c    -inpNiverMax            [uint|%d:%d:%d] "
    		"Maximum total occurrence of a variable to be checked with NIVER.\n",
    		PARAM_HARDMIN_INPNIVERMAX, PARAM_DEFAULT_INPNIVERMAX, PARAM_HARDMAX_INPNIVERMAX);
    printf("c    -inpGEMaxAONN           [uint|%d:%d:%d] "
    		"Maximum number of resolvents allowed to be added when extracting AND/OR/NAND/NOR gates in GE.\n",
    		PARAM_HARDMIN_INPGEMAXAONN, PARAM_DEFAULT_INPGEMAXAONN, PARAM_HARDMAX_INPGEMAXAONN);
    printf("c    -inpGEMaxXOR            [uint|%d:%d:%d] "
    		"Maximum number of resolvents allowed to be added when extracting XOR gates in GE.\n",
    		PARAM_HARDMIN_INPGEMAXXOR, PARAM_DEFAULT_INPGEMAXXOR, PARAM_HARDMAX_INPGEMAXXOR);
    printf("c    -inpNHBRMin             [uint|%d:%d:%d] "
    		"Minimum number of implications for a literal to be used in NHBR.\n",
    		PARAM_HARDMIN_INPNHBRMIN, PARAM_DEFAULT_INPNHBRMIN, PARAM_HARDMAX_INPNHBRMIN);
    printf("c    -inpUNHRounds           [uint|%d:%d:%d] "
    		"The number of Unhiding rounds in a single call to this technique.\n",
    		PARAM_HARDMIN_INPUNHROUNDS, PARAM_DEFAULT_INPUNHROUNDS, PARAM_HARDMAX_INPUNHROUNDS);
    printf("c    -inpStrategy            [uint|%d:%d:%d] "
    		"The type of in-processing strategy that is being used.\n",
    		INP_STRATEGY_MINVALUE, PARAM_DEFAULT_INPSTRATEGY, INP_STRATEGY_MINVALUE);
    inp_strategies_printHelp();
    //IN
    printf("c   IN module:\n");
    printf("c     The IN module currently has no parameters.\n");

    //SEARCH
    printf("c   SEARCH:\n");
    printf("c    -searchMIDBlockSize     [float|%f:%f:%f] "
    		"The fraction of variables of the formula we want to assign with MID as a block in search.\n",
    		PARAM_HARDMIN_SEARCHMIDBLOCKSIZE, PARAM_DEFAULT_SEARCHMIDBLOCKSIZE, PARAM_HARDMAX_SEARCHMIDBLOCKSIZE);
    printf("c    -searchRetreatFact      [float|%f:%f:%f] "
    		"The fraction of assigned variables we want to retain with MID in systematic search search.\n",
    		PARAM_HARDMIN_SEARCHRETREATFACT, PARAM_DEFAULT_SEARCHRETREATFACT, PARAM_HARDMAX_SEARCHRETREATFACT);
    printf("c    -searchMinClsImpact     [float|%f:%f:%f] "
    		"The minimum impact a learned clause must have on MP before it is retained as learned clause.\n",
    		PARAM_HARDMIN_SEARCHMINCLSIMPACT, PARAM_DEFAULT_SEARCHMINCLSIMPACT, PARAM_HARDMAX_SEARCHMINCLSIMPACT);
    printf("c    -searchImpactMaxFactor  [float|%f:%f:%f] "
    		"This times the number of variables gives the maximum impact of learned things for MP before it is disabled.\n",
    		PARAM_HARDMIN_SEARCHIMPACTMAXFACTOR, PARAM_DEFAULT_SEARCHIMPACTMAXFACTOR, PARAM_HARDMAX_SEARCHIMPACTMAXFACTOR);
    printf("c    -searchMaxMagForSLS     [float|%f:%f:%f] "
    		"The maximum magnetization of a sub-formula that we call SLS on for some hybrid strategies.\n",
    		PARAM_HARDMIN_SEARCHMAXMAGFORSLS, PARAM_DEFAULT_SEARCHMAXMAGFORSLS, PARAM_HARDMAX_SEARCHMAXMAGFORSLS);
    printf("c    -searchMPWeightModSAT   [float|%f:%f:%f] "
    		"The amount of weight we remove from a satisfying literal during MP weight reinforcement learning.\n",
    		PARAM_HARDMIN_SEARCHMPWEIGHTMODSAT, PARAM_DEFAULT_SEARCHMPWEIGHTMODSAT, PARAM_HARDMAX_SEARCHMPWEIGHTMODSAT);
    printf("c    -searchMPWeightModUNS   [float|%f:%f:%f] "
    		"The amount of weight we add to a non-satisfying literal during MP weight reinforcement learning.\n",
    		PARAM_HARDMIN_SEARCHMPWEIGHTMODUNS, PARAM_DEFAULT_SEARCHMPWEIGHTMODUNS, PARAM_HARDMAX_SEARCHMPWEIGHTMODUNS);
    printf("c    -searchStrategy         [uint|%d:%d:%d] "
    		"The type of search strategy that is being used.\n",
    		SEARCH_STRATEGY_MINVALUE, PARAM_DEFAULT_SEARCHSTRATEGY, SEARCH_STRATEGY_MAXVALUE);
    search_strategies_printHelp();
    //MP
    printf("c   MP module:\n");
    printf("c    -mpMaxNumIterations     [uint|%d:%d:%d] "
    		"The number of message passing rounds before non-convergence is claimed.\n",
    		PARAM_HARDMIN_MPMAXNUMITERATIONS, PARAM_DEFAULT_MPMAXNUMITERATIONS, PARAM_HARDMAX_MPMAXNUMITERATIONS);
    printf("c    -mpMaxNonImpIterations  [uint|%d:%d:%d] "
    		"The number of message passing rounds without improving the maximum message difference before non-convergence.\n",
    		PARAM_HARDMIN_MPMAXNONIMPITERATIONS, PARAM_DEFAULT_MPMAXNONIMPITERATIONS, PARAM_HARDMAX_MPMAXNONIMPITERATIONS);
    printf("c    -mpMaxConvergenceDiff   [float|%f:%f:%f] "
    		"The maximum difference allowed for updated messages before convergence is claimed.\n",
    		PARAM_HARDMIN_MPMAXCONVERGENCEDIFF, PARAM_DEFAULT_MPMAXCONVERGENCEDIFF, PARAM_HARDMAX_MPMAXCONVERGENCEDIFF);
    printf("c    -mpWeightNoise          [float|%f:%f:%f] "
    		"The noise that is randomly added or subtracted from literal weight during initialization. Zero means <none>.\n",
    		PARAM_HARDMIN_MPWEIGHTNOISE, PARAM_DEFAULT_MPWEIGHTNOISE, PARAM_HARDMAX_MPWEIGHTNOISE);
    printf("c    -mpMaxMagnetization     [float|%f:%f:%f] "
    		"The maximum global magnetization allowed such that we assume a paramagnetic state.\n",
    		PARAM_HARDMIN_MPMAXMAGNETIZATION, PARAM_DEFAULT_MPMAXMAGNETIZATION, PARAM_HARDMAX_MPMAXMAGNETIZATION);
    printf("c    -mpEpsilon              [float|%f:%f:%f] "
    		"The smallest fraction we allow for a message before we treat it as zero.\n",
    		PARAM_HARDMIN_MPEPSILON, PARAM_DEFAULT_MPEPSILON, PARAM_HARDMAX_MPEPSILON);
    printf("c    -mpRho                  [float|%f:%f:%f] "
    		"Controls the interpolation of BP (rho->0) and SP (rho->1) in the rho based URs.\n",
    		PARAM_HARDMIN_MPRHO, PARAM_DEFAULT_MPRHO, PARAM_HARDMAX_MPRHO);
    printf("c    -mpSigma                [float|%f:%f:%f] "
    		"Controls the interpolation of non-EM (sigma->0) and EM (sigma->1) in the sigma based URs.\n",
    		PARAM_HARDMIN_MPSIGMA, PARAM_DEFAULT_MPSIGMA, PARAM_HARDMAX_MPSIGMA);
    printf("c    -mpPhi                  [float|%f:%f:%f] "
    		"Controls the external force-field (phi=0 means no field, +1.0 strong force towards true) in the Phi based URs.\n",
    		PARAM_HARDMIN_MPPHI, PARAM_DEFAULT_MPPHI, PARAM_HARDMAX_MPPHI);
    printf("c    -mpUpsilon              [float|%f:%f:%f] "
    		"Controls the temperature (upsilon=0 means no heat, 1.0 means max heat) in the Upsilon based URs.\n",
    		PARAM_HARDMIN_MPUPSILON, PARAM_DEFAULT_MPUPSILON, PARAM_HARDMAX_MPUPSILON);
    printf("c    -mpUpdateRule           [int|%d:%d:%d] "
    		"Determines the MP update rule to use (UR plug-in).\n",
    		MP_UPDATERULE_MINVALUE, PARAM_DEFAULT_MPUPDATERULE, MP_UPDATERULE_MAXVALUE);
    mp_updateRules_printHelp();
    //SLS
    printf("c   SLS module:\n");
    printf("c    -slsMaxFlipsA           [float|%f:%f:%f] "
    		"Influences the flipping try length (see below).\n",
    		PARAM_HARDMIN_SLSMAXFLIPSA, PARAM_DEFAULT_SLSMAXFLIPSA, PARAM_HARDMAX_SLSMAXFLIPSA);
    printf("c    -slsMaxFlipsB           [float|%f:%f:%f] "
    		"Influences the flipping try length (see below).\n",
    		PARAM_HARDMIN_SLSMAXFLIPSB, PARAM_DEFAULT_SLSMAXFLIPSB, PARAM_HARDMAX_SLSMAXFLIPSB);
    printf("c    -slsMaxTries            [uint|%d:%d:%d] "
    		"Influences the number of tries (see below).\n",
    		PARAM_HARDMIN_SLSMAXTRIES, PARAM_DEFAULT_SLSMAXTRIES, PARAM_HARDMAX_SLSMAXTRIES);
    printf("c    -slsNoise               [float|%f:%f:%f] "
    		"A noise parameter that can be used in various SLS heuristics, i.e. in WalkSAT.\n",
    		PARAM_HARDMIN_SLSNOISE, PARAM_DEFAULT_SLSNOISE, PARAM_HARDMAX_SLSNOISE);
    printf("c    -slsProbsatCB           [float|%f:%f:%f] "
    		"This parameter is used as the brake base constant to compute ProbSAT scores.\n",
    		PARAM_HARDMIN_SLSPROBSATCB, PARAM_DEFAULT_SLSPROBSATCB, PARAM_HARDMAX_SLSPROBSATCB);
    printf("c    -slsProbsatCBShift      [float|%f:%f:%f] "
    		"This parameter is used to shift the brake values to compute ProbSAT scores.\n",
    		PARAM_HARDMIN_SLSPROBSATCBSHIFT, PARAM_DEFAULT_SLSPROBSATCBSHIFT, PARAM_HARDMAX_SLSPROBSATCBSHIFT);
    printf("c    -slsPickAndFlipVarRule  [uint|%d:%d:%d] "
    		"Determines what heuristic to use to pick and flip SLS variable assignments (PAFVR plug-in).\n",
    		SLS_PICKANDFLIPVARRULE_MINVALUE, PARAM_DEFAULT_SLSPICKANDFLIPVARRULE, SLS_PICKANDFLIPVARRULE_MAXVALUE);
    sls_pickAndFlipVarRules_printHelp();
    printf("c    -slsAssInitRule         [uint|%d:%d:%d] "
    		"Determines how the SLS initializes its initial assignment (AIR plug-in).\n",
    		SLS_ASSINITRULE_MINVALUE, PARAM_DEFAULT_SLSASSINITRULE, SLS_ASSINITRULE_MAXVALUE);
    sls_assInitRules_printHelp();
    printf("c    -slsTryRestartRule      [uint|%d:%d:%d] "
    		"Determines how the SLS retries and with what try length (TRR plug-in).\n",
    		SLS_TRYRESTARTRULE_MINVALUE, PARAM_DEFAULT_SLSTRYRESTARTRULE, SLS_TRYRESTARTRULE_MAXVALUE);
    sls_tryRestartRules_printHelp();
    //CDCL
    printf("c   CDCL module:\n");
    printf("c    -cdclOutput             [char*] "
    		"The relative path and filename to store the current CDCL formula.\n");
    printf("c    -cdclRestartStagMaxFrac [float|%f:%f:%f] "
    		"When the computed stack stagnation is above this value, we do not do a restart in the RR.\n",
    		PARAM_HARDMIN_CDCLRESTARTSTAGMAXFRAC, PARAM_DEFAULT_CDCLRESTARTSTAGMAXFRAC, PARAM_HARDMAX_CDCLRESTARTSTAGMAXFRAC);
    printf("c    -cdclRestartInnerouterInit [uint|%d:%d:%d] "
    		"How many conflict need to be found initially in the inner/outer restart strategy.\n",
    		PARAM_HARDMIN_CDCLRESTARTINNEROUTERINIT, PARAM_DEFAULT_CDCLRESTARTINNEROUTERINIT, PARAM_HARDMAX_CDCLRESTARTINNEROUTERINIT);
    printf("c    -cdclRestartInnerouterInc [float|%f:%f:%f] "
    		"The increase factor for the outer bound in the inner/outer restart strategy.\n",
    		PARAM_HARDMIN_CDCLRESTARTINNEROUTERINC, PARAM_DEFAULT_CDCLRESTARTINNEROUTERINC, PARAM_HARDMAX_CDCLRESTARTINNEROUTERINC);
    printf("c    -cdclRestartLubyUnit    [uint|%d:%d:%d] "
    		"The basic value for the luby series (reluctant doubling sequence).\n",
    		PARAM_HARDMIN_CDCLRESTARTLUBYUNIT, PARAM_DEFAULT_CDCLRESTARTLUBYUNIT, PARAM_HARDMAX_CDCLRESTARTLUBYUNIT);
    printf("c    -cdclRestartVarAgiIncBump [float|%f:%f:%f] "
    		"The value with which we bump the increase value for variable agilities (NOT the agilities) in RR.\n",
    		PARAM_HARDMIN_CDCLRESTARTVARAGIINCBUMP, PARAM_DEFAULT_CDCLRESTARTVARAGIINCBUMP, PARAM_HARDMAX_CDCLRESTARTVARAGIINCBUMP);
    printf("c    -cdclRestartMaxAgiFrac  [float|%f:%f:%f] "
    		"The maximum fraction of variable with high agility allowed in a restart for the RR.\n",
    		PARAM_HARDMIN_CDCLRESTARTMAXAGIFRAC, PARAM_DEFAULT_CDCLRESTARTMAXAGIFRAC, PARAM_HARDMAX_CDCLRESTARTMAXAGIFRAC);
    printf("c    -cdclRestartMaxAgiOutFrac [float|%f:%f:%f] "
    		"The maximum fraction of variable with high agility allowed in an outer restart for the RR.\n",
    		PARAM_HARDMIN_CDCLRESTARTMAXAGIOUTFRAC, PARAM_DEFAULT_CDCLRESTARTMAXAGIOUTFRAC, PARAM_HARDMAX_CDCLRESTARTMAXAGIOUTFRAC);
    printf("c    -cdclSelectVarActIncBump [float|%f:%f:%f] "
    		"The value with which we bump the increase value for variable activities (NOT the activities) in SVR/CAR.\n",
    		PARAM_HARDMIN_CDCLSELECTVARACTINCBUMP, PARAM_DEFAULT_CDCLSELECTVARACTINCBUMP, PARAM_HARDMAX_CDCLSELECTVARACTINCBUMP);
    printf("c    -cdclSelectVarRandProb  [float|%f:%f:%f] "
    		"The probability to select a variable at random for decision if the SVR supports it.\n",
    		PARAM_HARDMIN_CDCLSELECTVARRANDPROB, PARAM_DEFAULT_CDCLSELECTVARRANDPROB, PARAM_HARDMAX_CDCLSELECTVARRANDPROB);
    printf("c    -cdclSelectDirMinFlipDist [uint|%d:%d:%d] "
    		"Minimum distance in DL1 decisions before flipping the assignment to a DL1 decision again.\n",
    		PARAM_HARDMIN_CDCLSELECTDIRMINFLIPDIST, PARAM_DEFAULT_CDCLSELECTDIRMINFLIPDIST, PARAM_HARDMAX_CDCLSELECTDIRMINFLIPDIST);
    printf("c    -cdclMaintenanceMinDev  [float|%f:%f:%f] "
    		"If the standard deviation in scaled LBD values is below this, we perform activity based CDBM.\n",
    		PARAM_HARDMIN_CDCLMAINTENANCEMINDEV, PARAM_DEFAULT_CDCLMAINTENANCEMINDEV, PARAM_HARDMAX_CDCLMAINTENANCEMINDEV);
    printf("c    -cdclMaintenanceMaxAvg  [float|%f:%f:%f] "
    		"If the average in scaled LBD values is above this, we perform activity based CDBM.\n",
    		PARAM_HARDMIN_CDCLMAINTENANCEMAXAVG, PARAM_DEFAULT_CDCLMAINTENANCEMAXAVG, PARAM_HARDMAX_CDCLMAINTENANCEMAXAVG);
    printf("c    -cdclMaintenanceActIncBump [float|%f:%f:%f] "
    		"The value with which we bump the increase value for clause activities (NOT the activities) in SVR/CAR.\n",
    		PARAM_HARDMIN_CDCLMAINTENANCEACTINCBUMP, PARAM_DEFAULT_CDCLMAINTENANCEACTINCBUMP, PARAM_HARDMAX_CDCLMAINTENANCEACTINCBUMP);
    printf("c    -cdclMaintenanceInitial [float|%f:%f:%f] "
    		"The fraction of initial clauses (not counting reasons) we need before we do the first clause maintenance.\n",
    		PARAM_HARDMIN_CDCLMAINTENANCEINITIAL, PARAM_DEFAULT_CDCLMAINTENANCEINITIAL, PARAM_HARDMAX_CDCLMAINTENANCEINITIAL);
    printf("c    -cdclMaintenanceIncrease [float|%f:%f:%f] "
    		"The value with which we increase the target number of learned clauses for clause maintenance.\n",
    		PARAM_HARDMIN_CDCLMAINTENANCEINCREASE, PARAM_DEFAULT_CDCLMAINTENANCEINCREASE, PARAM_HARDMAX_CDCLMAINTENANCEINCREASE);
    printf("c    -cdclMaintenanceBase    [int|%d:%d:%d] "
    		"The absolute number of initial clauses that must be learned before CDBM.\n",
    		PARAM_HARDMIN_CDCLMAINTENANCEBASE, PARAM_DEFAULT_CDCLMAINTENANCEBASE, PARAM_HARDMAX_CDCLMAINTENANCEBASE);
    printf("c    -cdclMaintenanceBoost   [int|%d:%d:%d] "
    		"How many more clauses do we want to learn before a new CDBM is performed.\n",
    		PARAM_HARDMIN_CDCLMAINTENANCEBOOST, PARAM_DEFAULT_CDCLMAINTENANCEBOOST, PARAM_HARDMAX_CDCLMAINTENANCEBOOST);
    printf("c    -cdclIRUNHRounds        [uint|%d:%d:%d] "
    		"How many UNHIDING rounds to perform in a single call to the technique.\n",
    		PARAM_HARDMIN_CDCLIRUNHROUNDS, PARAM_DEFAULT_CDCLIRUNHROUNDS, PARAM_HARDMAX_CDCLIRUNHROUNDS);
    printf("c    -cdclIRTERNARYLitMax    [uint|%d:%d:%d] "
    		"How many ternary clauses is in-processing allowed to learn for a single resolution variable at maximum.\n",
    		PARAM_HARDMIN_CDCLIRTERNARYLITMAX, PARAM_DEFAULT_CDCLIRTERNARYLITMAX, PARAM_HARDMAX_CDCLIRTERNARYLITMAX);
    printf("c    -cdclIRTERNARYMax       [uint|%d:%d:%d] "
    		"How many ternary clauses is in-processing allowed to learn in total for a single call to ternary resolution.\n",
    		PARAM_HARDMIN_CDCLIRTERNARYMAX, PARAM_DEFAULT_CDCLIRTERNARYMAX, PARAM_HARDMAX_CDCLIRTERNARYMAX);
    printf("c    -cdclIRNIVERMax         [uint|%d:%d:%d] "
    		"The maximum variable total occurrence for checking it with NIVER during in-processing.\n",
    		PARAM_HARDMIN_CDCLIRNIVERMAX, PARAM_DEFAULT_CDCLIRNIVERMAX, PARAM_HARDMAX_CDCLIRNIVERMAX);
    printf("c    -cdclIRGEMaxAONN        [uint|%d:%d:%d] "
    		"Maximum number of resolvents allowed to be added in GE during in-processing for AND/OR/NAND/NOR gates.\n",
    		PARAM_HARDMIN_CDCLIRGEMAXAONN, PARAM_DEFAULT_CDCLIRGEMAXAONN, PARAM_HARDMAX_CDCLIRGEMAXAONN);
    printf("c    -cdclIRGEMaxXOR         [uint|%d:%d:%d] "
    		"Maximum number of resolvents allowed to be added in GE during in-processing for XOR gates.\n",
    		PARAM_HARDMIN_CDCLIRGEMAXXOR, PARAM_DEFAULT_CDCLIRGEMAXXOR, PARAM_HARDMAX_CDCLIRGEMAXXOR);
    printf("c    -cdclIRStrMaxSize       [uint|%d:%d:%d] "
    		"Maximum clause size to use clause in IR strengthening.\n",
    		PARAM_HARDMIN_CDCLIRSTRMAXSIZE, PARAM_DEFAULT_CDCLIRSTRMAXSIZE, PARAM_HARDMAX_CDCLIRSTRMAXSIZE);
    printf("c    -cdclIRNHBRMin          [uint|%d:%d:%d] "
    		"Minimum number of implications a literal must have to be used for NHBR.\n",
    		PARAM_HARDMIN_CDCLIRNHBRMIN, PARAM_DEFAULT_CDCLIRNHBRMIN, PARAM_HARDMAX_CDCLIRNHBRMIN);
    printf("c    -cdclIRMinConfDistance  [float|%f:%f:%f] "
    		"This multiplied with the number of variables gives the minimum conflicts between in-processing.\n",
    		PARAM_HARDMIN_CDCLIRMINCONFDISTANCE, PARAM_DEFAULT_CDCLIRMINCONFDISTANCE, PARAM_HARDMAX_CDCLIRMINCONFDISTANCE);
    printf("c    -cdclSelectVarRule      [uint|%d:%d:%d] "
    		"Determines what heuristic to use to pick the next variable to assign (SVR plug-in).\n",
    		CDCL_SELECTVARRULE_MINVALUE, PARAM_DEFAULT_CDCLSELECTVARRULE, CDCL_SELECTVARRULE_MAXVALUE);
    cdcl_selectVarRules_printHelp();
    printf("c    -cdclSelectDirRule      [uint|%d:%d:%d] "
    		"Determines what heuristic to use to pick an assignment for the decision variable (SVR plug-in).\n",
    		CDCL_SELECTDIRRULE_MINVALUE, PARAM_DEFAULT_CDCLSELECTDIRRULE, CDCL_SELECTDIRRULE_MAXVALUE);
    cdcl_selectDirRules_printHelp();
    printf("c    -cdclConflictAnalysisRule [uint|%d:%d:%d] "
    		"Determines what conflict analysis to perform in order to learn new clauses (CAR plug-in).\n",
    		CDCL_CONFLICTANALYSISRULE_MINVALUE, PARAM_DEFAULT_CDCLCONFLICTANALYSISRULE, CDCL_CONFLICTANALYSISRULE_MAXVALUE);
    cdcl_conflictAnalysisRules_printHelp();
    printf("c    -cdclStrLearnedRule     [uint|%d:%d:%d] "
    		"Determines what strengthening of learned clauses to perform (SLR plug-in).\n",
    		CDCL_STRLEARNEDRULE_MINVALUE, PARAM_DEFAULT_CDCLSTRLEARNEDRULE, CDCL_STRLEARNEDRULE_MAXVALUE);
    cdcl_strLearnedRules_printHelp();
    printf("c    -cdclStrOtherRule       [uint|%d:%d:%d] "
    		"Determines what strengthening of other with learned clauses to perform (SOR plug-in).\n",
    		CDCL_STROTHERRULE_MINVALUE, PARAM_DEFAULT_CDCLSTROTHERRULE, CDCL_STROTHERRULE_MAXVALUE);
    cdcl_strOtherRules_printHelp();
    printf("c    -cdclComputeBJLRule     [uint|%d:%d:%d] "
    		"Determines what computation is to be performed for computing the back-jump level (CBJLR plug-in).\n",
    		CDCL_COMPUTEBJLRULE_MINVALUE, PARAM_DEFAULT_CDCLCOMPUTEBJLRULE, CDCL_COMPUTEBJLRULE_MAXVALUE);
    cdcl_computeBJLRules_printHelp();
    printf("c    -cdclRestartRule        [uint|%d:%d:%d] "
    		"Determines what type of restarts to perform (RR plug-in).\n",
    		CDCL_RESTARTRULE_MINVALUE, PARAM_DEFAULT_CDCLRESTARTRULE, CDCL_RESTARTRULE_MAXVALUE);
    cdcl_restartRules_printHelp();
    printf("c    -cdclMaintenanceRule    [uint|%d:%d:%d] "
    		"Determines what type of clause database maintenance to perform (MR plug-in).\n",
    		CDCL_MAINTENANCERULE_MINVALUE, PARAM_DEFAULT_CDCLMAINTENANCERULE, CDCL_MAINTENANCERULE_MAXVALUE);
    cdcl_maintenanceRules_printHelp();
    printf("c    -cdclInprocessingRule   [uint|%d:%d:%d] "
    		"Determines what type of in-processing to perform (IR plug-in).\n",
    		CDCL_INPROCESSINGRULE_MINVALUE, PARAM_DEFAULT_CDCLINPROCESSINGRULE, CDCL_INPROCESSINGRULE_MAXVALUE);
    cdcl_inprocessingRules_printHelp();
    printf("c    -cdclAbortRule          [uint|%d:%d:%d] "
    		"Determines what type of search abort rule to use (AR plug-in).\n",
    		CDCL_ABORTRULE_MINVALUE, PARAM_DEFAULT_CDCLABORTRULE, CDCL_ABORTRULE_MAXVALUE);
    cdcl_abortRules_printHelp();
    //HANDLERESULT
    printf("c   HANDLERESULT:\n");
    printf("c    -handleresultCoarsen    [uint|%d:%d:%d] "
    		"Coarsens a satisfying assignment by unassigning variables such that all clauses retain at least one true literal.\n",
    		PARAM_HARDMIN_HANDLERESULTCOARSEN, PARAM_DEFAULT_HANDLERESULTCOARSEN, PARAM_HARDMAX_HANDLERESULTCOARSEN);
    //SOLWRITER
    printf("c   SOLWRITER module:\n");
    printf("c    -solwriterPrintAssignment [bool|%d|0:1:1] "
    		"Set this to 0 if you do not want the satisfying assignment to be presented.\n",
    		PARAM_DEFAULT_SOLWRITERPRINTASSIGNMENT);
    //SHUTDOWN
    printf("c   SHUTDOWN:\n");
    printf("c     The SHUTDOWN phase currently has no parameters.\n");
    printf("c\n");
}

//Opening files.
#ifdef SPECS_PARAMGRAPH
void params_openSpecsGraphOutputFile(int argc,char* argv[],int32_t paramNumber){
	#ifdef VERBOSE_PARAMS
	printf("c   PARAMS:   -specsGraphOutput... ");
	#endif

	//The file is opened here, but we leave it open until it has been read completely. See main.c:initializeMain().
	param_specsGraphOutput = fopen(argv[paramNumber], "w");
	if (param_specsGraphOutput == NULL) {
		printf("\nc ERROR. You must specify an output file that you have write access to!\n");
		params_returnCode = PARAMS_BADPARAM;
	} else {
		#ifdef VERBOSE_PARAMS
		printf("%s opened.\n", argv[paramNumber]);
		#endif
	}
}
#endif

void params_openFormulaFile(int argc,char* argv[],int32_t paramNumber){
	#ifdef VERBOSE_PARAMS
	printf("c   PARAMS:   -formula... ");
	#endif

	//The file is opened here, but we leave it open until it has been read completely. See main.c:initializeMain().
	param_formula = fopen(argv[paramNumber], "r");
	if (param_formula == NULL) {
		printf("\nc ERROR. You must specify an input file that you have read access to!\n");
		params_returnCode = PARAMS_BADPARAM;
	} else {
		#ifdef VERBOSE_PARAMS
		printf("%s opened.\n", argv[paramNumber]);
		#endif
	}
}

void params_openStartupOutputFile(int argc,char* argv[],int32_t paramNumber){
	#ifdef VERBOSE_PARAMS
	printf("c   PARAMS:   -startupOutput... ");
	#endif
	param_startupOutput = fopen(argv[paramNumber], "w");
	if (param_startupOutput == NULL) {
		printf("\nc ERROR. You must specify an output file that you have write access to!\n");
		params_returnCode = PARAMS_BADPARAM;
	} else {
		#ifdef VERBOSE_PARAMS
		printf("%s opened.\n", argv[paramNumber]);
		#endif
	}
}

void params_openPrepOutputFile(int argc,char* argv[],int32_t paramNumber){
	#ifdef VERBOSE_PARAMS
	printf("c   PARAMS:   -prepOutput... ");
	#endif
	param_prepOutput = fopen(argv[paramNumber], "w");
	if (param_prepOutput == NULL) {
		printf("\nc ERROR. You must specify an output file that you have write access to!\n");
		params_returnCode = PARAMS_BADPARAM;
	} else {
		#ifdef VERBOSE_PARAMS
		printf("%s opened.\n", argv[paramNumber]);
		#endif
	}
}

void params_openCdclOutputFile(int argc,char* argv[],int32_t paramNumber){
	#ifdef VERBOSE
	printf("c   PARAMS:   -cdclOutput... ");
	#endif
	param_cdclOutput = fopen(argv[paramNumber], "w");
	if (param_cdclOutput == NULL) {
		printf("\nc ERROR. You must specify an CDCL output file that you have write access to!\n");
		params_returnCode = PARAMS_BADPARAM;
	} else {
		#ifdef VERBOSE
		printf("%s opened.\n", argv[paramNumber]);
		#endif
	}
}

//Helper methods for reading from the command line.
//Reading integers.
void params_readIntUnlimited(int argc, char* argv[], int32_t paramNumber, char* name, int32_t *par){
	#ifdef VERBOSE_PARAMS
	printf("c   PARAMS:     -%s... ", name);
	#endif
	if (paramNumber < 0 || paramNumber >= argc || sscanf(argv[paramNumber],"%i", par)!=1){
		printf("\nc ERROR. Reading the integer following %s.\n", name);
		params_returnCode = PARAMS_BADPARAM; return;
	}
	#ifdef VERBOSE_PARAMS
	printf("%d.\n",*par);
	#endif
}

void params_readIntMinMax(int argc, char* argv[], int32_t paramNumber, char* name, int32_t *par, int32_t min, int32_t max){
	#ifdef VERBOSE_PARAMS
	printf("c   PARAMS:   -%s... ", name);
	#endif
	if (paramNumber < 0 || paramNumber >= argc || sscanf(argv[paramNumber],"%i", par)!=1){
		printf("\nc ERROR. Reading the integer following %s.\n", name);
		params_returnCode = PARAMS_BADPARAM; return;
	}
	if (*par < min || *par > max){
		printf("\nc ERROR. The %s value must be at least %d and at most %d.\n", name, min, max);
		params_returnCode = PARAMS_BADPARAM; return;
	}
	#ifdef VERBOSE_PARAMS
	printf("%d.\n",*par);
	#endif
}

void params_readIntMinMaxEx(int argc, char* argv[], int32_t paramNumber, char* name, int32_t *par, int32_t min, int32_t max, int32_t except){
	#ifdef VERBOSE_PARAMS
	printf("c   PARAMS:   -%s... ", name);
	#endif
	if (paramNumber < 0 || paramNumber >= argc || sscanf(argv[paramNumber],"%i", par)!=1){
		printf("\nc ERROR. Reading the integer following %s.\n", name);
		params_returnCode = PARAMS_BADPARAM; return;
	}
	if (*par != except && (*par < min || *par > max)){
		printf("\nc ERROR. The %s value must be either %d, or at least %d and at most %d.\n", name, except, min, max);
		params_returnCode = PARAMS_BADPARAM; return;
	}
	#ifdef VERBOSE_PARAMS
	if (params_returnCode == PARAMS_UNKNOWN) printf("%d.\n",*par);
	#endif
}

//Reading floats.
void params_readFloatUnlimited(int argc, char* argv[], int32_t paramNumber, char* name, float_ty *par){
	#ifdef VERBOSE_PARAMS
	printf("c   PARAMS:   -%s... ", name);
	#endif
	double value;
	if (paramNumber < 0 || paramNumber >= argc || sscanf(argv[paramNumber],"%lf", &value)!=1){
		printf("c ERROR. Reading the float following %s.\n", name);
		params_returnCode = PARAMS_BADPARAM; return;
	}
	*par = (float_ty) value;
	#ifdef VERBOSE_PARAMS
	printf("%f.\n",*par);
	#endif
}

void params_readFloatMinMax(int argc, char* argv[], int32_t paramNumber, char* name, float_ty *par, float_ty min, float_ty max){
	#ifdef VERBOSE_PARAMS
	printf("c   PARAMS:   -%s... ", name);
	#endif
	double value;
	if (paramNumber < 0 || paramNumber >= argc || sscanf(argv[paramNumber],"%lf", &value)!=1){
		printf("c ERROR. Reading the float following %s.\n", name);
		params_returnCode = PARAMS_BADPARAM; return;
	}
	*par = (float_ty) value;
	if (*par < min || *par > max){
		printf("\nc ERROR. The %s value must be at least %f and at most %f.\n", name, min, max);
		params_returnCode = PARAMS_BADPARAM; return;
	}
	#ifdef VERBOSE_PARAMS
	printf("%f.\n",*par);
	#endif
}

void params_readFloatMinMaxEx(int argc, char* argv[], int32_t paramNumber, char* name, float_ty *par, float_ty min, float_ty max, float_ty except){
	#ifdef VERBOSE_PARAMS
	printf("c   PARAMS:   -%s... ", name);
	#endif
	double value;
	if (paramNumber < 0 || paramNumber >= argc || sscanf(argv[paramNumber],"%lf", &value)!=1){
		printf("c ERROR. Reading the float following %s.\n", name);
		params_returnCode = PARAMS_BADPARAM; return;
	}
	*par = (float_ty) value;
	if (*par != except && (*par < min || *par > max)){
		printf("\nc ERROR. The %s value must be either %f, or at least %f and at most %f.\n", name, except, min, max);
		params_returnCode = PARAMS_BADPARAM; return;
	} else {
		#ifdef VERBOSE_PARAMS
		printf("%f.\n", *par);
		#endif
	}
}

//Finally, the methods that check the command line and output the result.
void params_extern_readSpecial(int argc, char** argv){
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_params_component_totalCalls;
	#endif
	#ifdef VERBOSE_PARAMS
	printf("c   PARAMS: Reading special parameters...\n");
	#endif
	int32_t parameter=0;
	for (parameter=1; parameter<argc && params_returnCode == PARAMS_UNKNOWN; ++parameter){
		if ( !strcmp("h", argv[parameter])
				|| !strcmp("/h", argv[parameter])
				|| !strcmp("-h", argv[parameter])
				|| !strcmp("help", argv[parameter])
				|| !strcmp("-help", argv[parameter])
				|| !strcmp("--help", argv[parameter])
			){
			#ifdef VERBOSE_PARAMS
			printf("c   PARAMS:   Special parameter for showing the help has been read.\n");
			#endif
			params_returnCode = PARAMS_SPECIALPARAM_HELP;
			break;
		}
		if (!strcmp("v", argv[parameter])
				|| !strcmp("/v", argv[parameter])
				|| !strcmp("-v", argv[parameter])
				|| !strcmp("version", argv[parameter])
				|| !strcmp("-version", argv[parameter])
				|| !strcmp("--version", argv[parameter])
			){
			#ifdef VERBOSE_PARAMS
			printf("c   PARAMS:   Special parameter for showing the version information has been read.\n");
			#endif
			params_returnCode = PARAMS_SPECIALPARAM_VERSION;
			break;
		}
		if (!strcmp("s", argv[parameter])
				|| !strcmp("/s", argv[parameter])
				|| !strcmp("-s", argv[parameter])
				|| !strcmp("specs", argv[parameter])
				|| !strcmp("-specs", argv[parameter])
				|| !strcmp("--specs", argv[parameter])
			){
			#ifdef VERBOSE_PARAMS
			printf("c   PARAMS:   Special parameter for showing the specifications has been read.\n");
			#endif
			params_returnCode = PARAMS_SPECIALPARAM_SPECS;
			break;
		}
	}
	if (params_returnCode == PARAMS_SPECIALPARAM_SPECS){
		//We hunt the seed because the random numbers might be needed while printing the solver specifications.
		for (parameter=1; parameter<argc; ++parameter){
			if (!strcmp("-seed",argv[parameter])){
				++parameter;
				if (parameter < argc){
					//We do not catch any errors here. This can fail, if it wants to.
					sscanf(argv[parameter],"%i", &param_seed);
					break;
				} else {
					param_seed = 42;
					break;
				}
			}
		}
		#ifdef SPECS_PARAMGRAPH
		for (parameter=1; parameter<argc; ++parameter){
			if (!strcmp("-specsGraphOutput", argv[parameter])){
				params_openSpecsGraphOutputFile(argc,argv,++parameter);
				break;
			}
		}
		#endif
	}

	#ifdef COLLECTSTATS
	stats_params_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}

void params_extern_readNormal(int argc, char** argv){
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_params_component_totalCalls;
	#endif
	#ifdef VERBOSE_PARAMS
	printf("c   PARAMS: Reading normal parameters...\n");
	#endif
	int32_t p;
	//READ FROM COMMAND LINE
	for (p=1; p<argc && params_returnCode == PARAMS_UNKNOWN; ++p){
		//MAIN
		if (!strcmp("-formula",argv[p])) {
			params_openFormulaFile(argc,argv,++p); continue;
		}
		if (!strcmp("-seed",argv[p])){
			params_readIntUnlimited(argc,argv,++p,
					"seed",&param_seed);continue;
		}

		//RESET

		//PARAMS

		//GUIDANCE
		if (!strcmp("-guide",argv[p])){
			params_readIntMinMaxEx(argc,argv,++p,
					"guide",&param_guide,
					GUIDE_MINVALUE,GUIDE_MAXVALUE,GUIDE_TESTING);continue;
		}

		//STARTUP
		if (!strcmp("-startupOutput",argv[p])){
			params_openStartupOutputFile(argc,argv,++p);continue;
		}
		if (!strcmp("-startupInputFormat",argv[p])){
			params_readIntMinMaxEx(argc,argv,++p,
					"startupInputFormat",&param_startupInputFormat,
					STARTUP_INPUTFORMAT_MINNUM,STARTUP_INPUTFORMAT_MAXNUM, STARTUP_INPUTFORMAT_UNKNOWN);continue;
		}

		//CLASSIFY
		if (!strcmp("-classifyInputDomain",argv[p])){
			params_readIntMinMaxEx(argc,argv,++p,
					"classifyInputDomain",&param_classifyInputDomain,
					CLASSIFY_INPUTDOMAIN_MINNUM,CLASSIFY_INPUTDOMAIN_MAXNUM, CLASSIFY_INPUTDOMAIN_GENERIC);continue;
		}
		if (!strcmp("-classifyAttSetTime",argv[p])){
			params_readFloatMinMax(argc,argv,++p,
					 "classifyAttSetTime",&param_classifyAttSetTime,ZERO,INT32_MAX);continue;
		}
		if (!strcmp("-classifyFloatPrec",argv[p])) {
			params_readIntMinMax(argc,argv,++p,
					 "classifyFloatPrec",&param_classifyFloatPrec,1,6);continue;
		}
		if (!strcmp("-classifyMaxVGSize",argv[p])){
			params_readIntMinMax(argc,argv,++p,
					 "classifyMaxVGSize",&param_classifyMaxVGSize,0,INT32_MAX);continue;
		}
		if (!strcmp("-classifyMaxCGSize",argv[p])){
			params_readIntMinMax(argc,argv,++p,
					 "classifyMaxCGSize",&param_classifyMaxCGSize,0,INT32_MAX);continue;
		}
		if (!strcmp("-classifyMaxUPImps",argv[p])){
			params_readIntMinMax(argc,argv,++p,
					 "classifyMaxUPImps",&param_classifyMaxUPImps,0,INT32_MAX);continue;
		}
		if (!strcmp("-classifyMaxUPn",argv[p])){
			params_readIntMinMax(argc,argv,++p,
				 	 "classifyMaxUPn",&param_classifyMaxUPn,0,INT32_MAX);continue;
		}
		if (!strcmp("-classifyPGFA",argv[p])){
			params_readFloatMinMax(argc,argv,++p,
				 	 "classifyPGFA",&param_classifyPGFA,0.000001,0.999999);continue;
		}
		if (!strcmp("-classifyPGFB",argv[p])){
			params_readFloatMinMax(argc,argv,++p,
				 	 "classifyPGFB",&param_classifyPGFB,0.000001,0.999999);continue;
		}
		if (!strcmp("-classifyPGFC",argv[p])){
			params_readFloatMinMax(argc,argv,++p,
				 	 "classifyPGFC",&param_classifyPGFC,0.000001,0.999999);continue;
		}
		if (!strcmp("-classifySimplify",argv[p])){
			params_readIntMinMax(argc,argv,++p,
					 "classifySimplify",&param_classifySimplify,0,1);continue;
		}
		if (!strcmp("-classifyStrategy",argv[p])){
			params_readIntMinMaxEx(argc,argv,++p,
					 "classifyStrategy",&param_classifyStrategy,
					 CLASSIFY_STRATEGY_MINVALUE,CLASSIFY_STRATEGY_MAXVALUE,CLASSIFY_STRATEGY_TESTING);continue;
		}

		//ADAPT
		if (!strcmp("-adaptStrategy",argv[p])){
			params_readIntMinMaxEx(argc,argv,++p,
					 "adaptStrategy",&param_adaptStrategy,
					 ADAPT_STRATEGY_MINVALUE,ADAPT_STRATEGY_MAXVALUE,ADAPT_STRATEGY_TESTING);continue;
		}
		if (!strcmp("-adaptPrepStrategy",argv[p])){
			params_readIntMinMax(argc,argv,++p,
					 "adaptPrepStrategy",&param_adaptPrepStrategy,0,1);continue;
		}
		if (!strcmp("-adaptPrepStrategyParams",argv[p])){
			params_readIntMinMax(argc,argv,++p,
					 "adaptPrepStrategyParams",&param_adaptPrepStrategyParams,0,1);continue;
		}
		if (!strcmp("-adaptInpStrategy",argv[p])){
			params_readIntMinMax(argc,argv,++p,
					 "adaptInpStrategy",&param_adaptInpStrategy,0,1);continue;
		}
		if (!strcmp("-adaptInpStrategyParams",argv[p])){
			params_readIntMinMax(argc,argv,++p,
					 "adaptInpStrategyParams",&param_adaptInpStrategyParams,0,1);continue;
		}
		if (!strcmp("-adaptSearchStrategy",argv[p])){
			params_readIntMinMax(argc,argv,++p,
					 "adaptSearchStrategy",&param_adaptSearchStrategy,0,1);continue;
		}
		if (!strcmp("-adaptSearchStrategyParams",argv[p])){
			params_readIntMinMax(argc,argv,++p,
					 "adaptSearchStrategyParams",&param_adaptSearchStrategyParams,0,1);continue;
		}
		if (!strcmp("-adaptMpUR",argv[p])){
			params_readIntMinMax(argc,argv,++p,
					 "adaptMpUR",&param_adaptMpUR,0,1);continue;
		}
		if (!strcmp("-adaptMpURParams",argv[p])){
			params_readIntMinMax(argc,argv,++p,
					 "adaptMpURParams",&param_adaptMpURParams,0,1);continue;
		}
		if (!strcmp("-adaptSlsPAFVR",argv[p])){
			params_readIntMinMax(argc,argv,++p,
					 "adaptSlsPAFVR",&param_adaptSlsPAFVR,0,1);continue;
		}
		if (!strcmp("-adaptSlsPAFVRParams",argv[p])){
			params_readIntMinMax(argc,argv,++p,
					 "adaptSlsPAFVRParams",&param_adaptSlsPAFVRParams,0,1);continue;
		}
		if (!strcmp("-adaptSlsAIR",argv[p])){
			params_readIntMinMax(argc,argv,++p,
					 "adaptSlsAIR",&param_adaptSlsAIR,0,1);continue;
		}
		if (!strcmp("-adaptSlsAIRParams",argv[p])){
			params_readIntMinMax(argc,argv,++p,
					 "adaptSlsAIRParams",&param_adaptSlsAIRParams,0,1);continue;
		}
		if (!strcmp("-adaptSlsTRR",argv[p])){
			params_readIntMinMax(argc,argv,++p,
					 "adaptSlsTRR",&param_adaptSlsTRR,0,1);continue;
		}
		if (!strcmp("-adaptSlsTRRParams",argv[p])){
			params_readIntMinMax(argc,argv,++p,
					 "adaptSlsTRRParams",&param_adaptSlsTRRParams,0,1);continue;
		}
		if (!strcmp("-adaptCdclSVR",argv[p])){
			params_readIntMinMax(argc,argv,++p,
					 "adaptCdclSVR",&param_adaptCdclSVR,0,1);continue;
		}
		if (!strcmp("-adaptCdclSVRParams",argv[p])){
			params_readIntMinMax(argc,argv,++p,
					 "adaptCdclSVRParams",&param_adaptCdclSVRParams,0,1);continue;
		}
		if (!strcmp("-adaptCdclSVAI",argv[p])){
			params_readIntMinMax(argc,argv,++p,
					 "adaptCdclSVAI",&param_adaptCdclSVAI,0,1);continue;
		}
		if (!strcmp("-adaptCdclSVAIParams",argv[p])){
			params_readIntMinMax(argc,argv,++p,
					 "adaptCdclSVAIParams",&param_adaptCdclSVAIParams,0,1);continue;
		}
		if (!strcmp("-adaptCdclSDR",argv[p])){
			params_readIntMinMax(argc,argv,++p,
					 "adaptCdclSDR",&param_adaptCdclSDR,0,1);continue;
		}
		if (!strcmp("-adaptCdclSDRParams",argv[p])){
			params_readIntMinMax(argc,argv,++p,
					 "adaptCdclSDRParams",&param_adaptCdclSDRParams,0,1);continue;
		}
		if (!strcmp("-adaptCdclSDDI",argv[p])){
			params_readIntMinMax(argc,argv,++p,
					 "adaptCdclSDDI",&param_adaptCdclSDDI,0,1);continue;
		}
		if (!strcmp("-adaptCdclSDDIParams",argv[p])){
			params_readIntMinMax(argc,argv,++p,
					 "adaptCdclSDDIParams",&param_adaptCdclSDDIParams,0,1);continue;
		}
		if (!strcmp("-adaptCdclCAR",argv[p])){
			params_readIntMinMax(argc,argv,++p,
					 "adaptCdclCAR",&param_adaptCdclCARParams,0,1);continue;
		}
		if (!strcmp("-adaptCdclCARParams",argv[p])){
			params_readIntMinMax(argc,argv,++p,
					 "adaptCdclCARParams",&param_adaptCdclCARParams,0,1);continue;
		}
		if (!strcmp("-adaptCdclSLR",argv[p])){
			params_readIntMinMax(argc,argv,++p,
					 "adaptCdclSLR",&param_adaptCdclSLR,0,1);continue;
		}
		if (!strcmp("-adaptCdclSLRParams",argv[p])){
			params_readIntMinMax(argc,argv,++p,
					 "adaptCdclSLRParams",&param_adaptCdclSLRParams,0,1);continue;
		}
		if (!strcmp("-adaptCdclSOR",argv[p])){
			params_readIntMinMax(argc,argv,++p,
					 "adaptCdclSOR",&param_adaptCdclSOR,0,1);continue;
		}
		if (!strcmp("-adaptCdclSORParams",argv[p])){
			params_readIntMinMax(argc,argv,++p,
					 "adaptCdclSORParams",&param_adaptCdclSORParams,0,1);continue;
		}
		if (!strcmp("-adaptCdclCBJLR",argv[p])){
			params_readIntMinMax(argc,argv,++p,
					 "adaptCdclCBJLR",&param_adaptCdclCBJLR,0,1);continue;
		}
		if (!strcmp("-adaptCdclCBJLRParams",argv[p])){
			params_readIntMinMax(argc,argv,++p,
					 "adaptCdclCBJLRParams",&param_adaptCdclCBJLRParams,0,1);continue;
		}
		if (!strcmp("-adaptCdclRR",argv[p])){
			params_readIntMinMax(argc,argv,++p,
					 "adaptCdclRR",&param_adaptCdclRR,0,1);continue;
		}
		if (!strcmp("-adaptCdclRRParams",argv[p])){
			params_readIntMinMax(argc,argv,++p,
					 "adaptCdclRRParams",&param_adaptCdclRRParams,0,1);continue;
		}
		if (!strcmp("-adaptCdclMR",argv[p])){
			params_readIntMinMax(argc,argv,++p,
					 "adaptCdclMR",&param_adaptCdclMR,0,1);continue;
		}
		if (!strcmp("-adaptCdclMRParams",argv[p])){
			params_readIntMinMax(argc,argv,++p,
					 "adaptCdclMRParams",&param_adaptCdclMRParams,0,1);continue;
		}
		if (!strcmp("-adaptCdclIR",argv[p])){
			params_readIntMinMax(argc,argv,++p,
					 "adaptCdclIR",&param_adaptCdclIR,0,1);continue;
		}
		if (!strcmp("-adaptCdclIRParams",argv[p])){
			params_readIntMinMax(argc,argv,++p,
					 "adaptCdclIRParams",&param_adaptCdclIRParams,0,1);continue;
		}
		if (!strcmp("-adaptCdclAR",argv[p])){
			params_readIntMinMax(argc,argv,++p,
					 "adaptCdclAR",&param_adaptCdclRR,0,1);continue;
		}
		if (!strcmp("-adaptCdclARParams",argv[p])){
			params_readIntMinMax(argc,argv,++p,
					 "adaptCdclARParams",&param_adaptCdclARParams,0,1);continue;
		}
		//PREP
		if (!strcmp("-prepOutput",argv[p])){
			params_openPrepOutputFile(argc,argv,++p);continue;
		}
		if (!strcmp("-prepReviveCls",argv[p])){
			params_readIntMinMax(argc,argv,++p,
					 "prepReviveCls",&param_prepReviveCls,
					 PARAM_HARDMIN_PREPREVIVECLS,PARAM_HARDMAX_PREPREVIVECLS);continue;
		}
		if (!strcmp("-prepUNHRounds",argv[p])){
			params_readIntMinMax(argc,argv,++p,
					 "prepUNHRounds",&param_prepUNHRounds,
					 PARAM_HARDMIN_PREPUNHROUNDS,PARAM_HARDMAX_PREPUNHROUNDS);continue;
		}
		if (!strcmp("-prepACCEMax",argv[p])){
			params_readIntMinMax(argc,argv,++p,
					 "prepACCEMax",&param_prepACCEMax,
					 PARAM_HARDMIN_PREPACCEMAX,PARAM_HARDMAX_PREPACCEMAX);continue;
		}
		if (!strcmp("-prepACCEMinClsSize",argv[p])){
			params_readIntMinMax(argc,argv,++p,
					 "prepACCEMinClsSize",&param_prepACCEMinClsSize,
					 PARAM_HARDMIN_PREPACCEMINCLSSIZE,PARAM_HARDMAX_PREPACCEMINCLSSIZE);continue;
		}
		if (!strcmp("-prepACCESaturate",argv[p])){
			params_readIntMinMax(argc,argv,++p,
					 "prepACCESaturate",&param_prepACCESaturate,
					 PARAM_HARDMIN_PREPACCESATURATE,PARAM_HARDMAX_PREPACCESATURATE);continue;
		}
		if (!strcmp("-prepNIVERMax",argv[p])){
			params_readIntMinMax(argc,argv,++p,
					 "prepNIVERMax",&param_prepNIVERMax,
					 PARAM_HARDMIN_PREPNIVERMAX,PARAM_HARDMAX_PREPNIVERMAX);continue;
		}
		if (!strcmp("-prepATEMin",argv[p])){
			params_readIntMinMax(argc,argv,++p,
					 "prepATEMin",&param_prepATEMin,
					 PARAM_HARDMIN_PREPATEMIN,PARAM_HARDMAX_PREPATEMIN);continue;
		}
		if (!strcmp("-prepGEMaxAONN",argv[p])){
			params_readIntMinMax(argc,argv,++p,
					 "prepGEMaxAONN",&param_prepGEMaxAONN,
					 PARAM_HARDMIN_PREPGEMAXAONN,PARAM_HARDMAX_PREPGEMAXAONN);continue;
		}
		if (!strcmp("-prepGEMaxXOR",argv[p])){
			params_readIntMinMax(argc,argv,++p,
					 "prepGEMaxXOR",&param_prepGEMaxXOR,
					 PARAM_HARDMIN_PREPGEMAXXOR,PARAM_HARDMAX_PREPGEMAXXOR);continue;
		}
		if (!strcmp("-prepTERNARYLitMax",argv[p])){
			params_readIntMinMax(argc,argv,++p,
					 "prepTERNARYLitMax",&param_prepTERNARYLitMax,
					 PARAM_HARDMIN_PREPTERNARYLITMAX,PARAM_HARDMAX_PREPTERNARYLITMAX);continue;
		}
		if (!strcmp("-prepTERNARYMax",argv[p])){
			params_readIntMinMax(argc,argv,++p,
					 "prepTERNARYMax",&param_prepTERNARYMax,
					 PARAM_HARDMIN_PREPTERNARYMAX,PARAM_HARDMAX_PREPTERNARYMAX);continue;
		}
		if (!strcmp("-prepNHBRMin",argv[p])){
			params_readIntMinMax(argc,argv,++p,
					 "prepNHBRMin",&param_prepNHBRMin,
					 PARAM_HARDMIN_PREPNHBRMIN,PARAM_HARDMAX_PREPNHBRMIN);continue;
		}
		if (!strcmp("-prepStrategy",argv[p])){
			params_readIntMinMaxEx(argc,argv,++p,
					 "prepStrategy",&param_prepStrategy,
					 PREP_STRATEGY_MINVALUE,PREP_STRATEGY_MAXVALUE,PREP_STRATEGY_TESTING);continue;
		}

		//INP
		if (!strcmp("-inpStrMaxSize",argv[p])){
			params_readIntMinMax(argc,argv,++p,
					 "inpStrMaxSize",&param_inpStrMaxSize,
					 PARAM_HARDMIN_INPSTRMAXSIZE,PARAM_HARDMAX_INPSTRMAXSIZE);continue;
		}
		if (!strcmp("-inpTernaryLitMax",argv[p])){
			params_readIntMinMax(argc,argv,++p,
					 "inpTernaryLitMax",&param_inpTernaryLitMax,
					 PARAM_HARDMIN_INPTERNARYLITMAX,PARAM_HARDMAX_INPTERNARYLITMAX);continue;
		}
		if (!strcmp("-inpTernaryMax",argv[p])){
			params_readIntMinMax(argc,argv,++p,
					 "inpTernaryMax",&param_inpTernaryMax,
					 PARAM_HARDMIN_INPTERNARYMAX,PARAM_HARDMAX_INPTERNARYMAX);continue;
		}
		if (!strcmp("-inpNiverMax",argv[p])){
			params_readIntMinMax(argc,argv,++p,
					 "inpNiverMax",&param_inpNiverMax,
					 PARAM_HARDMIN_INPNIVERMAX,PARAM_HARDMAX_INPNIVERMAX);continue;
		}
		if (!strcmp("-inpGEMaxAONN",argv[p])){
			params_readIntMinMax(argc,argv,++p,
					 "inpGEMaxAONN",&param_inpGEMaxAONN,
					 PARAM_HARDMIN_INPGEMAXAONN,PARAM_HARDMAX_INPGEMAXAONN);continue;
		}
		if (!strcmp("-inpGEMaxXOR",argv[p])){
			params_readIntMinMax(argc,argv,++p,
					 "inpGEMaxXOR",&param_inpGEMaxXOR,
					 PARAM_HARDMIN_INPGEMAXXOR,PARAM_HARDMAX_INPGEMAXXOR);continue;
		}
		if (!strcmp("-inpNHBRMin",argv[p])){
			params_readIntMinMax(argc,argv,++p,
					 "inpNHBRMin",&param_inpNHBRMin,
					 PARAM_HARDMIN_INPNHBRMIN,PARAM_HARDMAX_INPNHBRMIN);continue;
		}
		if (!strcmp("-inpUNHRounds",argv[p])){
			params_readIntMinMax(argc,argv,++p,
					 "inpUNHRounds",&param_inpUNHRounds,
					 PARAM_HARDMIN_INPUNHROUNDS,PARAM_HARDMAX_INPUNHROUNDS);continue;
		}
		if (!strcmp("-inpStrategy",argv[p])){
			params_readIntMinMaxEx(argc,argv,++p,
					 "inpStrategy",&param_inpStrategy,
					 INP_STRATEGY_MINVALUE,INP_STRATEGY_MAXVALUE,INP_STRATEGY_TESTING);continue;
		}

		//Search related.
		if (!strcmp("-searchMIDBlockSize",argv[p])) {
			params_readFloatMinMax(argc,argv,++p,
					 "searchMIDBlockSize",&param_searchMIDBlockSize,
					 PARAM_HARDMIN_SEARCHMIDBLOCKSIZE,PARAM_HARDMAX_SEARCHMIDBLOCKSIZE);continue;
		}
		if (!strcmp("-searchRetreatFact",argv[p])) {
			params_readFloatMinMax(argc,argv,++p,
					 "searchRetreatFact",&param_searchRetreatFact,
					 PARAM_HARDMIN_SEARCHRETREATFACT,PARAM_HARDMAX_SEARCHRETREATFACT);continue;
		}
		if (!strcmp("-searchMinClsImpact",argv[p])) {
			params_readFloatMinMax(argc,argv,++p,
					 "searchMinClsImpact",&param_searchMinClsImpact,
					 PARAM_HARDMIN_SEARCHMINCLSIMPACT,PARAM_HARDMAX_SEARCHMINCLSIMPACT);continue;
		}
		if (!strcmp("-searchImpactMaxFactor",argv[p])) {
			params_readFloatMinMax(argc,argv,++p,
					 "searchImpactMaxFactor",&param_searchImpactMaxFactor,
					 PARAM_HARDMIN_SEARCHIMPACTMAXFACTOR,PARAM_HARDMAX_SEARCHIMPACTMAXFACTOR);continue;
		}
		if (!strcmp("-searchMaxMagForSLS",argv[p])) {
			params_readFloatMinMax(argc,argv,++p,
					 "searchMaxMagForSLS",&param_searchMaxMagForSLS,
					 PARAM_HARDMIN_SEARCHMAXMAGFORSLS,PARAM_HARDMAX_SEARCHMAXMAGFORSLS);continue;
		}
		if (!strcmp("-searchMPWeightModSAT",argv[p])) {
			params_readFloatMinMax(argc,argv,++p,
					 "searchMPWeightModSAT",&param_searchMPWeightModSAT,
					 PARAM_HARDMIN_SEARCHMPWEIGHTMODSAT,PARAM_HARDMAX_SEARCHMPWEIGHTMODSAT);continue;
		}
		if (!strcmp("-searchMPWeightModUNS",argv[p])) {
			params_readFloatMinMax(argc,argv,++p,
					 "searchMPWeightModUNS",&param_searchMPWeightModUNS,
					 PARAM_HARDMIN_SEARCHMPWEIGHTMODUNS,PARAM_HARDMAX_SEARCHMPWEIGHTMODUNS);continue;
		}
		if (!strcmp("-searchStrategy",argv[p])){
			params_readIntMinMaxEx(argc,argv,++p,
					 "searchStrategy",&param_searchStrategy,
					 SEARCH_STRATEGY_MINVALUE,SEARCH_STRATEGY_MAXVALUE,SEARCH_STRATEGY_TESTING);continue;
		}

		//DETERM
		//Nothing yet.

		//SPECCER
		if (!strcmp("-speccerNNFFirst",argv[p])){
			params_readIntMinMax(argc,argv,++p,
					"speccerNNFFirst",&param_speccerNNFFirst,0,1);continue;
		}

		//LOADER
		//Nothing yet.

		//ATTRIBUTOR
		//Nothing yet.

		//RANDOMFOREST
		//Nothing yet.

		//ITEADAPTER
		//Nothing yet.

		//IN
		//Nothing yet.

		//PRE
		//Nothing yet.

		//SATELITE
		//Nothing yet.

		//MP
		if (!strcmp("-mpMaxNumIterations",argv[p])){
			params_readIntMinMax(argc,argv,++p,
					 "mpMaxNumIterations",&param_mpMaxNumIterations,
					 PARAM_HARDMIN_MPMAXNUMITERATIONS,PARAM_HARDMAX_MPMAXNUMITERATIONS);continue;
		}
		if (!strcmp("-mpMaxNonImpIterations",argv[p])){
			params_readIntMinMax(argc,argv,++p,
					 "mpMaxNonImpIterations",&param_mpMaxNonImpIterations,
					 PARAM_HARDMIN_MPMAXNONIMPITERATIONS,PARAM_HARDMAX_MPMAXNONIMPITERATIONS);continue;
		}
		if (!strcmp("-mpMaxConvergenceDiff",argv[p])) {
			params_readFloatMinMax(argc,argv,++p,
					 "mpMaxConvergenceDiff",&param_mpMaxConvergenceDiff,
					 PARAM_HARDMIN_MPMAXCONVERGENCEDIFF,PARAM_HARDMAX_MPMAXCONVERGENCEDIFF);continue;
		}
		if (!strcmp("-mpWeightNoise",argv[p])) {
			params_readFloatMinMax(argc,argv,++p,
					 "mpWeightNoise",&param_mpWeightNoise,
					 PARAM_HARDMIN_MPWEIGHTNOISE,PARAM_HARDMAX_MPWEIGHTNOISE);continue;
		}
		if (!strcmp("-mpMaxMagnetization",argv[p])) {
			params_readFloatMinMax(argc,argv,++p,
					 "mpMaxMagnetization",&param_mpMaxMagnetization,
					 PARAM_HARDMIN_MPMAXMAGNETIZATION,PARAM_HARDMAX_MPMAXMAGNETIZATION);continue;
		}
		if (!strcmp("-mpEpsilon",argv[p])) {
			params_readFloatMinMax(argc,argv,++p,
					 "mpEpsilon",&param_mpEpsilon,
					 PARAM_HARDMIN_MPEPSILON,PARAM_HARDMAX_MPEPSILON);continue;
		}
		if (!strcmp("-mpRho",argv[p])) {
			params_readFloatMinMax(argc,argv,++p,
					 "mpRho",&param_mpRho,
					 PARAM_HARDMIN_MPRHO,PARAM_HARDMAX_MPRHO);continue;
		}
		if (!strcmp("-mpSigma",argv[p])) {
			params_readFloatMinMax(argc,argv,++p,
					 "mpSigma",&param_mpSigma,
					 PARAM_HARDMIN_MPSIGMA,PARAM_HARDMAX_MPSIGMA);continue;
		}
		if (!strcmp("-mpPhi",argv[p])) {
			params_readFloatMinMax(argc,argv,++p,
					 "mpPhi",&param_mpPhi,
					 PARAM_HARDMIN_MPPHI,PARAM_HARDMAX_MPPHI);continue;
		}
		if (!strcmp("-mpUpsilon",argv[p])) {
			params_readFloatMinMax(argc,argv,++p,
					 "mpUpsilon",&param_mpUpsilon,
					 PARAM_HARDMIN_MPUPSILON,PARAM_HARDMAX_MPUPSILON);continue;
		}
		if (!strcmp("-mpUpdateRule",argv[p])) {
			params_readIntMinMax(argc,argv,++p,
					 "mpUpdateRule",&param_mpUpdateRule,
					 MP_UPDATERULE_MINVALUE,MP_UPDATERULE_MAXVALUE);continue;
		}

		//SLS
		if (!strcmp("-slsMaxFlipsA",argv[p])) {
			params_readFloatMinMax(argc,argv,++p,
					 "slsMaxFlipsA",&param_slsMaxFlipsA,
					 PARAM_HARDMIN_SLSMAXFLIPSA,PARAM_HARDMAX_SLSMAXFLIPSA);continue;
		}
		if (!strcmp("-slsMaxFlipsB",argv[p])) {
			params_readFloatMinMax(argc,argv,++p,
					 "slsMaxFlipsB",&param_slsMaxFlipsB,
					 PARAM_HARDMIN_SLSMAXFLIPSB,PARAM_HARDMAX_SLSMAXFLIPSB);continue;
		}
		if (!strcmp("-slsMaxTries",argv[p])){
			params_readIntMinMax(argc,argv,++p,
					 "slsMaxTries",&param_slsMaxTries,
					 PARAM_HARDMIN_SLSMAXTRIES,PARAM_HARDMAX_SLSMAXTRIES);continue;
		}
		if (!strcmp("-slsNoise",argv[p])) {
			params_readFloatMinMax(argc,argv,++p,
					 "slsNoise",&param_slsNoise,
					 PARAM_HARDMIN_SLSNOISE,PARAM_HARDMAX_SLSNOISE);continue;
		}
		if (!strcmp("-slsProbsatCB",argv[p])) {
			params_readFloatMinMax(argc,argv,++p,
					 "slsProbsatCB",&param_slsProbsatCB,
					 PARAM_HARDMIN_SLSPROBSATCB,PARAM_HARDMAX_SLSPROBSATCB);continue;
		}
		if (!strcmp("-slsProbsatCBShift",argv[p])) {
			params_readFloatMinMax(argc,argv,++p,
					 "slsProbsatCBShift",&param_slsProbsatCBShift,
					 PARAM_HARDMIN_SLSPROBSATCBSHIFT,PARAM_HARDMAX_SLSPROBSATCBSHIFT);continue;
		}

		if (!strcmp("-slsPickAndFlipVarRule",argv[p])) {
			params_readIntMinMax(argc,argv,++p,
					 "slsPickAndFlipVarRule",&param_slsPickAndFlipVarRule,
					 SLS_PICKANDFLIPVARRULE_MINVALUE,SLS_PICKANDFLIPVARRULE_MAXVALUE);continue;
		}
		if (!strcmp("-slsAssInitRule",argv[p])) {
			params_readIntMinMax(argc,argv,++p,
					 "slsAssInitRule",&param_slsAssInitRule,
					 SLS_ASSINITRULE_MINVALUE,SLS_ASSINITRULE_MAXVALUE);continue;
		}
		if (!strcmp("-slsTryRestartRule",argv[p])) {
			params_readIntMinMax(argc,argv,++p,
					 "slsTryRestartRule",&param_slsTryRestartRule,
					 SLS_TRYRESTARTRULE_MINVALUE,SLS_TRYRESTARTRULE_MAXVALUE);continue;
		}

		//CDCL
		if (!strcmp("-cdclOutput",argv[p])) 		{
			params_openCdclOutputFile(argc,argv,++p);continue;
		}
		if (!strcmp("-cdclRestartStagMaxFrac",argv[p])) {
			params_readFloatMinMax(argc,argv,++p,
					 "cdclRestartStagMaxFrac",&param_cdclRestartStagMaxFrac,
					 PARAM_HARDMIN_CDCLRESTARTSTAGMAXFRAC,PARAM_HARDMAX_CDCLRESTARTSTAGMAXFRAC);continue;
		}
		if (!strcmp("-cdclRestartInnerouterInit",argv[p])) {
			params_readIntMinMax(argc,argv,++p,
					 "cdclRestartInnerouterInit",&param_cdclRestartInnerouterInit,
					 PARAM_HARDMIN_CDCLRESTARTINNEROUTERINIT,PARAM_HARDMAX_CDCLRESTARTINNEROUTERINIT);continue;
		}
		if (!strcmp("-cdclRestartInnerouterInc",argv[p])) {
			params_readFloatMinMax(argc,argv,++p,
					 "cdclRestartInnerouterInc",&param_cdclRestartInnerouterInc,
					 PARAM_HARDMIN_CDCLRESTARTINNEROUTERINC,PARAM_HARDMAX_CDCLRESTARTINNEROUTERINC);continue;
		}
		if (!strcmp("-cdclRestartLubyUnit",argv[p])) {
			params_readIntMinMax(argc,argv,++p,
					 "cdclRestartLubyUnit",&param_cdclRestartLubyUnit,
					 PARAM_HARDMIN_CDCLRESTARTLUBYUNIT,PARAM_HARDMAX_CDCLRESTARTLUBYUNIT);continue;
		}
		if (!strcmp("-cdclRestartVarAgiIncBump",argv[p])) {
			params_readFloatMinMax(argc,argv,++p,
					 "cdclRestartVarAgiIncBump",&param_cdclRestartVarAgiIncBump,
					 PARAM_HARDMIN_CDCLRESTARTVARAGIINCBUMP,PARAM_HARDMAX_CDCLRESTARTVARAGIINCBUMP);continue;
		}
		if (!strcmp("-cdclRestartMaxAgiFrac",argv[p])) {
			params_readFloatMinMax(argc,argv,++p,
					 "cdclRestartMaxAgiFrac",&param_cdclRestartMaxAgiFrac,
					 PARAM_HARDMIN_CDCLRESTARTMAXAGIFRAC,PARAM_HARDMAX_CDCLRESTARTMAXAGIFRAC);continue;
		}
		if (!strcmp("-cdclRestartMaxAgiOutFrac",argv[p])) {
			params_readFloatMinMax(argc,argv,++p,
					 "cdclRestartMaxAgiOutFrac",&param_cdclRestartMaxAgiOutFrac,
					 PARAM_HARDMIN_CDCLRESTARTMAXAGIOUTFRAC,PARAM_HARDMAX_CDCLRESTARTMAXAGIOUTFRAC);continue;
		}
		if (!strcmp("-cdclSelectVarActIncBump",argv[p])) {
			params_readFloatMinMax(argc,argv,++p,
					 "cdclSelectVarActIncBump",&param_cdclSelectVarActIncBump,
					 PARAM_HARDMIN_CDCLSELECTVARACTINCBUMP,PARAM_HARDMAX_CDCLSELECTVARACTINCBUMP);continue;
		}
		if (!strcmp("-cdclSelectVarActInit",argv[p])) {
			params_readIntMinMax(argc,argv,++p,
					 "cdclSelectVarActInit",&param_cdclSelectVarActInit,
					 CDCL_SELECTVARRULE_VARACTINIT_MINVALUE,CDCL_SELECTVARRULE_VARACTINIT_MAXVALUE);continue;
		}
		if (!strcmp("-cdclSelectVarRandProb",argv[p])) {
			params_readFloatMinMax(argc,argv,++p,
					 "cdclSelectVarRandProb",&param_cdclSelectVarRandProb,
					 PARAM_HARDMIN_CDCLSELECTVARRANDPROB,PARAM_HARDMAX_CDCLSELECTVARRANDPROB);continue;
		}
		if (!strcmp("-cdclSelectDirMinFlipDist",argv[p])) {
			params_readIntMinMax(argc,argv,++p,
					 "cdclSelectDirMinFlipDist",&param_cdclSelectDirMinFlipDist,
					 PARAM_HARDMIN_CDCLSELECTDIRMINFLIPDIST,PARAM_HARDMAX_CDCLSELECTDIRMINFLIPDIST);continue;
		}
		if (!strcmp("-cdclSelectDirDirInit",argv[p])) {
			params_readIntMinMax(argc,argv,++p,
					 "cdclSelectDirDirInit",&param_cdclSelectDirDirInit,
					 CDCL_SELECTDIRRULE_DIRINIT_MINVALUE,CDCL_SELECTDIRRULE_DIRINIT_MAXVALUE);continue;
		}
		if (!strcmp("-cdclMaintenanceMinDev",argv[p])) {
			params_readFloatMinMax(argc,argv,++p,
					 "cdclMaintenanceMinDev",&param_cdclMaintenanceMinDev,
					 PARAM_HARDMIN_CDCLMAINTENANCEMINDEV,PARAM_HARDMAX_CDCLMAINTENANCEMINDEV);continue;
		}
		if (!strcmp("-cdclMaintenanceMaxAvg",argv[p])) {
			params_readFloatMinMax(argc,argv,++p,
					 "cdclMaintenanceMaxAvg",&param_cdclMaintenanceMaxAvg,
					 PARAM_HARDMIN_CDCLMAINTENANCEMAXAVG,PARAM_HARDMAX_CDCLMAINTENANCEMAXAVG);continue;
		}
		if (!strcmp("-cdclMaintenanceActIncBump",argv[p])) {
			params_readFloatMinMax(argc,argv,++p,
					 "cdclMaintenanceActIncBump",&param_cdclMaintenanceActIncBump,
					 PARAM_HARDMIN_CDCLMAINTENANCEACTINCBUMP,PARAM_HARDMAX_CDCLMAINTENANCEACTINCBUMP);continue;
		}
		if (!strcmp("-cdclMaintenanceInitial",argv[p])) {
			params_readFloatMinMax(argc,argv,++p,
					 "cdclMaintenanceInitial",&param_cdclMaintenanceInitial,
					 PARAM_HARDMIN_CDCLMAINTENANCEINITIAL,PARAM_HARDMAX_CDCLMAINTENANCEINITIAL);continue;
		}
		if (!strcmp("-cdclMaintenanceIncrease",argv[p])) {
			params_readFloatMinMax(argc,argv,++p,
					 "cdclMaintenanceIncrease",&param_cdclMaintenanceIncrease,
					 PARAM_HARDMIN_CDCLMAINTENANCEINCREASE,PARAM_HARDMAX_CDCLMAINTENANCEINCREASE);continue;
		}
		if (!strcmp("-cdclMaintenanceBase",argv[p])) {
			params_readIntMinMax(argc,argv,++p,
					 "cdclMaintenanceBase",&param_cdclMaintenanceBase,
					 PARAM_HARDMIN_CDCLMAINTENANCEBASE,PARAM_HARDMAX_CDCLMAINTENANCEBASE);continue;
		}
		if (!strcmp("-cdclMaintenanceBoost",argv[p])) {
			params_readIntMinMax(argc,argv,++p,
					 "cdclMaintenanceBoost",&param_cdclMaintenanceBoost,
					 PARAM_HARDMIN_CDCLMAINTENANCEBOOST,PARAM_HARDMAX_CDCLMAINTENANCEBOOST);continue;
		}
		if (!strcmp("-cdclIRUNHRounds",argv[p])) {
			params_readIntMinMax(argc,argv,++p,
					 "cdclIRUNHRounds",&param_cdclIRUNHRounds,
					 PARAM_HARDMIN_CDCLIRUNHROUNDS,PARAM_HARDMAX_CDCLIRUNHROUNDS);continue;
		}
		if (!strcmp("-cdclIRTERNARYLitMax",argv[p])) {
			params_readIntMinMax(argc,argv,++p,
					 "cdclIRTERNARYLitMax",&param_cdclIRTERNARYLitMax,
					 PARAM_HARDMIN_CDCLIRTERNARYLITMAX,PARAM_HARDMAX_CDCLIRTERNARYLITMAX);continue;
		}
		if (!strcmp("-cdclIRTERNARYMax",argv[p])) {
			params_readIntMinMax(argc,argv,++p,
					 "cdclIRTERNARYMax",&param_cdclIRTERNARYMax,
					 PARAM_HARDMIN_CDCLIRTERNARYMAX,PARAM_HARDMAX_CDCLIRTERNARYMAX);continue;
		}
		if (!strcmp("-cdclIRNIVERMax",argv[p])) {
			params_readIntMinMax(argc,argv,++p,
					 "cdclIRNIVERMax",&param_cdclIRNIVERMax,
					 PARAM_HARDMIN_CDCLIRNIVERMAX,PARAM_HARDMAX_CDCLIRNIVERMAX);continue;
		}
		if (!strcmp("-cdclIRGEMaxAONN",argv[p])) {
			params_readIntMinMax(argc,argv,++p,
					 "cdclIRGEMaxAONN",&param_cdclIRGEMaxAONN,
					 PARAM_HARDMIN_CDCLIRGEMAXAONN,PARAM_HARDMAX_CDCLIRGEMAXAONN);continue;
		}
		if (!strcmp("-cdclIRGEMaxXOR",argv[p])) {
			params_readIntMinMax(argc,argv,++p,
					 "cdclIRGEMaxXOR",&param_cdclIRGEMaxXOR,
					 PARAM_HARDMIN_CDCLIRGEMAXXOR,PARAM_HARDMAX_CDCLIRGEMAXXOR);continue;
		}
		if (!strcmp("-cdclIRStrMaxSize",argv[p])) {
			params_readIntMinMax(argc,argv,++p,
					 "cdclIRStrMaxSize",&param_cdclIRStrMaxSize,
					 PARAM_HARDMIN_CDCLIRSTRMAXSIZE,PARAM_HARDMAX_CDCLIRSTRMAXSIZE);continue;
		}
		if (!strcmp("-cdclIRNHBRMin",argv[p])) {
			params_readIntMinMax(argc,argv,++p,
					 "cdclIRNHBRMin",&param_cdclIRNHBRMin,
					 PARAM_HARDMIN_CDCLIRNHBRMIN,PARAM_HARDMAX_CDCLIRNHBRMIN);continue;
		}
		if (!strcmp("-cdclIRMinConfDistance",argv[p])) {
			params_readFloatMinMax(argc,argv,++p,
					 "cdclIRMinConfDistance",&param_cdclIRMinConfDistance,
					 PARAM_HARDMIN_CDCLIRMINCONFDISTANCE,PARAM_HARDMAX_CDCLIRMINCONFDISTANCE);continue;
		}
		if (!strcmp("-cdclSelectVarRule",argv[p])) {
			params_readIntMinMax(argc,argv,++p,
					 "cdclSelectVarRule",&param_cdclSelectVarRule,
					 CDCL_SELECTVARRULE_MINVALUE,CDCL_SELECTVARRULE_MAXVALUE);continue;
		}
		if (!strcmp("-cdclSelectDirRule",argv[p])) {
			params_readIntMinMax(argc,argv,++p,
					 "cdclSelectDirRule",&param_cdclSelectDirRule,
					 CDCL_SELECTDIRRULE_MINVALUE,CDCL_SELECTDIRRULE_MAXVALUE);continue;
		}
		if (!strcmp("-cdclConflictAnalysisRule",argv[p])) {
			params_readIntMinMax(argc,argv,++p,
					 "cdclConflictAnalysisRule",&param_cdclConflictAnalysisRule,
					 CDCL_CONFLICTANALYSISRULE_MINVALUE,CDCL_CONFLICTANALYSISRULE_MAXVALUE);continue;
		}
		if (!strcmp("-cdclStrLearnedRule",argv[p])) {
			params_readIntMinMax(argc,argv,++p,
					 "cdclStrLearnedRule",&param_cdclStrLearnedRule,
					 CDCL_STRLEARNEDRULE_MINVALUE,CDCL_STRLEARNEDRULE_MAXVALUE);continue;
		}
		if (!strcmp("-cdclStrOtherRule",argv[p])) {
			params_readIntMinMax(argc,argv,++p,
					 "cdclStrOtherRule",&param_cdclStrOtherRule,
					 CDCL_STROTHERRULE_MINVALUE,CDCL_STROTHERRULE_MAXVALUE);continue;
		}
		if (!strcmp("-cdclComputeBJLRule",argv[p])) {
			params_readIntMinMax(argc,argv,++p,
					 "cdclComputeBJLRule",&param_cdclComputeBJLRule,
					 CDCL_COMPUTEBJLRULE_MINVALUE,CDCL_COMPUTEBJLRULE_MAXVALUE);continue;
		}
		if (!strcmp("-cdclRestartRule",argv[p])) {
			params_readIntMinMax(argc,argv,++p,
					 "cdclRestartRule",&param_cdclRestartRule,
					 CDCL_RESTARTRULE_MINVALUE,CDCL_RESTARTRULE_MAXVALUE);continue;
		}
		if (!strcmp("-cdclMaintenanceRule",argv[p])) {
			params_readIntMinMax(argc,argv,++p,
					 "cdclMaintenanceRule",&param_cdclMaintenanceRule,
					 CDCL_MAINTENANCERULE_MINVALUE,CDCL_MAINTENANCERULE_MAXVALUE);continue;
		}
		if (!strcmp("-cdclInprocessingRule",argv[p])) {
			params_readIntMinMax(argc,argv,++p,
					 "cdclInprocessingRule",&param_cdclInprocessingRule,
					 CDCL_INPROCESSINGRULE_MINVALUE,CDCL_INPROCESSINGRULE_MAXVALUE);continue;
		}
		if (!strcmp("-cdclAbortRule",argv[p])) {
			params_readIntMinMax(argc,argv,++p,
					 "cdclAbortRule",&param_cdclAbortRule,
					 CDCL_ABORTRULE_MINVALUE,CDCL_ABORTRULE_MAXVALUE);continue;
		}
		//HANDLERESULT
		if (!strcmp("-handleresultCoarsen",argv[p])) {
			params_readIntMinMax(argc,argv,++p,
					 "handleresultCoarsen",&param_handleresultCoarsen,
					 PARAM_HARDMIN_HANDLERESULTCOARSEN,PARAM_HARDMAX_HANDLERESULTCOARSEN);continue;
		}

		//SOLWRITER
		if (!strcmp("-solwriterPrintAssignment",argv[p])){
			params_readIntMinMax(argc,argv,++p,
					"solwriterPrintAssignment",&param_solwriterPrintAssignment,0,1);continue;
		}

		//SHUTDOWN

		//Otherwise we read something we do not want. Call for error.
		printf("c ERROR. Parameter %s unrecognized.\n", argv[p]);
		params_returnCode = PARAMS_BADPARAM;
		break;
	}

	//CHECK RESULT
	//Check if we read all mandatory parameters.
	if (params_returnCode == PARAMS_UNKNOWN){
		#ifndef COMPILEASLIBRARY
		if (param_formula == NULL){
			printf("c ERROR. The mandatory parameter -formula is missing. Call ./dimetheus -h for help.\n");
			params_returnCode = PARAMS_BADPARAM;
		}
		#endif
	}
	//In case something went wrong, we output the parameter help.
	if (params_returnCode == PARAMS_BADPARAM) {
		printf("c Use ./dimetheus -h for help.\n");
		return;
	}

	#ifdef VERBOSE_PARAMS
	//PRINT PARAMETER SETTING.
	if (params_returnCode == PARAMS_UNKNOWN){
		params_extern_printAll("c   PARAMS:");
	}
	#endif
	#ifdef COLLECTSTATS
	stats_params_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}

void params_extern_printAll(char* prefix){
	printf("%s Current parameter settings...\n", prefix);
	//Main algorithm related.
	printf("%s Main:\n", prefix);
	printf("%s   -formula is set\n", prefix);
	printf("%s   -seed %d\n", prefix, param_seed);
	printf("%s PHASES:\n", prefix);
	//Reset related.
	printf("%s   Reset:\n", prefix);
	printf("%s     (nothing yet)\n", prefix);
	//Parameters related.
	printf("%s   Params:\n", prefix);
	printf("%s     (nothing yet)\n", prefix);
	//Guidance related.
	printf("%s   Guidance:\n", prefix);
	printf("%s     -guide %d (", prefix, param_guide); guidance_extern_printNameOfGuide(param_guide); printf(")\n");
	//Startup related.
	printf("%s   Startup:\n", prefix);
	if (param_startupOutput == NULL) {
		printf("%s     -startupOutput is UNSET\n", prefix);
	} else {
		printf("%s     -startupOutput is set\n", prefix);
	}
	printf("%s     -startupInputFormat %d (", prefix, param_startupInputFormat); startup_extern_printInputFormatName(param_startupInputFormat); printf(")\n");
	//Classification related.
	printf("%s   Classify:\n", prefix);
	printf("%s     -classifyInputDomain %d (", prefix, param_classifyInputDomain); classify_extern_printInputDomainName(param_classifyInputDomain); printf(")\n");
	printf("%s     -classifyAttSetTime %f\n", prefix, param_classifyAttSetTime);
	printf("%s     -classifyFloatPrec %d\n", prefix, param_classifyFloatPrec);
	printf("%s     -classifyMaxVGSize %d\n", prefix, param_classifyMaxVGSize);
	printf("%s     -classifyMaxCGSize %d\n", prefix, param_classifyMaxCGSize);
	printf("%s     -classifyMaxUPImps %d\n", prefix, param_classifyMaxUPImps);
	printf("%s     -classifyMaxUPn %d\n", prefix, param_classifyMaxUPn);
	printf("%s     -classifyPGFA %f\n", prefix, param_classifyPGFA);
	printf("%s     -classifyPGFB %f\n", prefix, param_classifyPGFB);
	printf("%s     -classifyPGFC %f\n", prefix, param_classifyPGFC);
	printf("%s     -classifySimplify %d\n", prefix, param_classifySimplify);
	printf("%s     -classifyStrategy %d\n", prefix, param_classifyStrategy);
	//Classification related.
	printf("%s   Adapt:\n", prefix);
	printf("%s     -adaptStrategy %d\n", prefix, param_adaptStrategy);
	printf("%s     -adaptPrepStrategy %d\n", prefix, param_adaptPrepStrategy);
	printf("%s     -adaptPrepStrategyParams %d\n", prefix, param_adaptPrepStrategyParams);
	printf("%s     -adaptInpStrategy %d\n", prefix, param_adaptInpStrategy);
	printf("%s     -adaptInpStrategyParams %d\n", prefix, param_adaptInpStrategyParams);
	printf("%s     -adaptSearchStrategy %d\n", prefix, param_adaptSearchStrategy);
	printf("%s     -adaptSearchStrategyParams %d\n", prefix, param_adaptSearchStrategyParams);
	printf("%s     -adaptMpUR %d\n", prefix, param_adaptMpUR);
	printf("%s     -adaptMpURParams %d\n", prefix, param_adaptMpURParams);
	printf("%s     -adaptSlsPAFVR %d\n", prefix, param_adaptSlsPAFVR);
	printf("%s     -adaptSlsPAFVRParams %d\n", prefix, param_adaptSlsPAFVRParams);
	printf("%s     -adaptSlsAIR %d\n", prefix, param_adaptSlsAIR);
	printf("%s     -adaptSlsAIRParams %d\n", prefix, param_adaptSlsAIRParams);
	printf("%s     -adaptSlsTRR %d\n", prefix, param_adaptSlsTRR);
	printf("%s     -adaptSlsTRRParams %d\n", prefix, param_adaptSlsTRRParams);
	printf("%s     -adaptCdclSVR %d\n", prefix, param_adaptCdclSVR);
	printf("%s     -adaptCdclSVRParams %d\n", prefix, param_adaptCdclSVRParams);
	printf("%s     -adaptCdclSVAI %d\n", prefix, param_adaptCdclSVAI);
	printf("%s     -adaptCdclSVAIParams %d\n", prefix, param_adaptCdclSVAIParams);
	printf("%s     -adaptCdclSDR %d\n", prefix, param_adaptCdclSDR);
	printf("%s     -adaptCdclSDRParams %d\n", prefix, param_adaptCdclSDRParams);
	printf("%s     -adaptCdclSDDI %d\n", prefix, param_adaptCdclSDDI);
	printf("%s     -adaptCdclSDDIParams %d\n", prefix, param_adaptCdclSDDIParams);
	printf("%s     -adaptCdclCAR %d\n", prefix, param_adaptCdclCAR);
	printf("%s     -adaptCdclCARParams %d\n", prefix, param_adaptCdclCARParams);
	printf("%s     -adaptCdclSLR %d\n", prefix, param_adaptCdclSLR);
	printf("%s     -adaptCdclSLRParams %d\n", prefix, param_adaptCdclSLRParams);
	printf("%s     -adaptCdclSOR %d\n", prefix, param_adaptCdclSOR);
	printf("%s     -adaptCdclSORParams %d\n", prefix, param_adaptCdclSORParams);
	printf("%s     -adaptCdclCBJLR %d\n", prefix, param_adaptCdclCBJLR);
	printf("%s     -adaptCdclCBJLRParams %d\n", prefix, param_adaptCdclCBJLRParams);
	printf("%s     -adaptCdclRR %d\n", prefix, param_adaptCdclRR);
	printf("%s     -adaptCdclRRParams %d\n", prefix, param_adaptCdclRRParams);
	printf("%s     -adaptCdclMR %d\n", prefix, param_adaptCdclMR);
	printf("%s     -adaptCdclMRParams %d\n", prefix, param_adaptCdclMRParams);
	printf("%s     -adaptCdclIR %d\n", prefix, param_adaptCdclIR);
	printf("%s     -adaptCdclIRParams %d\n", prefix, param_adaptCdclIRParams);
	printf("%s     -adaptCdclAR %d\n", prefix, param_adaptCdclAR);
	printf("%s     -adaptCdclARParams %d\n", prefix, param_adaptCdclARParams);
	//Preprocessor related.
	printf("%s   Preprocessor:\n", prefix);
	if (param_prepOutput == NULL) {
		printf("%s     -prepOutput is UNSET\n", prefix);
	} else {
		printf("%s     -prepOutput is set\n", prefix);
	}
	printf("%s     -prepReviveCls %d\n", prefix, param_prepReviveCls);
	printf("%s     -prepUNHRounds %d\n", prefix, param_prepUNHRounds);
	printf("%s     -prepACCEMax %d\n", prefix, param_prepACCEMax);
	printf("%s     -prepACCEMinClsSize %d\n", prefix, param_prepACCEMinClsSize);
	printf("%s     -prepACCESaturate %d\n", prefix, param_prepACCESaturate);
	printf("%s     -prepNIVERMax %d\n", prefix, param_prepNIVERMax);
	printf("%s     -prepATEMin %d\n", prefix, param_prepATEMin);
	printf("%s     -prepGEMaxAONN %d\n", prefix, param_prepGEMaxAONN);
	printf("%s     -prepGEMaxXOR %d\n", prefix, param_prepGEMaxXOR);
	printf("%s     -prepTERNARYLitMax %d\n", prefix, param_prepTERNARYLitMax);
	printf("%s     -prepTERNARYMax %d\n", prefix, param_prepTERNARYMax);
	printf("%s     -prepNHBRMin %d\n", prefix, param_prepNHBRMin);
	printf("%s     -prepStrategy %d\n", prefix, param_prepStrategy);
	//In-processor related.
	printf("%s   In-processor:\n", prefix);
	printf("%s     -inpStrMaxSize %d\n", prefix, param_inpStrMaxSize);
	printf("%s     -inpTernaryLitMax %d\n", prefix, param_inpTernaryLitMax);
	printf("%s     -inpTernaryMax %d\n", prefix, param_inpTernaryMax);
	printf("%s     -inpNiverMax %d\n", prefix, param_inpNiverMax);
	printf("%s     -inpGEMaxAONN %d\n", prefix, param_inpGEMaxAONN);
	printf("%s     -inpGEMaxXOR %d\n", prefix, param_inpGEMaxXOR);
	printf("%s     -inpNHBRMin %d\n", prefix, param_inpNHBRMin);
	printf("%s     -inpUNHRounds %d\n", prefix, param_inpUNHRounds);
	printf("%s     -inpStrategy %d\n", prefix, param_inpStrategy);
	//Search related.
	printf("%s   Search:\n", prefix);
	printf("%s     -searchMIDBlockSize %f\n", prefix, param_searchMIDBlockSize);
	printf("%s     -searchRetreatFact %f\n", prefix, param_searchRetreatFact);
	printf("%s     -searchMinClsImpact %f\n", prefix, param_searchMinClsImpact);
	printf("%s     -searchImpactMaxFactor %f\n", prefix, param_searchImpactMaxFactor);
	printf("%s     -searchMaxMagForSLS %f\n", prefix, param_searchMaxMagForSLS);
	printf("%s     -searchMPWeightModSAT %f\n", prefix, param_searchMPWeightModSAT);
	printf("%s     -searchMPWeightModUNS %f\n", prefix, param_searchMPWeightModUNS);
	printf("%s     -searchStrategy %d\n", prefix, param_searchStrategy);
	//Handle result related.
	printf("%s   Handle result:\n", prefix);
	printf("%s     -handleresultCoarsen %d\n", prefix, param_handleresultCoarsen);
	//Shutdown related.
	printf("%s   Shutdown:\n", prefix);
	printf("%s     (nothing yet)\n", prefix);
	printf("%s MODULES:\n", prefix);
	//DETERM
	printf("%s   DETERM:\n", prefix);
	printf("%s     (nothing yet)\n", prefix);
	//SPECCER
	printf("%s   SPECCER:\n", prefix);
	printf("%s     -speccerNNFFirst %d\n", prefix, param_speccerNNFFirst);
	//LOADER
	printf("%s   LOADER:\n", prefix);
	printf("%s     (nothing yet)\n", prefix);
	//ATTRIBUTOR
	printf("%s   ATTRIBUTOR:\n", prefix);
	printf("%s     (nothing yet)\n", prefix);
	//RANDOMFOREST
	printf("%s   RANDOMFOREST:\n", prefix);
	printf("%s     (nothing yet)\n", prefix);
	//ITEADAPTER
	printf("%s   ITEADAPTER:\n", prefix);
	printf("%s     (nothing yet)\n", prefix);
	//PRE
	printf("%s   PRE:\n", prefix);
	printf("%s     (nothing yet)\n", prefix);
	//SATELITE
	printf("%s   SATELITE:\n", prefix);
	printf("%s     (nothing yet)\n", prefix);
	//IN
	printf("%s   IN:\n", prefix);
	printf("%s     (nothing yet)\n", prefix);
	//MP
	printf("%s   MP:\n", prefix);
	printf("%s     -mpMaxNumIterations %d\n", prefix, param_mpMaxNumIterations);
	printf("%s     -mpMaxNonImpIterations %d\n", prefix, param_mpMaxNonImpIterations);
	printf("%s     -mpMaxConvergenceDiff %f\n", prefix,param_mpMaxConvergenceDiff);
	printf("%s     -mpMaxWeightNoise %f\n", prefix,param_mpWeightNoise);
	printf("%s     -mpMaxMagnetization %f\n", prefix, param_mpMaxMagnetization);
	printf("%s     -mpEpsilon %.12f\n", prefix, param_mpEpsilon);
	printf("%s     -mpRho %f\n", prefix, param_mpRho);
	printf("%s     -mpSigma %f\n", prefix, param_mpSigma);
	printf("%s     -mpPhi %f\n", prefix, param_mpPhi);
	printf("%s     -mpPhi %f\n", prefix, param_mpUpsilon);
	printf("%s     -mpUpdateRule %d\n", prefix, param_mpUpdateRule);
	//SLS
	printf("%s   SLS:\n", prefix);
	printf("%s     -slsMaxFlipsA %f\n", prefix, param_slsMaxFlipsA);
	printf("%s     -slsMaxFlipsB %f\n", prefix, param_slsMaxFlipsB);
	printf("%s     -slsMaxTries %d\n", prefix, param_slsMaxTries);
	printf("%s     -slsNoise %f\n", prefix, param_slsNoise);
	printf("%s     -slsProbsatCB %f\n", prefix, param_slsProbsatCB);
	printf("%s     -slsProbsatCBShift %f\n", prefix, param_slsProbsatCBShift);
	printf("%s     -slsPickAndFlipVarRule %d\n", prefix, param_slsPickAndFlipVarRule);
	printf("%s     -slsAssInitRule %d\n", prefix, param_slsAssInitRule);
	printf("%s     -slsTryRestartRule %d\n", prefix, param_slsTryRestartRule);
	//CDCL
	printf("%s   CDCL:\n", prefix);
	if (param_cdclOutput == NULL) {
		printf("%s     -cdclOutput is UNSET\n", prefix);
	} else {
		printf("%s     -cdclOutput is set\n", prefix);
	}
	printf("%s     -cdclRestartStagMaxFrac %f\n", prefix, param_cdclRestartStagMaxFrac);
	printf("%s     -cdclRestartInnerouterInit %d\n", prefix, param_cdclRestartInnerouterInit);
	printf("%s     -cdclRestartInnerouterInc %f\n", prefix, param_cdclRestartInnerouterInc);
	printf("%s     -cdclRestartLubyUnit %d\n", prefix, param_cdclRestartLubyUnit);
	printf("%s     -cdclRestartVarAgiIncBump %f\n", prefix, param_cdclRestartVarAgiIncBump);
	printf("%s     -cdclRestartMaxAgiFrac %f\n", prefix, param_cdclRestartMaxAgiFrac);
	printf("%s     -cdclRestartMaxAgiOutFrac %f\n", prefix, param_cdclRestartMaxAgiOutFrac);
	printf("%s     -cdclSelectVarActIncBump %f\n", prefix, param_cdclSelectVarActIncBump);
	printf("%s     -cdclSelectVarActInit %d\n", prefix, param_cdclSelectVarActInit);
	printf("%s     -cdclSelectVarRandProb %f\n", prefix, param_cdclSelectVarRandProb);
	printf("%s     -cdclSelectDirDirInit %d\n", prefix, param_cdclSelectDirDirInit);
	printf("%s     -cdclSelectDirMinFlipDist %d\n", prefix, param_cdclSelectDirMinFlipDist);
	printf("%s     -cdclMaintenanceMinDev %f\n", prefix, param_cdclMaintenanceMinDev);
	printf("%s     -cdclMaintenanceMaxAvg %f\n", prefix, param_cdclMaintenanceMaxAvg);
	printf("%s     -cdclMaintenanceActIncBump %f\n", prefix, param_cdclMaintenanceActIncBump);
	printf("%s     -cdclMaintenanceInitial %f\n", prefix, param_cdclMaintenanceInitial);
	printf("%s     -cdclMaintenanceIncrease %f\n", prefix, param_cdclMaintenanceIncrease);
	printf("%s     -cdclMaintenanceBase %d\n", prefix, param_cdclMaintenanceBase);
	printf("%s     -cdclMaintenanceBoost %d\n", prefix, param_cdclMaintenanceBoost);
	printf("%s     -cdclIRUNHRounds %d\n", prefix, param_cdclIRUNHRounds);
	printf("%s     -cdclIRTERNARYLitMax %d\n", prefix, param_cdclIRTERNARYLitMax);
	printf("%s     -cdclIRTERNARYMax %d\n", prefix, param_cdclIRTERNARYMax);
	printf("%s     -cdclIRNIVERMax %d\n", prefix, param_cdclIRNIVERMax);
	printf("%s     -cdclIRGEMaxAONN %d\n", prefix, param_cdclIRGEMaxAONN);
	printf("%s     -cdclIRGEMaxXOR %d\n", prefix, param_cdclIRGEMaxXOR);
	printf("%s     -cdclIRStrMaxSize %d\n", prefix, param_cdclIRStrMaxSize);
	printf("%s     -cdclIRNHBRMin %d\n", prefix, param_cdclIRNHBRMin);
	printf("%s     -cdclIRMinConfDistance %f\n", prefix, param_cdclIRMinConfDistance);
	printf("%s     -cdclSelectVarRule %d\n", prefix, param_cdclSelectVarRule);
	printf("%s     -cdclSelectDirRule %d\n", prefix, param_cdclSelectDirRule);
	printf("%s     -cdclConflictAnalysisRule %d\n", prefix, param_cdclConflictAnalysisRule);
	printf("%s     -cdclStrLearnedRule %d\n", prefix, param_cdclStrLearnedRule);
	printf("%s     -cdclStrOtherRule %d\n", prefix, param_cdclStrOtherRule);
	printf("%s     -cdclComputeBJLRule %d\n", prefix, param_cdclComputeBJLRule);
	printf("%s     -cdclRestartRule %d\n", prefix, param_cdclRestartRule);
	printf("%s     -cdclMaintenanceRule %d\n", prefix, param_cdclMaintenanceRule);
	printf("%s     -cdclInprocessingRule %d\n", prefix, param_cdclInprocessingRule);
	printf("%s     -cdclAbortRule %d\n", prefix, param_cdclAbortRule);
	printf("%s   SOLWRITER:\n", prefix);
	printf("%s     -solwriterPrintAssignment %d\n", prefix, param_solwriterPrintAssignment);
}

void params_extern_printAdaptableWithPreconditions(char* prefix){
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_params_component_totalCalls;
	#endif
	/*RESET PARAMETERS*/
	//None.

	/*PARAMETERS PARAMETERS*/
	//None.

	/*STARTUP PARAMETERS*/
	//None.

	/*PREPROCESSING PARAMETERS*/
	printf("%s adaptPrepStrategy "
			"prepStrategy\n", prefix);
	printf("%s adaptPrepStrategyParams "
			"prepReviveCls "
			"prepUNHRounds "
			"prepACCEMax "
			"prepACCEMinClsSize "
			"prepACCESaturate "
			"prepNIVERMax "
			"prepATEMin "
			"prepGEMaxAONN "
			"prepGEMaxXOR "
			"prepTERNARYLitMax "
			"prepTERNARYMax "
			"prepNHBRMin\n", prefix);

	/*INPROCESSING PARAMETERS*/
	printf("%s adaptInpStrategy "
			"inpStrategy\n", prefix);
	printf("%s adaptInpStrategyParams "
			"inpStrMaxSize "
			"inpTernaryLitMax "
			"inpTernaryMax "
			"inpNiverMax "
			"inpGEMaxAONN "
			"inpGEMaxXOR "
			"inpNHBRMin "
			"inpUNHRounds\n", prefix);

	/*SEARCH PARAMETERS*/
	printf("%s adaptSearchStrategy "
			"searchStrategy\n", prefix);
	printf("%s adaptSearchStrategyParams "
			"searchMIDBlockSize "
			"searchRetreatFact "
			"searchMinClsImpact "
			"searchImpactMaxFactor "
			"searchMaxMagForSLS "
			"searchMPWeightModSAT "
			"searchMPWeightModUNS\n", prefix);

	/*DETERM PARAMETERS*/
	//None.

	/*SPECCER PARAMETERS*/
	//None.

	/*LOADER PARAMETERS*/
	//None.

	/*ATTRIBUTOR PARAMETERS*/
	//None.

	/*RANDOMFOREST PARAMETERS*/
	//None.

	/*ITEADAPTER PARAMETERS*/
	//None.

	/*PRE PARAMETERS*/
	//None.

	/*SATELITE PARAMETERS*/
	//None.

	/*IN PARAMETERS*/
	//None.

	/*MP PARAMETERS*/
	printf("%s adaptMpUR "
			"mpUpdateRule\n", prefix);
	printf("%s adaptMpURParams "
			"mpRho "
			"mpSigma "
			"mpPhi "
			"mpUpsilon "
			"mpMaxConvergenceDiff\n", prefix);

	/*SLS PARAMETERS*/
	printf("%s adaptSlsPAFVR "
			"slsPickAndFlipVarRule\n", prefix);
	printf("%s adaptSlsPAFVRParams "
			"slsNoise "
			"slsProbsatCB "
			"slsProbsatCBShift\n", prefix);
	printf("%s adaptSlsAIR "
			"slsAssInitRule\n", prefix);
	//No parameters for AIR yet.

	printf("%s adaptSlsTRR "
			"slsTryRestartRule\n", prefix);
	printf("%s adaptSlsTRRParams "
			"slsMaxFlipsA "
			"slsMaxFlipsB "
			"slsMaxTries\n", prefix);

	/*CDCL PARAMETERS*/
	printf("%s adaptCdclRR "
			"cdclRestartRule\n", prefix);
	printf("%s adaptCdclRRParams "
			"cdclRestartStagMaxFrac "
			"cdclRestartInnerouterInit "
			"cdclRestartInnerouterInc "
			"cdclRestartLubyUnit "
			"cdclRestartVarAgiIncBump "
			"cdclRestartMaxAgiFrac "
			"cdclRestartMaxAgiOutFrac\n", prefix);

	guidance_cdclSelectVarActIncBump = -1;
	guidance_cdclSelectVarRandProb = -1;

	printf("%s adaptCdclSVR "
			"cdclSelectVarRule\n", prefix);
	printf("%s adaptCdclSVRParams "
			"cdclSelectVarActIncBump "
			"cdclSelectVarRandProb\n", prefix);

	printf("%s adaptCdclSVAI "
			"cdclSelectVarActInit\n", prefix);
	//No parameters for SVAI yet.

	printf("%s adaptCdclSDR "
			"cdclSelectDirRule\n", prefix);
	printf("%s adaptCdclSDRParams "
			"cdclSelectDirMinFlipDist\n", prefix);

	printf("%s adaptCdclSDDI "
			"cdclSelectDirDirInit\n", prefix);
	//No parameters for SDDI yet.

	printf("%s adaptCdclCAR "
			"cdclConflictAnalysisRule\n", prefix);
	//No parameters for CAR yet.

	printf("%s adaptCdclMR "
			"cdclMaintenanceRule\n", prefix);
	printf("%s adaptCdclMRParams "
			"cdclMaintenanceMinDev "
			"cdclMaintenanceMaxAvg "
			"cdclMaintenanceActIncBump "
			"cdclMaintenanceInitial "
			"cdclMaintenanceIncrease "
			"cdclMaintenanceBase "
			"cdclMaintenanceBoost\n", prefix);

	printf("%s adaptCdclIR "
			"cdclInprocessingRule\n", prefix);
	printf("%s adaptCdclIRParams "
			"cdclIRUNHRounds "
			"cdclIRTERNARYLitMax "
			"cdclIRTERNARYMax "
			"cdclIRNIVERMax "
			"cdclIRGEMaxAONN "
			"cdclIRGEMaxXOR "
			"cdclIRStrMaxSize "
			"cdclIRNHBRMin "
			"cdclIRMinConfDistance\n", prefix);

	printf("%s adaptCdclSLR "
			"cdclStrLearnedRule\n", prefix);
	//No parameters for SLR yet.

	printf("%s adaptCdclSOR "
			"cdclStrOtherRule\n", prefix);
	//No parameters for SOR yet.

	printf("%s adaptCdclCBJLR "
			"cdclComputeBJLRule\n", prefix);
	//No parameters for CBJLR yet.

	printf("%s adaptCdclAR "
			"cdclAbortRule\n", prefix);
	//No parameters for AR yet.

	/*HANDLERESULT PARAMETERS*/
	//None.

	//SOLWRITER
	//None.

	/*SHUTDOWN PARAMETERS*/
	//None.

	#ifdef COLLECTSTATS
	stats_params_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}

