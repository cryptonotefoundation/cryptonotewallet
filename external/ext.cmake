# Copyright (c) 2019 The Karbowanec developers
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.
#
# Code formatting based on CS106B Style


set(EXT_DIR_NAME "external")

# Qrencode library section
set(QRENCODE_LIB qrencode)
set(QRENCODE_LIB_DIR "libqrencode")

file(GLOB QRENCODE_SOURCES RELATIVE ${CMAKE_SOURCE_DIR} ${EXT_DIR_NAME}/${QRENCODE_LIB_DIR}/*.c)
list(REMOVE_ITEM QRENCODE_SOURCES ${QRENCODE_LIB_DIR}/qrenc.c)

list(APPEND qrencode_definitions "STATIC_IN_RELEASE=static")
list(APPEND qrencode_definitions "MAJOR_VERSION=4")
list(APPEND qrencode_definitions "MINOR_VERSION=0")
list(APPEND qrencode_definitions "MICRO_VERSION=0")
list(APPEND qrencode_definitions QRENCODE_VERSION="${MAJOR_VERSION}.${MINOR_VERSION}.${MICRO_VERSION}")

if (WIN32)
  list(APPEND qrencode_definitions "inline=__inline")
endif()

include_directories("${EXT_DIR_NAME}/${QRENCODE_LIB_DIR}")
add_library(${QRENCODE_LIB} STATIC ${QRENCODE_SOURCES})
set_target_properties(${QRENCODE_LIB} PROPERTIES COMPILE_DEFINITIONS "${qrencode_definitions}")
