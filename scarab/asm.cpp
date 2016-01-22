/*
  -----------------------------------------------------
  asm.cpp

  Created on: 2014/05/07 20:33:38
  Author: Zhibin Zhang
  Email: zzbthechaos@gmail.com

  -----------------------------------------------------
*/

#include "asm.h"
#include "asm-table.h"

char g_err_msg[80];
int g_mnemonic_index;

void clearAsmInstr(ASMINSTRUCTION *instr){
    instr->lockAndRepeat_ = -1;
    instr->segmentOverride_ = -1;
    instr->OperandSizeOverride_ = -1;
    instr->AddressSizeOverride_ = -1;
    instr->size_ = 0;
    instr->ModRM_ = false;
    instr->s_ = -1;

    instr->dest_ = NULL;
    instr->src1_ = NULL;
    instr->src2_ = NULL;
    instr->src3_ = NULL;
    instr->machineCode_ = NULL;
}

void clearAsmOperand(ASMOPERAND *operand){
    operand->type_ = OPERAND_UNDEFINED;
    operand->operand_size_ = -1;
    operand->addressing_size_ = -1;
    operand->displacement_size_ = -1;
    operand->segment_ = -1;
    operand->scale_ = -1;
    operand->index_ = -1;
    operand->base_ = -1;
    operand->signExtended_ = false;
}

bool strcmpMnemonic(const char *test, const char *compare){
    char mnemonic[15], *quotPtr, *temp = const_cast<char *>(compare);
    int size = -1;

    while(temp != NULL){
	if (temp[0] == ',')
	    temp++;

	quotPtr = strchr(temp, ',');
	size = (quotPtr == NULL) ? strlen(temp) : quotPtr - temp;

	memcpy(mnemonic, temp, size);
	mnemonic[size] = '\0';
	temp = quotPtr;

	if (strcmp(test, mnemonic) == 0)
	    return true;
    }

    return false;
}

Lex::Lex(){
    clear();
}

std::vector<TOKEN>* Lex::getToken(){
    return &token_;
}

std::vector<INSTRSTR> *Lex::getInstr(){
    return &instructions_;
}

std::vector<SYMBOL> *Lex::getSymbol(){
    return &symtab_;
}

char* Lex::getTokenName(bool include_current){
    if (start_point_ == current_point_ || start_point_ > (int)strlen(current_instr_))
	return NULL;

    int size = current_point_ - start_point_ + 1 + (include_current ? 1 : 0);
    char *name = (char*)malloc(size);
    memcpy(name, current_instr_ + start_point_, size - 1);
    name[size - 1] = '\0';

    return name;
}

void Lex::insertToken(INT8 token_type, INT8 token_class, bool include_current){
    if (include_current)
	current_point_++;

    char *name = getTokenName();
    if (name == NULL)
	return;

    TOKEN token = TOKEN(name, token_type, token_class, instr_index_, instrstr_index_, start_point_, current_point_);
    token_.push_back(token);

    current_point_--;
    current_token_count_++;
}

bool Lex::collideWithReservedWord(char *name, INT8 &return_type, INT8 &return_class){
    for (int i = 0; i < SEGMENT_NUM; i++){
	if (strcmp(name, g_segment[i]) == 0){
	    return_type = TT_SEGMENT;
	    return_class = (TOKEN_CLASS_SEGMENT)i;
	    return true;
	}
    }
    for (int i = 0; i < PREFIX_NUM; i++){
	if (strcmp(name, g_prefix_lr[i]) == 0){
	    return_type = TT_PREFIX;
	    return_class = (TOKEN_CLASS_PREFIX)i;
	    return true;
	}
    }
    for (int i = 0; i < GENERAL_REGISTER_NUM; i++){
	if (strcmp(name, g_general_register[i]) == 0){
	    return_type = TT_GENERAL_REGISTER;
	    return_class = (TOKEN_CLASS_GENERAL_REGISTER)i;
	    return true;
	}
    }
    for (int i = 0; i < FLOAT_REGISTER_NUM; i++){
	if (strcmp(name, g_float_register[0][i]) == 0){
	    return_type = TT_FLOAT_REGISTER;
	    return_class = (TOKEN_CLASS_FLOAT_REGISTER)i;
	    return true;
	}
    }
    for (int i = 0; i < SSEMMX_REGISTER_NUM; i++){
	if (strcmp(name, g_ssemmx_register[i]) == 0){
	    return_type = TT_SSEMMX_REGISTER;
	    return_class = (TOKEN_CLASS_SSEMMX_REGISTER)i;
	    return true;
	}
    }
    for (int i = 0; i < SIZE_NUM; i++){
	if (strcmp(name, g_size[i]) == 0){
	    return_type = TT_SIZE;
	    return_class = (TOKEN_CLASS_SIZE)i;
	    return true;
	}
    }
    for (int i = 0; i < SPECIAL_NUM; i++){
	if (strcmp(name, g_special[i]) == 0){
	    return_type = TT_SPECIAL;
	    return_class = (TOKEN_CLASS_SPECIAL)i;
	    return true;
	}
    }
    for (int i = 0; i < CR_NUM; i++){
	if (strcmp(name, g_cr[i]) == 0){
	    return_type = TT_CR;
	    return_class = (TOKEN_CLASS_CR)i;
	    return true;
	}
    }
    for(int i = 0; i < DR_NUM; i++){
	if (strcmp(name, g_dr[i]) == 0){
	    return_type = TT_DR;
	    return_class = (TOKEN_CLASS_DR)i;
	    return true;
	}
    }
    for (int i = 0; i < MNEMONIC_NUM; i++){
	if (strcmpMnemonic(name, g_instr[i].mnemonic_)){
	    g_mnemonic_index = i;
	    return_type = TT_MNEMONIC;
	    return_class = TC_UNDEFINED;
	    return true;
	}
    }

    return false;
}

void Lex::insertToken(char *name, INT8 token_type, INT8 token_class, bool include_current){
    if (include_current)
	current_point_++;

    TOKEN token = TOKEN(name, token_type, token_class, instr_index_, instrstr_index_, start_point_, current_point_);
    token_.push_back(token);

    current_point_--;
    current_token_count_++;
}

SYMBOL* Lex::getSymbolByName(char *name){
    for (int i = 0; i < (int)symtab_.size(); i++){
	if (strcmp(symtab_[i].name_, name) == 0)
	    return &symtab_[i];
    }
    return NULL;
}

UINT8 Lex::insertSymbol(char *name, int token_index){
    if (getSymbolByName(name)){
	sprintf(g_err_msg, "%s%s", "Multiple definition of label: ", name);
	return RC_MULTIPLE_LABEL;
    }

    SYMBOL sym = SYMBOL(name, token_index, instr_index_);
    symtab_.push_back(sym);

    return RC_SUCCESS;
}

UINT8 Lex::scanInstruction(){
    if (current_instr_ == NULL){
	sprintf(g_err_msg, "%s", "Empty instruction");
	return RC_LEX_ERR;
    }

    start_point_ = 0;
    current_point_ = 0;
    current_token_count_ = 0;
    int state = 0;
    char ch;
    UINT8 ret;

    while (1){
	ch = current_instr_[current_point_];

	if (state == 0){
	    while (ch == ' ' || ch == '\t'){
		current_point_++;
		ch = current_instr_[current_point_];
		if (ch == '\0'){
		    //instr_index_++;
		    return RC_SUCCESS;
		}
	    }
	    start_point_ = current_point_;
	}

	switch(state){
	    // start
	case 0:{
	    if (isalpha(ch) || ch == '_')
		state = 1;
	    else if (isdigit(ch)){
		if (ch == '0' && current_instr_[current_point_ + 1] == 'x'){
		    current_point_++;
		    state = 2;
		}
		else
		    state = 3;
	    }
	    else if (ch == ';')
		state = 4;
	    else if (ch == '<')
		state = 5;
	    else{
		state = 0;
		switch(ch){
		case '+':
		    insertToken(TT_MARK, TMK_ADD, INCLUDE_CURRENT);
		    break;
		case '-':
		    insertToken(TT_MARK, TMK_SUB, INCLUDE_CURRENT);
		    break;
		case '*':
		    insertToken(TT_MARK, TMK_MUL, INCLUDE_CURRENT);
		    break;
		case ',':
		    insertToken(TT_MARK, TMK_COMMA, INCLUDE_CURRENT);
		    break;
		case '[':
		    insertToken(TT_MARK, TMK_L_BRACKET, INCLUDE_CURRENT);
		    break;
		case ']':
		    insertToken(TT_MARK, TMK_R_BRACKET, INCLUDE_CURRENT);
		    break;
		case ':':{
		    int last = token_.size() - 1;
		    if (current_token_count_ == 1 && token_[last].type_ == TT_UNDEFINED){
			ret = insertSymbol(token_[last].name_, last);
			if (ret == RC_MULTIPLE_LABEL)
			    return RC_MULTIPLE_LABEL;

			token_[last].type_ = TT_LABEL;
		    }
		    if (current_token_count_ == 1 && token_[last].type_ != TT_LABEL){
			sprintf(g_err_msg, "%s", "Reserved words cannot be used as label");
			return RC_TOKEN_NOT_RECOGNIZED;
		    }
		    insertToken(TT_MARK, TMK_COLON, INCLUDE_CURRENT);
		    break;
		}
		case '?':
		    state = 6;
		    break;
		case '\0':
		    return RC_SUCCESS;
		default:
		    if (!isprint(ch)){
			state = 0;
			break;
		    }
		    sprintf(g_err_msg, "%s'%c'(hex: %s)", "Unrecognized mark: ", ch,
			    int2str(&current_instr_[current_point_], sizeof(UINT8), 1, 0));
		    return RC_LEX_ERR;
		}
	    }
	    break;
	}
	    // string
	case 1:{
	    if (isalnum(ch) || ch == '_')
		state = 1;
	    else if (ch == ':'){ // label or segment
		char *name = getTokenName();
		INT8 return_type = TT_UNDEFINED, return_class = TC_UNDEFINED;

		if (!collideWithReservedWord(name, return_type, return_class)){
		    insertToken(name, TT_LABEL, TC_UNDEFINED);
		    ret = insertSymbol(name, token_.size() - 1);
		    if (ret == RC_MULTIPLE_LABEL)
			return RC_MULTIPLE_LABEL;
		}
		else{
		    if (current_token_count_ == 0){
			sprintf(g_err_msg, "%s", "Reserved words cannot be used as label");
			return RC_TOKEN_NOT_RECOGNIZED;
		    }
		    else if (return_type != TT_SEGMENT && return_type != TT_PREFIX){
			sprintf(g_err_msg, "%s", "Token not recognized");
			return RC_TOKEN_NOT_RECOGNIZED;
		    }
		    else
			insertToken(name, return_type, return_class);
		}

		state = 0;
	    }
	    else if (ch == '('){
		current_point_ += 3;
		char *name = getTokenName();
		bool found = false;

		for (int i = 0; i < FLOAT_REGISTER_NUM; i++){
		    if (strcmp(name, g_float_register[1][i]) == 0){
			insertToken(name, TT_FLOAT_REGISTER, (TOKEN_CLASS_FLOAT_REGISTER)i);
			found = true;
			state = 0;
			break;
		    }
		}

		if (!found){
		    sprintf(g_err_msg, "%s", "Token not recognized");
		    return RC_TOKEN_NOT_RECOGNIZED;
		}
	    }
	    else if (ch == '?'){
		char *name = getTokenName(true);

		if (strcmp(name, "seg?") == 0)
		    insertToken(name, TT_SEGMENT, TSG_SEG, true);
		state = 0;
	    }
	    else{
		char *name = getTokenName();
		state = 0;

		INT8 return_type = TT_UNDEFINED, return_class = TC_UNDEFINED;
		if (collideWithReservedWord(name, return_type, return_class))
		    insertToken(name, return_type, return_class);
		else
		    insertToken(name, TT_UNDEFINED, TC_UNDEFINED);
	    }
	    break;
	}
	    // number
	case 2:{
	    if (isxdigit(ch))
		state = 2;
	    else if (ch == 'h'){
		sprintf(g_err_msg, "%s", "Don't put `0x` and `h` together");
		return RC_TOKEN_NOT_RECOGNIZED;
	    }
	    else if (isalpha(ch)){
		sprintf(g_err_msg, "%s", "Token not recognized");
		return RC_TOKEN_NOT_RECOGNIZED;
	    }
	    else{
		insertToken(TT_HEX, TC_UNDEFINED);
		state = 0;
	    }
	    break;
	}
	    // number
	case 3:{
	    if (isxdigit(ch))
		state = 3;
	    else if (ch == 'h'){
		insertToken(TT_HEX, TC_UNDEFINED, INCLUDE_CURRENT);
		state = 0;
	    }
	    else if (isalpha(ch)){
		sprintf(g_err_msg, "%s", "Token not recognized");
		return RC_TOKEN_NOT_RECOGNIZED;
	    }
	    else{
		insertToken(TT_HEX, TC_UNDEFINED);
		state = 0;
	    }
	    break;
	}
	    // comment
	case 4:{
	    if (ch == '\n' || ch == '\0')
		return RC_SUCCESS;
	    else
		state = 4;
	    break;
	}
	case 5:{
	    if (isalnum(ch) || ch == '-')
		state = 5;
	    else if (ch == '>'){
		char *name = getTokenName(INCLUDE_CURRENT);
		bool found = false;

		for (int i = 0; i < SIZE_NUM; i++){
		    if (strcmp(name, g_size[i]) == 0){
			state = 0;
			found = true;
			insertToken(name, TT_SIZE, (TOKEN_CLASS_SIZE)i, INCLUDE_CURRENT);
			break;
		    }
		}
		if (!found){
		    sprintf(g_err_msg, "%s", "Token not recognized");
		    return RC_TOKEN_NOT_RECOGNIZED;
		}
	    }
	    else{
		sprintf(g_err_msg, "%s", "Missing `>`");
		return RC_LEX_ERR;
	    }
	    break;
	}
	case 6:{
	    if (ch == '?')
		state = 6;
	    else if (ch == '\0'){
		insertToken(TT_UNDEFINED, TC_UNDEFINED);
		state = 0;
	    }
	    else{
		sprintf(g_err_msg, "%s", "Token not recognized");
		return RC_TOKEN_NOT_RECOGNIZED;
	    }
	    break;
	}
	default:
	    sprintf(g_err_msg, "%s", "Lex: UNDEFINED ERR");
	    return RC_LEX_ERR;
	}
	current_point_++;
    }
    return RC_SUCCESS;
}

void Lex::clear(){
    token_.clear();
    symtab_.clear();
    instructions_.clear();
    instr_index_ = 0;
    current_instr_ = NULL;
}

void Lex::pushInstr(char *instruction){
    if (instruction == NULL)
	return;

    int line_number = instructions_.size() + 1;
    INSTRSTR instr = INSTRSTR(line_number, instruction);
    instructions_.push_back(instr);
}

UINT8 Lex::updateUndefined(){
    for (int i = 0; i < (int)token_.size(); i++){
	instrstr_index_ = token_[i].instrstr_index_;
	current_instr_ = instructions_[instrstr_index_].instruction_;
	start_point_ = token_[i].start_point_;
	current_point_ = token_[i].end_point_;

	if (token_[i].type_ == TT_UNDEFINED){
	    if (getSymbolByName(token_[i].name_) != NULL)
		token_[i].type_ = TT_LABEL;
	    else{
		sprintf(g_err_msg, "%s", "Token not recognized");
		return RC_TOKEN_NOT_RECOGNIZED;
	    }
	}
	else if (strcmp(token_[i].name_, "seg?") == 0 || strcmp(token_[i].name_, "???") == 0){
	    sprintf(g_err_msg, "%s", "Token not recognized");
	    return RC_TOKEN_NOT_RECOGNIZED;
	}
    }
    return RC_SUCCESS;
}

void Lex::tokenize(){
    UINT8 ret;
    if (instructions_.size() <= 0){
	sprintf(g_err_msg, "Lexical: Please provide at least one assembly instruction");
	errorAndExit(RC_LEX_EMPTY);
    }

    for (int i  = 0; i < (int)instructions_.size(); i++){
	if (strcmp(instructions_[i].instruction_, "\0") == 0)
	    continue;

	g_mnemonic_index = -1;
	instrstr_index_ = i;
	current_instr_ = instructions_[i].instruction_;

	ret = scanInstruction();
	if (ret != RC_SUCCESS)
	    errorAndExit(ret);

	int last = token_.size() - 1;
	if (token_.size() >0 && !(token_.size() >= 2 && token_[last].type_ == TT_MARK && token_[last].class_ == TMK_COLON &&
				  token_[last - 1].type_ == TT_LABEL && token_[last].instr_index_ == token_[last - 1].instr_index_) &&
	    !(token_[last].instr_index_ < instr_index_))
	    instr_index_++;

	instructions_[i].mnemonic_index_ = g_mnemonic_index;
    }

    ret = updateUndefined();
    if (ret != RC_SUCCESS)
	errorAndExit(ret);
}

void Lex::errorAndExit(UINT8 err_type){
    if (err_type != RC_LEX_EMPTY){
	printf("In line %d:\n", instructions_[instrstr_index_].line_number_);

	if (current_instr_ != NULL)
	    printf("%s\n", current_instr_);

	for (int i = 0; i < start_point_; i++){
	    if (current_instr_[i] == '\t')
		printf("\t");
	    else
		printf(" ");
	}
	for (int i = start_point_; i < current_point_; i++){
	    if (err_type == RC_TOKEN_NOT_RECOGNIZED || err_type == RC_MULTIPLE_LABEL)
		printf("^");
	    else
		printf(" ");
	}
	if (current_instr_[current_point_] == ' ' || current_instr_[current_point_] == '\0' || !isalpha(current_instr_[current_point_]))
	    printf("  ");
	else
	    printf("^  ");
    }

    displayAndExit(g_err_msg);
}

void Lex::printToken(){
    int instr_index = 0, i = 0, index = 0;

    while (instr_index <= token_[token_.size() - 1].instr_index_){
	std::cout << instr_index + 1 << ". ";
	for (; i < (int)token_.size() && token_[i].instr_index_ == instr_index; i++)
	    std::cout << token_[i].name_ << " ";
	std::cout << std::endl;

	for (; index < (int)token_.size() && token_[index].instr_index_ == instr_index; index++){
	    std::cout << std::left << std::setw(25) << token_[index].name_<< " : ";

	    switch(token_[index].type_){
	    case TT_HEX:
		std::cout << "hex " << std::endl;
		break;
	    case TT_LABEL:
		std::cout << "label " << std::endl;
		break;
	    case TT_PREFIX:
		std::cout << "prefix " << std::endl;
		break;
	    case TT_MNEMONIC:
		std::cout << "mnemonic " << std::endl;
		break;
	    case TT_SIZE:
		std::cout << "size " << std::endl;
		break;
	    case TT_SPECIAL:
		std::cout << "special " << std::endl;
		break;
	    case TT_SEGMENT:
		std::cout << "segment " << std::endl;
		break;
	    case TT_GENERAL_REGISTER:
		std::cout << "general register " << std::endl;
		break;
	    case TT_FLOAT_REGISTER:
		std::cout << "float register " << std::endl;
		break;
	    case TT_SSEMMX_REGISTER:
		std::cout << "sse/mxx register " << std::endl;
		break;
	    case TT_CR:
		std::cout << "control register " << std::endl;
		break;
	    case TT_DR:
		std::cout << "debug register " << std::endl;
		break;
	    case TT_MARK:
		switch(token_[index].class_){
		case TMK_ADD:
		    std::cout << "add " << std::endl;
		    break;
		case TMK_SUB:
		    std::cout << "sub " << std::endl;
		    break;
		case TMK_MUL:
		    std::cout << "mul " << std::endl;
		    break;
		case TMK_COMMA:
		    std::cout << "comma " << std::endl;
		    break;
		case TMK_L_BRACKET:
		    std::cout << "left bracket " << std::endl;
		    break;
		case TMK_R_BRACKET:
		    std::cout << "right bracket " << std::endl;
		    break;
		case TMK_COLON:
		    std::cout << "colon " << std::endl;
		    break;
		case TMK_DOLLAR:
		    std::cout << "dollar " << std::endl;
		    break;
		default:
		    std::cout << "I don't know what the hell is this " << std::endl;
		    break;
		}
		break;
	    case TT_COMMENT:
		std::cout << "comment " << std::endl;
		break;
	    default:
		std::cout << "undefined" << std::endl;
	    }
	}
	instr_index++;
    }
}

void Lex::printSymbolTable(){
    for (int index = 0; index < (int)symtab_.size(); index++)
	std::cout << index << ". " << symtab_[index].name_ << " (line " << token_[symtab_[index].token_index_].instrstr_index_  + 1<< ")" << std::endl;
}

void Lex::printInstr(){
    for (int index = 0; index < (int)instructions_.size(); index++)
	std::cout << "Line " << instructions_[index].line_number_ << ": \"" << instructions_[index].instruction_ << "\", mnemonic index is " << instructions_[index].mnemonic_index_ << std::endl;
}

/* ----------- Parser ----------- */
Parser::Parser(){
    clear();
}

void Parser::init(std::vector<TOKEN> *token, std::vector<SYMBOL> *symtab, std::vector<INSTRSTR> *instructions){
    if (token == NULL || token->size() <= 0 || instructions == NULL || instructions->size() <= 0){
	sprintf(g_err_msg, "Syntax: Please provide enough token or instructions");
	errorAndExit(RC_PARSER_EMPTY, current_point_);
    }

    token_ = token;
    symtab_ = symtab;
    instructions_ = instructions;
    current_point_ = 0;
}

void Parser::clear(){
    token_ = NULL;
    symtab_ = NULL;
    instructions_ = NULL;
    current_point_ = 0;
    instr_index_ = 0;
    asmoperand_ = NULL;
}

void Parser::setAsmOperand(INT8 type, INT32 operand, int size){
    asmoperand_->type_ = type;
    asmoperand_->operand_ = operand;
    asmoperand_->operand_size_ = size;
    asmoperand_->displacement_size_ = -1;
    asmoperand_->segment_ = -1;
    asmoperand_->scale_ = -1;
    asmoperand_->index_ = -1;
    asmoperand_->base_ = -1;
}

std::vector<ASMINSTRUCTION>* Parser::getAsmInstr(){
    return &asminstrs_;
}

int Parser::getSymbolIndex(char *name){
    for (int i = 0; i < (int)symtab_->size(); i++){
	if (strcmp(name, symtab_->at(i).name_) == 0)
	    return i;
    }
    return -1;
}

bool Parser::matchToken(INT8 token_type, INT8 token_class, bool move_to_next, bool need, bool save){
    bool result = false;
    int operand;

    if (current_point_ >= (int)token_->size())
	result = false;
    else if (token_->at(current_point_).instr_index_ != instr_index_)
	result = false;
    else if (token_->at(current_point_).type_ == token_type && (token_->at(current_point_).class_ == token_class || token_class == TC_ANY))
	result = true;

    if (result && save){
	if (token_type == TT_PREFIX){
	    if (asminstr_.lockAndRepeat_ != -1){
		sprintf(g_err_msg, "Syntax: Multiple prefix: %s, %s", token_->at(current_point_ - 1).name_, token_->at(current_point_).name_);
		errorAndExit(RC_PARSER_MULTIPLE_PREFIX, current_point_);
	    }
	    switch(token_->at(current_point_).class_){
	    case TPX_LOCK:
		asminstr_.lockAndRepeat_ = PREFIX_LOCK;
		break;
	    case TPX_REP: case TPX_REPE: case TPX_REPZ:
		asminstr_.lockAndRepeat_ = PREFIX_REP;
		break;
	    case TPX_REPN: case TPX_REPNE: case TPX_REPNZ:
		asminstr_.lockAndRepeat_ = PREFIX_REPN;
		break;
	    default:
		sprintf(g_err_msg, "Syntax: Undefined error, lock & repeat prefix expected");
		errorAndExit(RC_PARSER_ERR, current_point_);
	    }
	}
	else if (token_type == TT_GENERAL_REGISTER && asmoperand_ != NULL){
	    operand = token_->at(current_point_).class_ % 8;

	    switch(token_->at(current_point_).class_ / 8){
	    case 0:
		setAsmOperand(OPERAND_REGISTER, (REGISTER_FOR_BYTE)operand, SIZE_BYTE);
		break;
	    case 1:
		setAsmOperand(OPERAND_REGISTER, (REGISTER_FOR_BYTE)operand, SIZE_WORD);
		break;
	    case 2:
		setAsmOperand(OPERAND_REGISTER, (REGISTER_FOR_BYTE)operand, SIZE_DWORD);
		break;
	    default:
		sprintf(g_err_msg, "Syntax: Undefined error, general register expected");
		errorAndExit(RC_PARSER_ERR, current_point_);
	    }
	}
	else if (token_type == TT_FLOAT_REGISTER && asmoperand_ != NULL){
	    operand = token_->at(current_point_).class_ % 8;
	    setAsmOperand(OPERAND_FLOAT, (REGISTER_FOR_FLOAT)operand, -1);
	}
	else if (token_type == TT_SSEMMX_REGISTER && asmoperand_ != NULL){
	    operand = token_->at(current_point_).class_ % 8;

	    switch(token_->at(current_point_).class_ / 8){
	    case 0:
		setAsmOperand(OPERAND_MMX_REGISTER, (REGISTER_FOR_MMX)operand, -1);
		break;
	    case 1:
		setAsmOperand(OPERAND_SSE_REGISTER, (REGISTER_FOR_SSE)operand, -1);
		break;
	    default:
		sprintf(g_err_msg, "Syntax: Undefined error, sse/mmx register expected");
		errorAndExit(RC_PARSER_ERR, current_point_);
	    }
	}
	else if (token_type == TT_LABEL && asmoperand_ != NULL){
	    if (getSymbolIndex(token_->at(current_point_).name_) == -1){
		sprintf(g_err_msg, "Syntax: Label not found");
		errorAndExit(RC_PARSER_ERR, current_point_);
	    }
	    setAsmOperand(OPERAND_LABEL, symtab_->at(getSymbolIndex(token_->at(current_point_).name_)).instr_index_, -1);
	}
	else if (token_type == TT_CR && asmoperand_ != NULL)
	    setAsmOperand(OPERAND_CR, token_->at(current_point_).class_, -1);
	else if (token_type == TT_DR && asmoperand_ != NULL)
	    setAsmOperand(OPERAND_DR, token_->at(current_point_).class_, -1);
	else{
	    sprintf(g_err_msg, "SAVE OPERAND FAILED");
	    errorAndExit(RC_PARSER_EMPTY, current_point_);
	}
    }

    if (result && move_to_next)
	current_point_++;
    else if (!result && need){
	switch(token_type){
	case TT_HEX:
	    sprintf(g_err_msg, "Syntax: Hex expected");
	    break;
	case TT_LABEL:
	    sprintf(g_err_msg, "Syntax: Label expected");
	    break;
	case TT_PREFIX:
	    sprintf(g_err_msg, "Syntax: Prefix expected");
	    break;
	case TT_MNEMONIC:
	    sprintf(g_err_msg, "Syntax: Mnemonic expected");
	    break;
	case TT_SIZE:
	    sprintf(g_err_msg, "Syntax: Size expected");
	    break;
	case TT_SEGMENT:
	    sprintf(g_err_msg, "Syntax: Segment expected");
	    break;
	case TT_GENERAL_REGISTER:
	    sprintf(g_err_msg, "Syntax: General register expected");
	    break;
	case TT_FLOAT_REGISTER:
	    sprintf(g_err_msg, "Syntax: Float point register expected");
	    break;
	case TT_SSEMMX_REGISTER:
	    sprintf(g_err_msg, "Syntax: Sse/mmx register expected");
	    break;
	case TT_MARK:{
	    switch(token_class){
	    case TMK_ADD:
		sprintf(g_err_msg, "Syntax: `+` expected");
		break;
	    case TMK_SUB:
		sprintf(g_err_msg, "Syntax: `-` expected");
		break;
	    case TMK_MUL:
		sprintf(g_err_msg, "Syntax: `*` expected");
		break;
	    case TMK_COLON:
		sprintf(g_err_msg, "Syntax: `:` expected");
		break;
	    case TMK_COMMA:
		sprintf(g_err_msg, "Syntax: `,` expected");
		break;
	    case TMK_L_BRACKET:
		sprintf(g_err_msg, "Syntax: `[` expected");
		break;
	    case TMK_R_BRACKET:
		sprintf(g_err_msg, "Syntax: `]` expected");
		break;
	    default:
		sprintf(g_err_msg, "Syntax undefine error: Mark expected");
		break;
	    }
	    break;
	}
	default:
	    sprintf(g_err_msg, "Syntax: undefined err ");
	    break;
	}
	errorAndExit(RC_PARSER_EXPECTED_WRONG, current_point_);
    }

    return result;
}

void Parser::parse(){
    if (token_ == NULL || token_->size() <= 0 || instructions_ == NULL || instructions_->size() <= 0){
	sprintf(g_err_msg, "Syntax: Please call `init` function first");
	errorAndExit(RC_PARSER_EMPTY, current_point_);
    }

    while(true){
	if (current_point_ >= (int)token_->size())
	    break;

	instr_index_ = token_->at(current_point_).instr_index_;
	clearAsmInstr(&asminstr_);

	parseAssembly();

	if (current_point_ < int(token_->size()) && token_->at(current_point_).instr_index_ == instr_index_){
	    sprintf(g_err_msg, "Syntax: parse incomplete: %s", token_->at(current_point_).name_);
	    errorAndExit(RC_PARSER_ERR, current_point_);
	}

	asminstr_.instrstr_index_ = token_->at(current_point_ - 1).instrstr_index_;
	asminstrs_.push_back(asminstr_);
    }
}

void Parser::parseAssembly(){
    // label ?
    while (matchToken(TT_LABEL, TC_UNDEFINED, MOVE_TO_NEXT))
	matchToken(TT_MARK, TMK_COLON, MOVE_TO_NEXT, NEEDED);

    // prefix ?
    while (matchToken(TT_PREFIX, TC_ANY, MOVE_TO_NEXT, NOT_NEED, SAVE_PREFIX)){}

    parseInstruction();
}

void Parser::parseInstruction(){
    matchToken(TT_MNEMONIC, TC_UNDEFINED, MOVE_TO_NEXT, NEEDED);
    char* mnemonic = (char*)malloc(strlen(token_->at(current_point_ - 1).name_));
    strcpy(mnemonic, token_->at(current_point_ - 1).name_);
    asminstr_.mnemonic_ = mnemonic;

    if (parseOperand()){
	while (matchToken(TT_MARK, TMK_COMMA, MOVE_TO_NEXT)){
	    if (!parseOperand()){
		sprintf(g_err_msg, "Syntax: Operand needed");
		errorAndExit(RC_PARSER_ERR, current_point_);
	    }
	}
    }
}

bool Parser::parseOperand(){
    asmoperand_ = new ASMOPERAND();
    clearAsmOperand(asmoperand_);
    bool isOperand = false;
    int size;
    UINT32 data;

    if (current_point_ >= (int)token_->size()){}
    else if (token_->at(current_point_).instr_index_ != instr_index_){}
    else if (matchToken(TT_GENERAL_REGISTER, TC_ANY, MOVE_TO_NEXT, NOT_NEED, SAVE_OPERAND))
	isOperand = true;
    else if (matchToken(TT_FLOAT_REGISTER, TC_ANY, MOVE_TO_NEXT, NOT_NEED, SAVE_OPERAND))
	isOperand = true;
    else if (matchToken(TT_SEGMENT, TC_ANY, MOVE_TO_NEXT, NOT_NEED)){
	if (matchToken(TT_MARK, TMK_COLON))
	    current_point_--;
	else{
	    setAsmOperand(OPERAND_SEGMENT, token_->at(current_point_ - 1).class_, -1);
	    isOperand = true;
	}
    }
    else if (matchToken(TT_SSEMMX_REGISTER, TC_ANY, MOVE_TO_NEXT, NOT_NEED, SAVE_OPERAND))
	isOperand = true;
    else if (matchToken(TT_CR, TC_ANY, MOVE_TO_NEXT, NOT_NEED, SAVE_OPERAND))
	isOperand = true;
    else if (matchToken(TT_DR, TC_ANY, MOVE_TO_NEXT, NOT_NEED, SAVE_OPERAND))
	isOperand = true;
    else if (matchToken(TT_HEX, TC_ANY, MOVE_TO_NEXT)){
	if (matchToken(TT_MARK, TMK_COLON, MOVE_TO_NEXT)){
	    matchToken(TT_HEX, TC_ANY, MOVE_TO_NEXT, NEEDED);

	    data = string2bin(token_->at(current_point_ - 1).name_, size);
	    setAsmOperand(OPERAND_FLOW, data, ABSOLUTE_ADDRESS_FAR_DWORD);
	    data = string2bin(token_->at(current_point_ - 3).name_, size);
	    if (size == SIZE_DWORD){
		sprintf(g_err_msg, "Syntax: Far address invalid");
		errorAndExit(RC_PARSER_OPERAND_ERR, current_point_);
	    }
	    asmoperand_->displacement_ = data;
	    asmoperand_->displacement_size_ = size;
	}
	else{
	    data = string2bin(token_->at(current_point_ - 1).name_, size);
	    setAsmOperand(OPERAND_IMMEDIATE, data, (OPERAND_SIZE)size);

	    if ((size == SIZE_WORD && (data & 0xff00)) || (size == SIZE_DWORD && (data & 0xffffff00)))
		asmoperand_->signExtended_ = true;
	}
	isOperand = true;
    }
    else if (parseNum(data, size)){
	setAsmOperand(OPERAND_IMMEDIATE, data, (OPERAND_SIZE)size);

	if (token_->at(current_point_ - 2).type_ == TT_MARK && token_->at(current_point_ - 2).class_ == TMK_SUB)
	    asmoperand_->signExtended_ = true;
	else if ((size == SIZE_WORD && (data & 0xff00)) || (size == SIZE_DWORD && (data & 0xffffff00)))
	    asmoperand_->signExtended_ = true;

	isOperand = true;
    }
    else if (matchToken(TT_LABEL, TC_ANY, MOVE_TO_NEXT, NOT_NEED, SAVE_OPERAND))
	isOperand = true;
    else if (matchToken(TT_SIZE, TSZ_SHORT, MOVE_TO_NEXT) && matchToken(TT_HEX, TC_ANY, MOVE_TO_NEXT, NEEDED)){
	data = string2bin(token_->at(current_point_ - 1).name_, size);
	setAsmOperand(OPERAND_FLOW, data, RELATIVE_ADDRESS_SHORT);
	isOperand = true;
    }
    else if (matchToken(TT_SIZE, TSZ_NEAR, MOVE_TO_NEXT)){
	if (matchToken(TT_HEX, TC_ANY, MOVE_TO_NEXT)){
	    data = string2bin(token_->at(current_point_ - 1).name_, size);
	    setAsmOperand(OPERAND_FLOW, data, RELATIVE_ADDRESS_NEAR_DWORD);
	    isOperand = true;
	}
	else if (matchToken(TT_GENERAL_REGISTER, TC_ANY, MOVE_TO_NEXT, NOT_NEED, SAVE_OPERAND))
	    isOperand = true;
	else
	    current_point_--;
    }
    else if (matchToken(TT_SIZE, TSZ_LONG, MOVE_TO_NEXT) && matchToken(TT_HEX, TC_ANY, MOVE_TO_NEXT, NEEDED)){
	data = string2bin(token_->at(current_point_ - 1).name_, size);
	setAsmOperand(OPERAND_FLOW, data, RELATIVE_ADDRESS_NEAR_DWORD);
	isOperand = true;
    }
    else if (matchToken(TT_SIZE, TSZ_FAR, MOVE_TO_NEXT)){
	if (matchToken(TT_HEX, TC_ANY, MOVE_TO_NEXT)){
	    matchToken(TT_MARK, TMK_COLON, MOVE_TO_NEXT, NEEDED);
	    matchToken(TT_HEX, TC_ANY, MOVE_TO_NEXT, NEEDED);

	    data = string2bin(token_->at(current_point_ - 1).name_, size);
	    setAsmOperand(OPERAND_FLOW, data, ABSOLUTE_ADDRESS_FAR_DWORD);
	    data = string2bin(token_->at(current_point_ - 3).name_, size);
	    if (size == SIZE_DWORD){
		sprintf(g_err_msg, "Syntax: Far address invalid");
		errorAndExit(RC_PARSER_OPERAND_ERR, current_point_);
	    }
	    asmoperand_->displacement_ = data;
	    asmoperand_->displacement_size_ = size;
	    isOperand = true;
	}
	else if (matchToken(TT_GENERAL_REGISTER, TC_ANY, MOVE_TO_NEXT, NOT_NEED, SAVE_OPERAND))
	    isOperand = true;
	else
	    current_point_--;
    }
    if (!isOperand && parseMemory())
	isOperand = true;

    if (!isOperand){
	delete asmoperand_;
	asmoperand_ = NULL;
    }
    else if (asminstr_.dest_ == NULL)
	asminstr_.dest_ = asmoperand_;
    else if (asminstr_.src1_ == NULL)
	asminstr_.src1_ = asmoperand_;
    else if (asminstr_.src2_ == NULL)
	asminstr_.src2_ = asmoperand_;
    else if (asminstr_.src3_ == NULL)
	asminstr_.src3_ = asmoperand_;
    else{
	sprintf(g_err_msg, "Syntax: Too many operand");
	errorAndExit(RC_PARSER_TOO_MANY_OPERAND, current_point_);
    }

    if (isOperand){
	switch(asmoperand_->operand_size_){
	case TSZ_BYTE:
	    asmoperand_->operand_size_ = SIZE_BYTE;
	    break;
	case TSZ_WORD:
	    asmoperand_->operand_size_ = SIZE_WORD;
	    break;
	case TSZ_DWORD:
	    asmoperand_->operand_size_ = SIZE_DWORD;
	    break;
	case TSZ_FWORD:
	    asmoperand_->operand_size_ = SIZE_FWORD;
	    break;
	case TSZ_QWORD:
	    asmoperand_->operand_size_ = SIZE_QWORD;
	    break;
	case TSZ_DQWORD:
	    asmoperand_->operand_size_ = SIZE_DQWORD;
	    break;
	case TSZ_TBYTE:
	    asmoperand_->operand_size_ = SIZE_TBYTE;
	    break;
	case TSZ_14BYTE:
	    asmoperand_->operand_size_ = SIZE_14BYTE;
	    break;
	case TSZ_28BYTE:
	    asmoperand_->operand_size_ = SIZE_28BYTE;
	    break;
	case TSZ_94BYTE:
	    asmoperand_->operand_size_ = SIZE_94BYTE;
	    break;
	case TSZ_108BYTE:
	    asmoperand_->operand_size_ = SIZE_108BYTE;
	    break;
	case TSZ_512BYTE:
	    asmoperand_->operand_size_ = SIZE_512BYTE;
	    break;
	default:
	    break;
	}
    }

    return isOperand;
}

bool Parser::parseMemory(){
    bool isMemory = false;

    if (matchToken(TT_SIZE, TC_ANY, MOVE_TO_NEXT)){
	if (token_->at(current_point_ - 1).class_ == TSZ_FAR || token_->at(current_point_ - 1).class_ == TSZ_NEAR)
	    matchToken(TT_SIZE, TC_ANY, MOVE_TO_NEXT, NEEDED);

	asmoperand_->operand_size_ = token_->at(current_point_ - 1).class_;

	// match `ptr`, if exists
	matchToken(TT_SPECIAL, TSP_PTR, MOVE_TO_NEXT);

	// match segment
	if (matchToken(TT_SEGMENT, TC_ANY, MOVE_TO_NEXT)){
	    matchToken(TT_MARK, TMK_COLON, MOVE_TO_NEXT, NEEDED);
	    asmoperand_->segment_ = token_->at(current_point_ - 2).class_;
	}

	matchToken(TT_MARK, TMK_L_BRACKET, MOVE_TO_NEXT, NEEDED);
	if (!parseAddressing()){
	    sprintf(g_err_msg, "Invalid memory format");
	    errorAndExit(RC_PARSER_ERR, current_point_);
	}
	matchToken(TT_MARK, TMK_R_BRACKET, MOVE_TO_NEXT, NEEDED);
	isMemory = true;
    }
    else if (matchToken(TT_SEGMENT, TC_ANY, MOVE_TO_NEXT)){
	matchToken(TT_MARK, TMK_COLON, MOVE_TO_NEXT, NEEDED);
	asmoperand_->segment_ = token_->at(current_point_ - 2).class_;
	asmoperand_->operand_size_ = SIZE_TO_BE_DECODE;

	matchToken(TT_MARK, TMK_L_BRACKET, MOVE_TO_NEXT, NEEDED);
	if (!parseAddressing()){
	    sprintf(g_err_msg, "Invalid memory format");
	    errorAndExit(RC_PARSER_ERR, current_point_);
	}
	matchToken(TT_MARK, TMK_R_BRACKET, MOVE_TO_NEXT, NEEDED);
	isMemory = true;
    }
    else if (matchToken(TT_MARK, TMK_L_BRACKET, MOVE_TO_NEXT)){
	asmoperand_->operand_size_ = SIZE_TO_BE_DECODE;

	if (!parseAddressing()){
	    sprintf(g_err_msg, "Invalid memory format");
	    errorAndExit(RC_PARSER_ERR, current_point_);
	}
	matchToken(TT_MARK, TMK_R_BRACKET, MOVE_TO_NEXT, NEEDED);
	isMemory = true;
    }
    if (isMemory)
	asmoperand_->type_ = OPERAND_MEMORY;

    return isMemory;
}

bool Parser::parseAddressing(){
    // expect type
#define EXPECT_REG     1
#define EXPECT_HEX     2
#define EXPECT_NUM     4
#define EXPECT_MARK    8
    // last token store to ?
#define TO_SCALE       0
#define TO_REG1        1
#define TO_REG2        2
#define TO_DISPLACEMENT 3
#define TO_NOWHERE     4
    // last operation
#define OPERATION_SUB  0
#define OPERATION_ADD  1
#define OPERATION_MUL  2
#define OPERATION_NONE 3

    bool isAddressing = false;
    INT8 scale = 0, index = -1, base = -1, expect_type = EXPECT_REG | EXPECT_NUM, last_to = TO_NOWHERE, last_op = OPERATION_NONE,
	reg1 = -1, reg2 = -1, reg1_scale = 0, reg2_scale = 0;
    UINT32 displacement = 0, data;
    int displacement_size = -1, size = -1, addressing_size = -1, index_size = -1, base_size = -1, reg1_size = -1, reg2_size = -1;

    while (!matchToken(TT_MARK, TMK_R_BRACKET)){
	bool satisfaction = false;

	if (expect_type & EXPECT_NUM){
	    if (parseNum(data, size) && size != -1){
		displacement += data;
		last_to = TO_DISPLACEMENT;
		expect_type = EXPECT_MARK;
		satisfaction = true;
	    }
	    else if (expect_type == EXPECT_NUM){
		sprintf(g_err_msg, "Syntax: Num expected");
		errorAndExit(RC_PARSER_EXPECTED_WRONG, current_point_);
	    }
	}
	if (expect_type & EXPECT_REG){
	    if (matchToken(TT_GENERAL_REGISTER, TC_ANY, MOVE_TO_NEXT)){
		if (last_op == OPERATION_SUB){
		    sprintf(g_err_msg, "Syntax: Do not support substract register");
		    errorAndExit(RC_PARSER_ERR, current_point_ - 1);
		}

		data = token_->at(current_point_ - 1).class_ % 8;
		size = token_->at(current_point_ - 1).class_ / 8 + SIZE_BYTE;

		if (reg1 == -1 || (reg1 != -1 && size == reg1_size && (INT8)data == reg1)){
		    reg1 = data;
		    reg1_size = size;
		    reg1_scale++;
		    last_to = TO_REG1;
		}
		else if (reg2 == -1 || (reg2 != -1 && size == reg2_size && (INT8)data == reg2)){
		    reg2 = data;
		    reg2_size = size;
		    reg2_scale++;
		    last_to = TO_REG2;
		}
		else{
		    sprintf(g_err_msg, "Syntax: More than 2 registers/multiple SIB structure");
		    errorAndExit(RC_PARSER_ADDRESSING_ERR, current_point_);
		}
		satisfaction = true;
		expect_type = EXPECT_MARK;
	    }
	    else if (expect_type == EXPECT_REG){
		sprintf(g_err_msg, "Syntax: Register expected");
		errorAndExit(RC_PARSER_EXPECTED_WRONG, current_point_);
	    }
	}
	if (expect_type & EXPECT_HEX){
	    if (parseNum(data, size, IS_POSITIVE) && size != -1){
		if (last_op == OPERATION_SUB)
		    displacement -= data;
		else
		    displacement += data;
		last_to = TO_DISPLACEMENT;
		expect_type = EXPECT_MARK;
		satisfaction = true;
	    }
	    else if (expect_type == EXPECT_HEX){
		sprintf(g_err_msg, "Syntax: Hex expected");
		errorAndExit(RC_PARSER_EXPECTED_WRONG, current_point_);
	    }
	}
	if (expect_type & EXPECT_MARK){
	    if (matchToken(TT_MARK, TMK_ADD, MOVE_TO_NEXT)){
		expect_type = EXPECT_REG | EXPECT_HEX;
		last_op = OPERATION_ADD;
		satisfaction = true;
	    }
	    else if (matchToken(TT_MARK, TMK_SUB, MOVE_TO_NEXT)){
		expect_type = EXPECT_HEX;
		last_op = OPERATION_SUB;
		satisfaction = true;
	    }
	    else if (matchToken(TT_MARK, TMK_MUL, MOVE_TO_NEXT)){
		if (last_op == OPERATION_SUB || last_op == OPERATION_MUL){
		    sprintf(g_err_msg, "Syntax: Operation substract not support for SIB structure");
		    errorAndExit(RC_PARSER_EXPECTED_WRONG, current_point_ - 3);
		}

		if (last_to == TO_DISPLACEMENT){
		    if ((INT32)data < 0){
			sprintf(g_err_msg, "Syntax: Invalid scale");
			errorAndExit(RC_PARSER_ERR, current_point_ - 2);
		    }

		    displacement -= data;
		    matchToken(TT_GENERAL_REGISTER, TC_ANY, MOVE_TO_NEXT, NEEDED);

		    if (reg1 == -1 || (reg1 != -1 && (reg1 == token_->at(current_point_ - 1).class_  % 8) &&
				       (reg1_size == token_->at(current_point_ - 1).class_  / 8))){
			reg1 = token_->at(current_point_ - 1).class_  % 8;
			reg1_size = token_->at(current_point_ - 1).class_  / 8;
			reg1_scale += data;
			last_to = TO_REG1;
		    }
		    else if (reg2 == -1 || (reg2 != -1 && (reg2 == token_->at(current_point_ - 1).class_  % 8) &&
					    (reg2_size == token_->at(current_point_ - 1).class_  / 8))){
			reg2 = token_->at(current_point_ - 1).class_ % 8;
			reg2_size = token_->at(current_point_ - 1).class_  / 8;
			reg2_scale += data;
			last_to = TO_REG2;
		    }
		    else{
			sprintf(g_err_msg, "Syntax: Operation not support for SIB structure");
			errorAndExit(RC_PARSER_EXPECTED_WRONG, current_point_);
		    }
		}
		else if (last_to == TO_REG1){
		    if (parseNum(data, size, IS_POSITIVE) && size != -1){
			if (data > 10){
			    sprintf(g_err_msg, "Syntax: Invalid scale");
			    errorAndExit(RC_PARSER_EXPECTED_WRONG, current_point_ - 1);
			}
			reg1_scale += data - 1;
			last_to = TO_SCALE;
			expect_type = EXPECT_MARK;
		    }
		    else{
			sprintf(g_err_msg, "Syntax: Hex (positive) expected");
			errorAndExit(RC_PARSER_EXPECTED_WRONG, current_point_ - 1);
		    }
		}
		else if (last_to == TO_REG2){
		    if (parseNum(data, size, IS_POSITIVE) && size != -1){
			if (data > 10){
			    sprintf(g_err_msg, "Syntax: Invalid scale");
			    errorAndExit(RC_PARSER_EXPECTED_WRONG, current_point_ - 1);
			}
			reg2_scale += data - 1;
			last_to = TO_SCALE;
			expect_type = EXPECT_MARK;
		    }
		    else{
			sprintf(g_err_msg, "Syntax: Hex (positive) expected");
			errorAndExit(RC_PARSER_EXPECTED_WRONG, current_point_ - 1);
		    }
		}

		last_op = OPERATION_MUL;
		expect_type = EXPECT_MARK;
		satisfaction = true;
	    }
	    else if (!matchToken(TT_MARK, TMK_R_BRACKET)){
		sprintf(g_err_msg, "Syntax: `+` or `-` or `*` or `]` expected");
		errorAndExit(RC_PARSER_EXPECTED_WRONG, current_point_);
	    }
	}

	if (reg1_scale == 0){
	    reg1 = -1;
	    reg1_size = -1;
	}
	if (reg2_scale == 0){
	    reg2 = -1;
	    reg2_size = -1;
	}

	isAddressing = (expect_type == EXPECT_MARK ? true : false);

	if (!satisfaction){
	    sprintf(g_err_msg, "Syntax: Hex or register expected");
	    errorAndExit(RC_PARSER_EXPECTED_WRONG, current_point_);
	}
    }

    // be careful for `xlat [ebx + al]`
    if (reg1_scale > 1 && reg2_scale > 1){
	sprintf(g_err_msg, "Syntax: Multiple SIB structure");
	errorAndExit(RC_PARSER_ADDRESSING_ERR, current_point_);
    }
    // for `xlat [ebx + al]`
    else if (reg1_scale == 1 && reg2_scale == 1){
	if ((reg1_size == SIZE_DWORD && reg1 == EBX && reg2_size == SIZE_BYTE && reg2 == AL) ||
	    (reg2_size == SIZE_DWORD && reg2 == EBX && reg1_size == SIZE_BYTE && reg1 == AL)){
	    if (displacement != 0){
		sprintf(g_err_msg, "Syntax: Addressing size not match");
		errorAndExit(RC_PARSER_ADDRESSING_ERR, current_point_);
	    }
	    base = EBX_PLUS_AL;
	    base_size = SIZE_DWORD;
	}
	else if ((reg1_size == SIZE_WORD && reg1 == BX && reg2_size == SIZE_BYTE && reg2 == AL) ||
		 (reg2_size == SIZE_WORD && reg2 == BX && reg1_size == SIZE_BYTE && reg1 == AL)){
	    if (displacement != 0){
		sprintf(g_err_msg, "Syntax: Addressing size not match");
		errorAndExit(RC_PARSER_ADDRESSING_ERR, current_point_);
	    }
	    base = BX_PLUS_AL;
	    base_size = SIZE_WORD;
	}
	else{
	    base = reg1;
	    base_size = reg1_size;
	    scale = reg2_scale;
	    index = reg2;
	    index_size = reg2_size;
	}
    }
    else if (reg1_scale == 1 || reg2_scale > 1){
	base = reg1;
	base_size = reg1_size;
	scale = reg2_scale;
	index = reg2;
	index_size = reg2_size;
    }
    else if (reg2_scale == 1 || reg1_scale > 1){
	base = reg2;
	base_size = reg2_size;
	scale = reg1_scale;
	index = reg1;
	index_size = reg1_size;
    }

    if (base == -1 && (scale == 2 || scale == 3 || scale == 5 || scale == 9)){
	scale--;
	base = index;
    }

    if (scale != 0 && scale != 1 && scale != 2 && scale != 4 && scale != 8){
	sprintf(g_err_msg, "Syntax: Invalid scale");
	errorAndExit(RC_PARSER_ADDRESSING_ERR, current_point_);
    }

    if (displacement == 0)
	displacement_size = -1;
    else if (abs(displacement) > 0xffff)
	displacement_size = SIZE_DWORD;
    else if (abs(displacement) > 0xff)
	displacement_size = SIZE_WORD;
    else
	displacement_size = SIZE_BYTE;

    // addressing size
    if (addressing_size == -1){
	bool addressing_ok = true;

	if (base_size != -1)
	    addressing_size = base_size;
	if (index_size != -1){
	    if (addressing_size != -1 && index_size != addressing_size){
		sprintf(g_err_msg, "Syntax: Addressing size not match, index exceeds");
		addressing_ok = false;
	    }
	    else if (addressing_size == -1)
		addressing_size = index_size;
	}
	if (displacement_size != -1){
	    if (addressing_size != -1 && displacement_size > addressing_size){
		sprintf(g_err_msg, "Syntax: Addressing size not match, displacement exceeds");
		addressing_ok = false;
	    }
	    else if (addressing_size == -1)
		addressing_size = displacement_size;
	}

	if (!addressing_ok)
	    errorAndExit(RC_PARSER_ADDRESSING_ERR, current_point_);
    }

    if (scale != 0 && index == ESP){
	if (scale == 1 && base != ESP){
	    index = base;
	    base = ESP;
	}
	else{
	    sprintf(g_err_msg, "Syntax: Invalid indexing(esp can't be uses as index in SIB)");
	    errorAndExit(RC_PARSER_ADDRESSING_ERR, current_point_);
	}
    }

    if (addressing_size == SIZE_WORD){
	if ((base == BX && index == SI && scale == 1) || (base == SI && index == BX && scale == 1)){
	    base = BX_PLUS_SI;
	    index = -1;
	    scale = -1;
	}
	else if ((base == BX && index == DI && scale == 1) || (base == DI && index == BX && scale == 1)){
	    base = BX_PLUS_DI;
	    index = -1;
	    scale = -1;
	}
	else if ((base == BP && index == SI && scale == 1) || (base == SI && index == BP && scale == 1)){
	    base = BP_PLUS_SI;
	    index = -1;
	    scale = -1;
	}
	else if ((base == BP && index == DI && scale == 1) || (base == DI && index == BP && scale == 1)){
	    base = BP_PLUS_DI;
	    index = - 1;
	    scale = -1;
	}
	else if ((base == SI && index == -1) || (base == -1 && index == SI && scale == 1)){
	    base = SI_;
	    index = -1;
	    scale = -1;
	}
	else if ((base == DI && index == -1) || (base == -1 && index == DI && scale == 1)){
	    base = DI_;
	    index = -1;
	    scale = -1;
	}
	else if ((base == BP && index == -1) || (base == -1 && index == BP && scale == 1)){
	    base = BP_;
	    index = -1;
	    scale = -1;
	}
	else if ((base == BX && index == -1) || (base == -1 && index == BX && scale == 1)){
	    base = BX_;
	    index = -1;
	    scale = -1;
	}
	else if ((base != -1 && base != BX_PLUS_AL) || index != -1){
	    sprintf(g_err_msg, "Syntax: Invalid 16-bits addressing");
	    errorAndExit(RC_PARSER_ADDRESSING_ERR, current_point_);
	}
    }

    if (isAddressing){
	asmoperand_->scale_ = scale;
	asmoperand_->index_ = index;
	asmoperand_->base_ = base;
	asmoperand_->displacement_ = displacement;
	asmoperand_->displacement_size_ = displacement_size;
	asmoperand_->addressing_size_ = addressing_size;
    }

    return isAddressing;
}

bool Parser::parseSI(){
    bool isSI = false;

    if (matchToken(TT_GENERAL_REGISTER, TC_ANY, MOVE_TO_NEXT)){
	isSI = true;
	if (matchToken(TT_MARK, TMK_MUL, MOVE_TO_NEXT))
	    matchToken(TT_HEX, TC_ANY, MOVE_TO_NEXT, NEEDED);
    }
    else if (matchToken(TT_HEX, TC_ANY, MOVE_TO_NEXT)){
	if (matchToken(TT_MARK, TMK_MUL, MOVE_TO_NEXT)){
	    matchToken(TT_GENERAL_REGISTER, TC_ANY, MOVE_TO_NEXT, NEEDED);
	    isSI = true;
	}
	else
	    current_point_--;
    }

    return isSI;
}

bool Parser::parseNum(UINT32 &data, int &size, bool isPositive){
    bool isNum = false;
    int data_size;

    if (matchToken(TT_HEX, TC_ANY, MOVE_TO_NEXT)){
	data = string2bin(token_->at(current_point_ - 1).name_, data_size);
	isNum = true;
    }
    else if (matchToken(TT_MARK, TMK_SUB, MOVE_TO_NEXT)){
	matchToken(TT_HEX, TC_ANY, MOVE_TO_NEXT, NEEDED);
	data = 0xffffffff - string2bin(token_->at(current_point_ - 1).name_, data_size) + 1;
	if (!isPositive)
	    isNum = true;
    }
    size = data_size;

    return isNum;
}

void Parser::print(){
    int token_index = 0;

    for (int i = 0; i < (int)asminstrs_.size(); i++){
	while (token_index < (int)token_->size() && token_->at(token_index).instr_index_ < i)
	    token_index++;
	while(token_->at(token_index).type_ == TT_LABEL || (token_->at(token_index).type_ == TT_MARK && token_->at(token_index).class_ == TMK_COLON))
	    token_index++;
	if (token_->at(token_index).instrstr_index_ < (int)instructions_->size())
	    std::cout << i << ". " << instructions_->at(token_->at(token_index).instrstr_index_).instruction_ << std::endl;
	std::cout << ">>> ";

	switch((UINT8)asminstrs_[i].lockAndRepeat_){
	case PREFIX_REP:
	    std::cout << "rep ";
	    break;
	case PREFIX_REPN:
	    std::cout << "repn ";
	    break;
	case PREFIX_LOCK:
	    std::cout << "lock ";
	    break;
	default:
	    break;
	}

	std::cout << asminstrs_[i].mnemonic_ << " ";

	if (asminstrs_[i].dest_ != NULL)
	    printOperand(asminstrs_[i].dest_);
	if (asminstrs_[i].src1_ != NULL){
	    std::cout << ", ";
	    printOperand(asminstrs_[i].src1_);
	}
	if (asminstrs_[i].src2_ != NULL){
	    std::cout << ", ";
	    printOperand(asminstrs_[i].src2_);
	}
	if (asminstrs_[i].src3_ != NULL){
	    std::cout << ", ";
	    printOperand(asminstrs_[i].src3_);
	}

	std::cout << std::endl;
    }
}

void Parser::printOperand(ASMOPERAND *operand){
    const char *Register32[] = {"eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi", "ebx + al"};
    const char *Register16[] = {"ax", "cx", "dx", "bx", "sp", "bp", "si", "di", "bx + al"};
    const char *Register16_for_addressing[] = {"bx + si", "bx + di", "bp + si", "bp + di", "si", "di", "bp", "bx"};
    const char *Register8[] = {"al", "cl", "dl", "bl", "ah", "ch", "dh", "bh"};
    if (operand->type_ == OPERAND_MEMORY){
	if (operand->operand_size_ != -1 && operand->operand_size_ != SIZE_TO_BE_DECODE)
	    std::cout << g_size[(int)operand->operand_size_] << " ptr ";

	if (operand->segment_ != -1)
	    std::cout << g_segment[(int)operand->segment_] << ": ";

	std::cout << "[";

	if (operand->base_ != -1){
	    if (operand->addressing_size_ == SIZE_BYTE)
		std::cout << Register8[(int)operand->base_];
	    else if (operand->addressing_size_ == SIZE_WORD)
		std::cout << Register16_for_addressing[(int)operand->base_];
	    else if (operand->addressing_size_ == SIZE_DWORD)
		std::cout << Register32[(int)operand->base_];
	    else
		std::cout << "???base";
	}
	if (operand->index_ != -1){
	    if (operand->base_ != -1)
		std::cout << " + ";
	    if (operand->addressing_size_ == SIZE_BYTE)
		std::cout << Register8[(int)operand->index_];
	    else if (operand->addressing_size_ == SIZE_WORD)
		std::cout << Register16[(int)operand->index_];
	    else if (operand->addressing_size_ == SIZE_DWORD)
		std::cout << Register32[(int)operand->index_];
	    else
		std::cout << "???index";
	}
	if (operand->scale_ != -1 && operand->scale_ != 1 && operand->scale_ != 0)
	    std::cout << " * " << int2str(&operand->scale_, sizeof(INT8), 1, 0);
	if (operand->displacement_size_ != -1){
	    INT32 displacement = operand->displacement_;
	    if (((operand->displacement_size_ == SIZE_BYTE && operand->displacement_ & 0x00000080)||
		 (operand->displacement_size_ == SIZE_WORD && operand->displacement_ & 0x00008000) ||
		 (operand->displacement_size_ == SIZE_DWORD && operand->displacement_ & 0x80000000)) &&
		(operand->base_ != -1 || operand->index_ != -1)){
		displacement *= -1;
		std::cout << " - " << int2str(&displacement, sizeof(INT32), 0, 0);
	    }
	    else{
		if (operand->base_ != -1 || operand->index_ != -1)
		    std::cout << " + ";
		std::cout << int2str(&displacement, sizeof(INT32), 0, 0);
	    }
	}
	std::cout << "]";
    }
    else if (operand->type_ == OPERAND_REGISTER){
	if (operand->operand_size_ == SIZE_BYTE)
	    std::cout << Register8[(int)operand->operand_];
	else if (operand->operand_size_ == SIZE_WORD)
	    std::cout << Register16[(int)operand->operand_];
	else if (operand->operand_size_ == SIZE_DWORD)
	    std::cout << Register32[(int)operand->operand_];
	else
	    std::cout << "???Register";
    }
    else if (operand->type_ == OPERAND_SEGMENT)
	std::cout << g_segment[(int)operand->operand_];
    else if (operand->type_ == OPERAND_MMX_REGISTER)
	std::cout << g_ssemmx_register[(int)operand->operand_];
    else if (operand->type_ == OPERAND_SSE_REGISTER)
	std::cout << g_ssemmx_register[(int)operand->operand_ + 8];
    else if (operand->type_ == OPERAND_LABEL)
	std::cout << symtab_->at(operand->operand_).name_;
    else if (operand->type_ == OPERAND_FLOAT)
	std::cout << g_float_register[1][(int)operand->operand_];
    else if (operand->type_ == OPERAND_FLOW){
	if (operand->displacement_size_ != -1){
	    std::cout << int2str(&operand->displacement_, (operand->displacement_size_ == SIZE_DWORD ? sizeof(INT32) : sizeof(INT16)), 1, 0);
	    std::cout << ":";
	}
	std::cout << int2str(&operand->operand_, sizeof(INT32), 1, 0);
    }
    else if (operand->type_ == OPERAND_IMMEDIATE){
	if (operand->operand_size_ == SIZE_DWORD)
	    std::cout << int2str(&operand->operand_, sizeof(INT32), 1, 0);
	else if (operand->operand_size_ == SIZE_WORD)
	    std::cout << int2str(&operand->operand_, sizeof(INT16), 1, 0);
	else if (operand->operand_size_ == SIZE_BYTE)
	    std::cout << int2str(&operand->operand_, sizeof(INT8), 1, 0);
    }
    else if (operand->type_ == OPERAND_CR)
	std::cout << g_cr[operand->operand_];
    else if (operand->type_ == OPERAND_DR)
	std::cout << g_dr[operand->operand_];
    else
	std::cout << "!!!!!!!!!!!!!!!!!!!!!";
}

void Parser::errorAndExit(UINT8 err_type, int token_index){
    int start = -1, end = -1, err_token = token_index;

    if (err_type != RC_PARSER_EMPTY){
	if (token_index >= (int)token_->size())
	    err_token = (int)token_->size() - 1;
	while (token_->at(err_token).instr_index_ != instr_index_)
	    err_token--;
	char *current_instr = instructions_->at(token_->at(err_token).instrstr_index_).instruction_;

	printf("In line %d:\n", instructions_->at(token_->at(err_token).instrstr_index_).line_number_);
	printf("%s\n", current_instr);

	if (err_type == RC_PARSER_TOO_MANY_OPERAND){
	    for (int i = err_token; i >= 0; i--){
		if (token_->at(i).type_ == TT_MARK && token_->at(i).class_ == TMK_COMMA){
		    start = token_->at(i).start_point_;
		    break;
		}
	    }
	    end = (int)strlen(current_instr);
	}
	else if (err_type == RC_PARSER_OPERAND_ERR){
	    for (int i = err_token; i >= 0; i--){
		if ((token_->at(i).type_ == TT_MARK && token_->at(i).class_ == TMK_COMMA) || token_->at(i).type_ == TT_MNEMONIC){
		    start = token_->at(i + 1).start_point_;
		    break;
		}
	    }
	    end = (int)strlen(current_instr);
	    for (int i = err_token; i < (int)token_->size(); i++){
		if (token_->at(i).type_ == TT_MARK && token_->at(i).class_ == TMK_COMMA){
		    end = token_->at(i).end_point_;
		    break;
		}
	    }
	}
	else if (err_type == RC_PARSER_ADDRESSING_ERR){
	    for (int i = err_token; i >= 0; i--){
		if (token_->at(i).type_ == TT_MARK && token_->at(i).class_ == TMK_L_BRACKET){
		    start = token_->at(i + 1).start_point_;
		    break;
		}
	    }
	    for (int i = current_point_; i < (int)token_->size() && token_->at(current_point_).instr_index_ == instr_index_; i++){
		if (token_->at(i).type_ == TT_MARK && token_->at(i).class_ == TMK_R_BRACKET){
		    end = token_->at(i - 1).end_point_;
		    break;
		}
	    }
	    if (end == -1)
		end = token_->at(err_token).end_point_;
	}
	else if (err_type == RC_PARSER_MULTIPLE_PREFIX){
	    start = 0;
	    end = token_->at(err_token).end_point_;
	}
	else if (token_index >= (int)token_->size() || (token_index < (int)token_->size() && token_->at(token_index).instr_index_ != instr_index_)){
	    start = (int)strlen(current_instr);
	    end = start + 1;
	}
	else{
	    start = token_->at(err_token).start_point_;
	    end = token_->at(err_token).end_point_;
	}

	for (int i = 0; i < start; i++){
	    if (current_instr[i] == '\t')
		printf("\t");
	    else if (current_instr[i] == ' ')
		printf(" ");
	    else
		printf(" ");
	}
	for (int i = start; i < end; i++){
	    if (current_instr[i] == ' ' && err_type == RC_PARSER_MULTIPLE_PREFIX)
		printf(" ");
	    else
		printf("^");
	}
	printf("\n");
    }

    displayAndExit(g_err_msg);
}

/* ---------------------------- */
Semantic::Semantic(){
    clear();
}

void Semantic::clear(){
    asminstrs_ = NULL;
    symtab_ = NULL;
    instructions_ = NULL;
    machineCodeSize_ = 0;
    instrIndex_ = -1;

    alternative_.clear();
    flowinstr_.clear();
    flowinstrIndex_.clear();
    originalAddress_.clear();
}

void Semantic::init(std::vector<ASMINSTRUCTION> *asminstrs, std::vector<SYMBOL> *symtab, std::vector<INSTRSTR> *instructions){
    if (asminstrs == NULL || asminstrs->size() <= 0 || instructions == NULL || instructions->size() <= 0){
	sprintf(g_err_msg, "Semantic: Please provide some instructions");
	errorAndExit(RC_SEM_EMPTY, NULL);
    }

    clear();
    asminstrs_ = asminstrs;
    symtab_ = symtab;
    instructions_ = instructions;
}

ASMINSTRUCTION* Semantic::copyAsmInstr(ASMINSTRUCTION *instr){
    ASMINSTRUCTION *newInstr = new ASMINSTRUCTION();
    memcpy(newInstr, instr, sizeof(ASMINSTRUCTION));

    if (instr->dest_ != NULL){
	ASMOPERAND *newOperand = new ASMOPERAND();
	memcpy(newOperand, instr->dest_, sizeof(ASMOPERAND));
	newInstr->dest_ = newOperand;
    }
    if (instr->src1_ != NULL){
	ASMOPERAND *newOperand = new ASMOPERAND();
	memcpy(newOperand, instr->src1_, sizeof(ASMOPERAND));
	newInstr->src1_ = newOperand;
    }
    if (instr->src2_ != NULL){
	ASMOPERAND *newOperand = new ASMOPERAND();
	memcpy(newOperand, instr->src2_, sizeof(ASMOPERAND));
	newInstr->src2_ = newOperand;
    }
    if (instr->src3_ != NULL){
	ASMOPERAND *newOperand = new ASMOPERAND();
	memcpy(newOperand, instr->src3_, sizeof(ASMOPERAND));
	newInstr->src3_ = newOperand;
    }

    return newInstr;
}

void Semantic::deleteAsmInstr(ASMINSTRUCTION *instr){
    if (instr == NULL)
	return;

    if (instr->dest_ != NULL){
	delete instr->dest_;
	instr->dest_ = NULL;
    }
    if (instr->src1_ != NULL){
	delete instr->src1_;
	instr->src1_ = NULL;
    }
    if (instr->src2_ != NULL){
	delete instr->src2_;
	instr->src2_ = NULL;
    }
    if (instr->src3_ != NULL){
	delete instr->src3_;
	instr->src3_ = NULL;
    }

    delete instr;
    instr = NULL;
}

bool Semantic::instrNotMatch(){
    ASMINSTRUCTION *instr1 = alternative_[0], *instr2;

    for (int i = 1; i < (int)alternative_.size(); i++){
	instr2 = alternative_[i];
	if (operandNotMatch(instr1->dest_, instr2->dest_) ||
	    operandNotMatch(instr1->src1_, instr2->src1_) ||
	    operandNotMatch(instr1->src2_, instr2->src2_) ||
	    operandNotMatch(instr1->src3_, instr2->src3_))
	    return true;
    }

    return false;
}

bool Semantic::operandNotMatch(ASMOPERAND *op1, ASMOPERAND *op2){
    if (op1 != NULL && op2 != NULL){
	if (op1->operand_size_ != op2->operand_size_)
	    return true;
    }
    else if (op1 != NULL || op2 != NULL)
	return true;

    return false;
}

void Semantic::sematicCheck(UINT32 base){
    if (asminstrs_ == NULL || asminstrs_->size() <= 0 || instructions_ == NULL || instructions_->size() <= 0){
	sprintf(g_err_msg, "Semantic: Please call `init` function first");
	errorAndExit(RC_SEM_EMPTY, NULL);
    }

    base_ = base;
    address_ = base;

    for (int i = 0; i < (int)asminstrs_->size(); i++){
	instrIndex_++;
	alternative_.clear();
	asminstrs_->at(i).address_ = address_;
	originalAddress_.push_back(address_);
	matchInstr(&asminstrs_->at(i));
	address_ += asminstrs_->at(i).size_;

	if (asminstrs_->at(i).type_ == FLOW_INSTRUCTION && asminstrs_->at(i).dest_ != NULL && asminstrs_->at(i).dest_->type_ != OPERAND_IMMEDIATE
	    && asminstrs_->at(i).dest_->operand_size_ != ABSOLUTE_ADDRESS_FAR_DWORD){
	    flowinstr_.push_back(&asminstrs_->at(i));
	    flowinstrIndex_.push_back(i);
	}
    }

    flowInstructionRelativeAddressReencode();
}

void Semantic::matchInstr(ASMINSTRUCTION *asminstr){
    while (instructions_->at(instrIndex_).mnemonic_index_ < 0)
	instrIndex_++;

    ASMINSTRUCTION *tempInstr = NULL;
    int minInstrSize = -1;
    int minSizeInstrIndex = -1;
    operand_size_not_specify_ = false;

    for (int i = instructions_->at(instrIndex_).mnemonic_index_; i < MNEMONIC_NUM && strcmpMnemonic(asminstr->mnemonic_, g_instr[i].mnemonic_); i++){
	deleteAsmInstr(tempInstr);
	tempInstr = copyAsmInstr(asminstr);

	if ((UINT8)tempInstr->lockAndRepeat_ == PREFIX_LOCK && (!(g_instr[i].lockAndRepeat_ & LOCK_PERMIT) || tempInstr->dest_ == NULL ||
								tempInstr->dest_->type_ != OPERAND_MEMORY)){
	    sprintf(g_err_msg, "Semantic: LOCK prefix are not allowed(only a certain number of instructions with a memory destination are allowed)");
	    errorAndExit(RC_SEM_EMPTY, asminstr);
	}
	else if ((UINT8)tempInstr->lockAndRepeat_ == PREFIX_REPN && !(g_instr[i].lockAndRepeat_ & REPN_PERMIT)){
	    sprintf(g_err_msg, "Semantic: Command does not support prefix repn");
	    errorAndExit(RC_SEM_EMPTY, asminstr);
	}
	else if ((UINT8)tempInstr->lockAndRepeat_ == PREFIX_REP && !(g_instr[i].lockAndRepeat_ & REP_PERMIT)){
	    sprintf(g_err_msg, "Semantic: Command does not support prefix rep");
	    errorAndExit(RC_SEM_EMPTY, asminstr);
	}

	if (matchOperand(tempInstr, &g_instr[i], tempInstr->dest_, g_instr[i].dest_) &&
	    matchOperand(tempInstr, &g_instr[i], tempInstr->src1_, g_instr[i].src1_) &&
	    matchOperand(tempInstr, &g_instr[i], tempInstr->src2_, g_instr[i].src2_) &&
	    matchOperand(tempInstr, &g_instr[i], tempInstr->src3_, g_instr[i].src3_)){
	    tempInstr->ModRM_ = g_instr[i].ModRM_;
	    tempInstr->s_ = g_instr[i].s_;
	    tempInstr->opcode_ = g_instr[i].opcode_;
	    tempInstr->auxiliaryOpcode_ = g_instr[i].auxiliaryOpcode_;
	    tempInstr->alternativeOpcode_ = g_instr[i].alternativeOpcode_;
	    tempInstr->type_ = g_instr[i].type_;

	    alternative_.push_back(tempInstr);
	    tempInstr = copyAsmInstr(asminstr);
	}
    }

    if (alternative_.empty()){
	sprintf(g_err_msg, "Semantic: Command does not support the given operand");
	errorAndExit(RC_SEM_EMPTY, asminstr);
    }
    else if (operand_size_not_specify_ && alternative_.size() >= 2 && instrNotMatch()){
	sprintf(g_err_msg, "Semantic: Please specify the operand size");
	errorAndExit(RC_SEM_EMPTY, asminstr);
    }

    for (int i = 0; i < (int)alternative_.size(); i++){
	machineCodeSize_ = 0;
	encodeInstr(alternative_[i]);
	alternative_[i]->size_ = machineCodeSize_;
	alternative_[i]->machineCode_ = new UINT8(machineCodeSize_);
	memcpy(alternative_[i]->machineCode_, machineCode_, machineCodeSize_);

	if (minInstrSize == -1 || minInstrSize > machineCodeSize_ || (minInstrSize == machineCodeSize_ && signExtended_)){
	    minInstrSize = machineCodeSize_;
	    minSizeInstrIndex = i;
	}
    }

    asminstr->OperandSizeOverride_ = alternative_[minSizeInstrIndex]->OperandSizeOverride_;
    asminstr->segmentOverride_ = alternative_[minSizeInstrIndex]->segmentOverride_;
    asminstr->size_ = minInstrSize;
    asminstr->machineCode_ = new UINT8(minInstrSize);
    asminstr->opcode_ = alternative_[minSizeInstrIndex]->opcode_;
    asminstr->type_ = alternative_[minSizeInstrIndex]->type_;
    asminstr->auxiliaryOpcode_ = alternative_[minSizeInstrIndex]->auxiliaryOpcode_;
    asminstr->alternativeOpcode_ = alternative_[minSizeInstrIndex]->alternativeOpcode_;
    memcpy(asminstr->machineCode_, alternative_[minSizeInstrIndex]->machineCode_, minInstrSize);

    if (asminstr->dest_ != NULL)
	memcpy(asminstr->dest_, alternative_[minSizeInstrIndex]->dest_, sizeof(ASMOPERAND));
    if (asminstr->src1_ != NULL)
	memcpy(asminstr->src1_, alternative_[minSizeInstrIndex]->src1_, sizeof(ASMOPERAND));
    if (asminstr->src2_ != NULL)
	memcpy(asminstr->src2_, alternative_[minSizeInstrIndex]->src2_, sizeof(ASMOPERAND));
    if (asminstr->src3_ != NULL)
	memcpy(asminstr->src3_, alternative_[minSizeInstrIndex]->src3_, sizeof(ASMOPERAND));

    for (int i = 0; i < (int)alternative_.size(); i++)
	deleteAsmInstr(alternative_[i]);
}

bool Semantic::matchOperand(ASMINSTRUCTION *inputInstr, ASMINSTRUCTION *compareInstr, ASMOPERAND *inputOperand,
			    ASMOPERAND *compareOperand){
    if ((inputOperand == NULL && compareOperand == NULL))
	return true;
    else if (inputOperand == NULL && compareOperand != NULL && compareOperand->default_ == true)
	return inputInstr->dest_ == NULL;
    else if (inputOperand == NULL || compareOperand == NULL)
	return false;

    if (inputOperand->type_ == OPERAND_REGISTER && compareOperand->type_ == OPERAND_MEMORY_REGISTER)
	inputOperand->type_ = OPERAND_MEMORY_REGISTER;
    else if (inputOperand->type_ == OPERAND_MEMORY && compareOperand->type_ == OPERAND_MEMORY_REGISTER)
	inputOperand->type_ = OPERAND_MEMORY_REGISTER;
    else if (inputOperand->type_ == OPERAND_REGISTER && compareOperand->type_ == OPERAND_MEMORY)
	inputOperand->type_ = OPERAND_MEMORY;
    else if (inputOperand->type_ == OPERAND_MEMORY && compareOperand->type_ == OPERAND_MEMORY_OFFSET &&
	     inputOperand->base_ == -1 && inputOperand->index_ == -1 && inputOperand->displacement_size_  != -1)
	inputOperand->type_ = OPERAND_MEMORY_OFFSET;
    else if (inputOperand->type_ == OPERAND_MEMORY && compareOperand->type_ == OPERAND_MEMORY_MEMORY)
	inputOperand->type_ = OPERAND_MEMORY_MEMORY;
    else if (inputOperand->type_ == OPERAND_MEMORY && compareOperand->type_ == OPERAND_MMXMEM)
	inputOperand->type_ = OPERAND_MMXMEM;
    else if (inputOperand->type_ == OPERAND_MEMORY && compareOperand->type_ == OPERAND_FLOATMEM)
	inputOperand->type_ = OPERAND_FLOATMEM;
    else if (inputOperand->type_ == OPERAND_IMMEDIATE && compareOperand->type_ == OPERAND_FLOWIMM){
	inputOperand->operand_size_ = compareOperand->operand_size_;
	inputOperand->type_ = OPERAND_FLOWIMM;
	return true;
    }
    else if (inputOperand->type_ == OPERAND_LABEL && compareOperand->type_ == OPERAND_LABEL){
	inputOperand->operand_size_ = compareOperand->operand_size_;
	return true;
    }
    else if (inputOperand->type_ == OPERAND_FLOAT && (compareOperand->type_ == OPERAND_FLOATMEM || compareOperand->type_ == OPERAND_MEMORY_FLOAT)){
	inputOperand->type_ = OPERAND_MEMORY_FLOAT;
	return true;
    }
    else if (inputOperand->type_ == OPERAND_SSE_REGISTER && (compareOperand->type_ == OPERAND_SSEMEM || compareOperand->type_ == OPERAND_MEMORY_SSE)){
	inputOperand->type_ = OPERAND_MEMORY_SSE;
	return true;
    }
    else if (inputOperand->type_ == OPERAND_MEMORY && compareOperand->type_ == OPERAND_SSEMEM)
	inputOperand->type_ = OPERAND_SSEMEM;
    else if (inputOperand->type_ == OPERAND_MMX_REGISTER && (compareOperand->type_ == OPERAND_MMXMEM || compareOperand->type_ == OPERAND_MEMORY_MMX)){
	inputOperand->type_ = OPERAND_MEMORY_MMX;
	return true;
    }

    if (inputOperand->type_ == compareOperand->type_){
	if (inputOperand->type_ == OPERAND_MEMORY_MEMORY)
	    inputOperand->type_ = OPERAND_MEMORY;
	else if (inputOperand->type_ == OPERAND_SSEMEM && inputOperand->addressing_size_ != -1)
	    inputOperand->type_ = OPERAND_MEMORY;
	else if (inputOperand->type_ == OPERAND_MMXMEM && inputOperand->addressing_size_ != -1)
	    inputOperand->type_ = OPERAND_MEMORY;
	else if (inputOperand->type_ == OPERAND_FLOATMEM && inputOperand->addressing_size_ != -1)
	    inputOperand->type_ = OPERAND_MEMORY;

	if (inputOperand->type_ == OPERAND_IMMEDIATE &&
	    ((inputOperand->operand_size_ == SIZE_WORD && ((inputOperand->operand_ & 0xff80) == 0xff80)) ||
	     (inputOperand->operand_size_ == SIZE_DWORD && ((inputOperand->operand_ & 0xffffff80) == 0xffffff80))))
	    inputOperand->operand_size_ = SIZE_BYTE;
	if (inputOperand->type_ == OPERAND_IMMEDIATE && inputOperand->operand_size_ <= compareOperand->operand_size_){
	    if (compareOperand->operand_size_ == SIZE_DWORD && inputInstr->OperandSizeOverride_ == 1)
		inputOperand->operand_size_ = SIZE_WORD;
	    else
		inputOperand->operand_size_ = compareOperand->operand_size_;
	}

	if (compareOperand->operand_size_ != -1){
	    if (inputOperand->operand_size_ == SIZE_TO_BE_DECODE){
		operand_size_not_specify_ = true;
		if (compareOperand->operand_size_ == SIZE_DWORD && inputInstr->OperandSizeOverride_ == 1)
		    inputOperand->operand_size_ = SIZE_WORD;
		else if ((compareOperand->operand_size_ == SIZE_QWORD || compareOperand->operand_size_ == SIZE_FWORD) &&
			 inputInstr->OperandSizeOverride_ == 1)
		    inputOperand->operand_size_ = SIZE_DWORD;
		else
		    inputOperand->operand_size_ = compareOperand->operand_size_;
	    }

	    if (inputOperand->operand_size_ != compareOperand->operand_size_){
		if (inputOperand->type_ == OPERAND_REGISTER || inputOperand->type_ == OPERAND_MEMORY ||
		    inputOperand->type_ == OPERAND_IMMEDIATE || inputOperand->type_ == OPERAND_MEMORY_REGISTER){
		    if (compareInstr->OperandSizeOverride_ != -1 &&
			inputOperand->operand_size_ == SIZE_WORD && compareOperand->operand_size_ == SIZE_DWORD)
			inputInstr->OperandSizeOverride_ = 1;
		    else if (compareInstr->OperandSizeOverride_ != -1 &&
			     inputOperand->operand_size_ == SIZE_14BYTE && compareOperand->operand_size_ == SIZE_28BYTE)
			inputInstr->OperandSizeOverride_ = 1;
		    else if (compareInstr->OperandSizeOverride_ != -1 &&
			     inputOperand->operand_size_ == SIZE_94BYTE && compareOperand->operand_size_ == SIZE_108BYTE)
			inputInstr->OperandSizeOverride_ = 1;
		    else if (compareInstr->OperandSizeOverride_ != -1 &&
			     inputOperand->operand_size_ == SIZE_DWORD && (compareOperand->operand_size_ == SIZE_QWORD ||
									   compareOperand->operand_size_ == SIZE_FWORD)){
			if (inputOperand->type_ == OPERAND_MEMORY && inputOperand->addressing_size_ != -1)
			    inputInstr->OperandSizeOverride_ = 1;
		    }
		    else if (((inputOperand->type_ == OPERAND_MEMORY && inputOperand->addressing_size_ != -1) ||
			      inputOperand->type_ == OPERAND_REGISTER || inputOperand->type_ == OPERAND_MEMORY_REGISTER) &&
			     ((inputOperand->operand_size_ == SIZE_DWORD && compareOperand->operand_size_ == SIZE_FIXED_DWORD) ||
			      (inputOperand->operand_size_ == SIZE_FWORD && compareOperand->operand_size_ == SIZE_FIXED_FWORD) ||
			      (inputOperand->operand_size_ == SIZE_QWORD && compareOperand->operand_size_ == SIZE_FIXED_QWORD))){}
		    else if ((inputOperand->type_ == OPERAND_MEMORY && inputOperand->addressing_size_ == -1) &&
			     (inputOperand->operand_size_ == SIZE_DWORD && (compareOperand->operand_size_ == SIZE_FIXED_DWORD ||
									    compareOperand->operand_size_ == SIZE_FIXED_FWORD ||
									    compareOperand->operand_size_ == SIZE_FIXED_QWORD))){}
		    else if (inputOperand->type_ == OPERAND_REGISTER && inputOperand->operand_size_ == SIZE_DWORD &&
			     compareOperand->operand_size_ == SIZE_28BYTE){}
		    else if (inputOperand->operand_size_ != compareOperand->operand_size_)
			return false;
		}
		else
		    return false;
	    }
	    else if (inputOperand->type_ == OPERAND_IMMEDIATE || inputOperand->type_ == OPERAND_REGISTER ||
		     inputOperand->type_ == OPERAND_MEMORY){
		if (inputInstr->OperandSizeOverride_ == 1 && compareOperand->operand_size_ == SIZE_DWORD)
		    return false;
	    }
	}
    }
    else
	return false;

    if (compareOperand->operand_ != -1 && inputOperand->operand_ != compareOperand->operand_)
	return false;

    if ((inputOperand->type_ == OPERAND_MEMORY || inputOperand->type_ == OPERAND_MEMORY_OFFSET) &&
	inputOperand->addressing_size_ != -1){
	if (compareOperand->type_ == OPERAND_MEMORY  && compareOperand->segment_ == ES &&
	    compareOperand->base_ == EDI && inputOperand->segment_ != -1 && inputOperand->segment_ != TSG_ES)
	    return false;
	else if (inputOperand->segment_ != -1){
	    if ((inputOperand->addressing_size_ == SIZE_DWORD &&
		 (inputOperand->base_ == EBP || inputOperand->base_ == ESP) && inputOperand->segment_ != TSG_SS) ||
		(inputOperand->addressing_size_ == SIZE_WORD &&
		 (inputOperand->base_ == BP_PLUS_SI || inputOperand->base_ == BP_PLUS_DI || inputOperand->base_ == BP_)
		 && inputOperand->segment_ != TSG_SS) ||
		(compareOperand->segment_ != -1 && inputOperand->segment_ != compareOperand->segment_) ||
		(compareOperand->segment_ == -1 && inputOperand->segment_ != TSG_DS)){
		switch(inputOperand->segment_){
		case TSG_CS:
		    inputInstr->segmentOverride_ = PREFIX_CS;
		    break;
		case TSG_SS:
		    inputInstr->segmentOverride_ = PREFIX_SS;
		    break;
		case TSG_DS:
		    inputInstr->segmentOverride_ = PREFIX_DS;
		    break;
		case TSG_ES:
		    inputInstr->segmentOverride_ = PREFIX_ES;
		    break;
		case TSG_FS:
		    inputInstr->segmentOverride_ = PREFIX_FS;
		    break;
		case TSG_GS:
		    inputInstr->segmentOverride_ = PREFIX_GS;
		    break;
		default:
		    sprintf(g_err_msg, "Semantic: Segment not recognised");
		    errorAndExit(RC_SEM_EMPTY, inputInstr);
		}
	    }
	}

	if (inputOperand->addressing_size_ == SIZE_WORD && compareInstr->AddressSizeOverride_ != -1)
	    inputInstr->AddressSizeOverride_ = 1;
	else if (inputOperand->addressing_size_ == SIZE_WORD && compareInstr->AddressSizeOverride_ == -1)
	    return false;

	if (compareOperand->base_ != -1){
	    if (inputOperand->addressing_size_ == SIZE_WORD && compareOperand->addressing_size_ == SIZE_DWORD){
		if ((inputOperand->base_ == SI_ && compareOperand->base_ != ESI) ||
		    (inputOperand->base_ == DI_ && compareOperand->base_ != EDI) ||
		    (inputOperand->base_ == BP_ && compareOperand->base_ != EBP) ||
		    (inputOperand->base_ == BX_ && compareOperand->base_ != EBX))
		    return false;
	    }
	    else if (inputOperand->base_ != compareOperand->base_)
		return false;
	}
	if (compareOperand->index_ != -1 && inputOperand->index_ != compareOperand->index_)
	    return false;
	if (compareOperand->scale_ != -1 && inputOperand->scale_ != compareOperand->scale_)
	    return false;
	if (compareOperand->displacement_size_ != -1 && inputOperand->displacement_ != compareOperand->displacement_)
	    return false;
    }
    else if (inputOperand->operand_size_ == ABSOLUTE_ADDRESS_FAR_DWORD  && ((inputOperand->operand_ & 0xffff0000) == 0))
	inputInstr->OperandSizeOverride_ = 1;

    inputOperand->default_ = compareOperand->default_;

    return true;
}

void Semantic::addMachineCode(int size, UINT32 code){
    for (int i = 0; i < size; i++, machineCodeSize_++){
	machineCode_[machineCodeSize_] = code & 0xff;
	code >>= 8;
    }
}

int Semantic::hexSize(UINT32 opcode){
    unsigned int mask = 0xff000000;
    int rotate = 3;

    while (rotate){
	if (opcode & mask)
	    break;
	mask >>= 8;
	rotate--;
    }

    return rotate + 1;
}

void Semantic::flowInstructionRelativeAddressReencode(){
    if (flowinstr_.empty())
	return;

    bool changed = true;
    ASMOPERAND *operand;
    ASMINSTRUCTION *instr;
    std::vector<offsetTable> bot;
    int size, temp, upper, lower;
    UINT32 destAddress;

    while (changed){
	changed = false;

	for (int i = 0; i < (int)flowinstr_.size(); i++){
	    instr = flowinstr_[i];
	    operand = instr->dest_;

	    if ((operand->type_ == OPERAND_FLOWIMM || operand->type_ == OPERAND_LABEL) &&
		operand->operand_size_ == SIZE_DWORD && instr->alternativeOpcode_ != -1){
		size = instr->size_ - hexSize(instr->opcode_) + hexSize(instr->alternativeOpcode_);
		if (instr->lockAndRepeat_ != -1)
		    size++;
		if (instr->segmentOverride_ != -1)
		    size++;
		if (instr->OperandSizeOverride_ != -1)
		    size -= 2;
		else
		    size -= 3;

		if (operand->type_ == OPERAND_FLOWIMM)
		    destAddress = operand->operand_;
		else{
		    destAddress = originalAddress_[operand->operand_];
		    for (int j = 0; j < (int)bot.size(); j++){
			if (originalAddress_[operand->operand_] > originalAddress_[flowinstrIndex_[bot[j].index_]])
			    destAddress -= bot[j].offset_;
		    }
		}

		if (instr->OperandSizeOverride_ == -1 &&
		    (int(destAddress - instr->address_ - size) <= 0xff / 2) &&
		    (int(destAddress - instr->address_ - size) >= (-0xff / 2 - 1))){
		    if (3 + hexSize(instr->opcode_) - hexSize(instr->alternativeOpcode_) > 0){
			bot.push_back(offsetTable(i, 3 + hexSize(instr->opcode_) - hexSize(instr->alternativeOpcode_)));
			balanceOffset(i, 3 + hexSize(instr->opcode_) - hexSize(instr->alternativeOpcode_));

			temp = instr->opcode_;
			instr->opcode_ = instr->alternativeOpcode_;
			instr->alternativeOpcode_ = temp;
			operand->operand_size_ = SIZE_BYTE;
			changed = true;

			if (operand->type_ == OPERAND_LABEL)
			    operand->displacement_ = destAddress;
		    }
		}
		else if (instr->OperandSizeOverride_ == 1 && (destAddress & 0xffff0000) == 0){
		    upper = (instr->address_ & 0x0000ffff) + size - 0x80 + 1;
		    upper &= 0x8000ffff;
		    lower = (instr->address_ & 0x0000ffff) + size + 0x7f + 1;
		    lower &= 0x0000ffff;

		    if (operand->operand_ <= lower && operand->operand_ >= upper){
			if (1 + hexSize(instr->opcode_) - hexSize(instr->alternativeOpcode_) > 0){
			    bot.push_back(offsetTable(i, 1 + hexSize(instr->opcode_) - hexSize(instr->alternativeOpcode_)));
			    balanceOffset(i, 1 + hexSize(instr->opcode_) - hexSize(instr->alternativeOpcode_));

			    instr->opcode_ = instr->alternativeOpcode_;
			    operand->operand_size_ = SIZE_BYTE;
			    changed = true;
			}
		    }
		}
	    }
	}
    }

    for (int i = 0; i < (int)flowinstr_.size(); i++){
	instr = flowinstr_[i];
	operand = instr->dest_;
	if ((operand->type_ == OPERAND_FLOW && operand->operand_size_ == RELATIVE_ADDRESS_SHORT) ||
	    (operand->type_ == OPERAND_FLOWIMM && operand->operand_size_ == SIZE_BYTE)){
	    size = hexSize(instr->opcode_);
	    if (instr->lockAndRepeat_ != -1)
		size++;
	    if (instr->segmentOverride_ != -1)
		size++;
	    if (instr->OperandSizeOverride_ != -1)
		size++;

	    if ((int(operand->operand_ - instr->address_ - size - 1) <= 0xff / 2) &&
		(int(operand->operand_ - instr->address_ - size) >= (-0xff / 2))){}
	    else if ((operand->operand_ & 0xffff0000) == 0){
		upper = (instr->address_ & 0x0000ffff) + size + 1 - 0x80;
		if (instr->OperandSizeOverride_ == -1)
		    upper++;
		upper &= 0x8000ffff;
		lower = (instr->address_ & 0x0000ffff) + size + 1 + 0x7f;
		if (instr->OperandSizeOverride_ == -1)
		    lower++;
		lower &= 0x0000ffff;

		if (operand->operand_ <= lower && operand->operand_ >= upper){}
		else {
		    sprintf(g_err_msg, "Semantic: Jmp short out of range(Address is %s and about to jmp to %s)",
			    int2str(&instr->address_, sizeof(UINT32), 1, 0), int2str(&operand->operand_, sizeof(UINT32), 1, 0));
		    errorAndExit(RC_SEM_EMPTY, flowinstr_[i]);
		}
	    }
	    else{
		sprintf(g_err_msg, "Semantic: Jmp short out of range(Address is %s and about to jmp to %s)",
			int2str(&instr->address_, sizeof(UINT32), 1, 0), int2str(&operand->operand_, sizeof(UINT32), 1, 0));
		errorAndExit(RC_SEM_EMPTY, flowinstr_[i]);
	    }
	}
	if (operand->type_ == OPERAND_LABEL){
	    destAddress = originalAddress_[operand->operand_];
	    for (int j = 0; j < (int)bot.size(); j++){
		if (originalAddress_[operand->operand_] > originalAddress_[flowinstrIndex_[bot[j].index_]])
		    destAddress -= bot[j].offset_;
	    }
	    operand->displacement_ = destAddress;
	}
    }

    for (int i = 0; i < (int)flowinstr_.size(); i++){
	machineCodeSize_ = 0;
	encodeInstr(flowinstr_[i]);
	flowinstr_[i]->size_ = machineCodeSize_;
	delete flowinstr_[i]->machineCode_;
	flowinstr_[i]->machineCode_ = new UINT8(machineCodeSize_);
	memcpy(flowinstr_[i]->machineCode_, machineCode_, machineCodeSize_);
    }
}

void Semantic::balanceOffset(int index, int offset){
    for (int i = index + 1; i < (int)flowinstr_.size(); i++)
	flowinstr_[i]->address_ -= offset;
}

void Semantic::encodeInstr(ASMINSTRUCTION *instr, UINT8 modrm){
    modrm_ = 0;
    sib_ = 0;
    sib_exist_ = false;
    displacement_size_ = -1;
    immediate_size1_ = -1;
    immediate_size2_ = -1;
    signExtended_ = false;

    encodeOperand(instr, instr->dest_, modrm);
    encodeOperand(instr, instr->src1_, modrm);
    encodeOperand(instr, instr->src2_, modrm);
    encodeOperand(instr, instr->src3_, modrm);

    if (instr->lockAndRepeat_ != -1)
	addMachineCode(1, (UINT8)instr->lockAndRepeat_);
    if (instr->segmentOverride_ != -1)
	addMachineCode(1, (UINT8)instr->segmentOverride_);
    if (instr->OperandSizeOverride_ == 1)
	addMachineCode(1, 0x66);
    if (instr->AddressSizeOverride_ == 1)
	addMachineCode(1, 0x67);
    if (instr->waitPrefix_ == 1)
	addMachineCode(1, 0x9b);

    for (int i = hexSize(instr->opcode_) - 1; i >= 0; i--)
	addMachineCode(1, (UINT32)(instr->opcode_ >> (8 * i)));

    if (instr->ModRM_ || instr->auxiliaryOpcode_ != -1){
	addMachineCode(1, modrm);

	if (sib_exist_)
	    addMachineCode(1, sib_);
    }
    if (displacement_size_ != -1){
	if (displacement_size_ == SIZE_BYTE)
	    addMachineCode(1, displacement_);
	else if (displacement_size_ == SIZE_WORD)
	    addMachineCode(2, displacement_);
	else
	    addMachineCode(4, displacement_);
    }
    if (immediate_size1_ != -1){
	if (immediate_size1_ == SIZE_BYTE)
	    addMachineCode(1, immediate1_);
	else if (immediate_size1_ == SIZE_WORD)
	    addMachineCode(2, immediate1_);
	else
	    addMachineCode(4, immediate1_);
    }
    if (immediate_size2_ != -1){
	if (immediate_size2_ == SIZE_BYTE)
	    addMachineCode(1, immediate2_);
	else if (immediate_size2_ == SIZE_WORD)
	    addMachineCode(2, immediate2_);
	else
	    addMachineCode(4, immediate2_);
    }

    instr->machineCode_ = new UINT8(machineCodeSize_);
    memcpy(instr->machineCode_, machineCode_, machineCodeSize_);
    instr->size_ = machineCodeSize_;
}

void Semantic::encodeOperand(ASMINSTRUCTION *instr, ASMOPERAND *operand, UINT8 modrm){
    if (operand == NULL || operand->default_)
	return;

    int upper, lower;
    int mnemonic_size = hexSize(instr->opcode_);
    if (instr->lockAndRepeat_ != -1)
	mnemonic_size++;
    if (instr->segmentOverride_ != -1)
	mnemonic_size++;
    if (instr->OperandSizeOverride_ != -1)
	mnemonic_size++;
    if (instr->AddressSizeOverride_ != -1)
	mnemonic_size++;

    if (instr->ModRM_ || instr->auxiliaryOpcode_ != -1){
	if (instr->auxiliaryOpcode_ != -1)
	    modrm_ |= (instr->auxiliaryOpcode_ << 3);

	if ((operand->type_ == OPERAND_REGISTER || operand->type_ == OPERAND_SEGMENT || operand->type_ == OPERAND_CR ||
	     operand->type_ == OPERAND_SSE_REGISTER || operand->type_ == OPERAND_MMX_REGISTER) &&
	    instr->auxiliaryOpcode_ == -1)
	    modrm_ |= (operand->operand_ << 3);
	else if (operand->type_ == OPERAND_MEMORY || operand->type_ == OPERAND_MEMORY_REGISTER ||
		 operand->type_ == OPERAND_MEMORY_SSE || operand->type_ == OPERAND_MEMORY_MMX ||
		 operand->type_ == OPERAND_MEMORY_FLOAT || operand->type_ == OPERAND_MEMORY_MEMORY){
	    if (operand->displacement_size_ != -1){
		if (operand->displacement_ < 0)
		    operand->displacement_size_ = SIZE_DWORD;
		if (operand->displacement_size_ == SIZE_BYTE && operand->displacement_ & 0x80){
		    if (operand->addressing_size_ == SIZE_DWORD)
			operand->displacement_size_ = SIZE_DWORD;
		    else
			operand->displacement_size_ = SIZE_WORD;
		}
		if ((operand->displacement_size_ == SIZE_WORD && ((operand->displacement_ & 0xff80) == 0xff80)) ||
		    (operand->displacement_size_ == SIZE_DWORD && ((operand->displacement_ & 0xffffff80) == 0xffffff80)))
		    operand->displacement_size_ = SIZE_BYTE;

		if (operand->base_ == -1 && operand->index_ == -1 && instr->AddressSizeOverride_ != 1)
		    operand->displacement_size_ = SIZE_DWORD;
	    }

	    if (operand->addressing_size_ == -1){
		modrm_ |= 0xc0;
		modrm_ |= operand->operand_;
	    }
	    else if (operand->addressing_size_ == SIZE_WORD){
		instr->AddressSizeOverride_ = 1;
		if (operand->displacement_size_ == SIZE_BYTE)
		    modrm_ |= 0x40;
		else if (operand->displacement_size_ == SIZE_WORD && operand->base_ != -1)
		    modrm_ |= 0x80;
		else if (operand->displacement_size_ == -1 && operand->base_ == BP_){
		    modrm_ |= 0x40;
		    operand->displacement_size_ = SIZE_BYTE;
		    operand->displacement_ = 0x0;
		}

		if (operand->base_ != -1)
		    modrm_ |= operand->base_;
		else
		    modrm_ |= 0x6;
	    }
	    else{
		if ((operand->base_ == EBP && operand->scale_ == 0 && operand->displacement_size_ == -1) ||
		    (operand->base_ == EBP && operand->scale_ > 1 && operand->displacement_size_ == -1) ||
		    (operand->base_ == EBP && operand->scale_ == 1 && operand->index_ == EBP &&
		     operand->displacement_size_ == -1)){
		    operand->displacement_size_ = SIZE_BYTE;
		    operand->displacement_ = 0;
		}
		else if (operand->base_ == EBP && operand->scale_ == 1 && operand->index_ != EBP){
		    operand->base_ = operand->index_;
		    operand->index_ = EBP;
		}
		else if (operand->base_ == -1 && operand->scale_ > 2 && operand->displacement_size_ != SIZE_DWORD){
		    if (operand->displacement_size_ == -1)
			operand->displacement_ = 0;
		    operand->displacement_size_ = SIZE_DWORD;
		}
		else if (((modrm >> 6) & 0x3) == 0x1 && operand->displacement_size_ == SIZE_DWORD){
		    operand->displacement_size_ = SIZE_BYTE;
		}

		if (operand->displacement_size_ == SIZE_BYTE)
		    modrm_ |= 0x40;
		else if ((operand->displacement_size_ == SIZE_WORD || operand->displacement_size_ == SIZE_DWORD) && operand->base_ != -1){
		    operand->displacement_size_ = SIZE_DWORD;
		    modrm_ |= 0x80;
		}

		if (operand->base_ != -1)
		    modrm_ |= operand->base_;
		else
		    modrm_ |= 0x5;

		if ((operand->index_ != -1 && operand->scale_ >= 0) || operand->base_ == ESP){
		    modrm_ = (modrm_ & 0xf8) | 0x4;
		    sib_exist_ = true;

		    if (operand->scale_ == 2)
			sib_ |= 0x40;
		    else if (operand->scale_ == 4)
			sib_ |= 0x80;
		    else if (operand->scale_ == 8)
			sib_ |= 0xc0;

		    if (operand->base_ == -1)
			sib_ |= 0x5;
		    else
			sib_ |= operand->base_;

		    if (operand->index_ != -1)
			sib_ |= (operand->index_ << 3);
		    else
			sib_ |= (0x4 << 3);
		}
	    }
	}
    }

    if (operand->type_ == OPERAND_FLOW){
	if (operand->operand_size_ == RELATIVE_ADDRESS_SHORT){
	    immediate_size1_ = SIZE_BYTE;
	    immediate1_ = operand->operand_ - instr->address_ - mnemonic_size - 1;

	    if ((operand->operand_ & 0xffff0000) == 0){
		upper = (instr->address_ & 0x0000ffff) + mnemonic_size + 1 - 0x80;
		if (instr->OperandSizeOverride_ == -1)
		    upper++;
		upper &= 0x8000ffff;
		lower = (instr->address_ & 0x0000ffff) + mnemonic_size + 1 + 0x7f;
		if (instr->OperandSizeOverride_ == -1)
		    lower++;
		lower &= 0x0000ffff;

		if (operand->operand_ <= lower && operand->operand_ >= upper){
		    instr->OperandSizeOverride_ = 1;
		    immediate1_ = operand->operand_ - (instr->address_ & 0x0000ffff) - mnemonic_size - 1;
		}
	    }
	}
	else if (operand->operand_size_ == RELATIVE_ADDRESS_NEAR_DWORD){
	    immediate_size1_ = SIZE_DWORD;
	    immediate1_ = operand->operand_ - instr->address_ - mnemonic_size - 4;

	    if ((operand->operand_ & 0xffff0000) == 0){
		instr->OperandSizeOverride_ = 1;
		immediate_size1_ = SIZE_WORD;
		immediate1_ = operand->operand_ - (instr->address_ & 0x0000ffff) - mnemonic_size - 2;
	    }
	}
	else if (operand->operand_size_ == ABSOLUTE_ADDRESS_FAR_DWORD){
	    if ((operand->operand_ & 0xffff0000) == 0)
		immediate_size1_ = SIZE_WORD;
	    else
		immediate_size1_ = SIZE_DWORD;
	    immediate1_ = operand->operand_;
	    immediate_size2_ = SIZE_WORD;
	    immediate2_ = operand->displacement_;
	}
    }
    else if (operand->type_ == OPERAND_FLOWIMM){
	immediate_size1_ = operand->operand_size_;
	if (operand->operand_size_ == SIZE_DWORD){
	    immediate1_ = operand->operand_ - instr->address_ - mnemonic_size - 4;

	    if ((operand->operand_ & 0xffff0000) == 0){
		immediate_size1_ = SIZE_WORD;
		instr->OperandSizeOverride_ = 1;
		immediate1_ = operand->operand_ - (instr->address_ & 0x0000ffff) - mnemonic_size - 2;
	    }
	}
	else{
	    immediate1_ = operand->operand_ - instr->address_ - mnemonic_size - 1;

	    if ((operand->operand_ & 0xffff0000) == 0)
		immediate1_ = operand->operand_ - (instr->address_ & 0x0000ffff) - mnemonic_size - 1;
	}
    }
    else if (operand->type_ == OPERAND_LABEL){
	immediate_size1_ = operand->operand_size_;
	if (operand->operand_size_ == SIZE_DWORD)
	    immediate1_ = operand->displacement_ - instr->address_ - mnemonic_size - 4;
	else
	    immediate1_ = operand->displacement_ - instr->address_ - mnemonic_size - 1;
    }
    else if (operand->type_ == OPERAND_IMMEDIATE){
	if (immediate_size1_ == -1){
	    if (instr->src1_ == NULL && operand->operand_size_ == SIZE_DWORD && (operand->operand_ & 0xffff0000) == 0){
		instr->OperandSizeOverride_ = 1;
		immediate_size1_ = SIZE_WORD;
	    }
	    else
		immediate_size1_ = operand->operand_size_;
	    immediate1_ = operand->operand_;
	}
	else if (immediate_size2_ == -1){
	    immediate_size2_ = operand->operand_size_;
	    immediate2_ = operand->operand_;
	}
	else{
	    sprintf(g_err_msg, "SYSTEM: Encode immediate number ran out");
	    errorAndExit(RC_SEM_EMPTY, NULL);
	}

	if (operand->signExtended_ && instr->s_ == 1)
	    signExtended_ = true;
    }

    if (operand->displacement_size_ != -1 && operand->type_ != OPERAND_FLOW){
	displacement_size_ = operand->displacement_size_;
	displacement_ = operand->displacement_;
    }
}

char* Semantic::outputMachineCode(){
    ASMINSTRUCTION *instr;
    FILE *output = fopen("output.txt", "w+");
    int total_size = 0, currentSize = 0;
    char *machineCode, *singleInstrMachineCode;

    for (int i = 0; i < (int)asminstrs_->size(); i++){
	instr = &asminstrs_->at(i);
	total_size += instr->size_ * 2;
    }

    machineCode = (char*)malloc(total_size + 1);
    machineCode[0] = '\0';

    for (int i = 0; i < (int)asminstrs_->size(); i++){
	instr = &asminstrs_->at(i);
	for (int i = 0; i < instr->size_; i++){
	    singleInstrMachineCode = int2str(&instr->machineCode_[i], sizeof(UINT8), 1, 0, false);
	    strcat(machineCode, singleInstrMachineCode);
	    currentSize += 2;
	}
    }
    fwrite(machineCode, total_size, 1, output);
    fclose(output);
    return machineCode;
}

void Semantic::errorAndExit(UINT8 err_type, ASMINSTRUCTION *instr){
    if (instr != NULL){
	std::cout << "In line " << instructions_->at(instr->instrstr_index_).line_number_ << ":" << std::endl;
	std::cout << instructions_->at(instr->instrstr_index_).instruction_ << std::endl;
    }

    displayAndExit(g_err_msg);
}
