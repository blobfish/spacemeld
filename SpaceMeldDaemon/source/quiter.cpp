#include "quiter.h"

quiter::quiter(QObject *parent) :
    QObject(parent)
{
}

void quiter::timout()
{
    emit quitApp();
}
