/*
 * Guangzhou ZHIYUAN electronics Co.,LTD.
 * http://www.embedtools.com
 * All rights reserved
 *
 * @file    BigDouble.h
 * @brief   BigDouble Class 大数值的计算
 *
 * Change Logs:
 * Date         Author      Notes
 * 2012-08-10   Libing   Second Edtion
 */

#ifndef _BIGDOUBLE_H
#define _BIGDOUBLE_H
#include <string>
using namespace std;


/**
 * @brief    自定义大数值类BigDouble
 * @details  在类中定义大数值的属性和方法，并对类的属性和方法进行封装 
 */
class BigDouble
{
public:
    BigDouble(void);
   ~BigDouble(void);

    /*  
     *  构造函数  
     */
    BigDouble(  bool  bNumber);
    BigDouble(   int  iNumber);
    BigDouble(double  dNumber);
    BigDouble(string  NumberStrStr);
    BigDouble(  char* NumberStr);
    BigDouble(double  dBaseNumber , int iIndexNumber);
   

    string getBigDoubleStr();                                           /*  获取字符串形式的BigDouble     */
    double getBaseNumber();                                             /*  或者基数                      */
       int getIndexNumber();                                            /*  获取指数                      */
      void Normalization();                                             /*  标准化                       */

    /*
     *  常用函数关于的BigDouble的计算方法
     */
    BigDouble BigLog10(BigDouble OtherBigDouble);
    BigDouble BigLog  (BigDouble OtherBigDouble);
    BigDouble BigAbs  (BigDouble OtherBigDouble);
    BigDouble BigSqrt (BigDouble OtherBigDouble);

    /*  
     *  常用运算符重载，并不改变运算符的优先级  
     */
    BigDouble operator +  (BigDouble &);
    BigDouble operator -  (BigDouble &);
    BigDouble operator *  (BigDouble &);
    BigDouble operator /  (BigDouble &);
         bool operator >  (BigDouble &);
         bool operator <  (BigDouble &);
         bool operator >= (BigDouble &);
         bool operator <= (BigDouble &);
         bool operator != (BigDouble &);
         bool operator == (BigDouble &);
         bool operator && (BigDouble &);
         bool operator || (BigDouble &);
    BigDouble operator %  (BigDouble &);
    BigDouble operator ~  ();

    BigDouble operator +  (double &);
    BigDouble operator -  (double &);
    BigDouble operator *  (double &);
    BigDouble operator /  (double &);
         bool operator >  (double &);
         bool operator <  (double &);
         bool operator >= (double &);
         bool operator <= (double &);
         bool operator != (double &);
         bool operator == (double &);
         bool operator && (double &);
         bool operator || (double &);
    BigDouble operator %  (double &);

    /*  
     *  TODO: 添加更多的运算符重载 
     */


     
private:
    double dBaseNumber;                                                 /**<  大数BigDouble类的基数      */
       int iIndexNumber;                                                /**<  大数BigDouble类的指数      */ 
};

#endif
/* end of file */
