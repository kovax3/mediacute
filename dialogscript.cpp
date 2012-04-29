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
#include "dialogscript.h"
#include "ui_dialogscript.h"
#include <QFileDialog>
#include <QDesktopServices>
DialogScript::DialogScript(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogScript)
{
    ui->setupUi(this);

}

DialogScript::~DialogScript()
{
    delete ui;
}
void DialogScript::setBitrate(int bit){
    biterate=QString::number(bit);
    ui->spinBox->setValue(bit);
}

 void DialogScript::setSuffix(QString ext)
 {
     saveSuffix=ext;
     ui->lineEditSuffix->setText(ext);
 }

 void DialogScript::setName(QString name)
 {
     saveName=name;
     ui->lineEditName->setText(name);
 }

 void DialogScript::setExecScript(bool exec)
 {
     executeScript=exec;
     if(exec==true){
         ui->lineEditFilename->setText(QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation));
     }else{
         ui->lineEditFilename->setText(QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation)+"/scriptmedia-sh");
     }

 }

  void DialogScript::setMediaName(bool isNamed)
  {
      if(isNamed==true){
          ui->widgetMediaName->setVisible(true);
      }else{
           ui->widgetMediaName->setVisible(false);
      }
  }

void DialogScript::on_buttonBox_accepted()
{
    biterate=QString::number(ui->spinBox->value());
    if(ui->lineEditName->text().isEmpty()){
        saveName="Name";
    }else{
        saveName=ui->lineEditName->text();
    }

    if(ui->lineEditSuffix->text().isEmpty()){
        saveSuffix=".mp3";
    }else{
        saveSuffix="."+ui->lineEditSuffix->text();
    }
    if (ui->lineEditFilename->text().isEmpty()){
        if(executeScript==true){
            ui->lineEditFilename->setText(QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation));
        }else{
            ui->lineEditFilename->setText(QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation)+"/scriptmedia-sh");
        }
    }else{
        saveFileName=ui->lineEditFilename->text();
    }
}

void DialogScript::on_toolButtonOpenFile_clicked()
{
    if(executeScript==false){
        QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                                        QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation)+"/scriptmedia-sh",
                                                        tr("ALL files (*.*)"));
        ui->lineEditFilename->setText(fileName);
    }else{
        QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                        "/home",
                                                        QFileDialog::ShowDirsOnly
                                                        | QFileDialog::DontResolveSymlinks);
        ui->lineEditFilename->setText(dir);
    }
}
