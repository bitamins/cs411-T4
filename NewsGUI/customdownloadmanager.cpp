#include "customdownloadmanager.h"


CustomDownloadManager* CustomDownloadManager::_instance = 0;
int CustomDownloadManager::count = 0;

CustomDownloadManager* CustomDownloadManager::Instance()
{
 if ( _instance == 0 )
    _instance = new CustomDownloadManager();
 return _instance;
}

//create a slot for finished downloads
CustomDownloadManager::CustomDownloadManager()
{
    connect(&manager, SIGNAL(finished(QNetworkReply*)),
            SLOT(downloadFinished(QNetworkReply*)));
}
CustomDownloadManager::~CustomDownloadManager(){
    delete _instance;
}

int CustomDownloadManager::getCount(){
    return count;
}

//download an image
void CustomDownloadManager::startDownload(const QUrl &url)
{
    if(count <= 4){
        QNetworkRequest request(url);
        QNetworkReply *reply = manager.get(request);

        //check errors in link or security
    /*
      #if QT_CONFIG(ssl)
        connect(reply, SIGNAL(sslErrors(QList<SslError>)),
                SLOT(sslErrors(QList<QSslError>)));
      #endif
    */
        //append the download
        currentDownloads.append(reply);
        count++;
       }
    else{qDebug()<<"wait for current image downloads";}
}


QString CustomDownloadManager::saveFileName(const QUrl &url)
{
    //QDir dir;
    QString path = url.path();
    QString saveFilePath;
    QStringList pathList = path.split('/');
    QString newName = pathList.at(pathList.count() - 1);
    saveFilePath = QString("imageFiles/" + newName);

    //qDebug() << "URL________ " << url.path() << endl;
    QString basename = QFileInfo(saveFilePath).fileName();

    if(basename.isEmpty()){
        basename = "download";
    }
    /* if we do not want to overwrite files
    if(QFile::exists(basename)){
        int i = 0;
        basename += '.';
        while(QFile::exists(basename + QString::number(i))){
            i++;
        }
        basename += QString::number(i);
    }
    */
    return basename;

}

bool CustomDownloadManager::saveFileToDisk(const QString &filename, QIODevice *data)
{
    QFile file(filename);
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
    //qDebug() << "download finished: " << reply->url() << endl;
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
                //printf("Download of %s succeded (save to %s)\n\n",
                       url.toEncoded().constData(),qPrintable(filename);
                emit imageDownloaded(filename);
            }
        }
      }
    currentDownloads.removeAll(reply);
    reply->deleteLater();
    count--;

    if(currentDownloads.isEmpty()){
        qDebug() << "all image downloads complete.\n";
        //no current downloads
        //QCoreApplication::instance()->quit();
    }
}
