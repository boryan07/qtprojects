#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include "playlist.h"
#include <QUrl>
#include <QListWidget>
#include <QString>
#include "help_function.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
signals:

private slots:
    void closeEvent(QCloseEvent*);
    void change_position();
    void on_Add_clicked();
    void on_Remove_clicked();

    void on_Volume_sliderMoved(int position);

    void on_Play_clicked();

    void on_Back_clicked();

    void on_Next_clicked();

    void on_SongList_itemDoubleClicked();

    void on_Shuffle_clicked();
    void set_duration();

    void on_mute_clicked();

    void on_Duration_sliderPressed();

    void on_Duration_sliderReleased();

    void on_Duration_sliderMoved();

    void on_repeat_clicked();

private:
    void set_all_incons();
    bool repeat;
    bool can_give_info;
    bool muted;
    bool shuffled;
    void start_track();
    bool is_empty();
    bool empty;
    int duration;
    int current_row;
    QString current_track;
    std::set<Track>::iterator search_track(std::string);
    void refresh_widgets(QString);
    QString GetSongName();
    void load_track();
    std::vector <QString>  initial_list;
    QMediaPlayer* player=new QMediaPlayer();
    PlayList* play_list=new PlayList();


private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H



