#include "../headers/sqlConnector.h"
#include <fstream>
#include <string>

using namespace sql;

string uniqueTable = "create table if not exists ";
string insert = "insert into ";
string channelParam = "(name, id, type) values (?, ?, ?)";
string channelDataParam = "(name, game, views, followers, date, time) values (?, ?, ?, ?, ?, ?)";
string slugParam = "(name, title, views, date_created, time_created) values (?, ?, ?, ?, ?)";
// if inserting into all columns of a table then no need to name the columns

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
    infoFile.open("../info/mysqlStuff");

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

// TODO: fix problem where if a streamer already exist in the map to avoid updating the table again
// this can be done in two ways. Use a hashmap for recent inserts or check the dates within the table
// 2nd option seems slower
void sqlConnector::insertToChannelDataTable( unordered_map<string, string> channelMap )
{
    try
    {
        pstmt = conn->prepareStatement( insert + "channel_data" + channelDataParam );
        pstmt->setString(1, channelMap["display_name"]);
        pstmt->setString(2, channelMap["game"]);
        pstmt->setString(3, channelMap["views"]);   // may get error here
        pstmt->setString(4, channelMap["followers"]);   // here
        pstmt->setString(5, channelMap["date"]);    // here
        pstmt->setString(6, channelMap["time"]);    // here
        pstmt->execute();

        pstmt->clearParameters();
        

    }
    catch ( SQLException & e )
    {
        printExceptionInfo(e);
    }

}

void sqlConnector::insertToSlugDataTable( unordered_map<string, string> slugMap )
{
    try
    {
        pstmt = conn->prepareStatement( insert + "slugs_data" + slugParam );
        pstmt->setString(1, slugMap["name"]);   // stick to either display_name or name
        pstmt->setString(2, slugMap["title"]);
        pstmt->setString(3, slugMap["views"]);   // may get error here
        pstmt->setString(4, slugMap["date"]);   // here
        pstmt->setString(5, slugMap["time"]);    // here
        pstmt->execute();

        pstmt->clearParameters();
    }

    catch ( SQLException & e )
    {
        printExceptionInfo(e);
    }
} 

void sqlConnector::addToRecentChannelMap( string channel )
{
    if ( recentChannels[channel] == false )
        recentChannels[channel] = true;
}

bool sqlConnector::ifRecentChannel( string channel )
{
    return recentChannels[channel];
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