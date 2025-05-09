#include "1.h"
#include "2.h"

/******************************************************************* 
* 																   *
* 																   * 
* 		      以下为语义分析部分，created by LingerXu          		  *
* 																   * 
* 																   * 
*******************************************************************/


typedef enum { typeKind, varKind, procKind }IdKind;	//标识符类型
typedef enum { indir, dir }AccessKind;

typedef struct typeIR TypeIR;

//域类型单元
typedef struct fieldChain {
    string idName;
    TypeIR* unitType;
    int off;
    fieldChain* next;
    fieldChain(string idName, TypeIR* unitType, int off);
    fieldChain();
}FieldChain;

FieldChain::fieldChain(string idName, TypeIR* unitType, int off) :idName(idName), unitType(unitType), off(off), next(nullptr) {
}

FieldChain::fieldChain() :next(nullptr), off(0) {
}

//类型内部结构
struct typeIR {
    int size;
    DecKcate typeKind;
    union {
        struct {
            int low;
            int up;
            TypeIR* elemTy;
        }ArrayAttr;
        FieldChain* body;
    }More;
    typeIR(DecKcate type);
    typeIR(DecKcate type, int low, int up, TypeIR* elemTy);
    typeIR(DecKcate type, FieldChain* bodyPtr);
    typeIR();
};

TypeIR::typeIR() {}
TypeIR::typeIR(DecKcate type) {
    typeKind = type;
    switch (type) {
    case IntegerK:size = 4;
        break;
    case CharK:size = 4;
        break;
    case BoolK:size = 4;
        break;
    }
}
TypeIR::typeIR(DecKcate type, int low, int up, TypeIR* elemTy) {
    typeKind = type;
    if (type != ArrayK) {
        cout << "type不是ArrayK" << endl;
        exit(1);
    }
    this->More.ArrayAttr.low = low;
    this->More.ArrayAttr.up = up;
    this->More.ArrayAttr.elemTy = elemTy;
    this->size = (up - low + 1) * elemTy->size;
}
TypeIR::typeIR(DecKcate type, FieldChain* bodyPtr) {
    typeKind = type;
    if (type != RecordK) {
        cout << "type不是RecordK" << endl;
        exit(1);
    }
    this->More.body = bodyPtr;
    int recordsize = 0;

    while (bodyPtr) {
        recordsize += bodyPtr->unitType->size;
        bodyPtr = bodyPtr->next;
    }
    this->size = recordsize;
}


//参数表
typedef struct paramTable {
    SymbolsTable* entry;
    paramTable* next;
    paramTable();
}ParamTable;
paramTable::paramTable() :entry(nullptr), next(nullptr) {}

//标识符信息项
typedef struct attributeIR {
    TypeIR* idType;
    IdKind idKind;
    union {
        struct {
            AccessKind access;
            int level;
            int off;
        }VarAttr;
        struct {
            int level;
            ParamTable* param;
            int off;
            int moff;
            int code;//过程目标代码在程序中的地址
            int size;//目标代码大小,在生成阶段填写
        }ProcAttr;
    }More;
    attributeIR();
    attributeIR(TypeIR* idType, IdKind idKind);
    attributeIR(TypeIR* idType, IdKind idKind, AccessKind access, int level, int off);
    attributeIR(TypeIR* idType, IdKind idKind, int level, ParamTable* param, int off);
}AttributeIR;

AttributeIR::attributeIR(TypeIR* idType, IdKind idKind) {
    if (idKind != typeKind) {
        cout << "attributeIr中IdKind不为typeKind" << endl;
        exit(1);
    }
    this->idType = idType;
    this->idKind = idKind;
}

AttributeIR::attributeIR(TypeIR* idType, IdKind idKind, AccessKind access, int level, int off) {
    if (idKind != varKind) {
        cout << "attributeIr中IdKind不为varKind" << endl;
        exit(1);
    }
    this->idType = idType;
    this->idKind = idKind;
    this->More.VarAttr.access = access;
    this->More.VarAttr.level = level;
    this->More.VarAttr.off = off;
}

AttributeIR::attributeIR(TypeIR* idType, IdKind idKind, int level, ParamTable* param, int off) {
    if (idKind != procKind) {
        cout << "attributeIr中IdKind不为procKind" << endl;
        exit(1);
    }
    this->idType = idType;
    this->idKind = idKind;
    this->More.ProcAttr.off = 0;
    this->More.ProcAttr.moff = 0;//level+1+当前层符号数
    this->More.ProcAttr.param = param;
    this->More.ProcAttr.level = level;
}

AttributeIR::attributeIR() {}


//SNL符号表
struct SymbolsTable {
    string idName;//标识符名字
    AttributeIR* attrIR;
    SymbolsTable* next;
};


TypeIR* intPtr, * charPtr, *boolPtr;
void initType() {
    intPtr = new TypeIR(IntegerK);
    charPtr = new TypeIR(CharK);
    boolPtr = new TypeIR(BoolK);
}

vector<SymbolsTable*>scope;
int Leveloff=0;
void CreateTable() {
    Leveloff = 28;
    SymbolsTable* add = new SymbolsTable();
    add->next = nullptr;
    scope.emplace_back(add);
}

SymbolsTable* Enter(string idname, AttributeIR* attriP, SymbolsTable* tableToEnter) {

    SymbolsTable* add = new SymbolsTable();
    add->attrIR = attriP;
    add->idName = idname;
    add->next = nullptr;
    SymbolsTable* end = tableToEnter;
    while (end->next) {
        end = end->next;
    }
    end->next = add;
    return add;
}

SymbolsTable* FindEntry(string idname, int flag) {
    
    int len = scope.size();
    if (flag == 1) {
        
        SymbolsTable* search = scope.back()->next;
        while (search != nullptr && search->idName != idname) {
            search = search->next;
        }
        return search;
    }
    else {
        int len = scope.size();
        for (int i = len - 1; i >= 0; i--) {
            SymbolsTable* search = scope[i]->next;
            while (search != nullptr && search->idName != idname)
            {
                search = search->next;
            }
            if (search != nullptr)return search;
        }
        return nullptr;
        
    }
    return nullptr;
}

TypeIR* arrayType(Treenode* t, string name) {
    int low = t->attr.ArrayAttr.low, up = t->attr.ArrayAttr.up;
    if (low > up) {
        
        cout << "line " << t->lineno << ": " << name << " :array下标越界" << endl;
        exit(1);
    }

    TypeIR* elemTy = nullptr;
    if (t->attr.ArrayAttr.childType == IntegerK) {
        elemTy = intPtr;
    }
    else if (t->attr.ArrayAttr.childType == CharK) {
        elemTy = charPtr;
    }
    else {
        cout << "line " << t->lineno << ": " << name << " :array成员类型无法识别" << endl;
        exit(1);
    }
    return new TypeIR(ArrayK, low, up, elemTy);
}

TypeIR* recordType(Treenode* t, string name) {
    TreenodePtr& tmp = t->child[0];

    int off = 0;
    FieldChain* search = new FieldChain(), * body = search;
    Treenode* recordMember=tmp.get();
    while (recordMember != nullptr) {

        if (recordMember->kind.dec == IntegerK) {

            for (string memName : recordMember->name) {
                search->next = new FieldChain(memName, intPtr, off);
                search = search->next;
                off += 4;
            }

        }
        else if (recordMember->kind.dec == CharK) {
            for (string memName : recordMember->name) {
                search->next = new FieldChain(memName, charPtr, off);
                search = search->next;
                off += 4;
            }
        }
        else if (recordMember->kind.dec == ArrayK) {
            for (string memName : recordMember->name) {
                search->next = new FieldChain(memName, arrayType(recordMember, memName), off);
                search = search->next;
                int size;
                switch (recordMember->attr.ArrayAttr.childType) {
                case IntegerK:size = 4;
                    break;
                case CharK:size = 4;
                    break;
                }
                off += (recordMember->attr.ArrayAttr.up - recordMember->attr.ArrayAttr.low + 1) * size;
            }
        }
        else {
            cout << "line " << t->lineno << ": " << name << " :record成员类型无法识别" << endl;
            exit(1);
        }

        recordMember = recordMember->sibling.get();
    }
    search = body;
    body = body->next;
    delete search;
    return new TypeIR(RecordK, body);
}

TypeIR* nameType(Treenode* t, string name) {
    SymbolsTable* symbol = FindEntry(name, 0);
    if (symbol == nullptr) {
        cout << "line " << t->lineno << ": 标识符" << name << "不存在" << endl;
        exit(1);
    }
    else if (symbol->attrIR->idKind != typeKind) {
        cout << "line " << t->lineno << ": " << name << "不是类型标识符" << endl;
        exit(1);

    }
    return symbol->attrIR->idType;
}

TypeIR* TypeProcess(Treenode* t, DecKcate typekind, string name) {

    switch (typekind) {
    case IntegerK:return intPtr;
        break;
    case CharK:return charPtr;
        break;
    case ArrayK:return arrayType(t, name);
        break;
    case RecordK:return recordType(t, name);
        break;
    case IdK:
        return nameType(t, name);
        break;
    }
    return nullptr;
}

void ProcessType(Treenode* t) {
    //TypeK Node :t
    TreenodePtr& tmp = t->child[0];
    Treenode* typePart=tmp.get();
    while (typePart != nullptr) {

        if (FindEntry(typePart->name[0], 1) != nullptr) {
            cout << "line " << t->lineno << ": " << typePart->name[0] << " :重复声明了一个类型" << endl;
            exit(1);
        }
        SymbolsTable* fill = Enter(typePart->name[0], new AttributeIR(TypeProcess(typePart, typePart->kind.dec, typePart->type_name),
            typeKind), scope[scope.size() - 1]);
        t->table.emplace_back(fill);
        typePart = typePart->sibling.get();
    }
}

void ProcessVar(Treenode* t) {
    //Vark Node:t

    TreenodePtr& tmp = t->child[0];
    Treenode* varPart=tmp.get();
    while (varPart) {
        for (string varName : varPart->name) {
            SymbolsTable* symbol = nullptr;
            symbol = FindEntry(varName, 1);
            if (symbol != nullptr) {
                if (symbol->attrIR->idKind != typeKind) {
                    cout << "line " << t->lineno << ": " << varName << " :重复定义" << endl;
                    exit(1);
                }
            }
            else {

                TypeIR* addTypeIR = TypeProcess(varPart, varPart->kind.dec, varPart->type_name);
                AttributeIR* addAttri = new AttributeIR(addTypeIR, varKind, dir, scope.size() - 1, Leveloff);
                SymbolsTable* fill = Enter(varName, addAttri, scope[scope.size() - 1]);
                Leveloff += addTypeIR->size;
                varPart->table.emplace_back(fill);

            }

        }
        varPart = varPart->sibling.get();
    }

}

SymbolsTable* HeadProcess(Treenode* t) {
    string procName = t->name[0];
    if (FindEntry(procName, 1) != nullptr) {
        cout << t->lineno << " :过程" << procName << "名已存在" << endl;
        exit(1);
    }
    TypeIR* procType = nullptr;
    paramTable* procParam = new paramTable();
    AttributeIR* procAttri = new AttributeIR(procType, procKind, scope.size() - 1, nullptr, Leveloff);
    SymbolsTable* procSymbol = Enter(procName, procAttri, scope[scope.size() - 1]);
    paramTable* search = procParam;
    t->table.emplace_back(procSymbol);
    CreateTable();
    TreenodePtr& tmp = t->child[0];
    Treenode* paramNode=tmp.get();
    while (paramNode != nullptr) {

        int paramCnt = paramNode->name.size();
        int start = 0;
        if (paramNode->kind.dec == IdK) {
            start = 1;
        }
        for (int i = start; i < paramCnt; i++) {
            string paramName = paramNode->name[i];

            if (FindEntry(paramName, 1)) {
                cout << "line " << t->lineno                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             << ": " << paramName << " :????????????????????" << endl;
                exit(1);
            }

            TypeIR* addTypeIR = TypeProcess(paramNode, paramNode->kind.dec, paramNode->type_name);
            AccessKind ac = paramNode->attr.paramt == ParamTcate::valparamtype ? dir : indir;
            AttributeIR* addAttri = new AttributeIR(addTypeIR, varKind, ac, scope.size() - 1, Leveloff);
            SymbolsTable* fill = Enter(paramName, addAttri, scope[scope.size() - 1]);
            Leveloff += (ac == dir ? addTypeIR->size : 4);
            t->table.emplace_back(fill);
            search->next = new paramTable();
            search = search->next;
            search->entry = fill;
        }
        paramNode = paramNode->sibling.get();
    }
    procSymbol->attrIR->More.ProcAttr.param = procParam->next;
    delete procParam;
    return procSymbol;
}

void statement(Treenode* t);

TypeIR* Expr(Treenode* t);
TypeIR* arrayVar(Treenode* t) {
    SymbolsTable* symbol = FindEntry(t->name[0], 0);
    t->table.emplace_back(symbol);
    if (symbol == nullptr) {
        cout << "line " << t->lineno << " : 标识符" << t->name[0] << "未定义" << endl;
        exit(1);
    }
    if (symbol->attrIR->idKind != varKind) {
        cout << "line " << t->lineno << " : " << t->name[0] << "不是变量" << endl;
        exit(1);
    }
    if (symbol->attrIR->idType->typeKind != ArrayK) {
        cout << "line " << t->lineno << " : " << t->name[0] << "不是数组" << endl;
        exit(1);
    }
    if (symbol->attrIR->idType->More.ArrayAttr.elemTy != Expr(t->child[0].get())) {
        cout << "line " << t->lineno << " :" << t->name[0] << "下标类型不符" << endl;
        exit(1);
    }
    return symbol->attrIR->idType->More.ArrayAttr.elemTy;
}

FieldChain* FindField(string idname, FieldChain* head) {
    while (head && idname != head->idName) {
        head = head->next;
    }
    return head != nullptr ? head : nullptr;
}

TypeIR* recordVar(Treenode* t) {
    SymbolsTable* symbol = FindEntry(t->name[0], 0);
    t->table.emplace_back(symbol);
    if (symbol == nullptr) {
        cout << "line " << t->lineno << " : 标识符" << t->name[0] << "未定义" << endl;
        exit(1);
    }
    if (symbol->attrIR->idKind != varKind) {
        cout << "line " << t->lineno << " : " << t->name[0] << "不是变量" << endl;
        exit(1);
    }
    if (symbol->attrIR->idType->typeKind != RecordK) {
        cout << "line " << t->lineno << " : " << t->name[0] << "不是记录变量" << endl;
        exit(1);
    }
    string fieldName = t->child[0]->name[0];
    FieldChain* fieldType = FindField(fieldName, symbol->attrIR->idType->More.body);
    if (fieldType == nullptr) {
        cout << "line " << t->lineno << " :" << t->name[0] << "域名" << fieldName << "不存在" << endl;
        exit(1);
    }
    return fieldType->unitType;
}

TypeIR* Expr(Treenode* t) {
    
    switch (t->kind.exp) {
    case OpK:
        TypeIR* Eptr;
        if (Expr(t->child[0].get()) != Expr(t->child[1].get())) {
            cout << "line " << t->lineno << " :" << "表达式类型不兼容" << endl;
            exit(1);
        }
        switch (t->attr.ExpAttr.op) {
        case LT:
        case EQ:
            Eptr = boolPtr;
            break;  
        case PLUS:
        case MINUS:
        case TIMES:
        case OVER:
            Eptr = intPtr;
            break;  
        }
        return Eptr;
        break;
    case ConstK:return intPtr;
        break;
    case IdEK:
        if (t->child[0] != nullptr) {
            //array OR record
            if (t->attr.ExpAttr.varkind == Varkind::ArrayMembV) {
                return arrayVar(t);
            }
            else if (t->attr.ExpAttr.varkind == Varkind::FieldMembV) {
                return recordVar(t);
            }
            else {
                cout << "line " << t->lineno << " :" << t->name[0] << "是非法的成员访问类型" << endl;
                exit(1);
            }
        }
        else {
            SymbolsTable* symbol = FindEntry(t->name[0], 0);
            if (symbol == nullptr) {
                cout << "line " << t->lineno << " : " << t->name[0] << "未声明的标识符" << endl;
                exit(1);
            }
            if (symbol->attrIR->idKind != varKind) {
                cout << "line " << t->lineno << " : " << t->name[0] << "标识符不是变量" << endl;
                exit(1);
            }
            
            t->table.emplace_back(symbol);

            return symbol->attrIR->idType;
        }
        break;
    }
    return nullptr;
}

void IfStatement(Treenode* t) {
    // Stmt NODE:t
    TreenodePtr& conditionNode = t->child[0];
    TreenodePtr& tmp1 = t->child[1];
    Treenode* thenNode=tmp1.get();
    TreenodePtr& tmp2 = t->child[2];
    Treenode* elseNode=tmp2.get();
    TypeIR* conditionType = Expr(conditionNode.get());
    if (conditionType->typeKind != BoolK) {
        cout << "line " << t->lineno << " :" << "if语句判断条件不是bool类型" << endl;
        exit(1);
    }
    while (thenNode) {
        statement(thenNode);
        thenNode = thenNode->sibling.get();
    }
    while (elseNode) {
        statement(elseNode);
        elseNode = elseNode->sibling.get();
    }
}

void WhileStatement(Treenode* t) {
    //WhileK Node:t
    TreenodePtr& conditionNode = t->child[0];
    TreenodePtr& tmp = t->child[1];
    TypeIR* conditionType = Expr(conditionNode.get());
    Treenode* loopNode=tmp.get();
    if (conditionType->typeKind != BoolK) {
        cout << "line " << t->lineno << " :" << "while语句判断条件不是bool类型" << endl;
        exit(1);
    }
    while (loopNode) {
        statement(loopNode);
        loopNode = loopNode->sibling.get();
    }
}

void AssignStatement(Treenode* t) {
    TreenodePtr& child1 = t->child[0];
    TreenodePtr& child2 = t->child[1];
    TypeIR* eptr1, * eptr2;
    SymbolsTable* child1Symbol = FindEntry(child1->name[0], 0);
    if (child1Symbol == nullptr) {
        cout << "line " << t->lineno << " :左标识符" << child1->name[0] << "未找到" << endl;
        exit(1);
    }
    if (child1Symbol->attrIR->idKind != varKind) {
        cout << "line " << t->lineno << " :左值不是变量" << endl;
        exit(1);
    }

    if (child1->kind.exp == IdEK) {
        if (child1->child[0] != nullptr) {
            if (child1->attr.ExpAttr.varkind == Varkind::ArrayMembV) {
                eptr1 = arrayVar(child1.get());
            }
            else if (child1->attr.ExpAttr.varkind == Varkind::FieldMembV) {
                eptr1 = recordVar(child1.get());
            }
        }
        else
        {
            child1->table.emplace_back(child1Symbol);
            //SymTable *symbol= FindEntry(child1->name[0],0);
            if (child1Symbol == nullptr) {
                cout << "line " << child1->lineno << " : 嵌套标识符" << child1->name[0] << "未找到" << endl;
                exit(1);

            }
            if (child1Symbol->attrIR->idKind != varKind) {
                cout << "line " << child1->lineno << " : " << child1->name[0] << "不是变量" << endl;
                exit(1);
            }
            eptr1 = child1Symbol->attrIR->idType;
        }
    }
    else {
        cout << "line " << t->lineno << " :非法的赋值左值表达式" << endl;
        exit(1);
    }
    eptr2 = Expr(child2.get());
    if (eptr1 != eptr2) {
        cout << "line " << t->lineno << " : " << "赋值类型不匹配" << endl;
        exit(1);
    }
}

void ReadStatement(Treenode* t) {
    // ReadK Node:t
    SymbolsTable* symbol = FindEntry(t->name[0], 0);
    if (symbol == nullptr) {
        cout << "line " << t->lineno << " :读入的" << t->name[0] << "未声明" << endl;
        exit(1);

    }
    if (symbol->attrIR->idKind != varKind) {
        cout << "line " << t->lineno << " :read读入对象" << t->name[0] << "不是变量" << endl;
        exit(1);
    }
    t->table.emplace_back(symbol);
}

void WriteStatement(Treenode* t) {
    //WriteK Node:t
    TypeIR* writeType = Expr(t->child[0].get());
    if (writeType != charPtr && writeType != intPtr) {
        cout << "line " << t->lineno << " :" << "write语句表达式类型必须是integer或char" << endl;
        exit(1);
    }
}

void CallStatement(Treenode* t) {
    //CallK Node:t
    string procName = t->child[0]->name[0];
    SymbolsTable* symbol = FindEntry(procName, 0);
    if (symbol == nullptr) {
        cout << "line " << t->lineno << " : 过程名" << procName << "未声明" << endl;
        exit(1);
    }
    if (symbol->attrIR->idKind != procKind) {
        cout << "line " << t->lineno << " : " << procName << "不是过程名" << endl;
        exit(1);
    }
    t->table.emplace_back(symbol);
    
    TreenodePtr& tmp = t->child[1];
    Treenode* param=tmp.get();
    
    ParamTable* procParam = symbol->attrIR->More.ProcAttr.param;
    while (param != nullptr && procParam != nullptr) {
        TypeIR* callParamType = Expr(param);
        if (callParamType->typeKind == ArrayK) {
            if (procParam->entry->attrIR->idType->typeKind != ArrayK) {
                cout << "line " << t->lineno << " : " << procName << "数组类型与普通类型不匹配" << endl;
                exit(1);
            }
            if (procParam->entry->attrIR->idType->More.ArrayAttr.elemTy != callParamType->More.ArrayAttr.elemTy) {
                cout << "line " << t->lineno << " : " << procName << "数组元素类型不匹配" << endl;
                exit(1);
            }
            if ((procParam->entry->attrIR->idType->More.ArrayAttr.up != callParamType->More.ArrayAttr.up) || (procParam->entry->attrIR->idType->More.ArrayAttr.low != callParamType->More.ArrayAttr.low)) {
                cout << "line " << t->lineno << " : " << procName << "数组上下界不匹配" << endl;
                exit(1);
            }
        }
        else if (callParamType != procParam->entry->attrIR->idType) {
            cout << "line " << t->lineno << " : " << procName << "普通类型不匹配" << endl;
            exit(1);
        }

        procParam = procParam->next;
        param = param->sibling.get();
    }
    if (param != nullptr || procParam != nullptr) {
        cout << "line " << t->lineno << " : " << procName << "参数个数不匹配" << endl;
        exit(1);
    }
}
void ReturnStatement(Treenode* t) {
    if (scope.size() == 1) {
        cout << "line " << t->lineno<< " : " << "主程序中不能使用return语句" << endl;
        exit(1);
    }
}

void statement(Treenode* t) {
    //StmtK NODE:t
    switch (t->kind.stmt) {
    case IfK:
        IfStatement(t);
        break;
    case WhileK:
        WhileStatement(t);
        break;
    case AssignK:
        AssignStatement(t);
        break;
    case ReadK:
        ReadStatement(t);
        break;
    case WriteK:
        WriteStatement(t);
        break;
    case CallK:
        CallStatement(t);
        break;
    case ReturnK:
        ReturnStatement(t);
        break;
    default:
        cout << "line " << t->lineno << " :非法的语句类型" << endl;
        exit(1);
        break;
    }
}

void Body(Treenode* t) {
    //StmLK NODE: t
    TreenodePtr& tmp = t->child[0];
    Treenode* stmtList=tmp.get();
    while (stmtList != nullptr) {

        statement(stmtList);
        stmtList = stmtList->sibling.get();
    }
}

void DestroyTable() {
    SymbolsTable* search = scope.back();
    delete search;
    scope.pop_back();

}


void ProcessProc(Treenode* t) {
    //ProcDeck Node:t
    int Savedoff = 0;
    SymbolsTable* proc = nullptr;
    while (t != nullptr) {
        proc = HeadProcess(t);
        Savedoff = Leveloff;
        TreenodePtr& tmp1 = t->child[1];
        TreenodePtr& tmp2 = t->child[2];
        Treenode* DecNode=tmp1.get();
        Treenode* BodyNode=tmp2.get();
        if (DecNode != nullptr && DecNode->nodekind == TypeK) {
            ProcessType(DecNode);
            DecNode = DecNode->sibling.get();
        }
        if (DecNode != nullptr && DecNode->nodekind == VarK) {
            ProcessVar(DecNode);
            Savedoff = Leveloff;
            DecNode = DecNode->sibling.get();
        }
        if (DecNode != nullptr && DecNode->nodekind == ProcDecK) {
            ProcessProc(DecNode);
            DecNode = DecNode->sibling.get();
        }

        if (proc != nullptr) {
            proc->attrIR->More.ProcAttr.off = Savedoff;
            proc->attrIR->More.ProcAttr.moff = Savedoff + (scope.size() - 1) * 4;
        }
        Body(BodyNode);
        DestroyTable();
        t = t->sibling.get();
    }
}

string toIdKind(int enumId) {
    switch (enumId) {
    case typeKind:return "typeKind";
    case varKind:return "varKind";
    case procKind:return "procKind";
    default:cout << "IdKind异常，不存在类型，变量，过程之外的kind，返回空字符" << endl;
        exit(1);
    }
    return "";
}

string toAccessKind(int enumId) {
    if (enumId == indir) {
        return "indir";
    }
    else if (enumId == dir) {
        return "dir";
    }
    else {
        cout << "访问方式异常，不存在indir和dir之外的访问" << endl;
        exit(1);
    }
    return "";
}

string toTypeKind(int enumId) {
    switch (enumId) {
    case IntegerK:return "IntegerK";
    case CharK:return "CharK";
    case ArrayK:return "ArrayK";
    case RecordK:return "RecordK";
        //case boolTy:return "boolTy";
    default:cout << "TypeKind异常，不存在int,char,array,record，bool之外的类型,返回空字符" << endl;
        exit(1);
    }
    return "";
}

void PrintSymTable(Treenode* t) {
    while (t != nullptr) {
        if (t->table.size() != 0) {

            for (SymbolsTable* symbol : t->table) {
                if (symbol->attrIR->idKind == procKind) {
                    printf("==============   Level %d:             %s   %s   %d   ACSIZE=%d\n", symbol->attrIR->More.ProcAttr.level, symbol->idName.c_str(), toIdKind(symbol->attrIR->idKind).c_str(), symbol->attrIR->More.ProcAttr.off, symbol->attrIR->More.ProcAttr.moff);

                }
                else if (symbol->attrIR->idKind == varKind) {
                    printf("==============   Level %d:             %s   %s   %s   %d\n", symbol->attrIR->More.VarAttr.level, symbol->idName.c_str(), toAccessKind(symbol->attrIR->More.VarAttr.access).c_str(), toIdKind(symbol->attrIR->idKind).c_str(), symbol->attrIR->More.VarAttr.off);
                }
                else {
                    printf("==============   TypeKind:             %s   %s  %s                               \n", symbol->idName.c_str(), toIdKind(symbol->attrIR->idKind).c_str(), toTypeKind(symbol->attrIR->idType->typeKind).c_str());

                }
            }
        }
        for (int i = 0; i < 3; i++)
        {
            PrintSymTable(t->child[i].get());
        }
        t = t->sibling.get();
    }
}

TreenodePtr semanticAnalyze(TreenodePtr&& t)
{
	initType();
	CreateTable();
	int Savedoff = Leveloff;
    TreenodePtr& MainProc = t->child[0];
    TreenodePtr& tmp = t->child[1];
    TreenodePtr& BodyNode = t->child[2];
    SymbolsTable* MainProSymbol = Enter(MainProc->name[0], new AttributeIR(nullptr, procKind, 0, nullptr, Leveloff), scope[0]);
    MainProc->table.emplace_back(MainProSymbol);
    CreateTable();
    Treenode* DecNode=tmp.get();
    if (DecNode && DecNode->nodekind == TypeK) {
    	ProcessType(DecNode);

        DecNode = DecNode->sibling.get();
    }
    if (DecNode && DecNode->nodekind == VarK) {
        ProcessVar(DecNode);
        Savedoff = Leveloff;
        DecNode = DecNode->sibling.get();
    }
    if (DecNode && DecNode->nodekind == ProcDecK) {
        ProcessProc(DecNode);

        DecNode = DecNode->sibling.get();
    }

    MainProSymbol->attrIR->More.ProcAttr.off = Savedoff;                //从何处开始存储过程体内局部变量
    MainProSymbol->attrIR->More.ProcAttr.moff = Savedoff + (scope.size() - 1) * 4;  //为后续递归或者嵌套调用需要留出的栈空间，除主过程之外的嵌套层数，每个子程序预留4单位空间
    Body(BodyNode.get());

    printf("\n\n");
    PrintSymTable(t.get());
    return move(t);
}

/******************************************************************* 
* 																   *
* 																   * 
* 		     以下为目标代码生成部分，created by LingerXu          	   *
* 																   * 
* 																   * 
*******************************************************************/
#define CODESEG ".text"
#define DATASEG ".data"
#define SP "$sp"
#define T0 "$t0"
#define T1 "$t1"
#define T2 "$t2"
#define T3 "$t3"
#define T4 "$t4"
#define T5 "$t5"
#define T6 "$t6"
#define T7 "$t7"
#define A0 "$a0"
#define A1 "$a1"
#define A2 "$a2"
#define A3 "$a3"
#define ZERO "$0"
#define RA "$ra"
#define V0 "$v0"
#define V1 "$v1"
#define READ 5
#define WRITESTR 4
#define WRITEINT 1
#define EXIT 10
vector<string> mipsCode;
int curLine=0;
int highLine=0; //记录最新行索引
int curAddr = 0x00400000;//相对偏移
int highAddr = 0x00400000;//最新地址

int  emitSkip(int num) {
    int i = curLine;
    while (num != 0) {
        mipsCode.emplace_back("");
        num--;
        curLine++;
        curAddr += 4;
    }
    highLine = max(curLine, highLine);
    highAddr = max(curAddr, highAddr);
    return i;
}

void emitComment(string c) {
    if (curLine >= mipsCode.size()) {
        mipsCode.resize(mipsCode.size() + 2000, "");
    }
    mipsCode[curLine] = "#" + c;
    curLine++;
    highLine = max(curLine, highLine);
}

int genLabel(string label) {
    string mips = label + ":";
    mipsCode[curLine] = mips;
    curLine++;
    curAddr += 4;
    highLine = max(curLine, highLine);
    highAddr = max(curAddr, highAddr);
    return curLine - 1;
}

int objMips_RM(string op, string rs, string rt, int immediate, string comment) {
    if (curLine >= mipsCode.size()) {
        mipsCode.resize(mipsCode.size() + 2000, "");
    }
    string mips = op + " " + rs + ", " + to_string(immediate) + "(" + rt + ")" + " #" + comment;
    mipsCode[curLine] = mips;
    curLine++;
    curAddr += 4;
    highLine = max(curLine, highLine);
    highAddr = max(curAddr, highAddr);
    return curLine - 1;//返回生成的代码行
}

void emitBackup(int loc, int addr) {
    curAddr = addr;
    curLine = loc;
    highLine = max(curLine, highLine);
    highAddr = max(curAddr, highAddr);
}

int objMips_UnConj(string op, string target, string comment) {
    if (curLine >= mipsCode.size()) {
        mipsCode.resize(mipsCode.size() + 2000, "");
    }
    string mips = op + " " + target + " #" + comment;
    mipsCode[curLine] = mips;
    curLine++;
    curAddr += 4;
    highLine = max(curLine, highLine);
    highAddr = max(curAddr, highAddr);
    return curLine - 1;//返回生成的代码行
}

void emitRestore() {
    curAddr = highAddr;
    curLine = highLine;
}

int objMips_Regs(string op, string rd, string rs, string comment)
{
    if (curLine >= mipsCode.size()) {
        mipsCode.resize(mipsCode.size() + 2000, "");
    }
    string mips = op + " " + rd + ", " + rs + " #" + comment;
    mipsCode[curLine] = mips;
    curLine++;
    curAddr += 4;
    highLine = max(curLine, highLine);
    highAddr = max(curAddr, highAddr);
    return curLine - 1;
}

int objMips_RegIm(string op, string rd, string rs, string immediate, string comment)
{
    if (curLine >= mipsCode.size()) {
        mipsCode.resize(mipsCode.size() + 2000, "");
    }
    string mips = op + " " + rd + ", " + rs + ", " + immediate + " #" + comment;
    mipsCode[curLine] = mips;
    curLine++;
    curAddr += 4;
    highLine = max(curLine, highLine);
    highAddr = max(curAddr, highAddr);
    return curLine - 1;
}

int objMips_Regs(string op, string rd, string rs, string rt, string comment)
{
    if (curLine >= mipsCode.size()) {
        mipsCode.resize(mipsCode.size() + 2000, "");
    }
    string mips = op + " " + rd + ", " + rs + ", " + rt + " #" + comment;
    mipsCode[curLine] = mips;
    curLine++;
    curAddr += 4;
    highLine = max(curLine, highLine);
    highAddr = max(curAddr, highAddr);
    return curLine - 1;
}

int objMips_Regs(string op, string rs, string comment)
{
    if (curLine >= mipsCode.size()) {
        mipsCode.resize(mipsCode.size() + 2000, "");
    }
    string mips = op + " " + rs + " #" + comment;
    mipsCode[curLine] = mips;
    curLine++;
    curAddr += 4;
    highLine = max(curLine, highLine);
    highAddr = max(curAddr, highAddr);
    return curLine - 1;
}

/*返回值t0，表示所要访问活动记录的基址*/
void getARBase(int varLevel) {
    /*var变量要对对应level-1的过程活动记录找！！！！！！！！*/
    /*先假定 t0是存放display表相对应基址的偏移的吧, 假定t2存当前的基址*/
    /*先求该变量层数在AR中的位置，其中varLevel表示变量所在层*/
    /*t4存当前活动记录的基址*/
    //lw $t2,24($t4)  //把display偏移值放到t2里，4*6
    objMips_RM("lw", "$t0", "$t4", -24, "#把display偏移值放到t0里，4*6");
    //addi $t1,$t2,varLevel*4 //找到display里的记录相对于基址的偏移
    objMips_RegIm("addi", "$t1", "$t0", to_string((varLevel - 1) * 4), "#找到display里的记录相对于基址的偏移");
    /*在求一下相对于基址的绝对偏移,现在t1里存放的就是varLevel层的AR的基址,是个地址*/
    //sub $t1,$t4,$t2 //求出记录的绝对偏移,栈是倒长的
    objMips_Regs("sub", "$t1", "$t4", "$t1", "#求出记录的绝对偏移,栈是倒长的");
    /*该变量所在AR的sp地址存在t1中，将t1地址里的数据取出来*/
    //lw $t0,0($t1); //取出记录里的内容
    objMips_RM("lw", "$t0", "$t1", 0, "#取出记录里的内容");
}

//这个函数里t0装要访问数组元素的下标，t1装数组下界
void findAbsAddr(Treenode* t) {
    int off;
    int varLevel;
    fieldChain* field = nullptr;
    /*可能是普通变量，域变量和数组变量*/
    if (t) {
        /*得到该变量在符号表中的地址，即变量相对于自己AR基址的偏移*/
        off = t->table[0]->attrIR->More.VarAttr.off;
        varLevel = t->table[0]->attrIR->More.VarAttr.level;
        /*此时要取数组第一个元素的地址*/
        /*如果是普通变量*/
        if (t->child[0] == nullptr) {

            //addi $t0,$zero,off //t0里存相对于自己AR基址的偏移
            objMips_RegIm("addi", "$t0", "$zero", to_string(off), "#t0里存相对于自己AR基址的偏移");
        }
        else if (t->attr.ExpAttr.varkind == Varkind::ArrayMembV) {
            /*将数组下标值送入t0中*/
            cGen(t->child[0].get());
            /*将数组下界送入t1*/
            int low = t->table[0]->attrIR->idType->More.ArrayAttr.low;

            //addi $t1,$zero,low      //t1为数组下界
            objMips_RegIm("addi", "$t1", "$zero", to_string(low), "#t1为数组下界");

            //sub $t0,$t0,$t1        //下标减去下界
            objMips_Regs("sub", "$t0", "$t0", "$t1", "#下标减去下界");

            //addi $t0,$t0,off       //求出数组变量的偏移，即数组里的这个元素相对于基址的偏移
            objMips_Regs("li", T1, to_string(4), "地址偏移乘4");
            objMips_Regs("mult", T0, T1, "乘4的偏移");
            objMips_Regs("mflo", T0, "将低32为载入T0");
            objMips_RegIm("addi", "$t0", "$t0", to_string(off), "#求出数组变量的偏移，即数组里的这个元素相对于基址的偏移");
        }
        else if (t->attr.ExpAttr.varkind == Varkind::FieldMembV) {
            field = t->table[0]->attrIR->idType->More.body;
            while (field) {
                int result = t->child[0]->name[0].compare(field->idName);
                if (result == 0) break;
                else field = field->next;
            }
            if (t->child[0]->child[0] == nullptr) {
                //addi $t0,$zero,off //这里是域基址相对于自己AR的偏移
                objMips_RegIm("addi", "$t0", "$zero", to_string(off), "#这里是域基址相对于自己AR的偏移");
                int foff = field->off;
                //addi $t0,$t0,foff  //域基址相对于自己AR的偏移和自己相对于域基址的偏移
                objMips_RegIm("addi", "$t0", "$t0", to_string(foff), "#域基址相对于自己AR的偏移和自己相对于域基址的偏移");
            }
            else {
                //数组元素下标放入t0
                genExp(t->child[0]->child[0].get());
                //将数组下标送入t1吧
                int low = t->table[0]->attrIR->idType->More.ArrayAttr.low;
                //addi $t1,$zero,low   //t1为数组下界
                objMips_RegIm("addi", "$t1", "$zero", to_string(low), "#t1为数组下界");
                //sub $t0,$t0,$t1      //下标减去下界
                objMips_Regs("sub", "$t0", "$t0", "$t1", "#下标减去下界");
                objMips_Regs("li", T1, to_string(4), "地址偏移乘4");
                objMips_Regs("mult", T0, T1, "乘4的偏移");
                int foff = field->off;
                //addi $t0,$t0,foff  //数组元素相对于域基址的相对地址
                objMips_RegIm("addi", "$t0", "$t0", to_string(foff), "#数组元素相对于域基址的相对地址");
                //addi $t0,$t0,off   //数组元素相对于AR基址的相对地址（偏移）
                objMips_RegIm("addi", "$t0", "$t0", to_string(off), "#数组元素相对于AR基址的相对地址（偏移）");
            }
        }
    }
    //add $t6,$t0,$zero //先把t0的值传到t6里，防止后面冲突
    objMips_Regs("add", "$t6", "$t0", "$zero", "#先把t0的值传到t6里，防止后面冲突");
    getARBase(varLevel); 	//基址存在t0里
    //sub $t0,$t0,$t6  	//计算绝对偏移，t0里存的是绝对地址
    objMips_Regs("sub", "$t0", "$t0", "$t6", "#计算绝对偏移，t0里存的是绝对地址");
}

int labelId=0;
string prepare_label(string kind = "", int beforeLabel = 0) {
    string ret = "label_";
    ret += to_string(labelId + beforeLabel);
    ret += "_" + kind;
    return ret;
}

int objMips_Conj(string op, string rs, string rt, string tag, string comment) {
    if (curLine >= mipsCode.size()) {
        mipsCode.resize(mipsCode.size() + 2000, "");
    }
    string mips = op + " " + rs + ", " + rt + ", " + tag + " #" + comment;
    mipsCode[curLine] = mips;
    curLine++;
    curAddr += 4;
    highLine = max(curLine, highLine);
    highAddr = max(curAddr, highAddr);
    return curLine - 1;//返回生成的代码行
}

string emitLabel(string kind = "") {
    if (curLine >= mipsCode.size()) {
        mipsCode.resize(mipsCode.size() + 2000, "");
    }
    string ret = "label_";
    ret += to_string(labelId++);
    ret += "_" + kind;
    mipsCode[curLine] = ret + ":";
    curLine++;
    highLine = max(curLine, highLine);
    return ret;
}

void genExp(Treenode* t)
{
    /* 语法树节点各个子节点 */
    Treenode* p1, * p2;
    /* 对语法树节点的表达式类型细分处理 */
    switch (t->kind.exp)
    {
        /* 语法树节点tree为ConstK表达式类型 */
    case ConstK:
        objMips_Regs("li", T0, to_string(t->attr.ExpAttr.val), "载入常量到T0");
        /* 如果代码生成追踪标志TraceCode为TRUE,写入注释,常数部分结束 */
        break;
        /* 语法树节点tree为IdK表达式类型 */
    case IdEK:
        /* 如果代码生成追踪标志TraceCode为TRUE,写入注释,标注标识符开始 */
        findAbsAddr(t);
        /*其中ac返回的是基本类型变量、域变量或下标变量的绝对偏移*/

        if (t->table[0]->attrIR->More.VarAttr.access == indir)
        {
            /*地址*/
            /*取值，作为地址*/
            objMips_RM("lw", T1, T0, 0, "indir load id value");
            /*按地址取单元内容*/
            objMips_RM("lw", T0, T1, 0, "");
        }
        else
        {
            /*值*/
            /* 写入数值载入指令,载入变量标识符的值*/
            objMips_RM("lw", T0, T0, 0, "load id value");
        }
        break;
        /* 语法树节点tree为OpK表达式类型 */
    case OpK:
        /* 语法树节点tree第一子节点为左操作数,赋给p1 */
        p1 = t->child[0].get();
        /* 语法树节点tree第二子节点为右操作数,赋给p2 */
        p2 = t->child[1].get();
        /* 对第一子节点递归调用函数cGen(),为左操作数生成目标代码 */
        cGen(p1);
        /* 生成单元设置指令,在临时数据存储区中压入左操作数 */
        objMips_RM("sw", T0, SP, 0, "存临时数据");
        objMips_Regs("li", T7, "4", "");
        objMips_Regs("sub", SP, SP, T7, "sp栈顶向下-4");
        /* 对第二子节点递归调用函数cGen(),为右操作数生成目标代码 */
        cGen(p2);
        objMips_RM("lw", T1, SP, 4, "载入数据");
        objMips_Regs("li", T7, "4", "");
        objMips_Regs("add", SP, SP, T7, "sp栈顶向上+4恢复");
        /* 生成数值载入指令,从临时数据存储区中载入左操作数 */
        /*交换一下*/
        objMips_Regs("move", T7, T1, "把t1放到t7里");
        objMips_Regs("move", T1, T0, "把t0放到t1里");
        objMips_Regs("move", T0, T7, "把t7放到t0里");
        /* 对语法树节点t的成员运算符attr.op分类处理 */
        switch (t->attr.ExpAttr.op)
        {
            /* 语法树节点成员运算符为PLUS,生成加法指令 */
        case PLUS:
            objMips_Regs("add", T0, T0, T1, "op +");
            break;
            /* 语法树节点成员运算符为MINUS,生成减法指令 */
        case MINUS:
            objMips_Regs("sub", T0, T0, T1, "op -");
            break;
            /* 语法树节点成员操作符为TIMES,写入乘法指令 */
        case TIMES:
            objMips_Regs("mult", T0, T1, "op *");
            objMips_Regs("mflo", T0, "将低32为载入T0");
            break;
            /* 语法树节点成员操作符为OVER,写入除法指令 */
        case OVER:
            objMips_Regs("div", T0, T1, "op /");
            objMips_Regs("mflo", T0, "将商为载入T0");
            break;
            /* 语法树节点成员操作符为LT,写入相应的指令序列 */
            /* 如果为真，结果为1；否则结果为0 */
        case LT: {
            /* 写入减指令,将(左-右)操作数相减,结果送t0 */
            objMips_Regs("sub", T0, T0, T1, "op <");
            /* 写入判断跳转指令,如果累加器ac的值小于0,	*
             * 则代码指令指示器跳过两条指令				*/
            string true_label = prepare_label("true");
            string false_label = prepare_label("false", 1);
            objMips_Conj("blt", T0, ZERO, true_label, "br if true");
            /* 写入载入常量指令,将累加器ac赋值为0 */
            objMips_Regs("li", T0, "0", "false case");
            /* 写入数值载入指令,代码指令指示器pc跳过下一条指令 */
            objMips_UnConj("j", false_label, "必定跳过下一条指令");
            emitLabel("true");
            /* 写入载入常量指令,将t0=1 */
            objMips_Regs("li", T0, "1", "true case");
            emitLabel("false");
            break;
            /* 语法树节点成员操作符为EQ,写入相应的指令序列 */
            /* 如果为真，结果为1；否则结果为0 */
        }
        case EQ:
        {
            /* 写入减法指令,将左,右操作数相减,结果送t0 */
            objMips_Regs("sub", T0, T1, T0, "op ==");
            /* 写入判断跳转指令,如果t0等于0,	*
             * 跳过两条指令			*/
            string true_label = prepare_label("true");
            string false_label = prepare_label("false", 1);
            objMips_Conj("beq", T0, ZERO, true_label, "br if true");
            /* 写入载入常量指令,将t0赋值为0 */
            objMips_Regs("li", T0, "0", "false case");
            /* 跳过一条指令 */
            objMips_UnConj("j", false_label, "必定跳过下一条指令");
            /* 写入载入常量指令,将t0赋值为1 */
            emitLabel("true");
            objMips_Regs("li", T0, "1", "true case");
            emitLabel("false");
            break;
        }
        /* 其他未知运算符,写入注释,标注未知运算符信息 */
        default:
            emitComment("BUG: Unknown operator");
            break;
        }
        break;
    default:
        break;
    }
}

void cGen(Treenode* tree) {
    if (tree != NULL)
    {
        /* 对语法树节点类型成员nodekind分类处理 */
        switch (tree->nodekind)
        {
            /* 对语句类型语法树节点调用代码生成函数,生成目标代码 */
        case StmtK:
            genStmt(tree);
            break;

            /* 对表达式类型语法树节点调用代码生成函数,生成目标代码 */
        case ExpK:
            genExp(tree);
            break;

        default:
            break;
        }
        /* 对语法树节点的兄弟节点递归调用函数cGen(),生成目标代码 */
        cGen(tree->sibling.get());
    }
}

void objMips_SysCall(int choice, string reg) {
    if (curLine >= mipsCode.size()) {
        mipsCode.resize(mipsCode.size() + 2000, "");
    }
    mipsCode[curLine] = "li $v0," + to_string(choice);
    curLine++;
    curAddr += 4;
    if (curLine >= mipsCode.size()) {
        mipsCode.resize(mipsCode.size() + 2000, "");
    }
    switch (choice) {
    case 5:
        break;
    case 4:mipsCode[curLine] = "la $a0," + reg;
        curLine++;
        curAddr += 4;
        break;
    case 1:/*打印一个整数*/
        mipsCode[curLine] = "move $a0," + reg;
        curLine++;
        curAddr += 4;
        break;
    case 10:
        break;
    }
    if (curLine >= mipsCode.size()) {
        mipsCode.resize(mipsCode.size() + 2000, "");
    }
    mipsCode[curLine] = "syscall";
    curLine++;
    curAddr += 4;
    if (curLine >= mipsCode.size()) {
        mipsCode.resize(mipsCode.size() + 2000, "");
    }
}

void genStmt(Treenode* t) {
    /*用于控制转移display表的各项sp值*/

    /*用于存储语法树的各个节点*/
    Treenode* p0 = NULL;
    Treenode* p1 = NULL;
    Treenode* pp = NULL;
    Treenode* p2 = NULL;
    SymbolsTable* entry = NULL;
    int FormParam;

    /*用于记录跳转回填时的地址*/
    int  savedLoc1, savedLoc2, currentLoc;
    int savedAddr1, savedAddr2, currentAddr;
    /*指向域变量的指针*/
    fieldChain* fieldMem = NULL;

    /*指向实参的指针*/
    ParamTable* curParam = NULL;

    switch (t->kind.stmt)
    {
        /*处理if语句*/
    case IfK: {
        p0 = t->child[0].get(); /*条件表达式部分*/
        p1 = t->child[1].get(); /*then语句序列部分*/
        p2 = t->child[2].get(); /*else语句序列部分*/
        emitComment("->if");
        cGen(p0);         /*产生测试表达式的代码*/

        /* savedLoc1赋值为当前指令的tm地址号(emitLoc),emitLoc加1,       *
        /* 为if条件表达式为假时，产生跳转地址回填所留一代码空位         */
        savedAddr1 = curAddr;
        savedLoc1 = emitSkip(1);

        /* 指令回填地址savedLoc1记录当前生成代码写入地址,				*
         * 再将当前生成代码写入地址emitLoc加1							*
         * 为条件表达式为假的跳转指令留一条指令写入空位,用于代码回填	*
         * 将注释写入目标代码文件code,此处为向else跳转指令位置			*/
         /*产生then语句部分的代码*/
        cGen(p1);

        /* 指令回填地址savedLoc2记录当前生成代码写入地址,				*
         * 再将当前生成代码写入地址emitLoc加1							*
         * 为跳转到判断语句结束位置的指令留一条代码空位,用于代码回填	*
         * 将注释写入目标代码文件code,此处为向end跳转指令位置	        */
        savedAddr2 = curAddr;
        savedLoc2 = emitSkip(1);

        /* 指令回填地址currentLoc为当前生成代码写入地址	*
         * 当前地址为条件为假的处理指令开始地址			*/
        currentAddr = curAddr;
        currentLoc = emitSkip(0);
        string label_else = emitLabel("Else");
        /*退回到指令回填地址savedLoc1,此处已经预留了一个指令空位*/
        emitBackup(savedLoc1, savedAddr1);

        /* 写入跳转到else的指令,此时跳转位置currentLoc已经得到 *
         * 指令的跳转地址为相对地址							   */
        objMips_Conj("beq", T0, ZERO, label_else, "跳转到else分支");
        /* 恢复当前生成代码写入地址emitLoc为指令回填地址currentLoc,	*
         * 恢复后地址为条件为假处理指令开始地址,即else开始位置		*/
        emitRestore();

        /* 处理else语句部分的代码 */
        cGen(p2);

        /* 指令回填地址currentLoc作为当前生成代码写入地址	*
         * 当前地址为判断语句结束位置						*/
        currentLoc = emitSkip(0);
        string label_endif = emitLabel("EndIf");
        /* 回退到指令回填地址savedLoc2，此处已经预留了一个指令空间 */
        emitBackup(savedLoc2, savedAddr2);

        /* 写入跳转到end的指令，此时跳转位置currentLoc已经得到地址 */

        objMips_UnConj("j", label_endif, "jmp to end");
        /* 恢复当前生成代码写入地址emitLoc为指令回填地址currentLoc,	*
         * 恢复后地址为判断语句结束地址,即end位置	    			*/
        emitRestore();

        emitComment("<- if");            /*if语句结束*/
        break;
    }
            /*处理while语句*/
    case WhileK: {
        emitComment("->while");
        string while_label = emitLabel("While");
        p0 = t->child[0].get();  /*p0为while语句的条件表达式部分*/
        p1 = t->child[1].get();  /*p1为while语句的语句序列部分*/
        currentAddr = curAddr;
        currentLoc = emitSkip(0);

        cGen(p0);          /*生成条件表达式部分代码*/

        /* 如果条件表达式为假，则跳转至while语句结束   *
        /* 此处为地址回填预留一个指令空间              */
        savedAddr1 = curAddr;
        savedLoc1 = emitSkip(1);

        emitComment(" while : jump out while ");

        cGen(p1);     /*生成语句序列部分代码*/

        /*跳到条件表达式处，进入下一次循环*/
        objMips_UnConj("j", while_label, "jump to start");
        emitComment(" return to condition exp");
        string endwhile_label = emitLabel("EndWhile");
        /*条件为假时,跳出while循环*/
        currentAddr = curAddr;
        currentLoc = emitSkip(0);

        emitBackup(savedLoc1, savedAddr1);

        objMips_Conj("beq", T0, ZERO, endwhile_label, " jump out while ");
        emitRestore();

        emitComment("<-while");

        break;
    }
               /*处理赋值语句*/
    case AssignK:
    {
        emitComment("->assign");

        p0 = t->child[0].get();  /*赋值号左侧的部分*/

        p1 = t->child[1].get();  /*赋值号右侧的部分*/

        findAbsAddr(p0);

        objMips_Regs("move", T2, T0, "save ac");
        cGen(p1);          /*处理赋值号右侧的表达式部分*/
        /*结果存入ac中*/

        if (p0->table[0]->attrIR->More.VarAttr.access == dir)
            /*赋值*/
            objMips_RM("sw", T0, T2, 0, "var assign : store value");
        else
        {

            objMips_RM("lw", T2, T2, 0, "indir var assign");
            objMips_RM("sw", T0, T2, 0, "store value");

        }
        emitComment("<- assign");
        break;
    }

    /*处理输入语句*/
    case ReadK:

        emitComment("-> read");
        objMips_SysCall(5);
        findAbsAddr(t);
        //记得往T0存变量的绝对地址!!!!!!!!!!!!!!!!!!
        objMips_Regs("li", T1, "4", "");
        objMips_RM("sw", V0, SP, 0, "保存读入的值在栈中");
        objMips_Regs("sub", SP, SP, T1, "read语句开辟栈存储读入的值");
        if (t->table[0]->attrIR->More.VarAttr.access == dir)
        {
            /*直接存*/
            /*最后生成存储指令*/
            objMips_RM("lw", T1, SP, 4, " 栈中取read的数");

            objMips_RM("sw", T1, T0, 0, " var read : store value");
        }
        else
        {
            objMips_RM("lw", T1, T0, 0, "");
            objMips_RM("lw", T0, SP, 4, " 栈中取read的数");
            objMips_RM("sw", T0, T1, 0, " var read : store value");
        }
        objMips_Regs("li", T1, "4", "");
        objMips_Regs("add", SP, SP, T1, "read语句开辟栈存储读入的值");
        emitComment("<- read");

        break;

        /* 处理write语句类型 */
    case WriteK:
        emitComment("-> write");

        p0 = t->child[0].get();
        cGen(p0);             /*处理write语句部分的表达式部分*/

        objMips_SysCall(1, T0);
        /*输出后都打印换行符*/
        mipsCode[curLine++] = "li $v0, 4       # 设置系统调用编号为4（打印字符串）";
        mipsCode[curLine++] = "la $a0, newline # 加载换行符的地址到$a0寄存器";
        mipsCode[curLine++] = "syscall         # 调用系统调用";
        curAddr += 12;
        emitComment("<- write");
        break;

        /* 处理过程调用语句 */

    case ReturnK:

        emitComment("-> return");
        objMips_RM("lw", T2, T4, -4, "");
        objMips_UnConj("jr", T2, " return address");
        emitComment("<- return");
        break;
    case CallK: {
        emitComment("-------------------> Call Start");
        p0 = t->child[0].get();    /*过程名*/
        p1 = t->child[1].get();    /*过程的实参*/

        pp = p0;
        curParam = t->table[0]->attrIR->More.ProcAttr.param;
        SymbolsTable* procSymbol = t->table[0];


        while ((curParam != NULL) && (p1 != NULL)) {
            FormParam = curParam->entry->attrIR->More.VarAttr.off;

            /*形参是indir*/
            if (curParam->entry->attrIR->More.VarAttr.access == indir) {
                SymbolsTable* symbol = p1->table[0];
                
                /*放弃了搞不了*/
                /*如果形参是var 数组,并且传入参数为array，应该把数组首地址传入*/
                /*if (symbol->attrIR->idType->typeKind == ArrayK) {
                    findAbsAddr(p1, 1);
                    objMips_RM("sw", T0, SP, -FormParam, "如果形参是var数组,并且传入参数为array，应该把数组首地址传入");
                }*/
                
                /*实参是indir,把地址传入形参*/
                if (symbol->attrIR->More.VarAttr.access == indir) {
                   
                        findAbsAddr(p1);
                        objMips_RM("lw", T0, T0, 0, "变参取值,取出来的是地址");
                        objMips_RM("sw", T0, SP, -FormParam, "变参，把地址传入为下一层存传入的参数");
                    
                }
                /*实参是dir，直接把地址传入，不用二次取址*/
                else
                {
                    findAbsAddr(p1);
                    objMips_RM("sw", T0, SP, -FormParam, "为下一层存传入的参数");
                }

            }
            /*如果形参是dir*/
            else
            {   
                /*如果传入的参数是标识符*/
                if (p1->kind.exp == IdEK) {
                    SymbolsTable* symbol = p1->table[0];
                    /*这个里面如果是dir,返回的是值，如果是indir也是返回的值*/
                    genExp(p1);
                    
                    if (symbol->attrIR->More.VarAttr.access == indir) {
                        objMips_Regs("move", T2, T0, "调用参数是间接变量");
                        //objMips_RM("lw", T2, T0, 0, "是间接变量");
                        //objMips_RM("lw", T2, T2, 0, "");
                        objMips_RM("sw", T2, SP, -FormParam, "为下一层存传入的参数");
                    }
                    /*如果是直接变量，把值传进去*/
                    else {
                        //objMips_RM("lw", T2, T0, 0, "是直接变量");
                        objMips_RM("sw", T0, SP, -FormParam, "为下一层存传入的参数");
                    }
                }
                /*如果传入的参数是常数*/
                else {
                    genExp(p1);
                    objMips_RM("sw", T0, SP, -FormParam, "为下一层存传入的参数");
                }
            }

            curParam = curParam->next;
            p1 = p1->sibling.get();
        }
        objMips_RM("sw", T4, SP, 0, "存储动态链条地址");
        objMips_Regs("li", T5, to_string(procSymbol->attrIR->More.ProcAttr.level), "T5此时存层数");
        objMips_RM("sw", T5, T4, -8, "T5存入活动记录");
        objMips_RM("sw", T0, SP, -12, "存T0数值到栈");
        objMips_RM("sw", T1, SP, -16, "存T1数值到栈");
        objMips_RM("sw", T2, SP, -20, "存T2数值到栈");
        objMips_Regs("li", T5, to_string(procSymbol->attrIR->More.ProcAttr.off), "T5此时displayoff表偏移");
        objMips_RM("sw", T5, SP, -24, "存displayOff到栈");
        objMips_Regs("sub", T5, SP, T5, "T5存display表绝对地址");
        int ACSIZE = procSymbol->attrIR->More.ProcAttr.moff;
        objMips_Regs("move", T4, SP, "将新生成记录的栈底存储T4");
        //接下来的栈底统一用T4！！！！
        objMips_RegIm("addi", SP, SP, to_string(-ACSIZE), "申请ACSIZE大小的活动记录空间,SP指向栈顶");
        int displayFill = procSymbol->attrIR->More.ProcAttr.off;
        savedAddr1 = curAddr;
        savedLoc1 = emitSkip(2);
        for (int i = 0; i < procSymbol->attrIR->More.ProcAttr.level; i++) {
            objMips_RM("lw", T5, T4, 0, "动态链取上层记录初始地址");
            objMips_RM("lw", T6, T5, -24, "取上层记录display表偏移地址");
            objMips_Regs("sub", T5, T5, T6, "T5存算出的上层display表绝对地址,T5=T5-T6");

            objMips_RM("lw", T6, T5, i * (-4), "T6此时上层display表中第i层内容");
            objMips_RM("sw", T6, T4, -displayFill, "这里T4是本层起始地址，从这计算的偏移,本层对应i层存入对应上层display表第i个数据");
            displayFill += 4;
        }
        objMips_RM("sw", T4, T4, -displayFill, "还有本层起始地址也要填入display表");
        //返回地址
        currentAddr = curAddr;
        currentLoc = emitSkip(0);
        emitBackup(savedLoc1, savedAddr1);
        /*这里不能这样填*/
        //objMips_Regs("li", T1, to_string(currentAddr), "载入返回地址");

        //objMips_RM("sw", T1, T4, -4, "回填返回地址");
        emitRestore();

        objMips_UnConj("jal", "Proc_" + p0->name[0], "转向子程序");

        objMips_RM("lw", T0, T4, -12, "恢复T0");
        objMips_RM("lw", T1, T4, -16, "恢复T1");
        objMips_RM("lw", T2, T4, -20, "恢复T2");
        objMips_RegIm("addi", SP, T4, "0", "恢复SP");
        objMips_RM("lw", T4, T4, 0, "恢复T4");
        emitComment("<------------------- Call End");
    }
              break;
    default:
        break;
    }
}

void genProc(Treenode* t) {
    int savedLoc1;         /*处理过程入口时所需的代码地址*/
    int savedAddr1;
    /*处理过程体部分*/
    int currentAddr = curAddr;
    int currentLoc = emitSkip(0);
    t->table[0]->attrIR->More.ProcAttr.code = currentLoc;
    emitComment("->procedure");
    /*生成标签*/
    /*修改处*/
    genLabel("Proc_" + t->name[0]);
    /*先填写返回地址*/
    objMips_RM("sw", RA, T4, -4, "填写返回地址");

    TreenodePtr& tp1 = t->child[1];//声明部分
    Treenode* p1=tp1.get();
    TreenodePtr& tp2 = t->child[2];//过程体
    Treenode* p2=tp2.get();

    /*存一条指令，跳转到begin处*/
    savedAddr1 = curAddr;
    savedLoc1 = emitSkip(1);

    while (p1 != NULL)
    {
        /*处理过程声明部分*/
        if (p1->nodekind == ProcDecK)
        {
            genProc(p1);
        }
        p1 = p1->sibling.get();
    }
    /*在此处生成begin标签*/
    genLabel(t->name[0] + "_begin");
    currentAddr = curAddr;
    currentLoc = emitSkip(0);
    emitBackup(savedLoc1, savedAddr1); //回填跳转指令 
    /*填写保留的跳转标签*/
    objMips_UnConj("j", t->name[0] + "_begin", "过程入口");
    emitRestore();
    if (p2 != NULL)
        p2 = p2->child[0].get();
    while (p2 != NULL)
    {
        genStmt(p2);
        p2 = p2->sibling.get();
    }
    /*那return用不用无所谓了*/
    objMips_RM("lw", T2, T4, -4, " fetch return address");
    objMips_UnConj("jr", T2, "回到上一级过程");
    emitComment("<-procedure");
}

void codeGen(TreenodePtr t) {
    int currentLoc;//存储主程序入口地址
    int currentAddr;
    int savedloc;//目标代码第一条地址
    int savedAddr;
    TreenodePtr& MainProc = t->child[0];//Phead
    TreenodePtr& tmp = t->child[1];
    Treenode* DecNode=tmp.get();
    TreenodePtr& BodyNode = t->child[2];
    mipsCode.resize(2000, "");

     /*数据区换行符*/
     //写数据段和代码段
    mipsCode[curLine++] = DATASEG;
    highLine = max(curLine, highLine);
    mipsCode[curLine++] = "newline: .asciiz \"\\n\"";
    highLine = max(curLine, highLine);

    //写代码段
    mipsCode[curLine] = CODESEG;
    curLine++;
    highLine = max(curLine, highLine);

    
    savedAddr = curAddr;
    savedloc = emitSkip(1);//为主程序跳转预留空间，预留1条指令的空间用于回填

    //处理主程序声明部分
    while(DecNode != nullptr) {
        if (DecNode->nodekind == ProcDecK) {
            genProc(DecNode);
        }
        DecNode = DecNode->sibling.get();
    }

    string mainlabel = emitLabel("main");
    objMips_RegIm("addi", T4, SP, "0", "主过程活动记录生成！！！");
    SymbolsTable* proSymbol = MainProc->table[0];
    int ACSIZE = proSymbol->attrIR->More.ProcAttr.moff;
    int off = proSymbol->attrIR->More.ProcAttr.off;
    objMips_Regs("li", T1, to_string(off), "Main程序display偏移");
    //此时SP=T4，还没有作为栈顶
    objMips_RM("sw", T1, SP, -24, "Main程序存displayoff偏移");
    objMips_Regs("sub", T1, SP, T1, "Main程序display表绝对偏移地址");
    objMips_RM("sw", SP, T1, 0, "存储Main程序display表第一个元素，即他自己");
    objMips_RegIm("addi", SP, SP, to_string(-ACSIZE), "申请Main程序栈空间");

    //处理主程序体
    currentAddr = curAddr;
    currentLoc = emitSkip(0);
    emitBackup(savedloc, savedAddr);
    objMips_UnConj("j", mainlabel, "main entry");
    emitRestore();
    Treenode* stmtNode = BodyNode->child[0].get();
    if (stmtNode != nullptr) {
         cGen(stmtNode);
     }

    //结束部分
    objMips_Regs("li", V0, to_string(10), "退出程序");
    mipsCode[curLine] = "syscall";
    ofstream outObj;
    outObj.open(objAddr);
    for (int i = 0; i <= curLine; i++) {
        outObj << mipsCode[i] << endl;
    }
    outObj.close();
}

int main() {
	lexicial();
	Parser parser(move(tokenList));
	auto syntaxTree = parser.parse();
	parser.printTree(syntaxTree);
    auto analyzedTree=semanticAnalyze(move(syntaxTree));
    codeGen(move(analyzedTree));
	return 0;
}