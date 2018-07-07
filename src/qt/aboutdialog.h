#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>
#include "generatexml.h"
#include "bitcoingui.h"

namespace Ui {
    class AboutDialog;
}
class ClientModel;

/** "About" dialog box */
class AboutDialog : public QDialog
{
    Q_OBJECT

public:
    GenerateXML xmlGen;
    explicit AboutDialog(QWidget *parent = 0);
    ~AboutDialog();

    void setModel(ClientModel *model);
    void setBitcoinGuide(BitcoinGUI *gui);
    bool isAutomaticUpdate();
private:
    ClientModel *model;
    Ui::AboutDialog *ui;
    bool notice = true;
    bool changed = false;
    int downloading = 0;
    BitcoinGUI *gui;
signals:
    void startDownload(QString url, int type);

private slots:
    void timeout();
    void nothingChanged();
    void httpConnectionTimeOut(QNetworkReply::NetworkError error);
    void downloadVersionFinish();
    void downloadFinish();
    void updateDownloadProgress(qint64 bytesRead, qint64 totalBytes);
    void on_buttonBox_accepted();
    void on_btnCheck_clicked();
    void on_cbAutomatic_clicked();
};

#endif // ABOUTDIALOG_H
