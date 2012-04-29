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
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialogopen.h"
#include "dialogscript.h"
#include <QtGui>
#include <QObject>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
       QApplication::setLayoutDirection(Qt::RightToLeft);
   scaleMenu=new QMenu(this);
 //  QMenu *scaleMenu = menu.addMenu(tr("&Scale mode"));
   QActionGroup *scaleGroup = new QActionGroup(scaleMenu);
   connect(scaleGroup, SIGNAL(triggered(QAction*)), this, SLOT(scaleChanged(QAction*)));
   scaleGroup->setExclusive(true);
   QAction *scaleActionFit = scaleMenu->addAction(tr("Fit in view"));
   scaleActionFit->setCheckable(true);
   scaleActionFit->setChecked(true);
   scaleGroup->addAction(scaleActionFit);
   QAction *scaleActionCrop = scaleMenu->addAction(tr("Scale and crop"));
   scaleActionCrop->setCheckable(true);
   scaleGroup->addAction(scaleActionCrop);
  // menu.addMenu(scaleMenu);
   actRemoveItem=new QAction(trUtf8("حذف العنصر المحدد"),this);
   actEditeItem=new QAction(trUtf8("تحرير العنصر المحد"),this);
    m_videoWidget=new  Phonon::VideoWidget(this);
    m_videoWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(actRemoveItem, SIGNAL(triggered()), this, SLOT(removeCurIte()));
     connect(actEditeItem, SIGNAL(triggered()), this, SLOT(editCurItem()));
    connect(&m_MediaObject, SIGNAL(stateChanged(Phonon::State,Phonon::State)), this, SLOT(stateChanged(Phonon::State,Phonon::State)));
    connect(&m_MediaObject, SIGNAL(totalTimeChanged(qint64)), this, SLOT(updateTime()));
    connect(&m_MediaObject, SIGNAL(tick(qint64)), this, SLOT(updateTime()));
    connect(&m_MediaObject, SIGNAL(metaDataChanged()), this, SLOT(updateInfo()));
     connect(&m_MediaObject, SIGNAL(hasVideoChanged(bool)), this, SLOT(hasVideoChanged(bool)));
    connect(m_videoWidget, SIGNAL(customContextMenuRequested(const QPoint &)), SLOT(showContextMenu(const QPoint &)));
    Phonon::createPath(&m_MediaObject, &m_AudioOutput);
   Phonon::createPath(&m_MediaObject, m_videoWidget);
   ui->vLayoutInfo->addWidget(m_videoWidget);
   slider = new Phonon::SeekSlider(this);
    slider->setMediaObject(&m_MediaObject);
    volume = new Phonon::VolumeSlider(&m_AudioOutput);
    volume->setFixedWidth(120);
    ui->hLayoutMedia->addWidget(slider);
    ui->hLayoutMedia->addWidget(volume);
    playIcon = style()->standardIcon(QStyle::SP_MediaPlay);
    pauseIcon = style()->standardIcon(QStyle::SP_MediaPause);
    ui->pButtonPlayPause->setIcon(playIcon);
    ui->pButtonBack->setIcon(style()->standardIcon(QStyle::SP_MediaSeekBackward));
    ui->pButtonForwar->setIcon(style()->standardIcon(QStyle::SP_MediaSeekForward));
    ui->textEditAya->setVisible(false);
     m_videoWidget->setVisible(false);

    loadSetting();

}

MainWindow::~MainWindow()
{


    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    if(ui->treeWidget->topLevelItemCount()>0){
    int reponse = QMessageBox::warning(this, trUtf8("تنبيه"), trUtf8("هل تريد حفظ الجلسة؟") ,QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
           if (reponse == QMessageBox::Save){
               saveSetting();
               saveTree();
           }else if (reponse == QMessageBox::Cancel){
               e->ignore();
               return;
           }
    }
}



void MainWindow::loadSetting()
{
    QSettings settings(QDir::homePath()+"/.mediacute/setting.ini",QSettings::IniFormat);

    settings.beginGroup("Main");

    m_fileName=  settings.value("fileName","").toString();
    m_bitrate=  settings.value("bitrate","64").toString();
    m_idSora=  settings.value("idSora", 0).toInt();
    m_isQuran=  settings.value("isQuran",false).toBool();

    settings.endGroup();
    if(QFile::exists(m_fileName)){
        setFile(m_fileName);
        m_MediaObject.stop();
        loadTree();
    }
}

void MainWindow::saveSetting()
{

    QSettings settings(QDir::homePath()+"/.mediacute/setting.ini",QSettings::IniFormat);


    settings.beginGroup("Main");

    settings.setValue("fileName", m_fileName);
    settings.setValue("bitrate", m_bitrate);
    settings.setValue("idSora", m_idSora);
    settings.setValue("isQuran", m_isQuran);

    settings.endGroup();

}

void MainWindow::hasVideoChanged(bool bHasVideo)
{
    ui->textEditAya->setVisible(!bHasVideo);
    m_videoWidget->setVisible(bHasVideo);
   // m_fullScreenAction->setEnabled(bHasVideo);
}
void MainWindow::showContextMenu(const QPoint &p)
{

     scaleMenu->exec(QCursor::pos());
}

void MainWindow::scaleChanged(QAction *act)
{
    if (act->text() == tr("Scale and crop"))
        m_videoWidget->setScaleMode(Phonon::VideoWidget::ScaleAndCrop);
    else
        m_videoWidget->setScaleMode(Phonon::VideoWidget::FitInView);
}

void MainWindow::stateChanged(Phonon::State newstate, Phonon::State )
{
    switch (newstate) {
        case Phonon::ErrorState:
            if (m_MediaObject.errorType() == Phonon::FatalError) {
                   ui->pButtonPlayPause->setEnabled(false);
                //rewindButton->setEnabled(false);
            } else {
                m_MediaObject.pause();
            }
            QMessageBox::warning(this, "Phonon Mediaplayer", m_MediaObject.errorString(), QMessageBox::Close);
            break;

        case Phonon::StoppedState:
          //  m_videoWidget->setFullScreen(false);
            // Fall through
        case Phonon::PausedState:
               ui->pButtonPlayPause->setIcon(playIcon);
            if (m_MediaObject.currentSource().type() != Phonon::MediaSource::Invalid){
                   ui->pButtonPlayPause->setEnabled(true);
                //rewindButton->setEnabled(true);
            } else {
                   ui->pButtonPlayPause->setEnabled(false);
               // rewindButton->setEnabled(false);
            }
            break;
        case Phonon::PlayingState:
               ui->pButtonPlayPause->setEnabled(true);
               ui->pButtonPlayPause->setIcon(pauseIcon);
            if (m_MediaObject.hasVideo())
               //m_videoWidget.show();
            // Fall through
        case Phonon::BufferingState:
            //rewindButton->setEnabled(true);
            break;
        case Phonon::LoadingState:
          // rewindButton->setEnabled(false);
            break;
    }
}

void MainWindow::playPause()
{
    if (m_MediaObject.state() == Phonon::PlayingState)
        m_MediaObject.pause();
    else {
        if (m_MediaObject.currentTime() == m_MediaObject.totalTime())
            m_MediaObject.seek(0);
        m_MediaObject.play();
    }
}

void MainWindow::setFile(const QString &fileName)
{
    ui->pButtonAddPoint->setVisible(true);
    ui->textEditAya->setVisible(false);

    if(m_isQuran==true){

         ui->pButtonAddPoint->setVisible(false);
         ui->textEditAya->setVisible(true);
    }
    setWindowTitle(fileName.right(fileName.length() - fileName.lastIndexOf('/') - 1));
    m_MediaObject.setCurrentSource(Phonon::MediaSource(fileName));
    m_MediaObject.play();
}

void MainWindow::updateTime()
{
    long len = m_MediaObject.totalTime();
    long pos = m_MediaObject.currentTime();
    QString timeString;

    if (pos || len)
    {
        int sec = pos/1000;
        int min = sec/60;
        int hour = min/60;
        int msec = pos;

        QTime playTime(hour%60, min%60, sec%60, msec%1000);
        sec = len / 1000;
        min = sec / 60;
        hour = min / 60;
        msec = len;

        QTime stopTime(hour%60, min%60, sec%60, msec%1000);
        QString timeFormat = "m:ss";
        if (hour > 0)
            timeFormat = "h:mm:ss";
        timeString = playTime.toString(timeFormat);
        if (len)
            timeString += " / " + stopTime.toString(timeFormat);
    }
    ui->labelTime->setText(timeString);
    //ui->spinBoxSeek->setValue(pos/1000);
  //  ui->labelCurTime->setText(QString::number(pos/1000));
}

void MainWindow::updateInfo()
{
    int maxLength = 50;

    QMap <QString, QString> metaData = m_MediaObject.metaData();
    QString trackArtist = metaData.value("ARTIST");
    if (trackArtist.length() > maxLength)
        trackArtist = trackArtist.left(maxLength) + "...";

    QString trackTitle = metaData.value("TITLE");
    int trackBitrate = metaData.value("BITRATE").toInt();

    QString fileName;
    if (m_MediaObject.currentSource().type() == Phonon::MediaSource::Url) {
        fileName = m_MediaObject.currentSource().url().toString();
    } else {
        fileName = m_MediaObject.currentSource().fileName();
        fileName = fileName.right(fileName.length() - fileName.lastIndexOf('/') - 1);
        if (fileName.length() > maxLength)
            fileName = fileName.left(maxLength) + "...";
    }

    QString title;
    if (!trackTitle.isEmpty()) {
        if (trackTitle.length() > maxLength)
            trackTitle = trackTitle.left(maxLength) + "...";
        title = "Title: "  + trackTitle + "<br>";
    } else if (!fileName.isEmpty()) {
        if (fileName.length() > maxLength)
            fileName = fileName.left(maxLength) + "...";
        title =  fileName ;
        if (m_MediaObject.currentSource().type() == Phonon::MediaSource::Url) {
            title.prepend("Url: ");
        } else {
            title.prepend("File: ");
        }
    }

    QString artist;
    if (!trackArtist.isEmpty())
        artist = "Artist:  "+ trackArtist ;

    QString bitrate;
    if (trackBitrate != 0)
        bitrate = "<br>Bitrate:  " + QString::number(trackBitrate/1000) + "kbit";
    m_bitrate=QString::number(trackBitrate/1000);
    ui->labelInfo->setText(title + artist + bitrate);


}

void MainWindow::addPoint()
{
    long pos = m_MediaObject.currentTime();
    QString count=QString::number(ui->treeWidget->topLevelItemCount()+1);
 //  QMessageBox::information(this,"",QString::number(pos/1000));
    QTreeWidgetItem *item=new QTreeWidgetItem(ui->treeWidget);
    item->setText(0,trUtf8("النقطة : ")+count);
    item->setText(1,QString::number(pos/1000));
    ui->treeWidget->setCurrentItem(item);
}

void MainWindow::curPoint()
{
    if (ui->treeWidget->topLevelItemCount()<1)
        return;
    long pos = m_MediaObject.currentTime();
QTreeWidgetItem *item=ui->treeWidget->currentItem();
QTreeWidgetItem *nextItem=ui->treeWidget->itemBelow(item);
    item->setText(1,QString::number(pos/1000));
    if(nextItem)
    ui->treeWidget->setCurrentItem(nextItem);
}

void MainWindow::treeChargeSoura(int idSora)
{


    QTreeWidgetItem *itemaya ;
    QFile file(":/Quran.xml");
    file.open(QIODevice::ReadOnly);

    ui->treeWidget->clear();
    QXmlStreamReader xml;
    xml.setDevice(&file);
    while (!xml.atEnd()) {
        xml.readNext();

        if (xml.name() == "Item"){
            QString idAya= xml.attributes().value("ID").toString();
            QString id=idAya.section("_",0,0);


            if(!idAya.isNull()){
                if (id.toInt()==idSora){
                QString textAya=xml.readElementText();
                itemaya= new QTreeWidgetItem(ui->treeWidget);
                itemaya->setText(0,textAya );
itemaya->setTextAlignment(0,Qt::AlignJustify);
                }
            }
        }
    }

    xml.clear();
    file.close();

}

void MainWindow::saveTree()
{
  int itemCount=ui->treeWidget->topLevelItemCount();
//    if(itemCount<1)
//        return;

    QFile data(QDir::homePath()+"/.mediacute/lastrc");
    if (!data.open(QFile::WriteOnly | QFile::Truncate))
        return;

        QTextStream out(&data);

        for (int i=0;i<itemCount;i++){
             QTreeWidgetItem *item=ui->treeWidget->topLevelItem(i);
             QString txt=item->text(0)+"|"+item->text(1)+"\n";
              out << txt;
        }


}

void MainWindow::loadTree()
{
    QFile file;
  file.setFileName(QDir::homePath()+"/.mediacute/lastrc");
  if(file.open( QFile::ReadOnly)){
      QTextStream textStream(&file);
      QString line;
      line = textStream.readLine();//premier line
      while (!line.isNull()) {

            QString txt=line.section("|",0,0);
          QString txtdata=line.section("|",1,1);
          QTreeWidgetItem *item=new QTreeWidgetItem(ui->treeWidget);
          item->setText(0,txt);
            item->setText(1,txtdata);

          line = textStream.readLine();
      }
  }
  file.close();
}

void MainWindow::editCurItem()
{

    QTreeWidgetItem *item=ui->treeWidget->currentItem();
    int value=item->text(1).toInt();
    long len = m_MediaObject.totalTime();
    int max=len/1000;
    bool ok;
    int i = QInputDialog::getInt(this, trUtf8("تحرير العنصر"),
                                 trUtf8("الوقت:"), value, 0, max, 1, &ok);
    if (ok)
        item->setText(1,QString::number(i));


}

void MainWindow::removeCurIte()
{
     QTreeWidgetItem *item=ui->treeWidget->currentItem();
     int index=ui->treeWidget->indexOfTopLevelItem(item);
             ui->treeWidget->takeTopLevelItem(index);
}

void MainWindow::on_pButtonPlayPause_clicked()
{
    playPause();
}

void MainWindow::on_pButtonBack_clicked()
{
    qint64 value= (m_MediaObject.currentTime())-1000;
    m_MediaObject.seek(value);
}

void MainWindow::on_pButtonForwar_clicked()
{
    qint64 value= (m_MediaObject.currentTime())+1000;
    m_MediaObject.seek(value);
}

void MainWindow::on_pButtonAddPoint_clicked()
{
    addPoint();
}

void MainWindow::on_pButtonCurPoint_clicked()
{
    curPoint();

}

void MainWindow::on_actionOpen_triggered()
{
    DialogOpen *dlg=new DialogOpen(this);
    if(dlg->exec()==QDialog::Accepted){

        QFile file;
        if(!file.exists(dlg->fileName))
            return;
        m_fileName=dlg->fileName;
        m_isQuran=dlg->isQuran;
        m_idSora=dlg->idSura;
         ui->treeWidget->clear();
        if(m_isQuran==true){
             treeChargeSoura(m_idSora);
        }

        setFile(m_fileName);

    }

}

void MainWindow::on_treeWidget_itemActivated(QTreeWidgetItem *item, int )
{

    qint64 value= item->text(1).toInt()*1000;


    if (m_isQuran==true){

        QTreeWidgetItem *oldItem=ui->treeWidget->itemAbove(item);
        if(oldItem){
            value=oldItem->text(1).toInt()*1000;
        }else{
            value=0;
        }
    }
    m_MediaObject.seek(value);
    m_MediaObject.play();
}

void MainWindow::on_treeWidget_itemSelectionChanged()
{
    QTreeWidgetItem *item=ui->treeWidget->currentItem();
    if(item)
    ui->textEditAya->setText(item->text(0));
}

void MainWindow::on_action_Creat_script_triggered()
{
    QString saveName;
    QString saveSuffix;
    QString saveFileName;
    int itemCount=ui->treeWidget->topLevelItemCount();
    if(itemCount<1)
        return;

    DialogScript *dlg=new DialogScript(this);
    QFileInfo fi(m_fileName);
    QString ext = fi.suffix();
    QString base = fi.baseName();
    dlg->setExecScript(false);
    dlg->setSuffix(ext);
    dlg->setBitrate(m_bitrate.toInt());
    dlg->setName(base);
    if(m_isQuran==true){
        dlg->setMediaName(false);
    }else{
         dlg->setMediaName(true);
    }
    if(dlg->exec()==QDialog::Accepted){
        m_bitrate=dlg->biterate;
        saveName=dlg->saveName;
        saveSuffix=dlg->saveSuffix;
        saveSuffix=saveSuffix.replace("..",".");
        saveFileName=dlg->saveFileName;
    }else{
        return;
    }

    QFile data(saveFileName);
    if (!data.open(QFile::WriteOnly | QFile::Truncate))
        return;
       data.setPermissions(QFile::ExeUser|QFile::ReadUser|QFile::WriteUser );

        QTextStream out(&data);
        out<<"#/bin/sh \n";

    for (int i=0;i<itemCount;i++){
        QTreeWidgetItem *item=ui->treeWidget->topLevelItem(i);
        QTreeWidgetItem *oldItem=ui->treeWidget->itemAbove(item);
        QString oldTime;
        if(i==0){
            oldTime="0";
        }else{
            oldTime=oldItem->text(1);
        }

        int duration;
        if (i==itemCount-1){
            duration=(m_MediaObject.totalTime()/1000)-oldTime.toInt();
        }else{
            duration=item->text(1).toInt()-oldTime.toInt();
        }
        QString curTime=QString::number(duration);
         QString name;
        if (m_isQuran==true){
            QString aya="000"+QString::number(i+1);
            QString sura="000"+QString::number(m_idSora);;
            name=sura.right(3)+aya.right(3) +saveSuffix;
        }else{
            name=saveName+"_"+QString::number(i)+saveSuffix;
        }



        if (!item->text(1).isEmpty()){
            QString txt=QString("ffmpeg -ss %1 -t %2 -i \"%3\" -ab %4k \"%5\"").arg(oldTime).arg(curTime).arg(m_fileName).arg(m_bitrate).arg(name);
           // QMessageBox::information(this,"",txt);
            out<<txt +"\n";
        }
    }
    QMessageBox::information(this,"",trUtf8("تم انشاء الملف بنجاح \n ")+(saveFileName));

}


void MainWindow::on_action_Quit_triggered()
{
    if(this->close()==true)
           qApp->quit();
}

void MainWindow::on_treeWidget_customContextMenuRequested(const QPoint)
{
    QTreeWidgetItem *item=ui->treeWidget->currentItem();
        if (item){
            QMenu menu(this);
           // menu.setLayoutDirection(Qt::RightToLeft);
            menu.addAction(actEditeItem);

            menu.addSeparator();
             menu.addAction(actRemoveItem);
             if (m_isQuran==true)
                 actRemoveItem->setEnabled(false);
            menu.exec(QCursor::pos());
        }
}





void MainWindow::on_action_execute_triggered()
{

    QString saveName;
    QString saveSuffix;
    QString saveFileName;
    int itemCount=ui->treeWidget->topLevelItemCount();
    if(itemCount<1)
        return;

    DialogScript *dlg=new DialogScript(this);
    QFileInfo fi(m_fileName);
    QString ext = fi.suffix();
    QString base = fi.baseName();
    dlg->setSuffix(ext);
    dlg->setBitrate(m_bitrate.toInt());
    dlg->setName(base);
    dlg->setExecScript(true);
    if(m_isQuran==true){
        dlg->setMediaName(false);
    }else{
        dlg->setMediaName(true);
    }
    if(dlg->exec()==QDialog::Accepted){
        m_bitrate=dlg->biterate;
        saveName=dlg->saveName;
        saveSuffix=dlg->saveSuffix;
        saveSuffix=saveSuffix.replace("..",".");
        saveFileName=dlg->saveFileName;
    }else{
        return;
    }

   QProgressDialog progress(trUtf8("الرجاء الانتظار...."), trUtf8("الغاء"), 0, itemCount-1, 0);
     progress.setWindowModality(Qt::WindowModal);
    // progress.setEnabled(false);
    progress.show();
    qApp->processEvents();


    for (int i=0;i<itemCount;i++){
        progress.setValue(i);
        if (progress.wasCanceled())
            break;
        QTreeWidgetItem *item=ui->treeWidget->topLevelItem(i);
        QTreeWidgetItem *oldItem=ui->treeWidget->itemAbove(item);
        QString oldTime;
        if(i==0){
            oldTime="0";
        }else{
            oldTime=oldItem->text(1);
        }

        int duration;
        if (i==itemCount-1){
            duration=(m_MediaObject.totalTime()/1000)-oldTime.toInt();
        }else{
            duration=item->text(1).toInt()-oldTime.toInt();
        }
        QString curTime=QString::number(duration);
        QString name;
        if (m_isQuran==true){
            QString aya="000"+QString::number(i+1);
            QString sura="000"+QString::number(m_idSora);;
            name=sura.right(3)+aya.right(3) +saveSuffix;
        }else{
            name=saveName+"_"+QString::number(i)+saveSuffix;
        }

        QFile file(saveFileName+"/"+name);
        if (!item->text(1).isEmpty() && !file.exists()){


            QString txt=QString("ffmpeg -ss %1 -t %2 -i \"%3\" -ab %4k \"%5\"").arg(oldTime).arg(curTime).arg(m_fileName).arg(m_bitrate).arg(saveFileName+"/"+name);
               QProcess prosses;
            prosses.start(txt);
            prosses.waitForFinished();
            QByteArray result = prosses.readAll();
            QTextCodec *codec = QTextCodec::codecForName("UTF-8");
            QString string = codec->toUnicode(result);
            //  ui->textEditAya->setText(saveFileName+"/"+name);
            ui->textEditAya->setText(string);
            ui->statusBar->showMessage(trUtf8("جاري انشاء الملف :")+(saveFileName+"/"+name));
            progress.setLabelText(trUtf8("جاري انشاء الملف :")+(saveFileName+"/"+name));
            qApp->processEvents();


        }
    }
    //QMessageBox::information(this,"",trUtf8("تم انشاء الملفات بنجاح \n ")+(saveFileName));
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox msgBox(this);
    QPixmap   pixmapG(":/icons/48x48/mediacute.png") ;
    msgBox.setLayoutDirection(Qt::RightToLeft);
    msgBox.setWindowTitle(trUtf8("حول برنامج تقسيم ملفات الصوت"));
    msgBox.setText(trUtf8("برنامج تقسيم ملفات الصوت"));
    msgBox.setIconPixmap(pixmapG);
    msgBox.setInformativeText(trUtf8("المؤلف : أبو زكريا")+"\n"+trUtf8("الاصدار : 0.1 تجريبي")
                              +"\n"+trUtf8("الرخصة :  gpl3")
                              +"\n"+trUtf8("برنامج تقسيم ملفات الصوت عمل خصيصا لتقسيم ملفات اصوات القرءان الكريم من ملفات للسور الى ملفات للايات   ")
+"\n"+trUtf8("كما يمكن تقسيم الملفات الاخرى مثل ملفات الفيديو وذلك بالاستعانة ببرنامج ffmpeg") );
    msgBox.exec();
}
