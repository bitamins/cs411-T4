#ifndef CUSTOMDOWNLOADMANAGER_H
#define CUSTOMDOWNLOADMANAGER_H

#include <QtCore>
#include <QtNetwork>
#include <QString>
#include <QPixmap>
#include <QDebug>

#include <cstdio>

class QSslError;

using namespace std;


class CustomDownloadManager: public QObject
{
    Q_OBJECT
    QNetworkAccessManager manager;
    QVector<QNetworkReply *> currentDownloads;

private:
    bool imageReady;

public:
    CustomDownloadManager();
    QPixmap downloadImage(const QUrl &url);
    void startDownload(const QUrl &url);
    bool saveFileToDisk(const QString &fileame, QIODevice *data);
    static bool isHttpRedirect(QNetworkReply *reply);
    QString saveFileName(const QUrl &url);


public slots:
    bool imageDLcomplete(QNetworkReply *reply);
    void execute();
    void downloadFinished(QNetworkReply *reply);
    void sslErrors(const QList<QSslError> &errors);
};

#endif // CUSTOMDOWNLOADMANAGER_H
