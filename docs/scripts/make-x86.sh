#!/bin/bash
# author: JaeJoon Jung <rgbi3307@nate.com> on the www.kernel.bz

# Makefile, KBUILD_CFLAGS, -save-temps=obj
# x86_64: CC_HAVE_ASM_GOTO := 1, -DCC_HAVE_ASM_GOTO

#CPU_CNT=$(grep processor /proc/cpuinfo | awk '{field=$NF};END{print field+2}')
CPU_CNT=$(nproc)
START_TIME=`date +%s`

#VER=[v4.14, v5.4, v5.10, v5.15, v6.1, v6.6]
VER=$1
#BRD=[hp7100, hp-white, hp-purple]
BRD=$2
#CMD=[menuconfig, clean, distclean, tag, log, strip]
CMD=$3

ARCH="x86_64"
COMPILER=""

BUILD_PATH="../build/kernel-$VER-$BRD"
LOG_PATH="../build/log"
LOG_FILE="$LOG_PATH/kernel-$VER-$BRD.txt"
TAG_FILE="$BUILD_PATH/tags"
STRIP="$COMPILER""strip"

if [ ! -d $BUILD_PATH ]; then
    echo "mkdir $BUILD_PATH"
    mkdir $BUILD_PATH
    chmod 755 $BUILD_PATH
fi

if [ ! -d $LOG_PATH ]; then
    echo "mkdir $LOG_PATH"
    mkdir $LOG_PATH
    chmod 755 $LOG_PATH
fi

if [ ! -f $BUILD_PATH/.config ]; then
    echo "make defconfig"
    make ARCH=$ARCH O=$BUILD_PATH defconfig 
fi

if [ $CMD == "log" ]; then
    make O=$BUILD_PATH CROSS_COMPILE=$COMPILER ARCH=$ARCH > $LOG_FILE

elif [ $CMD == "logfast" ]; then
    make -j$CPU_CNT O=$BUILD_PATH CROSS_COMPILE=$COMPILER ARCH=$ARCH > $LOG_FILE

elif [ $CMD == "tag" ]; then
    make tags -j$CPU_CNT O=$BUILD_PATH CROSS_COMPILE=$COMPILER ARCH=$ARCH
#    echo "set tags=$TAG_FILE" > ~/.vimrc

elif [ $CMD == "strip" ]; then
    $STRIP -o $BUILD_PATH/vmlinux-stripped $BUILD_PATH/vmlinux

else
    make -j$CPU_CNT O=$BUILD_PATH CROSS_COMPILE=$COMPILER ARCH=$ARCH $CMD
fi

END_TIME=`date +%s`
echo "Total build-time is $((($END_TIME-$START_TIME)/60)) minutes $((($END_TIME-$START_TIME)%60)) seconds"
