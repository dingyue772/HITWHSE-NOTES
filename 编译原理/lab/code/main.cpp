#include <iostream>
#include <string.h>
#include <iomanip>
//#include <windows.h>
using namespace std;

// 数据结构定义
typedef struct WORD{
    int code; // token记号
    string key; // 值
    string addr = "#"; // 对于单个标记的token为#，对于类标记的toke，addr为其具体的值
    string position;
    int lineNum;
    string extra;
    bool variable;
} WORD; 

typedef struct SymbolLine{
    string addr;
    string value;
    string type;
    string vType;
} SymbolLine;

// 用于词法分析的关键字 
enum symbol{
    ADD = 21, SUBTRACT, MULTIPLY, DIVIDE, EQ_TWO, EQ, L_BRACE, R_BRACE, BIGGER_EQ, BIGGER, SMALLER_EQ, SMALLER, L_PARENTHESIS, R_PARENTHESIS, SEMICOLON, SRTAT_END
};

#define KEYWORD_NUM 8 
WORD token[100];
int top = 0;
int line = 1;

SymbolLine symbolTableL[100];
int tableTopL = 0;

char keywords[KEYWORD_NUM][20] = {"if", "else", "main", "while", "int", "return", "double", "boolean"};

WORD* getToken(){
    return token;
}

SymbolLine* getLSymbolTable(){
    return symbolTableL;
}

int getLSymbolTableSize(){
    return tableTopL;
}

void printSymbolTableL(){
    cout << "-----------------------符号表--------------------------" << endl;
    cout << "id    " << "name     " << "addr     " << endl;
    for(int i = 0; i < tableTopL; i++){
        cout << i << "     " << symbolTableL[i].value << "        " << symbolTableL[i].addr << endl;
    }
}

int getTop(){
    return top;
}

bool digit(char target){
    if(target >= '0' && target <= '9'){
        return true;
    }
    return false;
}

bool letter(char target){
    if(target >= 'a' && target <= 'z' || target >= 'A' && target <= 'Z' || target == '_'){
        return true;
    }
    return false;
}

int keyword(char target[]){
    for(int i = 0; i < KEYWORD_NUM; i++){
        if(strcmp(target, keywords[i]) == 0){
            return i + 1; // 返回关键字在数组中的位置
        }
    }
    return -1;
}

bool point(char target){ // 判断当前字符是否为小数点
    if(target == '.'){
        return true;
    }
    return false;
}

void deleteBlank(char *target){
    while(*target == ' ' || *target == 10 || *target == '\n') // 删除输入串中的空格、回车和换行符 
	{
        if(*target == '\n'){
            line++;
        }
        target++;
    }
}

// 在符号表中新增一行
void addNewSymbolLine(string target){
    for(int i = 0; i < tableTopL; i++){
        if(symbolTableL[i].value.compare(target) == 0){ // 当前符号在符号表中已经存在
            return;
        }
    }

    symbolTableL[tableTopL].addr = to_string(tableTopL);
    symbolTableL[tableTopL].type = "variables";
    symbolTableL[tableTopL].value = target; // 当前token
    symbolTableL[tableTopL].vType = "unknown";
    tableTopL++;
}

// 记录单词位置
void savePosition(){
    int leftTop = (top > 10) ? 10 : top;
    // 记录所在行数
    // token[top - 1].lineNum = line;
    // 最多记录附近单词
    for(int i = 0; i < leftTop - 1; i++){
        if(token[top - leftTop + i].lineNum != token[top - 1].lineNum){
            continue;
        }
        token[top - 1].position = token[top - 1].position + " " + token[top - leftTop + i].extra;
    }
    // 记录当前单词
    token[top - 1].position = token[top - 1].position + " <" + token[top - 1].extra + ">";
}

void addToken(int code, char* word){
    WORD w;
    w.code = code;
    w.key = string(word);
    w.extra = string(word);  // 变量要用到
    w.lineNum = line;
    token[top++] = w;
    // 记录单词位置
    savePosition();
}

void addTokenM(int code, char* word, char* addr, bool v){
    WORD w;
    w.code = code;
    w.addr = addr;
    w.key = string(word);
    w.variable = v;
    w.lineNum = line;

    w.extra = string(addr);

    token[top++] = w;

    savePosition();
}

void addOPToken(int code, char* word){
    WORD w;
    w.code = code;
    w.key = string(word);
    w.extra = string(word);  // 运算符要用到
    w.lineNum = line;
    token[top++] = w;

    savePosition();
}

void printToken(){
    for(int i = 0; i < top; i++){
        cout << "(" << token[i].code << ", " << token[i].key << ", " << token[i].addr << ")" << endl;
    }
}

void scan(char *words){
	cout << "lexical analysis begins!" << endl; 
    while(1){
        // 删除空格和换行符
        deleteBlank(words);
        // # 截止
        if(*words == '#'){
            break;
        }
        // 注释
        if(*words == '/'){
            if(*(words + 1) == '/'){
                words++;
                words++;
                while(*words != '\n'){
                    words++;
                }
                line++;
                addToken(200, "//");
                words++;
            } else if(*(words + 1) == '*'){
                words++;
                words++;
                while(*words != '*' && *(words + 1) != '/'){
                    if(*words == '\n'){
                        line++;
                    }
                    words++;
                }
                addToken(201, "/**/");
                words++;
                words++;
            }
        }
        // 字母
        if(letter(*words)){
            int i = 0;
            char temp[500] = "1";
            while(letter(*words) || digit(*words)){
                temp[i] = *words;
                words++;
                i++;
            }
            // 标识符是否为关键字
            int code = keyword(temp);
            if(code != -1){
               addToken(code, temp);
            } else {
                // 变量
               addTokenM(300, "id", temp, true);
               // 符号表
               addNewSymbolLine(string(temp));
            }
        } else if(digit(*words)){
            int i = 0;
            char temp[500] = "0";
            bool hasPoint = true; // 预设数字串中会有一个小数点
            while(digit(*words) || point(*words)){
                if(point(*words)){
                    if(hasPoint){
                        hasPoint = false; // 已经识别到唯一的小数点，标记之后不能再有小数点出现
                    } else {
                        temp[i] = *words;
                        words++;
                        i++;
                        addTokenM(100, "num", temp, false);
                        cout << "FloatError in line " << token[top - 1].lineNum << ":" << token[top - 1].position << endl;
                        exit(0);
                    }
                }
                temp[i] = *words;
                words++;
                i++;
            }
            if(point(*(words - 1))){
                cout << "This is error in syntax " << string(temp) << endl;
                return;
            }
            addTokenM(100, "num", temp, false);
        } else {
            switch (*words){
                case '+':
                    addOPToken(ADD, "+");
                    break;

                case '-':
                    addOPToken(SUBTRACT, "-");
                    break;

                case '*':
                    addOPToken(MULTIPLY, "*");
                    break;

                case '/':
                    addOPToken(DIVIDE, "/");
                    break;

                case '=':
                    if(*(words + 1) == '='){
                        addOPToken(EQ_TWO, "==");
                        words++;
                    } else {
                        addOPToken(EQ, "=");
                    }
                    break;

                case '{':
                    addOPToken(L_BRACE, "{");
                    break;

                case '}':
                    addOPToken(R_BRACE, "}");
                    break;

                case '>':
                    if(*(words + 1) == '='){
                        addOPToken(BIGGER_EQ, ">=");
                        words++;
                    } else {
                        addOPToken(BIGGER, ">");
                    }
                    break;

                case '<':
                    if(*(words + 1) == '='){
                        addOPToken(SMALLER_EQ, "<=");
                        words++;
                    } else {
                        addOPToken(SMALLER, "<");
                    }
                    break;

                case '(':
                    addOPToken(L_PARENTHESIS, "(");
                    break;

                case ')':
                    addOPToken(R_PARENTHESIS, ")");
                    break;

                case ';':
                    addOPToken(SEMICOLON, ";");
                    break;

                cout << "There is error in symbol " << *words << endl;
            }
            words++;
        }
    }
}



// 语法分析part
// 数据结构定义
typedef struct Item{
    int pId; // 使用产生式的id，项目表示使用的哪一个产生式 
    int idx;  // 在分析栈中最后一个元素的下标
} Item;
typedef struct Production{
    //int id;
    string leftPart; // 产生式左部 
    string rightPart[10]; // 产生式右部的符号数据 
    int rightPartLength; // 产生式右部最后一个符号对应下标 
} Production;
typedef struct State{
    int id;
    Item *items = (Item*)malloc(100 * sizeof(Item)); // 当前状态对应的项目集 
    int top = 0; // 尾部下标，即最后一个项目的下标 
} State;
typedef struct AnalysisCell{
    string op; // 操作，s——操作跳转/r——用第几个产生式进行规约/acc——接收/*——出错/gt——状态转换 
    int direct; // s、gt——跳转到的状态/r——规约时使用第几个产生式 
} AnalysisCell;
typedef struct Collection{
    int* vts = (int*)malloc(100 * sizeof(int)); // 存储first集和follow集的元素 
    int top = 0; // 计数用 
} Collection;

#define MAX 10000
#define START_END 0
// 变量定义
// 产生式
Production productions[100];  // 产生式集合
int productionTop = 0;  // 产生式集合最大下标

// 非终结符
string vtn[100];  // 非终结符集合
int vtnTop = 0;  // 非终结符集合最大下标

// 终结符
string terminal[100];  // 终结符集合
int terminalTop = 0; // 终结符集合最大下标 

// 状态
State CC[100];  // 项目集规范族
int CCTop = 0;  // 最大下标
State firstState;  // 初始状态

// 分析表
AnalysisCell actionTable[100][100];  // action 表
AnalysisCell gotoTable[100][100];  // goto 表

// FIRST 集
Collection FIRST[100];  // 非终结符下标对应的 first 集
int firstTop = 0;  // 数组尾

// FOLLOW 集
Collection FOLLOW[100];  // 非终结符下标对应的 follow 集
int followTop = 0;  // 数组尾 

// 复制item数组
Item* copyItems(State target){
    Item *newArr = (Item*)malloc(100 * sizeof(Item));
    for(int i = 0; i < target.top; i++){
        newArr[i] = target.items[i];
    }
    return newArr;
}

// 判断指定字符串是不是终结符
bool isNonTerminal(string target){
    for(int i = 0; i < vtnTop; i++){
        if(target.compare(vtn[i]) == 0){
            return true;
        }
    }
    return false;
}

// 判断一个状态里的item是否重复
bool itemRepeat(State state, int pId, int idx){
    for(int i = 0; i < state.top; i++){
        if(state.items[i].pId == pId && state.items[i].idx == idx){
            return true;
        }
    }
    return false;
}

// 是否存在产生式左部是指定字符串的产生式并返回其id
int* existLeftPro(string target){
    int* result = (int*)malloc(sizeof(int) * productionTop);
    int idx = 0;
    // 初始化-1
    for(int i = 0; i < productionTop; i++){
        result[i] = -1;
    }

    // 获取左部为指定非终结符的产生式
    for(int i = 0; i < productionTop; i++){
        if(productions[i].leftPart.compare(target) == 0){
            *(result + (idx++)) = i;
        }
    }

    return result;
}

// 获取闭包
State CLOSURE(State target){
    State newState;
    newState.items = copyItems(target);
    newState.top = target.top;


    // 判断是否停止
    Item *itemStack = copyItems(target);  // 待处理item栈
    int stackTop = target.top;  // 栈顶

    while(stackTop != 0){
        // 出栈
        stackTop--;
        int idx =  itemStack[stackTop].idx;
        Production tempPro = productions[itemStack[stackTop].pId];
        // 判断该item是否还能扩展
        if(idx < tempPro.rightPartLength){
            string tempVT = tempPro.rightPart[idx + 1];  // 点右边的第一个字符
            if(isNonTerminal(tempVT)){  // 如果是非终结符
                int* pId = existLeftPro(tempVT);
                for(int i = 0; i < productionTop; i++){
                    int id = *(pId + i);
                    if(id != -1 && !itemRepeat(newState, id, -1)){
                        // 添加新的item，点标在最左边
                        newState.items[newState.top].pId = id;
                        newState.items[newState.top].idx = -1;
                        // 入栈
                        itemStack[stackTop++] = newState.items[newState.top];
                        newState.top++;
                    }
                }
            }
        }
    }

    return newState;
}

// GOTO
State GOTO(State state, string x){
    State j;
    j.top = 0;
    for(int i = 0; i < state.top; i++){
        int pId = state.items[i].pId;
        int idx = state.items[i].idx;
        if(productions[pId].rightPart[idx + 1].compare(x) == 0){  // 如果下一个符号是目标符号
            j.items[j.top].idx = idx + 1;
            j.items[j.top].pId = pId;
            j.top++;
        }
    }
    return CLOSURE(j);
}

// 检查两个状态是否一致
bool checkTwoState(State s1, State s2){
    // item个数都不一样肯定不一致
    if(s1.top != s2.top){
        return false;
    }

    // 检查item是否一致
    for(int i = 0; i < s1.top; i++){
        if(!itemRepeat(s2, s1.items[i].pId, s1.items[i].idx)){
            return false;
        }
    }

    return true;
}

// 检查项目集规范族是否已经有一个状态
bool checkItemInCC(State state){
    //cout << "s" << endl;
    for(int i = 0; i < CCTop; i++){
        if(checkTwoState(CC[i], state)){
            return true;
        }
    }

    return false;
}

// 打印状态项目
void printState(State tempState){
    for(int j = 0; j < tempState.top; j++){
            Item tempItem = tempState.items[j];
            Production tempPro = productions[tempItem.pId];
            cout << tempPro.leftPart << "-->";
            for(int k = 0; k <= tempPro.rightPartLength + 1; k++){
                if(tempItem.idx == k - 1){
                    cout << ".";
                }
                if(k != tempPro.rightPartLength + 1){
                    cout << tempPro.rightPart[k];
                }
            }
            cout << endl;
        }
        cout << "--------------------------------" << endl;
}

// 获取项目集规范族
void getCanonicalCollection(){
    // 获取拓广文法新加产生式的项目（如：S'->.S）
    firstState.items[0].pId = 0;
    firstState.items[0].idx = -1;
    firstState.top = 1;
    // 获取该项目的闭包作为初始状态
    CC[CCTop++] = CLOSURE(firstState);

    State stateStack[MAX];
    stateStack[0] = CC[CCTop - 1];
    int stackTop = 1;

    while(stackTop != 0){
        // 出栈
        stackTop--;
        State topState = stateStack[stackTop];
        // 对非终结符
        for(int i = 0; i < vtnTop; i++){
            State temp = GOTO(topState, vtn[i]);
            if(temp.top !=0 && !checkItemInCC(temp)){  // 一个状态不能为空，如当GOTO(i, "#")时
                stateStack[stackTop++] = temp;
                CC[CCTop++] = temp;
            }
        }
        // 对终结符
        for(int i = 0; i < terminalTop; i++){
            State temp = GOTO(topState, terminal[i]);
            if(temp.top !=0 && !checkItemInCC(temp)){
                stateStack[stackTop++] = temp;
                CC[CCTop++] = temp;
            }
        }
    }
}

// 获取一个状态的序号
int getStateId(State target){
    for(int i = 0; i < CCTop; i++){
        if(checkTwoState(CC[i], target)){
            return i;
        }
    }

    printf("get state id error!!!!!!!!!\n");
    return -1;
}

// 获取一个非终结符id
int getVtnId(string target){
    for(int i = 0; i < vtnTop; i++){
        if(target.compare(vtn[i]) == 0){
            return i;
        }
    }

    //printf("get vtn id error!!!!!!!!!\n");
    return -1;
}

// 获取一个终结符id
int getTerminalId(string target){
    for(int i = 0; i < terminalTop; i++){
        if(target.compare(terminal[i]) == 0){
            return i;
        }
    }

    //printf("get terminal id error!!!!!!!!!\n");
    return -1;
}

// 初始化分析表
void initAnalysisTable(){
    // goto 表
    for(int i = 0; i < CCTop; i++){
        for(int j = 0; j < vtnTop; j++){
            gotoTable[i][j].direct = -1;
            gotoTable[i][j].op = "*";
        }
    }
    // action 表
    for(int i = 0; i < CCTop; i++){
        for(int j = 0; j < terminalTop; j++){
            actionTable[i][j].direct = -1;
            actionTable[i][j].op = "*";
        }
    }
}

// 检查follow或first集中是否有一个字符
bool checkCollection(Collection cl, int vId){
    for(int i = 0; i < cl.top; i++){
        if(cl.vts[i] == vId){
            return true;
        }
    }

    return false;
}

// 将一个集合加到另一个中，参数follow是指是否加在follow集中
void addCollection(int id, Collection target, bool follow){
    // 加到follow集
    if(follow){
        for(int i = 0; i < target.top; i++){
            if(!checkCollection(FOLLOW[id], target.vts[i])){
                FOLLOW[id].vts[FOLLOW[id].top++] = target.vts[i];
            }
        }
    } else {
        // 加到first集
        for(int i = 0; i < target.top; i++){
            if(!checkCollection(FIRST[id], target.vts[i])){
                FIRST[id].vts[FIRST[id].top++] = target.vts[i];
            }
        }
    }
}

// 获取并产生某一非终结符的FIRST集
Collection getFIRST(int vId){
    // 如果该first集已经有了，就直接返回
    if(FIRST[vId].top != 0){
        return FIRST[vId];
    }
    // 计算其first集
    for(int i = 0; i < productionTop; i++){
        // 如果产生式的左部是v
        if(productions[i].leftPart.compare(vtn[vId]) == 0){
            // 如果产生式右部的第一个符号是自己，要跳过不然就是将FIRST[X]放到FIRST[X]中，会死循环
            if(productions[i].rightPart[0].compare(vtn[vId]) == 0){
                continue;
            }
            // 如果产生式右部第一个符号是终结符，就把该终结符加进去
            int id = getTerminalId(productions[i].rightPart[0]);
            if(id != -1){
                if(!checkCollection(FIRST[vId], id)){
                    FIRST[vId].vts[FIRST[vId].top++] = id;
                }
            } else {
                id = getVtnId(productions[i].rightPart[0]);
                if(id == -1){
                    cout << "cant find symbol [" << productions[i].rightPart[0] << "]" << endl;
                    return FIRST[vId];
                }
                // 产生式右部第一个符号是非终结符，将它的first集加入
                addCollection(vId, getFIRST(id), false);
            }
        }
    }

    return FIRST[vId];
}

// 初始化FIRST集
void initFIRST(){
    // 为每个非终结符计算first集
    for(int i = 0; i < vtnTop; i++){
        getFIRST(i);
    }
}

// 获取follow集
Collection getFOLLOW(int vId){
    // 如果该follow集已经有了，就直接返回
    if(FOLLOW[vId].top != 0){
        return FOLLOW[vId];
    }
    if(vId == 0){
        // 扩广文法新加的产生式的右部自带 #，如 S' -> S 中的 S
        FOLLOW[0].vts[FOLLOW[0].top++] = getTerminalId("#");
    }
    // 构建follow集
    for(int i = 1; i < productionTop; i++){
        for(int j = 0; j <= productions[i].rightPartLength; j++){
            if(vtn[vId].compare(productions[i].rightPart[j]) == 0){
                int id = -1;
                // 如果该非终结符在产生式右部最后
                if(j == productions[i].rightPartLength){
                    // 左部最后一个符号不能是自己，不然会死循环
                    if(vtn[vId].compare(productions[i].leftPart) == 0){
                        continue;
                    }
                    id = getVtnId(productions[i].leftPart);
                    if(id == -1){
                        cout << "unknown non-terminal v: [" << productions[i].leftPart << "]" << endl;
                        return FOLLOW[vId];
                    }
                    // 把产生式左部非终结符的follow集加入
                    addCollection(vId, getFOLLOW(id), true);
                } else if((id = getTerminalId(productions[i].rightPart[j + 1])) != -1) {
                    // 如果该非终结符的下一个符号是终结符，直接加到follow集中
                    if(!checkCollection(FOLLOW[vId], id)){
                        FOLLOW[vId].vts[FOLLOW[vId].top++] = id;
                    }
                } else if((id = getVtnId(productions[i].rightPart[j + 1])) != -1){
                    // 如果该非终结符下一个符号是另一个非终结符，就把该非终结符的first集加入
                    addCollection(vId, getFIRST(id), true);
                }
            }
        }
    }

    return FOLLOW[vId];
}

// 初始化FOLLOW集
void initFOLLOW(){
    // 为每个非终结符计算follow集
    for(int i = 0; i < vtnTop; i++){
        getFOLLOW(i);
    }
}

// FOLLOW 集是否存在某个元素
bool inFOLLOW(string v, int target){
    // 获取非终结符id
    int vId = getVtnId(v);
    if(vId == -1){
        cout << "get non-terminal v error" << endl;
        return false;
    }
    // 获取follow集
    Collection flc = FOLLOW[vId];
    // 判断对应终结符是否在非终结符v的follow集中
    for(int i = 0; i < flc.top; i++){
        if(target == flc.vts[i]){
            return true;
        }
    }

    return false;
}

// 打印分析表
void printAnalysisTable(){
    cout << std::setw(6) << "";
    for(int i = 0; i < terminalTop; i++){
        cout << std::setw(6) << terminal[i];
    }
    for(int i = 0; i < vtnTop; i++){
        cout << std::setw(6) << vtn[i];
    }
    cout << endl;
    for(int i = 0; i < CCTop; i++){
        cout << i << "   ";
        for(int j = 0; j < terminalTop; j++){
            if(actionTable[i][j].direct == -1){
                cout << std::left << std::setw(6) << actionTable[i][j].op;
            } else {
                string display = actionTable[i][j].op + to_string(actionTable[i][j].direct);
                cout << std::left << std::setw(6) << display;
            }
        }
        for(int j = 0; j < vtnTop; j++){
            if(gotoTable[i][j].direct == -1){
                cout << std::left << std::setw(6) << gotoTable[i][j].op;
            } else {
                string display = gotoTable[i][j].op + to_string(gotoTable[i][j].direct);
                cout << std::left << std::setw(6) << display;
            }
        }
        printf("\n");
    }
}

// 构造分析表
void developAnalysisTable(){
    // 遍历状态
    for(int i = 0; i < CCTop; i++){
        // 当前状态
        State tempState = CC[i];
        // 遍历状态中的项目
        for(int j = 0; j < tempState.top; j++){
            // 当前项目
            Item tempItem = tempState.items[j];
            // action 与 goto
            if(tempItem.idx < productions[tempItem.pId].rightPartLength){
            	// 获取下一个符号 
                string terompEle = productions[tempItem.pId].rightPart[tempItem.idx + 1];
                // 终结符
                int eleId = getTerminalId(terompEle);
                // 如果不是终结符，填goto表
                if(eleId == -1){
                    eleId = getVtnId(terompEle);
                    if(eleId == -1){
                        cout << "cant recognize the symbol [" << terompEle << "]" << endl;
                        return;
                    }
                    // 获取下一个状态
                    State nextState = GOTO(tempState, terompEle);
                    // 获取状态id
                    int nextStateId = getStateId(nextState);
                    if(nextStateId == -1){
                        printf("cant get state id!!!!!!!!!!!\n");
                        return;
                    }
                    // 填表
                    gotoTable[i][eleId].direct = nextStateId;
                    gotoTable[i][eleId].op = "gt";
                } else {
                    // action 表
                    // 获取下一个状态
                    State nextState = GOTO(tempState, terompEle);
                    // 获取状态id
                    int nextStateId = getStateId(nextState);
                    if(nextStateId == -1){
                        printf("cant get state id!!!!!!!!!!!\n");
                        return;
                    }
                    // 填表
                    actionTable[i][eleId].direct = nextStateId;
                    actionTable[i][eleId].op = "s";
                }
            } else {
                // acc
                if(tempItem.pId == 0 && tempItem.idx == 0){
                    actionTable[i][getTerminalId("#")].op = "acc";
                } else {
                    // 归约
                    for(int m = 0; m < terminalTop; m++){
                        // 判断下一个终结符是否在该变量的follow集中
                        if(inFOLLOW(productions[tempItem.pId].leftPart, m)){
                        //if(true){
                            actionTable[i][getTerminalId(terminal[m])].op = "r";
                            actionTable[i][getTerminalId(terminal[m])].direct = tempItem.pId;
                        }
                    }
                }
            }
        }
    }
}

// 打印栈内外字符串
void printSymbolString(WORD *symbolStack, int stackTop, WORD *symbolNotInStack, int waitIn, int notInTop){
    string in;
    for(int i = 0; i < stackTop; i++){
        in = in + "{" + symbolStack[i].key + "}";
    }
    string out;
    for(int i = waitIn; i < notInTop; i++){
        out = out + "{" + symbolNotInStack[i].key + "}";
    }
    cout << "symbol stack: " << endl;
    cout << std::left << std::setw(50) << in;
    cout << std::setw(50) << out;
    cout << endl;
}

// 打印状态栈
void printStateString(int *stateStack, int stackTop){
    string states;
    for(int i = 0; i < stackTop; i++){
        states = states + "{" + to_string(stateStack[i]) + "}";
    }
    cout << "state stack: " << endl;
    cout << std::left << std::setw(50) << states;
    cout << endl;
}

// 语法分析
void analysis(WORD target[], int wordNum){
    WORD symbolStack[MAX];  // 符号栈
    int symbolStackTop = 0;  // 符号栈顶
    int stateStack[MAX];  // 状态栈
    int stateStackTop = 0;  // 状态栈顶

    // 最后是#
    target[wordNum].key = "#";
    target[wordNum].code = getTerminalId("#");
    wordNum++;

    // 将状态0和#入栈
    symbolStack[symbolStackTop].code = START_END;
    symbolStack[symbolStackTop].key = "#";
    symbolStackTop++;

    stateStack[stateStackTop++] = 0;

    // 开始归约
    int wait = 0;

    cout << "------------------state analysis-------------------" << endl;
    printStateString(stateStack, stateStackTop);
    printSymbolString(symbolStack, symbolStackTop, target, wait, wordNum);
    cout << "---------------------------------------------" << endl;

    while(true){
        // 获取要读取的符号
        WORD tempWord = target[wait++];
        int id = getTerminalId(tempWord.key);
        int currentState = stateStack[stateStackTop - 1];
        // 判断是不是终结符
        if(id == -1){  // 是变量
            id = getVtnId(tempWord.key);
            if(id == -1){
                cout << "cant recognize the symbol [" << tempWord.key << "]" << endl;
            }

            AnalysisCell cell = gotoTable[currentState][id];
            if(cell.op.compare("*") == 0){
                cout << "analysis error in goto table (" << currentState << "," << id << ")" << endl;
                return;
            }
            cout << "goto(" << currentState << ", " << tempWord.key << ") = " << cell.direct << endl;
            // 将状态压入栈
            stateStack[stateStackTop++] = cell.direct;
            // 将符号压入栈
            symbolStack[symbolStackTop++] = tempWord;
        } else {  // 终结符
            AnalysisCell cell = actionTable[currentState][id];
            if(cell.op.compare("acc") == 0){
                symbolStack[symbolStackTop].key = "#";
                symbolStack[symbolStackTop].code = getTerminalId("#");
                symbolStackTop++;
                cout << "---------------------------------------------" << endl;
                printStateString(stateStack, stateStackTop);
                printSymbolString(symbolStack, symbolStackTop, target, wait, wordNum);
                cout << "---------------------------------------------" << endl;
                cout << "analysis successfully!" << endl;
                return;
            }
            if(cell.op.compare("*") == 0){
                cout << "analysis error in action table (" << currentState << "," << id << ")" << endl;
                cout << "throw error at line " << tempWord.lineNum << ":" << tempWord.position << endl;
                exit(0);
            }
            // 继续移进
            if(cell.op.compare("s") == 0){
                cout << "action(" << currentState << ", " << tempWord.key << ") = " << cell.op << to_string(cell.direct) << endl;
                stateStack[stateStackTop++] = cell.direct;
                symbolStack[symbolStackTop++] = tempWord;
            }
            // 归约
            if(cell.op.compare("r") == 0){
                // 获取产生式
                Production tempProduction = productions[cell.direct];
                cout << "action(" << currentState << ", " << tempWord.key << ") = " << cell.op << to_string(cell.direct) << endl;
                cout << "use production: {" << tempProduction.leftPart << " -> ";
                for(int k = 0; k <= tempProduction.rightPartLength; k++){
                    cout << tempProduction.rightPart[k];
                }
                cout << "} to reduce" << endl;
                // 将待识别的符号改成产生式左部
                wait = wait - 2;
                target[wait].key = tempProduction.leftPart;
                target[wait].code = getVtnId(tempProduction.leftPart);
                // 符号栈和状态栈弹出产生式右部长度个元素
                stateStackTop = stateStackTop - tempProduction.rightPartLength - 1;
                symbolStackTop = symbolStackTop - tempProduction.rightPartLength - 1;
            }
        }

        // 打印分析过程
        printStateString(stateStack, stateStackTop);
        printSymbolString(symbolStack, symbolStackTop, target, wait, wordNum);
        cout << "---------------------------------------------" << endl;
    }
}

// 打印first集
void printFIRST(){
    cout << "----------------------- FIRST --------------------------" << endl;
    for(int i = 0; i < vtnTop; i++){
        cout << std::left << std::setw(7) << "{" + vtn[i] + "}" + "'s fisrt collection is : ";
        for(int j = 0; j < FIRST[i].top; j++){
            cout << "{" + terminal[FIRST[i].vts[j]] + "}";
        }
        cout << endl;
    }
}

// 打印follow集
void printFOLLOW(){
    cout << "----------------------- FOLLOW --------------------------" << endl;
    for(int i = 0; i < vtnTop; i++){
        cout << std::left << std::setw(7) << "{" + vtn[i] + "}" + "'s follow collection is : ";
        for(int j = 0; j < FOLLOW[i].top; j++){
            cout << "{" + terminal[FOLLOW[i].vts[j]] + "}";
        }
        cout << endl;
    }
}

// 语法分析，target是待处理符号串，wordNum是target的符号个数
void grammaticalAnalysis(WORD* target, int wordNum){
        // 设定产生式
        productions[productionTop].leftPart = "S'";
        productions[productionTop].rightPart[0] = {"S"};
        productions[productionTop].rightPartLength = 0;
        productionTop++;

        productions[productionTop].leftPart = "S";
        productions[productionTop].rightPartLength = -1;
        productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {"W"};
        productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {"("};
        productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {"C"};
        productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {")"};
        productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {"{"};
        productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {"S"};
        productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {"}"};
        productionTop++;

        productions[productionTop].leftPart = "W";
        productions[productionTop].rightPartLength = -1;
        productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {"if"};
        productionTop++;

        productions[productionTop].leftPart = "S";
        productions[productionTop].rightPartLength = -1;
        productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {"E"};
        productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {"S"};
        productionTop++;

        productions[productionTop].leftPart = "S";
        productions[productionTop].rightPartLength = -1;
        productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {"E"};
        productionTop++;

        productions[productionTop].leftPart = "S";
        productions[productionTop].rightPartLength = -1;
        productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {"E"};
        productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {";"};
        productionTop++;

        productions[productionTop].leftPart = "W";
        productions[productionTop].rightPartLength = -1;
        productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {"while"};
        productionTop++;

        productions[productionTop].leftPart = "E";
        productions[productionTop].rightPartLength = -1;
        productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {"id"};
        productionTop++;

        productions[productionTop].leftPart = "E";
        productions[productionTop].rightPartLength = -1;
        productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {"num"};
        productionTop++;

        productions[productionTop].leftPart = "C";
        productions[productionTop].rightPartLength = -1;
        productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {"E"};
        productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {"U"};
        productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {"E"};
        productionTop++;

        productions[productionTop].leftPart = "U";
        productions[productionTop].rightPartLength = -1;
        productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {"=="};
        productionTop++;

        productions[productionTop].leftPart = "U";
        productions[productionTop].rightPartLength = -1;
        productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {"<"};
        productionTop++;

        productions[productionTop].leftPart = "U";
        productions[productionTop].rightPartLength = -1;
        productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {">"};
        productionTop++;

        productions[productionTop].leftPart = "E";
        productions[productionTop].rightPartLength = -1;
        productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {"E"};
        productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {"O"};
        productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {"E"};
        productionTop++;

        productions[productionTop].leftPart = "O";
        productions[productionTop].rightPartLength = -1;
        productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {"+"};
        productionTop++;

        productions[productionTop].leftPart = "O";
        productions[productionTop].rightPartLength = -1;
        productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {"-"};
        productionTop++;

        productions[productionTop].leftPart = "O";
        productions[productionTop].rightPartLength = -1;
        productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {"*"};
        productionTop++;

        productions[productionTop].leftPart = "O";
        productions[productionTop].rightPartLength = -1;
        productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {"/"};
        productionTop++;

        productions[productionTop].leftPart = "O";
        productions[productionTop].rightPartLength = -1;
        productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {"="};
        productionTop++;

        productions[productionTop].leftPart = "E";
        productions[productionTop].rightPartLength = -1;
        productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {"T"};
        productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {"E"};
        productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {"="};
        productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {"E"};
        productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {";"};
        productionTop++;

        productions[productionTop].leftPart = "T";
        productions[productionTop].rightPartLength = -1;
        productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {"int"};
        productionTop++;

        productions[productionTop].leftPart = "T";
        productions[productionTop].rightPartLength = -1;
        productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {"double"};
        productionTop++;

        productions[productionTop].leftPart = "T";
        productions[productionTop].rightPartLength = -1;
        productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {"boolean"};
        productionTop++;

        cout << "----------------productions-----------------" << endl;
        for(int i = 0; i < productionTop; i++){
            cout << i << ". " << productions[i].leftPart << " -> ";
            for(int j = 0; j <= productions[i].rightPartLength; j++){
                cout << productions[i].rightPart[j];
            }
            cout << endl;
        }

        // 设定非终结符
        vtn[vtnTop++] = "S";
        vtn[vtnTop++] = "C";
        vtn[vtnTop++] = "W";
        vtn[vtnTop++] = "O";
        vtn[vtnTop++] = "U";
        vtn[vtnTop++] = "E";
        vtn[vtnTop++] = "T";

        // 设定终结符
        terminal[terminalTop++] = "id";
        terminal[terminalTop++] = "num";
        terminal[terminalTop++] = "==";
        terminal[terminalTop++] = ">";
        terminal[terminalTop++] = "<";
        terminal[terminalTop++] = "=";
        terminal[terminalTop++] = "+";
        terminal[terminalTop++] = "-";
        terminal[terminalTop++] = "*";
        terminal[terminalTop++] = "/";
        terminal[terminalTop++] = "if";
        terminal[terminalTop++] = "while";
        terminal[terminalTop++] = "int";
        terminal[terminalTop++] = "double";
        terminal[terminalTop++] = "boolean";
        terminal[terminalTop++] = ";";
        terminal[terminalTop++] = "{";
        terminal[terminalTop++] = "}";
        terminal[terminalTop++] = "(";
        terminal[terminalTop++] = ")";
        terminal[terminalTop++] = "#";

        // 生成first集
        initFIRST();
        printFIRST();

        // 生成follow集
        initFOLLOW();
        printFOLLOW();

        // 获取规范族
        getCanonicalCollection();

        // 初始化分析表
        initAnalysisTable();

        // 构造分析表
        developAnalysisTable();

        cout << "---------------analysis table------------------" << endl;
        printAnalysisTable();

        // 分析

        analysis(target, wordNum);

}



 
int main()
{
    char test[] = "double x = 5.1; int b = x + 5; int a = 0; while(a < 3){ int z = a + 1;}#"; // 识别正确的程序
    // char test[] = "/* 注释 */ int main() \n // hhh \n { int a = 123.60 + 23; if ( a == 1 ) { return;}#"; // 语法分析抛出error的程序 
    scan(test);
    printToken();
    grammaticalAnalysis(token, top); 
    return 0;
}




