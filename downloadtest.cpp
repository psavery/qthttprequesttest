
#include <QCoreApplication>

#include "httprequest.h"

int main(int argc, char* argv[])
{
  QCoreApplication app(argc, argv);

  QUrl url("https://google.com");
  HttpRequest req;

  req.sendRequest(url);

  app.connect(&req, &HttpRequest::received,
              &app, &QCoreApplication::quit);

  return app.exec();
}
