#   This file is part of SpaceMeld.
#
#   SpaceMeld is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#
#   SpaceMeld is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with SpaceMeld.  If not, see <http://www.gnu.org/licenses/>.

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
