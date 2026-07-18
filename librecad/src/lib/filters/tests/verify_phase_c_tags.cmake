# ********************************************************************************
# This file is part of the LibreCAD project, a 2D CAD program
#
# Copyright (C) 2026 LibreCAD.org
# Copyright (C) 2026 Dongxu Li (github.com/dxli)
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301,
# USA.
# ********************************************************************************

if(NOT DEFINED TEST_EXECUTABLE OR NOT EXISTS "${TEST_EXECUTABLE}")
    message(FATAL_ERROR "TEST_EXECUTABLE must name the built librecad_tests binary")
endif()

foreach(tag IN ITEMS "[block-insert]" "[wipeout-native-frame]")
    execute_process(
        COMMAND "${TEST_EXECUTABLE}" --list-tests "${tag}"
        RESULT_VARIABLE result
        OUTPUT_VARIABLE listed
        ERROR_VARIABLE errors)
    if(NOT result EQUAL 0 OR listed MATCHES "No test cases matched" OR
       NOT listed MATCHES "test case")
        message(FATAL_ERROR "Required Phase C Catch2 tag ${tag} is empty: ${errors}${listed}")
    endif()
endforeach()
