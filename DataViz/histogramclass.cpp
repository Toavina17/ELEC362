#include "histogramclass.h"
#include "ui_histogramclass.h"

HistogramClass::HistogramClass(QList<DataSet*> AllDataSets, QWidget *parent):
    QDialog(parent),
    ui(new Ui::HistogramClass)
{
    ui->setupUi(this);
    qDebug() << "ui setup";

    //populate the comboBox
    for(int i = 0; i < AllDataSets.size(); i++){
        ui->comboBox->addItem(AllDataSets[i]->getName());
        //OpenDataSets[i] = AllDataSets[i];
        OpenDataSets.append(AllDataSets[i]);
    }
    //connect the button ok

    connect(ui->okButton, SIGNAL(clicked()), this, SLOT(setHistogramSetting()));
}


HistogramClass::~HistogramClass(){
    //called when the window of the figure is closed
    delete ui;
    gsl_histogram_free(histogram);
}

void HistogramClass::setHistogramSetting(){

    qDebug() << "setHistogramSetting function called";

    ui->customPlot->clearPlottables();

    QString selectedData = ui->comboBox->currentText();
    DataSet* chosenDataSet = nullptr;

    // Find the chosen dataset
    for (DataSet* data : OpenDataSets) {
        if (data->getName() == selectedData) {
            chosenDataSet = data;
            break;
        }
    }

    // Ensure that the dataset is valid
    if (!chosenDataSet) {
        qDebug() << "Selected dataset not found or is null.";
        return;
    }

    // Free the existing histogram if it was allocated previously
    if (histogram) {
        gsl_histogram_free(histogram);
    }

    // Put the information from the dataSet into a QVector
    QVector<double> D;
    for (int i = 0; i < chosenDataSet->Size(); i++) {
        D.push_back(*chosenDataSet->getPoint(i));
    }

    const int kHistogramBinNumber = 5; // Number of bins in the histogram


    // Calculate the bin width
    double dataMin = *std::min_element(D.constBegin(), D.constEnd());
    double dataMax = *std::max_element(D.constBegin(), D.constEnd());
    double binWidth = (dataMax - dataMin) / kHistogramBinNumber;

    // Allocate memory for the histogram
    histogram = gsl_histogram_alloc(kHistogramBinNumber);

    // Set up the histogram with non-overlapping bins
    gsl_histogram_set_ranges_uniform(histogram, dataMin, dataMax + binWidth);

    // Populate the histogram with data
    for (int i = 0; i < D.size(); i++) {
        gsl_histogram_increment(histogram, D[i]);
    }

    // Plot the histogram
    PlotHistogram();
}

void HistogramClass::PlotHistogram(){


    qDebug() << "PlotHistogram function called";

    if (!histogram) {
        // If there is no histogram plotted then return
        return;
    }

    //ui->customPlot->clearPlottables(); // Clear previous plots

    QCPBars *bars = new QCPBars(ui->customPlot->xAxis, ui->customPlot->yAxis);


    // Get bin centers and counts from the GSL histogram
    QVector<double> binCenters, binCounts;
    for (size_t i = 0; i < gsl_histogram_bins(histogram); i++) {
        double lower, upper;
        gsl_histogram_get_range(histogram, i, &lower, &upper);
        double center = 0.5 * (lower + upper);

        binCenters.append(center);
        binCounts.append(gsl_histogram_get(histogram, i));
    }

    // Set up the bars
    bars->setData(binCenters, binCounts);

    bars->setWidthType(QCPBars::wtPlotCoords);
    double width = gsl_histogram_max(histogram) - gsl_histogram_min(histogram);
    bars->setWidth(width);

    // Set some properties for better visualization (you can customize these)
    bars->setAntialiased(false);
    bars->setPen(Qt::NoPen);
    bars->setBrush(QColor(120, 120, 255, 200)); // Adjust color and transparency as needed

    // Set axis labels
    ui->customPlot->xAxis->setLabel("Bin Centers");
    ui->customPlot->yAxis->setLabel("Bin Counts");

    // Set axis ranges (adjust as needed)
    ui->customPlot->xAxis->setRange(binCenters.first() - 0.5 * width, binCenters.last() + 0.5 * width);
    ui->customPlot->yAxis->setRange(0, *std::max_element(binCounts.begin(), binCounts.end()) + 1);

    // Set plot title
    ui->customPlot->plotLayout()->insertRow(0);
    ui->customPlot->plotLayout()->addElement(0, 0, new QCPTextElement(ui->customPlot, "Histogram Plot", QFont("sans", 12, QFont::Bold)));

    // Replot the custom plot
    ui->customPlot->replot();

}

