///****************************************************************************
//**
//** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
//** Contact: http://www.qt-project.org/legal
//**
//** This file is part of the examples of the Qt Toolkit.
//**
//** $QT_BEGIN_LICENSE:BSD$
//** You may use this file under the terms of the BSD license as follows:
//**
//** "Redistribution and use in source and binary forms, with or without
//** modification, are permitted provided that the following conditions are
//** met:
//**   * Redistributions of source code must retain the above copyright
//**     notice, this list of conditions and the following disclaimer.
//**   * Redistributions in binary form must reproduce the above copyright
//**     notice, this list of conditions and the following disclaimer in
//**     the documentation and/or other materials provided with the
//**     distribution.
//**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
//**     of its contributors may be used to endorse or promote products derived
//**     from this software without specific prior written permission.
//**
//**
//** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
//** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
//** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
//** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
//** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
//** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
//** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
//** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
//** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
//** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
//** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
//**
//** $QT_END_LICENSE$
//**
//****************************************************************************/

//#include "downloadmanager.h"


//void DownloadManager::httpReadyRead()
//{
//    // this slot gets called every time the QNetworkReply has new data.
//    // We read all of its new data and write it into the file.
//    // That way we use less RAM than when reading it at the finished()
//    // signal of the QNetworkReply
//    if (file)
//        file->write(reply->readAll());
//}

//void DownloadManager::updateDownloadProgress(qint64 bytesRead, qint64 totalBytes)
//{
//    if (httpRequestAborted)
//        return;

//    //progressDialog->setMaximum(totalBytes);
//    //progressDialog->setValue(bytesRead);
//}

//// During the download progress, it can be canceled
//void DownloadManager::cancelDownload()
//{
//    //ui->statusLabel->setText(tr("Download canceled."));
//    httpRequestAborted = true;
//    reply->abort();
//    //ui->downloadButton->setEnabled(true);
//}

//// When download finished or canceled, this will be called
//void DownloadManager::httpDownloadFinished()
//{
//    // when canceled
//    if (httpRequestAborted) {
//        if (file) {
//            file->close();
//            file->remove();
//            delete file;
//            file = 0;
//        }
//        reply->deleteLater();
//        //progressDialog->hide();
//        return;
//    }

//    // download finished normally
//    //progressDialog->hide();
//    file->flush();
//    file->close();

//    // get redirection url
//    QVariant redirectionTarget = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
//    if (reply->error()) {
//        file->remove();
////        QMessageBox::information(this, tr("HTTP"),
////                                 tr("Download failed: %1.")
////                                 .arg(reply->errorString()));
//        //ui->downloadButton->setEnabled(true);
//    } else if (!redirectionTarget.isNull()) {
//        QUrl newUrl = url.resolved(redirectionTarget.toUrl());
////        if (QMessageBox::question(this, tr("HTTP"),
////                                  tr("Redirect to %1 ?").arg(newUrl.toString()),
////                                  QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
////            url = newUrl;
////            reply->deleteLater();
////            file->open(QIODevice::WriteOnly);
////            file->resize(0);
////            startRequest(url);
////            return;
////        }
//    } else {
//        //QString fileName = QFileInfo(QUrl(ui->urlEdit->text()).path()).fileName();
//        //ui->statusLabel->setText(tr("Downloaded %1 to %2.").arg(fileName).arg(QDir::currentPath()));
//        //ui->downloadButton->setEnabled(true);
//    }

//    reply->deleteLater();
//    reply = 0;
//    delete file;
//    file = 0;
//    manager = 0;
//}

//// This will be called when download button is clicked
//void DownloadManager::startRequest(QUrl url)
//{
//    // get() method posts a request
//    // to obtain the contents of the target request
//    // and returns a new QNetworkReply object
//    // opened for reading which emits
//    // the readyRead() signal whenever new data arrives.
//    reply = manager->get(QNetworkRequest(url));

//    // Whenever more data is received from the network,
//    // this readyRead() signal is emitted
//    connect(reply, SIGNAL(readyRead()),
//            this, SLOT(httpReadyRead()));

//    // Also, downloadProgress() signal is emitted when data is received
//    connect(reply, SIGNAL(downloadProgress(qint64,qint64)),
//            this, SLOT(updateDownloadProgress(qint64,qint64)));

//    // This signal is emitted when the reply has finished processing.
//    // After this signal is emitted,
//    // there will be no more updates to the reply's data or metadata.
//    connect(reply, SIGNAL(finished()),
//            this, SLOT(httpDownloadFinished()));
//}

//void DownloadManager::downloadFile(QString url, bool notice)
//{
//    manager = new QNetworkAccessManager(this);

//    // get url
//    //url = (ui->urlEdit->text());
//    QUrl url1(url);
//    QFileInfo fileInfo(url1.path());
//    QString fileName = fileInfo.fileName();

//    if (fileName.isEmpty())
//        fileName = "index.html";

//    if (QFile::exists(fileName)) {
////        if (QMessageBox::question(this, tr("HTTP"),
////                tr("There already exists a file called %1 in "
////                "the current directory. Overwrite?").arg(fileName),
////                QMessageBox::Yes|QMessageBox::No, QMessageBox::No)
////                == QMessageBox::No)
////                return;
//        QFile::remove(fileName);
//    }

//    file = new QFile(fileName);
//    if (!file->open(QIODevice::WriteOnly)) {
////        QMessageBox::information(this, tr("HTTP"),
////                      tr("Unable to save the file %1: %2.")
////                      .arg(fileName).arg(file->errorString()));
//        delete file;
//        file = 0;
//        return;
//    }

//    // used for progressDialog
//    // This will be set true when canceled from progress dialog
//    httpRequestAborted = false;

//    //progressDialog->setWindowTitle(tr("HTTP"));
//    //progressDialog->setLabelText(tr("Downloading %1.").arg(fileName));

//    // download button disabled after requesting download
//    //ui->downloadButton->setEnabled(false);

//    startRequest(url);
////    QObject::connect(&manager, SIGNAL(finished()), &app, SLOT(quit()));
////    app.exec();

////    QEventLoop loop;
////    connect(manager, SIGNAL(finished()), &loop, SLOT(quit()));
////    loop.exec();

////    QNetworkAccessManager *manager=new QNetworkAccessManager;
////    //QString url="ftp://ftp.trolltech.com/qt/source/INSTALL";
////    QNetworkReply *reply = manager->get(QNetworkRequest(url));
////    QEventLoop loop;
////    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
////    loop.exec();
////    QString filename="ttttt.zip";
////    QFile file(filename);
////    file.open(QIODevice::WriteOnly);
////    file.write(reply->readAll());
////    delete reply;

////    QDir directory("C:/Users/Mr.Lov3/Desktop/gene");
////    DownloadManager(url, directory, true);
////    QUrl imageUrl(url);
////    m_pImgCtrl = new DownloadManager(imageUrl, this);

////    connect(m_pImgCtrl, SIGNAL(downloaded()), SLOT(noticeMessage()));

////    DownloadManager manager;
////    QTimer::singleShot(0, &manager, SLOT(execute(url)));
////    manager.doDownload(url);
////    QNetworkRequest request(url);
////    currentDownload = manager.get(request);
////    connect(currentDownload, SIGNAL(downloadProgress(qint64,qint64)),
////            SLOT(downloadProgress(qint64,qint64)));
////    connect(currentDownload, SIGNAL(finished()),
////            SLOT(downloadFinished()));
////    connect(currentDownload, SIGNAL(readyRead()),
////            SLOT(downloadReadyRead()));
////    QObject app;
////    QStringList arguments;
////    arguments.append(url);
////    if (arguments.isEmpty()) {
////        return;
////    }

////    manager.append(arguments);
////    connect(&manager, SIGNAL(finished()), this, SLOT(noticeMessage()));
//}

////void DownloadManager::noticeMessage()
////{
////    int i = 1;
////    i++;
////    i++;
////}
