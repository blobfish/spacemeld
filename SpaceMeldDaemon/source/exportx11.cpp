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

#include <qglobal.h>
#include <QTimer>
#include <QDebug>
#include <QCoreApplication>

#if defined(Q_WS_X11) && defined(SPACEMELD_BUILD_EXPORT_X11_MAG)

#include "exportx11.h"
#include <X11/keysym.h>


ExportX11::ExportX11(QObject *parent) :
    ExportBase(parent)
{
    display = 0;
    xWindow = 0;
}

ExportX11::~ExportX11()
{
    finish();
}

ExportX11* ExportX11::instance()
{
    static ExportX11 *pointer = new ExportX11(QCoreApplication::instance());
    if (!pointer->initializedTest)
        pointer->initialize();
    return pointer;
}

bool ExportX11::initialize()
{
    display = XOpenDisplay(NULL);
    Q_ASSERT(display);

    XSetErrorHandler(xError);
    XSetIOErrorHandler(xInputOutputError);

    xEventMotion = XInternAtom(display, "MotionEvent", False);
    xEventButtonPress = XInternAtom(display, "ButtonPressEvent", False);
    xEventButtonRelease = XInternAtom(display, "ButtonReleaseEvent", False);
    xEventCommand = XInternAtom(display, "CommandEvent", False);

    Window root = RootWindow(display, DefaultScreen(display));

    xWindow = XCreateWindow(display, root, 0, 0, 100, 100, 0, 0, InputOutput, CopyFromParent, 0, 0);

    char windowName[] = "Magellan Window";
    char *ptr = windowName;
    XTextProperty textPropertyName;
    XStringListToTextProperty(&ptr, 1, &textPropertyName);
    XSetWMName(display, xWindow, &textPropertyName);
    XFree(textPropertyName.value);

    Atom temp = XInternAtom(display, "MagellanCommand", False);
    XChangeProperty(display, root, xEventCommand, temp, 32, PropModeReplace, (unsigned char*)&xWindow, 1);
    XFlush(display);

    QTimer *xTimer = new QTimer(this);
    xTimer->setInterval(100);
    QObject::connect(xTimer, SIGNAL(timeout()), this, SLOT(xEventsIn()));
    xTimer->start();

    initializedTest = true;
    return true;
}

void ExportX11::finish()
{
    XDestroyWindow(display, xWindow);
    XCloseDisplay(display);
    display = 0;
}

int ExportX11::xError(Display *aDisplay, XErrorEvent *anError)
{
    qDebug() << "x error code: " << anError->error_code;
    return 0;
}

int ExportX11::xInputOutputError(Display *aDisplay)
{
    qDebug() << "X input output error.";
    return 0;
}

void ExportX11::xEventsIn()
{
    while (XPending(display))
    {
//        qDebug() << "got x event";
        XEvent event;
        XNextEvent(display, &event);
        if (event.type == ClientMessage && event.xclient.message_type == xEventCommand)
        {
            if (event.xclient.data.s[2] == Magellan::SetWindowCommand)
            {
                quint32 windowId = static_cast<quint32>((event.xclient.data.s[0] << 16) | event.xclient.data.s[1]);
                if (!clients.contains(windowId))
                    clients.push_back(static_cast<Window>(windowId));
            }
        }
    }
}

void ExportX11::displacementIn(qint16 a0, qint16 a1, qint16 a2, qint16 a3, qint16 a4, qint16 a5)
{
//    qDebug() << "Displacement: " << values;

    XEvent event;
    event.type = ClientMessage;
    event.xclient.display = display;
    event.xclient.send_event = False;
    event.xclient.message_type = xEventMotion;
    event.xclient.format = 16;
    event.xclient.data.s[0] = event.xclient.data.s[1] = 0;

    event.xclient.data.s[2] = a0;
    event.xclient.data.s[3] = a1;
    event.xclient.data.s[4] = a2;
    event.xclient.data.s[5] = a3;
    event.xclient.data.s[6] = a4;
    event.xclient.data.s[7] = a5;

    QVector<Window>::iterator it;
    for (it = clients.begin(); it != clients.end(); ++it)
    {
        event.xclient.window = *it;
        XSendEvent(display, *it, False, 0, &event);
    }
    XFlush(display);
}

void ExportX11::buttonIn(qint8 buttonNumber, bool buttonDown)
{
    if (buttonKeyMap.contains(buttonNumber))
    {
        if (!buttonKeyMap.value(buttonNumber).isEmpty())
        {
            sendKeyMessage(buttonNumber, buttonDown);
            return;
        }
    }
    sendButtonMessage(buttonNumber, buttonDown);
}

void ExportX11::sendKeyMessage(qint8 buttonNumber, bool buttonDown)
{
    //only send on button down.
    if (!buttonDown)
        return;
    //note on X meta and alt appear to be the same.
    //run xmodmap on terminal to discover.
    QString string = buttonKeyMap.value(buttonNumber);
    quint32 modState(0);
    if (string.contains("Ctrl"))
        modState |= ControlMask;
    if (string.contains("Shift"))
        modState |= ShiftMask;
    if (string.contains("Alt") || string.contains("Meta"))
        modState |= Mod1Mask;

    string.remove("Alt");
    string.remove("Shift");
    string.remove("Ctrl");
    string.remove("Meta");
    string.remove("+");

    if (string.size() != 1)
    {
        qDebug() << "string size is not 1 in ExportX11::sendKeyMessage";
        return;
    }

    int inputKey = static_cast<int>(string.at(0).toAscii());

    XKeyEvent event;
    event.type = KeyPress;
    event.root = XDefaultRootWindow(display);
    event.subwindow = None;
    event.display = display;
    event.same_screen = True;
    event.keycode = XKeysymToKeycode(display, inputKey);
    event.same_screen = True;
    event.state = modState;

    QVector<Window>::iterator it;
    for (it = clients.begin(); it != clients.end(); ++it)
    {
        event.window = *it;
        XSendEvent(display, *it, True, KeyPressMask, (XEvent*)(&event));
    }
    XFlush(display);
}

void ExportX11::sendButtonMessage(qint8 buttonNumber, bool buttonDown)
{
    //    qDebug() << "Button: " << buttonNumber << ((buttonDown) ? " pressed" : " released");

        XEvent event;
        event.type = ClientMessage;
        event.xclient.display = display;
        event.xclient.send_event = False;
        event.xclient.message_type = (buttonDown) ? xEventButtonPress : xEventButtonRelease;
        event.xclient.format = 16;
        event.xclient.data.s[2] = static_cast<qint16>(buttonNumber);

        QVector<Window>::iterator it;
        for (it = clients.begin(); it != clients.end(); ++it)
        {
            event.xclient.window = *it;
            XSendEvent(display, *it, False, 0, &event);
        }
        XFlush(display);
}

void ExportX11::setButtonMap(const DeviceInfo &info)
{
    buttonKeyMap = info.exports.at(static_cast<int>(OutputType::X11)).buttonKeyMap;
}

#endif
