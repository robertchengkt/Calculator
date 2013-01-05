/*
 * Guangzhou ZHIYUAN electronics Co.,LTD.
 * http://www.embedtools.com
 * All rights reserved
 *
 * @file    BigDouble.cpp
 * @brief   BigDouble Class 大数值的计算
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
 * 用于BigDouble类的初始化,此版本的大数值采用科学计数法的形式对大数值进行计算，基数使用double数据类型，指数
 * 使用int数据类型，理论上此方法可以表示无限大的数据，运算速度损耗很小，但是有效位数和double相同，适合于进
 * 行大数值运算但对有效位数要求不是很高的情况，下一版本会解决有效位数受限的问题。
 */

/**
 * @defgroup  Destructors_BigDouble
 * 用于BigDouble类的销毁
 */

/**
 * @defgroup  MemberFunction_BigDouble
 * BigDouble类的成员函数，进行大数值的计算
 */



/**
 * @ingroup  Constructors_BigDouble
 * @brief    BigDouble类无参的构造函数
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
 * @brief    BigDouble类有参的构造函数
 *
 * @code
 * BigDouble bigDouble(1.0);
 * @endcode
 */
BigDouble::BigDouble(double dNumber){
    int i;
    double dTemp;
    if (dNumber == 0.0)                                                 /*  值为0，这基数和指数均为0     */
    {
        this->iIndexNumber = 0;
        this->dBaseNumber = dNumber;
    }else                                                               /*  确定指数                     */
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
 * @brief    BigDouble类有参的构造函数
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
 * @brief    BigDouble类有参的构造函数
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
 * @brief    BigDouble类有参的构造函数
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
 * @brief    BigDouble类有参的构造函数
 * @param[in] NumberStr    字符串类型的数字
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
         *  字符串过长，不能直接转化为数字，拆分成指数和基数来存储
         */
       int iDotindex = iLength;
       int iFirstNumberNonZeroIndex = iLength;
       char cSubStr[310];

       /*
        *  搜索小数点的位置和首个非零值的位置
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
       *  转化到标准形式，基数的小数点前面只有一位
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
 * @brief    BigDouble类有参的构造函数
 * @param[in] NumberStrStr    string类型的数字
 *
 * @code
 * char NumberStrStr = string("11111111111111111.1111111111111");
 * BigDouble bigDouble(NumberStrStr);
 * @endcode
 */
BigDouble::BigDouble(string NumberStrStr){

    /*
     *  string类型转化为char* 类型
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

    if (iLength <= 307)                                                 /*  长度<307,可以直接转化为double*/
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
    }else if (iLength > 307)                                            /*  长度>307，直接化我BigDouble  */
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
         *  转化到标准形式
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
 * @brief    BigDouble类析构
 *
 * @code
 * @endcode
 */
BigDouble::~BigDouble(void)
{
}


/**
 * @ingroup  MemberFunction_BigDouble
 * @brief    获取string形式的BigDouble，用于显示
 * @return   返回string形式的BigDouble
 *
 * @code
 * @endcode
 */
string BigDouble::getBigDoubleStr(){

    /*
     *  用字符串的形式表示一个字符串，基数+e+指数形式 如1.22e+10000
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
 * @brief    获取BigDouble的基数
 * @return 返回BigDouble的基数
 *
 * @code
 * @endcode
 */
double BigDouble::getBaseNumber(){
    return this->dBaseNumber;
}

/**
 * @ingroup  MemberFunction_BigDouble
 * @brief    获取BigDouble的指数
 * @return 返回BigDouble的指数
 *
 * @code
 * @endcode
 */
int BigDouble::getIndexNumber(){
    return this->iIndexNumber;
}

/**
 * @ingroup  MemberFunction_BigDouble
 * @brief    BigDouble的标准化
 * @details  基数化为小数点前面一位形式
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

/*  运算符重载  */


/**
 * @ingroup  MemberFunction_BigDouble
 * @brief    对 + 运算符进行重载
 * @return   返回BigDouble类型的运算结果
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
 * @brief    对 - 运算符进行重载
 * @return   返回BigDouble类型的运算结果
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
 * @brief    对 * 运算符进行重载
 * @return   返回BigDouble类型的运算结果
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
 * @brief    对 / 运算符进行重载
 * @return   返回BigDouble类型的运算结果
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
 * @brief    对 > 运算符进行重载
 * @return   返回bool类型的运算结果
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
 * @brief    对 < 运算符进行重载
 * @return   返回bool类型的运算结果
 */
bool BigDouble::operator < (BigDouble & OtherBigDouble){
    return (OtherBigDouble > *this);
}



/**
 * @ingroup  MemberFunction_BigDouble
 * @brief    对 >= 运算符进行重载
 * @return   返回bool类型的运算结果
 */
bool BigDouble::operator >= (BigDouble & OtherBigDouble){
    return !(OtherBigDouble > *this);
}

/**
 * @ingroup  MemberFunction_BigDouble
 * @brief    对 <= 运算符进行重载
 * @return   返回bool类型的运算结果
 */
bool BigDouble::operator <= (BigDouble & OtherBigDouble){
    return !( *this > OtherBigDouble);
}

/**
 * @ingroup  MemberFunction_BigDouble
 * @brief    对 == 运算符进行重载
 * @return   返回bool类型的运算结果
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
 * @brief    对 != 运算符进行重载
 * @return   返回bool类型的运算结果
 */
bool BigDouble::operator != (BigDouble & OtherBigDouble){
    return !( *this == OtherBigDouble);
}

/**
 * @ingroup  MemberFunction_BigDouble
 * @brief    对 && 运算符进行重载
 * @return   返回bool类型的运算结果
 */
bool BigDouble::operator  && (BigDouble & OtherBigDouble){

    return (this->dBaseNumber && OtherBigDouble.dBaseNumber);
}

/**
 * @ingroup  MemberFunction_BigDouble
 * @brief    对 || 运算符进行重载
 * @return   返回bool类型的运算结果
 */
bool BigDouble::operator  || (BigDouble & OtherBigDouble){
     return (this->dBaseNumber || OtherBigDouble.dBaseNumber);
}

/**
 * @ingroup  MemberFunction_BigDouble
 * @brief    对 % 运算符进行重载
 * @return   返回BigDouble类型的运算结果
 */

BigDouble BigDouble::operator  % (BigDouble & OtherBigDouble){
    BigDouble TempBigDouble(0.0);
    /*
     *  TODO： 添加求模的代码
     */
    return TempBigDouble;
}


/**
 * @ingroup  MemberFunction_BigDouble
 * @brief    对 %~ 运算符进行重载
 * @return   返回BigDouble类型的运算结果
 */
BigDouble BigDouble::operator  ~ (){
    /*
     *  TODO: 添加~的代码
     */
    return BigDouble(0.0);
}

/**
 * @ingroup  MemberFunction_BigDouble
 * @brief    BigDouble类型的取对数函数
 * @return   返回BigDouble类型的运算结果
 */
 BigDouble BigDouble::BigLog10(BigDouble OtherBigDouble){
     double dBaseNumber = OtherBigDouble.dBaseNumber;
     int iIndexNumber = OtherBigDouble.iIndexNumber;
     double dNumber = log10(dBaseNumber) + iIndexNumber;
     return BigDouble(dNumber);
 }

 /**
 * @ingroup  MemberFunction_BigDouble
 * @brief    BigDouble类型的取对数函数
 * @return   返回BigDouble类型的运算结果
 */
 BigDouble BigDouble::BigLog(BigDouble OtherBigDouble){
     int iIndexNumber = OtherBigDouble.iIndexNumber;
     double dBaseNumber = OtherBigDouble.dBaseNumber;
     double dNumber = log(dBaseNumber) + iIndexNumber*log(10.0);
     return BigDouble(dNumber);
 }

 /**
 * @ingroup  MemberFunction_BigDouble
 * @brief    BigDouble类型的取绝对值函数
 * @return   返回BigDouble类型的运算结果
 */
 BigDouble BigDouble::BigAbs(BigDouble OtherBigDouble){
     int iIndexNumber = OtherBigDouble.iIndexNumber;
     double dBaseNumber = OtherBigDouble.dBaseNumber;
     return BigDouble(abs(dBaseNumber),iIndexNumber);
 }

 /**
 * @ingroup  MemberFunction_BigDouble
 * @brief    BigDouble类型的开方函数
 * @return   返回BigDouble类型的运算结果
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
 * @brief    对 + 运算符进行重载
 * @return   返回BigDouble类型的运算结果
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
 * @brief    对 - 运算符进行重载
 * @return   返回BigDouble类型的运算结果
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
 * @brief    对 * 运算符进行重载
 * @return   返回BigDouble类型的运算结果
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
 * @brief    对 / 运算符进行重载
 * @return   返回BigDouble类型的运算结果
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
 * @brief    对 > 运算符进行重载
 * @return   返回bool类型的运算结果
 */
bool BigDouble::operator > (double & OtherDouble){
    BigDouble TempBigDouble(OtherDouble);
    return (*this > TempBigDouble);
}

/**
 * @ingroup  MemberFunction_BigDouble
 * @brief    对 < 运算符进行重载
 * @return   返回bool类型的运算结果
 */
bool BigDouble::operator < (double & OtherDouble){
    BigDouble TempBigDouble(OtherDouble);
    return (*this < TempBigDouble);
}

/**
 * @ingroup  MemberFunction_BigDouble
 * @brief    对 >= 运算符进行重载
 * @return   返回bool类型的运算结果
 */
bool BigDouble::operator >= (double & OtherDouble){
    BigDouble TempBigDouble(OtherDouble);
    return (*this >= TempBigDouble);
}

/**
 * @ingroup  MemberFunction_BigDouble
 * @brief    对 <= 运算符进行重载
 * @return   返回bool类型的运算结果
 */
bool BigDouble::operator <= (double & OtherDouble){
    BigDouble TempBigDouble(OtherDouble);
    return (*this > TempBigDouble);
}

/**
 * @ingroup  MemberFunction_BigDouble
 * @brief    对 == 运算符进行重载
 * @return   返回bool类型的运算结果
 */
bool BigDouble::operator == (double & OtherDouble){
    BigDouble TempBigDouble(OtherDouble);
    return (*this == TempBigDouble);
}

/**
 * @ingroup  MemberFunction_BigDouble
 * @brief    对 != 运算符进行重载
 * @return   返回bool类型的运算结果
 */
bool BigDouble::operator != (double & OtherDouble){
    BigDouble TempBigDouble(OtherDouble);
    return (*this != TempBigDouble);
}


/**
 * @ingroup  MemberFunction_BigDouble
 * @brief    对 && 运算符进行重载
 * @return   返回bool类型的运算结果
 */
bool BigDouble::operator  && (double & OtherDouble){
    return (this->dBaseNumber && OtherDouble);
}

/**
 * @ingroup  MemberFunction_BigDouble
 * @brief    对 || 运算符进行重载
 * @return   返回bool类型的运算结果
 */
bool BigDouble::operator  || (double & OtherDouble){
    return (this->dBaseNumber || OtherDouble);
}

/**
 * @ingroup  MemberFunction_BigDouble
 * @brief    对 % 运算符进行重载
 * @return   返回BigDouble类型的运算结果
 */
BigDouble BigDouble::operator  % (double &){
    BigDouble TempBigDouble(0.0);
    /*
     *  TODO： 添加求模的代码
     */
    return TempBigDouble;
  }

/* end of file */
