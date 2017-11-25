#include "httprequest.h"

#include <QDebug>
#include <QNetworkRequest>

HttpRequestManager::HttpRequestManager(
    const std::shared_ptr<QNetworkAccessManager>& networkManager,
    QObject* parent) :
 m_networkManager(networkManager),
 m_requestCounter(0),
 QObject(parent)
{
  connect(m_networkManager.get(), &QNetworkAccessManager::finished,
          this, &HttpRequestManager::dataReceived);
}

size_t HttpRequestManager::sendRequest(QUrl url)
{
  std::unique_lock<std::mutex> lock(m_mutex);

  QNetworkRequest request(url);
  QNetworkReply* reply = m_networkManager->get(request);
  m_pendingReplies[m_requestCounter] = reply;

  ++m_requestCounter;
}

size_t HttpRequestManager::sendPost(QUrl url, const QByteArray& data)
{
  std::unique_lock<std::mutex> lock(m_mutex);

  QNetworkRequest request(url);
  request.setHeader(QNetworkRequest::ContentTypeHeader,
                    "application/x-www-form-urlencoded");
  QNetworkReply* reply = m_networkManager->post(request, data);
  m_pendingReplies[m_requestCounter] = reply;

  ++m_requestCounter;
}

bool HttpRequestManager::containsData(size_t i) const
{
  std::unique_lock<std::mutex> lock(m_mutex);
  return m_receivedReplies.find(i) != m_receivedReplies.end();
}

const QByteArray& HttpRequestManager::data(size_t i) const
{
  std::unique_lock<std::mutex> lock(m_mutex);

  static const QByteArray empty = "";

  if (m_receivedReplies.find(i) == m_receivedReplies.end())
    return empty;

  return m_receivedReplies.at(i);
}

void HttpRequestManager::dataReceived(QNetworkReply* reply)
{
  std::unique_lock<std::mutex> lock(m_mutex);

  // Make sure this HttpRequestManager owns this reply
  auto it = std::find_if(m_pendingReplies.begin(),
                         m_pendingReplies.end(),
                         [reply](const std::pair<size_t, QNetworkReply*>& item)
                         {
                           return reply == item.second;
                         });

  // If not, just return
  if (it == m_pendingReplies.end())
    return;

  size_t receivedInd = it->first;

  m_receivedReplies[receivedInd] = reply->readAll();

  qDebug() << "Received data!\n";
  qDebug() << "Recieved data is:\n" << m_receivedReplies[receivedInd];

  m_pendingReplies.erase(receivedInd);

  reply->deleteLater();

  // Emit a signal
  emit received(receivedInd);
}
