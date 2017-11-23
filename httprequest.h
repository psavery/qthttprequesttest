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
  explicit HttpRequest(QNetworkAccessManager* networkManager,
                       QObject *parent = nullptr);
  QByteArray data() const { return m_receivedData; }

  void sendRequest(QUrl url);

 signals:
  void received();

 private slots:
  void dataReceived(QNetworkReply* pReply);

 private:
  // A pointer to the network manager. The network manager should not be
  // deleted until the HttpRequest object is deleted.
  QNetworkAccessManager* m_networkManager;
  QByteArray m_receivedData;
};

#endif // HTTPREQUEST_H
