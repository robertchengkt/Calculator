/*
 * Guangzhou ZHIYUAN electronics Co.,LTD.
 * http://www.embedtools.com
 * All rights reserved
 *
 * @file    BigDouble.cpp
 * @brief   BigDouble Class ����ֵ�ļ���
 *
 * Change Logs:
 * Date         Author      Notes
 * 2012-08-10   Libing   Second Edtion
 */

#include "BigDouble.h"
#include<cmath>
#include <iostream>
#include<cstring>
#include<cstdlib>
#include<cstdio>
#include<string>
using namespace std;



/**
 * @defgroup  Constructors_BigDouble
 * ����BigDouble��ĳ�ʼ��,�˰汾�Ĵ���ֵ���ÿ�ѧ����������ʽ�Դ���ֵ���м��㣬����ʹ��double�������ͣ�ָ��
 * ʹ��int�������ͣ������ϴ˷������Ա�ʾ���޴�����ݣ������ٶ���ĺ�С��������Чλ����double��ͬ���ʺ��ڽ�
 * �д���ֵ���㵫����Чλ��Ҫ���Ǻܸߵ��������һ�汾������Чλ�����޵����⡣
 */

/**
 * @defgroup  Destructors_BigDouble
 * ����BigDouble�������
 */

/**
 * @defgroup  MemberFunction_BigDouble
 * BigDouble��ĳ�Ա���������д���ֵ�ļ���
 */



/**
 * @ingroup  Constructors_BigDouble
 * @brief    BigDouble���޲εĹ��캯��
 *
 * @code
 * BigDouble bigDouble;
 * @endcode
 */
BigDouble::BigDouble(void)
{
    this->dBaseNumber = 0.0;
    this->iIndexNumber = 0;
}

/**
 * @ingroup  Constructors_BigDouble
 * @brief    BigDouble���вεĹ��캯��
 *
 * @code
 * BigDouble bigDouble(1.0);
 * @endcode
 */
BigDouble::BigDouble(double dNumber){
    int i;
    double dTemp;
    if (dNumber == 0.0)                                                 /*  ֵΪ0���������ָ����Ϊ0     */
    {
        this->iIndexNumber = 0;
        this->dBaseNumber = dNumber;
    }else                                                               /*  ȷ��ָ��                     */
    {
        for (i = -310;i< 310;i++)
        {
            dTemp = dNumber/(pow(10.0,i));
            if ( abs(dTemp) >= 1.0 && abs(dTemp) < 10.0)
            {
                this->iIndexNumber = i;
                this->dBaseNumber = dTemp;
                break;
            }
        }
    }
}

/**
 * @ingroup  Constructors_BigDouble
 * @brief    BigDouble���вεĹ��캯��
 *
 * @code
 * int iIndexNumber = 5;
 * double dBaseNumber = 1.2;
 * BigDouble bigDouble( dBaseNumber, iIndexNumber);
 * @endcode
 */
BigDouble::BigDouble(double dBaseNumber,int iIndexNumber){
    this->dBaseNumber  = dBaseNumber;
    this->iIndexNumber = iIndexNumber;
    this->Normalization();
}

/**
 * @ingroup  Constructors_BigDouble
 * @brief    BigDouble���вεĹ��캯��
 *
 * @code
 * bool bNumber = true;
 * BigDouble bigDouble(bNumber);
 * @endcode
 */
BigDouble::BigDouble(bool bNumber){
    BigDouble(static_cast<double>(bNumber));
}

/**
 * @ingroup  Constructors_BigDouble
 * @brief    BigDouble���вεĹ��캯��
 *
 * @code
 * int iNumber = 2;
 * BigDouble bigDouble(iNumber);
 * @endcode
 */
BigDouble::BigDouble(int iNumber){
    //BigDouble(static_cast<double>(iNumber));
    this->dBaseNumber  = static_cast<double>(iNumber);
    this->iIndexNumber = 0;
    this->Normalization();
}

/**
 * @ingroup  Constructors_BigDouble
 * @brief    BigDouble���вεĹ��캯��
 * @param[in] NumberStr    �ַ������͵�����
 *
 * @code
 * char cNumberStr[] = "11111111111111111.1111111111111";
 * BigDouble bigDouble(cNumberStr);
 * @endcode
 */
BigDouble::BigDouble(char *NumberStr){
    int iLength = strlen(NumberStr);
    double dTemp;
    int i;
    double dNumber;

    if (iLength <= 307)
    {
        //string str = string(NumberStr);
        dNumber =  atof(NumberStr);

        if (dNumber == 0.0)
        {
            this->iIndexNumber = 0;
            this->dBaseNumber = dNumber;
        }else
        {
            for (i = -310;i< 310;i++)
            {
                dTemp = dNumber/(pow(10.0,i));

                if ( abs(dTemp) >= 1.0 && abs(dTemp) < 10.0)
                {
                    this->iIndexNumber = i;
                    this->dBaseNumber = dTemp;
                    break;
                }
            }
        }
    }else if (iLength > 307)
    {
        /*
         *  �ַ�������������ֱ��ת��Ϊ���֣���ֳ�ָ���ͻ������洢
         */
       int iDotindex = iLength;
       int iFirstNumberNonZeroIndex = iLength;
       char cSubStr[310];

       /*
        *  ����С�����λ�ú��׸�����ֵ��λ��
        */
       for (int i = iLength -1; i >= 0;i--)
       {
           if (NumberStr[i] == '.')
           {
               iDotindex = i;
           }else if (NumberStr[i] > '0' && NumberStr[i] <= '9')
           {
               iFirstNumberNonZeroIndex = i;
           }
       }

       if ( iFirstNumberNonZeroIndex > iDotindex)
       {
           this->iIndexNumber = iDotindex-iFirstNumberNonZeroIndex;
       }else{
           this->iIndexNumber = iDotindex-iFirstNumberNonZeroIndex -1;
       }
      strncpy(cSubStr,NumberStr,307);
      cSubStr[307] = '\0';


      //string str = string(cSubStr);
      double dNumber = atof(cSubStr);
      /*
       *  ת������׼��ʽ��������С����ǰ��ֻ��һλ
       */

      double dTemp;
      if (dNumber == 0.0)
      {
          this->iIndexNumber = 0;
          this->dBaseNumber = 0.0;
      }else
      {
          for (int i = -310;i< 310;i++)
          {
              dTemp = dNumber/(pow(10.0,i));
              if ( abs(dTemp) >= 1.0 && abs(dTemp) < 10.0)
              {
                  this->dBaseNumber = dTemp;
                  break;
              }
          }
      }

    }
}


/**
 * @ingroup  Constructors_BigDouble
 * @brief    BigDouble���вεĹ��캯��
 * @param[in] NumberStrStr    string���͵�����
 *
 * @code
 * char NumberStrStr = string("11111111111111111.1111111111111");
 * BigDouble bigDouble(NumberStrStr);
 * @endcode
 */
BigDouble::BigDouble(string NumberStrStr){

    /*
     *  string����ת��Ϊchar* ����
     */
    int iLength = NumberStrStr.size();
    char *NumberStr = new char [iLength+1];
    for (int i = 0 ; i< iLength;i++)
    {
        NumberStr[i] = NumberStrStr.at(i);
    }
    NumberStr[iLength] = '\0';
    double dTemp;
    int i;
    double dNumber;

    if (iLength <= 307)                                                 /*  ����<307,����ֱ��ת��Ϊdouble*/
    {
        //string str = string(NumberStr);
        dNumber =  atof(NumberStr);

        if (dNumber == 0.0)
        {
            this->iIndexNumber = 0;
            this->dBaseNumber = dNumber;
        }else
        {
            for (i = -310;i< 310;i++)
            {
                dTemp = dNumber/(pow(10.0,i));

                if ( abs(dTemp) >= 1.0 && abs(dTemp) < 10.0)
                {
                    this->iIndexNumber = i;
                    this->dBaseNumber = dTemp;
                    break;
                }
            }
        }
    }else if (iLength > 307)                                            /*  ����>307��ֱ�ӻ���BigDouble  */
    {

        int iDotindex = iLength;
        int iFirstNumberNonZeroIndex = iLength;
        char cSubStr[310];

        for (int i = iLength -1; i >= 0;i--)
        {
            if (NumberStr[i] == '.')
            {
                iDotindex = i;
            }else if (NumberStr[i] > '0' && NumberStr[i] <= '9')
            {
                iFirstNumberNonZeroIndex = i;
            }
        }

        if ( iFirstNumberNonZeroIndex > iDotindex)
        {
            this->iIndexNumber = iDotindex-iFirstNumberNonZeroIndex;
        }else{
            this->iIndexNumber = iDotindex-iFirstNumberNonZeroIndex -1;
        }
        strncpy(cSubStr,NumberStr,307);
        cSubStr[307] = '\0';


        //string str = string(cSubStr);
        double dNumber = atof(cSubStr);

        /*
         *  ת������׼��ʽ
         */
        double dTemp;
        if (dNumber == 0.0)
        {
            this->iIndexNumber = 0;
            this->dBaseNumber = 0.0;
        }else
        {
            for (int i = -310;i< 310;i++)
            {
                dTemp = dNumber/(pow(10.0,i));
                if ( abs(dTemp) >= 1.0 && abs(dTemp) < 10.0)
                {
                    this->dBaseNumber = dTemp;
                    break;
                }
            }
        }

    }
    delete [] NumberStr;
}

/**
 * @ingroup  Destructors_BigDouble
 * @brief    BigDouble������
 *
 * @code
 * @endcode
 */
BigDouble::~BigDouble(void)
{
}


/**
 * @ingroup  MemberFunction_BigDouble
 * @brief    ��ȡstring��ʽ��BigDouble��������ʾ
 * @return   ����string��ʽ��BigDouble
 *
 * @code
 * @endcode
 */
string BigDouble::getBigDoubleStr(){

    /*
     *  ���ַ�������ʽ��ʾһ���ַ���������+e+ָ����ʽ ��1.22e+10000
     */
    string BigDoubleStr;
    if (this->dBaseNumber == 0.0)
    {
        return string("0.0");
    }else {
        char cStrBaseNumber[1000];
        char cStrIndexNumber[1000];
        sprintf(cStrBaseNumber,"%f",this->dBaseNumber);
        sprintf(cStrIndexNumber,"%d",this->iIndexNumber);
        if (this->iIndexNumber < 0)
        {
            BigDoubleStr = string(cStrBaseNumber)+string("e")+string(cStrIndexNumber);
        }else if (this->iIndexNumber >= 0)
        {
            BigDoubleStr = string(cStrBaseNumber)+string("e+")+string(cStrIndexNumber);
        }
        return BigDoubleStr;
    }
}

/**
 * @ingroup  MemberFunction_BigDouble
 * @brief    ��ȡBigDouble�Ļ���
 * @return ����BigDouble�Ļ���
 *
 * @code
 * @endcode
 */
double BigDouble::getBaseNumber(){
    return this->dBaseNumber;
}

/**
 * @ingroup  MemberFunction_BigDouble
 * @brief    ��ȡBigDouble��ָ��
 * @return ����BigDouble��ָ��
 *
 * @code
 * @endcode
 */
int BigDouble::getIndexNumber(){
    return this->iIndexNumber;
}

/**
 * @ingroup  MemberFunction_BigDouble
 * @brief    BigDouble�ı�׼��
 * @details  ������ΪС����ǰ��һλ��ʽ
 *
 * @code
 * @endcode
 */
void BigDouble::Normalization(){
    int i;
    double dTemp;
    if (this->dBaseNumber == 0.0)
    {
        this->iIndexNumber = 0;
        this->dBaseNumber = 0.0;
    }else
    {
        for (i = -310;i< 310;i++)
        {
            dTemp = this->dBaseNumber/(pow(10.0,i));
            if ( abs(dTemp) >= 1.0 && abs(dTemp) < 10.0)
            {
                this->iIndexNumber = i + this->iIndexNumber;
                this->dBaseNumber = dTemp;
                break;
            }
        }
    }

}

/*  ���������  */


/**
 * @ingroup  MemberFunction_BigDouble
 * @brief    �� + �������������
 * @return   ����BigDouble���͵�������
 */
BigDouble BigDouble::operator + (BigDouble & OtherBigDouble){
    BigDouble bigDoubleTemp(0.0);
    int iDifference = this->iIndexNumber-OtherBigDouble.iIndexNumber;
    if (iDifference > 300 )
    {
        bigDoubleTemp.dBaseNumber  = this->dBaseNumber;
        bigDoubleTemp.iIndexNumber = this->iIndexNumber;
    }else if (iDifference < -300)
    {
        bigDoubleTemp.dBaseNumber  = OtherBigDouble.dBaseNumber;
        bigDoubleTemp.iIndexNumber = OtherBigDouble.iIndexNumber;
    }else{
        bigDoubleTemp.dBaseNumber  = this->dBaseNumber + OtherBigDouble.dBaseNumber/(pow(10.0,iDifference));
        bigDoubleTemp.iIndexNumber = this->iIndexNumber;
    }
    bigDoubleTemp.Normalization();
    return bigDoubleTemp;
}
/**
 * @ingroup  MemberFunction_BigDouble
 * @brief    �� - �������������
 * @return   ����BigDouble���͵�������
 */
BigDouble BigDouble::operator - (BigDouble & OtherBigDouble){
    BigDouble bigDoubleTemp(0.0);
    int iDifference = this->iIndexNumber-OtherBigDouble.iIndexNumber;
    if (iDifference > 300 )
    {
        bigDoubleTemp.dBaseNumber  = this->dBaseNumber;
        bigDoubleTemp.iIndexNumber = this->iIndexNumber;
    }else if (iDifference < -300)
    {
        bigDoubleTemp.dBaseNumber  = OtherBigDouble.dBaseNumber;
        bigDoubleTemp.iIndexNumber = OtherBigDouble.iIndexNumber;
    }else{
        bigDoubleTemp.dBaseNumber = this->dBaseNumber - OtherBigDouble.dBaseNumber/(pow(10.0,iDifference));
        bigDoubleTemp.iIndexNumber = this->iIndexNumber;
    }

    bigDoubleTemp.Normalization();
    return bigDoubleTemp;
}
/**
 * @ingroup  MemberFunction_BigDouble
 * @brief    �� * �������������
 * @return   ����BigDouble���͵�������
 */
BigDouble BigDouble::operator * (BigDouble & OtherBigDouble){
    BigDouble bigDoubleTemp(0.0);
    bigDoubleTemp.dBaseNumber  = this->dBaseNumber*OtherBigDouble.dBaseNumber;
    bigDoubleTemp.iIndexNumber = this->iIndexNumber+OtherBigDouble.iIndexNumber;
    bigDoubleTemp.Normalization();
    return bigDoubleTemp;
}

/**
 * @ingroup  MemberFunction_BigDouble
 * @brief    �� / �������������
 * @return   ����BigDouble���͵�������
 */
BigDouble BigDouble::operator / (BigDouble & OtherBigDouble){
    BigDouble bigDoubleTemp(0.0);
    bigDoubleTemp.dBaseNumber  = this->dBaseNumber / OtherBigDouble.dBaseNumber;
    bigDoubleTemp.iIndexNumber = this->iIndexNumber - OtherBigDouble.iIndexNumber;
    bigDoubleTemp.Normalization();
    return bigDoubleTemp;
}


/**
 * @ingroup  MemberFunction_BigDouble
 * @brief    �� > �������������
 * @return   ����bool���͵�������
 */
bool BigDouble::operator > (BigDouble & OtherBigDouble){
    if (this->dBaseNumber > 0 && OtherBigDouble.dBaseNumber <= 0)
    {
        return true;
    }else if (this->dBaseNumber > 0 && OtherBigDouble.dBaseNumber >0)
    {
        if (this->iIndexNumber == OtherBigDouble.iIndexNumber)
        {
            return (this->dBaseNumber > OtherBigDouble.dBaseNumber);
        }else {
            return (this->iIndexNumber > OtherBigDouble.iIndexNumber);
        }
    }else if (this->dBaseNumber < 0 && OtherBigDouble.dBaseNumber >= 0)
    {
        return false;
    }else if (this->dBaseNumber < 0 && OtherBigDouble.dBaseNumber < 0)
    {
        if (this->iIndexNumber == OtherBigDouble.iIndexNumber)
        {
            return (this->dBaseNumber < OtherBigDouble.dBaseNumber);
        }else {
            return (this->iIndexNumber < OtherBigDouble.iIndexNumber);
        }
    }else if (this->dBaseNumber == 0 && OtherBigDouble.dBaseNumber > 0)
    {
        return false;
    }else if (this->dBaseNumber == 0 && OtherBigDouble.dBaseNumber <= 0)
    {
        return true;
    }else {
        return false;
    }
}

/**
 * @ingroup  MemberFunction_BigDouble
 * @brief    �� < �������������
 * @return   ����bool���͵�������
 */
bool BigDouble::operator < (BigDouble & OtherBigDouble){
    return (OtherBigDouble > *this);
}



/**
 * @ingroup  MemberFunction_BigDouble
 * @brief    �� >= �������������
 * @return   ����bool���͵�������
 */
bool BigDouble::operator >= (BigDouble & OtherBigDouble){
    return !(OtherBigDouble > *this);
}

/**
 * @ingroup  MemberFunction_BigDouble
 * @brief    �� <= �������������
 * @return   ����bool���͵�������
 */
bool BigDouble::operator <= (BigDouble & OtherBigDouble){
    return !( *this > OtherBigDouble);
}

/**
 * @ingroup  MemberFunction_BigDouble
 * @brief    �� == �������������
 * @return   ����bool���͵�������
 */
bool BigDouble::operator == (BigDouble & OtherBigDouble){
    if (this->dBaseNumber == OtherBigDouble.dBaseNumber
        && this->iIndexNumber == OtherBigDouble.iIndexNumber)
    {
        return true;
    }else{
        return false;
    }
}


/**
 * @ingroup  MemberFunction_BigDouble
 * @brief    �� != �������������
 * @return   ����bool���͵�������
 */
bool BigDouble::operator != (BigDouble & OtherBigDouble){
    return !( *this == OtherBigDouble);
}

/**
 * @ingroup  MemberFunction_BigDouble
 * @brief    �� && �������������
 * @return   ����bool���͵�������
 */
bool BigDouble::operator  && (BigDouble & OtherBigDouble){

    return (this->dBaseNumber && OtherBigDouble.dBaseNumber);
}

/**
 * @ingroup  MemberFunction_BigDouble
 * @brief    �� || �������������
 * @return   ����bool���͵�������
 */
bool BigDouble::operator  || (BigDouble & OtherBigDouble){
     return (this->dBaseNumber || OtherBigDouble.dBaseNumber);
}

/**
 * @ingroup  MemberFunction_BigDouble
 * @brief    �� % �������������
 * @return   ����BigDouble���͵�������
 */

BigDouble BigDouble::operator  % (BigDouble & OtherBigDouble){
    BigDouble TempBigDouble(0.0);
    /*
     *  TODO�� �����ģ�Ĵ���
     */
    return TempBigDouble;
}


/**
 * @ingroup  MemberFunction_BigDouble
 * @brief    �� %~ �������������
 * @return   ����BigDouble���͵�������
 */
BigDouble BigDouble::operator  ~ (){
    /*
     *  TODO: ���~�Ĵ���
     */
    return BigDouble(0.0);
}

/**
 * @ingroup  MemberFunction_BigDouble
 * @brief    BigDouble���͵�ȡ��������
 * @return   ����BigDouble���͵�������
 */
 BigDouble BigDouble::BigLog10(BigDouble OtherBigDouble){
     double dBaseNumber = OtherBigDouble.dBaseNumber;
     int iIndexNumber = OtherBigDouble.iIndexNumber;
     double dNumber = log10(dBaseNumber) + iIndexNumber;
     return BigDouble(dNumber);
 }

 /**
 * @ingroup  MemberFunction_BigDouble
 * @brief    BigDouble���͵�ȡ��������
 * @return   ����BigDouble���͵�������
 */
 BigDouble BigDouble::BigLog(BigDouble OtherBigDouble){
     int iIndexNumber = OtherBigDouble.iIndexNumber;
     double dBaseNumber = OtherBigDouble.dBaseNumber;
     double dNumber = log(dBaseNumber) + iIndexNumber*log(10.0);
     return BigDouble(dNumber);
 }

 /**
 * @ingroup  MemberFunction_BigDouble
 * @brief    BigDouble���͵�ȡ����ֵ����
 * @return   ����BigDouble���͵�������
 */
 BigDouble BigDouble::BigAbs(BigDouble OtherBigDouble){
     int iIndexNumber = OtherBigDouble.iIndexNumber;
     double dBaseNumber = OtherBigDouble.dBaseNumber;
     return BigDouble(abs(dBaseNumber),iIndexNumber);
 }

 /**
 * @ingroup  MemberFunction_BigDouble
 * @brief    BigDouble���͵Ŀ�������
 * @return   ����BigDouble���͵�������
 */
 BigDouble BigDouble::BigSqrt(BigDouble OtherBigDouble){
     int iIndexNumber = OtherBigDouble.iIndexNumber;
     double dBaseNumber = OtherBigDouble.dBaseNumber;

     if (iIndexNumber % 2 != 0)
     {
         iIndexNumber -=1;
         dBaseNumber *= 10.0;
     }
     return BigDouble(sqrt(dBaseNumber),iIndexNumber/2);
 }




 /**
 * @ingroup  MemberFunction_BigDouble
 * @brief    �� + �������������
 * @return   ����BigDouble���͵�������
 */
BigDouble BigDouble::operator + (double & OtherDouble){
    BigDouble OtherBigDouble (OtherDouble);
    BigDouble bigDoubleTemp(0.0);
    int iDifference = this->iIndexNumber- OtherBigDouble.iIndexNumber;
    if (iDifference > 300 )
    {
        bigDoubleTemp.dBaseNumber  = this->dBaseNumber;
        bigDoubleTemp.iIndexNumber = this->iIndexNumber;
    }else if (iDifference < -300)
    {
        bigDoubleTemp.dBaseNumber  = OtherBigDouble.dBaseNumber;
        bigDoubleTemp.iIndexNumber = OtherBigDouble.iIndexNumber;
    }else{
        bigDoubleTemp.dBaseNumber  = this->dBaseNumber + OtherBigDouble.dBaseNumber/(pow(10.0,iDifference));
        bigDoubleTemp.iIndexNumber = this->iIndexNumber;
    }
    bigDoubleTemp.Normalization();
    return bigDoubleTemp;
}

/**
 * @ingroup  MemberFunction_BigDouble
 * @brief    �� - �������������
 * @return   ����BigDouble���͵�������
 */
BigDouble BigDouble::operator - (double & OtherDouble){
    BigDouble OtherBigDouble(OtherDouble);
    BigDouble bigDoubleTemp(0.0);
    int iDifference = this->iIndexNumber-OtherBigDouble.iIndexNumber;
    if (iDifference > 300 )
    {
        bigDoubleTemp.dBaseNumber  = this->dBaseNumber;
        bigDoubleTemp.iIndexNumber = this->iIndexNumber;
    }else if (iDifference < -300)
    {
        bigDoubleTemp.dBaseNumber  = OtherBigDouble.dBaseNumber;
        bigDoubleTemp.iIndexNumber = OtherBigDouble.iIndexNumber;
    }else{
        bigDoubleTemp.dBaseNumber = this->dBaseNumber - OtherBigDouble.dBaseNumber/(pow(10.0,iDifference));
        bigDoubleTemp.iIndexNumber = this->iIndexNumber;
    }

    bigDoubleTemp.Normalization();
    return bigDoubleTemp;
}

/**
 * @ingroup  MemberFunction_BigDouble
 * @brief    �� * �������������
 * @return   ����BigDouble���͵�������
 */
BigDouble BigDouble::operator * (double & OtherDouble){
    BigDouble bigDoubleTemp(0.0);
    bigDoubleTemp.dBaseNumber  = this->dBaseNumber*OtherDouble;
    bigDoubleTemp.iIndexNumber = this->iIndexNumber;
    bigDoubleTemp.Normalization();
    return bigDoubleTemp;
}

/**
 * @ingroup  MemberFunction_BigDouble
 * @brief    �� / �������������
 * @return   ����BigDouble���͵�������
 */
BigDouble BigDouble::operator / (double & OtherDouble){
    BigDouble bigDoubleTemp(0.0);
    bigDoubleTemp.dBaseNumber  = this->dBaseNumber/OtherDouble;
    bigDoubleTemp.iIndexNumber = this->iIndexNumber;
    bigDoubleTemp.Normalization();
    return bigDoubleTemp;
}


/**
 * @ingroup  MemberFunction_BigDouble
 * @brief    �� > �������������
 * @return   ����bool���͵�������
 */
bool BigDouble::operator > (double & OtherDouble){
    BigDouble TempBigDouble(OtherDouble);
    return (*this > TempBigDouble);
}

/**
 * @ingroup  MemberFunction_BigDouble
 * @brief    �� < �������������
 * @return   ����bool���͵�������
 */
bool BigDouble::operator < (double & OtherDouble){
    BigDouble TempBigDouble(OtherDouble);
    return (*this < TempBigDouble);
}

/**
 * @ingroup  MemberFunction_BigDouble
 * @brief    �� >= �������������
 * @return   ����bool���͵�������
 */
bool BigDouble::operator >= (double & OtherDouble){
    BigDouble TempBigDouble(OtherDouble);
    return (*this >= TempBigDouble);
}

/**
 * @ingroup  MemberFunction_BigDouble
 * @brief    �� <= �������������
 * @return   ����bool���͵�������
 */
bool BigDouble::operator <= (double & OtherDouble){
    BigDouble TempBigDouble(OtherDouble);
    return (*this > TempBigDouble);
}

/**
 * @ingroup  MemberFunction_BigDouble
 * @brief    �� == �������������
 * @return   ����bool���͵�������
 */
bool BigDouble::operator == (double & OtherDouble){
    BigDouble TempBigDouble(OtherDouble);
    return (*this == TempBigDouble);
}

/**
 * @ingroup  MemberFunction_BigDouble
 * @brief    �� != �������������
 * @return   ����bool���͵�������
 */
bool BigDouble::operator != (double & OtherDouble){
    BigDouble TempBigDouble(OtherDouble);
    return (*this != TempBigDouble);
}


/**
 * @ingroup  MemberFunction_BigDouble
 * @brief    �� && �������������
 * @return   ����bool���͵�������
 */
bool BigDouble::operator  && (double & OtherDouble){
    return (this->dBaseNumber && OtherDouble);
}

/**
 * @ingroup  MemberFunction_BigDouble
 * @brief    �� || �������������
 * @return   ����bool���͵�������
 */
bool BigDouble::operator  || (double & OtherDouble){
    return (this->dBaseNumber || OtherDouble);
}

/**
 * @ingroup  MemberFunction_BigDouble
 * @brief    �� % �������������
 * @return   ����BigDouble���͵�������
 */
BigDouble BigDouble::operator  % (double &){
    BigDouble TempBigDouble(0.0);
    /*
     *  TODO�� �����ģ�Ĵ���
     */
    return TempBigDouble;
  }

/* end of file */
