#include "help_function.h"
#include <QDebug>

help_function::help_function()
{

}

QString help_function::GetName(QString location)
{
    return location.right(location.size()-1-location.lastIndexOf("/")); //return only file_name without path
}

QString help_function::convert_to_time(qint64 dur)
{
   qint64 minutes=dur/60/1000;
   qint64 seconds=(dur-minutes*60000)/1000;
   QString answer=(minutes>9?"":"0")+QString::number(minutes)+":"+(seconds>9?"":"0")+QString::number(seconds);
   return answer.right(5);
}


int help_function::find_position(QListWidget * listwidegt,QString widget)
{
   int left=0;
   int right=listwidegt->count();
   while (left<right)
   {
       int mid=(left+right)/2;
       if (listwidegt->item(mid)->text()<widget)
       {
           left=mid+1;
       }
       else
       {
           right=mid;
       }
   }
   return std::max(0,left);
}
