#ifndef CF60210D_68B5_4BE0_BADA_F962EF54198B
#define CF60210D_68B5_4BE0_BADA_F962EF54198B

#include <iostream>
#include <vector>
#include<stack>
#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>
using namespace std;

// 词法分析器类型定义
typedef enum {
	ENDFILE, ERROR,
	PROGRAM, PROCEDURE, TYPE, VAR, IF,
	THEN, ELSE, FI, WHILE, DO, ENDWH,
	BEGIN, END, READ, WRITE, ARRAY, OF,
	RECORD, RETURN,
	INTEGER, CHAR,
	ID, INTC, CHARC,
	ASSIGN, EQ, LT, PLUS, MINUS,
	TIMES, OVER, LPAREN, RPAREN, DOT,
	COLON, strI, COMMA, LMIDPAREN, RMIDPAREN,
	UNDERANGE
} LexType;

// Token类定义
class MyToken {
public:
	int line;
	string str;
	LexType lexType;
	string lexTypeString;

	MyToken(int line, string str, LexType lexType = ID);
};

// 全局变量
unordered_map<string, LexType> reversedMap;
unordered_map<LexType, string> LexTypeMap;
unordered_map<string, LexType> sigLexTypeMap;
vector<MyToken> tokenList;
const char* srcAddr = "/Users/linxu/Desktop/编译原理课程设计/source.txt"; // 源代码文件
const char* tokenAddr = "/Users/linxu/Desktop/编译原理课程设计/token.txt"; // 词法分析结果文件
const char* errorAddr = "/Users/linxu/Desktop/编译原理课程设计/error.txt"; // 错误信息文件
const char* treeAddr = "/Users/linxu/Desktop/编译原理课程设计/tree.txt";
const char* objAddr = "/Users/linxu/Desktop/编译原理课程设计/mips.txt";

// MyToken构造函数实现
MyToken::MyToken(int line, string str, LexType lexType) {
	this->line = line;
	this->str = str;

	if (reversedMap.find(str) != reversedMap.end()) {
		this->lexType = reversedMap.find(str)->second;
		this->lexTypeString = LexTypeMap.find(this->lexType)->second;
	}
	else if (sigLexTypeMap.find(str) != sigLexTypeMap.end()) {
		this->lexType = sigLexTypeMap.find(str)->second;
		this->lexTypeString = LexTypeMap.find(this->lexType)->second;
	}
	else if (lexType == CHARC || lexType == ID || lexType == ERROR || lexType == INTC || lexType == ENDFILE) {
		this->lexType = lexType;
		this->lexTypeString = LexTypeMap.find(this->lexType)->second;
	}
}

// 初始化函数
static void init() {
	reversedMap.insert({ "program", PROGRAM });
	reversedMap.insert({ "type", TYPE });
	reversedMap.insert({ "var", VAR });
	reversedMap.insert({ "procedure", PROCEDURE });
	reversedMap.insert({ "begin", BEGIN });
	reversedMap.insert({ "end", END });
	reversedMap.insert({ "array", ARRAY });
	reversedMap.insert({ "of", OF });
	reversedMap.insert({ "record", RECORD });
	reversedMap.insert({ "if", IF });
	reversedMap.insert({ "then", THEN });
	reversedMap.insert({ "else", ELSE });
	reversedMap.insert({ "fi", FI });
	reversedMap.insert({ "while", WHILE });
	reversedMap.insert({ "do", DO });
	reversedMap.insert({ "endwh", ENDWH });
	reversedMap.insert({ "read", READ });
	reversedMap.insert({ "write", WRITE });
	reversedMap.insert({ "return", RETURN });
	reversedMap.insert({ "integer", INTEGER });
	reversedMap.insert({ "char", CHAR });

	LexTypeMap.insert({ ENDFILE, "ENDFILE" });
	LexTypeMap.insert({ ERROR, "ERROR" });
	LexTypeMap.insert({ PROGRAM, "PROGRAM" });
	LexTypeMap.insert({ PROCEDURE, "PROCEDURE" });
	LexTypeMap.insert({ TYPE, "TYPE" });
	LexTypeMap.insert({ VAR, "VAR" });
	LexTypeMap.insert({ IF, "IF" });
	LexTypeMap.insert({ THEN, "THEN" });
	LexTypeMap.insert({ ELSE, "ELSE" });
	LexTypeMap.insert({ FI, "FI" });
	LexTypeMap.insert({ WHILE, "WHILE" });
	LexTypeMap.insert({ DO, "DO" });
	LexTypeMap.insert({ ENDWH, "ENDWH" });
	LexTypeMap.insert({ BEGIN, "BEGIN" });
	LexTypeMap.insert({ END, "END" });
	LexTypeMap.insert({ READ, "READ" });
	LexTypeMap.insert({ WRITE, "WRITE" });
	LexTypeMap.insert({ ARRAY, "ARRAY" });
	LexTypeMap.insert({ OF, "OF" });
	LexTypeMap.insert({ RECORD, "RECORD" });
	LexTypeMap.insert({ RETURN, "RETURN" });
	LexTypeMap.insert({ INTEGER, "INTEGER" });
	LexTypeMap.insert({ CHAR, "CHAR" });
	LexTypeMap.insert({ ID, "ID" });
	LexTypeMap.insert({ INTC, "INTC" });
	LexTypeMap.insert({ CHARC, "CHARC" });
	LexTypeMap.insert({ ASSIGN, "ASSIGN" });
	LexTypeMap.insert({ EQ, "EQ" });
	LexTypeMap.insert({ LT, "LT" });
	LexTypeMap.insert({ PLUS, "PLUS" });
	LexTypeMap.insert({ MINUS, "MINUS" });
	LexTypeMap.insert({ TIMES, "TIMES" });
	LexTypeMap.insert({ OVER, "OVER" });
	LexTypeMap.insert({ LPAREN, "LPAREN" });
	LexTypeMap.insert({ RPAREN, "RPAREN" });
	LexTypeMap.insert({ DOT, "DOT" });
	LexTypeMap.insert({ COLON, "COLON" });
	LexTypeMap.insert({ strI, "strI" });
	LexTypeMap.insert({ COMMA, "COMMA" });
	LexTypeMap.insert({ LMIDPAREN, "LMIDPAREN" });
	LexTypeMap.insert({ RMIDPAREN, "RMIDPAREN" });
	LexTypeMap.insert({ UNDERANGE, "UNDERANGE" });

	sigLexTypeMap.insert({ "(", LPAREN });
	sigLexTypeMap.insert({ ")", RPAREN });
	sigLexTypeMap.insert({ "[", LMIDPAREN });
	sigLexTypeMap.insert({ "]", RMIDPAREN });
	sigLexTypeMap.insert({ ";", strI });
	sigLexTypeMap.insert({ ",", COMMA });
	sigLexTypeMap.insert({ "+", PLUS });
	sigLexTypeMap.insert({ "-", MINUS });
	sigLexTypeMap.insert({ "*", TIMES });
	sigLexTypeMap.insert({ "/", OVER });
	sigLexTypeMap.insert({ "<", LT });
	sigLexTypeMap.insert({ ".", DOT });
	sigLexTypeMap.insert({ "..", UNDERANGE });
	sigLexTypeMap.insert({ ":=", ASSIGN });
	sigLexTypeMap.insert({ "=", EQ });
}

// 判断函数
static bool isDigit(char ch) {
	return ch >= '0' && ch <= '9';
}

static bool isLetter(char ch) {
	return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}

static bool isBlank(char ch) {
	return ch == ' ' || ch == '\n' || ch == '\t' || ch == '\r';
}

static bool isOperator(char ch) {
	return ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '<' || ch == '=';
}

static bool isSeparator(char ch) {
	return ch == ';' || ch == ',' || ch == '{' || ch == '}' || ch == '[' ||
		ch == ']' || ch == '(' || ch == ')' || ch == '.' || ch == '\'' || ch == ':';
}

// 词法分析主函数
bool lexicalAnalysis() {
	int line = 1;
	bool error = false;
	FILE* fp = fopen(srcAddr, "r");
	if (fp == nullptr) {
		cout << "打开源文件失败" << endl;
		return true;
	}

	char ch = fgetc(fp);
	string word = "";

	while (ch != EOF) {
		if (isLetter(ch)) {
			word = "";
			word += ch;
			ch = fgetc(fp);

			while (isLetter(ch) || isDigit(ch)) {
				word += ch;
				ch = fgetc(fp);
			}

			MyToken token(line, word);
			tokenList.push_back(token);
		}
		else if (isDigit(ch)) {
			word = "";
			word += ch;
			ch = fgetc(fp);

			while (isDigit(ch) || isLetter(ch)) {
				word += ch;
				ch = fgetc(fp);
			}

			bool flag = true;
			for (auto c : word) {
				if (isLetter(c)) {
					flag = false;
					error = true;
					MyToken token(line, "数字开头的单词！", ERROR);
					tokenList.push_back(token);
					break;
				}
			}

			if (flag) {
				MyToken token(line, word, INTC);
				tokenList.push_back(token);
			}
		}
		else if (isOperator(ch)) {
			string tmp = "";
			tmp += ch;
			MyToken token(line, tmp);
			tokenList.push_back(token);
			ch = fgetc(fp);
		}
		else if (isSeparator(ch)) {
			word = "";

			if (ch == '{') {
				while (ch != '}') {
					ch = fgetc(fp);
					if (ch == '\n') line++;
				}
				ch = fgetc(fp);
			}
			else if (ch == '.') {
				word += ch;
				if ((ch = fgetc(fp)) == '.') {
					word += ch;
					MyToken token(line, word);
					tokenList.push_back(token);
					ch = fgetc(fp);
				}
				else {
					MyToken token(line, word);
					tokenList.push_back(token);
				}
			}
			else if (ch == '\'') {
				string tmp = "";
				while ((ch = fgetc(fp)) != '\'') {
					tmp += ch;
				}
				MyToken token(line, tmp, CHARC);
				tokenList.push_back(token);
				ch = fgetc(fp);
			}
			else if (ch == ':') {
				word += ch;
				if ((ch = fgetc(fp)) == '=') {
					word += ch;
					MyToken token(line, word);
					tokenList.push_back(token);
					ch = fgetc(fp);
				}
				else {
					error = true;
					MyToken token(line, "=出现错误", ERROR);
					tokenList.push_back(token);
					ch = fgetc(fp);
				}
			}
			else {
				string tmp = "";
				tmp += ch;
				MyToken token(line, tmp);
				tokenList.push_back(token);
				ch = fgetc(fp);
			}
		}
		else if (isBlank(ch)) {
			if (ch == '\n') {
				line++;
			}
			ch = fgetc(fp);
		}
		else {
			error = true;
			MyToken token(line, "未知字符", ERROR);
			tokenList.push_back(token);
			ch = fgetc(fp);
		}
	}

	MyToken token(++line, "ENDFILE", ENDFILE);
	tokenList.push_back(token);
	fclose(fp);
	return error;
}

// 打印错误信息
void printErrorMsg() {
	ofstream output(errorAddr);
	for (MyToken t : tokenList) {
		if (t.lexType == ERROR) {
			cout << t.line << "行" << '\t' << t.str << endl;
			output << t.line << "行" << '\t' << t.str << '\n';
		}
	}
	output.close();
}

// 打印Token列表
void printTokenList() {
	ofstream output(tokenAddr);
	for (MyToken t : tokenList) {
		cout << t.line << '\t' << t.lexTypeString << '\t' << t.str << endl;
		output << t.line << '\t' << t.lexTypeString << '\t' << t.str << '\n';
	}
	output.close();
}

// 词法分析主流程
void lexicial() {
	init();
	if (lexicalAnalysis()) {
		printErrorMsg();
		cout << "词法分析发现错误，程序即将退出!" << endl;
	}
	else {
		printTokenList();
		cout << "词法分析完成，结果已输出到文件!" << endl;
	}
}

/// <summary>
/// 下面是语法分析程序
/// </summary>

enum Treenodecate {	//语法树节点类型
	ProK, PheadK, TypeK, VarK, ProcDecK, StmLK, DecK, StmtK, ExpK
};

enum DecKcate {	//语法树节点的声明类型
	ArrayK, CharK, IntegerK, RecordK, IdK, BoolK
};

enum StmtKcate {	//语法树节点的语句类型
	IfK, WhileK, AssignK, ReadK, WriteK, CallK, ReturnK
};

enum ExpKcate {	//语法树节点的表达式类型
	OpK, ConstK, IdEK
};

enum class ParamTcate {	//valparamtype或者varparamtype,表示过程的参数是值参还是变参
	valparamtype, varparamtype
};

enum class Varkind {	//变量的类别
	IdV, ArrayMembV, FieldMembV
};

enum class ExpType {	//语法树节点的检查类型
	Void, Integer, Boolean
};

enum class ErrorType {
	MissingProgramHeader, MissingDeclaration, MissingProgramBody, SyntaxError
};

struct SymbolsTable;	//符号表

struct ArrayAttrx {	//数组类型的属性
	int low;		//整数类型变量，记录数组的下界。
	int up;			//整数类型变量，记录数组的上界。
	DecKcate childType;	//记录数组的成员类型。
};

struct ExpAttrx {
	LexType op;		//记录语法树节点的运算符单词，为单词类型
	int val;		//记录语法树节点的数值，当语法树节点为“数字因子"对应的语法树节点时有效,为整数类型
	Varkind varkind;//记录变量的类别
	ExpType type;	//记录语法树节点的检查类型,取值 Void,Integer, Boolean,为类型检查ExpType类型
};

struct Attr {
	ArrayAttrx ArrayAttr;	//记录数组类型的属性
	ParamTcate paramt;		//记录过程的参数类型
	ExpAttrx ExpAttr;		//记录表达式的属性
};

class Treenode;
using TreenodePtr = unique_ptr<Treenode>;
const string NodeKind_str[] = { "ProK","PheadK","TypeK","VarK","ProcDecK","StmLK","DecK","StmtK","ExpK" };
const string Deck_str[] = { "ArrayK","CharK","IntegerK","RecordK","IdK" };
const string StmtK_str[] = { "IfK","WhileK","AssignK","ReadK","WriteK","CallK","ReturnK" };
const string ExpK_str[] = { "OpK","ConstK","IdK" };
const string LexType_str[] = { "endfile","error","program","procedure","type","var","if"
		,"then","else","fi","while","do","endwh","begin","end","read","write","array","of",
							  "record","return","integer","char","id","intc","charc",":=","=","<","+",
							  "-","*","/","(",")",".","colon","stri",",","[","]",".." };

class Treenode {
public:
	TreenodePtr child[3]; //指向子语法树节点指针，为语法树节点指针类型
	TreenodePtr sibling;	//指向兄弟语法树节点指针，为语法树节点指针类型
	int lineno;			//记录源程序行号，为整数类型
	Treenodecate nodekind;//记录语法树节点类型
	union {
		DecKcate dec;	//记录语法树节点的声明类型，当nodekind=DecK时 有效
		StmtKcate stmt;	//记录语法树节点的语句类型，当nodekind=StmtK时有效
		ExpKcate exp;	//记录语法树节点的表达式类型，当nodekind=ExpK时有效
	}kind;				//记录语法树节点的具体类型，为共用体结构
	int idnum = 0;		//记录一个节点中的标志符的个数
	int level = 0;
	std::vector<std::string> name;	//字符串数组。数组成员是节点中的标志符的名字
	std::vector<SymbolsTable*> table;	//指针数组，数组成员是节点中的各个标志符在符号表中的入口
	string type_name;	//记录类型名，当节点为声明类型，且类型是由类型标志符表示时有效
	Attr attr;			//成员attr记录语法树节点其他属性，为结构体类型

	Treenode() = default;

	static TreenodePtr create(Treenodecate kind, int lineno) {
		auto node = make_unique<Treenode>();
		node->nodekind = kind;
		node->lineno = lineno;
		return node;
	}

	void addChild(int index, TreenodePtr children) {
		if (index >= 0 && index < 3) child[index] = move(children);
	}

	void addSibling(TreenodePtr sib) {
		Treenode* current = this;
		while (current->sibling) current = current->sibling.get();
		current->sibling = move(sib);
	}

	void addName(const string& id) {
		idnum++;
		name.push_back(id);
	}
};



class Parser {
private:
	vector<MyToken> tokens;
	size_t currentIndex = 0;
	stringstream errorLog;

	MyToken& currentToken() { return tokens[currentIndex]; }

	void consumeToken() { if (currentIndex < tokens.size() - 1) currentIndex++; }

	void logError(ErrorType type, const string& details = "") {
		errorLog << "Line " << currentToken().line << ": ";
		switch (type) {
		case ErrorType::MissingProgramHeader:
			errorLog << "Missing program header"; cout<<errorLog.str()<<endl; break;
		case ErrorType::MissingDeclaration:
			errorLog << "Missing declaration"; cout<<errorLog.str()<<endl; break;
		case ErrorType::MissingProgramBody:
			errorLog << "Missing program body"; cout<<errorLog.str()<<endl; break;
		default:
			errorLog << "Syntax error"; cout<<errorLog.str()<<endl; break;
		}
		if (!details.empty()) {errorLog << " (" << details << ")"; cout<<errorLog.str()<<endl;}
		errorLog << endl;
	}

public:
	explicit Parser(vector<MyToken> tokenList) : tokens(move(tokenList)) {}

	TreenodePtr parse() {
		return program();
	}

	std::string lexTypeToString(LexType type) {
		switch (type) {
		case LexType::PROGRAM:    return "PROGRAM";
		case LexType::PROCEDURE:  return "PROCEDURE";
		case LexType::TYPE:       return "TYPE";
		case LexType::VAR:        return "VAR";
		case LexType::IF:         return "IF";
		case LexType::THEN:       return "THEN";
		case LexType::ELSE:       return "ELSE";
		case LexType::FI:         return "FI";
		case LexType::WHILE:      return "WHILE";
		case LexType::DO:         return "DO";
		case LexType::ENDWH:      return "ENDWH";
		case LexType::BEGIN:      return "BEGIN";
		case LexType::END:        return "END";
		case LexType::READ:       return "READ";
		case LexType::WRITE:      return "WRITE";
		case LexType::ARRAY:      return "ARRAY";
		case LexType::OF:         return "OF";
		case LexType::RECORD:     return "RECORD";
		case LexType::RETURN:     return "RETURN";
		case LexType::INTEGER:    return "INTEGER";
		case LexType::CHAR:       return "CHAR";
		case LexType::ID:         return "IDENTIFIER";
		case LexType::INTC:       return "INTEGER_CONST";
		case LexType::CHARC:      return "CHAR_CONST";
		case LexType::ASSIGN:    return ":=";
		case LexType::EQ:         return "=";
		case LexType::LT:         return "<";
		case LexType::PLUS:       return "+";
		case LexType::MINUS:      return "-";
		case LexType::TIMES:      return "*";
		case LexType::OVER:       return "/";
		case LexType::LPAREN:     return "(";
		case LexType::RPAREN:     return ")";
		case LexType::DOT:       return ".";
		case LexType::COLON:      return ":";
		case LexType::strI:       return ";";
		case LexType::COMMA:      return ",";
		case LexType::LMIDPAREN:  return "[";
		case LexType::RMIDPAREN:  return "]";
		case LexType::UNDERANGE:  return "..";
		case LexType::ENDFILE:   return "END_OF_FILE";
		default:                  return "UNKNOWN_TOKEN";
		}
	}

	/*void printTree(Treenode* root) {

		ofstream outTree;
		outTree.open(treeAddr);

		stack<Treenode*> outStack;
		//outTree << "打印不出来符号！" << endl;
		root->level = 0;
		outStack.push(root);
		while (!outStack.empty()) {
			Treenode* t = outStack.top();
			outStack.pop();
			for (int i = 0; i < t->level; i++) {
				//为什么文件里打印不出来呢
				outTree << "  ┆  ";
				cout << "  ┆  ";
			}
			outTree << NodeKind_str[t->nodekind] << "  ";
			cout << NodeKind_str[t->nodekind] << "  ";
			switch (t->nodekind) {
			case Treenodecate::DecK:
				outTree << Deck_str[t->kind.dec] << "  ";
				cout << Deck_str[t->kind.dec] << "  ";
				break;
			case Treenodecate::StmtK:
				outTree << StmtK_str[t->kind.stmt] << "  ";
				cout << StmtK_str[t->kind.stmt] << "  ";
				break;
			case Treenodecate::ExpK:
				outTree << ExpK_str[t->kind.exp] << "  ";
				cout << ExpK_str[t->kind.exp] << "  ";
				if (t->kind.exp == ExpKcate::OpK) {
					//Opk
					outTree << LexType_str[t->attr.ExpAttr.op] << "  ";
					cout << LexType_str[t->attr.ExpAttr.op] << "  ";
				}
				if (t->kind.exp == ExpKcate::ConstK) {
					//Constk
					outTree << t->attr.ExpAttr.val << "  ";
					cout << t->attr.ExpAttr.val << "  ";
				}
				break;
			default:break;
			}
			for (int i = 0; i < t->idnum; i++) {
				outTree << t->name[i] << "  ";
				cout << t->name[i] << "  ";
			}
			if (t->nodekind == DecK) {
				if (t->kind.dec == ArrayK) {
					outTree << t->attr.ArrayAttr.low << ".." << t->attr.ArrayAttr.up << "  ";
					cout << t->attr.ArrayAttr.low << ".." << t->attr.ArrayAttr.up << "  ";
				}
			}
			outTree << t->level << endl;
			cout << t->level << endl;
			//有兄弟，兄弟入栈，注意顺序
			if (t->sibling != nullptr) {
				t->sibling->level = t->level;
				outStack.push((Treenode*)t->sibling);
			}
			//有儿子，儿子入栈，注意顺序
			for (int i = 2; i >= 0; i--) {
				if (t->child[i] != nullptr) {
					t->child[i]->level = t->level + 1;
					outStack.push(t->child[i]);
				}
			}
		}
		outTree.close();
	}*/

	void printTree(const TreenodePtr& root) {
		std::ofstream outTree(treeAddr);
		if (!outTree.is_open()) {
			std::cerr << "Error: Cannot open output file " << treeAddr << std::endl;
			return;
		}

		std::stack<Treenode*> nodeStack;
		root->level = 0;
		nodeStack.push(root.get());

		while (!nodeStack.empty()) {
			Treenode* current = nodeStack.top();
			nodeStack.pop();

			// 打印缩进
			for (int i = 0; i < current->level; ++i) {
				outTree << "  │  ";
				std::cout << "  │  ";
			}

			// 打印节点基本信息
			outTree << NodeKind_str[current->nodekind] << " ";
			std::cout << NodeKind_str[current->nodekind] << " ";

			// 处理不同节点类型
			switch (current->nodekind) {
			case Treenodecate::DecK:
				outTree << Deck_str[current->kind.dec] << " ";
				std::cout << Deck_str[current->kind.dec] << " ";
				if (current->kind.dec == DecKcate::ArrayK) {
					outTree << "[" << current->attr.ArrayAttr.low << ".."
						<< current->attr.ArrayAttr.up << "] ";
					std::cout << "[" << current->attr.ArrayAttr.low << ".."
						<< current->attr.ArrayAttr.up << "] ";
				}
				break;

			case Treenodecate::StmtK:
				outTree << StmtK_str[current->kind.stmt] << " ";
				std::cout << StmtK_str[current->kind.stmt] << " ";
				break;

			case Treenodecate::ExpK:
				outTree << ExpK_str[current->kind.exp] << " ";
				std::cout << ExpK_str[current->kind.exp] << " ";
				if (current->kind.exp == ExpKcate::OpK) {
					outTree << LexType_str[current->attr.ExpAttr.op] << " ";
					std::cout << LexType_str[current->attr.ExpAttr.op] << " ";
				}
				else if (current->kind.exp == ExpKcate::ConstK) {
					outTree << current->attr.ExpAttr.val << " ";
					std::cout << current->attr.ExpAttr.val << " ";
				}
				break;
			}

			// 打印标识符
			for (const auto& name : current->name) {
				outTree << name << " ";
				std::cout << name << " ";
			}

			// 打印层级
			outTree << "(Level: " << current->level << ")\n";
			std::cout << "(Level: " << current->level << ")\n";

			// 兄弟节点入栈（相同层级）
			if (current->sibling) {
				current->sibling->level = current->level;
				nodeStack.push(current->sibling.get());
			}

			// 子节点逆序入栈（保证正序处理）
			for (int i = 2; i >= 0; --i) {
				if (current->child[i]) {
					current->child[i]->level = current->level + 1;
					nodeStack.push(current->child[i].get());
				}
			}
		}

		outTree.close();
	}

private:
	TreenodePtr program() {
		auto root = Treenode::create(Treenodecate::ProK, currentToken().line);

		TreenodePtr head = programHead();
		if (!head) logError(ErrorType::MissingProgramHeader);
		root->addChild(0, move(head));

		TreenodePtr decls = declarePart();
		if (!decls) logError(ErrorType::MissingDeclaration);
		root->addChild(1, move(decls));

		TreenodePtr body = programBody();
		if (!body) logError(ErrorType::MissingProgramBody);
		root->addChild(2, move(body));

		match(DOT);
		return root;
	}

	TreenodePtr programHead() {
		auto head = Treenode::create(Treenodecate::PheadK, currentToken().line);
		match(PROGRAM);
		if (currentToken().lexType == ID) {
			head->addName(currentToken().str);
		}
		match(ID);
		return head;
	}

	TreenodePtr declarePart() {
		TreenodePtr typeP = nullptr, varP = nullptr;

		// 类型声明部分 TypeDecpart
		int line = currentToken().line;
		TreenodePtr tp1 = typeDecpart();
		if (tp1) {
			typeP = Treenode::create(Treenodecate::TypeK, line);
			typeP->addChild(0, move(tp1)); // TypeK 节点的第一个子节点是类型定义
		}

		// 变量声明部分 VarDecpart
		line = currentToken().line;
		TreenodePtr tp2 = varDecpart();
		if (tp2) {
			varP = Treenode::create(Treenodecate::VarK, line);
			varP->addChild(0, move(tp2)); // VarK 节点的第一个子节点是变量定义
		}

		// 过程声明部分 ProcDecpart
		TreenodePtr s = procDecpart();

		// 组合兄弟节点链
		if (!varP) {
			if (!typeP) {
				typeP = move(s); // 只有过程声明
			}
			else {
				typeP->addSibling(move(s)); // TypeK -> ProcDecK
			}
		}
		else {
			varP->addSibling(move(s));      // VarK -> ProcDecK
			if (!typeP) {
				typeP = move(varP);         // 只有变量和过程声明
			}
			else {
				typeP->addSibling(move(varP)); // TypeK -> VarK -> ProcDecK
			}
		}

		return typeP;
	}

	TreenodePtr typeDecpart() {
		TreenodePtr t = nullptr;

		if (currentToken().lexType == TYPE) {
			// 匹配 TYPE 声明
			t = typeDec(); // 假设 typeDec() 返回 TreenodePtr
		}
		else if (currentToken().lexType == VAR ||
			currentToken().lexType == PROCEDURE ||
			currentToken().lexType == BEGIN) {
			// 允许无类型声明，直接返回空指针
		}
		else {
			// 非法 Token 报错
			logError(ErrorType::SyntaxError,
				"Unexpected token " + lexTypeToString(currentToken().lexType) + " in type declaration");
		}

		return t;
	}

	TreenodePtr typeDec() {
		match(TYPE);
		TreenodePtr t = typeDecList();
		if (!t) {
			logError(ErrorType::MissingDeclaration, "Empty type declarations after TYPE");
		}
		return t;
	}

	TreenodePtr typeDecList() {
		auto t = Treenode::create(Treenodecate::DecK, currentToken().line);
		typeId(t);
		match(EQ);
		typeDef(t);
		match(strI);
		TreenodePtr p = typeDecMore();
		if (p)t->addSibling(move(p));
		return t;
	}

	TreenodePtr typeDecMore() {
		TreenodePtr t = nullptr;

		if (currentToken().lexType == ID) {
			// 递归解析后续类型声明
			t = typeDecList();
		}
		else if (currentToken().lexType == VAR ||
			currentToken().lexType == PROCEDURE ||
			currentToken().lexType == BEGIN) {
			// 允许无更多声明，不报错
		}
		else {
			logError(ErrorType::SyntaxError,
				"Unexpected token " + lexTypeToString(currentToken().lexType) + " after type declaration");
		}

		return t;
	}

	void typeId(TreenodePtr& t) {
		if (currentToken().lexType == ID && t != nullptr)t->addName(currentToken().str);
		match(ID);
	}

	void typeDef(TreenodePtr& t) {
		if (!t) {
			logError(ErrorType::SyntaxError, "TypeDef called with null node");
			return;
		}

		// 处理基础类型 (INTEGER/CHAR)
		if (currentToken().lexType == LexType::INTEGER || currentToken().lexType == LexType::CHAR) {
			baseType(t);
		}
		// 处理结构类型 (ARRAY/RECORD)
		else if (currentToken().lexType == LexType::ARRAY || currentToken().lexType == LexType::RECORD) {
			structureType(t);
		}
		// 处理用户定义类型 (ID)
		else if (currentToken().lexType == LexType::ID) {
			t->kind.dec = DecKcate::IdK;  // 设置声明类型为标识符
			t->addName(currentToken().str);
			t->type_name = currentToken().str;  // 记录类型名
			match(LexType::ID);
		}
		// 非法 Token 类型
		else {
			logError(ErrorType::SyntaxError,
				"Invalid type definition token: " + lexTypeToString(currentToken().lexType));
		}
	}

	void baseType(TreenodePtr& t) {
		if (currentToken().lexType == LexType::INTEGER) {
			t->kind.dec = DecKcate::IntegerK;  // 假设枚举值正确映射
			match(LexType::INTEGER);
		}
		else if (currentToken().lexType == LexType::CHAR) {
			t->kind.dec = DecKcate::CharK;     // 假设枚举值正确映射
			match(LexType::CHAR);
		}
	}

	void structureType(TreenodePtr& t) {
		if (currentToken().lexType == LexType::ARRAY) {
			arrayType(t);  // 假设已实现 arrayType 函数
		}
		else if (currentToken().lexType == LexType::RECORD) {
			t->kind.dec = DecKcate::RecordK;  // 先设置类型再处理子结构
			recType(t);    // 假设已实现 recType 函数
		}
	}

	void arrayType(TreenodePtr& t) {
		match(LexType::ARRAY);
		match(LexType::LMIDPAREN);
		if (currentToken().lexType == LexType::INTC) {
			t->attr.ArrayAttr.low = stoi(currentToken().str);
		}
		match(INTC);
		match(UNDERANGE);
		if (currentToken().lexType == LexType::INTC) {
			t->attr.ArrayAttr.up = stoi(currentToken().str);
		}
		match(INTC);
		match(RMIDPAREN);
		match(OF);
		baseType(t);
		t->attr.ArrayAttr.childType = t->kind.dec; // 保存元素类型
		t->kind.dec = DecKcate::ArrayK;
	}

	void recType(TreenodePtr& t) {
		match(LexType::RECORD);
		TreenodePtr p = fieldDecList();
		if (p) {
			t->addChild(0, move(p));
		}
		else {
			logError(ErrorType::MissingDeclaration, "Empty field list in RECORD");
		}
		match(LexType::END);
	}

	TreenodePtr fieldDecList() {
		auto t = Treenode::create(Treenodecate::DecK, currentToken().line);
		TreenodePtr p = nullptr;
		if (currentToken().lexType == LexType::INTEGER ||
			currentToken().lexType == LexType::CHAR) {

			baseType(t);     // 设置类型为 IntegerK/CharK
			idList(t);       // 解析标识符列表

			match(LexType::strI);
			p = fieldDecMore();  // 解析后续字段声明

		}
		else if (currentToken().lexType == LexType::ARRAY) {

			arrayType(t);    // 设置类型为 ArrayK 并解析数组属性
			idList(t);       // 解析标识符列表
			match(LexType::strI);
			p = fieldDecMore();
		}
		else {
			logError(ErrorType::SyntaxError,
				"Invalid field type: " + lexTypeToString(currentToken().lexType));
		}
		// 连接兄弟节点
		if (p) {
			t->addSibling(move(p));
		}
		return t;
	}

	TreenodePtr fieldDecMore() {
		TreenodePtr t = nullptr;

		if (currentToken().lexType != LexType::END) {
			// 检查是否还有字段声明
			if (currentToken().lexType == LexType::INTEGER ||
				currentToken().lexType == LexType::CHAR ||
				currentToken().lexType == LexType::ARRAY) {

				t = fieldDecList();  // 递归解析字段声明
			}
			else {
				logError(ErrorType::SyntaxError,
					"Unexpected token in field declaration: " +
					lexTypeToString(currentToken().lexType));
			}
		}

		return t;
	}

	void idList(TreenodePtr& t) {
		if (currentToken().lexType == LexType::ID) {
			t->addName(currentToken().str);  // 使用 addName 方法
		}
		match(LexType::ID);
		idMore(t);  // 处理后续标识符
	}

	void idMore(TreenodePtr& t) {
		if (currentToken().lexType == LexType::strI) {
			return;  // 分号由上层函数处理
		}
		// 逗号分隔的更多标识符
		else if (currentToken().lexType == COMMA) {
			match(COMMA);
			idList(t);
		}
		// 非法 Token 类型
		else {
			logError(ErrorType::SyntaxError,
				"Unexpected token after identifier: " +
				lexTypeToString(currentToken().lexType));
		}
	}

	TreenodePtr varDecpart() {
		TreenodePtr t = nullptr;

		if (currentToken().lexType == LexType::PROCEDURE ||
			currentToken().lexType == LexType::BEGIN) {
			// 允许无变量声明，不处理
		}
		else if (currentToken().lexType == LexType::VAR) {  // 尝试匹配 VAR
			t = varDec();  // 解析变量声明
		}
		else {
			logError(ErrorType::MissingDeclaration,
				"Expected VAR/PROCEDURE/BEGIN but got " +
				lexTypeToString(currentToken().lexType));
		}

		return t;
	}

	TreenodePtr varDec() {
		match(LexType::VAR);
		TreenodePtr t = varDecList();
		if (!t)logError(ErrorType::MissingDeclaration, "Empty variable declarations after VAR");
		return t;
	}

	TreenodePtr varDecList() {
		auto t = Treenode::create(Treenodecate::DecK, currentToken().line);
		TreenodePtr p = nullptr;
		typeDef(t);
		varIdList(t);
		match(strI);
		p = varDecMore();
		t->addSibling(move(p));
		return t;
	}

	TreenodePtr varDecMore() {
		TreenodePtr t = nullptr;

		if (currentToken().lexType == LexType::PROCEDURE ||
			currentToken().lexType == LexType::BEGIN) {
			// 允许无更多声明
		}
		// 检查是否为新变量声明的起始 Token
		else if (currentToken().lexType == LexType::INTEGER ||
			currentToken().lexType == LexType::CHAR ||
			currentToken().lexType == LexType::ARRAY ||
			currentToken().lexType == LexType::RECORD ||
			currentToken().lexType == LexType::ID) {
			t = varDecList(); // 递归解析变量声明
		}
		// 非法 Token 类型
		else {
			logError(ErrorType::SyntaxError,
				"Unexpected token in variable declaration: " +
				lexTypeToString(currentToken().lexType));
		}

		return t;
	}

	void varIdList(TreenodePtr& t) {
		if (currentToken().lexType == LexType::ID) {
			t->addName(currentToken().str);  // 自动管理 idnum 和 name
			match(LexType::ID);
		}
		else {
			logError(ErrorType::SyntaxError,
				"Missing identifier in variable declaration");
		}
		varIdMore(t);  // 处理后续标识符
	}

	void varIdMore(TreenodePtr& t) {
		if (currentToken().lexType == LexType::strI) {
			return;
		}
		// 逗号分隔更多标识符
		else if (currentToken().lexType == LexType::COMMA) {
			match(LexType::COMMA);
			varIdList(t);
		}
		// 非法 Token 类型
		else {
			logError(ErrorType::SyntaxError,
				"Unexpected token in variable list: " +
				lexTypeToString(currentToken().lexType));
		}
	}

	TreenodePtr procDecpart() {
		TreenodePtr t = nullptr;

		if (currentToken().lexType == LexType::BEGIN) {
			// 允许无过程声明，直接返回空
		}
		else if (currentToken().lexType == LexType::PROCEDURE) {  // 尝试匹配 PROCEDURE
			t = procDec();  // 解析过程声明
		}
		else {
			logError(ErrorType::MissingDeclaration,
				"Expected PROCEDURE/BEGIN but got " +
				lexTypeToString(currentToken().lexType));
		}

		return t;
	}

	TreenodePtr procDec() {
		auto t = Treenode::create(Treenodecate::ProcDecK, currentToken().line);
		match(LexType::PROCEDURE);
		if (currentToken().lexType == LexType::ID) {
			t->addName(currentToken().str);  // 存储过程名
			match(LexType::ID);
		}
		match(LexType::LPAREN);
		paramList(t);
		match(LexType::RPAREN);
		match(LexType::strI);
		t->addChild(1, procDecPart());  // 第二个子节点
		// 解析过程体 (BEGIN-END 块)
		t->addChild(2, procBody());     // 第三个子节点
		t->addSibling(move(procDecpart()));
		return t;
	}

	void paramList(TreenodePtr& t) {
		TreenodePtr p = nullptr;

		if (currentToken().lexType == LexType::RPAREN) {
			// 空参数列表，无需处理
		}
		// 检查有效参数起始 Token
		else if (currentToken().lexType == LexType::INTEGER ||
			currentToken().lexType == LexType::CHAR ||
			currentToken().lexType == LexType::ARRAY ||
			currentToken().lexType == LexType::RECORD ||
			currentToken().lexType == LexType::ID ||
			currentToken().lexType == LexType::VAR) {

			p = paramDecList();  // 解析参数声明链
			t->addChild(0, move(p)); // 参数列表作为第一个子节点

		}
		else {
			logError(ErrorType::SyntaxError,
				"Invalid parameter type: " + lexTypeToString(currentToken().lexType));
		}
	}

	TreenodePtr paramDecList() {
		TreenodePtr t = param();         // 第一个参数
		TreenodePtr p = paramMore();     // 更多参数

		if (p) {
			t->addSibling(move(p));  // 连接兄弟节点
		}

		return t;
	}

	TreenodePtr paramMore() {
		TreenodePtr t = nullptr;
		if (currentToken().lexType == LexType::RPAREN) {
			// 空参数列表，无需处理
		}
		else if (currentToken().lexType == LexType::strI) {  // 分号分隔参数
			match(LexType::strI);
			t = paramDecList();        // 递归解析后续参数
			if (!t) {
				logError(ErrorType::MissingDeclaration,
					"Missing parameter after ;");
			}
		}

		return t;
	}

	TreenodePtr param() {
		auto t = Treenode::create(Treenodecate::DecK, currentToken().line);
		if (currentToken().lexType == LexType::INTEGER ||
			currentToken().lexType == LexType::CHAR ||
			currentToken().lexType == LexType::ARRAY ||
			currentToken().lexType == LexType::RECORD ||
			currentToken().lexType == LexType::ID) {
			t->attr.paramt = ParamTcate::valparamtype;
			typeDef(t);
			formList(t);
		}
		else if (currentToken().lexType == LexType::VAR) {
			match(LexType::VAR);
			t->attr.paramt = ParamTcate::varparamtype;
			typeDef(t);
			formList(t);
		}
		return t;
	}

	void formList(TreenodePtr& t) {
		if (currentToken().lexType == LexType::ID) {
			t->addName(currentToken().str);  // 存储参数名
			match(LexType::ID);
		}
		fidMore(t);  // 处理更多参数
	}

	void fidMore(TreenodePtr& t) {
		if (currentToken().lexType == LexType::strI ||
			currentToken().lexType == LexType::RPAREN) {
			return;  // 由上层函数处理
		}
		else if (currentToken().lexType == LexType::COMMA) {
			match(LexType::COMMA);
			formList(t);
		}
		else {
			logError(ErrorType::SyntaxError,
				"Unexpected token in parameter list: " +
				lexTypeToString(currentToken().lexType));
		}
	}

	TreenodePtr procDecPart() {
		return declarePart();
	}

	TreenodePtr procBody() {
		TreenodePtr t = programBody();  // 复用 programBody 逻辑

		if (!t) {
			logError(ErrorType::MissingProgramBody,
				"Procedure body is incomplete or missing");
		}

		return t;
	}

	TreenodePtr programBody() {
		auto t = Treenode::create(Treenodecate::StmLK, currentToken().line);
		match(LexType::BEGIN);
		t->addChild(0, move(stmList()));
		match(LexType::END);
		return t;
	}

	TreenodePtr stmList() {
		TreenodePtr t = stm();          // 解析第一个语句
		TreenodePtr p = stmMore();      // 解析后续语句

		if (p) {
			t->addSibling(move(p));     // 将后续语句链接为兄弟节点
		}

		return t;
	}

	TreenodePtr stmMore() {
		TreenodePtr t = nullptr;

		// 允许语句列表结束的情况
		if (currentToken().lexType == LexType::END ||
			currentToken().lexType == LexType::ENDWH ||
			currentToken().lexType == LexType::ELSE ||
			currentToken().lexType == LexType::FI) {
			// 不生成节点，直接返回空
		}
		else if (currentToken().lexType == LexType::strI) {
			match(LexType::strI);
			t = stmList();
		}
		else {
			logError(ErrorType::SyntaxError,
				"Unexpected token after statement: " +
				lexTypeToString(currentToken().lexType));
		}

		return t;
	}

	TreenodePtr stm() {
		TreenodePtr t = nullptr;
		switch (currentToken().lexType) {
		case LexType::IF:
			t = conditionalStm();
			break;
		case LexType::WHILE:
			t = loopStm();
			break;
		case LexType::RETURN:
			t = returnStm();
			break;
		case LexType::READ:
			t = inputStm();
			break;
		case LexType::WRITE:
			t = outputStm();
			break;
		case LexType::ID: {
			auto f = Treenode::create(Treenodecate::StmtK, currentToken().line);
			auto t1 = Treenode::create(Treenodecate::ExpK, currentToken().line);
			t1->kind.exp = ExpKcate::IdEK;
			t1->attr.ExpAttr.varkind = Varkind::IdV;
			t1->addName(currentToken().str); // 存储标识符名称
			f->addChild(0, move(t1));
			assCall(f);
			t = move(f);
			break;
		}
		default:
			logError(ErrorType::SyntaxError,
				"Unexpected token in statement: " +
				lexTypeToString(currentToken().lexType));
			break;
		}

		return t;
	}

	void assCall(TreenodePtr& t) {
		match(LexType::ID);
		if (currentToken().lexType == LexType::ASSIGN ||
			currentToken().lexType == LexType::LMIDPAREN ||
			currentToken().lexType == LexType::DOT) {
			assignmentRest(t);
			t->kind.stmt = StmtKcate::AssignK;
		}
		else if (currentToken().lexType == LexType::LPAREN) {
			// 处理过程调用
			callStmRest(t);
			t->kind.stmt = StmtKcate::CallK;

		}
		else {
			logError(ErrorType::SyntaxError,
				"Expected assignment or call after identifier, got: " +
				lexTypeToString(currentToken().lexType));
		}
	}

	TreenodePtr conditionalStm() {
		auto t = Treenode::create(Treenodecate::StmtK, currentToken().line);
		t->kind.stmt = StmtKcate::IfK;
		match(LexType::IF);
		t->addChild(0, expression());
		match(LexType::THEN);
		t->addChild(1, stmList());
		if (currentToken().lexType == LexType::ELSE) {
			match(LexType::ELSE);
			t->addChild(2, stmList());
		}
		match(LexType::FI);
		return t;
	}

	TreenodePtr loopStm() {
		auto t = Treenode::create(Treenodecate::StmtK, currentToken().line);
		t->kind.stmt = StmtKcate::WhileK;
		match(LexType::WHILE);  // 必须匹配 WHILE
		// 解析循环条件 (第一个子节点)
		t->addChild(0, expression());
		match(LexType::DO);
		// 解析循环体 (第二个子节点)
		t->addChild(1, stmList());
		match(LexType::ENDWH);
		return t;
	}

	TreenodePtr returnStm() {
		auto t = Treenode::create(Treenodecate::StmtK, currentToken().line);
		t->kind.stmt = StmtKcate::ReturnK;

		match(LexType::RETURN);

		// 注意：根据实际语法，可能需要解析返回值表达式
		// 此处根据原代码逻辑，RETURN 后无表达式
		return t;
	}

	TreenodePtr inputStm() {
		auto t = Treenode::create(Treenodecate::StmtK, currentToken().line);
		t->kind.stmt = StmtKcate::ReadK;
		match(LexType::READ);
		match(LexType::LPAREN);
		// 解析变量名
		if (currentToken().lexType == LexType::ID) {
			t->addName(currentToken().str);
		}
		match(LexType::ID);
		match(LexType::RPAREN);
		return t;
	}

	TreenodePtr outputStm() {
		auto t = Treenode::create(Treenodecate::StmtK, currentToken().line);
		t->kind.stmt = StmtKcate::WriteK;
		match(LexType::WRITE);
		match(LexType::LPAREN);
		// 解析输出表达式 (第一个子节点)
		t->addChild(0, expression());
		match(LexType::RPAREN);
		return t;
	}

	void assignmentRest(TreenodePtr& t) {
		variMore(t->child[0]);  // 假设 variMore 已实现

		// 匹配赋值符号 :=
		match(LexType::ASSIGN);
		// 解析右部表达式作为第二个子节点
		t->addChild(1, expression());
	}

	void callStmRest(TreenodePtr& t) {
		match(LexType::LPAREN);
		// 解析实际参数列表作为第二个子节点
		t->addChild(1, actParamList());

		match(LexType::RPAREN);
	}

	TreenodePtr actParamList() {
		TreenodePtr t = nullptr;
		if (currentToken().lexType == LexType::RPAREN) {
			// 空参数列表
		}
		// 检查有效参数起始 Token
		else if (currentToken().lexType == LexType::ID ||
			currentToken().lexType == LexType::INTC) {
			t = expression();  // 解析第一个参数
			// 处理更多参数
			if (t) {
				t->addSibling(move(actParamMore()));
			}
		}
		else {
			logError(ErrorType::SyntaxError,
				"Invalid actual parameter: " +
				lexTypeToString(currentToken().lexType));
		}
		return t;
	}

	TreenodePtr actParamMore() {
		TreenodePtr t = nullptr;
		if (currentToken().lexType == LexType::RPAREN) {
			// 空参数列表
		}
		else if (currentToken().lexType == LexType::COMMA) {  // 逗号分隔参数
			match(LexType::COMMA);
			t = actParamList();         // 递归解析后续参数
		}
		return t;
	}

	TreenodePtr expression() {
		int line = currentToken().line;
		TreenodePtr t = simpleExp();  // 解析左操作数

		// 检查比较运算符
		if (currentToken().lexType == LexType::LT ||
			currentToken().lexType == LexType::EQ) {

			auto p = Treenode::create(Treenodecate::ExpK, line);
			p->kind.exp = ExpKcate::OpK;
			p->attr.ExpAttr.op = currentToken().lexType;  // 存储操作符类型
			p->addChild(0, move(t));

			// 消费操作符
			match(currentToken().lexType);

			// 解析右操作数作为第二个子节点
			p->addChild(1, simpleExp());

			t = move(p);  // 更新当前根节点
		}
		return t;
	}

	TreenodePtr simpleExp() {
		int line = currentToken().line;
		TreenodePtr t = term();  // 解析第一个项

		// 循环处理连续的操作符
		while (currentToken().lexType == LexType::PLUS ||
			currentToken().lexType == LexType::MINUS) {

			auto p = Treenode::create(Treenodecate::ExpK, line);
			p->kind.exp = ExpKcate::OpK;
			p->attr.ExpAttr.op = currentToken().lexType;
			p->addChild(0, move(t));

			// 消费操作符
			match(currentToken().lexType);

			// 解析右项作为第二个子节点
			p->addChild(1, term());

			t = move(p);  // 更新当前根节点
			line = t->lineno;  // 保持行号一致
		}
		return t;
	}

	TreenodePtr term() {
		int line = currentToken().line;
		TreenodePtr t = factor();  // 解析第一个因子

		// 处理连续乘除运算
		while (currentToken().lexType == LexType::TIMES ||
			currentToken().lexType == LexType::OVER) {

			auto p = Treenode::create(Treenodecate::ExpK, line);
			p->kind.exp = ExpKcate::OpK;
			p->attr.ExpAttr.op = currentToken().lexType;
			p->addChild(0, move(t));

			// 消费操作符
			match(currentToken().lexType);

			// 解析右操作数作为第二个子节点
			p->addChild(1, factor());

			t = move(p);  // 更新当前根节点
			line = t->lineno;  // 保持行号更新
		}
		return t;
	}

	TreenodePtr factor() {
		TreenodePtr t = nullptr;
		int line = currentToken().line;

		switch (currentToken().lexType) {
		case LexType::INTC: {  // 整型常量
			t = Treenode::create(Treenodecate::ExpK, line);
			t->kind.exp = ExpKcate::ConstK;
			t->attr.ExpAttr.val = stoi(currentToken().str);
			match(LexType::INTC);
			break;
		}
		case LexType::ID:      // 变量或函数调用
			t = variable();    // 假设 variable() 返回 TreenodePtr
			break;
		case LexType::LPAREN:  // 括号表达式
			match(LexType::LPAREN);
			t = expression();
			match(LexType::RPAREN);
			break;
		default:
			logError(ErrorType::SyntaxError,
				"Invalid factor: " + lexTypeToString(currentToken().lexType));
			break;
		}
		return t;
	}

	TreenodePtr variable() {
		auto t = Treenode::create(Treenodecate::ExpK, currentToken().line);
		t->kind.exp = ExpKcate::IdEK;  // 默认为基本标识符

		if (currentToken().lexType == LexType::ID) {
			t->addName(currentToken().str);  // 存储变量名
			match(LexType::ID);
			variMore(t);  // 处理数组/记录访问
		}
		else {
			logError(ErrorType::SyntaxError, "Expected identifier");
		}
		return t;
	}

	void variMore(TreenodePtr& t) {
		// 检查可能的结束符号（不需要处理的情况）
		if (currentToken().lexType == LexType::ASSIGN ||
			currentToken().lexType == LexType::TIMES ||
			currentToken().lexType == LexType::EQ||
			currentToken().lexType == LexType::LT ||
			currentToken().lexType == LexType::PLUS ||
			currentToken().lexType == LexType::MINUS||
			currentToken().lexType == LexType::OVER ||
			currentToken().lexType == LexType::RPAREN ||
			currentToken().lexType == LexType::strI||
			currentToken().lexType == LexType::COMMA ||
			currentToken().lexType == LexType::THEN ||
			currentToken().lexType == LexType::RMIDPAREN ||
			currentToken().lexType == LexType::ELSE ||
			currentToken().lexType == LexType::FI ||
			currentToken().lexType == LexType::DO ||
			currentToken().lexType == LexType::ENDWH ||
			currentToken().lexType == LexType::END
			){ }
		
		else if (currentToken().lexType == LexType::LMIDPAREN) {  // 数组访问 [exp]
			match(LexType::LMIDPAREN);

			auto indexExp = expression();  // 解析下标表达式
			t->addChild(0, move(indexExp));
			t->attr.ExpAttr.varkind = Varkind::ArrayMembV;
			t->child[0]->attr.ExpAttr.varkind = Varkind::IdV;
			match(LexType::RMIDPAREN);

		}
		else if (currentToken().lexType == LexType::DOT) {  // 记录访问 .field
			match(LexType::DOT);
			t->addChild(0, move(fieldVar()));
			t->attr.ExpAttr.varkind = Varkind::FieldMembV;
			t->child[0]->attr.ExpAttr.varkind = Varkind::IdV;
		}
		else {
			logError(ErrorType::SyntaxError,
				"Unexpected token after variable: " +
				lexTypeToString(currentToken().lexType));
		}
	}

	TreenodePtr fieldVar() {
		auto t = Treenode::create(Treenodecate::ExpK, currentToken().line);
		t->kind.exp = ExpKcate::IdEK;
		if (currentToken().lexType == LexType::ID) {
			t->addName(currentToken().str);  // 存储字段名
			match(LexType::ID);
			fieldVarMore(t);  // 处理可能的数组访问后缀
		}
		else {
			logError(ErrorType::SyntaxError, "Missing field name after .");
		}
		return t;
	}

	void fieldVarMore(TreenodePtr& t) {
		if (currentToken().lexType == LexType::ASSIGN ||
			currentToken().lexType == LexType::TIMES ||
			currentToken().lexType == LexType::EQ ||
			currentToken().lexType == LexType::LT ||
			currentToken().lexType == LexType::PLUS ||
			currentToken().lexType == LexType::MINUS ||
			currentToken().lexType == LexType::OVER ||
			currentToken().lexType == LexType::RPAREN ||
			currentToken().lexType == LexType::strI ||
			currentToken().lexType == LexType::COMMA ||
			currentToken().lexType == LexType::THEN ||
			currentToken().lexType == LexType::RMIDPAREN ||
			currentToken().lexType == LexType::ELSE ||
			currentToken().lexType == LexType::FI ||
			currentToken().lexType == LexType::DO ||
			currentToken().lexType == LexType::ENDWH ||
			currentToken().lexType == LexType::END
			) { }
		else if (currentToken().lexType == LexType::LMIDPAREN) {
			match(LexType::LMIDPAREN);

			// 解析下标表达式作为第一个子节点
			t->addChild(0, expression());
			t->child[0]->attr.ExpAttr.varkind = Varkind::ArrayMembV;
			match(LexType::RMIDPAREN);
		}
		else{
			logError(ErrorType::SyntaxError,
				"Unexpected token after field access: " +
				lexTypeToString(currentToken().lexType));
		}
	}

	bool match(LexType expected) {
		if (currentToken().lexType == expected) {
			consumeToken();
			return true;
		}
		logError(ErrorType::SyntaxError,
			"Expected " + lexTypeToString(expected) +
			" but got " + lexTypeToString(currentToken().lexType));
		return false;
	}

	/*bool matchIf(LexType expected) {
		if (currentToken().lexType == expected) {
			consumeToken();
			return true;
		}
		return false;
	}*/

};


#endif /* CF60210D_68B5_4BE0_BADA_F962EF54198B */
