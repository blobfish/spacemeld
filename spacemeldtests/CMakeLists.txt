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


find_package(X11)
if(X11_FOUND)
  add_subdirectory(X11)
endif(X11_FOUND)

find_package(Qt5DBus)
find_package(Qt5Core)
if(Qt5DBus_FOUND AND Qt5Core_FOUND)
  add_subdirectory(qdbus)
endif(Qt5DBus_FOUND AND Qt5Core_FOUND)
