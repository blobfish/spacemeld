#This file contains common parameters for multiple spacemeld projects.
#Any custom build parameters will also be located here.

#relative path to qtservice pri.
#This path should be updated to reflect the qtservice.pri location.
include(../../../../QtSolutions/qt-solutions/qtservice/src/qtservice.pri)

#path is relative to the file including this file and not where this file is located.
#This should be fine as long as directory structure of project/s is original.
INCLUDEPATH += ../../SpaceMeldCommon

#control the building of the X11 output. A system test combined with this user parameter will
#control whether X11 output is built or not. Remarking the following line will turn off X11 support even on a
#X11 platform.
SPACE_MELD_CONFIG += USER_BUILD_X11
