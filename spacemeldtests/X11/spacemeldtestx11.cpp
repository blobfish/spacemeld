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
#include <ctime>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

int main(int argc, char *argv[])
{
  if (argc < 2)
    std::cout << "This test runs on timer and defaults to 10 seconds." << std::endl <<
    "This timer can be set with a 2nd parameter like: 'spacemeldtestx11 20'. runs for 20 seconds." << std::endl;
  std::cout << "use spaceball to show numbers." << std::endl << std::endl;
  
  Display *display = XOpenDisplay("");
  if(display == NULL)
  {
    std::cout << "Error: couldn't open display" << std::endl;
    return 0;
  }
  
  int defaultScreen = DefaultScreen(display);
  Window defaultWindow = DefaultRootWindow(display);
  unsigned long foreground = WhitePixel(display, defaultScreen);
  unsigned long background = BlackPixel(display, defaultScreen);
  
  Window window = XCreateSimpleWindow(display, defaultWindow, 0, 0, 1, 1, 0, foreground, background);
  
  Atom xEventMotion = XInternAtom(display, "MotionEvent", True);
  Atom xEventButtonPress = XInternAtom(display, "ButtonPressEvent", True);
  Atom xEventButtonRelease = XInternAtom(display, "ButtonReleaseEvent", True);
  Atom xEventCommand = XInternAtom(display, "CommandEvent", True);
  if(!(xEventMotion && xEventButtonPress && xEventButtonRelease && xEventCommand))
  {
    std::cout << "Error: Atoms not found" << std::endl;
    return 0;
  }
  
  Atom actualType;
  int actualFormat;
  unsigned long itemCount, bytesReturn;
  unsigned char *propReturn;
  XGetWindowProperty(display, defaultWindow, xEventCommand, 0, 1, false, AnyPropertyType, &actualType,
                      &actualFormat, &itemCount, &bytesReturn, &propReturn);
  if (propReturn == NULL)
  {
    std::cout << "Error: Couldn't get driver window" << std::endl;
    return 0;
  }
  Window daemonWindow = *(reinterpret_cast<Window *>(propReturn));
  XFree(propReturn);
  XTextProperty daemonWindowNameProperty;
  if (!XGetWMName(display, daemonWindow, &daemonWindowNameProperty))
  {
    std::cout << "Error: couldn't get daemon window name" << std::endl;
    return 0;
  }
  std::string daemonWindowName(reinterpret_cast<char *>(daemonWindowNameProperty.value));
  if (daemonWindowName != "Magellan Window")
  {
    std::cout << "Error: wrong window name" << std::endl;
    return 0;
  }
  
  XEvent xevent;
  xevent.type = ClientMessage;
  xevent.xclient.send_event = False;
  xevent.xclient.display = display;
  xevent.xclient.window = window;
  xevent.xclient.message_type = xEventCommand;
  xevent.xclient.format = 16;
  xevent.xclient.data.s[0] = ((unsigned int)window & 0xffff0000) >> 16;
  xevent.xclient.data.s[1] = (unsigned int)window & 0xffff;
  xevent.xclient.data.s[2] = 27695;

  XSendEvent(display, daemonWindow, False, 0, &xevent);
  XSync(display, False);
  
  //event loop here.
  double duration = 10; //in seconds;
  if (argc > 1)
  {
    std::string tempString(argv[1]);
    double tempDuration = std::stod(tempString);
    if (tempDuration > 1.0)
      duration = tempDuration;
  }
  std::time_t startTime = std::time(nullptr);
  for(;;)
  {
    std::time_t currentTime = std::time(nullptr); 
    if (std::difftime(currentTime, startTime) > duration)
      break;
    
    XEvent currentEvent;
    XNextEvent(display, &currentEvent);
    if (currentEvent.type != ClientMessage)
      continue;
    if (currentEvent.xclient.message_type == xEventMotion)
    {
      std::cout << "Motion:    " << 
        currentEvent.xclient.data.s[2] << "    " <<
        currentEvent.xclient.data.s[3] << "    " <<
        currentEvent.xclient.data.s[4] << "    " <<
        currentEvent.xclient.data.s[5] << "    " <<
        currentEvent.xclient.data.s[6] << "    " <<
        currentEvent.xclient.data.s[7] << std::endl;
    }
    if (currentEvent.xclient.message_type == xEventButtonPress)
    {
      std::cout << "Button Pressed: " << currentEvent.xclient.data.s[2] << std::endl;
    }
    if (currentEvent.xclient.message_type == xEventButtonRelease)
    {
      std::cout << "Button Released: " << currentEvent.xclient.data.s[2] << std::endl;
    }
  }
  
  XDestroyWindow(display ,window);
  XCloseDisplay(display);
  
  return 0;
}
