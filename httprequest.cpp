#include "httprequest.h"

#include <QDebug>

HttpRequest::HttpRequest(QObject *parent) :
 QObject(parent)
{
  connect(&m_WebCtrl, &QNetworkAccessManager::finished,
          this, &HttpRequest::dataReceived);
}

void HttpRequest::sendRequest(QUrl url)
{
  QNetworkRequest request(url);
  m_WebCtrl.get(request);
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
