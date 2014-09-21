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

#ifndef DEVICESERIALBALL4000FLX_H
#define DEVICESERIALBALL4000FLX_H

#include "deviceserialball.h"

class DeviceSerialBall4000FLX : public DeviceSerialBall
{
    Q_OBJECT
public:
    virtual bool launch();
    virtual void processPacket(const QByteArray &packet);
    friend DeviceBase* createSerialBall4000FLX(QObject *parent, const DeviceInfo &infoIn,
                                               const QSerialPortInfo &portInfoIn);

protected:
    explicit DeviceSerialBall4000FLX(QObject *parent, const DeviceInfo &infoIn, const QSerialPortInfo &portInfoIn);
    void processDisplacementPacket(const QByteArray &packet);
    void processButtonPacket(const QByteArray &packet);

private:
    bool initialize();

};

DeviceBase* createSerialBall4000FLX(QObject *parent, const DeviceInfo &infoIn, const QSerialPortInfo &portInfoIn);

#endif // DEVICESERIALBALL4000FLX_H
