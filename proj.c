#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "proj.h"
#include "Queue.h"
#include "SymbolTable.h"

int main()
{
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);
	int  i = 0; FILE* fp;
	string s = malloc(sizeof(string) * maxStringLength);
	string fileNames[maxFilesOnRun];
	/*getFileName(fileNames);*/
	fileNames[i] = "ex3";
	/*for(;strcmp(fileNames[i], "NULL") != 0; i++){*/
		initSymbolTable(labelTABLE);
		initSymbolTable(entryTABLE);
		initSymbolTable(externTABLE);
		QUE_initiate();

		if((fp = fopen(strcat(strcpy(s, fileNames[i]), asEndOfFile), "r")) == NULL )
			fprintf(stderr, "\nError opening file %s ", fileNames[i]);
		else{
			firstStep(fp);
			secondStep();
			printMemory(strcat(strcpy(s, fileNames[i]),obEndOfFile));
			printEntryTable(entryTABLE, strcat(strcpy(s, fileNames[i]),entEndOfFile));
			printExternTable(strcat(strcpy(s, fileNames[i]),extEndOfFile));
		}
		freeSymbolTable(labelTABLE);
		freeSymbolTable(entryTABLE);
		freeSymbolTable(externTABLE);
	/*}*/
	return 1;
}

/*
 * This function is reading from input names of files.
 * fileNames - array of file names.
 */
void getFileName(string fileNames[])
{
	string s = malloc(sizeof(string));
	line* ln = malloc(sizeof(line));
	bool temp = FALSE;
	int i = 0, idx = 0;
	printf("Write 'assembler' and then the name of the files you want to run.\n"
			"example: assembler x y hello\nYour command:");
	fgets(s, maxLineLength, stdin);
	readNextWord(s, &idx, &temp, ln);
	if(strcmp((*ln).word, "assembler") == 0){
		do {
			readNextWord(s, &idx, &temp, ln);
			fileNames[i] = (*ln).word;
		}while(strcmp(fileNames[i++], "EMPTY") != 0);
	}else fprintf(stderr, "wrong input. check your way of writing assembler.");
	fileNames[i - 1] = "NULL";
	free(ln);
	free(s);
}

/*
 * This function returns a substring from index start to index end.
 * start - start index.
 * end - end index.
 * returns  a substring from index start to index end.
 */
string getSubString(string s, int start, int end)
{
	string temp = malloc(sizeof(string));
	int i = 0;
	for(; (i + start) < end; i++)
		temp[i] = s[i + start];
	temp[i] = '\0';
	return temp;
}

/*
 * This function cast the decimal value to binary and prints it.
 * num - the number to cast.
 * digits - number of digits in the number.
 * fp - the name of the file that the function writes to.
 */
void printBinary(int num, int digits, FILE* fp)
{
	int mask = 1, i = 0;
	for(i = 0; i < digits - 1; i++)
		mask = mask * 2;
	while (digits--) {
	    if (num & mask)
	        fprintf(fp, "1");
	    else
	        fprintf(fp ,"0");
	    mask = mask >> 1;
	}
}

/*
 * This function cast the decimal value to base 4 and prints it.
 * num - the number to cast.
 * digits - number of digits in the number.
 * fp - the name of the file that the function writes to.
 */
void printBase4(int num, int digits, FILE* fp)
{
	int mask = 3, i = 0, temp = 0;
	digits = digits / 2;
	for(i = 0; i < digits - 1; i++)
		mask = mask << 2;
	while(digits--){
		temp = num & mask;
		fprintf(fp,"%d", temp >> (digits * 2));
		mask = mask >> 2;
	}
}

/*
 * This function cast the decimal value to base 4 and prints it.
 * num - the number to cast.
 * fp - the name of the file that the function writes to.
 */
void cast10To4(int num, FILE* fp)
{
	int shalem = 0, shever = 0, place = 1;
	while(num != 0){
		shalem = num / 4;
		shever += (num % 4)* place;
		place = place * 10;
		num = shalem;
	}
	fprintf(fp, "%d ", shever);
}

/*
 * This function prints the memory to a file.
 * file - the file which we write to.
 */
void printMemory(string file)
{
	int i = 0, z = 0;
	int temp = 0;
	FILE* fp = fopen(file, "w");
	for(; i < memorySize; i++)
	{
		if(mem[i].fieldNum == -1)
			break;
		cast10To4(i + startIcValue, fp);
		switch(mem[i].fieldNum){
			case 1:
				printBase4(mem[i].cml.cml.group, 2, fp);
				printBase4(mem[i].cml.cml.opcode, 4, fp);
				printBase4(mem[i].cml.cml.src, 2, fp);
				printBase4(mem[i].cml.cml.dest, 2, fp);
				printBase4(mem[i].cml.cml.era, 2, fp);
				break;
			case 2:
				printBase4(guidel[z++].d.num, 12, fp);
				break;
			case 3:
				if(mem[i].cml.label.era == 2){
					printBase4(mem[i].cml.label.addr + startIcValue,10, fp);
				}else{
					printBase4(mem[i].cml.label.addr, 10, fp);
				}
				printBase4(mem[i].cml.label.era, 2, fp);
				break;
			case 4:
				printBase4(mem[i].cml.number.addr, 10, fp);
				printBase4(mem[i].cml.number.era, 2, fp);
				break;
			case 5:
				temp = mem[i].cml.reg.dest;
				if((mem[i].cml.reg.src & 1) == 1)
					temp = mem[i].cml.reg.dest | 64;
				printBase4(mem[i].cml.reg.src >> 1, 4, fp);
				printBase4(temp, 6, fp);
				printBase4(mem[i].cml.reg.era, 2, fp);
				break;
			case 6:
				printBase4(guidel[z++].s.ascii, 12, fp);
				break;
		}
		fprintf(fp, "\n");
	}
}

/*
 * This function initiate a line struct.
 * ln - the struct to initiate.
 */
void initiateLine(line ln[])
{
	int i = 0;
	for(; i < lineVar; i++){
		/*ln[i].word = malloc(sizeof(string) * maxLineLength);*/
		ln[i].word = "NULL";
		ln[i].wordIdx = 0;
	}
}

/*
 * This function commints the first step of the program.
 * fp - the name of the file that contains the assembler code.
 */
void firstStep(FILE* fp)
{
	int i = 0, z = 1, cmdNum, repeatCmd = 0; /* number of command*/
	int* addType;
	bool errFlag = FALSE, stepOneEnd = FALSE;
	line ln[lineVar], lastOperand;/* last read line*/
	initiateLine(ln);
	while(stepOneEnd == FALSE && ++lineNumber){
		z = 1;
		getLine(fp, ln, &stepOneEnd);
		QUE_setPtrToHead();
		if(strcmp(ln[z].word, "EMPTY") == 0)
			continue;
		if((ln[z].word[0] == ';' && strcmp(ln[0].word, "NULL") == 0) || (ln[0].word[0] == ';'))
			continue;
		if(isLabelOK(ln[z - 1]))
			insertToSymbolTable(labelTABLE, ln[z - 1].word, IC);
		if(ln[z].word[0] == '.'){ /* in case of guide sentence*/
			guideSentence(ln);
			continue;
		}
		if((cmdNum = checkCmd(ln[z], &repeatCmd)) == -1) continue;
		if(!isArgsNumFits(cmd[cmdNum].args, ln)) errFlag = TRUE;
		addType = calloc(cmd[cmdNum].args, sizeof(int));
		for(i = 0; i < cmd[cmdNum].args; i++){
			addType[i] = checkAddresingType(ln[++z].word);
			if(!isAddressingTypeOK(cmdNum, addType[i], i)) errFlag = TRUE;
			if(i == 0 && !(strcmp(ln[z].word, "$$") == 0)){
				lastOperand = ln[z];
				lastOperand.wordIdx = addType[0];
			}
		}
		if(errFlag == FALSE){
			setLineInMemory(cmdNum, addType, ln[2],ln[3], lastOperand);
			if(repeatCmd == 2){
				setLineInMemory(cmdNum, addType, ln[2],ln[3], lastOperand);
				if(addType[0] == 1 || addType[0] == 1){
					if(mem[IC- cmd[cmdNum].args].fieldNum == 1)
						dupLineLabels(IC- cmd[cmdNum].args );
					else
						dupLineLabels(IC- cmd[cmdNum].args - 1);
				}
			}
		}else errFlag = TRUE;
		free(addType);
	}
	mem[IC].fieldNum = -1;
}

/**
 * This function add to the queue the last labels that were on the last read line.
 * line - the line of the program.
 */
void dupLineLabels(int line)
{
	int counter = returnNodesNumber() - returnPtrNodeNumber();
	while(counter--){
		addToQueue(QUE_getLabel(), line);
		QUE_getNext();
	}
}

/**
 * This function checks if the number of arguments on the line is what supposed to be.
 * varNumber - the number of variables that need to be in the command.
 * ln - the last line.
 * returns TRUE if the number of variables is correct , FALSE otherwise.
 */
int isArgsNumFits(int varNumber, line ln[])
{
	int counter = 0, i = 2;
	for(;strcmp(ln[i].word, "NULL") != 0 && i < lineVar; i++, counter++);
	if(counter > varNumber)
		fprintf(stderr, "\n[line %d]: There is Too much variables on line.", lineNumber);
	if(counter < varNumber){
		fprintf(stderr, "\n[line %d]: There is a variable missing.", lineNumber);
		return FALSE;
	}
	return TRUE;
}

/**
 * This function checks if the label stands on standards. if it doesn't - the function prints error note.
 * ln - the last line.
 * returns TRUE if the label is fine, otherwisr - FALSE.
 */
int isLabelOK(line ln)
{
	int flag = 0;
	if(strcmp(ln.word, "NULL") == 0)
		return FALSE;
	if(!isalpha(ln.word[0]))
		fprintf(stderr, "\n[line %d]: First character of label must be a letter", lineNumber);
	else flag++;
	if(strlen(ln.word) > maxStringLength)
		fprintf(stderr, "\n[line %d]: The label length is longer than the maximum available (%d)",lineNumber, maxStringLength);
	else flag++;
	if(isRegister(ln.word))
		fprintf(stderr, "\n[line %d]: The name of the label %s is equal to register. YOU CANNOT USE THIS NAME", lineNumber , ln.word);
	else flag++;
	if(getLabelAddress(ln.word, labelTABLE) != -1)
		fprintf(stderr, "\n[line %d]: The label %s is already exists on program", lineNumber ,ln.word);
	else flag++;
	if(flag == 4)
		return TRUE;
	return FALSE;
}

/**
 * This function returns TRUE if the string stands on standards of label.
 * ln - the struct that contains the string word.
 * returns TRUE if the string stands on standards of label.
 */
int isLabel(line ln)
{
	if(ln.wordIdx > tabSpaces - 1)
		return FALSE;
	if(ln.word[strlen(ln.word) - 1] !=  ':')
		return FALSE;
	else
		ln.word[strlen(ln.word) - 1] = '\0';
	return TRUE;
}

/**
 * This function checks if the addressing type is ok.
 * cmdNum - the number of the command.
 * adrType - the addressing type.
 * idx - represents which of the variables is checked. value 0 - source, 1- destination
 */
int isAddressingTypeOK(int cmdNum, int adrType, int idx)
{
	char type = (char)(adrType + '0');
	string srcAd = strchr(cmd[cmdNum].srcAddressing, type);
	string destAd = strchr(cmd[cmdNum].destAddressing, type);
	if(cmd[cmdNum].args == 1)
		idx = 1;
	if(idx == 0 && srcAd == NULL){
		fprintf(stderr, "\n[line %d]: The source addressing type is not valid. ", lineNumber);
		return FALSE;
	}
	if(idx != 0 && destAd == NULL){
		fprintf(stderr, "\n[line %d]: The dest addressing type is not valid. ", lineNumber);
		return FALSE;
	}
	return TRUE;
}

/*
 * This fucntion commit the second step of the program.
 * adding addresses to the three symbol tables.
 */
void secondStep()
{
	int memIdx = 0, labelAddress = 0;
	IC = 0; DC = 0; lineNumber = 0;
	QUE_setPtrToHead();
	while(strcmp(QUE_getLabel(),"NULL") != 0 && memIdx != -1)
	{
		if((labelAddress = getLabelAddress(QUE_getLabel(), labelTABLE)) != -1){
			memIdx = setLabelAddressInMemory(memIdx, labelAddress); /* update label address on memory*/
			QUE_setIsExtern(FALSE);
		}else{
			if(getLabelAddress(QUE_getLabel(), externTABLE) == -1)
				fprintf(stderr, "\n[line %d]: the label %s is not exsists on file ",QUE_getLine(), QUE_getLabel());
			else{
				QUE_setIsExtern(TRUE);
				if(mem[QUE_getLine() + 1].fieldNum == 3 && mem[QUE_getLine() + 1].cml.label.addr == 0){
					mem[QUE_getLine() + 1].cml.label.era = 1;
					memIdx = QUE_getLine() + 2;
					QUE_setLine(QUE_getLine() + 1);
				}else{
					mem[QUE_getLine() + 2].cml.label.era = 1;
					memIdx = QUE_getLine() + 3;
					QUE_setLine(QUE_getLine() + 2);
				}
			}
		}
		QUE_getNext();
	}
	seAddressEnEX(entryTABLE);
	seAddressEnEX(externTABLE);
}

/**
 * This function search for a label line on memory and enters the address of the label.
 * memIdx - the function will search from this index to end
 * address - the address of the label.
 */
int setLabelAddressInMemory(int memIdx, int address)
{
	while(mem[memIdx].fieldNum != -1){
		if(mem[memIdx].fieldNum == 3){
			mem[memIdx].cml.label.addr = address;
			return memIdx + 1;
		}
		memIdx++;
	}
	return -1;
}

/**
 * This function handles the guide sentences.
 * ln - The last line.
 */
void guideSentence(line ln[])
{
	int i = 0;
	if(strcmp(ln[1].word, ".data") == 0){
		IC += ln[1].wordIdx;
		DC += ln[1].wordIdx;
	}
	if(strcmp(ln[1].word, ".string") == 0){
		if(ln[2].word[0] != '\"' || ln[2].word[strlen(ln[2].word) -1] != '\"')
			fprintf(stderr, "\n[line %d]: String without quotes. ",lineNumber);
		for(i = 1; i < strlen(ln[2].word) -1; i++, IC++, DC++){
			guidel[DC].s.ascii = ln[2].word[i];
			mem[IC].cml.isString = TRUE;
			mem[IC].fieldNum = 6;
		}
		guidel[DC++].s.ascii = 0;
		mem[IC].cml.isString = TRUE;
		mem[IC++].fieldNum = 6;
	}
	if(strcmp(ln[1].word, ".entry") == 0)
		insertToSymbolTable(entryTABLE, ln[2].word, 0);
	if(strcmp(ln[1].word, ".extern") == 0)
		insertToSymbolTable(externTABLE, ln[2].word, 0);
}

/**
 * This function checks if the char is space.
 * c - character to check.
 * stepOneEnd - boolean pointer
 * returns 1 in case of space , -1 in case of newline, 0 in case on no break.
 * changes the value of the boolean variable if the char is EOF.
 */
int isBreak(char c, bool* stepOneEnd)
{
	if(c == '\0')
		*stepOneEnd = TRUE;
	if(c == ' ' || c == ',' || c == '\r' || c == '\0')
		return 1;
	if(c == '\t')
		return tabSpaces;
	if(c == '\n')
			return -1;
	return 0;
}

/**
 * This function returns the next index of break.
 * str - the string.
 * idx - start index.
 * stepOneEnd - boolean pointer (is EOF?)
 * returns the next index of break.
 */
int returnNextBreakIdx(string str, int idx, bool* stepOneEnd)
{
	while(!isBreak(str[idx], stepOneEnd))
		idx++;
	return idx;
}

/**
 * This function returns the index of the command.
 * l - contains the string of the command.
 * repeatCmd - pointer to int that indicates if the command committed more than once.
 * returns the index of the command.
 */
int checkCmd(line l, int* repeatCmd)
{
	int i = 0;
	if(between1TO10(l.word[strlen(l.word) - 1] - '0')){
		(*repeatCmd) = l.word[strlen(l.word) - 1] - '0';
		l.word[strlen(l.word) - 1] = '\0';
	}
	for(; i < numOfCommands + 1; i++ )
		if(strcmp(l.word, cmd[i].name) == 0)
			return i;
	fprintf(stderr, "\n[line %d]: The command %s is unknown", lineNumber, l.word);
	return -1;
}

/**
 * This function returns TRUE if the num is between 1 to 10, false otherwise.
 * num - the number.
 */
int between1TO10(int num)
{
	if(num > 0 && num < 10)
		return TRUE;
	return FALSE;
}

/**
 * This function returns the type of the addressing
 * 0 - number , 1 - label, 2- last operand, 3- register
 * str - the string.
 */
int checkAddresingType(string str)
{
	if(str[0] == '#')
		return 0;
	if(str[0] == '$' && str[1] == '$')
		return 2;
	if(isRegister(str))
		return 3;
	addToQueue(str, IC);
	return 1;
}

/**
 * This function returns TRUE if the string is register or FALSE otherwise.
 * str - the string.
 */
int isRegister(string str)
{
	int i = 0;
	for(; i < numOfRegisters; i++)
		if(strcmp(registers[i], str) == 0)
			return TRUE;
	return FALSE;
}

/**
 * This function set line in memory.
 * cmdNum - the index of the command.
 * addType - the addressing types.
 * first - the first variable.
 * second - the second variable.
 * lastOperand - the last operand.
 */
void setLineInMemory(int cmdNum, int addType[], line first, line second, line lastOperand)
{
	mem[IC].cml.cml.era = 0;
	mem[IC].cml.cml.opcode = cmd[cmdNum].value;
	mem[IC].cml.cml.group = cmd[cmdNum].args;
	mem[IC].fieldNum = 1;
	if(cmd[cmdNum].args == 2){
		mem[IC].cml.cml.src =  addType[0];
		mem[IC].cml.cml.dest =  addType[1];
		setVariableOnMemory(IC + 1, addType[0], first, lastOperand);
		if((addType[0] == addType[1] && addType[1] == 3) ||(addType[0] == 2 && addType[1] == 3	&& lastOperand.wordIdx == 3))
			mem[IC--].cml.cml.src =  addType[1];
		setVariableOnMemory(IC + 2, addType[1], second, lastOperand);
	}else{
		if(cmd[cmdNum].args == 1){
			mem[IC].cml.cml.dest =  addType[0];
			mem[IC].cml.cml.src =  0;
			setVariableOnMemory(IC + 1, addType[0], first, lastOperand);
		}else{
			mem[IC].cml.cml.src =  0;
			mem[IC].cml.cml.dest = 0;
		}
	}
	IC += cmd[cmdNum].args + 1;
}

/**
 * This function set the variables on memory.
 * idx - the index in memory.
 * type - the addressing type
 * word - the variable.
 * lastOperand - the last operand.
 */
void setVariableOnMemory(int idx, int type, line word, line lastOperand)
{
	string temp = word.word;
	if(type == 0){
		mem[idx].cml.number.addr = atoi(++temp);
		mem[idx].cml.number.era = 0;
		mem[idx].fieldNum = 4;
	}
	if(type == 1){
		mem[idx].cml.label.addr = 0; /* we will update this value on the second step*/
		mem[idx].cml.label.era = 2;
		mem[idx].fieldNum = 3;
	}
	if(type == 2){
		if(strcmp(lastOperand.word, "NULL") != 0 )
			setVariableOnMemory(idx, lastOperand.wordIdx, lastOperand, lastOperand);
		else
			fprintf(stderr, "\n[line %d]: You cannot use $$ in first command", lineNumber);
	}
	if(type == 3){
		if(idx - IC == 1)
			mem[idx].cml.reg.src = word.word[1] - '0';
		else
			mem[idx].cml.reg.dest = word.word[1] - '0';
		mem[idx].fieldNum = 5;
	}
}

/**
 * This function is reading the next word from a string.
 * stream - the string.
 * idx - start index.
 * stepOneEnd - boolean pointer. (is EOF?)
 * returns the next word.
 */
void readNextWord(string stream, int* idx, bool* stepOneEnd, line* ln)
{
	/*line* ln = malloc(sizeof(line));*/
	(*ln).word = "EMPTY";
	(*ln).wordIdx = 0;
	int nextBreak = 0, z = 0;
	while((z = isBreak(stream[*idx], stepOneEnd)) > 0){
		(*idx)++;
		(*ln).wordIdx += z;
	}
	if(z == -1 || (*idx) > strlen(stream))
			return;
	nextBreak = returnNextBreakIdx(stream, (*idx), stepOneEnd);
	(*ln).word = malloc(sizeof(string));
	strncpy((*ln).word ,stream + (*idx), nextBreak - (*idx));
	(*ln).word[nextBreak - (*idx)] = (char)'\0';
	(*idx) = nextBreak + 1;
	/*return (*ln);*/
}

/**
 * This function handles the data input.
 * idx - the current index on the stream.
 * stream - contains string with the last line.
 * stepOneEnd - boolean pointer. (is EOF?)
 * return the number of  new variables on memory.
 */
int handleData(int idx, string stream, bool* stepOneEnd)
{
	int startIC = IC;
	line ln;
	while(idx < strlen(stream)){
		readNextWord(stream, &idx, stepOneEnd, &ln);
		if(strcmp(ln.word, "EMPTY") == 0 )
				break;
		guidel[DC++].d.num = atoi(ln.word);
		mem[IC].cml.isData = TRUE;
		mem[IC++].fieldNum = 2;
	}
	startIC = IC - startIC;
	DC -= startIC;
	IC -= startIC;
	return startIC;
}

/**
 * This function returns the next line on file.
 * fp - the file.
 * ln - empty array , will contain the line.
 * stepOneEnd - boolean pointer. (is EOF?)
 */
void getLine(FILE* fp, line ln[], bool* stepOneEnd)
{
	string startline = malloc(sizeof(char) * maxLineLength);
	int idx = 0, i = 1;
	fgets(startline, maxLineLength, fp);
	initiateLine(ln);
	while(idx < strlen(startline) && i < lineVar){
		readNextWord(startline, &idx, stepOneEnd, &ln[i]);
		if(strcmp(ln[i].word, "EMPTY") == 0)
			break;
		if(i == 1 && isLabel(ln[i])){
			ln[i].word[idx - 2] = (char)'\0';
			ln[--i].word = malloc(sizeof(string));
			strcpy(ln[i].word, ln[i + 1].word);
			ln[i].wordIdx = ln[i + 1].wordIdx;
		}
		if(strcmp(ln[i].word, ".data") == 0){
			ln[i].wordIdx = handleData(idx, startline, stepOneEnd);
			break;
		}
		if(idx < strlen(startline))
			ln[i + 1].wordIdx = ln[i].wordIdx;
		i++;
	}
	free(startline);
}
