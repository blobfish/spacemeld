#include <iostream>
#include <assert.h>

#include <QtCore/QCoreApplication>
#include <QtCore/QTimer>
#include <QtCore/QDebug>
#include <QtNetwork/QLocalSocket>

#include "sockettester.h"

int main(int argc, char *argv[])
{
  int duration = 10;
  if (argc < 2)
    std::cout << "This test runs on timer and defaults to 10 seconds." << std::endl <<
    "This timer can be set with a 2nd parameter like: 'spacemeldtestqlocalsocket 20'. runs for 20 seconds." << std::endl;
  else
  {
    std::string tempString(argv[1]);
    double tempDuration = std::stod(tempString);
    if (tempDuration > 1.0)
      duration = tempDuration;
  }
  
  int dummyArgCount = 1;
  QCoreApplication app(dummyArgCount, argv);
  
  SocketTester tester;
  std::cout << std::endl << "Use spaceball to test signals." << std::endl << std::endl;
  QTimer::singleShot(duration * 1000, &app, SLOT(quit()));
  app.exec();
  
  return 0;
}
