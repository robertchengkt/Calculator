/*
 * Guangzhou ZHIYUAN electronics Co.,LTD.
 * http://www.embedtools.com
 * All rights reserved
 *
 * @file    Poland.cpp
 * @brief   Poland Expression Class 波兰表达式的计算
 *
 * Change Logs:
 * Date         Author      Notes
 * 2012-08-10   Libing   Second Edtion
 */

#include "Poland.h"
#define OPERATORMOST         38                                         /**< 运算符个数                  */
#define CONSTVARIABLEMOST     2                                         /**< 常量个数                    */
#define COMMANDMOST          11                                         /**< 命令个数                    */

#include"BigDouble.h"
#include<string>
#include <iostream>
#include<vector>
#include <stack>
#include <fstream>
#include<cstring>
#include<cstdio>
#include<cstdlib>
#include<cmath>
#include <iomanip>
/**
 * @brief    常量字典
 * @details  存储计算中常用到的常数，用户可以直接使用
 */
const Variable VariableDic[CONSTVARIABLEMOST]= {
    {"pi",ConstVariable,3.141592653589793,BigDouble(3.141592653589793)},
    {"e" ,ConstVariable,2.718281828459046,BigDouble(2.718281828459046)},
};

/**
 * @brief    常用运算符字典
 * @details  存储数学表达式中常用到的运算符、函数、括号等以及类型和优先级
 */
const Operator OperatorDic[OPERATORMOST] = {

    {"&&",BinOperator  ,2},
    {"||",BinOperator  ,2},
    {"<=",BinOperator  ,3},
    {">=",BinOperator  ,3},
    {"==",BinOperator  ,4},
    {"!=",BinOperator  ,4},

    {"=" ,BinOperator  ,1},
    {"," ,BinOperator  ,0},
    {"<" ,BinOperator  ,3},
    {">" ,BinOperator  ,3},
    {"+" ,BinOperator  ,5},
    {"-" ,BinOperator  ,5},
    {"*" ,BinOperator  ,6},
    {"/" ,BinOperator  ,6},
    {"%" ,BinOperator  ,6},
    {"^" ,BinOperator  ,7},
    {"#" ,UnayOprator  ,7},                                             /*  负号，使用#代替，和减号区分  */
    {"~" ,UnayOprator  ,7},                                             /*  布尔取反                     */
    {"!" ,UnayOprator  ,7},                                             /*  阶乘                         */
    {"(" ,LeftParen    ,8},
    {")" ,RightParen   ,8},
    {";" ,EndExpression,9},
    /*  函数  */
    {"sin"  ,Function  ,7},
    {"cos"  ,Function  ,7},
    {"tan"  ,Function  ,7},
    {"cot"  ,Function  ,7},
    {"asin" ,Function  ,7},
    {"acos" ,Function  ,7},
    {"atan" ,Function  ,7},
    {"acot" ,Function  ,7},
    {"log10",Function  ,7},
    {"log"  ,Function  ,7},
    {"exp"  ,Function  ,7},
    {"pow"  ,Function  ,7},
    {"sqrt" ,Function  ,7},
    {"abs"  ,Function  ,7},
    {"ceil" ,Function  ,7},
    {"floor",Function  ,7},
};

 /**
 * @brief    命令字典
 * @details  存储命令系统中常用的命令，以及注解和用法示例
 */
const Command CommandDic[COMMANDMOST]={
    {"help"     ,  "Display help information"      , "help whos \nhelp"           },/**<  查看帮助       */
    {"whos"     ,  "Lists all the variables"       , "whos"                       },/**<  查看所有变量   */
    {"who"      ,  "List current variables."       , "who variableName"           },/**<  查看指定变量   */
    {"clc"      ,  "Clear command window."         , "clc"                        },/**<  清屏           */
    {"clear"    ,  "Removes all variables"         , "clear variableName \nclear" },/**<  清除变量       */
    {"operator" ,  "Define  user's Operator"       , "operator $ a$b 1/2*pi*a*b 4"},/**<  清屏           */
    {"copyright",  "Display copyright declaration" , "copyright"                  },/**<  版权声明       */
    {"history"  ,  "Display history expression."   , "history"                    },/**<  历史记录       */
    {"exit"     ,  "Exit from the console"         , "exit"                       },/**<  退出程序       */
    {"test"     ,  "Auto test the programm"        , "test test.txt"              },/**<  自动测试       */
    {"dic"      ,  "Display the dictionary"        , "dic"                        },/**<  打印字典       */

};
/**
 * @defgroup  Constructors_Poland
 * 用于Poland类的初始化
 */

/**
 * @defgroup  Destructors_Poland
 * 用于Poland类的销毁
 */

/**
 * @defgroup  MemberFunction_Poland
 * Poland类的成员函数，进行波兰表达式的计算
 */


/**
 * @ingroup  Constructors_Poland
 * @brief    Poland类的构造函数
 *
 * @code
 * Poland poland;
 * @endcode
 */
Poland::Poland(void)
{
    this->Initialize();
}


/**
 * @ingroup  Destructors_Poland
 * @brief    Poland类的析构函数
 *
 * @code
 * Poland poland;
 * ~poland;
 * @endcode
 */
Poland::~Poland(void)
{
}


/**
 * @ingroup  MemberFunction_Poland
 * @brief    Poland类的参数的初始化，在构造函数中自动调用
 *
 */
void Poland::Initialize(){
    /*
     *  初始化代码，为了计算方便，这里加入一个变量ans和ansB，用于存储计算表达式的结果，供下次计算使用
     */
    Variable VariableTemp = {"ans",VariableDouble,0.0,BigDouble(0.0)};
    VariableVector.push_back(VariableTemp);

    Variable VariableTemp2 = {"ansB",VariableBigDouble,0.0,BigDouble(1.23,1000)};
    VariableVector.push_back(VariableTemp2);
}

/**
 * @ingroup  MemberFunction_Poland
 * @brief    打印语法字典，包括运算符、常量、变量、自定义运算符
 *
 * @code
 * Poland poland;
 * Poland.PrintDictionary();
 * @endcode
 */
void Poland::PrintDictionary(){
    /*
     *  打印常量
     */
    cout<<"Variable Name\t\t"<<"Number"<<endl;
    for (int i = 0; i< CONSTVARIABLEMOST;i++)
    {
        cout<<VariableDic[i].cName<<"\t\t\t"<<VariableDic[i].dNumber<<endl;
        /*cout<<setw(35)<<VariableDic[i].cName<<VariableDic[i].dNumber<<endl;*/
    }
    /*
     *  打印用户定义变量
     */
    cout<<"\n\nUser Variable\t\t"<<"Number"<<endl;
    for (int i = 0; i< static_cast<int>(VariableVector.size());i++)
    {
        if (VariableVector.at(i).kind == VariableDouble)
        {
            cout<<VariableVector.at(i).cName<<"\t\t\t"<<VariableVector.at(i).dNumber<<endl;
        }else if (VariableVector.at(i).kind == VariableBigDouble)
        {
            cout<<VariableVector.at(i).cName<<"\t\t\t"
                <<VariableVector.at(i).bdNumber.getBigDoubleStr()<<endl;
        }
    }

    /*
     *  打印常用运算符
     */
    cout<<"\n\nOperator Name\t\t"<<"Priority"<<endl;
    for (int i = 0; i< OPERATORMOST;i++)
    {
        cout<<OperatorDic[i].cName<<"\t\t\t"<<OperatorDic[i].iPriority<<endl;
    }

    /*
     *  打印用户自定义运算符
     */
    cout<<"\n\nUser Operator\t\t"<<"Priority\t\t"<<"Express"<<endl;
    for (int i = 0; i< static_cast<int>(UserOperatorVector.size());i++)
    {
        cout<<UserOperatorVector.at(i).cOperator<<"\t\t\t"<<UserOperatorVector.at(i).iPriority<<"\t\t\t"
            <<UserOperatorVector.at(i).sExpress<<endl;
    }
}


/**
 * @ingroup  MemberFunction_Poland
 * @brief    打印用户自定义变量
 *
 * @code
 * Poland poland;
 * Poland.PrintVariable();
 * @endcode
 */
void Poland::PrintVariable(){
    cout<<setiosflags(ios_base::left);
    cout<<setw(35)<<"User Variable"<<"Number"<<endl;
    for (int i = 0; i< static_cast<int>(VariableVector.size());i++)
    {
        if (VariableVector.at(i).kind == VariableDouble)
        {
            cout<<setw(35)<<VariableVector.at(i).cName
                <<VariableVector.at(i).dNumber<<endl;
        }else if (VariableVector.at(i).kind == VariableBigDouble)
        {
            cout<<setw(35)<<VariableVector.at(i).cName
                <<VariableVector.at(i).bdNumber.getBigDoubleStr()<<endl;
        }
    }
    cout<<resetiosflags(ios_base::left);
}

/**
 * @ingroup  MemberFunction_Poland
 * @brief    打印常用运算符和用户自定义运算符
 *
 * @code
 * Poland poland;
 * Poland.PrintOperator();
 * @endcode
 */
void Poland::PrintOperator(){
    cout<<setiosflags(ios_base::left);
    cout<<setw(20)<<"Operator Name"<<"Priority"<<endl;
    for (int i = 0; i< OPERATORMOST;i++)
    {
        cout<<setw(20)<<OperatorDic[i].cName<<OperatorDic[i].iPriority<<endl;
    }

    cout<<setw(20)<<"User Operator"
        <<setw(20)<<"Priority"<<"Express"<<endl;
    for (int i = 0; i< static_cast<int>(UserOperatorVector.size());i++)
    {
        cout<<setw(20)<<UserOperatorVector.at(i).cOperator
            <<setw(20)<<UserOperatorVector.at(i).iPriority
            <<UserOperatorVector.at(i).sExpress<<endl;
    }
    cout<<resetiosflags(ios_base::left);
}


/**
 * @ingroup  MemberFunction_Poland
 * @brief    将string类型表达式拆分成数值、变量、常量、运算符、自定义运算符等基本元素，分别存储到相应的向量
 * vector中
 * @param[in] sExpress    用户输入的的string类型的数学表达式
 * @note   sExpress中不能有汉字；可以有空格；可以含有多条表达式，但是需要用分号";"分隔
 * @code
 * string sExpress;
 * Poland poland;
 * getline(cin,sExpress);
 * Poland.BreakExpress(sExpress);
 * @endcode
 */
void Poland::BreakExpress(string sExpress){

    /*
     *  先将存储下标和类型的向量清空
     */
    this->IndexVector.clear();
    this->KindTypeVector.clear();


    int iLength = sExpress.size();
    char *cExpress = new char[iLength +1];
    bool OperatorMatch = false;

    int LeftIndex  = 0;
    int RightIndex = iLength;

    for (int i = 0; i< iLength;i++)
    {
        cExpress[i] = sExpress.at(i);
    }
    cExpress[iLength] = '\0';

    this->MarkNegative(cExpress);                                       /*  对负号尽心标记              */

    for (int i = 0;i<iLength;i++)
    {
        OperatorMatch = false;

        /*
         * 进行运算符匹配时，为了加快速度，首先对第一个字符进行匹配，若匹配第一个，再进行串匹配
         */
        for (int j = 0 ; j< OPERATORMOST;j++)
        {
            bool FirstCharEqual = (cExpress[i] == OperatorDic[j].cName[0]);
            if (FirstCharEqual)
            {
                int iOperatorLength = strlen(OperatorDic[j].cName);
                bool IsEqual = this->strnmcmp(OperatorDic[j].cName,cExpress,i);
                if (IsEqual)
                {

                    if (OperatorDic[j].kind == Function && cExpress[i+ iOperatorLength] != '(')
                    {
                         /*
                         *  若函数名后面不是左括号，则认为不是函数，有可能是变量，如singal = 1，不能将sin
                         *  拆出作为函数处理
                         */
                        OperatorMatch = false;
                        break;
                    }else{
                        OperatorMatch = true;

                        RightIndex = i-1;
                        /*
                         *  提取运算符前面的操作数
                         */
                        this->AbstractOperand(cExpress,LeftIndex,RightIndex);


                        /*
                         *  将运算符存入vector
                         */
                        IndexVector.push_back(j);
                        KindTypeVector.push_back(OperatorDic[j].kind);

                        LeftIndex = i + iOperatorLength;
                        i = i+ iOperatorLength  -1;
                        break;
                    }
                }
            }
        }


        if (!OperatorMatch)
        {
            /*
             *  若不是常用运算符，则搜索用户自定义运算符
             */
            for (int k = 0; k<static_cast<int>(UserOperatorVector.size());k++)
            {
                if (UserOperatorVector.at(k).cOperator == cExpress[i])
                {
                    OperatorMatch = true;

                    RightIndex = i-1;

                    /*
                     *  提取运算符前面的操作数
                     */
                    this->AbstractOperand(cExpress,LeftIndex,RightIndex);
                    /*
                     *  将运算符存入vector
                     */
                    IndexVector.push_back(k);
                    KindTypeVector.push_back(UserOperatorVector.at(k).kind);

                    LeftIndex = i + 1;
                    break;
                }
            }
        }
    }

    /*
     *  提取表达式最后的一个操作数
     */
    this->AbstractOperand(cExpress,LeftIndex,iLength -1);
    /*
     *  允许一样输入多个表达式，表达式之间使用分号分隔，表达式末尾若没有分号，则自动追加
     */
    if (KindTypeVector.back() != EndExpression)
    {
        IndexVector.push_back(20);
        KindTypeVector.push_back(EndExpression);
    }

    delete [] cExpress;

    /*
     *   将打碎之后的表达式进行保存到历史记录
     */
    this->SaveBreakedExpress();
}


/**
 * @ingroup  MemberFunction_Poland
 * @brief    负号-和减号-在数学运算中具有不同的优先级，为了区分负号和减号，将表达式中的负号用一个不常用符号
 * "#"代替
 * @param[in] cExpress    char类型字符串构成的数学表达式
 * @code
 * char cExpress[] = "1*-1/-2";
 * Poland poland;
  * Poland.MarkNegative(cExpress);
 * @endcode
 */
void Poland::MarkNegative(char* cExpress){

     /*
      *  负号和减号的区分，加入'-'跟在一个运算符或者函数之后，或者左括号之后，或者新表达式的开头，则是负号
      *  否则，认为是减号
      */
    int iNextOperatorIndex = 0;
    if (cExpress[0] == '-')
    {
        cExpress[0] = '#';
    }

    int iLength = strlen(cExpress);
    bool OperatorMatch = false;

    for (int i = 0;i<iLength;i++)
    {

        OperatorMatch = false;

        for (int j = 0 ;
              j< OPERATORMOST
              &&( OperatorDic[j].kind == BinOperator
               || OperatorDic[j].kind == UnayOprator
               || OperatorDic[j].kind == LeftParen
               || OperatorDic[j].kind == EndExpression );
               j++)
        {
            bool FirstCharEqual = (cExpress[i] == OperatorDic[j].cName[0]);
            if (FirstCharEqual)
            {
                int iOperatorLength = strlen(OperatorDic[j].cName);
                bool IsEqual = this->strnmcmp(OperatorDic[j].cName,cExpress,i);
                if (IsEqual)
                {
                    iNextOperatorIndex = i + iOperatorLength;
                    while(cExpress[iNextOperatorIndex] == ' '){iNextOperatorIndex++;}
                    if (cExpress[iNextOperatorIndex] == '-')
                    {
                        cExpress[iNextOperatorIndex] = '#';
                        i = iNextOperatorIndex -1;
                        break;
                    }
                }
            }
        }

        if (!OperatorMatch)                                             /*  不是常用运算符               */
        {
            /* 查找用户自定义运算符  */
            for (int k = 0; k< static_cast<int>(UserOperatorVector.size());k++)
            {
                if (UserOperatorVector.at(k).cOperator == cExpress[i])
                {
                    if (cExpress[i+1] == '-')
                    {
                        cExpress[i+1] = '#';
                        break;
                    }
                }
            }
        }
    }
}


/**
 * @ingroup  MemberFunction_Poland
 * @brief    自定义字符串比较函数
 * @details  子串cSubStr与cStr的从下标StartIndex开始逐个比对，若完全匹配则返回true，否则范围false
 * @param[in] cSubStr    子字符串
 * @param[in] cStr       字符串
 * @param[in] StartIndex 字符串开始下标，从此处开始与子串比对
 * @retval  true  子串与字符串匹配
 * @retval  false 子串与字符串不匹配
 */
bool Poland::strnmcmp(const char * cSubStr,char * cStr,int StartIndex){

    int iLengthSubStr  = strlen(cSubStr);
    int iLengthStr     = strlen(cStr);
    bool IsEqual = true;
    if ((iLengthSubStr + StartIndex) <= iLengthStr)
    {
        for (int i = 0;i< iLengthSubStr;i++)
        {
            if (cSubStr[i] != cStr[i+StartIndex])
            {
                IsEqual = false;
                break;
            }
        }
        return IsEqual;
    }else {
        return false;
    }

}


/**
 * @ingroup  MemberFunction_Poland
 * @brief    自定义字符串拷贝函数
 * @details  从字符串cStr的下标iStartIndex到下标iStopIndex，将之间的字符串拷贝到cSubStr
 * @param[in] cSubStr    字符串拷贝目标字符串
 * @param[in] cStr       字符串拷贝源字符串
 * @param[in] iStartIndex  字符串拷贝开始下标
 * @param[in] iStopIndex   字符串拷贝结束下标
 */
void Poland::my_strnmcpy(char * cSubStr,char * cStr,int iStartIndex,int iStopIndex){
    int i;
    for (i = iStartIndex ; i<= iStopIndex ;i++)
    {
        cSubStr[i -iStartIndex] = cStr[i];
    }
    cSubStr[i-iStartIndex] = '\0';
    /*cSubStr[iStopIndex-iStartIndex + 1] = '\0';*/
}

/**
 * @ingroup  MemberFunction_Poland
 * @brief    将字符串格式的操作数转化为数值
 * @details  其中操作数可以是十六进制、十进制、八进制、二进制数据
 * @param[in] cOperandStr    从数学表达式中提取的操作数的字符串
 * @return   返回从字符串中提取的double类型的数值
 * @note   使用不同进制时，需要加进制标志，例如十六进制加前缀0x或者后缀h，八进制添加后缀o，二进制添加后缀b，
 * 十进制添加后缀d，若没有前缀后缀则默认是十进制数，前缀后缀字母使用大小写均可
 * @code
 * char cOperandStr[] = "ad.ffh";
 * Poland poland;
 * double dNumber = Poland.OperandStr2Number(cOperandStr);
 * @endcode
 */
double Poland::OperandStr2Number(char * cOperandStr){
    /*
     *  寻找小数点，根据每个数距离小数点的位置得到不同的权值，
     *  每个权值*该位的值并求和得到数值
     */
    int iLength = strlen(cOperandStr);
    int iDotIndex;
    double dNumber = 0.0;
    char cNumberType = this->NumberTypeCheck(cOperandStr);

    if (cNumberType == 'd')                                             /*  十进制，无前缀后缀           */
    {
        //string DecimalStr = string(cOperandStr);

        return atof(cOperandStr);

    }else if (cNumberType == 'D')                                      /*  十进制，后缀d                 */
    {
        char * cDecimalStr = new char [iLength + 1];
        double dTempNumber = 0.0;
        for (int j = 0; j<iLength -1;j++)
        {
            cDecimalStr[j] = cOperandStr[j];
        }
        cDecimalStr[iLength-1] = '\0';

        dTempNumber = atof(cDecimalStr);
        delete [] cDecimalStr;
        return dTempNumber;


    }else if (cNumberType == 'h')                                       /*  十六进制，前缀0x             */
    {
        iDotIndex = iLength;

        for (int j  = 2 ;j< iLength;j++)
        {
            if (cOperandStr[j] == '.')
            {
                iDotIndex = j;
            }
        }

        dNumber = 0.0;
        for (int j = 2;j< iLength;j++)
        {
            if (j > iDotIndex)
            {
                if (isdigit(cOperandStr[j]))
                {
                    dNumber += pow(16.0, iDotIndex - j)*(cOperandStr[j]-48);
                }else {
                    dNumber += pow(16.0, iDotIndex - j)*(tolower(cOperandStr[j])-'a' + 10);
                }
            }else if (j< iDotIndex)
            {
                if (isdigit(cOperandStr[j]))
                {
                    dNumber += pow(16.0, iDotIndex -j-1)*(cOperandStr[j]-48);
                }else{
                    dNumber += pow(16.0, iDotIndex -j-1)*(tolower(cOperandStr[j])-'a' + 10);
                }
            }
        }
        return dNumber;

    }else if (cNumberType == 'H')                                       /*  十六进制，后缀h              */
    {
        iDotIndex = iLength - 1;

        for (int j  = 0 ;j< iLength - 1;j++)
        {
            if (cOperandStr[j] == '.')
            {
                iDotIndex = j;
            }
        }

        dNumber = 0.0;
        for (int j = 0;j< iLength -1;j++)
        {
            if (j > iDotIndex)
            {
                if (isdigit(cOperandStr[j]))
                {
                    dNumber += pow(16.0, iDotIndex - j)*(cOperandStr[j]-48);
                }else {
                    dNumber += pow(16.0, iDotIndex - j)*(tolower(cOperandStr[j])-'a' + 10);
                }
            }else if (j< iDotIndex)
            {
                if (isdigit(cOperandStr[j]))
                {
                    dNumber += pow(16.0, iDotIndex -j-1)*(cOperandStr[j]-48);
                }else{
                    dNumber += pow(16.0, iDotIndex -j-1)*(tolower(cOperandStr[j])-'a' + 10);
                }
            }
        }
        return dNumber;

    }else if (cNumberType == 'o')                                       /*  八进制，后缀o               */
    {
        iDotIndex = iLength -1;
        for (int j  = 0 ;j< iLength -1;j++)
        {
            if (cOperandStr[j] == '.')
            {
                iDotIndex = j;
            }
        }

        dNumber = 0.0;
        for (int j = 0;j< iLength -1;j++)
        {
            if (j > iDotIndex)
            {
                dNumber += pow(8.0, iDotIndex - j)*(cOperandStr[j]-48);
            }else if (j< iDotIndex)
            {
                dNumber += pow(8.0, iDotIndex -j-1)*(cOperandStr[j]-48);
            }
        }
        return dNumber;

    }else if (cNumberType == 'b')                                       /*  二进制，后缀b                */
    {
        iDotIndex = iLength -1;
        for (int j  = 0 ;j< iLength -1;j++)
        {
            if (cOperandStr[j] == '.')
            {
                iDotIndex = j;
            }
        }

        dNumber = 0.0;
        for (int j = 0;j< iLength -1;j++)
        {
            if (j > iDotIndex)
            {
                dNumber += pow(2.0, iDotIndex - j)*(cOperandStr[j]-48);
            }else if (j< iDotIndex)
            {
                dNumber += pow(2.0, iDotIndex -j-1)*(cOperandStr[j]-48);
            }
        }
        return dNumber;
    }else if (cNumberType == 'n')                                       /*  不是数字                    */
    {
        cout<<"invalid number '"<<cOperandStr<<"'"<<endl;
        return -1.0;
    }
    return -1.0;
}


/**
 * @ingroup  MemberFunction_Poland
 * @brief    将字符串格式的操作数转化为数值
 * @details  其中操作数可以是十六进制、十进制、八进制、二进制数据
 * @param[in] sOperandStr    从数学表达式中提取的操作数的字符串
 * @return   返回从字符串中提取的double类型的数值
 * @note   使用不同进制时，需要加进制标志，例如十六进制加前缀0x或者后缀h，八进制添加后缀o，二进制添加后缀b，
 * 十进制添加后缀d，若没有前缀后缀则默认是十进制数，前缀后缀字母使用大小写均可
 * @code
 * string sOperandStr = string（"ad.ffh）";
 * Poland poland;
 * double dNumber = Poland.OperandStr2Number(sOperandStr);
 * @endcode
 */
double Poland::OperandStr2Number(string sOperandStr){
    int iLength = sOperandStr.size();
    char * cOperandStr = new char [iLength +1];
    double dTempNumber = 0.0;
    for (int i = 0;i< iLength;i++)
    {
        cOperandStr[i] = sOperandStr.at(i);
    }
    cOperandStr[iLength] = '\0';
    dTempNumber = this->OperandStr2Number(cOperandStr);
    delete [] cOperandStr;
    return  dTempNumber;


}



/**
 * @ingroup  MemberFunction_Poland
 * @brief    变量搜索函数
 * @param[in] cVariableStr    待搜索的变量名字符串
 * @return 返回搜索到的变量名的下标，不存在时则返回-1
 * @note
 * @code
 * @endcode
 */
int Poland::SearchVariable(char *cVariableStr){

    for (int i = 0; i< static_cast<int>(VariableVector.size());i++)
    {
        if (!strcmp(cVariableStr,VariableVector.at(i).cName))
        {
            return i;
        }
    }
    return -1;
}

/**
 * @ingroup  MemberFunction_Poland
 * @brief    常量搜索函数
 * @param[in] cVariableStr    待搜索的常量量名字符串
 * @return 返回搜索到的常量量名的下标，不存在时则返回-1
 * @note
 * @code
 * @endcode
 */
int Poland::SearchConstVariable(char * cVariableStr){

    for (int i =0 ;i< CONSTVARIABLEMOST;i++)
    {
        if (!strcmp(cVariableStr,VariableDic[i].cName))
        {
            return i;
        }
    }
    return -1;
}

/**
 * @ingroup  MemberFunction_Poland
 * @brief    打印被拆分成变量、函数、运算符等基本元素之后的表达式，用于查看
 *           成员函数BreakExpress()的结果
 *
 * @code
 * Poland poland;
 * Poland.PrintBreakedExpress();
 * @endcode
 */
void Poland::PrintBreakedExpress(){
    cout<<"Infix Expression: "<<endl;
    this->PrintExpress(this->IndexVector,this->KindTypeVector);
}


/**
 * @ingroup  MemberFunction_Poland
 * @brief    从字符串中提取操作数
 * @details  操作数可以是变量或者数字，字符串前后可以有空格
 * @param[in] cExpress    字符串形式的数学表达式
 * @param[in] LeftIndex   操作数字串在cExpress中的开始下标
 * @param[in] RightIndex  操作数字串在cExpress中的结束下标
 * @return 返回操作数的类型
 * @code
 * @endcode
 */



KindType Poland::AbstractOperand(char * cExpress, int LeftIndex,int RightIndex){

    int iLeftSpace = LeftIndex  - 1;  
    int iRightSpace = RightIndex + 1; 

    /*
     *  操作符两端可能有空格，将操作数两端的空格剔除
     */
    for (int i = LeftIndex ;i<= RightIndex ;i++)
    {
        if (cExpress[i] != ' ')
        {
            break;
        }
        iLeftSpace = i;
    }

    for (int i = RightIndex ;i>= LeftIndex ;i--)
    {
        if (cExpress[i] != ' ')
        {
            break;
        }
        iRightSpace = i;
    }
    int iLength = iRightSpace - iLeftSpace -1;


    /*
     * 操作数长度不超过308可以使用double类型表示，否则换用BigDouble来表示
     */

    if (iLength > 0 && iLength <= 308)
    {
        char *cOperandStr = new char[iLength+1];
        this->my_strnmcpy(cOperandStr,cExpress,iLeftSpace+1,iRightSpace-1);
        /*for (int i = iLeftSpace + 1 ; i<= iRightSpace-1 ;i++)
        {
            cOperandStr[i -iLeftSpace -1] = cExpress[i];
        }
        cOperandStr[iLength] = '\0';*/

        /*
         *  变量名合格检测，要求和c的变量名规范相同，数字、下划线、字母，首字符不能使数字
         */
        if(this->IsValidVariableName(cOperandStr)){

            int iIndex = -1;
            iIndex = this->SearchConstVariable(cOperandStr);

            /*
             *  若是常量，存储类型和下标即可
             */
            if (iIndex>= 0 && iIndex < CONSTVARIABLEMOST)
            {
                IndexVector.push_back(iIndex);
                KindTypeVector.push_back(ConstVariable);


                return ConstVariable;

            }else{
                iIndex = this->SearchVariable(cOperandStr);
                /*
                 *  自定义变量，并记下下标和类型
                 */
                if (iIndex >= 0 && iIndex < static_cast<int>(VariableVector.size()))
                {
                    IndexVector.push_back(iIndex);
                    KindTypeVector.push_back(VariableVector.at(iIndex).kind);

                    return VariableVector.at(iIndex).kind;
                }else{
                    /*
                     * 未定义变量，存储变量到VariableVector中，并记下下标和类型，
                     * 使用时只允许出现在等号的左侧，并且对变量名长度有限制，32个字符以内
                     */

                    if (strlen(cOperandStr) <= 32)
                    {
                        Variable VariableTemp;
                        strcpy(VariableTemp.cName,cOperandStr);
                        VariableTemp.kind = Undefine;
                        VariableTemp.dNumber = 0.0;
                        VariableTemp.bdNumber = BigDouble(0.0);

                        IndexVector.push_back(VariableVector.size());
                        KindTypeVector.push_back(Undefine);
                        VariableVector.push_back(VariableTemp);
                        return Undefine;
                    }else{
                        cout<<"Variable name must be less than 32 Byte"<<endl;
                        /*
                         *  TODO: 添加对变量超长时的错误处理
                         */
                    }

                }
            }

        }else if (this->NumberTypeCheck(cOperandStr) != 'n')
        {
            /*
             *  操作数不是变量，而是数字，将数字解析成double并存储到DoubleVector，记下下标和类型
             */
            double dOperand = this->OperandStr2Number(cOperandStr);

            IndexVector.push_back(DoubleVector.size());
            KindTypeVector.push_back(OperandDouble);
            DoubleVector.push_back(dOperand);

            delete [] cOperandStr;
            return OperandDouble;

        }
        else{
            delete [] cOperandStr;
            cout<<"Invalid Variable or Function '"<<cOperandStr<<"'"<<endl;
        }

    }else if (iLength > 308)
    {
        /*
         *  若操作数长度超过308，有可能是BigDouble大数值，提取大数值，并存储
         */

        char *cOperandStr = new char[iLength+1];
        this->my_strnmcpy(cOperandStr,cExpress,iLeftSpace+1,iRightSpace-1);

        if (isdigit(cOperandStr[0])  || cOperandStr[0] == '.')
        {
            BigDouble BigDoubleTemp = BigDouble(cOperandStr);

            IndexVector.push_back(BigDoubleVector.size());
            KindTypeVector.push_back(OperandBigDouble);
            BigDoubleVector.push_back(BigDoubleTemp);

            delete [] cOperandStr;
            return OperandBigDouble;
        }else{
            delete [] cOperandStr;
            cout<<"Undefine Variable or Function '"<<cOperandStr<<"'"<<endl;
            return Undefine;
        }

    }else {

    }
    return Undefine;
}


/**
 * @ingroup  MemberFunction_Poland
 * @brief    数值进制检测函数
 * @param[in] cOperandStr    从数学表达式中提取的字符串形式的数值
 * @retval  n 不是数值
 * @retval  h 十六进制 0x前缀
 * @retval  H 十六进制 h后缀
 * @retval  d 十进制 无前缀或后缀
 * @retval  D 十进制 d后缀
 * @retval  o 八进制 o后缀
 * @retval  b 二进制 b后缀
 * @note   前缀或者缀写作大写小写均支持
 * @code
 * char cOperandStr[] = "ad.ffh";
 * Poland poland;
 * char NumberType = Poland.NumberTypeCheck(cOperandStr);
 * @endcode
 */
char Poland::NumberTypeCheck(char * cOperandStr){

    int iLength = strlen(cOperandStr);
    int iDotCount = 0;

    /*
     *  进制判别
     */
    if((cOperandStr[0] == '0' && tolower(cOperandStr[1]) == 'x') ){
        for (int i = 2; i< iLength ;i++)
        {
            if (cOperandStr[i] == '.')
            {
                iDotCount++;
            }else if (!(isdigit(cOperandStr[i]) ||
                       (tolower(cOperandStr[i]) >= 97 &&
                        tolower(cOperandStr[i])<= 102)))
            {
                return 'n';
            }
        }
        if (iDotCount>1)
        {
            return 'n';
        }
        return 'h';

    }else if ( tolower(cOperandStr[iLength-1]) == 'h')
    {
        for (int i = 0; i< iLength-1 ;i++)
        {
            if (cOperandStr[i] == '.')
            {
                iDotCount++;
            }else if (!(isdigit(cOperandStr[i]) ||
                       (tolower(cOperandStr[i]) >= 'a' &&
                        tolower(cOperandStr[i])<= 'f')))
            {
                return 'n';
            }
        }
        if (iDotCount>1)
        {
            return 'n';
        }
        return 'H';
    }else if (tolower(cOperandStr[iLength-1]) == 'o')
    {
        for (int i = 0; i< iLength-1 ;i++)
        {
            if (cOperandStr[i] == '.')
            {
                iDotCount++;
            }else if (!(cOperandStr[i] >= '0' && cOperandStr[i]<= '7'))
            {
                return 'n';
            }
        }
        if (iDotCount>1)
        {
            return 'n';
        }
        return 'o';
    }else if (tolower(cOperandStr[iLength-1]) == 'b')
    {
        for (int i = 0; i< iLength-1 ;i++)
        {
            if (cOperandStr[i] == '.')
            {
                iDotCount++;
            }else if (!(cOperandStr[i] == '0' || cOperandStr[i] == '1'))
            {
                return 'n';
            }
        }
        if (iDotCount>1)
        {
            return 'n';
        }
        return 'b';
    }else if (tolower(cOperandStr[iLength-1]) == 'd')
    {
        for (int i = 0; i< iLength-1 ;i++)
        {
            if (cOperandStr[i] == '.')
            {
                iDotCount++;
            }else if (!isdigit(cOperandStr[i]))
            {
                return 'n';
            }
        }
        if (iDotCount>1)
        {
            return 'n';
        }
        return 'D';
    }else{
        for (int i = 0; i< iLength ;i++)
        {
            if (cOperandStr[i] == '.')
            {
                iDotCount++;
            }else if (!isdigit(cOperandStr[i]))
            {
                return 'n';
            }
        }
        if (iDotCount>1)
        {
            return 'n';
        }
        return 'd';
    }
    return 'n';
}

/**
 * @ingroup  MemberFunction_Poland
 * @brief    数值进制检测函数
 * @param[in] sOperandStr    从数学表达式中提取的字符串形式的数值
 * @retval  n 不是数值
 * @retval  h 十六进制 0x前缀
 * @retval  H 十六进制 h后缀
 * @retval  d 十进制 无前缀或后缀
 * @retval  D 十进制 d后缀
 * @retval  o 八进制 o后缀
 * @retval  b 二进制 b后缀
 * @note   前缀或者缀写作大写小写均支持
 * @code
 * @endcode
 */
char Poland::NumberTypeCheck    (string sOperandStr){
    char cNumberType;
    int iLength = sOperandStr.size();
    char * cOperandStr = new char [iLength +1];
    for (int i = 0;i< iLength;i++)
    {
        cOperandStr[i] = sOperandStr.at(i);
    }
    cOperandStr[iLength] = '\0';

    cNumberType = this->NumberTypeCheck(cOperandStr);

    delete [] cOperandStr;
    return cNumberType;

}

/**
 * @ingroup  MemberFunction_Poland
 * @brief    变量名合法检测函数
 * @details  合法的变量名应由数字、字母、下划线构成，并要求首字母不能使数字
 * @param[in] cOperandStr    从数学表达式中提取的字符串形式的变量名
 * @retval  false 不是合法变量名
 * @retval  true  合法变量名
 * @code
 * char cOperandStr[] = "_validName1213";
 * Poland poland;
 * bool valid  = Poland.IsValidVariableName(cOperandStr);
 * @endcode
 */
bool Poland::IsValidVariableName(char* cOperandStr){

    /*
     *  变量名中只能出现数字、字母、下划线，并且首字符不能使数字
     */
    bool isValid = true;
    int  iLength = strlen(cOperandStr);
    char ch = 0;
    int i;
    for (i = 0 ; i< iLength;i++)
    {
        ch = cOperandStr[i];
        if(!(isdigit(ch) || isalpha(ch) || ch == '_'))
        {
            isValid = false;
        }
    }
    if (isdigit(cOperandStr[0]))
    {
        isValid = false;
    }
    return isValid;
}


/**
 * @ingroup  MemberFunction_Poland
 * @brief    自动测试函数
 * @details  使用test.txt中存储的大量表达式，进行测试，提高测试效率
 * @code
 * @endcode
 */

void Poland::Test(string sFileName){
     /*
     *  自动测试程序
     *  使用test.txt中存储的大量表达式，进行测试，提高测试效率
     *  TODO： 添加对文件名的支持，使得用户可以编写脚本，自动运行
     */
    int iLength = sFileName.size();
    char * FileName = new char [iLength + 1];
    for (int i = 0; i< iLength;i++)
    {
        FileName[i] = sFileName.at(i);
    }
    FileName[iLength] = '\0';
    ifstream TestFile;
    TestFile.open(FileName);
    if (!TestFile.is_open())
    {
        cout<<"\""<<FileName<<"\""<<"cannot open"<<endl;
        return;
    }
    Variable AnsVariable = {"ans",VariableDouble,0.0,BigDouble(0.0)};
    string sExpress;

    getline(TestFile,sExpress);
    while (TestFile.good())
    {
        if (sExpress.empty())
        {
            continue;
        }
        cout<<">>";
        cout<<sExpress<<endl;
        int CommandType = this->CommandType(sExpress);
        if (CommandType >= 0)
        {
            this->ExecuteCommand(sExpress,CommandType);
        }else {
            /*poland.PrintDictionary();*/
           this->BreakExpress(sExpress);
            /*  poland.PrintBreakedExpress();*/
            this->Infix2Postfix();
            /* poland.PrintPostfixExpress();*/
           this->Postfix2Number(AnsVariable);

        }
        getline(TestFile,sExpress);
    }

    TestFile.close();
}

/**
 * @ingroup  MemberFunction_Poland
 * @brief    将中缀表达式转化为后缀表达式
 * @note     表示转换并没有改变表达式基本元素的存储位置，而是改变了索引。索引由vector<int>IndexVector
 * 和vector<KindType>KindTypeVector组成，根据下标和类型可以获取该基本元素。改变之后的索引存放在
 * vector<int>PostfixIndexVector 和 vector<KindType>PostfixKindTypeVector
 */
void Poland::Infix2Postfix(){
    /*
     *  将中缀式转化为后缀式，改变两个vector中标记的顺序
     *  遇到操作数则放到后缀表达式的vector中，遇到运算符，将在堆栈中的运算符出栈，放入后缀表达式的vector，
     *  直到遇到优先级大于等于自己的运算符时停止，并将自己入栈
     *  特殊情况： =是从右往左计算，只能将优先级高于自己的运算符出栈
     *  单目运算符和函数也是只能将高于自己的运算符出栈
     */
    this->PostfixIndexVector.clear();
    this->PostfixKindTypeVector.clear();

    KindType kind;
    int iIndex;
    int iPriority = -1;
    int iTempPriority = 100;
    int iTempIndex;
    KindType TempKindType;

    stack<int>IndexStack;
    stack<KindType>KindTypeStack;

    for (int i = 0; i< static_cast<int>(KindTypeVector.size());i++)
    {
        kind   = KindTypeVector.at(i);
        iIndex = IndexVector.at(i);

        if (  kind == OperandDouble    ||
              kind == OperandBigDouble ||
              kind == ConstVariable    ||
              kind == VariableDouble   ||
              kind == VariableBigDouble||
              kind == Undefine)
        {
            this->PostfixIndexVector.push_back(iIndex);
            this->PostfixKindTypeVector.push_back(kind);

        }else if (kind == LeftParen)                                    /*  左括号，入栈                 */
        {

            IndexStack.push(iIndex);
            KindTypeStack.push(kind);
        }else if (kind ==  RightParen)
        {
            /*
             *  右括号，将匹配左括号之前的元素全部出栈
             */
            while (!KindTypeStack.empty())
            {
                if (KindTypeStack.top() != LeftParen)
                {
                    PostfixKindTypeVector.push_back(KindTypeStack.top());
                    PostfixIndexVector.push_back(IndexStack.top());

                    KindTypeStack.pop();
                    IndexStack.pop();
                }else{
                    break;
                }
            }
            if (!KindTypeStack.empty())
            {
                if (KindTypeStack.top() == LeftParen)
                {
                    KindTypeStack.pop();
                    IndexStack.pop();
                }
            }else{
               /*
                *  error
                */
                cout<<"Unmatched parentheses"<<endl;
            }

        }else if(kind == Function ||
                 kind == BinOperator||
                 kind == UnayOprator||
                 kind == UserDefineBinOprator||
                 kind == UserDefineUnayOprator)
        {
            /*
            *  将在堆栈中的运算符出栈，放入后缀表达式的vector,直到遇到优先级大于等于自己的运算符时停止，
            *  并将自己入栈
            *  特殊情况： =是从右往左计算，只能将优先级高于自己的运算符出栈
            *  单目运算符和函数也是只能将高于自己的运算符出栈
            */
            if (   kind == Function ||
                   kind == BinOperator||
                   kind == UnayOprator)
            {
                iPriority = OperatorDic[iIndex].iPriority;
            }else if (kind == UserDefineBinOprator
                    ||kind == UserDefineUnayOprator)
            {
                iPriority = UserOperatorVector.at(iIndex).iPriority;
            }
            while (!KindTypeStack.empty())
            {
                TempKindType = KindTypeStack.top();
                iTempIndex   = IndexStack.top();

                if (TempKindType == LeftParen)
                {
                    break;
                }else if (TempKindType == Function ||
                          TempKindType == BinOperator ||
                          TempKindType == UnayOprator)
                {
                    iTempPriority = OperatorDic[iTempIndex].iPriority;
                }else if (TempKindType == UserDefineBinOprator ||
                          TempKindType == UserDefineUnayOprator)
                {
                    iTempPriority = UserOperatorVector.at(iTempIndex).iPriority;
                }

                /*
                 *  单目运算符和函数放在第七级，当小于第七级时，只要遇到小于或者等于的运算符就会使前面的出栈，
                 *  但是当是第七级的单目运算符时，必须是严格的小于才能使前面的出栈
                 */
                if (iTempPriority == 1 && iPriority == 1)
                {
                    break;
                }else if (iTempPriority < iPriority)
                {
                    break;
                }else if (iPriority < 7 && iTempPriority >= iPriority)
                {
                    PostfixKindTypeVector.push_back(TempKindType);
                    PostfixIndexVector.push_back(iTempIndex);

                    KindTypeStack.pop();
                    IndexStack.pop();
                }else if (iPriority == 7 && iTempPriority > iPriority)
                {
                    PostfixKindTypeVector.push_back(TempKindType);
                    PostfixIndexVector.push_back(iTempIndex);

                    KindTypeStack.pop();
                    IndexStack.pop();
                }else if (iPriority == 7 && iTempPriority == iPriority)
                {
                    break;
                }
            }

            KindTypeStack.push(kind);
            IndexStack.push(iIndex);
        }else if (kind == EndExpression)                                /*  表达式结尾符，清空堆栈       */
        {
            while(!KindTypeStack.empty()){
                PostfixKindTypeVector.push_back(KindTypeStack.top());
                PostfixIndexVector.push_back(IndexStack.top());

                KindTypeStack.pop();
                IndexStack.pop();
            }
            if (!PostfixKindTypeVector.empty())
            {
                if (PostfixKindTypeVector.back() != EndExpression)
                {
                    PostfixKindTypeVector.push_back(kind);
                    PostfixIndexVector.push_back(iIndex);
                }
            }
        }
        else {
            cerr<<"error"<<endl;
        }
    }
}


/**
 * @ingroup  MemberFunction_Poland
 * @brief    打印被拆分成变量、函数、运算符等基本元素，并由中缀式转化为后缀式
 * 的表达式，用于查看成员函数Infix2Postfix()的结果
 *
 * @code
 * Poland poland;
 * Poland.PrintPostfixExpress();
 * @endcode
 */
void Poland::PrintPostfixExpress(){
    cout<<"Postfix Expression: "<<endl;
    this->PrintExpress(this->PostfixIndexVector,this->PostfixKindTypeVector);
}



/**
 * @ingroup  MemberFunction_Poland
 * @brief    保存拆分之后的数学表达式
 * @note
 * @code
 * @endcode
 */
void Poland::SaveBreakedExpress(){
    /*
     *  将拆分成基本元素的表达式存入到历史记录中，每个表达式使用表达式结尾符分号来分隔
     */
    KindType kind;
    int iIndex;
    for (int i = 0; i< static_cast<int>(KindTypeVector.size());i++)
    {
        iIndex = IndexVector.at(i);
        kind   = KindTypeVector.at(i);

        if (kind == EndExpression)
        {
            if (!HistoryKindTypeVector.empty())
            {
                if (HistoryKindTypeVector.back() != EndExpression)
                {
                    this->HistoryKindTypeVector.push_back(kind);
                    this->HistoryIndexVector.push_back(iIndex);
                }

            }
        }else {
            this->HistoryKindTypeVector.push_back(kind);
            this->HistoryIndexVector.push_back(iIndex);
        }

    }
}

/**
 * @ingroup  MemberFunction_Poland
 * @brief    打印用户输入表达式的历史记录
 *
 * @code
 * Poland poland;
 * Poland.PrintHistoryExpress();
 * @endcode
 */
void Poland::PrintHistoryExpress(){
    cout<<"History Expression :"<<endl;
    this->PrintExpress(this->HistoryIndexVector, this->HistoryKindTypeVector);
}


/**
 * @ingroup  MemberFunction_Poland
 * @brief    打印存储在数组Array或向量vector中的表达式
 * @param[in] IndexVector    表达式基本元素在数组或者向量中的下标
 * @param[in] KindTypeVector 表达式基本元素元素的类型
 * @note   已知某基本元素的类型和下标，可以到相应的数组或向量中取出该元素
 * @code
 * Poland poland;
 * Poland.PrintExpress(this->IndexVector ,this->KindTypeVector);
 * Poland.PrintExpress(this->PostfixIndexVector,this->PostfixKindTypeVector);
 * @endcode
 */
void Poland::PrintExpress(vector<int> & IndexVector ,vector<KindType>& KindTypeVector){
    KindType kind ;
    int iIndex;
    for (int i = 0 ; i< static_cast<int>(KindTypeVector.size());i++)
    {
        kind = KindTypeVector.at(i);
        iIndex = IndexVector.at(i);
        if (kind == OperandDouble)
        {
            cout<<DoubleVector.at(iIndex)<<" ";
        }else if (kind == OperandBigDouble)
        {
            cout<<BigDoubleVector.at(iIndex).getBigDoubleStr()<<" ";
        }else if (kind == ConstVariable)
        {
            cout<<VariableDic[iIndex].cName<<" "/*<<VariableDic[iIndex].dNumber<<" "*/;
        }else if (kind == VariableDouble)
        {
            cout<<VariableVector.at(iIndex).cName<<" "/*<<VariableVector.at(iIndex).dNumber<<" "*/;
        }else if (kind == VariableBigDouble)
        {
            cout<<VariableVector.at(iIndex).cName<<" "
                /*<<VariableVector.at(iIndex).bdNumber.getBigDoubleStr()<<" "*/;
        }else if (kind == Undefine)
        {
            cout<<VariableVector.at(iIndex).cName<<" ";
        }else if (kind == UnayOprator || kind == BinOperator)
        {
            cout<<OperatorDic[iIndex].cName<<" ";

        }else if (kind == UserDefineBinOprator || kind == UserDefineUnayOprator)
        {
            cout<<UserOperatorVector.at(iIndex).cOperator<<" "
                /*<<UserOperatorVector.at(iIndex).sExpress<<" "*/;
        }else if (kind == Function || kind ==  LeftParen || kind == RightParen)
        {
            cout<<OperatorDic[iIndex].cName<<" ";
        }else if (kind == EndExpression)
        {
            cout<<";"<<endl;
        }

    }

}

/**
 * @ingroup  MemberFunction_Poland
 * @brief    后缀表达式求值函数
 * @param[in] PostfixIndexVector    表达式基本元素在数组或者向量中的下标
 * @param[in] PostfixKindTypeVector 表达式基本元素的类型
 * @param[out] AnsVariable  将运算结果传出
 * @retval  false 求值成功
 * @retval  true  求值失败
 * @note    求值结果不返回，如果是赋值语句就把结果放到所赋值的变量，否则就把值放到变量名为"ans"的变量中
 * @code
 * @endcode
 */
bool Poland::Postfix2Number(vector<int> & PostfixIndexVector ,
                            vector<KindType>& PostfixKindTypeVector,
                            Variable& AnsVariable
                            )
{


    int iIndex;
    KindType kind;
    int ExpressType = 0;

    /*
     *  处理每行多条表达式的情况，
     *  遇到表达式结尾符分号，认为计算完毕，开始下一个表达式的计算
     */
    while (!PostfixIndexVector.empty())
    {
            /*
             *  由于赋值和计算差别较大，将赋值表达式和计算表达式分开计算
             *  检查表达式，返回赋值的层数 如a = b =2 则返回2
             */
            ExpressType = this->ExpressTypeCheck(this->IndexVector,this->KindTypeVector);
            if (ExpressType == -1)
            {
                cout<<"Some thing error in assign expression"<<endl;
                return false;
            }else if (ExpressType == 0)                                 /*  求值表达式，不含赋值语句     */
            {
                bool success = this->EvaluateExpress(PostfixIndexVector,
                                                     PostfixKindTypeVector,
                                                     ExpressType,
                                                     AnsVariable);


                if (success)
                {
                    if (AnsVariable.kind == OperandDouble)
                    {
                        cout<<"ans = "<<endl<<"      "<<AnsVariable.dNumber<<endl;
                        AnsVariable.kind = VariableDouble;
                        this->SaveVariable(AnsVariable,this->VariableVector);
                    }else if (AnsVariable.kind == OperandBigDouble)
                    {
                        cout<<"ans = "<<endl<<"      "<<AnsVariable.bdNumber.getBigDoubleStr()<<endl;
                        AnsVariable.kind = VariableBigDouble;
                        this->SaveVariable(AnsVariable,this->VariableVector);
                    }

                }else{
                   /*
                   *  something Error
                   */
                }
            }else {
                bool success = this->EvaluateExpress(PostfixIndexVector,PostfixKindTypeVector,ExpressType,AnsVariable);
                if (success)
                {
                    if (AnsVariable.kind == OperandDouble)
                    {
                        for (int j= 0; j< ExpressType;j++)
                        {
                            kind = this->KindTypeVector.at(2*j);
                            iIndex = this->IndexVector.at(2*j);
                            if (kind == ConstVariable)
                            {
                                cerr<<"const variable '"<<VariableDic[iIndex].cName<<"' cannot be changed"<<endl;
                            }else if (kind == Undefine||
                                      kind == VariableDouble||
                                      kind == VariableBigDouble)
                            {
                                cout<<VariableVector.at(iIndex).cName<<" = "<<endl<<"      "<<AnsVariable.dNumber<<endl;
                                VariableVector.at(iIndex).kind = VariableDouble;
                                VariableVector.at(iIndex).dNumber = AnsVariable.dNumber;
                            }
                        }
                    }else if (AnsVariable.kind == OperandBigDouble)
                    {
                        for (int j= 0; j< ExpressType;j++)
                        {
                            kind = this->KindTypeVector.at(2*j);
                            iIndex = this->IndexVector.at(2*j);
                            if (kind == ConstVariable)
                            {
                                cerr<<"const variable '"<<VariableDic[iIndex].cName
                                    <<"' cannot be changed"<<endl;
                            }else if (kind == Undefine||
                                      kind == VariableDouble||
                                      kind == VariableBigDouble)
                            {
                                cout<<VariableVector.at(iIndex).cName<<" = "<<endl
                                    <<"      "<<AnsVariable.bdNumber.getBigDoubleStr()<<endl;
                                VariableVector.at(iIndex).kind = VariableBigDouble;
                                VariableVector.at(iIndex).bdNumber = AnsVariable.bdNumber;
                            }
                        }
                    }

                }else{
                  /*
                   *  something Error
                   */
                }
            }
            while (!PostfixIndexVector.empty())
            {
                kind = PostfixKindTypeVector.front();
                PostfixKindTypeVector.erase(PostfixKindTypeVector.begin());
                PostfixIndexVector.erase(PostfixIndexVector.begin());
                if (kind == EndExpression)
                {
                    break;
                }
            }
            while (!KindTypeVector.empty())
            {
                kind = KindTypeVector.front();
                KindTypeVector.erase(KindTypeVector.begin());
                IndexVector.erase(IndexVector.begin());
                if (kind == EndExpression)
                {
                    break;
                }
            }
    }

    return true;
}

/**
 * @ingroup  MemberFunction_Poland
 * @brief    后缀表达式求值函数
 * @param[out] AnsVariable  将运算结果传出
 * @retval  false 求值成功
 * @retval  true  求值失败
 * @note    求值结果不返回，如果是赋值语句就把结果放到所赋值的变量，否则就把值放到变量名为"ans"的变量中
 *
 * @code
 * @endcode
 */
bool Poland::Postfix2Number(Variable& AnsVariable){

    return this->Postfix2Number(PostfixIndexVector ,PostfixKindTypeVector,AnsVariable);

}


/**
 * @ingroup  MemberFunction_Poland
 * @brief    表达式类型判定
 * @details  判断表达式是赋值语句或求值语句，若是赋值语句，则返回赋值的层数
 * @param[in] IndexVector    表达式基本元素在数组或者向量中的下标
 * @param[in] KindTypeVector 表达式基本元素的类型
 * @return   返回-1表示是求值语句，大于等于0则表示赋值的层数，如 "a =  b = 2" 则返回2
 * @note
 * @code
 * @endcode
 */
int Poland::ExpressTypeCheck(vector<int> & IndexVector ,vector<KindType>& KindTypeVector){
    int iIndex;
    KindType kind;
    int iPriorIndex;
    KindType PriorKind;
    int ExpressType = 0;                                                /*  赋值语句的层数，-1表示错误   */
    int iEqualSignIndex = -1;
    for (int i = 0 ; i< static_cast<int>(KindTypeVector.size());i++)
    {
        iIndex = IndexVector.at(i);
        kind   =KindTypeVector.at(i);

        if (kind == BinOperator)
        {
           if (!strcmp("=",OperatorDic[iIndex].cName))
           {
               /*
                *  统计等号的个数，针对多重赋值语句
                */
               iEqualSignIndex = i;

               if (i == 0)
               {
                   return -1;
               }else{
                   iPriorIndex = IndexVector.at(i-1);
                   PriorKind   = KindTypeVector.at(i-1);
                   if (PriorKind == VariableDouble
                    || PriorKind == VariableBigDouble
                    || PriorKind == ConstVariable
                    || PriorKind == Undefine)
                   {
                       ExpressType++;
                   }else{
                       return -1;
                   }
               }
           }
        }else if (kind == EndExpression)
        {
            if (iEqualSignIndex == 2*ExpressType-1)
            {
                return ExpressType;
            }else{
                return -1;
            }
            break;
        }
    }
    return -1;
}

 /**
 * @ingroup  MemberFunction_Poland
 * @brief    求值表达式求值函数
 * @param[in] PostfixIndexVector     表达式基本元素在数组或者向量中的下标
 * @param[in] PostfixKindTypeVector  表达式基本元素的类型
 * @param[in] iStartIndex            从PostfixIndexVector和PostfixKindTypeVector的取元素的开始下标，添加开
 * 始下标是为了适应每行多个表达式的情况
 * @param[out] AnsVariable 引用调用的方式，将结果存入AnsVariable传出
 * @retval  false 求值失败
 * @retval  true  求值成功
 * @note
 * @code
 * @endcode
 */
bool Poland::EvaluateExpress(vector<int> &PostfixIndexVector,
                        vector<KindType> &PostfixKindTypeVector,
                                    int iStartIndex,
                               Variable & AnsVariable)
{

    bool BigDoubleExist = false;
    int CalSuccess = 1;
    for (int i = iStartIndex; i< static_cast<int>(PostfixKindTypeVector.size());i++)
    {
        if (PostfixKindTypeVector.at(i) == OperandBigDouble
          ||PostfixKindTypeVector.at(i) == VariableBigDouble)
        {
            BigDoubleExist = true;
            break;
        }
    }
    if (!BigDoubleExist)                                                /*  不存在大数值，使用double计算 */
    {
        CalSuccess=EvaluateExpressDouble(PostfixIndexVector,PostfixKindTypeVector,iStartIndex,AnsVariable); 
        /*return EvaluateExpressDouble(PostfixIndexVector,PostfixKindTypeVector,iStartIndex,AnsVariable);*/
        if (CalSuccess == 1)
        {
            return true;
        }else if (CalSuccess == -1)
        {
            return this->EvaluateExpressBigDouble(PostfixIndexVector,
                PostfixKindTypeVector,
                iStartIndex,
                AnsVariable);
        }else if (CalSuccess == 0)
        {
            return false;
        }
    }else{                                                              /*  存在BigDouble,大数值计算     */

        return this->EvaluateExpressBigDouble(PostfixIndexVector,
                                              PostfixKindTypeVector,
                                              iStartIndex,
                                              AnsVariable);

    }
    return false;
}

 /**
 * @ingroup  MemberFunction_Poland
 * @brief    求值表达式求值函数，对应不存在大数值的情况
 * @param[in] PostfixIndexVector     表达式基本元素在数组或者向量中的下标
 * @param[in] PostfixKindTypeVector  表达式基本元素的类型
 * @param[in] iStartIndex            从PostfixIndexVector和PostfixKindTypeVector的取元素的开始下标，添加开
 * 始下标是为了适应每行多个表达式的情况
 * @param[out] AnsVariable 引用调用的方式，将结果存入AnsVariable传出
 * @retval  false 求值失败
 * @retval  true  求值成功
 * @note
 * @code
 * @endcode
 */

int Poland::EvaluateExpressDouble(vector<int> &PostfixIndexVector,
                              vector<KindType> &PostfixKindTypeVector,
                                          int iStartIndex,
                                     Variable & AnsVariable)
{

     /*
     *  遇到操作数，则存储到堆栈中
     *  遇到运算符或者函数，将堆栈中的操作数取出，运算，再将运算结果存入堆栈
     *  遇到表达式结尾符，则计算结束
     */
    stack<double>AnsDoubleStack;
    KindType kind;
    int iIndex;
    double LeftOperand  = 0.0;
    double RightOperand = 0.0;
    char cName[6];
    double dNumber = 0.0;
    bool bdOverFlow = true;

    for (int i = iStartIndex; i< static_cast<int>(PostfixKindTypeVector.size());i++)
    {
        kind   = PostfixKindTypeVector.at(i);
        iIndex = PostfixIndexVector.at(i);

        if (kind == OperandDouble)
        {
            AnsDoubleStack.push(DoubleVector.at(iIndex));
        }else if (kind == ConstVariable)
        {
            AnsDoubleStack.push(VariableDic[iIndex].dNumber);
        }else if (kind == VariableDouble)
        {
            AnsDoubleStack.push(VariableVector.at(iIndex).dNumber);
        }else if (kind == Undefine)
        {
            cout<<"Undefine Variable or Function '"<<VariableVector.at(iIndex).cName<<"'"<<endl;
           /* return false;*/
            return 0;
        }else if (kind == BinOperator)
        {
            strcpy(cName,OperatorDic[iIndex].cName);
            if (!strcmp(cName,"="))                                     /*  =赋值 与普通二元运算符不同   */
            {
                if (AnsDoubleStack.size() == 1)
                {

                    AnsVariable.dNumber = AnsDoubleStack.top();
                    AnsVariable.kind = OperandDouble;
                    /*return true;*/
                    return 1;
                }else{
                    /*return false;*/
                    return 0;
                }
            }else if (!strcmp(cName,","))                               /*  逗号不用处理                 */
            {

            }else {
                if (AnsDoubleStack.size() >= 2)                         /*  从堆栈中取出两个操作数       */
                {
                    RightOperand = AnsDoubleStack.top();
                    AnsDoubleStack.pop();
                    LeftOperand = AnsDoubleStack.top();
                    AnsDoubleStack.pop();
                }else {
                    /*return false;*/
                    return 0;
                }

                if (!strcmp(cName,"+"))
                {
                    AnsDoubleStack.push(LeftOperand + RightOperand);
                }else if (!strcmp(cName,"-"))
                {
                    AnsDoubleStack.push(LeftOperand - RightOperand);
                }else if (!strcmp(cName,"*"))
                {
                    AnsDoubleStack.push(LeftOperand * RightOperand);
                }else if (!strcmp(cName,"/"))
                {
                    AnsDoubleStack.push(LeftOperand / RightOperand);
                }else if (!strcmp(cName,"^"))
                {
                    AnsDoubleStack.push(pow(LeftOperand,RightOperand));
                }else if (!strcmp(cName,">"))
                {
                    AnsDoubleStack.push(static_cast<double>(LeftOperand > RightOperand));
                }else if (!strcmp(cName,">="))
                {
                    AnsDoubleStack.push(static_cast<double>(LeftOperand >= RightOperand));
                }else if (!strcmp(cName,"<"))
                {
                    AnsDoubleStack.push(static_cast<double>(LeftOperand < RightOperand));
                }else if (!strcmp(cName,"<="))
                {
                    AnsDoubleStack.push(static_cast<double>(LeftOperand <= RightOperand));
                }else if (!strcmp(cName,"=="))
                {
                    AnsDoubleStack.push(static_cast<double>(LeftOperand == RightOperand));
                }else if (!strcmp(cName,"!="))
                {
                    AnsDoubleStack.push(static_cast<double>(LeftOperand != RightOperand));
                }else if (!strcmp(cName,"&&"))
                {
                    AnsDoubleStack.push(static_cast<double>(LeftOperand && RightOperand));
                }else if (!strcmp(cName,"||"))
                {
                    AnsDoubleStack.push(static_cast<double>(LeftOperand || RightOperand));
                }else if (!strcmp(cName,"%"))
                {
                    AnsDoubleStack.push(static_cast<double>(static_cast<int>(LeftOperand)
                                                          % static_cast<int>(RightOperand)));
                }else if (!strcmp(cName,"="))
                {
                    if (AnsDoubleStack.size() == 1)
                    {
                        AnsVariable.dNumber = AnsDoubleStack.top();
                        AnsVariable.kind = OperandDouble;
                        /*return true;*/
                        return 1;
                    }else{
                        /*return false;*/
                        return 0;
                    }
                }else {
                    /*
                     *  maybe error
                     */
                    /*return false;*/
                    return 0;
                }
            }
        }else if (kind == UnayOprator)
        {
            if (!AnsDoubleStack.empty())
            {
                RightOperand = AnsDoubleStack.top();
                AnsDoubleStack.pop();
            }else {
                /*return false;*/
                return 0;
            }
            strcpy(cName,OperatorDic[iIndex].cName);
            if (!strcmp(cName,"!"))
            {
                dNumber = this->factorial(static_cast<unsigned int>(RightOperand));
                if (dNumber == -1)
                {
                    cout<<"Invalid Factorial Operand '"<<RightOperand<<"'"<<endl;
                }else {
                    AnsDoubleStack.push(dNumber);
                }
            }else if (!strcmp(cName,"~"))
            {
                AnsDoubleStack.push(static_cast<double>(~static_cast<int>(RightOperand)));
            }else if (!strcmp(cName,"#"))
            {
                AnsDoubleStack.push(static_cast<double>(-1*RightOperand));
            }
        }else if (kind == Function)
        {
            strcpy(cName,OperatorDic[iIndex].cName);
            if (!strcmp("pow",cName))                                   /*  pow相当于二元寒素            */
            {
                if (AnsDoubleStack.size() >= 2)                         /*  从堆栈中取出两个操作数       */
                {
                    RightOperand = AnsDoubleStack.top();
                    AnsDoubleStack.pop();
                    LeftOperand = AnsDoubleStack.top();
                    AnsDoubleStack.pop();
                }else {
                   /* return false;*/
                    return 0;
                }
                AnsDoubleStack.push(pow(LeftOperand, RightOperand));
            }else{                                                      /*  一元函数                     */
                if (!AnsDoubleStack.empty())                            /*  从堆栈中取出一个操作数       */
                {
                    RightOperand = AnsDoubleStack.top();
                    AnsDoubleStack.pop();
                }else {
                    /*return false;*/
                    return 0;
                }
                /*
                 *  进行函数计算，并经运算结果存入堆栈
                 */
                if (!(strcmp("sin",cName)))
                {
                    AnsDoubleStack.push(sin(RightOperand));
                }else if (!(strcmp("cos",cName)))
                {
                    AnsDoubleStack.push(cos(RightOperand));
                }else if (!(strcmp("tan",cName)))
                {
                    AnsDoubleStack.push(tan(RightOperand));
                }else if (!(strcmp("cot",cName)))
                {
                    AnsDoubleStack.push(1.0/tan(RightOperand));
                }else if (!(strcmp("asin",cName)))
                {
                    AnsDoubleStack.push(asin(RightOperand));
                }else if (!(strcmp("acos",cName)))
                {
                    AnsDoubleStack.push(acos(RightOperand));
                }else if (!(strcmp("atan",cName)))
                {
                    AnsDoubleStack.push(atan(RightOperand));
                }else if (!(strcmp("acot",cName)))
                {
                    AnsDoubleStack.push(1.0/atan(RightOperand));
                }else if (!(strcmp("log10",cName)))
                {
                    AnsDoubleStack.push(log10(RightOperand));
                }else if (!(strcmp("log",cName)))
                {
                    AnsDoubleStack.push(log(RightOperand));
                }else if (!(strcmp("exp",cName)))
                {
                    AnsDoubleStack.push(exp(RightOperand));
                }else if (!(strcmp("sqrt",cName)))
                {
                    AnsDoubleStack.push(sqrt(RightOperand));
                }else if (!(strcmp("abs",cName)))
                {
                    AnsDoubleStack.push(abs(RightOperand));
                }else if (!(strcmp("ceil",cName)))
                {
                    AnsDoubleStack.push(ceil(RightOperand));
                }else if (!(strcmp("floor",cName)))
                {
                    AnsDoubleStack.push(floor(RightOperand));
                }else {
                    /*return false;*/
                    return 0;
                }
            }
        }else if (kind == UserDefineBinOprator)                         /*  用户自定义二元运算符         */
        {
            if (AnsDoubleStack.size() >= 2)                             /*  从堆栈中取出两个操作数       */
            {
                RightOperand = AnsDoubleStack.top();
                AnsDoubleStack.pop();
                LeftOperand = AnsDoubleStack.top();
                AnsDoubleStack.pop();
            }else {
                /*return false;*/
                return 0;
            }
            /*
             *  新建一个Poland对象，将自定义运算符的表达式当做一个表达式来计算
             *  并将运算结果存入堆栈
             */
            Poland TempPoland;
            /* 拷贝自定义运算符 */
            if (!this->UserOperatorVector.empty())
            {
                for (unsigned int t = 0;t < this->UserOperatorVector.size();t++)
                {
                    TempPoland.UserOperatorVector.push_back(this->UserOperatorVector.at(t));
                } 
            }
            Variable TempAnsVariable;
            Variable LeftVariable;
            Variable RightVariable;
            LeftVariable.kind = VariableDouble;
            strcpy(LeftVariable.cName,UserOperatorVector.at(iIndex).cLeftOperand);
            LeftVariable.dNumber = LeftOperand;

            RightVariable.kind = VariableDouble;
            strcpy(RightVariable.cName,UserOperatorVector.at(iIndex).cRightOperand);
            RightVariable.dNumber = RightOperand;

            TempPoland.VariableVector.push_back(LeftVariable);
            TempPoland.VariableVector.push_back(RightVariable);

            TempPoland.BreakExpress(UserOperatorVector.at(iIndex).sExpress);
            TempPoland.Infix2Postfix();
            TempPoland.Postfix2Number(TempAnsVariable);

            AnsDoubleStack.push(TempAnsVariable.dNumber);

        }else if (kind == UserDefineUnayOprator)                        /*  用户自定义一元运算符         */
        {
            if (!AnsDoubleStack.empty())                                /*  从堆栈中取出一个操作数       */
            {
                RightOperand = AnsDoubleStack.top();
                AnsDoubleStack.pop();
            }else {
               /* return false;*/
                return 0;
            }
            /*
             *  新建一个Poland对象，将自定义运算符的表达式当做一个表达式来计算
             *  并将运算结果存入堆栈
             */
            Poland TempPoland;
            /* 拷贝自定义运算符 */
            if (!this->UserOperatorVector.empty())
            {
                for (unsigned int t = 0;t < this->UserOperatorVector.size();t++)
                {
                      TempPoland.UserOperatorVector.push_back(this->UserOperatorVector.at(t));
                } 
            }
            Variable TempAnsVariable;
            Variable RightVariable;

            RightVariable.kind = VariableDouble;
            strcpy(RightVariable.cName,UserOperatorVector.at(iIndex).cRightOperand);
            RightVariable.dNumber = RightOperand;

            TempPoland.VariableVector.push_back(RightVariable);

            TempPoland.BreakExpress(UserOperatorVector.at(iIndex).sExpress);
            TempPoland.Infix2Postfix();
            TempPoland.Postfix2Number(TempAnsVariable);

            AnsDoubleStack.push(TempAnsVariable.dNumber);
        }else if (kind == EndExpression)                                /*  表达式结尾符，保存结果       */
        {
            if (AnsDoubleStack.size() == 1)
            {
                AnsVariable.dNumber = AnsDoubleStack.top();
                AnsVariable.kind = OperandDouble;
                /*TODO: 此处应该判断数据是否溢出，若溢出，切换到BigDouble继续计算*/
                int iIndex = *((int*)(&AnsVariable.dNumber) + 1);
                iIndex = (iIndex & 0x7ff00000)>>20;
                bdOverFlow = this->doubleOverflowTest(AnsVariable.dNumber);
                if (bdOverFlow)
                {
                    return -1;
                }else if (!bdOverFlow)
                {
                    return 1;
                }
            }else{
              /*  return false;*/
                return 0;
            }
        }else
        {
            cerr<<"error"<<endl;
            /*return false;*/
            return 0;
        }
    }
    return false;
}

 /**
 * @ingroup  MemberFunction_Poland
 * @brief    求值表达式求值函数，对应存在大数值的情况
 * @param[in] PostfixIndexVector     表达式基本元素在数组或者向量中的下标
 * @param[in] PostfixKindTypeVector  表达式基本元素的类型
 * @param[in] iStartIndex            从PostfixIndexVector和PostfixKindTypeVector的取元素的开始下标，添加开
 * 始下标是为了适应每行多个表达式的情况
 * @param[out] AnsVariable 引用调用的方式，将结果存入AnsVariable传出
 * @retval  false 求值失败
 * @retval  true  求值成功
 * @note
 * @code
 * @endcode
 */

bool Poland::EvaluateExpressBigDouble(vector<int> &PostfixIndexVector,
                                 vector<KindType> &PostfixKindTypeVector,
                                              int iStartIndex,
                                         Variable & AnsVariable)
{
    stack<BigDouble>AnsBigDoubleStack;
    KindType kind;
    int iIndex;
    BigDouble LeftOperand  = BigDouble(0.0);
    BigDouble RightOperand = BigDouble(0.0);

    char cName[6];
    BigDouble bdNumber = 0.0;
    /*
     *  遇到操作数，则存储到堆栈中
     *  遇到运算符或者函数，将堆栈中的操作数取出，运算，再将运算结果存入堆栈
     *  遇到表达式结尾符，则计算结束
     */
    for (int i = iStartIndex; i< static_cast<int>(PostfixKindTypeVector.size());i++)
    {
        kind   = PostfixKindTypeVector.at(i);
        iIndex = PostfixIndexVector.at(i);

        if (kind == OperandDouble)
        {
            AnsBigDoubleStack.push(BigDouble(DoubleVector.at(iIndex)));
        }else if (kind == OperandBigDouble)
        {
            AnsBigDoubleStack.push(BigDoubleVector.at(iIndex));
        }else if (kind == ConstVariable)
        {
            AnsBigDoubleStack.push(BigDouble(VariableDic->dNumber));
        }else if (kind == VariableBigDouble)
        {
            AnsBigDoubleStack.push(VariableVector.at(iIndex).bdNumber);
        }else if (kind == VariableDouble)
        {
            AnsBigDoubleStack.push(BigDouble(VariableVector.at(iIndex).dNumber));
        }else if (kind == Undefine)
        {
            cout<<"Undefine Variable or Function '"<<VariableVector.at(iIndex).cName<<"'"<<endl;
            return false;
        }else if (kind == BinOperator)                                  /*  双目运算符                   */
        {
            strcpy(cName,OperatorDic[iIndex].cName);
            if (!strcmp(cName,"="))                                     /*  遇到=，直接返回右侧的运算结果*/
            {
                if (AnsBigDoubleStack.size() == 1)
                {
                    AnsVariable.bdNumber = AnsBigDoubleStack.top();
                    AnsVariable.kind = OperandBigDouble;
                    return true;
                }else{
                    return false;
                }
            }else if (!strcmp(cName,","))                               /*  逗号不进行处理              */
            {

            }else {
                if (AnsBigDoubleStack.size() >= 2)
                {
                    RightOperand = AnsBigDoubleStack.top();
                    AnsBigDoubleStack.pop();
                    LeftOperand = AnsBigDoubleStack.top();
                    AnsBigDoubleStack.pop();
                }else {
                    return false;
                }

                /*
                 *  遇到运算符，堆栈中取出操作数，计算结果，并将结果存入堆栈
                 */
                if (!strcmp(cName,"+"))
                {
                    AnsBigDoubleStack.push(LeftOperand + RightOperand);
                }else if (!strcmp(cName,"-"))
                {
                    AnsBigDoubleStack.push(LeftOperand - RightOperand);
                }else if (!strcmp(cName,"*"))
                {
                    AnsBigDoubleStack.push(LeftOperand * RightOperand);
                }else if (!strcmp(cName,"/"))
                {
                    AnsBigDoubleStack.push(LeftOperand / RightOperand);
                }else if (!strcmp(cName,">"))
                {
                    AnsBigDoubleStack.push(BigDouble(LeftOperand > RightOperand));
                }else if (!strcmp(cName,">="))
                {
                    AnsBigDoubleStack.push(BigDouble(LeftOperand >= RightOperand));
                }else if (!strcmp(cName,"<"))
                {
                    AnsBigDoubleStack.push(BigDouble(LeftOperand < RightOperand));
                }else if (!strcmp(cName,"<="))
                {
                    AnsBigDoubleStack.push(BigDouble(LeftOperand <= RightOperand));
                }else if (!strcmp(cName,"=="))
                {
                    AnsBigDoubleStack.push(BigDouble(LeftOperand == RightOperand));
                }else if (!strcmp(cName,"!="))
                {
                    AnsBigDoubleStack.push(BigDouble(LeftOperand != RightOperand));
                }else if (!strcmp(cName,"&&"))
                {
                    AnsBigDoubleStack.push(BigDouble(LeftOperand && RightOperand));
                }else if (!strcmp(cName,"||"))
                {
                    AnsBigDoubleStack.push(BigDouble(LeftOperand || RightOperand));
                }else if (!strcmp(cName,"%"))
                {
                    cout<<"Unsupported  Operator '%' on Big Double"<<endl;
                    return false;
                }else if (!strcmp(cName,"="))
                {
                    if (AnsBigDoubleStack.size() == 1)
                    {
                        AnsVariable.bdNumber = AnsBigDoubleStack.top();
                        AnsVariable.kind = OperandBigDouble;
                        return true;
                    }else{
                        return false;
                    }
                }else {
                     cout<<"Unsupported  Operator '"<<cName<<"' on Big Double"<<endl;
                    return false;
                }
            }
        }else if (kind == UnayOprator)                                  /*  单目运算符                   */
        {
            if (!AnsBigDoubleStack.empty())
            {
                RightOperand = AnsBigDoubleStack.top();
                AnsBigDoubleStack.pop();
            }else {
                return false;
            }
            strcpy(cName,OperatorDic[iIndex].cName);
            if (!strcmp(cName,"!"))
            {

                bdNumber = this->factorial(RightOperand);
                AnsBigDoubleStack.push(bdNumber);

            }else if (!strcmp(cName,"~"))
            {
                cout<<"Unsupported  Operator '~' on Big Double"<<endl;
            }else if (!strcmp(cName,"#"))
            {
                AnsBigDoubleStack.push((BigDouble(-1.0)*RightOperand));
            }
        }else if (kind == Function)                                     /*  函数                         */
        {
            strcpy(cName,OperatorDic[iIndex].cName);
            if (!strcmp("pow",cName))                                   /* pow相当于二元函数，单独处理   */
            {
                if (AnsBigDoubleStack.size() >= 2)
                {
                    RightOperand = AnsBigDoubleStack.top();
                    AnsBigDoubleStack.pop();
                    LeftOperand = AnsBigDoubleStack.top();
                    AnsBigDoubleStack.pop();
                }else {
                    return false;
                }
                cout<<"Unsupported  Function '"<<cName<<"' on Big Double"<<endl;
            }else{                                                      /*  一元函数                     */
                if (!AnsBigDoubleStack.empty())
                {
                    RightOperand = AnsBigDoubleStack.top();
                    AnsBigDoubleStack.pop();
                }else {
                    return false;
                }


                if (!(strcmp("log10",cName)))
                {
                    AnsBigDoubleStack.push(RightOperand.BigLog10(RightOperand));
                }else if (!(strcmp("log",cName)))
                {
                     AnsBigDoubleStack.push(RightOperand.BigLog(RightOperand));
                }else if (!(strcmp("abs",cName)))
                {
                    AnsBigDoubleStack.push(RightOperand.BigAbs(RightOperand));
                }else if (!(strcmp("sqrt",cName)))
                {
                    AnsBigDoubleStack.push(RightOperand.BigSqrt(RightOperand));
                }else {
                    cout<<"Unsupported  Function '"<<cName<<"' on Big Double"<<endl;
                    return false;
                }
            }
        }else if (kind == UserDefineBinOprator)                         /*  自定义二元运算符             */
        {
            /*
             *  新建一个Poland对象，将自定义运算符的表达式当做一个表达式来计算
             *  并将运算结果存入堆栈
             */
            if (AnsBigDoubleStack.size() >= 2)
            {
                RightOperand = AnsBigDoubleStack.top();
                AnsBigDoubleStack.pop();
                LeftOperand = AnsBigDoubleStack.top();
                AnsBigDoubleStack.pop();
            }else {
                return false;
            }
            Poland TempPoland;
            /* 拷贝自定义运算符 */
            if (!this->UserOperatorVector.empty())
            {
                for (unsigned int t = 0;t < this->UserOperatorVector.size();t++)
                {
                    TempPoland.UserOperatorVector.push_back(this->UserOperatorVector.at(t));
                } 
            }
            Variable TempAnsVariable;
            Variable LeftVariable;
            Variable RightVariable;
            LeftVariable.kind = VariableDouble;
            strcpy(LeftVariable.cName,UserOperatorVector.at(iIndex).cLeftOperand);
            LeftVariable.bdNumber = LeftOperand;

            RightVariable.kind = VariableDouble;
            strcpy(RightVariable.cName,UserOperatorVector.at(iIndex).cRightOperand);
            RightVariable.bdNumber = RightOperand;

            TempPoland.VariableVector.push_back(LeftVariable);
            TempPoland.VariableVector.push_back(RightVariable);

            TempPoland.BreakExpress(UserOperatorVector.at(iIndex).sExpress);
            TempPoland.Infix2Postfix();
            TempPoland.Postfix2Number(TempAnsVariable);
            AnsBigDoubleStack.push(TempAnsVariable.dNumber);

        }else if (kind == UserDefineUnayOprator)                        /*  自定义一元运算符             */
        {
            if (!AnsBigDoubleStack.empty())
            {
                RightOperand = AnsBigDoubleStack.top();
                AnsBigDoubleStack.pop();
            }else {
                return false;
            }

            /*
             *  新建一个Poland对象，将自定义运算符的表达式当做一个表达式来计算
             *  并将运算结果存入堆栈
             */
            Poland TempPoland;
            /* 拷贝自定义运算符 */
            if (!this->UserOperatorVector.empty())
            {
                for (unsigned int t = 0;t < this->UserOperatorVector.size();t++)
                {
                    TempPoland.UserOperatorVector.push_back(this->UserOperatorVector.at(t));
                } 
            }
            Variable TempAnsVariable;
            Variable RightVariable;

            RightVariable.kind = VariableDouble;
            strcpy(RightVariable.cName,UserOperatorVector.at(iIndex).cRightOperand);
            RightVariable.bdNumber = RightOperand;

            TempPoland.VariableVector.push_back(RightVariable);

            TempPoland.BreakExpress(UserOperatorVector.at(iIndex).sExpress);
            TempPoland.Infix2Postfix();
            TempPoland.Postfix2Number(TempAnsVariable);
            AnsBigDoubleStack.push(TempAnsVariable.dNumber);
                                                                        /*  表达式结尾符，保存运算结果   */
        }else if (kind == EndExpression)
        {
            if (AnsBigDoubleStack.size() == 1)
            {
                /* cout<<"ans = "<<endl<<"      "<<AnsDoubleStack.top()<<endl;*/
                AnsVariable.bdNumber = AnsBigDoubleStack.top();
                AnsVariable.kind = OperandBigDouble;
                return true;
            }else{
                return false;
            }
        }else
        {
            cerr<<"error"<<endl;
            return false;
        }
    }
    return false;
}

 /**
 * @ingroup  MemberFunction_Poland
 * @brief    阶乘求值函数
 * @param[in] n   阶乘的参数
 * @return  阶乘的运算结果
 * @note
 * @code
 * @endcode
 */
double Poland::factorial(unsigned int n){
    double dAnswer = 1.0;
    for (int i = 1; i <= static_cast<int>(n); i++ )
    {
        dAnswer *= i;
    }
    return dAnswer;


    if (n <= 150)
    {
        for (int i = 1; i <= static_cast<int>(n); i++ )
        {
            dAnswer *= i;
        }
        return dAnswer;
    }else {
        cout<<"Too big Operand for '!'"<<endl;
        return -1;
    }
}


 /**
 * @ingroup  MemberFunction_Poland
 * @brief    阶乘求值函数,对应于大数值的阶乘计算
 * @param[in] nBigDouble   阶乘的参数
 * @return  阶乘的运算结果
 * @note
 * @code
 * @endcode
 */
 BigDouble Poland::factorial(BigDouble nBigDouble){
     BigDouble bigAns = BigDouble(1);
     BigDouble bigIndex = BigDouble(1);

     for (bigIndex = BigDouble(1); bigIndex <= nBigDouble ;bigIndex = bigIndex + BigDouble(1))
     {
         bigAns  = bigAns*bigIndex;
     }
    /* cout<<"big:  "<<bigAns.getBigDoubleStr()<<endl;*/
     return bigAns;
 }
 BigDouble Poland::factorialBig(unsigned int n){
     BigDouble bdAnswer = BigDouble(1);
     int i = 0; 
     for (i = 1; i <= n ; i++);
     {
         bdAnswer = bdAnswer * BigDouble(i);
     }
     return bdAnswer;
 }


 /**
 * @ingroup  MemberFunction_Poland
 * @brief    变量保存函数
 * @details  将变量保存到VariableVector中
 * @param[in] AnsVariable     需要保存的Variable类型的变量
 * @param[in] VariableVector  Variable类型变量要保存的位置
 * @retval  false VariableVector中不存在同名变量，将AnsVariable放入VariableVector
 * @retval  true  VariableVector中存在同名变量，将对应同名变量的值和类型修改
 * @code
 * @endcode
 */
bool Poland::SaveVariable(Variable & AnsVariable, vector<Variable>& VariableVector){
    bool VariableExist = false;
    for (int i = 0 ;i< static_cast<int>(VariableVector.size());i++)
    {
        if (!strcmp(AnsVariable.cName, VariableVector.at(i).cName))
        {
            VariableExist = true;
            VariableVector.at(i).kind = AnsVariable.kind;
            VariableVector.at(i).dNumber = AnsVariable.dNumber;
            VariableVector.at(i).bdNumber = AnsVariable.bdNumber;
            return true;
        }
    }
    if (!VariableExist)
    {
        VariableVector.push_back(AnsVariable);
        return false;
    }
    return true;
}


/**
 * @ingroup  MemberFunction_Poland
 * @brief    打印帮助系统所有的命令和含义
 * @note
 * @code
 * @endcode
 */
void Poland::Help(){
    cout<<"Command Topics:"<<endl;
    for (int i = 0 ;i< COMMANDMOST;i++)
    {
        cout<<CommandDic[i].cName<<"     \t-"<<CommandDic[i].sCommandDetail<<endl;
    }

    cout<<"For more information, Input: help command\nFor example:\nhelp test"<<endl;
}


/**
 * @ingroup  MemberFunction_Poland
 * @brief    命令类型检测
 * @param[in] sExpress    命令表达式式或者数学表达式
 * @retval  0 不是命令
 * @retval  大于等于0 命令在数组CommandDic[]中的下标
 * @note    参数列表必须符合规范
 * @code
 * string sExpress = string("help clear");
 * Poland poland;
 * int ComandType = Poland.CommandType(sExpress);
 * @endcode
 */
int Poland::CommandType(string sExpress){
    /*
     *  命令形式： Command ParameterList ……
     *  先提取命令Command，然后解析参数列表，命令与参数列表，以及参数列表之间采用空格分隔
     */
    int iLength = sExpress.size();
    int iSubLength = 0;
    int i = 0;
    for (i = 0 ;i< iLength;i++)
    {
        if (sExpress.at(i) == ' ')
        {
            break;
        }
    }
    iSubLength = i;
    char *cSubStr = new char[iSubLength +1];
   for (int i = 0; i<iSubLength;i++)
   {
       cSubStr[i] = sExpress.at(i);
   }
   cSubStr[iSubLength] = '\0';

    for (int i = 0;i< COMMANDMOST;i++)
    {
        if (!strcmp(cSubStr,CommandDic[i].cName))
        {
            delete [] cSubStr;
            return i;
        }
    }

  delete [] cSubStr;
  return -1;
}


/**
 * @ingroup  MemberFunction_Poland
 * @brief    命令执行函数
 * @param[in] sExpress    命令表达式
 * @param[in] CommandType    命令类型
 * @retval  0 参数不合法，命令执行出错
 * @retval  成功执行命令
 * @note    参数列表必须符合规范
 * @code
 * string sExpress = string("help clear");
 * Poland poland;
 * int ComandType = Poland.CommandType(sExpress);
 * if(ComandType > = 0){
 *      poland.ExecuteCommand(sExpress,CommandType);
 * }else {
 *
 * }
 * @endcode
 */
bool Poland::ExecuteCommand(string sExpress, int CommandType){
    int iParameterCount = 0;
    int iLength = sExpress.size();
    char * cExpress = new char [iLength+1];
    for (int i = 0; i < iLength ;i++)
    {
        cExpress[i] = sExpress.at(i);
    }cExpress[iLength] = '\0';

    /*
     *  统计命令的参数个数
     */
    for (int i = strlen(CommandDic[CommandType].cName); i < iLength - 1 ;i++ )
    {
        if (cExpress[i] == ' ')
        {
            if (cExpress[i + 1] != ' ')
            {
                iParameterCount++;
            }
        }
    }
    /*
     *  将参数列表中的参数提取出来，放入strArray
     */
    string * strArray = new string [iParameterCount + 1];
    int iArrayCount = 0;

    for (int i = strlen(CommandDic[CommandType].cName) + 1 ;i<= iLength;i++)
    {
        if ((cExpress[i] == ' ' || cExpress[i] == '\0') && cExpress[i-1] != ' ')
        {
           iArrayCount++;
        }else {
            strArray[iArrayCount] += cExpress[i];
        }
    }

    bool bParameterMatch = false;
    switch (CommandType){
    case 0:                                                             /*  help命令，打印帮助           */

        if (iParameterCount == 0)
        {
            this->Help();
        }else {
            for (int j = 0;j< iParameterCount;j++)
            {
                bParameterMatch = false;
                for (int k = 0; k < COMMANDMOST;k++ )
                {
                    if (strArray[j] == string(CommandDic[k].cName))
                    {
                        bParameterMatch = true;
                        cout<<CommandDic[k].cName<<"\t"<<CommandDic[k].sCommandDetail<<endl;
                        cout<<"For Example: \n"<<CommandDic[k].sExample<<endl;
                    }
                }
                if (!bParameterMatch)
                {
                    cout<<"Unknow Parameter '"<<strArray[j]<<"'"<<endl;
                }
            }
        }
        break;
    case 1:                                                             /*  whos命令，查看所有变量       */
        if (iParameterCount == 0)
        {
            this->PrintVariable();
        }else{
            cout<<"Too many parameters for '"<<CommandDic[CommandType].cName<<"'"<<endl;
        }
      break;
    case 2:                                                             /*  who命令，查看指定变量        */

        if (iParameterCount == 0)
        {
            cout<<"Too less parameter for '"<<CommandDic[CommandType].cName<<"'"<<endl;
        }else {
            for (int j = 0;j< iParameterCount;j++)
            {
                bParameterMatch = false;
                for (int k = 0; k < static_cast<int>(VariableVector.size());k++ )
                {
                    if (strArray[j] == string(VariableVector.at(k).cName))
                    {
                        bParameterMatch = true;
                        if (VariableVector.at(k).kind == VariableDouble)
                        {
                             cout<<VariableVector.at(k).cName<<"\t"<<VariableVector.at(k).dNumber<<endl;
                        }else  if (VariableVector.at(k).kind == VariableBigDouble)
                        {
                            cout<<VariableVector.at(k).cName<<"\t"
                                <<VariableVector.at(k).bdNumber.getBigDoubleStr()<<endl;
                        }
                    }
                }
                if (!bParameterMatch)
                {
                    cout<<"Undefine variable or function '"<<strArray[j]<<"'"<<endl;
                }
            }
        }
        break;
    case 3:                                                             /*  clc命令，清屏                */

        if (iParameterCount == 0)
        {
             system("CLS");
        }else{
           cout<<"Too many parameters for '"<<CommandDic[CommandType].cName<<"'"<<endl;
        }
         break;
    case 4:                                                             /*  clear命令，清除变量          */
        this->HistoryIndexVector.clear();
        this->HistoryKindTypeVector.clear();
        if (iParameterCount == 0)
        {
             VariableVector.clear();
        }else{
            for (int j = 0;j< iParameterCount;j++)
            {
                for (int k = 0; k < static_cast<int>(VariableVector.size());k++ )
                {
                    if (strArray[j] == string(VariableVector.at(k).cName))
                    {

                         VariableVector.erase(VariableVector.begin() + k);
                    }
                }
            }
        }
         break;
    case 5:                                                             /*  operator命令，自定义运算符   */
        if (iParameterCount == 4)
        {
             this->SaveOperator(sExpress,strArray);
        }else if (iParameterCount < 4)
        {
            cout<<"Too less Parameter for '"<<CommandDic[CommandType].cName<<"'"<<endl;
        }else {
          cout<<"Too many parameters for '"<<CommandDic[CommandType].cName<<"'"<<endl;
        }
         break;
    case 6:                                                             /*  copyright命令，查看版权声明  */

        if (iParameterCount == 0)
        {
             cout<<"Guangzhou ZHIYUAN electronics Co.,LTD."<<endl;
             cout<<"All rights reserved."<<endl;
        }else{
            cout<<"Too many parameters for '"<<CommandDic[CommandType].cName<<"'"<<endl;
        }
        break;
    case 7:                                                             /*  history命令，查看历史记录    */
        if (iParameterCount == 0)
        {
             this->PrintHistoryExpress();
        }else{
            cout<<"Too many parameters for '"<<CommandDic[CommandType].cName<<"'"<<endl;
        }
       break;
    case 8:                                                             /*  exit命令，退出程序           */
        if (iParameterCount == 0)
        {
            exit(1);
        }else{
            cout<<"Too many parameters for '"<<CommandDic[CommandType].cName<<"'"<<endl;
        }
        break;
    case 9:                                                             /* test命令，自动测试程序        */
        if (iParameterCount == 0)
        {
             this->Test(string("test.txt"));
        }else {
            for (int i = 0 ; i< iParameterCount;i++)
            {
                this->Test(strArray[i]);
            }  
        }
       break;
    case 10:                                                            /*  打印运算符字典               */
        if (iParameterCount == 0)
        {
            this->PrintOperator();
        }else {
            cout<<"Too many parameters for '"<<CommandDic[CommandType].cName<<"'"<<endl; 
        }
        break;
    default:
       ;
    }
    delete [] strArray;
    delete [] cExpress;
    return true;
}




/**
 * @ingroup  MemberFunction_Poland 波兰表达式的成员函数
 * @brief    将自定义运算符按固定格式存储
 * @details  将自定义运算符存储到向量UserOperatorVector中
 * @param[in] sExprsss  自定义运算符的字符串
 * @retval  false 自定义运算符失败
 * @retval  true  自定义运算符成功
 * @note    应按照如下格式："operator operatorName format Express priority
 * 例如：operator @ a@b a+b 4"
 * @code
 * @endcode
 */
bool Poland::SaveOperator(string sExprsss, string * sParameter){
    char * cLeftOperand;
    char * cRightOperand;
    int iOperatorIndex = 0;
    char cOperator;

    UserDefineOprator uOperator;

    if (sParameter[0].size() == 1)                                      /*  自定义运算符要求长度为1      */
    {
        cOperator = sParameter[0].at(0);
         if (this->SearchOperator(cOperator) >= 0){
             /*
              *  常用运算符，已经存在，不允许用户自定义
              */
             cout<<"Operator '"<<sParameter[0]<<"' is a common operator, cannot redefine it"<<endl;
         }else {
             /*
              *  未定义运算符，直接定义一个即可
              *  若该运算符用户已定义，则重新定义
              */
             uOperator.cOperator = cOperator;
             /*
              *  逐个提取参数列表中的参数
              */
             bool OperatorExist = false;
             for (int i = 0;i< static_cast<int>(sParameter[1].size());i++)
             {
                  if (sParameter[1].at(i) == cOperator)
                  {
                      OperatorExist = true;
                      iOperatorIndex = i;
                      cLeftOperand = new char [i+1];
                      cRightOperand = new char [sParameter[1].size() - i];
                      break;
                  }
             }

             if (!OperatorExist)                                        /*   Format中无运算符，错误      */
             {
                 cout<<"Invalid expression to define Operator"<<endl;
             }else {                                                    /*   有该运算符，提取左右变量名  */
                 for (int j = 0;j< iOperatorIndex;j++)
                 {
                      cLeftOperand[j] = sParameter[1].at(j);
                 }
                 cLeftOperand[iOperatorIndex] = '\0';

                 for (int j = iOperatorIndex + 1;j< static_cast<int>(sParameter[1].size());j++)
                 {
                     cRightOperand[j - iOperatorIndex -1] = sParameter[1].at(j);
                 }
                 cRightOperand[sParameter[1].size() - iOperatorIndex-1] = '\0';

                 if (iOperatorIndex == 0 )                              /*  单目运算符，左运算符         */
                 {
                     if (this->IsValidVariableName(cLeftOperand))
                     {
                         uOperator.kind = UserDefineUnayOprator;
                         strcpy(uOperator.cLeftOperand, cRightOperand);
                         strcpy(uOperator.cRightOperand,cRightOperand);
                     }else {
                         cout<<"Invalid Variable '"<<cRightOperand<< "' to define Operator"<<endl;
                         return false;
                     }
                 }else if (iOperatorIndex == static_cast<int>(sParameter[1].size() -1))
                 {
                     /*
                      *  单目运算符，右运算符
                      */
                     if (this->IsValidVariableName(cLeftOperand))
                     {
                         uOperator.kind = UserDefineUnayOprator;
                         strcpy(uOperator.cLeftOperand, cLeftOperand);
                         strcpy(uOperator.cRightOperand,cLeftOperand);
                     }else {
                         cout<<"Invalid Variable '"<<cLeftOperand<< "' to define Operator"<<endl;
                         return false;
                     }

                 }else {                                                /*  双目运算符                   */

                     if (this->IsValidVariableName(cLeftOperand)
                      && this->IsValidVariableName(cRightOperand))
                     {
                         uOperator.kind = UserDefineBinOprator;
                         strcpy(uOperator.cLeftOperand, cLeftOperand);
                         strcpy(uOperator.cRightOperand,cRightOperand);
                     }else{
                         cout<<"Invalid Variable name '"<<cLeftOperand<<"'  and  '"
                             <<cRightOperand<< "' to define Operator"<<endl;
                     }

                 }
                 uOperator.sExpress = sParameter[2];
                 /*
                  *  优先级提取
                  */
                 if ( this->NumberTypeCheck(sParameter[3]) != 'n')
                 {
                      uOperator.iPriority = static_cast<int>(this->OperandStr2Number(sParameter[3]));
                 }else {
                    cout<<"Invalid Priority '"<<sParameter[3]<<"'" <<endl;
                 }

                 delete [] cLeftOperand;
                 delete [] cRightOperand;
             }

             /*
              *  查看该运算符是否定义过
              *  若定义过则从新定义，若没有则添加新的自定义运算符
              */

             int iIndex = this->SearchUserDefineOperator(cOperator);
             if (iIndex >= 0)                                           /*  已定义该运算符，修改定义     */
             {
                 cout<<"Operator '"<<cOperator<<"' is redefined"<<endl;
                 UserOperatorVector.at(iIndex) = uOperator;
             }else {                                                    /*  未定义该运算符，添加定义     */
               UserOperatorVector.push_back(uOperator);
             }
         }
    }else {
        cout<<"Length of user define Operator must be 1"<<endl;
        return false;
    }
    return true;
}



/**
 * @ingroup  MemberFunction_Poland
 * @brief    常用运算符搜索函数
 * @param[in] cOperator    待搜索的char型的运算符
 * @return 返回搜索到的常用运算符的下标，不存在时则返回-1
 * @note   用于自定义运算符，并且自定义运算符限定只能是一个字符，所以参数是char型
 * @code
 * @endcode
 */
int Poland::SearchOperator(char cOperator){
   char cOperatorTemp[2];
   cOperatorTemp[0] = cOperator;
   cOperatorTemp[1] = '\0';
   for (int i = 0 ; i< OPERATORMOST;i++)
   {
       if (!strcmp(cOperatorTemp,OperatorDic[i].cName))
       {
           return i;
       }
   }
   return -1;
}

/**
 * @ingroup  MemberFunction_Poland
 * @brief    自定义运算符运算符搜索函数
 * @param[in] cOperator    待搜索的char型的运算符
 * @return 返回搜索到的自定义运算符的下标，不存在时则返回-1
 * @note   用于自定义运算符，并且自定义运算符限定只能是一个字符，所以参数是char型
 * @code
 * @endcode
 */
int Poland::SearchUserDefineOperator(char cOperator){
    for (int i = 0 ; i< static_cast<int>(UserOperatorVector.size());i++)
    {
        if (cOperator == UserOperatorVector.at(i).cOperator)
        {
            return i;
        }
    }
    return -1;
}

bool Poland::doubleOverflowTest(double dNumber){
     int iIndex = 0;
     int * piIndex = NULL;
     piIndex = (int*)(&dNumber);
     iIndex = piIndex[1];
     iIndex = (iIndex & 0x7ff00000)>>20;
     if (iIndex == 0x7ff)
     {
         return true;
     }else{
         return false;
     }
}
/* end of file */
