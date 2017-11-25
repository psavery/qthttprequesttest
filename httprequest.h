#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include <memory>
#include <mutex>
#include <unordered_map>

#include <QObject>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class HttpRequestManager : public QObject
{
 Q_OBJECT
 public:
  explicit HttpRequestManager(
      const std::shared_ptr<QNetworkAccessManager>& networkManager,
      QObject *parent = nullptr);

  // Sends a URL request and returns an index that is used to access the
  // obtained data.
  size_t sendRequest(QUrl url);

  // Check to see if data has been received for reply index @p i.
  bool containsData(size_t i) const;

  // Get the received data for a particular reply index @p i. Call
  // "containsData()" to make sure the data was actually received first.
  const QByteArray& data(size_t i) const;

  // If data for a particular reply index @p i exists, erase it from the map.
  void eraseData(size_t i) { m_receivedReplies.erase(i); }

 signals:
  // When a reply is received for a particular index, this signal will be
  // emitted with the index.
  void received(size_t);

 private slots:
  // When the network manager receives a reply, this slot will be activated.
  // This function will make sure that the reply belongs to this object before
  // doing anything with it.
  void dataReceived(QNetworkReply* reply);

 private:
  // A shared pointer to the network access manager.
  std::shared_ptr<QNetworkAccessManager> m_networkManager;

  // The map of pending replies accessed via their unique index
  std::unordered_map<size_t, QNetworkReply*> m_pendingReplies;

  // The map of received replies accessed via their unique index
  std::unordered_map<size_t, QByteArray> m_receivedReplies;

  // This mutex gets locked for all reading and writing operations
  mutable std::mutex m_mutex;

  // A counter for the request indices
  size_t m_requestCounter;
};

#endif // HTTPREQUEST_H
