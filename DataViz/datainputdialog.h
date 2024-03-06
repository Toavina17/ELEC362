// datainputdialog.h

/*
 * This section creates a new Dataset by inputting x and y values
 */
#ifndef DATAINPUTDIALOG_H
#define DATAINPUTDIALOG_H

#include <QDialog>
#include <QTableWidget>
#include <QPushButton>
#include <QPointF>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>


class DataInputDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DataInputDialog(QWidget *parent = nullptr);
    ~DataInputDialog();
    QList<QPointF> getDataPoints();

private slots:
    void addDataPoint();
    void removeDataPoint();
    void doneButtonClicked();

private:
    QTableWidget *tableWidget;
    QPushButton *addButton;
    QPushButton *removeButton;
    QPushButton *doneButton;

};
#endif // DATAINPUTDIALOG_H
