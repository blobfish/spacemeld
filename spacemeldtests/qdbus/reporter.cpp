/*
SpaceMeld is a driver/API for spaceballs, spacemice, spacenavigators etc..
Copyright (C) 2014 Thomas Anderson blobfish[at]gmx.com

This file is part of SpaceMeld.

SpaceMeld is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

SpaceMeld is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with SpaceMeld.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <iostream>
#include <sstream>

#include "reporter.h"

Reporter::Reporter(const QString &nameIn, QObject* parent) : QObject(parent), name(nameIn)
{

}

void Reporter::displacementSlot(short axis0, short axis1, short axis2,
                                short axis3, short axis4, short axis5)
{
  std::ostringstream stream;
  stream << name.toStdString() << " displacement: " << axis0 << " " << axis1 << " " << axis2 <<
    " " << axis3 << " " << axis4 << " " << axis5 << std::endl;
  std::cout << stream.str();
}

void Reporter::buttonSlot(uchar buttonNumber, bool buttonDown)
{
  //std::cout doesn't like uchar.
  
  std::ostringstream stream;
  stream << name.toStdString() << " button: " << static_cast<int>(buttonNumber) << ((buttonDown) ? " pressed" : " released") << std::endl;
  std::cout << stream.str();
}
