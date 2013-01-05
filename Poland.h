/*
 * Guangzhou ZHIYUAN electronics Co.,LTD.
 * http://www.embedtools.com
 * All rights reserved
 *
 * @file    Poland.h
 * @brief   Poland Expression Class 波兰表达式的计算
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
 * @brief    表达式的基本元素的所有可能类型
 * @details  将表达式拆分成基本元素，然后转化为后缀表达式，逐步计算
 */
enum KindType{
    OperandDouble,                                                      /**<  double型操作数             */
    OperandBigDouble,                                                   /**<  BigDouble型操作数          */

    ConstVariable,                                                      /**<  常量                       */
    VariableDouble,                                                     /**<  double型变量               */
    VariableBigDouble,                                                  /**<  BigDouble型变量            */
    Undefine,                                                           /**<  未定义变量                 */

    UnayOprator,                                                        /**<  一元运算符                 */
    BinOperator,                                                        /**<  二元运算符                 */
    UserDefineUnayOprator,                                              /**<  用户自定义一元运算符       */
    UserDefineBinOprator,                                               /**<  用户自定义二元运算符       */

    LeftParen,                                                          /**<  左括号                     */
    RightParen,                                                         /**<  右括号                     */
    Function,                                                           /**<  函数                       */

    EndExpression,                                                      /**<  表达式结尾符               */

};


/**
 * @brief    用户自定义运算符类型
 * @details  存储用户自定义运算符，运算规则使用一个string类型的表达式来表示
 */
struct UserDefineOprator{
        char  cOperator;                                                /**<  运算符                     */
    KindType  kind;                                                     /**<  用户自定义运算符的类型     */
         int  iPriority;                                                /**<  用户自定义运算符的优先级   */
        char  cLeftOperand[33];                                         /**<  用户自定义运算符的左操作数 */
        char  cRightOperand[33];                                        /**<  用户自定义运算符的右操作数 */
      string  sExpress;                                                 /**<  用户自定义运算符的表达式   */
};

/**
 * @brief    普通运算符类型
 * @details  存储常用运算符和函数等
 */
struct Operator{
        char cName[6];                                                  /**<  运算符名字                 */
    KindType kind;                                                      /**<  运算符类型                 */
         int iPriority;                                                 /**<  运算符优先级               */
};
/**
 * @brief  变量类型
 * @details  存储变量名和变量值，以及类型，其中变量值可能是double或者BigDouble
 */
struct Variable{
        char  cName[33];                                                /**<  变量名                     */
    KindType  kind;                                                     /**<  变量类型                   */
      double  dNumber;                                                  /**<  double型变量的值           */
   BigDouble  bdNumber;                                                 /**<  BigDouble型变量的值        */
};

/**
 * @brief    命令类型
 * @details  存储常用命令系统常用命令以及用法
 */
struct Command{
      char cName[10];                                                   /**<  命令名                     */
    string sCommandDetail;                                              /**<  命令含义                   */
    string sExample;                                                    /**<  命令用法示例               */
};


/**
 * @brief    自定义波兰表达式的类Poland
 * @details  在类中定义Poland表达式对象的的属性和方法，并对类的属性和方法进行封装
 * 计算的基本思想是将表达式拆分成常量、变量、运算符、函数、用户自定义运算符、数字、括号、表达式结尾符，其中
 * 运算符和用户自定义运算符又分为单目运算符合双目运算符，数字又分为double型和BigDouble型
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
          int SearchVariable      (char *cVariableStr);                 /*  搜索变量，返回下标           */
          int SearchConstVariable (char * cVariableStr);                /*  搜索常量，返回下标           */
         char NumberTypeCheck    (char * cOperandStr);
         char NumberTypeCheck    (string sOperandStr);
         bool IsValidVariableName(char * cOperandStr);
         bool SaveOperator(string sExprsss, string *sParameter);         /*  解析运算符，并存储          */
          int SearchOperator(char cOperator);
          int SearchUserDefineOperator(char cOperator);
          bool doubleOverflowTest(double dNumber);

          /*
           *  引用调用，计算时简单，不用进行指针解引用
           *  同时避免对对内存的拷贝，提高运算速度
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
     *  表达式拆分成基本元素之后，分别存储在如下位置
     */
    vector<UserDefineOprator>UserOperatorVector;                        /**<  存储用户自定义运算符       */
    vector<Variable>VariableVector;                                     /**<  存储用户定义变量           */
    vector<double>DoubleVector;                                         /**<  存储double型操作数         */
    vector<BigDouble>BigDoubleVector;                                   /**<  存储BigDouble型操作数      */
    /*
     *  中缀表达式的存储向量
     */
    vector<int>IndexVector;                                             /**<  存储表达式元素的下标       */
    vector<KindType>KindTypeVector;                                     /**<  存储表达式元素的类型       */

    /*
     *  表达式的历史记录的存储向量
     */
    vector<int>HistoryIndexVector;                                      /**<  存储历史表达式元素的下标   */
    vector<KindType>HistoryKindTypeVector;                              /**<  存储历史表达式元素的类型   */
    


    /*
     *  后缀表达式的存储向量
     */
    vector<int>PostfixIndexVector;                                      /**<  存储后缀表达式元素的下标   */
    vector<KindType>PostfixKindTypeVector;                              /**<  存储后缀表达式元素的类型   */
};

#endif
/* end of file */
