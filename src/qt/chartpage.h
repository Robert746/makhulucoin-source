#ifndef CHARTPAGE_H
#define CHARTPAGE_H

#include "clientmodel.h"
#include "uint256.h"

#include <QWidget>

#include <QDir>
#include <QFile>
#include <QProcess>
#include <QTime>
#include <QTimer>
#include <QStringList>
#include <QMap>
#include <QSettings>

#include <QMainWindow>

namespace Ui {
class ChartPage;
}

class ChartPage : public QMainWindow
{
    Q_OBJECT

public:
    void DrawChart();
    explicit ChartPage(QWidget *parent = 0);
    ~ChartPage();

private:

    int CountLine(const char* filename);
    void GetMaxMin(int line, int firstTime, double *max, double *min, double *data);
    void GetRawMiningDataFromFile(double *data, char* filename);
    void GetDrawInfo(int line, int width, int height, int* lineDraw, double *maxdb, int *max, double *rawdata, double *drawdata);
    Ui::ChartPage *ui;
};

#endif // CHARTPAGE_H
