#!/bin/bash
#
# $Id: win32-setup.sh 48979 2013-04-22 20:49:02Z pascal $

# 32-bit wrapper for win-setup.sh.

export DOWNLOAD_TAG="2013-04-22"
export WIRESHARK_TARGET_PLATFORM="win32"

WIN_SETUP=`echo $0 | sed -e s/win32/win/`

exec $WIN_SETUP $@
