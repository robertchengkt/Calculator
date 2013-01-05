/*
 * Guangzhou ZHIYUAN electronics Co.,LTD.
 * http://www.embedtools.com
 * All rights reserved
 *
 * @file    Poland.cpp
 * @brief   Poland Expression Class �������ʽ�ļ���
 *
 * Change Logs:
 * Date         Author      Notes
 * 2012-08-10   Libing   Second Edtion
 */

#include "Poland.h"
#define OPERATORMOST         38                                         /**< ���������                  */
#define CONSTVARIABLEMOST     2                                         /**< ��������                    */
#define COMMANDMOST          11                                         /**< �������                    */

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
 * @brief    �����ֵ�
 * @details  �洢�����г��õ��ĳ������û�����ֱ��ʹ��
 */
const Variable VariableDic[CONSTVARIABLEMOST]= {
    {"pi",ConstVariable,3.141592653589793,BigDouble(3.141592653589793)},
    {"e" ,ConstVariable,2.718281828459046,BigDouble(2.718281828459046)},
};

/**
 * @brief    ����������ֵ�
 * @details  �洢��ѧ���ʽ�г��õ�������������������ŵ��Լ����ͺ����ȼ�
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
    {"#" ,UnayOprator  ,7},                                             /*  ���ţ�ʹ��#���棬�ͼ�������  */
    {"~" ,UnayOprator  ,7},                                             /*  ����ȡ��                     */
    {"!" ,UnayOprator  ,7},                                             /*  �׳�                         */
    {"(" ,LeftParen    ,8},
    {")" ,RightParen   ,8},
    {";" ,EndExpression,9},
    /*  ����  */
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
 * @brief    �����ֵ�
 * @details  �洢����ϵͳ�г��õ�����Լ�ע����÷�ʾ��
 */
const Command CommandDic[COMMANDMOST]={
    {"help"     ,  "Display help information"      , "help whos \nhelp"           },/**<  �鿴����       */
    {"whos"     ,  "Lists all the variables"       , "whos"                       },/**<  �鿴���б���   */
    {"who"      ,  "List current variables."       , "who variableName"           },/**<  �鿴ָ������   */
    {"clc"      ,  "Clear command window."         , "clc"                        },/**<  ����           */
    {"clear"    ,  "Removes all variables"         , "clear variableName \nclear" },/**<  �������       */
    {"operator" ,  "Define  user's Operator"       , "operator $ a$b 1/2*pi*a*b 4"},/**<  ����           */
    {"copyright",  "Display copyright declaration" , "copyright"                  },/**<  ��Ȩ����       */
    {"history"  ,  "Display history expression."   , "history"                    },/**<  ��ʷ��¼       */
    {"exit"     ,  "Exit from the console"         , "exit"                       },/**<  �˳�����       */
    {"test"     ,  "Auto test the programm"        , "test test.txt"              },/**<  �Զ�����       */
    {"dic"      ,  "Display the dictionary"        , "dic"                        },/**<  ��ӡ�ֵ�       */

};
/**
 * @defgroup  Constructors_Poland
 * ����Poland��ĳ�ʼ��
 */

/**
 * @defgroup  Destructors_Poland
 * ����Poland�������
 */

/**
 * @defgroup  MemberFunction_Poland
 * Poland��ĳ�Ա���������в������ʽ�ļ���
 */


/**
 * @ingroup  Constructors_Poland
 * @brief    Poland��Ĺ��캯��
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
 * @brief    Poland�����������
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
 * @brief    Poland��Ĳ����ĳ�ʼ�����ڹ��캯�����Զ�����
 *
 */
void Poland::Initialize(){
    /*
     *  ��ʼ�����룬Ϊ�˼��㷽�㣬�������һ������ans��ansB�����ڴ洢������ʽ�Ľ�������´μ���ʹ��
     */
    Variable VariableTemp = {"ans",VariableDouble,0.0,BigDouble(0.0)};
    VariableVector.push_back(VariableTemp);

    Variable VariableTemp2 = {"ansB",VariableBigDouble,0.0,BigDouble(1.23,1000)};
    VariableVector.push_back(VariableTemp2);
}

/**
 * @ingroup  MemberFunction_Poland
 * @brief    ��ӡ�﷨�ֵ䣬������������������������Զ��������
 *
 * @code
 * Poland poland;
 * Poland.PrintDictionary();
 * @endcode
 */
void Poland::PrintDictionary(){
    /*
     *  ��ӡ����
     */
    cout<<"Variable Name\t\t"<<"Number"<<endl;
    for (int i = 0; i< CONSTVARIABLEMOST;i++)
    {
        cout<<VariableDic[i].cName<<"\t\t\t"<<VariableDic[i].dNumber<<endl;
        /*cout<<setw(35)<<VariableDic[i].cName<<VariableDic[i].dNumber<<endl;*/
    }
    /*
     *  ��ӡ�û��������
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
     *  ��ӡ���������
     */
    cout<<"\n\nOperator Name\t\t"<<"Priority"<<endl;
    for (int i = 0; i< OPERATORMOST;i++)
    {
        cout<<OperatorDic[i].cName<<"\t\t\t"<<OperatorDic[i].iPriority<<endl;
    }

    /*
     *  ��ӡ�û��Զ��������
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
 * @brief    ��ӡ�û��Զ������
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
 * @brief    ��ӡ������������û��Զ��������
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
 * @brief    ��string���ͱ��ʽ��ֳ���ֵ����������������������Զ���������Ȼ���Ԫ�أ��ֱ�洢����Ӧ������
 * vector��
 * @param[in] sExpress    �û�����ĵ�string���͵���ѧ���ʽ
 * @note   sExpress�в����к��֣������пո񣻿��Ժ��ж������ʽ��������Ҫ�÷ֺ�";"�ָ�
 * @code
 * string sExpress;
 * Poland poland;
 * getline(cin,sExpress);
 * Poland.BreakExpress(sExpress);
 * @endcode
 */
void Poland::BreakExpress(string sExpress){

    /*
     *  �Ƚ��洢�±�����͵��������
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

    this->MarkNegative(cExpress);                                       /*  �Ը��ž��ı��              */

    for (int i = 0;i<iLength;i++)
    {
        OperatorMatch = false;

        /*
         * ���������ƥ��ʱ��Ϊ�˼ӿ��ٶȣ����ȶԵ�һ���ַ�����ƥ�䣬��ƥ���һ�����ٽ��д�ƥ��
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
                         *  �����������治�������ţ�����Ϊ���Ǻ������п����Ǳ�������singal = 1�����ܽ�sin
                         *  �����Ϊ��������
                         */
                        OperatorMatch = false;
                        break;
                    }else{
                        OperatorMatch = true;

                        RightIndex = i-1;
                        /*
                         *  ��ȡ�����ǰ��Ĳ�����
                         */
                        this->AbstractOperand(cExpress,LeftIndex,RightIndex);


                        /*
                         *  �����������vector
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
             *  �����ǳ�����������������û��Զ��������
             */
            for (int k = 0; k<static_cast<int>(UserOperatorVector.size());k++)
            {
                if (UserOperatorVector.at(k).cOperator == cExpress[i])
                {
                    OperatorMatch = true;

                    RightIndex = i-1;

                    /*
                     *  ��ȡ�����ǰ��Ĳ�����
                     */
                    this->AbstractOperand(cExpress,LeftIndex,RightIndex);
                    /*
                     *  �����������vector
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
     *  ��ȡ���ʽ����һ��������
     */
    this->AbstractOperand(cExpress,LeftIndex,iLength -1);
    /*
     *  ����һ�����������ʽ�����ʽ֮��ʹ�÷ֺŷָ������ʽĩβ��û�зֺţ����Զ�׷��
     */
    if (KindTypeVector.back() != EndExpression)
    {
        IndexVector.push_back(20);
        KindTypeVector.push_back(EndExpression);
    }

    delete [] cExpress;

    /*
     *   ������֮��ı��ʽ���б��浽��ʷ��¼
     */
    this->SaveBreakedExpress();
}


/**
 * @ingroup  MemberFunction_Poland
 * @brief    ����-�ͼ���-����ѧ�����о��в�ͬ�����ȼ���Ϊ�����ָ��źͼ��ţ������ʽ�еĸ�����һ�������÷���
 * "#"����
 * @param[in] cExpress    char�����ַ������ɵ���ѧ���ʽ
 * @code
 * char cExpress[] = "1*-1/-2";
 * Poland poland;
  * Poland.MarkNegative(cExpress);
 * @endcode
 */
void Poland::MarkNegative(char* cExpress){

     /*
      *  ���źͼ��ŵ����֣�����'-'����һ����������ߺ���֮�󣬻���������֮�󣬻����±��ʽ�Ŀ�ͷ�����Ǹ���
      *  ������Ϊ�Ǽ���
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

        if (!OperatorMatch)                                             /*  ���ǳ��������               */
        {
            /* �����û��Զ��������  */
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
 * @brief    �Զ����ַ����ȽϺ���
 * @details  �Ӵ�cSubStr��cStr�Ĵ��±�StartIndex��ʼ����ȶԣ�����ȫƥ���򷵻�true������Χfalse
 * @param[in] cSubStr    ���ַ���
 * @param[in] cStr       �ַ���
 * @param[in] StartIndex �ַ�����ʼ�±꣬�Ӵ˴���ʼ���Ӵ��ȶ�
 * @retval  true  �Ӵ����ַ���ƥ��
 * @retval  false �Ӵ����ַ�����ƥ��
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
 * @brief    �Զ����ַ�����������
 * @details  ���ַ���cStr���±�iStartIndex���±�iStopIndex����֮����ַ���������cSubStr
 * @param[in] cSubStr    �ַ�������Ŀ���ַ���
 * @param[in] cStr       �ַ�������Դ�ַ���
 * @param[in] iStartIndex  �ַ���������ʼ�±�
 * @param[in] iStopIndex   �ַ������������±�
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
 * @brief    ���ַ�����ʽ�Ĳ�����ת��Ϊ��ֵ
 * @details  ���в�����������ʮ�����ơ�ʮ���ơ��˽��ơ�����������
 * @param[in] cOperandStr    ����ѧ���ʽ����ȡ�Ĳ��������ַ���
 * @return   ���ش��ַ�������ȡ��double���͵���ֵ
 * @note   ʹ�ò�ͬ����ʱ����Ҫ�ӽ��Ʊ�־������ʮ�����Ƽ�ǰ׺0x���ߺ�׺h���˽�����Ӻ�׺o����������Ӻ�׺b��
 * ʮ������Ӻ�׺d����û��ǰ׺��׺��Ĭ����ʮ��������ǰ׺��׺��ĸʹ�ô�Сд����
 * @code
 * char cOperandStr[] = "ad.ffh";
 * Poland poland;
 * double dNumber = Poland.OperandStr2Number(cOperandStr);
 * @endcode
 */
double Poland::OperandStr2Number(char * cOperandStr){
    /*
     *  Ѱ��С���㣬����ÿ��������С�����λ�õõ���ͬ��Ȩֵ��
     *  ÿ��Ȩֵ*��λ��ֵ����͵õ���ֵ
     */
    int iLength = strlen(cOperandStr);
    int iDotIndex;
    double dNumber = 0.0;
    char cNumberType = this->NumberTypeCheck(cOperandStr);

    if (cNumberType == 'd')                                             /*  ʮ���ƣ���ǰ׺��׺           */
    {
        //string DecimalStr = string(cOperandStr);

        return atof(cOperandStr);

    }else if (cNumberType == 'D')                                      /*  ʮ���ƣ���׺d                 */
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


    }else if (cNumberType == 'h')                                       /*  ʮ�����ƣ�ǰ׺0x             */
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

    }else if (cNumberType == 'H')                                       /*  ʮ�����ƣ���׺h              */
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

    }else if (cNumberType == 'o')                                       /*  �˽��ƣ���׺o               */
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

    }else if (cNumberType == 'b')                                       /*  �����ƣ���׺b                */
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
    }else if (cNumberType == 'n')                                       /*  ��������                    */
    {
        cout<<"invalid number '"<<cOperandStr<<"'"<<endl;
        return -1.0;
    }
    return -1.0;
}


/**
 * @ingroup  MemberFunction_Poland
 * @brief    ���ַ�����ʽ�Ĳ�����ת��Ϊ��ֵ
 * @details  ���в�����������ʮ�����ơ�ʮ���ơ��˽��ơ�����������
 * @param[in] sOperandStr    ����ѧ���ʽ����ȡ�Ĳ��������ַ���
 * @return   ���ش��ַ�������ȡ��double���͵���ֵ
 * @note   ʹ�ò�ͬ����ʱ����Ҫ�ӽ��Ʊ�־������ʮ�����Ƽ�ǰ׺0x���ߺ�׺h���˽�����Ӻ�׺o����������Ӻ�׺b��
 * ʮ������Ӻ�׺d����û��ǰ׺��׺��Ĭ����ʮ��������ǰ׺��׺��ĸʹ�ô�Сд����
 * @code
 * string sOperandStr = string��"ad.ffh��";
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
 * @brief    ������������
 * @param[in] cVariableStr    �������ı������ַ���
 * @return �����������ı��������±꣬������ʱ�򷵻�-1
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
 * @brief    ������������
 * @param[in] cVariableStr    �������ĳ��������ַ���
 * @return �����������ĳ����������±꣬������ʱ�򷵻�-1
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
 * @brief    ��ӡ����ֳɱ�����������������Ȼ���Ԫ��֮��ı��ʽ�����ڲ鿴
 *           ��Ա����BreakExpress()�Ľ��
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
 * @brief    ���ַ�������ȡ������
 * @details  �����������Ǳ����������֣��ַ���ǰ������пո�
 * @param[in] cExpress    �ַ�����ʽ����ѧ���ʽ
 * @param[in] LeftIndex   �������ִ���cExpress�еĿ�ʼ�±�
 * @param[in] RightIndex  �������ִ���cExpress�еĽ����±�
 * @return ���ز�����������
 * @code
 * @endcode
 */



KindType Poland::AbstractOperand(char * cExpress, int LeftIndex,int RightIndex){

    int iLeftSpace = LeftIndex  - 1;  
    int iRightSpace = RightIndex + 1; 

    /*
     *  ���������˿����пո񣬽����������˵Ŀո��޳�
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
     * ���������Ȳ�����308����ʹ��double���ͱ�ʾ��������BigDouble����ʾ
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
         *  �������ϸ��⣬Ҫ���c�ı������淶��ͬ�����֡��»��ߡ���ĸ�����ַ�����ʹ����
         */
        if(this->IsValidVariableName(cOperandStr)){

            int iIndex = -1;
            iIndex = this->SearchConstVariable(cOperandStr);

            /*
             *  ���ǳ������洢���ͺ��±꼴��
             */
            if (iIndex>= 0 && iIndex < CONSTVARIABLEMOST)
            {
                IndexVector.push_back(iIndex);
                KindTypeVector.push_back(ConstVariable);


                return ConstVariable;

            }else{
                iIndex = this->SearchVariable(cOperandStr);
                /*
                 *  �Զ���������������±������
                 */
                if (iIndex >= 0 && iIndex < static_cast<int>(VariableVector.size()))
                {
                    IndexVector.push_back(iIndex);
                    KindTypeVector.push_back(VariableVector.at(iIndex).kind);

                    return VariableVector.at(iIndex).kind;
                }else{
                    /*
                     * δ����������洢������VariableVector�У��������±�����ͣ�
                     * ʹ��ʱֻ��������ڵȺŵ���࣬���ҶԱ��������������ƣ�32���ַ�����
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
                         *  TODO: ��ӶԱ�������ʱ�Ĵ�����
                         */
                    }

                }
            }

        }else if (this->NumberTypeCheck(cOperandStr) != 'n')
        {
            /*
             *  ���������Ǳ������������֣������ֽ�����double���洢��DoubleVector�������±������
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
         *  �����������ȳ���308���п�����BigDouble����ֵ����ȡ����ֵ�����洢
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
 * @brief    ��ֵ���Ƽ�⺯��
 * @param[in] cOperandStr    ����ѧ���ʽ����ȡ���ַ�����ʽ����ֵ
 * @retval  n ������ֵ
 * @retval  h ʮ������ 0xǰ׺
 * @retval  H ʮ������ h��׺
 * @retval  d ʮ���� ��ǰ׺���׺
 * @retval  D ʮ���� d��׺
 * @retval  o �˽��� o��׺
 * @retval  b ������ b��׺
 * @note   ǰ׺����׺д����дСд��֧��
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
     *  �����б�
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
 * @brief    ��ֵ���Ƽ�⺯��
 * @param[in] sOperandStr    ����ѧ���ʽ����ȡ���ַ�����ʽ����ֵ
 * @retval  n ������ֵ
 * @retval  h ʮ������ 0xǰ׺
 * @retval  H ʮ������ h��׺
 * @retval  d ʮ���� ��ǰ׺���׺
 * @retval  D ʮ���� d��׺
 * @retval  o �˽��� o��׺
 * @retval  b ������ b��׺
 * @note   ǰ׺����׺д����дСд��֧��
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
 * @brief    �������Ϸ���⺯��
 * @details  �Ϸ��ı�����Ӧ�����֡���ĸ���»��߹��ɣ���Ҫ������ĸ����ʹ����
 * @param[in] cOperandStr    ����ѧ���ʽ����ȡ���ַ�����ʽ�ı�����
 * @retval  false ���ǺϷ�������
 * @retval  true  �Ϸ�������
 * @code
 * char cOperandStr[] = "_validName1213";
 * Poland poland;
 * bool valid  = Poland.IsValidVariableName(cOperandStr);
 * @endcode
 */
bool Poland::IsValidVariableName(char* cOperandStr){

    /*
     *  ��������ֻ�ܳ������֡���ĸ���»��ߣ��������ַ�����ʹ����
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
 * @brief    �Զ����Ժ���
 * @details  ʹ��test.txt�д洢�Ĵ������ʽ�����в��ԣ���߲���Ч��
 * @code
 * @endcode
 */

void Poland::Test(string sFileName){
     /*
     *  �Զ����Գ���
     *  ʹ��test.txt�д洢�Ĵ������ʽ�����в��ԣ���߲���Ч��
     *  TODO�� ��Ӷ��ļ�����֧�֣�ʹ���û����Ա�д�ű����Զ�����
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
 * @brief    ����׺���ʽת��Ϊ��׺���ʽ
 * @note     ��ʾת����û�иı���ʽ����Ԫ�صĴ洢λ�ã����Ǹı���������������vector<int>IndexVector
 * ��vector<KindType>KindTypeVector��ɣ������±�����Ϳ��Ի�ȡ�û���Ԫ�ء��ı�֮������������
 * vector<int>PostfixIndexVector �� vector<KindType>PostfixKindTypeVector
 */
void Poland::Infix2Postfix(){
    /*
     *  ����׺ʽת��Ϊ��׺ʽ���ı�����vector�б�ǵ�˳��
     *  ������������ŵ���׺���ʽ��vector�У���������������ڶ�ջ�е��������ջ�������׺���ʽ��vector��
     *  ֱ���������ȼ����ڵ����Լ��������ʱֹͣ�������Լ���ջ
     *  ��������� =�Ǵ���������㣬ֻ�ܽ����ȼ������Լ����������ջ
     *  ��Ŀ������ͺ���Ҳ��ֻ�ܽ������Լ����������ջ
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

        }else if (kind == LeftParen)                                    /*  �����ţ���ջ                 */
        {

            IndexStack.push(iIndex);
            KindTypeStack.push(kind);
        }else if (kind ==  RightParen)
        {
            /*
             *  �����ţ���ƥ��������֮ǰ��Ԫ��ȫ����ջ
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
            *  ���ڶ�ջ�е��������ջ�������׺���ʽ��vector,ֱ���������ȼ����ڵ����Լ��������ʱֹͣ��
            *  �����Լ���ջ
            *  ��������� =�Ǵ���������㣬ֻ�ܽ����ȼ������Լ����������ջ
            *  ��Ŀ������ͺ���Ҳ��ֻ�ܽ������Լ����������ջ
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
                 *  ��Ŀ������ͺ������ڵ��߼�����С�ڵ��߼�ʱ��ֻҪ����С�ڻ��ߵ��ڵ�������ͻ�ʹǰ��ĳ�ջ��
                 *  ���ǵ��ǵ��߼��ĵ�Ŀ�����ʱ���������ϸ��С�ڲ���ʹǰ��ĳ�ջ
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
        }else if (kind == EndExpression)                                /*  ���ʽ��β������ն�ջ       */
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
 * @brief    ��ӡ����ֳɱ�����������������Ȼ���Ԫ�أ�������׺ʽת��Ϊ��׺ʽ
 * �ı��ʽ�����ڲ鿴��Ա����Infix2Postfix()�Ľ��
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
 * @brief    ������֮�����ѧ���ʽ
 * @note
 * @code
 * @endcode
 */
void Poland::SaveBreakedExpress(){
    /*
     *  ����ֳɻ���Ԫ�صı��ʽ���뵽��ʷ��¼�У�ÿ�����ʽʹ�ñ��ʽ��β���ֺ����ָ�
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
 * @brief    ��ӡ�û�������ʽ����ʷ��¼
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
 * @brief    ��ӡ�洢������Array������vector�еı��ʽ
 * @param[in] IndexVector    ���ʽ����Ԫ����������������е��±�
 * @param[in] KindTypeVector ���ʽ����Ԫ��Ԫ�ص�����
 * @note   ��֪ĳ����Ԫ�ص����ͺ��±꣬���Ե���Ӧ�������������ȡ����Ԫ��
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
 * @brief    ��׺���ʽ��ֵ����
 * @param[in] PostfixIndexVector    ���ʽ����Ԫ����������������е��±�
 * @param[in] PostfixKindTypeVector ���ʽ����Ԫ�ص�����
 * @param[out] AnsVariable  ������������
 * @retval  false ��ֵ�ɹ�
 * @retval  true  ��ֵʧ��
 * @note    ��ֵ��������أ�����Ǹ�ֵ���Ͱѽ���ŵ�����ֵ�ı���������Ͱ�ֵ�ŵ�������Ϊ"ans"�ı�����
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
     *  ����ÿ�ж������ʽ�������
     *  �������ʽ��β���ֺţ���Ϊ������ϣ���ʼ��һ�����ʽ�ļ���
     */
    while (!PostfixIndexVector.empty())
    {
            /*
             *  ���ڸ�ֵ�ͼ�����ϴ󣬽���ֵ���ʽ�ͼ�����ʽ�ֿ�����
             *  �����ʽ�����ظ�ֵ�Ĳ��� ��a = b =2 �򷵻�2
             */
            ExpressType = this->ExpressTypeCheck(this->IndexVector,this->KindTypeVector);
            if (ExpressType == -1)
            {
                cout<<"Some thing error in assign expression"<<endl;
                return false;
            }else if (ExpressType == 0)                                 /*  ��ֵ���ʽ��������ֵ���     */
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
 * @brief    ��׺���ʽ��ֵ����
 * @param[out] AnsVariable  ������������
 * @retval  false ��ֵ�ɹ�
 * @retval  true  ��ֵʧ��
 * @note    ��ֵ��������أ�����Ǹ�ֵ���Ͱѽ���ŵ�����ֵ�ı���������Ͱ�ֵ�ŵ�������Ϊ"ans"�ı�����
 *
 * @code
 * @endcode
 */
bool Poland::Postfix2Number(Variable& AnsVariable){

    return this->Postfix2Number(PostfixIndexVector ,PostfixKindTypeVector,AnsVariable);

}


/**
 * @ingroup  MemberFunction_Poland
 * @brief    ���ʽ�����ж�
 * @details  �жϱ��ʽ�Ǹ�ֵ������ֵ��䣬���Ǹ�ֵ��䣬�򷵻ظ�ֵ�Ĳ���
 * @param[in] IndexVector    ���ʽ����Ԫ����������������е��±�
 * @param[in] KindTypeVector ���ʽ����Ԫ�ص�����
 * @return   ����-1��ʾ����ֵ��䣬���ڵ���0���ʾ��ֵ�Ĳ������� "a =  b = 2" �򷵻�2
 * @note
 * @code
 * @endcode
 */
int Poland::ExpressTypeCheck(vector<int> & IndexVector ,vector<KindType>& KindTypeVector){
    int iIndex;
    KindType kind;
    int iPriorIndex;
    KindType PriorKind;
    int ExpressType = 0;                                                /*  ��ֵ���Ĳ�����-1��ʾ����   */
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
                *  ͳ�ƵȺŵĸ�������Զ��ظ�ֵ���
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
 * @brief    ��ֵ���ʽ��ֵ����
 * @param[in] PostfixIndexVector     ���ʽ����Ԫ����������������е��±�
 * @param[in] PostfixKindTypeVector  ���ʽ����Ԫ�ص�����
 * @param[in] iStartIndex            ��PostfixIndexVector��PostfixKindTypeVector��ȡԪ�صĿ�ʼ�±꣬��ӿ�
 * ʼ�±���Ϊ����Ӧÿ�ж�����ʽ�����
 * @param[out] AnsVariable ���õ��õķ�ʽ�����������AnsVariable����
 * @retval  false ��ֵʧ��
 * @retval  true  ��ֵ�ɹ�
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
    if (!BigDoubleExist)                                                /*  �����ڴ���ֵ��ʹ��double���� */
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
    }else{                                                              /*  ����BigDouble,����ֵ����     */

        return this->EvaluateExpressBigDouble(PostfixIndexVector,
                                              PostfixKindTypeVector,
                                              iStartIndex,
                                              AnsVariable);

    }
    return false;
}

 /**
 * @ingroup  MemberFunction_Poland
 * @brief    ��ֵ���ʽ��ֵ��������Ӧ�����ڴ���ֵ�����
 * @param[in] PostfixIndexVector     ���ʽ����Ԫ����������������е��±�
 * @param[in] PostfixKindTypeVector  ���ʽ����Ԫ�ص�����
 * @param[in] iStartIndex            ��PostfixIndexVector��PostfixKindTypeVector��ȡԪ�صĿ�ʼ�±꣬��ӿ�
 * ʼ�±���Ϊ����Ӧÿ�ж�����ʽ�����
 * @param[out] AnsVariable ���õ��õķ�ʽ�����������AnsVariable����
 * @retval  false ��ֵʧ��
 * @retval  true  ��ֵ�ɹ�
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
     *  ��������������洢����ջ��
     *  ������������ߺ���������ջ�еĲ�����ȡ�������㣬�ٽ������������ջ
     *  �������ʽ��β������������
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
            if (!strcmp(cName,"="))                                     /*  =��ֵ ����ͨ��Ԫ�������ͬ   */
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
            }else if (!strcmp(cName,","))                               /*  ���Ų��ô���                 */
            {

            }else {
                if (AnsDoubleStack.size() >= 2)                         /*  �Ӷ�ջ��ȡ������������       */
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
            if (!strcmp("pow",cName))                                   /*  pow�൱�ڶ�Ԫ����            */
            {
                if (AnsDoubleStack.size() >= 2)                         /*  �Ӷ�ջ��ȡ������������       */
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
            }else{                                                      /*  һԪ����                     */
                if (!AnsDoubleStack.empty())                            /*  �Ӷ�ջ��ȡ��һ��������       */
                {
                    RightOperand = AnsDoubleStack.top();
                    AnsDoubleStack.pop();
                }else {
                    /*return false;*/
                    return 0;
                }
                /*
                 *  ���к������㣬���������������ջ
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
        }else if (kind == UserDefineBinOprator)                         /*  �û��Զ����Ԫ�����         */
        {
            if (AnsDoubleStack.size() >= 2)                             /*  �Ӷ�ջ��ȡ������������       */
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
             *  �½�һ��Poland���󣬽��Զ���������ı��ʽ����һ�����ʽ������
             *  ���������������ջ
             */
            Poland TempPoland;
            /* �����Զ�������� */
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

        }else if (kind == UserDefineUnayOprator)                        /*  �û��Զ���һԪ�����         */
        {
            if (!AnsDoubleStack.empty())                                /*  �Ӷ�ջ��ȡ��һ��������       */
            {
                RightOperand = AnsDoubleStack.top();
                AnsDoubleStack.pop();
            }else {
               /* return false;*/
                return 0;
            }
            /*
             *  �½�һ��Poland���󣬽��Զ���������ı��ʽ����һ�����ʽ������
             *  ���������������ջ
             */
            Poland TempPoland;
            /* �����Զ�������� */
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
        }else if (kind == EndExpression)                                /*  ���ʽ��β����������       */
        {
            if (AnsDoubleStack.size() == 1)
            {
                AnsVariable.dNumber = AnsDoubleStack.top();
                AnsVariable.kind = OperandDouble;
                /*TODO: �˴�Ӧ���ж������Ƿ��������������л���BigDouble��������*/
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
 * @brief    ��ֵ���ʽ��ֵ��������Ӧ���ڴ���ֵ�����
 * @param[in] PostfixIndexVector     ���ʽ����Ԫ����������������е��±�
 * @param[in] PostfixKindTypeVector  ���ʽ����Ԫ�ص�����
 * @param[in] iStartIndex            ��PostfixIndexVector��PostfixKindTypeVector��ȡԪ�صĿ�ʼ�±꣬��ӿ�
 * ʼ�±���Ϊ����Ӧÿ�ж�����ʽ�����
 * @param[out] AnsVariable ���õ��õķ�ʽ�����������AnsVariable����
 * @retval  false ��ֵʧ��
 * @retval  true  ��ֵ�ɹ�
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
     *  ��������������洢����ջ��
     *  ������������ߺ���������ջ�еĲ�����ȡ�������㣬�ٽ������������ջ
     *  �������ʽ��β������������
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
        }else if (kind == BinOperator)                                  /*  ˫Ŀ�����                   */
        {
            strcpy(cName,OperatorDic[iIndex].cName);
            if (!strcmp(cName,"="))                                     /*  ����=��ֱ�ӷ����Ҳ��������*/
            {
                if (AnsBigDoubleStack.size() == 1)
                {
                    AnsVariable.bdNumber = AnsBigDoubleStack.top();
                    AnsVariable.kind = OperandBigDouble;
                    return true;
                }else{
                    return false;
                }
            }else if (!strcmp(cName,","))                               /*  ���Ų����д���              */
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
                 *  �������������ջ��ȡ������������������������������ջ
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
        }else if (kind == UnayOprator)                                  /*  ��Ŀ�����                   */
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
        }else if (kind == Function)                                     /*  ����                         */
        {
            strcpy(cName,OperatorDic[iIndex].cName);
            if (!strcmp("pow",cName))                                   /* pow�൱�ڶ�Ԫ��������������   */
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
            }else{                                                      /*  һԪ����                     */
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
        }else if (kind == UserDefineBinOprator)                         /*  �Զ����Ԫ�����             */
        {
            /*
             *  �½�һ��Poland���󣬽��Զ���������ı��ʽ����һ�����ʽ������
             *  ���������������ջ
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
            /* �����Զ�������� */
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

        }else if (kind == UserDefineUnayOprator)                        /*  �Զ���һԪ�����             */
        {
            if (!AnsBigDoubleStack.empty())
            {
                RightOperand = AnsBigDoubleStack.top();
                AnsBigDoubleStack.pop();
            }else {
                return false;
            }

            /*
             *  �½�һ��Poland���󣬽��Զ���������ı��ʽ����һ�����ʽ������
             *  ���������������ջ
             */
            Poland TempPoland;
            /* �����Զ�������� */
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
                                                                        /*  ���ʽ��β��������������   */
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
 * @brief    �׳���ֵ����
 * @param[in] n   �׳˵Ĳ���
 * @return  �׳˵�������
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
 * @brief    �׳���ֵ����,��Ӧ�ڴ���ֵ�Ľ׳˼���
 * @param[in] nBigDouble   �׳˵Ĳ���
 * @return  �׳˵�������
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
 * @brief    �������溯��
 * @details  ���������浽VariableVector��
 * @param[in] AnsVariable     ��Ҫ�����Variable���͵ı���
 * @param[in] VariableVector  Variable���ͱ���Ҫ�����λ��
 * @retval  false VariableVector�в�����ͬ����������AnsVariable����VariableVector
 * @retval  true  VariableVector�д���ͬ������������Ӧͬ��������ֵ�������޸�
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
 * @brief    ��ӡ����ϵͳ���е�����ͺ���
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
 * @brief    �������ͼ��
 * @param[in] sExpress    ������ʽʽ������ѧ���ʽ
 * @retval  0 ��������
 * @retval  ���ڵ���0 ����������CommandDic[]�е��±�
 * @note    �����б������Ϲ淶
 * @code
 * string sExpress = string("help clear");
 * Poland poland;
 * int ComandType = Poland.CommandType(sExpress);
 * @endcode
 */
int Poland::CommandType(string sExpress){
    /*
     *  ������ʽ�� Command ParameterList ����
     *  ����ȡ����Command��Ȼ����������б�����������б��Լ������б�֮����ÿո�ָ�
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
 * @brief    ����ִ�к���
 * @param[in] sExpress    ������ʽ
 * @param[in] CommandType    ��������
 * @retval  0 �������Ϸ�������ִ�г���
 * @retval  �ɹ�ִ������
 * @note    �����б������Ϲ淶
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
     *  ͳ������Ĳ�������
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
     *  �������б��еĲ�����ȡ����������strArray
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
    case 0:                                                             /*  help�����ӡ����           */

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
    case 1:                                                             /*  whos����鿴���б���       */
        if (iParameterCount == 0)
        {
            this->PrintVariable();
        }else{
            cout<<"Too many parameters for '"<<CommandDic[CommandType].cName<<"'"<<endl;
        }
      break;
    case 2:                                                             /*  who����鿴ָ������        */

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
    case 3:                                                             /*  clc�������                */

        if (iParameterCount == 0)
        {
             system("CLS");
        }else{
           cout<<"Too many parameters for '"<<CommandDic[CommandType].cName<<"'"<<endl;
        }
         break;
    case 4:                                                             /*  clear����������          */
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
    case 5:                                                             /*  operator����Զ��������   */
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
    case 6:                                                             /*  copyright����鿴��Ȩ����  */

        if (iParameterCount == 0)
        {
             cout<<"Guangzhou ZHIYUAN electronics Co.,LTD."<<endl;
             cout<<"All rights reserved."<<endl;
        }else{
            cout<<"Too many parameters for '"<<CommandDic[CommandType].cName<<"'"<<endl;
        }
        break;
    case 7:                                                             /*  history����鿴��ʷ��¼    */
        if (iParameterCount == 0)
        {
             this->PrintHistoryExpress();
        }else{
            cout<<"Too many parameters for '"<<CommandDic[CommandType].cName<<"'"<<endl;
        }
       break;
    case 8:                                                             /*  exit����˳�����           */
        if (iParameterCount == 0)
        {
            exit(1);
        }else{
            cout<<"Too many parameters for '"<<CommandDic[CommandType].cName<<"'"<<endl;
        }
        break;
    case 9:                                                             /* test����Զ����Գ���        */
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
    case 10:                                                            /*  ��ӡ������ֵ�               */
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
 * @ingroup  MemberFunction_Poland �������ʽ�ĳ�Ա����
 * @brief    ���Զ�����������̶���ʽ�洢
 * @details  ���Զ���������洢������UserOperatorVector��
 * @param[in] sExprsss  �Զ�����������ַ���
 * @retval  false �Զ��������ʧ��
 * @retval  true  �Զ���������ɹ�
 * @note    Ӧ�������¸�ʽ��"operator operatorName format Express priority
 * ���磺operator @ a@b a+b 4"
 * @code
 * @endcode
 */
bool Poland::SaveOperator(string sExprsss, string * sParameter){
    char * cLeftOperand;
    char * cRightOperand;
    int iOperatorIndex = 0;
    char cOperator;

    UserDefineOprator uOperator;

    if (sParameter[0].size() == 1)                                      /*  �Զ��������Ҫ�󳤶�Ϊ1      */
    {
        cOperator = sParameter[0].at(0);
         if (this->SearchOperator(cOperator) >= 0){
             /*
              *  ������������Ѿ����ڣ��������û��Զ���
              */
             cout<<"Operator '"<<sParameter[0]<<"' is a common operator, cannot redefine it"<<endl;
         }else {
             /*
              *  δ�����������ֱ�Ӷ���һ������
              *  ����������û��Ѷ��壬�����¶���
              */
             uOperator.cOperator = cOperator;
             /*
              *  �����ȡ�����б��еĲ���
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

             if (!OperatorExist)                                        /*   Format���������������      */
             {
                 cout<<"Invalid expression to define Operator"<<endl;
             }else {                                                    /*   �и����������ȡ���ұ�����  */
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

                 if (iOperatorIndex == 0 )                              /*  ��Ŀ��������������         */
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
                      *  ��Ŀ��������������
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

                 }else {                                                /*  ˫Ŀ�����                   */

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
                  *  ���ȼ���ȡ
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
              *  �鿴��������Ƿ����
              *  �����������¶��壬��û��������µ��Զ��������
              */

             int iIndex = this->SearchUserDefineOperator(cOperator);
             if (iIndex >= 0)                                           /*  �Ѷ������������޸Ķ���     */
             {
                 cout<<"Operator '"<<cOperator<<"' is redefined"<<endl;
                 UserOperatorVector.at(iIndex) = uOperator;
             }else {                                                    /*  δ��������������Ӷ���     */
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
 * @brief    �����������������
 * @param[in] cOperator    ��������char�͵������
 * @return �����������ĳ�����������±꣬������ʱ�򷵻�-1
 * @note   �����Զ���������������Զ���������޶�ֻ����һ���ַ������Բ�����char��
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
 * @brief    �Զ���������������������
 * @param[in] cOperator    ��������char�͵������
 * @return �������������Զ�����������±꣬������ʱ�򷵻�-1
 * @note   �����Զ���������������Զ���������޶�ֻ����һ���ַ������Բ�����char��
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
