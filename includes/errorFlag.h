#ifndef ERRORFLAG_H_
#define ERRORFLAG_H_

#include <iostream>

using namespace std;

class errorFlag
{
    public:
        errorFlag();

        void setErrorInfo(bool, string, string, int );
        void printErrorInfo();
        bool errorExist();
    
    private:
        bool error;
        string errorFile;
        string errorFunction;
        int errorLineNumber;

};

#endif