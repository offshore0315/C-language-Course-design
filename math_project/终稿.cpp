//小学生运算系统
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include<windows.h>
#include<mmsystem.h>
#include<assert.h>
# pragma comment(lib,"WinMM.Lib")

 //函数声明;
char** creatFormula(char* symbol, int numb, int length);  //创造算式;
void menu(char* symbol, int* numb, int* length);          //打印菜单;
char* myItoa(int number);                               //整形转化为字符串;

typedef struct Node  //栈的节点;
{
    int data;
    double data_f;
    char data_c;
    struct Node* pNext;
}Node, * pNode;


typedef struct Stack   //自定义栈的声明
{
    pNode pTop;
    pNode pBottom;
}Stack, * pStack;

void ini_Stack(pStack pS);	//初始化;

//函数重载,因为重载所以第二个参数必须传入;
void push(pStack pS, int val);	//压栈;
void push(pStack pS, double val);
void push(pStack pS, char val);

//函数重载,因为重载所以第二个参数必须传入;
bool pop(pStack pS, double* val);   //出栈;
bool pop(pStack pS, int* val);
bool pop(pStack pS, char* val);

bool is_empty(pStack pS);		//是否为空;
void show_Stack(pStack pS);     //打印栈;

void convert(pStack pS, char* src, char* dest);	//转换为后缀表达式;
int judge(char symbol);		//判断优先级;
double complexAdd(pStack pS, char* src);		//计算;

//主函数;
int main() {
    char symbol[6];      //符号位;
    int numb = 0, length = 0, score = 0;    //符号个数,数字长度;
    double result, ans;          //真实结果和小学生给出的答案;

    memset(symbol, 0, sizeof(symbol));

    menu(symbol, &numb, &length);

    char** formula = creatFormula(symbol, numb, length);

    //先打一套起手式;
    Stack stack;
    ini_Stack(&stack);
    char dest[1024];    //这里src就是前面的formula;
    memset(dest, 0, sizeof(dest));

    for (int i = 0; i < 10; i++) {
        int flag = 1;
        memset(dest, 0, sizeof(dest));

        convert(&stack, formula[i], dest);
        result = complexAdd(&stack, dest);

        printf("======请看第%d题======\n", i + 1);
        printf("%s\n", formula[i]);

        for (int j = 0; j < 3; j++) {
            printf("请输入答案(保留两位小数点,整数写xxx.00):");
            scanf("%lf", &ans);
            if (result - ans < 0.01 && result - ans > -0.01) {
                flag = 0;
                if (j == 0)   printf("完全正确，你非常聪明！加10分\n\n");
                if (j == 1)   printf("答对了，你很棒！加8分\n\n");
                if (j == 2)   printf("答对了，祝贺你！加6分\n\n");
                score += 10 - j * 2;
                break;
            }
            printf("你答错了，继续努力！");
        }
        if (flag) {
            printf("非常遗憾你又答错了,请继续做下一题。本题的正确答案为%1f\n\n",result);
        }
    }

    system("cls");
    printf("你的得分是%d", score);

    return 0;
}


//函数定义;
void menu(char* symbol, int* numb, int* length) {
    int music;

    memset(symbol, 0, strlen(symbol) + 1);

    printf("-----小学生计算系统-----\n");
    printf("请选择运算符(1.+ 2.- 3.* 4./)\n"); //运算符请输入加减乘除，不要输入数字 
    scanf("%s", symbol);
    printf("请选择运算符个数:");
    scanf("%d", numb);
    printf("请选择数字位数:");
    scanf("%d", length);
    printf("请选择是否播放背景音乐：1.是 2.否");
    scanf("%d", &music);
    
    if (music == 1) {
        PlaySound(TEXT("纯音乐 - 遇见 (钢琴版)~2.wav"), NULL, SND_ASYNC | SND_LOOP);
        system("pause");
    }
    system("cls");
}

char* myItoa(int number) {
    int n = 0, len = 0;
    char* returnStr;

    while (number) {
        n = n * 10 + number % 10;
        number /= 10;
        len++;
    }

    returnStr = (char*)malloc(sizeof(char) * len + 1);
    memset(returnStr, 0, len + 1);

    for (int i = 0; i < len; i++) {
        returnStr[i] = n % 10 + '0';
        n /= 10;
    }

    return returnStr;
}

char** creatFormula(char* symbol, int numb, int length)
{
    int kinds = 0, figure = 0, basal;      //二维数组行数和列数,符号种类数,数字,基底;
    char** formula;

    kinds = strlen(symbol);

    formula = (char**)malloc(sizeof(char*) * 10);
    for (int j = 0; j < 10; j++) {
        int val = length * (numb + 1) + numb + 10;    //多留出两个位置;
        formula[j] = (char*)malloc(sizeof(char) * val);
        memset(formula[j], 0, sizeof(char) * val);
    }

    srand((time_t)time(NULL) * time(NULL));

    basal = pow(10, length - 1);

    for (int row = 0; row < 10; row++) {

        int col = 0;

        for (int i = 0; i < numb; i++) {
            figure = rand() % (basal * 9) + basal;
            char* str = myItoa(figure);
            strcpy(formula[row] + col, str);
            col += length;
            formula[row][col++] = symbol[rand() % kinds];
        }
        figure = rand() % (basal * 9) + basal;
        char* str = myItoa(figure);
        strcpy(formula[row] + col, str);
    }

    return formula;
}

void ini_Stack(pStack pS)
{
    pS->pBottom = (pNode)malloc(sizeof(Node));
    pS->pBottom->pNext = NULL;
    pS->pTop = pS->pBottom;
}

void push(pStack pS, int val)
{
    pNode pTemp = pS->pTop;
    pS->pTop = (pNode)malloc(sizeof(Node));
    pS->pTop->data = val;
    pS->pTop->pNext = pTemp;
}
void push(pStack pS, double val)
{
    pNode pTemp = pS->pTop;
    pS->pTop = (pNode)malloc(sizeof(Node));
    pS->pTop->data_f = val;
    pS->pTop->pNext = pTemp;
}

void push(pStack pS, char val)
{
    pNode pTemp = pS->pTop;
    pS->pTop = (pNode)malloc(sizeof(Node));
    pS->pTop->data_c = val;
    pS->pTop->pNext = pTemp;
}

bool pop(pStack pS, double* val)
{
    if (is_empty(pS))
    {
        return false;
    }
    pNode pTemp = pS->pTop;
    *val = pS->pTop->data_f;
    pS->pTop = pS->pTop->pNext;
    free(pTemp);
}
bool pop(pStack pS, int* val)
{
    if (is_empty(pS))
    {
        return false;
    }
    pNode pTemp = pS->pTop;
    *val = pS->pTop->data;
    pS->pTop = pS->pTop->pNext;
    free(pTemp);
}
bool pop(pStack pS, char* val)
{
    if (is_empty(pS))
    {
        return false;
    }
    pNode pTemp = pS->pTop;
    *val = pS->pTop->data_c;
    pS->pTop = pS->pTop->pNext;
    free(pTemp);
}

bool is_empty(pStack pS)
{
    if (pS->pTop == pS->pBottom)
    {
        return true;
    }
    return false;
}

void show_Stack(pStack pS)
{
    if (is_empty(pS))
    {
        printf("Stack is empty!\n");
        return;
    }
    pNode pTemp = pS->pTop;
    while (pTemp != pS->pBottom)
    {
        printf("%d\n", pTemp->data);
        pTemp = pTemp->pNext;
    }
}

//计算器;

int judge(char symbol)
{
    if (symbol == '+' || symbol == '-') return 1;
    return 2;
}

void convert(pStack pS, char* src, char* dest)
{
    int i = 0, j = 0;
    double val;
    char ch;
    while (src[i] != '\0')
    {
        if ((src[i] >= '0' && src[i] <= '9') || (src[i] >= 'a' && src[i] <= 'z'))
        {
            dest[j++] = src[i++];
            continue;
        }
        if (src[i] == '.') { dest[j++] = src[i++]; continue; }
        if (src[i] == '(')
        {
            push(pS, '('); i++;
            continue;
        }
        if (src[i] == ')')
        {
            while (pS->pTop->data_c != '(')
            {
                //dest[j++] = pS->pTop->data_f;
                pop(pS, &dest[j++]);
            }
            pop(pS, &ch);
            i++; continue;
        }
        dest[j++] = ' ';
        while (pS->pTop != pS->pBottom\
            && pS->pTop->data_c != '('\
            && judge(pS->pTop->data_c) >= judge(src[i]))
        {
            //dest[j++] = pS->pTop->data_f;
            pop(pS, &dest[j++]);
        }
        push(pS, src[i++]);
    }

    while (pS->pTop != pS->pBottom)
    {
        //dest[j++] = pS->pTop->data_f;
        pop(pS, &dest[j++]);
    }
}

double complexAdd(pStack pS, char* src)
{
    int i = 0, index = 0;
    double number = 0, result = 0, value1 = 0, value2 = 0;
    char temp[1024];
    memset(temp, 0, sizeof(temp));
    while (src[i] != 0)
    {
        if ((src[i] >= '0' && src[i] <= '9') || src[i] == '.')
        {
            temp[index] = src[i];
            index++; i++;
            continue;
        }
        if (src[i] == ' ')
        {
            if (temp[0] == '\0') { i++; continue; }
            number = atof(temp);
            push(pS, (double)number);
            memset(temp, 0, sizeof(temp));
            index = 0; i++;
            continue;
        }
        if (temp[0] != '\0')
        {
            number = atof(temp);
            push(pS, (double)number);
            memset(temp, 0, sizeof(temp));
            index = 0;
        }
        pop(pS, &value1); pop(pS, &value2);
        switch (src[i])
        {
        case '+':result = value2 + value1; break;
        case '-':result = value2 - value1; break;
        case '*':result = value2 * value1; break;
        case '/':
            if (value1 == 0) break;
            result = value2 / value1; break;
        }
        push(pS, result);
        i++;
    }
    pop(pS, &result);
    return result;
}

