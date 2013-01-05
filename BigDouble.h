/*
 * Guangzhou ZHIYUAN electronics Co.,LTD.
 * http://www.embedtools.com
 * All rights reserved
 *
 * @file    BigDouble.h
 * @brief   BigDouble Class ����ֵ�ļ���
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
 * @brief    �Զ������ֵ��BigDouble
 * @details  �����ж������ֵ�����Ժͷ���������������Ժͷ������з�װ 
 */
class BigDouble
{
public:
    BigDouble(void);
   ~BigDouble(void);

    /*  
     *  ���캯��  
     */
    BigDouble(  bool  bNumber);
    BigDouble(   int  iNumber);
    BigDouble(double  dNumber);
    BigDouble(string  NumberStrStr);
    BigDouble(  char* NumberStr);
    BigDouble(double  dBaseNumber , int iIndexNumber);
   

    string getBigDoubleStr();                                           /*  ��ȡ�ַ�����ʽ��BigDouble     */
    double getBaseNumber();                                             /*  ���߻���                      */
       int getIndexNumber();                                            /*  ��ȡָ��                      */
      void Normalization();                                             /*  ��׼��                       */

    /*
     *  ���ú������ڵ�BigDouble�ļ��㷽��
     */
    BigDouble BigLog10(BigDouble OtherBigDouble);
    BigDouble BigLog  (BigDouble OtherBigDouble);
    BigDouble BigAbs  (BigDouble OtherBigDouble);
    BigDouble BigSqrt (BigDouble OtherBigDouble);

    /*  
     *  ������������أ������ı�����������ȼ�  
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
     *  TODO: ��Ӹ������������� 
     */


     
private:
    double dBaseNumber;                                                 /**<  ����BigDouble��Ļ���      */
       int iIndexNumber;                                                /**<  ����BigDouble���ָ��      */ 
};

#endif
/* end of file */
