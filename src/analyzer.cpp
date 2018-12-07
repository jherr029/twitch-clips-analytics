#include "../includes/analyzer.h"


analyzer::analyzer()
{
    cout << "analyzer initialized" << endl;
    getAllChannels();

    for ( auto name : channelNames )
    {
        getChannelViewsFollowers( name );
    }

    // for ( auto face : dataCubeStats )
    // {
    //     cout << face[0].name << endl;
    //     // for ( auto cube : face )
    //     // {
    //     //     cout << cube.views << endl;
    //     //     cout << cube.followers << endl;
    //     //     cout << cube.dateUpdated << endl;
    //     //     cout << cube.timeUpdated << endl;
    //     //     cout << endl;
    //     // }

    //     cout << "------" << endl;
    // }

    calcStats();

}

void analyzer::getAllChannels()
{
    channelNames = sqlConn.getAllChannelNames();

    // cout << "Size: " << channelNames.size() << endl;

    // for ( auto name : channelNames )
    //     cout << name << endl;
}

void analyzer::getChannelViewsFollowers( string name )
{

    vector<rawChannelStats> tempVec;

    // tempVec = sqlConn.fillStruct(name);
    dataCubeStats.push_back( sqlConn.fillStruct(name) );
    
}

// At this stage of development, it is best to use followers as a metric
// for increasing the fan base
// Later iterations will be more advanced and accurate
// in other words my scope will be on follower but view count will be used
// to determine things such as online state etc
void analyzer::calcStats()
{
    // vector<rawChannelStats> temp = dataCubeStats[17];

    // for (auto x : temp )
    // {
    //     cout << x.name << endl;
    //     cout << x.views << endl;
    //     cout << x.followers << endl;
    //     cout << endl;

    // }

    // for ( int i = 0; i < temp.size() - 1; i++ )
    // {
    //     cout << "Views: " << temp[i + 1].views - temp[i].views << endl;
    //     cout << "Followers: " << temp[i + 1].followers - temp[i].followers << endl;
    //     cout << endl;
    // }
    cout << endl;

    for ( auto temp : dataCubeStats )
    {
        cout << "channel: " << temp[0].name << endl;
        cout << "new views: " << temp.back().views - temp.front().views << "( " << temp.back().views << " - " << temp.front().views << ")" <<endl;
        cout << "new followers: " << temp.back().followers - temp.front().followers << endl;
        cout << string(50, '-') << endl;

    }

}

// the following is an examplequery used to subtract two values from one another
//select (a.views - b.views) as diff from channel_data a inner join channel_data b  where a.id = 146 and b.id = 10;
