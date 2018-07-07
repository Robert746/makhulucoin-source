#include "chartpage.h"
#include "ui_chartpage.h"
#include "main.h"

#include <QFileDialog>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPointF>
#include <QVector>
#include <Qpainter>
#include <QGraphicsItem>

using namespace boost;

ChartPage::ChartPage(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ChartPage)
{
    ui->setupUi(this);
    DrawChart();
}

ChartPage::~ChartPage()
{
    delete ui;
}

void ChartPage::DrawChart()
{
    // create a scene and add it your view
    QGraphicsScene* scene = new QGraphicsScene();
    ui->graphicsView->setScene(scene);

    static int height = 300;
    static int width = 1000;

    boost::filesystem::path pathDebug = GetDataDir() / "debugblc.log";
    int line = CountLine(pathDebug.string().c_str());

    double *rawdata = new double[line * 2];
    double *drawdata = new double[line * 2 + 4]{0};
    int lineDraw = 1;
    int max;
    double maxdb;

    GetRawMiningDataFromFile(rawdata, (char*)pathDebug.string().c_str());
    GetDrawInfo(line, height, width, &lineDraw, &maxdb, &max, rawdata, drawdata);

    QPen pen(Qt::green, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    QPen pen2(Qt::black, 1, Qt::DashLine, Qt::RoundCap, Qt::RoundJoin);
    QPen pen3(Qt::red, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);

    //draw horizontal line
    for (int y = 60; y <= height + 60; y += 60)
    {
        scene->addLine(0, -y, width, -y, pen2);
        if(y != 0 && y != 360)
        {
            QGraphicsTextItem * item;
            item = ui->graphicsView->scene()->addText (QString::number(maxdb * (y / 60) / 5 / COIN));
            item->setPos(0, -y - 20);
        }
    }
    QGraphicsTextItem * item;
    item = ui->graphicsView->scene()->addText ("0");
    item->setPos(0, -20);

    scene->addLine(0, 0, width, 0, pen3);
    scene->addLine(0, 0, 0, -height - 60, pen3);

    item = ui->graphicsView->scene()->addText ("24h");
    item->setPos(0, 0);
    //draw vertical line
    for (int x = 125; x < width; x+=125)
    {
        if(x != width)
            scene->addLine(x, 0, x, -height - 60, pen2);
        QGraphicsTextItem * item;
        item = ui->graphicsView->scene()->addText (QString::number((8 - x / 125) * 3) + "h");
        item->setPos(x, 0);
    }

    //draw chart
    for (int x = 0; x < lineDraw - 1; x++)
    {
        int x1 = drawdata[x * 2];
        int y1 = -drawdata[x * 2 + 1];
        int x2 = drawdata[x * 2 + 2];
        int y2 = -drawdata[x * 2 + 3];
        scene->addLine(x1, y1, x2, y2, pen);
    }

    ui->graphicsView->fitInView(scene->itemsBoundingRect());//,Qt::KeepAspectRatio);
    delete(rawdata);
    delete(drawdata);
}

int ChartPage::CountLine(const char* filename)
{
    FILE *fd;
    int lineNumb = 0;
    char buff[1000];
    if ((fd = fopen(filename, "r")) != NULL) // open file
   {
       fseek(fd, 0, SEEK_SET); // make sure start from 0

       while(!feof(fd))
       {
           lineNumb++;
           fgets(buff,1000,fd);
       }
       fclose(fd);
       return lineNumb;
   }
   else
   {
       return 0;
   }
}

void ChartPage::GetMaxMin(int line, int firstTime, double *max, double *min, double *data)
{
    *max = data[line * 2 - 1];
    *min = data[line * 2 - 1];
    for(int i = 0; i < line; i++)
    {
        if(data[i * 2] >= firstTime)
        {
            if(data[i * 2 + 1] < *min)
                *min = data[i * 2 + 1];
            if(data[i * 2 + 1] > *max)
                *max = data[i * 2 + 1];
        }
    }
}

//convert from raw data to draw data
void ChartPage::GetDrawInfo(int line, int height, int width, int* lineDraw, double *maxdb, int *max, double *rawdata, double *drawdata)
{
    double lattestCoin = rawdata[(line -1) * 2 + 1];
    double firstCoin = rawdata[1];
    int64 currentTime = GetTime();
    int64 firstTime = currentTime - 86400;
    double maxdb1, mindb;
    double rank = 0;

    for(int i = 0; i < line; i++)
    {
        if(rawdata[i * 2] >= firstTime)
        {
            firstCoin = rawdata[i * 2 + 1];
            break;
        }
    }
    GetMaxMin(line, firstTime, &maxdb1, &mindb, rawdata);
    rank = maxdb1 - 0;
    *max = maxdb1 / rank * height;
    *maxdb = maxdb1;
    drawdata[0] = 0;
    drawdata[1] = firstCoin / rank * height;

    for(int i = 0; i < line; i++)
    {
        if(rawdata[i * 2] >= firstTime)
        {
            drawdata[*lineDraw * 2] = (rawdata[i * 2] - firstTime) / 86400 * width; //get time
            drawdata[*lineDraw * 2 + 1] = rawdata[i * 2 + 1] / rank * height;
            (*lineDraw)++;
        }
    }
    drawdata[*lineDraw * 2] = (currentTime - firstTime) / 86400 * width;
    drawdata[*lineDraw * 2 + 1] = lattestCoin / rank * height;
    (*lineDraw)++;
}

void ChartPage::GetRawMiningDataFromFile(double *data, char* filename)
{
    char buff[1000];
    char date[1000];
    char balance[1000];
    int line = CountLine(filename);
    FILE *fd;
    if ((fd = fopen(filename, "r")) != NULL) // open file
   {
       fseek(fd, 0, SEEK_SET);
   }
    else
        return;
    for(int i = 0; i < line; i++)
    {
        fgets(buff,1000,fd); // read file
        char * pch;
        pch = strtok (buff," ");
        int index = 0;
        while (pch != NULL)
        {
            if(index == 0)
                strcpy(date,pch);
            else if(index == 1)
                strcpy(balance,pch);
            pch = strtok (NULL, " ");
            index++;
        }
        data[i * 2] = atof(date);
        data[i * 2 + 1] = atof(balance);
    }
}

