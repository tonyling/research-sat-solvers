OBJ = $(SRCPATH)/dimetheus.o \
	$(SRCPATH)/params/params.o \
	$(SRCPATH)/params/_paramGraph/paramgraph.o \
	$(SRCPATH)/params/_paramGraph/parametergraph_mp.o \
	$(SRCPATH)/params/_paramGraph/parametergraph_sls.o \
	$(SRCPATH)/params/_paramGraph/parametergraph_cdcl.o \
	$(SRCPATH)/params/_fuzzing/paramfuzzing.o \
	$(SRCPATH)/_interface/dimetheus_ifc.o \
	$(SRCPATH)/_datastructures/variables.o \
	$(SRCPATH)/_datastructures/literals.o \
	$(SRCPATH)/_datastructures/clauses.o \
	$(SRCPATH)/_datastructures/formula.o \
	$(SRCPATH)/_tools/rand.o \
	$(SRCPATH)/_tools/tools.o \
	$(SRCPATH)/_tools/cQueue.o \
	$(SRCPATH)/_tools/lQueue.o \
	$(SRCPATH)/_tools/vQueue.o \
	$(SRCPATH)/_tools/simpleUP.o \
	$(SRCPATH)/_tools/complexUP.o \
	$(SRCPATH)/_tools/decStack.o \
	$(SRCPATH)/_tools/perm.o \
	$(SRCPATH)/_tools/stats.o \
	$(SRCPATH)/_tools/scoring.o \
	$(SRCPATH)/_tools/aplefoo.o \
	$(SRCPATH)/reset/reset.o \
	$(SRCPATH)/guidance/guidance.o \
	$(SRCPATH)/guidance/_guides/g_manual.o \
	$(SRCPATH)/guidance/_guides/g_autoadapt.o \
	$(SRCPATH)/guidance/_guides/g_attonly.o \
	$(SRCPATH)/guidance/_guides/g_randforonly.o \
	$(SRCPATH)/guidance/_guides/g_preponly.o \
	$(SRCPATH)/guidance/_guides/g_cdcl.o \
	$(SRCPATH)/guidance/_guides/g_sls.o \
	$(SRCPATH)/guidance/_guides/g_pmpcdcl.o \
	$(SRCPATH)/guidance/_guides/g_pmpsls.o \
	$(SRCPATH)/guidance/_guides/g_hybridmsc.o \
	$(SRCPATH)/guidance/_guides/g_emp.o \
	$(SRCPATH)/guidance/_guides/g_testing.o \
	$(SRCPATH)/startup/startup.o \
	$(SRCPATH)/startup/DETERM/_DIMACSCNF/det_dimacscnf.o \
	$(SRCPATH)/startup/DETERM/_APLEFOO/det_aplefoo.o \
	$(SRCPATH)/startup/DETERM/determ.o \
	$(SRCPATH)/startup/SPECCER/_DIMACSCNF/spec_dimacscnf.o \
	$(SRCPATH)/startup/SPECCER/_APLEFOO/spec_aplefoo.o \
	$(SRCPATH)/startup/SPECCER/speccer.o \
	$(SRCPATH)/startup/LOADER/_DIMACSCNF/load_dimacscnf.o \
	$(SRCPATH)/startup/LOADER/_APLEFOO/load_aplefoo.o \
	$(SRCPATH)/startup/LOADER/loader.o \
	$(SRCPATH)/classify/classify.o \
	$(SRCPATH)/classify/_classifyStrategies/classifyStrategies.o \
	$(SRCPATH)/classify/_classifyStrategies/cs_null.o \
	$(SRCPATH)/classify/_classifyStrategies/cs_attonly.o \
	$(SRCPATH)/classify/_classifyStrategies/cs_randfor.o \
	$(SRCPATH)/classify/_classifyStrategies/cs_testing.o \
	$(SRCPATH)/classify/ATTRIBUTOR/attributor.o \
	$(SRCPATH)/classify/RANDOMFOREST/randomforest_random.o \
	$(SRCPATH)/classify/RANDOMFOREST/randomforest_crafted.o \
	$(SRCPATH)/classify/RANDOMFOREST/randomforest_application.o \
	$(SRCPATH)/classify/RANDOMFOREST/randomforest_generic.o \
	$(SRCPATH)/classify/RANDOMFOREST/randomforest.o \
	$(SRCPATH)/adapt/adapt.o \
	$(SRCPATH)/adapt/_adaptStrategies/adaptStrategies.o \
	$(SRCPATH)/adapt/_adaptStrategies/as_null.o \
	$(SRCPATH)/adapt/_adaptStrategies/as_itegeneric.o \
	$(SRCPATH)/adapt/_adaptStrategies/as_testing.o \
	$(SRCPATH)/adapt/ITEADAPTER/iteadapter_generic.o \
	$(SRCPATH)/adapt/ITEADAPTER/iteadapter.o \
	$(SRCPATH)/prep/prep.o \
	$(SRCPATH)/prep/_prepStrategies/prepStrategies.o \
	$(SRCPATH)/prep/_prepStrategies/ps_null.o \
	$(SRCPATH)/prep/_prepStrategies/ps_simple.o \
	$(SRCPATH)/prep/_prepStrategies/ps_strong.o \
	$(SRCPATH)/prep/_prepStrategies/ps_full.o \
	$(SRCPATH)/prep/_prepStrategies/ps_rand.o \
	$(SRCPATH)/prep/_prepStrategies/ps_gigrid.o \
	$(SRCPATH)/prep/_prepStrategies/ps_satelite.o \
	$(SRCPATH)/prep/_prepStrategies/ps_testing.o \
	$(SRCPATH)/prep/PRE/pre.o \
	$(SRCPATH)/prep/SATELITE/satelite.o \
	$(SRCPATH)/search/search.o \
	$(SRCPATH)/search/_searchStrategies/searchStrategies.o \
	$(SRCPATH)/search/_searchStrategies/ss_null.o \
	$(SRCPATH)/search/_searchStrategies/ss_sls.o \
	$(SRCPATH)/search/_searchStrategies/ss_cdcl.o \
	$(SRCPATH)/search/_searchStrategies/ss_pmpcdcl.o \
	$(SRCPATH)/search/_searchStrategies/ss_pmpsls.o \
	$(SRCPATH)/search/_searchStrategies/ss_hybridmsc.o \
	$(SRCPATH)/search/_searchStrategies/ss_emp.o \
	$(SRCPATH)/search/_searchStrategies/ss_testing.o \
	$(SRCPATH)/search/inp/inp.o \
	$(SRCPATH)/search/inp/_inpStrategies/inpStrategies.o \
	$(SRCPATH)/search/inp/_inpStrategies/is_null.o \
	$(SRCPATH)/search/inp/_inpStrategies/is_reduceOnly.o \
	$(SRCPATH)/search/inp/_inpStrategies/is_simple.o \
	$(SRCPATH)/search/inp/_inpStrategies/is_full.o \
	$(SRCPATH)/search/inp/_inpStrategies/is_rand.o \
	$(SRCPATH)/search/inp/_inpStrategies/is_testing.o \
	$(SRCPATH)/search/inp/IN/in.o \
	$(SRCPATH)/search/MP/mp.o \
	$(SRCPATH)/search/MP/_datastructures/mpVariables.o \
	$(SRCPATH)/search/MP/_datastructures/mpLiterals.o \
	$(SRCPATH)/search/MP/_datastructures/mpClauses.o \
	$(SRCPATH)/search/MP/_updateRules/updateRules.o \
	$(SRCPATH)/search/MP/_updateRules/ur_l0_bp.o \
	$(SRCPATH)/search/MP/_updateRules/ur_l0_sp.o \
	$(SRCPATH)/search/MP/_updateRules/ur_l0_embpg.o \
	$(SRCPATH)/search/MP/_updateRules/ur_l0_emspg.o \
	$(SRCPATH)/search/MP/_updateRules/ur_l1i_rhosp.o \
	$(SRCPATH)/search/MP/_updateRules/ur_l1i_rhoemspg.o \
	$(SRCPATH)/search/MP/_updateRules/ur_l1i_sigmaembpg.o \
	$(SRCPATH)/search/MP/_updateRules/ur_l1i_sigmaemspg.o \
	$(SRCPATH)/search/MP/_updateRules/ur_l2i_rhosigmapmp.o \
	$(SRCPATH)/search/MP/_updateRules/ur_l2im_rhosigmapmp.o \
	$(SRCPATH)/search/MP/_updateRules/ur_l2e_rhosigmapmp.o \
	$(SRCPATH)/search/SLS/sls.o \
	$(SRCPATH)/search/SLS/_datastructures/slsVariables.o \
	$(SRCPATH)/search/SLS/_datastructures/slsLiterals.o \
	$(SRCPATH)/search/SLS/_datastructures/slsClauses.o \
	$(SRCPATH)/search/SLS/_pickAndFlipVarRules/pickAndFlipVarRules.o \
	$(SRCPATH)/search/SLS/_pickAndFlipVarRules/pafvr_walksat.o \
	$(SRCPATH)/search/SLS/_pickAndFlipVarRules/pafvr_probsatPB.o \
	$(SRCPATH)/search/SLS/_pickAndFlipVarRules/pafvr_probsatEB.o \
	$(SRCPATH)/search/SLS/_assInitRules/assInitRules.o \
	$(SRCPATH)/search/SLS/_assInitRules/air_random.o \
	$(SRCPATH)/search/SLS/_assInitRules/air_provided.o \
	$(SRCPATH)/search/SLS/_assInitRules/air_providedthenrand.o \
	$(SRCPATH)/search/SLS/_tryRestartRules/tryRestartRules.o \
	$(SRCPATH)/search/SLS/_tryRestartRules/trr_singleunlimited.o \
	$(SRCPATH)/search/SLS/_tryRestartRules/trr_const.o \
	$(SRCPATH)/search/SLS/_tryRestartRules/trr_linear.o \
	$(SRCPATH)/search/SLS/_tryRestartRules/trr_sqrt.o \
	$(SRCPATH)/search/SLS/_tryRestartRules/trr_exp.o \
	$(SRCPATH)/search/CDCL/cdcl.o \
	$(SRCPATH)/search/CDCL/_datastructures/cdclVariables.o \
	$(SRCPATH)/search/CDCL/_datastructures/cdclLiterals.o \
	$(SRCPATH)/search/CDCL/_datastructures/cdclClauses.o \
	$(SRCPATH)/search/CDCL/_tools/cdclTools.o \
	$(SRCPATH)/search/CDCL/_tools/cdclUP.o \
	$(SRCPATH)/search/CDCL/_tools/cdclDecStack.o \
	$(SRCPATH)/search/CDCL/_selectVarRules/selectVarRules.o \
	$(SRCPATH)/search/CDCL/_selectVarRules/svr_random.o \
	$(SRCPATH)/search/CDCL/_selectVarRules/svr_vsids.o \
	$(SRCPATH)/search/CDCL/_selectVarRules/svr_rvsids.o \
	$(SRCPATH)/search/CDCL/_selectVarRules/svr_rrvsids.o \
	$(SRCPATH)/search/CDCL/_selectDirRules/selectDirRules.o \
	$(SRCPATH)/search/CDCL/_selectDirRules/sdr_random.o \
	$(SRCPATH)/search/CDCL/_selectDirRules/sdr_phasesaving.o \
	$(SRCPATH)/search/CDCL/_selectDirRules/sdr_failbinary.o \
	$(SRCPATH)/search/CDCL/_selectDirRules/sdr_phaseflip.o \
	$(SRCPATH)/search/CDCL/_conflictAnalysisRules/conflictAnalysisRules.o \
	$(SRCPATH)/search/CDCL/_conflictAnalysisRules/car_alldec.o \
	$(SRCPATH)/search/CDCL/_conflictAnalysisRules/car_firstuip.o \
	$(SRCPATH)/search/CDCL/_conflictAnalysisRules/car_lastuip.o \
	$(SRCPATH)/search/CDCL/_conflictAnalysisRules/car_alluip.o \
	$(SRCPATH)/search/CDCL/_strLearnedRules/strLearnedRules.o \
	$(SRCPATH)/search/CDCL/_strLearnedRules/slr_null.o \
	$(SRCPATH)/search/CDCL/_strLearnedRules/slr_allocc.o \
	$(SRCPATH)/search/CDCL/_strLearnedRules/slr_reasons.o \
	$(SRCPATH)/search/CDCL/_strLearnedRules/slr_antecedents.o \
	$(SRCPATH)/search/CDCL/_strLearnedRules/slr_local.o \
	$(SRCPATH)/search/CDCL/_strLearnedRules/slr_localrec.o \
	$(SRCPATH)/search/CDCL/_strOtherRules/strOtherRules.o \
	$(SRCPATH)/search/CDCL/_strOtherRules/sor_null.o \
	$(SRCPATH)/search/CDCL/_strOtherRules/sor_wtwo.o \
	$(SRCPATH)/search/CDCL/_computeBJLRules/computeBJLRules.o \
	$(SRCPATH)/search/CDCL/_computeBJLRules/cbjlr_firstassert.o \
	$(SRCPATH)/search/CDCL/_restartRules/restartRules.o \
	$(SRCPATH)/search/CDCL/_restartRules/rr_null.o \
	$(SRCPATH)/search/CDCL/_restartRules/rr_luby.o \
	$(SRCPATH)/search/CDCL/_restartRules/rr_lubystag.o \
	$(SRCPATH)/search/CDCL/_restartRules/rr_lubyagility.o \
	$(SRCPATH)/search/CDCL/_restartRules/rr_innerouter.o \
	$(SRCPATH)/search/CDCL/_restartRules/rr_innerouteragility.o \
	$(SRCPATH)/search/CDCL/_restartRules/rr_innerouterstag.o \
	$(SRCPATH)/search/CDCL/_maintenanceRules/maintenanceRules.o \
	$(SRCPATH)/search/CDCL/_maintenanceRules/mr_null.o \
	$(SRCPATH)/search/CDCL/_maintenanceRules/mr_inactivity.o \
	$(SRCPATH)/search/CDCL/_maintenanceRules/mr_lbd.o \
	$(SRCPATH)/search/CDCL/_maintenanceRules/mr_lbdinact.o \
	$(SRCPATH)/search/CDCL/_maintenanceRules/mr_lbdinactluby.o \
	$(SRCPATH)/search/CDCL/_inprocessingRules/inprocessingRules.o \
	$(SRCPATH)/search/CDCL/_inprocessingRules/ir_null.o \
	$(SRCPATH)/search/CDCL/_inprocessingRules/ir_reduceonly.o \
	$(SRCPATH)/search/CDCL/_inprocessingRules/ir_simple.o \
	$(SRCPATH)/search/CDCL/_inprocessingRules/ir_full.o \
	$(SRCPATH)/search/CDCL/_inprocessingRules/ir_testing.o \
	$(SRCPATH)/search/CDCL/_abortRules/abortRules.o \
	$(SRCPATH)/search/CDCL/_abortRules/ar_null.o \
	$(SRCPATH)/search/CDCL/_abortRules/ar_pmpcdcl.o \
	$(SRCPATH)/search/CDCL/_abortRules/ar_hybridmsc.o \
	$(SRCPATH)/handleresult/SOLWRITER/_DIMACSCNF/or_dimacscnf.o \
	$(SRCPATH)/handleresult/SOLWRITER/_APLEFOO/or_aplefoo.o \
	$(SRCPATH)/handleresult/SOLWRITER/solwriter.o \
	$(SRCPATH)/handleresult/handleresult.o \
	$(SRCPATH)/shutdown/shutdown.o 

%.o: %.c
	@echo "Compiling: $@"
	$(CC) $(CFLAGS) -c $< -o $@

STANDALONEBINARY: $(OBJ)
	@echo "Creating stand-alone binary (this may take a while)..." 
	$(CC) $(CFLAGS) -o $(BINPATH)/$(BINNAME) $(OBJ) $(LDFLAGS)

STATICLIBRARY: $(OBJ)
	@echo "Creating static library (this may take a while)..."
	$(AR) $(ARFLAGS) $(BINPATH)/$(STATICLIBNAME) $(OBJ)
	$(RANLIB) $(BINPATH)/$(STATICLIBNAME)

DYNAMICLIBRARY: $(OBJ)
	@echo "Creating dynamic library (this may take a while)..."
	$(CC) $(CLIBFLAGS) -o $(BINPATH)/$(DYNAMICLIBNAME) $(OBJ)

CLEANUP: 
	@echo "Cleaning up. Removing object files."
	@-rm -f -R *.o
	@-rm -f -R $(SRCPATH)/*.o
	@-rm -f -R $(SRCPATH)/_interface/*.o
	@-rm -f -R $(SRCPATH)/_datastructures/*.o
	@-rm -f -R $(SRCPATH)/_tools/*.o
	@-rm -f -R $(SRCPATH)/reset/*.o
	@-rm -f -R $(SRCPATH)/params/*.o
	@-rm -f -R $(SRCPATH)/params/_paramGraph/*.o
	@-rm -f -R $(SRCPATH)/params/_fuzzing/*.o
	@-rm -f -R $(SRCPATH)/guidance/*.o
	@-rm -f -R $(SRCPATH)/guidance/_guides/*.o
	@-rm -f -R $(SRCPATH)/startup/*.o
	@-rm -f -R $(SRCPATH)/startup/DETERM/_DIMACSCNF/*.o
	@-rm -f -R $(SRCPATH)/startup/DETERM/_APLEFOO/*.o
	@-rm -f -R $(SRCPATH)/startup/DETERM/*.o
	@-rm -f -R $(SRCPATH)/startup/SPECCER/_DIMACSCNF/*.o
	@-rm -f -R $(SRCPATH)/startup/SPECCER/_APLEFOO/*.o
	@-rm -f -R $(SRCPATH)/startup/SPECCER/*.o
	@-rm -f -R $(SRCPATH)/startup/LOADER/_DIMACSCNF/*.o
	@-rm -f -R $(SRCPATH)/startup/LOADER/_APLEFOO/*.o
	@-rm -f -R $(SRCPATH)/startup/LOADER/*.o
	@-rm -f -R $(SRCPATH)/prep/*.o
	@-rm -f -R $(SRCPATH)/prep/_prepStrategies/*.o
	@-rm -f -R $(SRCPATH)/prep/PRE/*.o
	@-rm -f -R $(SRCPATH)/prep/SATELITE/*.o
	@-rm -f -R $(SRCPATH)/classify/*.o
	@-rm -f -R $(SRCPATH)/classify/_classifyStrategies/*.o
	@-rm -f -R $(SRCPATH)/classify/ATTRIBUTOR/*.o
	@-rm -f -R $(SRCPATH)/classify/RANDOMFOREST/*.o
	@-rm -f -R $(SRCPATH)/adapt/*.o
	@-rm -f -R $(SRCPATH)/adapt/_adaptStrategies/*.o
	@-rm -f -R $(SRCPATH)/adapt/ITEADAPTER/*.o
	@-rm -f -R $(SRCPATH)/search/*.o
	@-rm -f -R $(SRCPATH)/search/_searchStrategies/*.o
	@-rm -f -R $(SRCPATH)/search/inp/*.o
	@-rm -f -R $(SRCPATH)/search/inp/_inpStrategies/*.o
	@-rm -f -R $(SRCPATH)/search/inp/IN/*.o
	@-rm -f -R $(SRCPATH)/search/MP/*.o
	@-rm -f -R $(SRCPATH)/search/MP/_datastructures/*.o
	@-rm -f -R $(SRCPATH)/search/MP/_updateRules/*.o
	@-rm -f -R $(SRCPATH)/search/SLS/*.o
	@-rm -f -R $(SRCPATH)/search/SLS/_datastructures/*.o
	@-rm -f -R $(SRCPATH)/search/SLS/_pickAndFlipVarRules/*.o
	@-rm -f -R $(SRCPATH)/search/SLS/_assInitRules/*.o
	@-rm -f -R $(SRCPATH)/search/SLS/_tryRestartRules/*.o
	@-rm -f -R $(SRCPATH)/search/CDCL/*.o
	@-rm -f -R $(SRCPATH)/search/CDCL/_tools/*.o
	@-rm -f -R $(SRCPATH)/search/CDCL/_datastructures/*.o
	@-rm -f -R $(SRCPATH)/search/CDCL/_selectVarRules/*.o
	@-rm -f -R $(SRCPATH)/search/CDCL/_selectDirRules/*.o
	@-rm -f -R $(SRCPATH)/search/CDCL/_conflictAnalysisRules/*.o
	@-rm -f -R $(SRCPATH)/search/CDCL/_strLearnedRules/*.o
	@-rm -f -R $(SRCPATH)/search/CDCL/_strOtherRules/*.o
	@-rm -f -R $(SRCPATH)/search/CDCL/_computeBJLRules/*.o
	@-rm -f -R $(SRCPATH)/search/CDCL/_restartRules/*.o
	@-rm -f -R $(SRCPATH)/search/CDCL/_maintenanceRules/*.o
	@-rm -f -R $(SRCPATH)/search/CDCL/_inprocessingRules/*.o
	@-rm -f -R $(SRCPATH)/search/CDCL/_abortRules/*.o
	@-rm -f -R $(SRCPATH)/handleresult/SOLWRITER/_DIMACSCNF/*.o
	@-rm -f -R $(SRCPATH)/handleresult/SOLWRITER/_APLEFOO/*.o
	@-rm -f -R $(SRCPATH)/handleresult/SOLWRITER/*.o
	@-rm -f -R $(SRCPATH)/handleresult/*.o
	@-rm -f -R $(SRCPATH)/shutdown/*.o
	
clean: CLEANUP
	@echo "Cleaning up. Removing binary files."
	@-rm -f $(BINPATH)/*