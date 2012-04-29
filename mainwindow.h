/****************************************************************************
//   mediacute Copyright (C) 2010 yahia abouzakaria <yahiaui@gmail.com>
//
//      This program comes with ABSOLUTELY NO WARRANTY; for details type `show w'.
//      This is free software, and you are welcome to redistribute it
//      under certain conditions; type `show c' for details.
//
//  The hypothetical commands `show w' and `show c' should show the appropriate
//  parts of the General Public License.  Of course, your program's commands
//  might be different; for a GUI interface, you would use an "about box".
//
//    You should also get your employer (if you work as a programmer) or school,
//  if any, to sign a "copyright disclaimer" for the program, if necessary.
//  For more information on this, and how to apply and follow the GNU GPL, see
//  <http://www.gnu.org/licenses/>.
//
//    The GNU General Public License does not permit incorporating your program
//  into proprietary programs.  If your program is a subroutine library, you
//  may consider it more useful to permit linking proprietary applications with
//  the library.  If this is what you want to do, use the GNU Lesser General
//  Public License instead of this License.  But first, please read
//  <http://www.gnu.org/philosophy/why-not-lgpl.html>.
// ----------------------------------------------------------
** If you have questions regarding the use of this file, please contact
** abouzakaria (yahiaui@gmail.com)
** $mediacute_END_LICENSE$
**
****************************************************************************/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QObject>
#include <QtGui/QWidget>
#include <QtGui/QApplication>
//#include <QtCore/QTimerEvent>
//#include <QtGui/QShowEvent>
#include <QtGui/QIcon>
//#include <QtCore/QBasicTimer>
#include <QtGui/QAction>
#include <QtGui/QTreeWidget>
#include <phonon/audiooutput.h>

//#include <phonon/backendcapabilities.h>
#include <phonon/videowidget.h>
#include <phonon/mediaobject.h>
#include <phonon/seekslider.h>

#include <phonon/volumeslider.h>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    virtual void closeEvent(QCloseEvent *e);

private slots:

    void loadSetting();
    void saveSetting();
void hasVideoChanged(bool bHasVideo);
void showContextMenu(const QPoint &p);
void scaleChanged(QAction *act);
    void stateChanged(Phonon::State newstate, Phonon::State );
    void playPause();
    void setFile(const QString &text);
    void updateTime();
    void updateInfo();
    void addPoint();
    void curPoint();

    void treeChargeSoura(int idSora);
    void saveTree();
    void loadTree();
    void editCurItem();
    void removeCurIte();
    void on_pButtonPlayPause_clicked();
    void on_pButtonBack_clicked();
    void on_pButtonForwar_clicked();
    void on_pButtonAddPoint_clicked();
    void on_pButtonCurPoint_clicked();
    void on_actionOpen_triggered();
    void on_treeWidget_itemActivated(QTreeWidgetItem *item, int );
    void on_treeWidget_itemSelectionChanged();
    void on_action_Creat_script_triggered();


    void on_action_Quit_triggered();

    void on_treeWidget_customContextMenuRequested(const QPoint);





    void on_action_execute_triggered();

    void on_actionAbout_triggered();

private:
    Ui::MainWindow *ui;
QAction *actRemoveItem;
QAction *actEditeItem;
QMenu *scaleMenu;
    QIcon playIcon;
    QIcon pauseIcon;
    QString m_fileName;
    QString m_bitrate;
    bool m_isQuran;
    int m_idSora;
    Phonon::SeekSlider *slider;
    Phonon::VolumeSlider *volume;

    Phonon::MediaObject m_MediaObject;

    Phonon::AudioOutput m_AudioOutput;

   Phonon::VideoWidget *m_videoWidget;
 //  MediaVideoWidget *m_videoWidget;
    // Phonon::Path m_audioOutputPath;
    //bool m_smallScreen;

};

#endif // MAINWINDOW_H
