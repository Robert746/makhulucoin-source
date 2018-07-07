#include "aboutdialog.h"
#include "ui_aboutdialog.h"
#include "main.h"
#include "clientmodel.h"
#include "clientversion.h"

using namespace boost;


// Copyright year (2009-this)
// Todo: update this when changing our copyright comments in the source
const int ABOUTDIALOG_COPYRIGHT_YEAR = 2014;

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);

    // Set current copyright year
    ui->copyrightLabel->setText(tr("Copyright") + QString(" &copy; 2009-%1 ").arg(COPYRIGHT_YEAR) + tr("The Bitcoin developers") + QString("<br>") + tr("Copyright") + QString(" &copy; ") + tr("2011-%1 The LiteCoin developers").arg(ABOUTDIALOG_COPYRIGHT_YEAR)
                                + QString("<br>") + tr("Copyright") + QString(" &copy; %1 ").arg(COPYRIGHT_YEAR) + tr("The VertCoin developers")
                                + QString("<br>") + tr("Copyright") + QString(" &copy; %1 ").arg(COPYRIGHT_YEAR) + tr("The MakhuluCoin developers"));
}

void AboutDialog::setModel(ClientModel *model)
{
    this->model = model;
    if(model)
    {
        ui->versionLabel->setText(model->formatFullVersion());
    }

    if(isAutomaticUpdate())
        ui->cbAutomatic->setChecked(true);
}

void AboutDialog::setBitcoinGuide(BitcoinGUI *gui)
{
    this->gui = gui;
    connect(this, SIGNAL(startDownload(QString, int)), gui, SLOT(startDownload(QString, int)));
    connect(gui, SIGNAL(timeout()), this, SLOT(timeout()));
    connect(gui, SIGNAL(downloadFinish()), this, SLOT(downloadFinish()));
    connect(gui, SIGNAL(downloadVersionFinish()), this, SLOT(downloadVersionFinish()));
    connect(gui, SIGNAL(updateDownloadProgressSignal(qint64,qint64)), this, SLOT(updateDownloadProgress(qint64,qint64)));
    connect(gui,SIGNAL(nothingChanged()),this,SLOT(nothingChanged()));
    connect(gui, SIGNAL(httpConnectionTimeOutSignal(QNetworkReply::NetworkError)), this, SLOT(httpConnectionTimeOut(QNetworkReply::NetworkError)));
}

AboutDialog::~AboutDialog()
{
    delete ui;
}

void AboutDialog::on_buttonBox_accepted()
{
    if(changed)
    {
//        QMessageBox msgBox;
//        msgBox.setText("Please restart your wallet to take change");
//        int reply = msgBox.exec();
    }
    close();
}

void AboutDialog::on_btnCheck_clicked()
{
    ui->btnCheck->setEnabled(false);
    ui->btnCheck->setText("Checking");
    emit startDownload("http://makhulucoin.info/version.ini", 4);
}

void AboutDialog::updateDownloadProgress(qint64 bytesRead, qint64 totalBytes)
{
    downloading++;
    if(notice)
    {
        ui->btnCheck->setEnabled(false);
        if(downloading == 1)
            ui->btnCheck->setText("Connecting...");
        if(downloading == 2)
            ui->btnCheck->setText("Downloading.");
        if(downloading == 3)
            ui->btnCheck->setText("Downloading..");
        if(downloading == 4)
        {
            ui->btnCheck->setText("Downloading...");
            downloading = 1;
        }
        ui->lblProcess->setText(QString::number(bytesRead) + "/" + QString::number(totalBytes));
    }
}

bool AboutDialog::isAutomaticUpdate()
{
    //update check autoupdate radio button
    boost::filesystem::path pathData = GetDataDir() / "data.xml";
    QString autoUpdateVal = xmlGen.ReadXml("AutoUpdate", pathData.string().c_str());
    if(autoUpdateVal == "True")
        return true;
    return false;
}

void AboutDialog::downloadFinish()
{
    ui->btnCheck->setEnabled(true);
    ui->btnCheck->setText("Check");
    ui->lblProcess->setText("Finished");
    QMessageBox msgBox;
    msgBox.setText("The new wallet was downloaded to your wallet folder. Reason: " + gui->getUpdatReason());
    msgBox.exec();
    downloading = 0;
}

void AboutDialog::httpConnectionTimeOut(QNetworkReply::NetworkError error)
{
    if(error == QNetworkReply::TimeoutError)
    {
        ui->btnCheck->setText("Check");
        ui->lblProcess->setText("Time out");
    }
    ui->btnCheck->setEnabled(true);
}

void AboutDialog::downloadVersionFinish()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirm!!!", "Do you want to download new wallet version. Reason: " + gui->getUpdatReason(),
                                QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        ui->btnCheck->setEnabled(false);
        notice = true;
        emit startDownload("http://makhulucoin.info", 1);
    }
    else if(reply == QMessageBox::No)
    {
        connect(model, SIGNAL(startDownload(QString, int)), gui, SLOT(startDownload(QString, int)));
    }
}

void AboutDialog::on_cbAutomatic_clicked()
{
    boost::filesystem::path pathDebug = GetDataDir() / "data.xml";
    if(ui->cbAutomatic->isChecked())
    {
        xmlGen.WriteXML("AutoUpdate","True",pathDebug.string().c_str());
        connect(model, SIGNAL(startDownload(QString, int)), gui, SLOT(startDownload(QString, int)));
    }
    else
        xmlGen.WriteXML("AutoUpdate","False",pathDebug.string().c_str());
}

void AboutDialog::nothingChanged()
{
    QMessageBox msgBox;
    msgBox.setText("You are using the newest version");
    msgBox.exec();
    ui->btnCheck->setText("Check");
    ui->btnCheck->setEnabled(true);
}

void AboutDialog::timeout()
{
    ui->btnCheck->setText("Check");
    ui->btnCheck->setEnabled(true);
}
