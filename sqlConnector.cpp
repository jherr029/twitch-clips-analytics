#include "headers/sqlConnector.h"
#include <fstream>

#include <cppconn/statement.h>
#include <cppconn/resultset.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>

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
    try 
    {

        Driver * driver;
        Connection * conn;
        driver = get_driver_instance();

        vector<string> info = getInfoSQL();

        conn = driver->connect(info[0], info[1], info[2]);

        conn->setSchema("lsf_test");

        Statement * stmt = conn->createStatement();
        // ResultSet * res = stmt->executeQuery("select * from channels");
        PreparedStatement * pstmt;
        // ResultSet * res = stmt->executeQuery("select * from lsf.channels");

        // while ( res->next() )
        // {
        //     cout << res->getString(1) << " " << res->getString(2) << " " << 
        //             res->getString(3) << " " << res->getString(4) << endl;

        //     cout << res->getString("channel");
        // }


        string temp = "cali";
        stmt->execute("create table if not exist " + temp + "(id varchar(21))");
        pstmt = conn->prepareStatement("insert into " + temp + "(id) values (?)");
        pstmt->setString(1, "hii");
        pstmt->executeUpdate();
        
        delete conn;
        delete stmt;
        // delete res;
        delete pstmt;

    }

    catch (SQLException &e )
    {
        cout << "ERROR sqlexecption in" << __FILE__ << endl;
        cout << "Line: " << __LINE__ << endl;
        cout << "ERROR # " << e.what() << endl;
        cout << "ERROR code: " << e.getErrorCode();
        cout << "SQL state: " << e.getSQLState();

    }
    

    // cout << "hello" << endl;

}