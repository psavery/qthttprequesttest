
#include <memory>

#include <QCoreApplication>

#include "httprequestmanager.h"

int main(int argc, char* argv[])
{
  QCoreApplication app(argc, argv);

  auto netManager = std::make_shared<QNetworkAccessManager>();
  HttpRequestManager req(netManager);

  QUrl url("https://google.com");
  req.sendGet(url);

  app.connect(&req, &HttpRequestManager::received,
              &app, &QCoreApplication::quit);

  return app.exec();
}
