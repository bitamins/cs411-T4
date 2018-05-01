#ifndef DownloadManager_H
#define DownloadManager_H

#include <QtCore>
#include <QtNetwork>
#include <QString>
#include <QPixmap>
#include <QDebug>
#include <QDir>
#include <cstdio>

class QSslError;

using namespace std;

//used qt5 documentation example http://doc.qt.io/qt-5/qtnetwork-download-main-cpp.html
class DownloadManager: public QObject
{
    Q_OBJECT
    QNetworkAccessManager manager;
    QVector<QNetworkReply *> currentDownloads;

private:
    string imageDirectory;
    static DownloadManager* _instance;
    static int count;

protected:
    DownloadManager();

public:
    ~DownloadManager();
    static DownloadManager* Instance();
    void startDownload(const QUrl &url);
    bool saveFileToDisk(const QString &fileame, QIODevice *data);
    static bool isHttpRedirect(QNetworkReply *reply);
    QString saveFileName(const QUrl &url);
    int getCount();

public slots:
    void execute();
    void downloadFinished(QNetworkReply *reply);
    void sslErrors(const QList<QSslError> &errors);

signals:
    void imageDownloaded(QString filename);

};

#endif // DownloadManager_H
