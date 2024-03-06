#ifndef HISTOGRAMCLASS_H
#define HISTOGRAMCLASS_H

#include <QVector>
#include <QString>
#include <QObject>
#include <QList>
#include <gsl/gsl_histogram.h>
#include "qcustomplot.h"
#include "dataset.h"

namespace Ui{
class HistogramClass;
}

class HistogramClass : public QDialog
{
    Q_OBJECT

public:
    explicit HistogramClass(QList<DataSet*> AllDataSets, QWidget *parent=nullptr);
    ~HistogramClass();
    void PlotHistogram();

public slots:
    void setHistogramSetting();

signals:
    void histogramLoaded();

private:
    Ui::HistogramClass* ui;
    gsl_histogram* histogram;
    QVector<double> data;
    void clearHistogram();
    QList<DataSet*> OpenDataSets;

};

#endif // HISTOGRAMCLASS_H
