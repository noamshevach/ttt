#ifndef PROJ_H
#define PROJ_H

#define numOfLetters 26
#define maxStringLength 30
#define memorySize 1000
#define numOfCommands 15
#define numOfRegisters 8
#define startIcValue 100
#define maxFilesOnRun 10
#define maxLineLength 80
#define lineVar 5
#define obEndOfFile ".ob"
#define asEndOfFile  ".as"
#define entEndOfFile  ".ent"
#define extEndOfFile  ".ext"
#define tabSpaces 8

typedef char* string;
typedef enum boolean { FALSE,TRUE} bool;

/*registers*/
string registers[] = {"r0","r1","r2","r3","r4","r5","r6","r7"};

/*regular command line*/
typedef struct regcommandline {
	unsigned int group:2;
	unsigned int opcode :4;
	unsigned int dest :2;
	unsigned int src :2;
	unsigned int era :2;
} regcommandline;

/*adress of label*/
typedef struct address{
	unsigned int addr :10;
	unsigned int era :2;
} address;

/*register memory representation*/
typedef struct sregister
{
	unsigned int src :5;
	unsigned int dest :5;
	unsigned int era :2;
} sregister;

/* command line representation on memory*/
typedef union commandline
{
	regcommandline cml;
	enum boolean isData;
	address label; /* address of label*/
	address number; /* # - number*/
	sregister reg;
	enum boolean isString;
}commandline;

/* memory*/
typedef struct memory{
	commandline cml;
	short fieldNum;
}memory;

/* commands*/
struct{
	string name;
	int value;
	int args;
	string srcAddressing;
	string destAddressing;
} cmd[] = {
	{"mov",0, 2, "0123", "13"},
	{"cmp",1, 2, "0123", "0123"},
	{"add",2, 2, "0123", "13"},
	{"sub",3, 2, "0123", "13"},
	{"not",4, 1, "4", "13"},
	{"clr", 5, 1, "4", "13"},
	{"lea", 6, 2, "1", "13"},
	{"inc", 7, 1, "4", "13"},
	{"dec", 8, 1, "4", "13"},
	{"jmp", 9, 1, "4", "123"},
	{"bne", 10, 1, "4", "123"},
	{"red", 11, 1, "4", "123"},
	{"prn", 12, 1, "4", "0123"},
	{"jsr", 13, 1, "4", "1"},
	{"rts", 14, 0, "4", "4"},
	{"stop", 15, 0, "4", "4"}
};

/*guide lines*/
typedef struct data{
	signed int num :12;
} data;

typedef struct str{
	unsigned int ascii :12;
} str;

typedef union guideline
{
	data d;
	str s;
}guideline;

int IC = 0, DC = 0;
memory mem[memorySize];
guideline guidel[memorySize];
int lineNumber = 0;

/*line*/
typedef struct line
{
	string word;
	int wordIdx;
}line;

void getFileName(string fileNames[]);
string getSubString(string s, int start, int end);
void printBinary(int num, int digits, FILE* fp);
void printBase4(int num, int digits, FILE* fp);
void cast10To4(int num, FILE* fp);
void printMemory(string file);
void initiateLine(line ln[]);
void firstStep(FILE* fp);
void dupLineLabels(int line);
int isArgsNumFits(int varNumber, line ln[]);
int isLabelOK(line ln);
int isLabel(line ln);
int isAddressingTypeOK(int cmdNum, int adrType, int idx);
void secondStep();
int setLabelAddressInMemory(int memIdx, int address);
void guideSentence(line ln[]);
int isBreak(char c, bool* stepOneEnd);
int returnNextBreakIdx(string str, int idx, bool* stepOneEnd);
int checkCmd(line l, int* repeatCmd);
int between1TO10(int num);
int checkAddresingType(string str);
int isRegister(string str);
void setLineInMemory(int cmdNum, int addType[], line first, line second, line lastOperand);
void setVariableOnMemory(int idx, int type, line word, line lastOperand);
void getLine(FILE* fp, line ln[5],bool* stepOneEnd);
int handleData(int idx, string stream,bool* stepOneEnd);
void readNextWord(string stream, int* idx, bool* stepOneEnd, line* ln);


#endif
