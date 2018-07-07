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

//#ifndef DOWNLOADMANAGER_H
//#define DOWNLOADMANAGER_H

//#include <QFile>
//#include <QObject>
//#include <QQueue>
//#include <QTime>
//#include <QUrl>
//#include <QNetworkAccessManager>
//#include <QCoreApplication>
//#include <QFileInfo>
//#include <QNetworkRequest>
//#include <QNetworkReply>
//#include <QString>
//#include <QStringList>
//#include <QTimer>
//#include <stdio.h>

//#include <QDir>
//#include <QCoreApplication>
//#include <QFile>
//#include <QFileInfo>
//#include <QList>
//#include <QNetworkAccessManager>
//#include <QNetworkRequest>
//#include <QNetworkReply>
//#include <QSslError>
//#include <QStringList>
//#include <QTimer>
//#include <QUrl>
//#include <QObject>
//#include <QByteArray>
//#include <QNetworkAccessManager>
//#include <QNetworkRequest>
//#include <QNetworkReply>
//#include <QMessageBox>
//#include "aboutdialog.h"

//class DownloadManager: public QObject
//{
//    Q_OBJECT

//public:
//    void startRequest(QUrl url);
//    void downloadFile(QString url, bool notice);
//public slots:
//    // slot for readyRead() signal
//    void httpReadyRead();

//    // slot for finished() signal from reply
//    void httpDownloadFinished();

//    // slot for downloadProgress()
//    void updateDownloadProgress(qint64, qint64);

//    void cancelDownload();

//private:
//    QUrl url;
//    QNetworkAccessManager *manager;
//    QNetworkReply *reply;
//    QFile *file;
//    bool httpRequestAborted;
//    qint64 fileSize;
//};


////QT_BEGIN_NAMESPACE
////class QSslError;
////QT_END_NAMESPACE

////QT_USE_NAMESPACE

////class DownloadManager: public QObject
////{

////    Q_OBJECT
////public:
////    explicit DownloadManager(QUrl imageUrl, QObject *parent = 0);

////    //virtual ~DownloadManager();

////    QByteArray downloadedData() const;

////    void downloadFile(QString url, bool notice);
////signals:
////        void downloaded();

////private slots:
////    void noticeMessage();
////    void fileDownloaded(QNetworkReply* pReply);

////private:

////    QNetworkAccessManager m_WebCtrl;

////    QByteArray m_DownloadedData;

//////    Q_OBJECT
//////    QNetworkAccessManager manager;
//////    QList<QNetworkReply *> currentDownloads;

//////public:
//////    DownloadManager();
//////    void doDownload(const QUrl &url);
//////    QString saveFileName(const QUrl &url);
//////    bool saveToDisk(const QString &filename, QIODevice *data);
//////    void downloadFile(QString url, bool notice);
//////public slots:
//////    void execute(QString url);
//////    void downloadFinished(QNetworkReply *reply);
//////    void sslErrors(const QList<QSslError> &errors);
//////    void noticeMessage();
////};

////class DownloadManager : public QObject
////{
////    Q_OBJECT

////public:
////    DownloadManager(const QUrl &downloadUrl, const QDir &storageLocation,
////                        bool startImmediately = false);

////    DownloadManager() : QObject() {}

////    inline void setFile(QFile *file) { pFile = file; }
////    inline void setStorageLocation(const QDir &location) { mStorageLocation = location; }
////    inline void setDownloadURL(const QUrl &url) { mDownloadUrl = url; }

////    inline QFile* file() const { return pFile; }
////    inline QDir storageLocation() const { return mStorageLocation; }
////    inline QUrl downloadUrl() const { return mDownloadUrl; }
////    void downloadFile(QString url, bool notice);

////signals:
////    void fileAlreadyExistsError(QString existingFileDisplayName);
////    void fileOpeningError(QString errorMessage);
////    void fileDownloadError(QString errorMessage);
////    void fileDownloadStarted();
////    void fileDownloadCancelled();
////    void fileDownloadFinished();

////public slots:
////    void setDownloadURLSlot(const QString &url);

////    void beginDownload();
////    void cancelDownload();
////    void downloadFinished();
////    void downloadReadyRead();

////private:
////    QFile *pFile;
////    QDir mStorageLocation;

////    QUrl mDownloadUrl;

////    QNetworkReply *pNetworkReply;
////    QNetworkAccessManager mNetworkManager;
////};

//#endif
