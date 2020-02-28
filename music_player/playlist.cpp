#include "playlist.h"
#include <QStringList>
#include <QString>
#include "mainwindow.h"
#include <fstream>

PlayList::PlayList()
{
    load();
}

void PlayList::load()
{
    std::ifstream read("playlist");
    std::string input;
    while (getline(read,input))
    {
        Track track;
        track.SetLocation(QString::fromStdString(input));
        track.SetName(help_function::GetName(QString::fromStdString(input)));
        track_list.insert(track);
    }
}

bool PlayList::add(QString track_name)
{
    Track track;
    track.SetName(help_function::GetName(track_name));
    track.SetLocation(track_name);
    if (track_list.find(track)==track_list.end())
    {
        track_list.insert(track);
        return true;
    }
    return false;
}

void PlayList::save()
{
    std::ofstream write("playlist");
    for(Track item:track_list)
    {
        item.GetLocation();
        write << item.GetLocation() << std::endl;
    }
}

QStringList PlayList::GetTracksNames()
{
    QStringList list;
    for(Track item:track_list)
    {
        QString qstr = QString::fromStdString(item.GetName());
        list.push_back(qstr);
    }
    return list;
}
