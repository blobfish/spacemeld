#include "interfacebase.h"

InterfaceBase::InterfaceBase(QObject *parent, bool enabledIn) : QObject(parent), enabled(enabledIn)
{
}
