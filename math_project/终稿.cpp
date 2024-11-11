//Сѧ������ϵͳ
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include<windows.h>
#include<mmsystem.h>
#include<assert.h>
# pragma comment(lib,"WinMM.Lib")

 //��������;
char** creatFormula(char* symbol, int numb, int length);  //������ʽ;
void menu(char* symbol, int* numb, int* length);          //��ӡ�˵�;
char* myItoa(int number);                               //����ת��Ϊ�ַ���;

typedef struct Node  //ջ�Ľڵ�;
{
    int data;
    double data_f;
    char data_c;
    struct Node* pNext;
}Node, * pNode;


typedef struct Stack   //�Զ���ջ������
{
    pNode pTop;
    pNode pBottom;
}Stack, * pStack;

void ini_Stack(pStack pS);	//��ʼ��;

//��������,��Ϊ�������Եڶ����������봫��;
void push(pStack pS, int val);	//ѹջ;
void push(pStack pS, double val);
void push(pStack pS, char val);

//��������,��Ϊ�������Եڶ����������봫��;
bool pop(pStack pS, double* val);   //��ջ;
bool pop(pStack pS, int* val);
bool pop(pStack pS, char* val);

bool is_empty(pStack pS);		//�Ƿ�Ϊ��;
void show_Stack(pStack pS);     //��ӡջ;

void convert(pStack pS, char* src, char* dest);	//ת��Ϊ��׺���ʽ;
int judge(char symbol);		//�ж����ȼ�;
double complexAdd(pStack pS, char* src);		//����;

//������;
int main() {
    char symbol[6];      //����λ;
    int numb = 0, length = 0, score = 0;    //���Ÿ���,���ֳ���;
    double result, ans;          //��ʵ�����Сѧ�������Ĵ�;

    memset(symbol, 0, sizeof(symbol));

    menu(symbol, &numb, &length);

    char** formula = creatFormula(symbol, numb, length);

    //�ȴ�һ������ʽ;
    Stack stack;
    ini_Stack(&stack);
    char dest[1024];    //����src����ǰ���formula;
    memset(dest, 0, sizeof(dest));

    for (int i = 0; i < 10; i++) {
        int flag = 1;
        memset(dest, 0, sizeof(dest));

        convert(&stack, formula[i], dest);
        result = complexAdd(&stack, dest);

        printf("======�뿴��%d��======\n", i + 1);
        printf("%s\n", formula[i]);

        for (int j = 0; j < 3; j++) {
            printf("�������(������λС����,����дxxx.00):");
            scanf("%lf", &ans);
            if (result - ans < 0.01 && result - ans > -0.01) {
                flag = 0;
                if (j == 0)   printf("��ȫ��ȷ����ǳ���������10��\n\n");
                if (j == 1)   printf("����ˣ���ܰ�����8��\n\n");
                if (j == 2)   printf("����ˣ�ף���㣡��6��\n\n");
                score += 10 - j * 2;
                break;
            }
            printf("�����ˣ�����Ŭ����");
        }
        if (flag) {
            printf("�ǳ��ź����ִ����,���������һ�⡣�������ȷ��Ϊ%1f\n\n",result);
        }
    }

    system("cls");
    printf("��ĵ÷���%d", score);

    return 0;
}


//��������;
void menu(char* symbol, int* numb, int* length) {
    int music;

    memset(symbol, 0, strlen(symbol) + 1);

    printf("-----Сѧ������ϵͳ-----\n");
    printf("��ѡ�������(1.+ 2.- 3.* 4./)\n"); //�����������Ӽ��˳�����Ҫ�������� 
    scanf("%s", symbol);
    printf("��ѡ�����������:");
    scanf("%d", numb);
    printf("��ѡ������λ��:");
    scanf("%d", length);
    printf("��ѡ���Ƿ񲥷ű������֣�1.�� 2.��");
    scanf("%d", &music);
    
    if (music == 1) {
        PlaySound(TEXT("������ - ���� (���ٰ�)~2.wav"), NULL, SND_ASYNC | SND_LOOP);
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
    int kinds = 0, figure = 0, basal;      //��ά��������������,����������,����,����;
    char** formula;

    kinds = strlen(symbol);

    formula = (char**)malloc(sizeof(char*) * 10);
    for (int j = 0; j < 10; j++) {
        int val = length * (numb + 1) + numb + 10;    //����������λ��;
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

//������;

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

