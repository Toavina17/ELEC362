#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

/********************************
 *
 *  This class is defined for the "About" dialog,
 *  an object of this class is the window of the "About" dialog in the app
 *
**********************************/


#include <QDialog>

namespace Ui {
class AboutDialog;
}

class AboutDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AboutDialog(QWidget *parent = nullptr);
    ~AboutDialog();

private:
    Ui::AboutDialog *ui;
};

#endif // ABOUTDIALOG_H
