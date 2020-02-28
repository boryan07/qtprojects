#include "track.h"

Track::Track()
{

}

bool Track::operator<(const Track &track) const
{
    return this->track_name<track.track_name;
}

void Track::SetName(QString name)
{
    track_name=name.toStdString();
}

void Track::SetLocation(QString location)
{
    track_location=location.toStdString();
}

std::string Track::GetLocation() const
{
    return this->track_location;
}

std::string Track::GetName() const
{
    return this->track_name;
}
