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

class ExportBase : public QObject
{
    Q_OBJECT
public:
    explicit ExportBase(QObject *parent = 0);
    virtual bool initialize() = 0;
    
signals:
    
public slots:
    
};

#endif // EXPORTBASE_H
