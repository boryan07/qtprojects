#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDebug>
#include <random>
#include <algorithm>
#include <iterator>
#include <QMessageBox>
#include <QDialog>
#include <QEvent>
#include <QCloseEvent>
#include <QPixmap>
#include <QIcon>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QObject::connect(player,&QMediaPlayer::positionChanged,this,&MainWindow::change_position);
    QObject::connect(player,&QMediaPlayer::durationChanged,this,&MainWindow::set_duration);
    empty=true;
    shuffled=false;
    repeat=false;
    muted=false;
    can_give_info=true;
    current_row=duration=0;
    current_track="";
    player->setVolume(100);
    ui->current_duration_ms->setText("00:00");
    ui->duration_ms->setText("00:00");
    ui->SongList->addItems(play_list->GetTracksNames());
    ui->SongList->sortItems();
    set_all_incons();
}
void MainWindow::set_all_incons()
{
    ui->Remove->setIcon(QIcon(":/img/remove.png"));
    ui->Add->setIcon(QIcon(":/img/add.png"));
    ui->Shuffle->setIcon(QIcon(":/img/shuffle.png"));
    ui->repeat->setIcon(QIcon(":/img/repeat.png"));
    ui->repeat->setFlat(true);
    ui->Play->setFlat(true);
    ui->mute->setFlat(true);
    ui->Back->setFlat(true);
    ui->Next->setFlat(true);
    ui->Shuffle->setFlat(true);
    ui->Add->setFlat(true);
    ui->Remove->setFlat(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    play_list->save();
}

void MainWindow::refresh_widgets(QString input)
{
    ui->SongList->addItem(help_function::GetName(input));      // add song to QListWidget
}


void MainWindow::on_Add_clicked()
{
    QStringList names=QFileDialog::getOpenFileNames(this,tr("choose tracks")); //open file
    if (!names.empty())
    {
        for (auto object:names)
        {
            if (play_list->add(object)) // if track with this name already exist
            {
                refresh_widgets(object);
            }

        }
        ui->SongList->sortItems();
        current_row=help_function::find_position(ui->SongList,current_track);
    }
}


std::set<Track>::iterator MainWindow::search_track(std::string track_name)
{
    return lower_bound(play_list->track_list.begin(),play_list->track_list.end(),track_name,
                       [](Track a,std::string track_name) {return a.GetName()<track_name;}); // delete also from structure that contains name of this track
}

void MainWindow::on_Remove_clicked()
{
    if (!ui->SongList->count()) return;
    auto deleted_row=ui->SongList->currentRow();
    QString track_name=ui->SongList->item(deleted_row)->text();
    ui->SongList->takeItem(deleted_row);                        //widget that we deleted
    play_list->track_list.erase(search_track(track_name.toStdString())); //also delete from track_list
    if (deleted_row<current_row) current_row--; // if deleted track was before active track we should decrease current row
}

void MainWindow::on_Volume_sliderMoved(int position)
{
    player->setVolume(position);
    if (position)
    {
        muted=false;
        player->setMuted(false);
        ui->mute->setIcon(QIcon(":/img/on_volume.png"));
    }
}

void MainWindow::on_Play_clicked()
{
    if(player->state() == QMediaPlayer::PlayingState)
    {
        player->pause();
        ui->Play->setIcon(QIcon(":/img/play.png"));
    }
    else
    {
        player->play();
        ui->Play->setIcon(QIcon(":/img/pause.png"));

    }
}

void MainWindow::set_duration()
{
    ui->duration_ms->setText(help_function::convert_to_time(player->duration()));
}


void MainWindow::load_track()
{
    QString track_name=ui->SongList->item(current_row)->text();
    QString location=QString::fromStdString((*search_track(track_name.toStdString())).GetLocation());
    player->setMedia(QUrl::fromLocalFile(location)); //use path to setMedia
    current_track=track_name;
    ui->SongList->setCurrentRow(current_row);
    ui->track_name->setText(current_track);

}

void MainWindow::on_Back_clicked()
{
   int list_size=ui->SongList->count();
   if (list_size)
   {
        ui->Play->setIcon(QIcon(":/img/pause.png"));
        current_row=(current_row+list_size-1)%list_size;
        load_track();
        player->play();
   }
}
void MainWindow::on_Next_clicked()
{
    int list_size=ui->SongList->count();
    if (list_size)
    {
         ui->Play->setIcon(QIcon(":/img/pause.png"));
         current_row=(current_row+list_size+1)%list_size;
         load_track();
         player->play();
    }
}

void MainWindow::on_SongList_itemDoubleClicked()
{
    ui->Play->setIcon(QIcon(":/img/pause.png"));
    current_row=ui->SongList->currentRow();
    load_track();
    player->play();
}

void MainWindow::on_Shuffle_clicked()
{
    shuffled=!shuffled;
    ui->Shuffle->setDown(shuffled);
    int size=ui->SongList->count();
    if (!size)  return; // QListWidget is empty}
    QString curr_name=ui->SongList->item(current_row)->text();
    if (shuffled)
    {
        std::random_device rd;
        std::mt19937 g(rd());
        std::vector<QString> vector_items;
        for (int iter=0;iter<size;++iter)
        {
            initial_list.push_back(ui->SongList->item(iter)->text());
            if (iter==current_row) continue;
            vector_items.push_back(ui->SongList->item(iter)->text());
        }
        ui->SongList->clear();
        shuffle(vector_items.begin(),vector_items.end(),g);
        vector_items.insert(vector_items.begin(),curr_name);
        for (int iter=0;iter<size;++iter)
        {
            ui->SongList->insertItem(iter,vector_items[static_cast<unsigned int>(iter)]);
        }
        vector_items.clear();
        current_row=0;
    }
    else
    {
        ui->SongList->clear();
        for (int iter=0;iter<size;++iter)
        {   if (curr_name==initial_list[static_cast<unsigned int>(iter)]) current_row=iter;
            ui->SongList->insertItem(iter,initial_list[static_cast<unsigned int>(iter)]);
        }
        initial_list.clear();
    }
    ui->SongList->setCurrentRow(current_row);
}


void MainWindow::change_position()
{
    if (can_give_info)
    {
        int mean=static_cast<int>(static_cast<double>(player->position())/static_cast<double>(player->duration()) * 1000.0);
        ui->Duration->setValue(mean);
        ui->current_duration_ms->setText(help_function::convert_to_time(player->position()));
    }
    if (player->duration()==player->position()&&player->duration())
    {
        if (repeat) current_row--;
        on_Next_clicked();
    }
}

void MainWindow::on_mute_clicked()
{
    muted=!muted;
    player->setMuted(muted);
    ui->Volume->setValue(static_cast<int>(!muted)*player->volume());
    if (muted)
    {
        ui->mute->setIcon(QIcon(":/img/off_volume.png"));
    }
    else
    {
        ui->mute->setIcon(QIcon(":/img/on_volume.png"));
    }
}

void MainWindow::on_Duration_sliderPressed()
{
    can_give_info=false;
}

void MainWindow::on_Duration_sliderReleased()
{
    player->setPosition(static_cast<int>(static_cast<double>(player->duration())/1000.0*static_cast<double>(ui->Duration->sliderPosition())));
    can_give_info=true;
}

void MainWindow::on_Duration_sliderMoved()
{
    double time=static_cast<double>(ui->Duration->sliderPosition())/1000.0*static_cast<double>(player->duration());
    ui->current_duration_ms->setText(help_function::convert_to_time(static_cast<qint64>(time)));
}

void MainWindow::on_repeat_clicked()
{
    repeat=!repeat;
    ui->repeat->setDown(repeat);
}
