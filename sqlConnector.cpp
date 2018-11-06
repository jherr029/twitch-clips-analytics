#include "headers/sqlConnector.h"
#include <fstream>

#include <cppconn/statement.h>
#include <cppconn/resultset.h>

using namespace sql;

vector<string> getInfoSQL()
{
    string line;
    vector<string> info;

    ifstream infoFile;
    infoFile.open("mysqlStuff");

    while ( getline( infoFile, line ) )
    {
        // cout << line << endl;
        info.push_back(line);
    }

    infoFile.close();

    return info;

}

void connectToDB()
{
    Driver * driver;
    Connection * conn;
    driver = get_driver_instance();

    vector<string> info = getInfoSQL();

    conn = driver->connect(info[0], info[1], info[2]);

    conn->setSchema("lsf");

    Statement * stmt = conn->createStatement();
    ResultSet * res = stmt->executeQuery("select * from channels");

    while ( res->next() )
    {
        cout << res->getString(1) << " " << res->getString(2) << " " << 
                res->getString(3) << " " << res->getString(4) << endl;
    }
    

    // cout << "hello" << endl;

}