#ifndef HELP_FUNCTION_H
#define HELP_FUNCTION_H
#include <QString>
#include <QListWidget>

class help_function
{
public:
    help_function();
    static QString convert_to_time(qint64);
    static QString GetName(QString);
    static int find_position(QListWidget*,QString);
};

#endif // HELP_FUNCTION_H
