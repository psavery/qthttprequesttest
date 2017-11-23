#include "httprequest.h"

#include <QDebug>

HttpRequest::HttpRequest(QNetworkAccessManager* networkManager,
                         QObject *parent) :
 m_networkManager(networkManager),
 QObject(parent)
{
  connect(m_networkManager, &QNetworkAccessManager::finished,
          this, &HttpRequest::dataReceived);
}

void HttpRequest::sendRequest(QUrl url)
{
  QNetworkRequest request(url);
  m_networkManager->get(request);
}

void HttpRequest::dataReceived(QNetworkReply* pReply)
{
  m_receivedData = pReply->readAll();

  qDebug() << "Received data!\n";
  qDebug() << "Recieved data is:\n" << m_receivedData;

  //emit a signal
  pReply->deleteLater();
  emit received();
}
