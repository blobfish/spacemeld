#SpaceMeld is a driver/API for spaceballs, spacemice, spacenavigators etc..
#Copyright (C) 2014 Thomas Anderson blobfish[at]gmx.com
#
#This file is part of SpaceMeld.
#
#SpaceMeld is free software: you can redistribute it and/or modify
#it under the terms of the GNU General Public License as published by
#the Free Software Foundation, either version 3 of the License, or
#(at your option) any later version.
#
#SpaceMeld is distributed in the hope that it will be useful,
#but WITHOUT ANY WARRANTY; without even the implied warranty of
#MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#GNU General Public License for more details.
#
#You should have received a copy of the GNU General Public License
#along with SpaceMeld.  If not, see <http://www.gnu.org/licenses/>.

project(spacemeldtestqlocalsocket)
message("\nConfiguring qlocalsocket test")
set(CMAKE_AUTOMOC TRUE)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/spacemeldcommon)

set(TESTQLOCALSOCKET_SRCS
      spacemeldtestqlocalsocket.cpp
      sockettester.cpp
   )

add_executable(spacemeldtestqlocalsocket ${TESTQLOCALSOCKET_SRCS})
TARGET_LINK_LIBRARIES(spacemeldtestqlocalsocket Qt5::Network Qt5::Core)
