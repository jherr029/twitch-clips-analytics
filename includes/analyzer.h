#ifndef ANALYZER_H_
#define ANALYZER_H_

#include "sqlConnector.h"

#include <iostream>

class analyzer 
{
    public:
        analyzer();


    private:
        vector<string> channelNames;
        vector<vector<rawChannelStats> > dataCubeStats;

        sqlConnector sqlConn;

        void getAllChannels();
        void getChannelViewsFollowers( string );
        void calcStats();


};

#endif