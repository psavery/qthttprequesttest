#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include <QObject>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

class HttpRequest : public QObject
{
 Q_OBJECT
 public:
  explicit HttpRequest(QObject *parent = nullptr);
  QByteArray data() const { return m_receivedData; }

  void sendRequest(QUrl url);

 signals:
  void received();

 private slots:
  void dataReceived(QNetworkReply* pReply);

 private:
  QNetworkAccessManager m_WebCtrl;
  QByteArray m_receivedData;
};

#endif // HTTPREQUEST_H
