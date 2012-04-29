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
#include "dialogopen.h"
#include "ui_dialogopen.h"
#include <QFileDialog>
#include <QDesktopServices>
#include <QXmlStreamReader>

DialogOpen::DialogOpen(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogOpen)
{
    ui->setupUi(this);
    comboCharge();
}

DialogOpen::~DialogOpen()
{
    delete ui;
}

void DialogOpen::on_toolButtonOpenFile_clicked()
{
    QString fileN = QFileDialog::getOpenFileName(this, QString(),
                                                          QDesktopServices::storageLocation(QDesktopServices::MusicLocation));
    if (fileN.size() > 0) {

        fileName= fileN;
        ui->lineEditFileName->setText(fileN);
    }
}

void DialogOpen::on_buttonBox_accepted()
{
    QFile file;
    if(file.exists(ui->lineEditFileName->text())){
        fileName=  ui->lineEditFileName->text();
    isQuran=ui->checkBoxIsQuran->isChecked();
    idSura=ui->comboBoxQuran->currentIndex()+1;

    }
}

void DialogOpen::comboCharge()
{

    QFile file(":/Quran.xml");
    file.open(QIODevice::ReadOnly);

    QXmlStreamReader xml;
    xml.setDevice(&file);
    while (!xml.atEnd()) {
        xml.readNext();
        if (xml.name() == "Department"){
            QString textSora;

            textSora=xml.attributes().value("Name").toString();

            if(!textSora.isNull()){
                ui->comboBoxQuran->addItem(textSora);
            }
        }

    }

    xml.clear();
    file.close();

}
