#ifndef SQLCONNECTOR_H_
#define SQLCONNECTOR_H_

#include <cppconn/connection.h>
#include <cppconn/driver.h>
#include <vector>

using namespace std;

vector<string> getInfoSQL();

void connectToDB();

#endif