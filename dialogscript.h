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
#ifndef DIALOGSCRIPT_H
#define DIALOGSCRIPT_H

#include <QDialog>

namespace Ui {
    class DialogScript;
}

class DialogScript : public QDialog
{
    Q_OBJECT

public:
    explicit DialogScript(QWidget *parent = 0);
    ~DialogScript();
   void setBitrate(int bit);
   void setSuffix(QString ext);
   void setName(QString name);
   void setExecScript(bool exec);
   void setMediaName(bool isNamed);
   bool executeScript;
   QString biterate;
   QString saveName;
   QString saveSuffix;
   QString saveFileName;
private slots:
   void on_buttonBox_accepted();

   void on_toolButtonOpenFile_clicked();

private:
    Ui::DialogScript *ui;
};

#endif // DIALOGSCRIPT_H
