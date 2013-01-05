/*
 * Guangzhou ZHIYUAN electronics Co.,LTD.
 * http://www.embedtools.com
 * All rights reserved
 *
 * @file    Poland.h
 * @brief   Poland Expression Class �������ʽ�ļ���
 *
 * Change Logs:
 * Date         Author      Notes
 * 2012-08-10   Libing   Second Edtion
 */




#ifndef _POLAND_H
#define _POLAND_H
#include "BigDouble.h"
#include <iostream>
#include<vector>
#include <stack>
#include <fstream>
#include<cstring>
#include<stdio.h>
#include<stdlib.h>

using namespace std;

/**
 * @brief    ���ʽ�Ļ���Ԫ�ص����п�������
 * @details  �����ʽ��ֳɻ���Ԫ�أ�Ȼ��ת��Ϊ��׺���ʽ���𲽼���
 */
enum KindType{
    OperandDouble,                                                      /**<  double�Ͳ�����             */
    OperandBigDouble,                                                   /**<  BigDouble�Ͳ�����          */

    ConstVariable,                                                      /**<  ����                       */
    VariableDouble,                                                     /**<  double�ͱ���               */
    VariableBigDouble,                                                  /**<  BigDouble�ͱ���            */
    Undefine,                                                           /**<  δ�������                 */

    UnayOprator,                                                        /**<  һԪ�����                 */
    BinOperator,                                                        /**<  ��Ԫ�����                 */
    UserDefineUnayOprator,                                              /**<  �û��Զ���һԪ�����       */
    UserDefineBinOprator,                                               /**<  �û��Զ����Ԫ�����       */

    LeftParen,                                                          /**<  ������                     */
    RightParen,                                                         /**<  ������                     */
    Function,                                                           /**<  ����                       */

    EndExpression,                                                      /**<  ���ʽ��β��               */

};


/**
 * @brief    �û��Զ������������
 * @details  �洢�û��Զ�����������������ʹ��һ��string���͵ı��ʽ����ʾ
 */
struct UserDefineOprator{
        char  cOperator;                                                /**<  �����                     */
    KindType  kind;                                                     /**<  �û��Զ��������������     */
         int  iPriority;                                                /**<  �û��Զ�������������ȼ�   */
        char  cLeftOperand[33];                                         /**<  �û��Զ����������������� */
        char  cRightOperand[33];                                        /**<  �û��Զ�����������Ҳ����� */
      string  sExpress;                                                 /**<  �û��Զ���������ı��ʽ   */
};

/**
 * @brief    ��ͨ���������
 * @details  �洢����������ͺ�����
 */
struct Operator{
        char cName[6];                                                  /**<  ���������                 */
    KindType kind;                                                      /**<  ���������                 */
         int iPriority;                                                 /**<  ��������ȼ�               */
};
/**
 * @brief  ��������
 * @details  �洢�������ͱ���ֵ���Լ����ͣ����б���ֵ������double����BigDouble
 */
struct Variable{
        char  cName[33];                                                /**<  ������                     */
    KindType  kind;                                                     /**<  ��������                   */
      double  dNumber;                                                  /**<  double�ͱ�����ֵ           */
   BigDouble  bdNumber;                                                 /**<  BigDouble�ͱ�����ֵ        */
};

/**
 * @brief    ��������
 * @details  �洢��������ϵͳ���������Լ��÷�
 */
struct Command{
      char cName[10];                                                   /**<  ������                     */
    string sCommandDetail;                                              /**<  �����                   */
    string sExample;                                                    /**<  �����÷�ʾ��               */
};


/**
 * @brief    �Զ��岨�����ʽ����Poland
 * @details  �����ж���Poland���ʽ����ĵ����Ժͷ���������������Ժͷ������з�װ
 * ����Ļ���˼���ǽ����ʽ��ֳɳ�������������������������û��Զ�������������֡����š����ʽ��β��������
 * ��������û��Զ���������ַ�Ϊ��Ŀ�������˫Ŀ������������ַ�Ϊdouble�ͺ�BigDouble��
 */
class Poland
{
public:
    Poland(void);
   ~Poland(void);

    int  CommandType   (string sExpress);
    bool ExecuteCommand(string sExpress, int CommandType);

    void BreakExpress(string sExpress);
    void Infix2Postfix();
    bool Postfix2Number(Variable& AnsVariable);




    void PrintDictionary();
    void PrintBreakedExpress();
    void PrintPostfixExpress();
    void PrintOperator();
    void PrintVariable();
    void PrintHistoryExpress();
    void SaveBreakedExpress();
    void Help();
    /*void Test();*/
    void Test(string sFileName);
    

private:


       double factorial(unsigned int n);
    BigDouble factorial(BigDouble nBigDouble);
    BigDouble factorialBig(unsigned int n);
       double OperandStr2Number(char * cOperandStr);
       double OperandStr2Number(string sOperandStr);
     KindType AbstractOperand(char * cExpress, int LeftIndex,int RightIndex);
         bool strnmcmp(const char* cSubStr,char* cStr,int StartIndex);
         void my_strnmcpy(char* cSubStr,char* cStr,int iStartIndex,int iStopIndex);
         void Initialize();
         void MarkNegative(char * cExpress);
          int ExpressTypeCheck(vector<int> & IndexVector ,vector<KindType>& KindTypeVector);
         void PrintExpress(vector<int>& IndexVector ,vector<KindType>& KindTypeVector);
         bool SaveVariable(  Variable & AnsVariable ,vector<Variable>& VariableVector);
          int SearchVariable      (char *cVariableStr);                 /*  ���������������±�           */
          int SearchConstVariable (char * cVariableStr);                /*  ���������������±�           */
         char NumberTypeCheck    (char * cOperandStr);
         char NumberTypeCheck    (string sOperandStr);
         bool IsValidVariableName(char * cOperandStr);
         bool SaveOperator(string sExprsss, string *sParameter);         /*  ��������������洢          */
          int SearchOperator(char cOperator);
          int SearchUserDefineOperator(char cOperator);
          bool doubleOverflowTest(double dNumber);

          /*
           *  ���õ��ã�����ʱ�򵥣����ý���ָ�������
           *  ͬʱ����Զ��ڴ�Ŀ�������������ٶ�
           */
        bool Postfix2Number(     vector<int>& PostfixIndexVector ,
                            vector<KindType>& PostfixKindTypeVector,
                            Variable& AnsVariable);

        bool EvaluateExpress(     vector<int>& IndexVector,
                             vector<KindType>& KindTypeVector,
                                        int   iStartIndex,
                                   Variable & AnsVariable );

       /*bool EvaluateExpressDouble(     vector<int>& PostfixIndexVector,
                                  vector<KindType>& PostfixKindTypeVector,
                                              int   iStartIndex,
                                         Variable & AnsVariable );*/
        int EvaluateExpressDouble(     vector<int>& PostfixIndexVector,
            vector<KindType>& PostfixKindTypeVector,
            int   iStartIndex,
            Variable & AnsVariable );

       bool EvaluateExpressBigDouble(    vector<int>& PostfixIndexVector,
                                    vector<KindType>& PostfixKindTypeVector,
                                                int   iStartIndex,
                                           Variable & AnsVariable );


    /*
     *  ���ʽ��ֳɻ���Ԫ��֮�󣬷ֱ�洢������λ��
     */
    vector<UserDefineOprator>UserOperatorVector;                        /**<  �洢�û��Զ��������       */
    vector<Variable>VariableVector;                                     /**<  �洢�û��������           */
    vector<double>DoubleVector;                                         /**<  �洢double�Ͳ�����         */
    vector<BigDouble>BigDoubleVector;                                   /**<  �洢BigDouble�Ͳ�����      */
    /*
     *  ��׺���ʽ�Ĵ洢����
     */
    vector<int>IndexVector;                                             /**<  �洢���ʽԪ�ص��±�       */
    vector<KindType>KindTypeVector;                                     /**<  �洢���ʽԪ�ص�����       */

    /*
     *  ���ʽ����ʷ��¼�Ĵ洢����
     */
    vector<int>HistoryIndexVector;                                      /**<  �洢��ʷ���ʽԪ�ص��±�   */
    vector<KindType>HistoryKindTypeVector;                              /**<  �洢��ʷ���ʽԪ�ص�����   */
    


    /*
     *  ��׺���ʽ�Ĵ洢����
     */
    vector<int>PostfixIndexVector;                                      /**<  �洢��׺���ʽԪ�ص��±�   */
    vector<KindType>PostfixKindTypeVector;                              /**<  �洢��׺���ʽԪ�ص�����   */
};

#endif
/* end of file */
