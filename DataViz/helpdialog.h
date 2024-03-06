#ifndef HELPDIALOG_H
#define HELPDIALOG_H

/********************************
 *
 *  This class is defined for the "Help" dialog,
 *  an object of this class is the window of the "Help" dialog in the app
 *
**********************************/


#include <QDialog>

namespace Ui {
class HelpDialog;
}

class HelpDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HelpDialog(QWidget *parent = nullptr);
    ~HelpDialog();

private:
    Ui::HelpDialog *ui;
};

#endif // HELPDIALOG_H
