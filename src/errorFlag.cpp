#include "../includes/errorFlag.h"

errorFlag::errorFlag()
{
    error = false;
    errorFile = "";
    errorFunction = "";
    errorLineNumber = 0;
}

void errorFlag::setErrorInfo(bool val, string file, string func, int line )
{
    error = val;
    errorFile = file;
    errorFunction = func;
    errorLineNumber = line;
}

void errorFlag::printErrorInfo()
{
    cout << "ERROR" << endl;
    cout << "At: " << errorFile << endl;
    cout << errorFunction << endl << errorLineNumber << endl;
}

bool errorFlag::errorExist()
{
    if (error)
        printErrorInfo();

    return error;
}