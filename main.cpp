#include "BigDouble.h"
#include "Poland.h"
#include <iostream>
#include <string>
using namespace std;

int main(){

    Poland poland;
    poland.Help();
    string sExpress;
    Variable AnsVariable = {"ans",VariableDouble,0.0,BigDouble(0.0)};
    while (1)
    {
        cout<<">>";                                                     /*  输入提示符                   */
        getline(cin,sExpress);                                           
        if (sExpress.empty())
        {
            continue;
        }
        int CommandType = poland.CommandType(sExpress);
        if (CommandType >= 0)                                           /*  命令                         */
        {
            poland.ExecuteCommand(sExpress,CommandType);
        }else {                                                         /*  运算表达式                   */
            poland.BreakExpress(sExpress);
            poland.Infix2Postfix();
            poland.Postfix2Number(AnsVariable);
        }
       
    }
    return 0;
}


/* end of file */