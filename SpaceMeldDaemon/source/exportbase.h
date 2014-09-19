/*
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

#ifndef EXPORTBASE_H
#define EXPORTBASE_H

#include <QObject>

//To do: if possible move initialize to private. as of right now, initialize is called
// by the static instance function and doesn't need to be public.

namespace Magellan
{
enum Commands {SetWindowCommand = 27695, ApplicationSensitivity, RingBell,
                               ApplicationStarts, ModeChange, NullRadiusChange,
                               ControlPanel, InternSendCommand};
}

class ExportBase : public QObject
{
    Q_OBJECT
public:
    explicit ExportBase(QObject *parent = 0);
    virtual ~ExportBase(){}
    virtual bool initialize() = 0;
    bool isInitialized(void){return initializedTest;}
    
protected:
    bool initializedTest;
    
};

#endif // EXPORTBASE_H
