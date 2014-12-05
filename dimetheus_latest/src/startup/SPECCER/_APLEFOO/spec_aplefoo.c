/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "spec_aplefoo.h"

void speccer_loadSpecs_aplefoo_resetPlugin(){
	#ifdef VERBOSE_SPECCER
	printf("c       SPECCER: Resetting APLEFOO plug-in...\n");
	#endif

	aplefoo_currentIdentifier = NULL;

	aplefoo_currentToken = NULL;

	aplefoo_tree_root = NULL;
	aplefoo_tree_current = NULL;
}

void speccer_loadSpecs_aplefoo_initPlugin(){
	#ifdef VERBOSE_SPECCER
	printf("c       SPECCER: Initializing APLEFOO plug-in...\n");
	#endif
	aplefoo_currentToken = malloc(sizeof(aplefoo_token) * 1);
	if (aplefoo_currentToken == NULL){
		printf("c ERROR. SPECCER was unable to allocate memory for the APLEFOO current token. Out of memory?\n");
		speccer_returnCode = SPECCER_ERROR;
		return;
	}

	aplefoo_currentIdentifier = NULL;
	aplefoo_currentIdentifier = malloc(sizeof(char) * APLEFOO_IDENTIFIER_MAXLEN);
	if (aplefoo_currentIdentifier == NULL){
		printf("c ERROR. SPECCER was unable to allocate memory for the APLEFOO current identifier. Out of memory?\n");
		speccer_returnCode = SPECCER_ERROR;
		return;
	}

	aplefoo_currentToken->op = APLEFOO_OP_VAL_NULL;
	aplefoo_currentToken->type = APLEFOO_TOKEN_TYPE_NULL;
	aplefoo_currentToken->var = 0;
}

void speccer_loadSpecs_aplefoo_disposePlugin(){
	#ifdef VERBOSE_SPECCER
	printf("c       SPECCER: Disposing APLEFOO plug-in...\n");
	#endif
	if (aplefoo_currentIdentifier != NULL){
		free(aplefoo_currentIdentifier);
		aplefoo_currentIdentifier = NULL;
	}

	if (aplefoo_currentToken != NULL){
		free(aplefoo_currentToken);
		aplefoo_currentToken = NULL;
	}

	aplefoo_extren_disposeTree(aplefoo_tree_root);
	aplefoo_tree_root = NULL;
	aplefoo_tree_current = NULL;
}

#ifndef COMPILEASLIBRARY
void speccer_loadSpecs_aplefoo_resetChars(){
	rewind(param_formula);
}

char speccer_loadSpecs_aplefoo_getNextChar(){
	return fgetc(param_formula);
}

void speccer_loadSpecs_aplefoo_ungetChar(char c){
	ungetc(c, param_formula);
	c = ' ';
}
#else
void speccer_loadSpecs_aplefoo_resetChars(){
	speccer_loadSpecs_aplefoo_currentCharNum = 0;
}

char speccer_loadSpecs_aplefoo_getNextChar(){
	if (dimetheus_ifc_aplefooFormula[speccer_loadSpecs_aplefoo_currentCharNum] == '\0'){
		return EOF;
	} else {
		++speccer_loadSpecs_aplefoo_currentCharNum;
		return dimetheus_ifc_aplefooFormula[speccer_loadSpecs_aplefoo_currentCharNum-1];
	}
}

void speccer_loadSpecs_aplefoo_ungetChar(char c){
	--speccer_loadSpecs_aplefoo_currentCharNum;
	c = ' ';
}
#endif


void speccer_loadSpecs_aplefoo_readOpSymbols(){
	//To load all the operator symbol definitions.
	char chr, chr2;
	char opname[255], opsym;

	#ifdef VERBOSE_SPECCER
	printf("c       SPECCER:   Checking d-lines...\n");
	#endif

	//We reset the operator symbols to their default values.
	aplefoo_op_sym_not = APLEFOO_OP_SYM_NOT;
	aplefoo_op_sym_and = APLEFOO_OP_SYM_AND;
	aplefoo_op_sym_xor = APLEFOO_OP_SYM_XOR;
	aplefoo_op_sym_or  = APLEFOO_OP_SYM_OR;
	aplefoo_op_sym_rim = APLEFOO_OP_SYM_RIM;
	aplefoo_op_sym_lim = APLEFOO_OP_SYM_LIM;
	aplefoo_op_sym_equ = APLEFOO_OP_SYM_EQU;

	speccer_loadSpecs_aplefoo_resetChars();
	chr = '#';
	while (chr != EOF) {
		//Get the next character.
		chr = speccer_loadSpecs_aplefoo_getNextChar();
		//If it indicates a comment, problem, or formula line, we just skip.
		if (chr == 'c' || chr == 'p' || chr == '$') {
			while ((chr2 = speccer_loadSpecs_aplefoo_getNextChar()) != EOF && chr2 != '\n');
		}
		//In case we just read a definition line, we load it, and then skip the line.
		if (chr == 'd'){
			//We have read the 'd' at the beginning of a definition.
			speccer_loadSpecs_aplefoo_ungetChar(chr);

			//We load the definition and check if that works out.
			if (fscanf(param_formula, "d %s %c", opname, &opsym) != 2){
				printf("c ERROR. SPECCER was unable to read a definition line, starting with d. They must have\n");
				printf("c ERROR. the following format: \"d %%s %%c\", where the string is the operator name like \"AND\".\n");
				printf("c ERROR. and the character is a single character used to denote the operation.\n");
				speccer_returnCode = SPECCER_ERROR;
				return;
			}

			//We must now ensure, that the operator symbol is not in the variable name intervals.
			if (APLEFOO_ISCHAR_VAR(opsym)){
				printf("c ERROR. The read operator character \"%c\" is a valid variable identification symbol and\n", opsym);
				printf("c ERROR. must not be used. You are not allowed to use characters in [a-z,A-Z,0-9,_] as operators.\n");
				speccer_returnCode = SPECCER_ERROR;
				return;
			}

			//We now check that the definition makes a statement about one of our operators NOT, AND, XOR, OR, ...
			if (strcmp("NOT", opname) != 0
					&& strcmp("AND", opname) != 0
					&& strcmp("XOR", opname) != 0
					&& strcmp("OR", opname) != 0
					&& strcmp("RIM", opname) != 0
					&& strcmp("LIM", opname) != 0
					&& strcmp("EQU", opname) != 0 ){
				printf("c ERROR. The operator name %s is not recognized. You must choose from the following list:\n",opname);
				printf("c ERROR.   NOT, AND, XOR, OR, RIM, LIM, EQU\n");
				speccer_returnCode = SPECCER_ERROR;
				return;
			}

			//We have now read the operator name and the operator symbol. We will override the default with the correct value.
			if (strcmp("NOT", opname) == 0) {
				aplefoo_op_sym_not = opsym;
			} else if (strcmp("AND", opname) == 0) {
				aplefoo_op_sym_and = opsym;
			} else if (strcmp("XOR", opname) == 0) {
				aplefoo_op_sym_xor = opsym;
			} else if (strcmp("OR", opname) == 0) {
				aplefoo_op_sym_or = opsym;
			} else if (strcmp("RIM", opname) == 0) {
				aplefoo_op_sym_rim = opsym;
			} else if (strcmp("LIM", opname) == 0) {
				aplefoo_op_sym_lim = opsym;
			} else if (strcmp("EQU", opname) == 0) {
				aplefoo_op_sym_equ = opsym;
			}

			#ifdef VERBOSE_SPECCER
			printf("c       SPECCER:     Read operator definition %s: %c\n", opname,opsym);
			#endif

			while ((chr2 = speccer_loadSpecs_aplefoo_getNextChar()) != EOF && chr2 != '\n');
		}
	}

	speccer_loadSpecs_aplefoo_resetChars();
}

uint32_t speccer_loadSpecs_aplefoo_addNewVariableIdentifier(uint32_t len){
	//Reads the current identifier and determines if this is a new one. If so, it appends it to the list. If not, it returns
	//the variable ID of the variable with this identifier.
	uint32_t id = 0;
	do {
		++id;
		//Check if we reached the end of the list.
		if (main_varNames[id] == NULL){
			//Yes, we use the current ID for this new variable.
			break;
		}
	} while (strcmp(aplefoo_currentIdentifier, main_varNames[id]) != 0);
	//To possible situations remain: we either have a NULL name-field, which means that we must add the new identifier, or,
	//the name field is not NULL, in which case the current ID is the ID for the variable.
	if (main_varNames[id] == NULL){
		//It is a new variable identifier, we append it to the list at the current position. We will first add this new
		//variable name to the current ID, and then we extend the number of variable name identifiers we have by one and set
		//it to NULL.
		main_varNames[id] = malloc(sizeof(char)*(len+1));
		if (main_varNames[id] == NULL){
			printf("c ERROR. SPECCER was unable to allocate memory for a new variable name. Out of memory?\n");
			speccer_returnCode = SPECCER_ERROR;
			return 0;
		}
		strcpy(main_varNames[id], aplefoo_currentIdentifier);
		//We must now increase the names array in the first dimension to allow us to store more stuff. Allocate memory using
		//ID + 2. The +2 makes sure, that we have main_varNames[0] and main_varNames[id+1], the latter being the new variable.
		main_varNames = realloc(main_varNames, sizeof(char*) * (id+2));
		main_varNames[id+1] = NULL;
		//The current ID, after writing, denotes the newest variable and therefore, the maximum number of variables read so
		//far. We can use it to denote that in the formula already.
		f.n_initial = id;
		f.n_afterPrep = id;
	}

	return id;
}

void speccer_loadSpecs_aplefoo_getNextToken(){
	//We assume that we are in a formula definition line. We will now read the next token.
	char chr, *idWritePos = aplefoo_currentIdentifier;
	uint32_t idWritten = 0, varID = 0;

	//Reset the current token.
	APLEFOO_CURRENTTOKEN_RESET();

	//We read a new token.
	chr = speccer_loadSpecs_aplefoo_getNextChar();
	if (APLEFOO_ISCHAR_UOP(chr)){
		//We create a new unary operator token.
		APLEFOO_NEWTOKEN_UOP(chr);
	} else if (APLEFOO_ISCHAR_BOP(chr)){
		//We create a new binary operator token.
		APLEFOO_NEWTOKEN_BOP(chr);
	} else if (APLEFOO_ISCHAR_OBR(chr)){
		//We create a new opening bracket token.
		APLEFOO_NEWTOKEN_OBR();
	} else if (APLEFOO_ISCHAR_CBR(chr)){
		//We create a new closing bracket token.
		APLEFOO_NEWTOKEN_CBR();
	} else if (APLEFOO_ISCHAR_VAR(chr)){
		//We create a new identifier token. The tricky part here is that we must keep reading until the whole identifier
		//has been read.
		do {
			idWritePos[idWritten++] = chr;
			if (idWritten == APLEFOO_IDENTIFIER_MAXLEN){
				printf("c ERROR. SPECCER was unable to read an identifier, it exceeds length %d.\n", APLEFOO_IDENTIFIER_MAXLEN);
				speccer_returnCode = SPECCER_ERROR;
				return;
			}
			chr = speccer_loadSpecs_aplefoo_getNextChar();
		} while (APLEFOO_ISCHAR_VAR(chr));
		//Terminate the variable identifier.
		idWritePos[idWritten] = '\0';
		speccer_loadSpecs_aplefoo_ungetChar(chr);
		//We must append this identifier to the list of known ones. The ID returned will be used for the token.
		varID = speccer_loadSpecs_aplefoo_addNewVariableIdentifier(idWritten);if (speccer_returnCode != SPECCER_UNKNOWN){return;}
		APLEFOO_NEWTOKEN_VAR(varID);
	} else {
		printf("c ERROR. SPECCER was unable to use character %c for any known token.\n", chr);
		speccer_returnCode = SPECCER_ERROR;
		return;
	}

}

void speccer_loadSpecs_aplefoo_tokenize(){
	#ifdef VERBOSE_SPECCER
	printf("c       SPECCER:   Splitting formula into tokens and loading tokens into tree...\n");
	#endif

	int32_t bracketBalance = 0, readTokens = 0;
	char chr, chr2;
	speccer_loadSpecs_aplefoo_resetChars();

	//We manually create an opening bracket as the root of our tree.
	aplefoo_currentToken->type = APLEFOO_TOKEN_TYPE_OBR;
	aplefoo_currentToken->op = APLEFOO_OP_VAL_NULL;
	aplefoo_currentToken->var = 0;
	aplefoo_token_type previousTokenType = APLEFOO_TOKEN_TYPE_OBR;

	aplefoo_tree_root = aplefoo_extern_createNode(NULL, 0, APLEFOO_TOKEN_TYPE_OBR, APLEFOO_OP_VAL_NULL,0,2);
	aplefoo_tree_current = aplefoo_tree_root;

	chr = '#';
	//We search for the formula definition lines.
	while (chr != EOF && speccer_returnCode == SPECCER_UNKNOWN) {
		chr = speccer_loadSpecs_aplefoo_getNextChar();
		if (chr != '$') {
			while ((chr2 = speccer_loadSpecs_aplefoo_getNextChar()) != EOF && chr2 != '\n');
			continue;
		}
		speccer_loadSpecs_aplefoo_ungetChar(chr);
		//We have found a formula definition line and must now read all tokens from it.
		while ((chr2 = speccer_loadSpecs_aplefoo_getNextChar()) != EOF && chr2 != '\n'){
			if (chr2 == '$' || chr2 == ' ' || chr2 == '\t') continue;
			speccer_loadSpecs_aplefoo_ungetChar(chr2);
			speccer_loadSpecs_aplefoo_getNextToken();
			if (speccer_returnCode != SPECCER_UNKNOWN){
				printf("c ERROR. SPECCER asked for a new token but loading it somehow failed.\n");
				return;
			}

			//We perform some sanity checks on the file here while reading it. We make sure, that the following table of read
			//tokens versus current token is enforced. We have VAR being a variable token, OBR is an opening bracket, CBR is
			//a closing bracket, UOP is a unary prefix operator, BOP is a binary infix operator. Here, o means OK X means not
			//OK. The lines tell you what was the previous token read, and the columns identify the current token read.
			//     VAR OBR CBR UOP BOP
			//VAR   X   X   o   X   o
			//OBR   o   o   X   o   X
			//CBR   X   X   o   X   o
			//UOP   o   o   X   o   X
			//BOP   o   o   X   o   X
			//Note how similar the lines and columns are. For sake of easy-code-reading, we will not exploit this below.
			if (previousTokenType == APLEFOO_TOKEN_TYPE_VAR){
				if (aplefoo_currentToken->type == APLEFOO_TOKEN_TYPE_VAR){
					printf("c ERROR. SPECCER read previous token: VAR, current token: VAR.\n");
					speccer_returnCode = SPECCER_BADTOKEN; break;
				} else if (aplefoo_currentToken->type == APLEFOO_TOKEN_TYPE_OBR){
					printf("c ERROR. SPECCER read previous token: VAR, current token: OBR.\n");
					speccer_returnCode = SPECCER_BADTOKEN; break;
				} else if (aplefoo_currentToken->type == APLEFOO_TOKEN_TYPE_OBR){
					printf("c ERROR. SPECCER read previous token: VAR, current token: UOP.\n");
					speccer_returnCode = SPECCER_BADTOKEN; break;
				}
			} else if (previousTokenType == APLEFOO_TOKEN_TYPE_OBR){
				++bracketBalance;
				if (aplefoo_currentToken->type == APLEFOO_TOKEN_TYPE_CBR){
					printf("c ERROR. SPECCER read previous token: OBR, current token: CBR.\n");
					speccer_returnCode = SPECCER_BADTOKEN; break;
				} else if (aplefoo_currentToken->type == APLEFOO_TOKEN_TYPE_BOP){
					printf("c ERROR. SPECCER read previous token: OBR, current token: BOP.\n");
					speccer_returnCode = SPECCER_BADTOKEN; break;
				}
			} else if (previousTokenType == APLEFOO_TOKEN_TYPE_CBR){
				--bracketBalance;
				if (aplefoo_currentToken->type == APLEFOO_TOKEN_TYPE_VAR){
					printf("c ERROR. SPECCER read previous token: CBR, current token: VAR.\n");
					speccer_returnCode = SPECCER_BADTOKEN; break;
				} else if (aplefoo_currentToken->type == APLEFOO_TOKEN_TYPE_OBR){
					printf("c ERROR. SPECCER read previous token: CBR, current token: OBR.\n");
					speccer_returnCode = SPECCER_BADTOKEN; break;
				} else if (aplefoo_currentToken->type == APLEFOO_TOKEN_TYPE_OBR){
					printf("c ERROR. SPECCER read previous token: CBR, current token: UOP.\n");
					speccer_returnCode = SPECCER_BADTOKEN; break;
				}
			} else if (previousTokenType == APLEFOO_TOKEN_TYPE_UOP){
				if (aplefoo_currentToken->type == APLEFOO_TOKEN_TYPE_CBR){
					printf("c ERROR. SPECCER read previous token: UOP, current token: CBR.\n");
					speccer_returnCode = SPECCER_BADTOKEN; break;
				} else if (aplefoo_currentToken->type == APLEFOO_TOKEN_TYPE_BOP){
					printf("c ERROR. SPECCER read previous token: UOP, current token: BOP.\n");
					speccer_returnCode = SPECCER_BADTOKEN; break;
				}
			} else if (previousTokenType == APLEFOO_TOKEN_TYPE_BOP){
				if (aplefoo_currentToken->type == APLEFOO_TOKEN_TYPE_CBR){
					printf("c ERROR. SPECCER read previous token: BOP, current token: CBR.\n");
					speccer_returnCode = SPECCER_BADTOKEN; break;
				} else if (aplefoo_currentToken->type == APLEFOO_TOKEN_TYPE_BOP){
					printf("c ERROR. SPECCER read previous token: BOP, current token: BOP.\n");
					speccer_returnCode = SPECCER_BADTOKEN; break;
				}
			}

			//We also check, that the bracket balance is ok.
			if (bracketBalance < 0){
				printf("c ERROR. SPECCER read more closing brackets than opening brackets.\n");
				speccer_returnCode = SPECCER_BADTOKEN; break;
			}

			//We retain the last read token, which is the type of the current token that we are about to buffer.
			previousTokenType = aplefoo_currentToken->type;

			//We now modify the tree according to the current token.
			++readTokens;
			if (!aplefoo_extern_alterTree()){
				printf("c ERROR. SPECCER tried to alter the operator tree using a token but that failed.\n");
				break;
			}
		}
	}

	//We will now write the matching closing bracket for the start of the formula. Before we write it, we will, however,
	//check that the closing bracket rules apply. Check the little table above to see what is NOT allowed as previous token, if
	//we want to write the final closing bracket.
	if (speccer_returnCode == SPECCER_UNKNOWN){
		if (readTokens == 0){
			printf("c ERROR. SPECCER was unable to find any token in the formula. Empty formula? Bad input format?\n");
			speccer_returnCode = SPECCER_ERROR; return;
		}
		aplefoo_currentToken->type 	= APLEFOO_TOKEN_TYPE_CBR;
		aplefoo_currentToken->op 	= APLEFOO_OP_VAL_NULL;
		aplefoo_currentToken->var 	= 0;
		if (previousTokenType == APLEFOO_TOKEN_TYPE_OBR
				|| previousTokenType == APLEFOO_TOKEN_TYPE_UOP
				|| previousTokenType == APLEFOO_TOKEN_TYPE_BOP){
			//Not looking good. The previous token implies that the formula is not "done" yet.
			if (previousTokenType == APLEFOO_TOKEN_TYPE_OBR){
				printf("c ERROR. SPECCER read previous token: OBR, current token: CBR (final).\n");
			} else if (previousTokenType == APLEFOO_TOKEN_TYPE_UOP){
				printf("c ERROR. SPECCER read previous token: UOP, current token: CBR (final).\n");
			} else if (previousTokenType == APLEFOO_TOKEN_TYPE_BOP){
				printf("c ERROR. SPECCER read previous token: BOP, current token: CBR (final).\n");
			}
			speccer_returnCode = SPECCER_BADTOKEN;
		} else {
			//Looking good, we can append the final bracket.
			if (!aplefoo_extern_alterTree()){
				printf("c ERROR. SPECCER tried write the final closing bracket but that failed.\n");
			}
			//We can now delete the old root node and make the first child the new root.
			aplefoo_tree_current = aplefoo_tree_root->children[0];
			aplefoo_extern_deleteNode(aplefoo_tree_root);
			aplefoo_tree_root = aplefoo_tree_current;
		}
	}

	if (speccer_returnCode == SPECCER_BADTOKEN){
		printf("c ERROR. The formula has a format that does not make sense for propositional logic in the APLEFOO.\n");
		speccer_returnCode = SPECCER_ERROR; return;
	}

	//Check, that the tree pointer now points to the opening bracket with no parent.
	if (aplefoo_tree_current != aplefoo_tree_root){
		printf("c ERROR. SPECCER was unable to close the read formula correctly. Bad format for APLEFOO?\n");
		speccer_returnCode = SPECCER_ERROR; return;
	}

	//We now completely translated the formula into the operator tree.
	speccer_loadSpecs_aplefoo_resetChars();
	#ifdef VERBOSE_SPECCER
	printf("c       SPECCER:   Done. Read tokens: %d\n", readTokens);
	#endif
}

void speccer_loadSpecs_apleefoo_translateToStdBasis(){
	#ifdef VERBOSE_SPECCER
	printf("c       SPECCER:   Translating tree into standard basis...\n");
	#endif

	if (!aplefoo_extern_translateToStdBasis(aplefoo_tree_root)){
		printf("c ERROR. SPECCER was unable to translate the APLEFOO tree into the standard basis.\n");
		speccer_returnCode = SPECCER_ERROR;
		return;
	}

	#ifdef VERBOSE_SPECCER
	printf("c       SPECCER:   Done.\n");
	#endif
}

void speccer_loadSpecs_aplefoo_pushNegations(){
	#ifdef VERBOSE_SPECCER
	printf("c       SPECCER:   Pushing down negations...\n");
	#endif

	if (!aplefoo_extern_pushNegations(aplefoo_tree_root, 0)){
		printf("c ERROR. SPECCER was unable to push down negations after APLEFOO tree translation.\n");
		speccer_returnCode = SPECCER_ERROR;
		return;
	}

	#ifdef VERBOSE_SPECCER
	printf("c       SPECCER:   Done.\n");
	#endif
}

void speccer_loadSpecs_aplefoo_tseitinTransform(){
	#ifdef VERBOSE_SPECCER
	printf("c       SPECCER:   Transforming into CNF using Tseitin...\n");
	#endif
	int32_t unitLit = 0;
	aplefoo_numAuxVars = 0;
	if (param_speccerNNFFirst){
		#ifdef VERBOSE_SPECCER
		printf("c       SPECCER:     Assuming NNF is given, using optimized Tseitin.\n");fflush(stdout);
		#endif
		unitLit = aplefoo_extern_tseitinTransformFromNNF(aplefoo_tree_root);
	} else {
		#ifdef VERBOSE_SPECCER
		printf("c       SPECCER:     Assuming that no NNF is given, using full Tseitin.\n");fflush(stdout);
		#endif
		unitLit = aplefoo_extern_tseitinTransformFromArbitrary(aplefoo_tree_root);
	}
	if (unitLit == 0){
		printf("c ERROR. SPECCER was unable to translate the formula into CNF using Tseitin.\n");
		speccer_returnCode = SPECCER_ERROR;
		return;
	}
	#ifdef VERBOSE_SPECCER
	printf("c       SPECCER:   Done.\n");
	#endif
}

void speccer_loadSpecs_aplefoo(){
	#ifdef COMPILEASLIBRARY
	speccer_loadSpecs_aplefoo_currentCharNum = 0;
	#endif

	//We will now parse the file to find the operator definitions, if any.
	speccer_loadSpecs_aplefoo_readOpSymbols();
	if (speccer_returnCode != SPECCER_UNKNOWN){
		printf("c ERROR. SPECCER was unable to load the operator definitions.\n");
		return;
	}

	//We first allocate some memory for the variable names. In the first dimension, we add exactly two pointers. This array
	//is extended dynamically and the number of elements not being NULL will later tell us how many variables there are.
	main_varNames = malloc(sizeof(char*) * 2);
	if (main_varNames == NULL){
		printf("c ERROR. SPECCER was unable to allocate memory for the variable names. Out of memory?\n");
		speccer_returnCode = SPECCER_ERROR;
		return;
	}
	main_varNames[0] = NULL;
	main_varNames[1] = NULL;

	//We will now load the formula as tokens into the two token stacks.
	speccer_loadSpecs_aplefoo_tokenize();
	if (speccer_returnCode != SPECCER_UNKNOWN){
		printf("c ERROR. SPECCER was unable to split the formula into tokens.\n");
		return;
	}

	//After loading the formula into the token buffers, we will perform some checks on it to make sure this formula can be
	//processed by the solver.
	if (f.n_initial > VAR_OFFSET_MASK){
		printf("c ERROR. The formula has more than %u variables.\n", VAR_OFFSET_MASK);
		printf("c ERROR. This solver can only solve formulas with less variables.\n");
		speccer_returnCode = SPECCER_ERROR;
		speccer_loadSpecs_aplefoo_resetChars();
		return;
	}

	if (f.n_initial == 0){
		printf("c ERROR. Propositional logic does not allow the variable set to be empty, but there are no variables.\n");
		speccer_returnCode = SPECCER_ERROR;
		speccer_loadSpecs_aplefoo_resetChars();
		return;
	}

	if (param_speccerNNFFirst){
		//We will now normalize the tree. The first step is to translate all boolean operators such that we only work on the
		//standard basis using NOT, AND, OR.
		speccer_loadSpecs_apleefoo_translateToStdBasis();
		if (speccer_returnCode != SPECCER_UNKNOWN){
			printf("c ERROR. SPECCER failed to translate the formula into the standard basis.\n");
			return;
		}
		//We must now push down the negations in order to get the NNF.
		speccer_loadSpecs_aplefoo_pushNegations();
		if (speccer_returnCode != SPECCER_UNKNOWN){
			printf("c ERROR. SPECCER failed to push the negations on the standard basis formula.\n");
			return;
		}
	}

	//We can now transform the formula into a CNF.
	speccer_loadSpecs_aplefoo_tseitinTransform();
	if (speccer_returnCode != SPECCER_UNKNOWN){
		printf("c ERROR. SPECCER failed to apply the Tseitin transformation.\n");
		return;
	}

	//Again, we check that the amount of variables does not exceed the solvers capabilities.
	if (f.n_initial > VAR_OFFSET_MASK){
		printf("c ERROR. The formula has more than %u variables.\n", VAR_OFFSET_MASK);
		printf("c ERROR. This solver can only solve formulas with less variables.\n");
		speccer_returnCode = SPECCER_ERROR;
		speccer_loadSpecs_aplefoo_resetChars();
		return;
	}

	if (f.n_initial == 0){
		printf("c ERROR. Propositional logic does not allow the variable set to be empty, but there are no variables.\n");
		speccer_returnCode = SPECCER_ERROR;
		speccer_loadSpecs_aplefoo_resetChars();
		return;
	}

	//After the transformation, we now have the number of clauses we need to work on given in startup_cnfClausesNumUsed.
	f.m_initial = startup_cnfClausesNumUsed;

	//Finally, we make sure that the formula is not exceeding the solvers capabilities for solving it.
	if (f.m_initial > CLS_OFFSET_MASK){
		printf("c ERROR. The formula has more than %u clauses after translation into CNF.\n", CLS_OFFSET_MASK);
		printf("c ERROR. This solver can only solve formulas with less clauses.\n");
		speccer_returnCode = SPECCER_ERROR;
		speccer_loadSpecs_aplefoo_resetChars();
		return;
	}

	speccer_loadSpecs_aplefoo_resetChars();
}
