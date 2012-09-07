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

#include <QCoreApplication>
#include <QDebug>

#if defined(Q_WS_WIN) && defined(SPACEMELD_BUILD_WIN_MAG)
#include "exportwinmag.h"

static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
//    switch(msg)
//    {
//        case WM_CLOSE:
//            DestroyWindow(hwnd);
//        break;
//        case WM_DESTROY:
//            PostQuitMessage(0);
//        break;
//        default:
//            return DefWindowProc(hwnd, msg, wParam, lParam);
//    }

    ExportWinMag *pointer = reinterpret_cast<ExportWinMag *>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    if (!pointer)
        return DefWindowProc(hwnd, msg, wParam, lParam);
    if (!(pointer->isInitialized()))
        return DefWindowProc(hwnd, msg, wParam, lParam);
    if(msg == pointer->winEventCommand)
    {
//        qDebug() << "Have win event command " << msg;
        if (wParam == Magellan::SetWindowCommand)
        {
            HWND temp = reinterpret_cast<HWND>(lParam);
            if (temp)
               pointer->addWindow(temp);
            else
                qDebug() << "couldn't cast lParam to hwnd";
        }
    }


    return DefWindowProc(hwnd, msg, wParam, lParam);
}

ExportWinMag::ExportWinMag(QObject *parent) :
    ExportBase(parent)
{
}

bool ExportWinMag::initialize()
{
    qDebug() << "initializing ExportWinMag";

    wchar_t winClassName[32];
    int size  = QString::fromAscii(WIN_MAG_CLASS_NAME).toWCharArray(winClassName);
    winClassName[size] = '\0';

    wchar_t winWindowName[32];
    size = QString::fromAscii(WIN_MAG_WINDOW_NAME).toWCharArray(winWindowName);
    winWindowName[size] = '\0';

    hInstance = (HINSTANCE)::GetModuleHandle(NULL);

    WNDCLASSEX wc;
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = 0;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = NULL;
    wc.hCursor = NULL;
    wc.hbrBackground = NULL;
    wc.lpszMenuName = NULL;
    wc.lpszClassName = winClassName;
    wc.hIconSm = NULL;

    if(!RegisterClassEx(&wc))
    {
        qDebug() << "Window Registration Failed in ExportWinMag::initialize(). last error: " << GetLastError();
        return false;
    }
//    qDebug() << "Window Registration worked";

    hwnd = CreateWindowEx(NULL, winClassName, winWindowName, NULL,
                          CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0,
                          NULL, hInstance, NULL);
    if(hwnd == NULL)
    {
        qDebug() << "Create window failed in ExportWinMag::initialize(). last error: " << GetLastError();
        return false;
    }
//    qDebug() << "Create Window worked";

    SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

    wchar_t motionName[32];
    size = QString::fromAscii("MotionEvent").toWCharArray(motionName);
    motionName[size] = '\0';

    wchar_t buttonPressName[32];
    size = QString::fromAscii("ButtonPressEvent").toWCharArray(buttonPressName);
    buttonPressName[size] = '\0';

    wchar_t buttonReleaseName[32];
    size = QString::fromAscii("ButtonReleaseEvent").toWCharArray(buttonReleaseName);
    buttonReleaseName[size] = '\0';

    wchar_t commandName[32];
    size = QString::fromAscii("CommandEvent").toWCharArray(commandName);
    commandName[size] = '\0';

    winEventMotion = RegisterWindowMessage(motionName);
    winEventButtonPress = RegisterWindowMessage(buttonPressName);
    winEventButtonRelease = RegisterWindowMessage(buttonReleaseName);
    winEventCommand = RegisterWindowMessage(commandName);

    if (!winEventMotion || !winEventButtonPress || !winEventButtonRelease || !winEventCommand)
    {
        qDebug() << "failed registering events. last error is: " << GetLastError();
        return false;
    }

//    qDebug() << "winEventCommand: " << winEventCommand;

    initializedTest = true;
    return true;
}

ExportWinMag* ExportWinMag::instance()
{
    static ExportWinMag *pointer = new ExportWinMag(QCoreApplication::instance());
    if (!pointer->initializedTest)
        pointer->initialize();
    return pointer;
}

void ExportWinMag::setButtonMap(const DeviceInfo &info)
{
    buttonKeyMap = info.exports.at(static_cast<int>(OutputType::WIN)).buttonKeyMap;
}

void ExportWinMag::displacementIn(QVector<qint16> values)
{
    DWORD translations = 0;
    translations = (values.at(0) & 0x07FF | (values.at(1) & 0x07FF) << 11 | (values.at(2) & 0x03FF) << 22);

    DWORD rotations = 0;
    rotations = (values.at(3) & 0x07FF |(values.at(4) & 0x07FF) << 11 | (values.at(5) & 0x03FF) << 22);

    QList<HWND>::Iterator it;
    for (it = clients.begin(); it != clients.end();)
    {
        //there is some controversy on the usage of iswindow
        //very basic test shows this working.
        if (!IsWindow(*it))
        {
            it = clients.erase(it);
            continue;
        }
        PostMessage((*it), winEventMotion, translations, rotations);
        ++it;
    }
}

void ExportWinMag::buttonIn(qint8 buttonNumber, bool buttonDown)
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

void ExportWinMag::sendButtonMessage(qint8 buttonNumber, bool buttonDown)
{
    QList<HWND>::Iterator it;
    for (it = clients.begin(); it != clients.end();)
    {
        //there is some controversy on the usage of iswindow
        //very basic test shows this working.
        if (!IsWindow(*it))
        {
            it = clients.erase(it);
            continue;
        }
        if (buttonDown)
            PostMessage((*it), winEventButtonPress, (WPARAM) buttonNumber + 1, (LPARAM) 0);
        else
            PostMessage((*it), winEventButtonRelease, (WPARAM) buttonNumber + 1, (LPARAM) 0);
        ++it;
    }
}

void ExportWinMag::sendKeyMessage(qint8 buttonNumber, bool buttonDown)
{
    //only send on button down.
    if (!buttonDown)
        return;
}

#endif //Q_WS_WIN
