#include "customdownloadmanager.h"

//create a slot for finished downloads
CustomDownloadManager::CustomDownloadManager()
{
    connect(&manager, SIGNAL(finished(QNetworkReply*)),
            SLOT(downloadFinished(QNetworkReply*)));
    imageReady = false;
}

bool CustomDownloadManager::imageDLcomplete(QNetworkReply *reply){
    imageReady = true;
}

//download an image
void CustomDownloadManager::startDownload(const QUrl &url)
{
    QNetworkRequest request(url);
    QNetworkReply *reply = manager.get(request);

    //check errors in link or security
  #if QT_CONFIG(ssl)
    connect(reply, SIGNAL(sslErrors(QList<SslError>)),
            SLOT(sslErrors(QList<QSslError>)));
  #endif

    //append the download
    currentDownloads.append(reply);
}

//do not use this function
QPixmap CustomDownloadManager::downloadImage(const QUrl &url)
{
    QNetworkRequest request(url);
    QNetworkReply *reply = manager.get(request);

    //check errors in link or security

#if QT_CONFIG(ssl)
    connect(reply, SIGNAL(sslErrors(QList<QSslError>)),
            SLOT(sslErrors(QList<QSslError>)));
  #endif

    //append the download
   QByteArray jpegData = reply->readAll();
   QPixmap pm;
   pm.loadFromData(jpegData);
   return pm;
}


QString CustomDownloadManager::saveFileName(const QUrl &url)
{
    QString path = url.path();
    qDebug() << url.path();
    QString basename = QFileInfo(path).fileName();

    if(basename.isEmpty()){
        basename = "download";
    }
    if(QFile::exists(basename)){
        int i = 0;
        basename += '.';
        while(QFile::exists(basename + QString::number(i))){
            i++;
        }
        basename += QString::number(i);
    }
    return basename;

}

bool CustomDownloadManager::saveFileToDisk(const QString &filename, QIODevice *data)
{
    QString folderstring = "newsImages/";
    folderstring.append(filename);
    QFile file(folderstring);
    if(!file.open(QIODevice::WriteOnly)){
        fprintf(stderr, "could not open %s for writing: %s\n",
                qPrintable(filename),
                qPrintable(file.errorString()));
        return false;
    }
    file.write(data->readAll());
    file.close();

    return true;
}

//bool CustomDownloadManager::getFileFromDisk(const QString &filename, QIODevice *data)

bool CustomDownloadManager::isHttpRedirect(QNetworkReply *reply)
{
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    return statusCode == 301 ||
            statusCode == 301 ||
            statusCode == 303 ||
            statusCode == 305 ||
            statusCode == 307 ||
            statusCode == 308;
}

void CustomDownloadManager::execute()
{
    QStringList args = QCoreApplication::instance()->arguments();
    args.takeFirst();
    if(args.isEmpty()){
        printf("No arguements passed\n");
        QCoreApplication::instance()->quit();
        return;
    }
    for(const QString &arg : qAsConst(args)){
        QUrl url = QUrl::fromEncoded(arg.toLocal8Bit());
        startDownload(url);
    }

}


void CustomDownloadManager::sslErrors(const QList<QSslError> &sslErrors){
#if QT_CONFIG(ssl)
    for(const QSslError &error : sslErrors){
        fprintf(stderr,"SSL error: %s\n",qPrintable(error.errorString()));
    }
#else
    Q_UNUSED(sslErrors);
#endif
}

void CustomDownloadManager::downloadFinished(QNetworkReply *reply){
    qDebug() << "download finished " << endl;
    QUrl url = reply->url();
    if(reply->error()){
        fprintf(stderr, "Download of %s failed: %s\n",
                url.toEncoded().constData(),
                qPrintable(reply->errorString()));
    }else{
        if(isHttpRedirect(reply)){
            fputs("Request was redirected.\n",stderr);
        }else{
            QString filename = saveFileName(url);
            if(saveFileToDisk(filename,reply)){
                printf("Download of %s succeded (save to %s)\n",
                       url.toEncoded().constData(),qPrintable(filename));
            }
        }
      }
    currentDownloads.removeAll(reply);
    reply->deleteLater();

    if(currentDownloads.isEmpty()){
        //no current downloads
        QCoreApplication::instance()->quit();
    }
}
