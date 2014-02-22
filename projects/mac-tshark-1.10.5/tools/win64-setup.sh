#!/bin/bash
#
# $Id: win64-setup.sh 48979 2013-04-22 20:49:02Z pascal $

# 64-bit wrapper for win-setup.sh.

export DOWNLOAD_TAG="2013-04-22"
export WIRESHARK_TARGET_PLATFORM="win64"

WIN_SETUP=`echo $0 | sed -e s/win64/win/`

exec $WIN_SETUP $@
