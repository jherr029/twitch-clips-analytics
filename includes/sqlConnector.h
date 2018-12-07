#ifndef SQLCONNECTOR_H_
#define SQLCONNECTOR_H_

#include <stdlib.h>
#include <cppconn/connection.h>
#include <cppconn/driver.h>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
#include <unordered_map>
#include <vector>

#include "extern.h"

using namespace std;
using namespace sql;


class sqlConnector
{
    public:
        sqlConnector();
        ~sqlConnector();

        void insertToChannelTable(unordered_map<string, string> );
        void insertToChannelDataTable( unordered_map<string, string> );
        void insertToSlugDataTable( unordered_map<string, string>);
        void createClipTable(unordered_map<string, string> );

        void addToRecentChannelMap( string );
        bool ifRecentChannel( string );

        string getNameFromID( string );

        bool checkConnection();
        bool checkIfTableExist( string );

        vector<string> getAllChannelNames();
        vector<rawChannelStats> fillStruct( string );
        // vector<unsigned long int> 

    private:
        Driver * driver = nullptr;
        Connection * conn = nullptr;
        Statement * stmt = nullptr;
        ResultSet * res = nullptr;
        PreparedStatement * pstmt = nullptr;

        unordered_map<string, bool> recentChannels; // check what is the default value
        // create a recentChannelsMap

        void insertToTable(unordered_map<string, string> );
        void printExceptionInfo(SQLException &);
};


#endif