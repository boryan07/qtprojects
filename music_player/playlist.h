#ifndef PLAYLIST_H
#define PLAYLIST_H

#include "track.h"
#include <set>
#include "help_function.h"

class PlayList
{
public:
    bool add(QString);
    void save();
    void load();
    QStringList GetTracksNames();
    std::set<Track> track_list;
    PlayList();

};

#endif // PLAYLIST_H
