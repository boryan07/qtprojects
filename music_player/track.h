#ifndef TRACK_H
#define TRACK_H

#include <QString>
#include <string>

class Track
{
public:
    void SetName(QString);
    void SetLocation(QString);
    std::string GetName() const;
    std::string GetLocation() const;
    std::string track_name;
    std::string track_location;
    Track();
    bool operator<(const Track &track) const;
};

#endif // TRACK_H
