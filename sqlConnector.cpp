#include "headers/sqlConnector.h"
#include <fstream>
#include <string>

using namespace sql;

string uniqueTable = "create table if not exists ";
string insert = "insert into ";
string channelParam = "(name, id, type) values (?, ?, ?)";

sqlConnector::sqlConnector()
{
    vector<string> info = getInfoSQL();

    try
    {
        driver = get_driver_instance();
        conn = driver->connect(info[0], info[1], info[2]);
        conn->setSchema("lsf_test");
        stmt = conn->createStatement();
    }

    catch ( SQLException & e)
    { printExceptionInfo(e); }

}

sqlConnector::~sqlConnector()
{
    // delete driver;
    delete conn;
    delete stmt;
    delete res;
    delete pstmt;
}

vector<string> sqlConnector::getInfoSQL()
{
    string line;
    vector<string> info;

    ifstream infoFile;
    infoFile.open("mysqlStuff");

    while ( getline( infoFile, line ) )
    {
        info.push_back(line);
    }

    infoFile.close();

    return info;

}


void sqlConnector::insertToChannelTable( unordered_map<string, string> channelMap )
{
    try
    {
        pstmt = conn->prepareStatement( insert + "channels" + channelParam );
        pstmt->setString(1, channelMap["display_name"]);
        pstmt->setString(2, channelMap["id"]);
        pstmt->setString(3, channelMap["broadcaster_type"]);
        pstmt->execute();

        pstmt->clearParameters();
    }

    catch ( SQLException & e )
    {
        // if not duplicate
        if (e.getErrorCode() != 1062)
            printExceptionInfo(e);
    }

}

void sqlConnector::createClipTable( unordered_map<string, string> channelMap )
{
    // stmt->execute( uniqueTable  "" )

}


void sqlConnector::printExceptionInfo(SQLException & e)
{
    cout << string(100, '!') << endl;
    cout << "ERROR sqlexecption in " << __FILE__ << endl;
    cout << "(" << __FUNCTION__ << ") on ";
    cout << "line: " << __LINE__ << endl;
    cout << "ERROR # " << e.what() << endl;
    cout << "ERROR code: " << e.getErrorCode() << endl;
    cout << "SQL state: " << e.getSQLState() << endl;
    cout << string(100, '!') << endl;
    cout << endl;
}