#ifndef SQLCONNECTOR_H_
#define SQLCONNECTOR_H_

#include <cppconn/connection.h>
#include <cppconn/driver.h>
#include <vector>

// maybe make this into a class


using namespace std;

// vector<string> getInfoSQL();

void connectToDB();

#endif