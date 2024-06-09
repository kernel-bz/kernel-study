#!/bin/bash
# author: JaeJoon Jung <rgbi3307@nate.com> on the www.kernel.bz
# Makefile, KBUILD_CFLAGS, -save-temps=obj

ARCH="riscv"
#VER=[v4.14, v5.4, v5.10, v5.15, v6.1, v6.6]
VER=$1
#BRD=[fu540, jh7110, allwinner]
BRD=$2
#CMD=[menuconfig, clean, distclean, tag, log, strip, modules_install]
CMD=$3

START_TIME=`date +%s`
CPU_CNT=$(nproc)
COMPILER="riscv64-unknown-linux-gnu-"

BUILD_PATH="../build/kernel-$VER-$BRD"
INS_PATH="/home/$ARCH/Projects/SDK/install"
LOG_PATH="../build/log"
LOG_FILE="$LOG_PATH/kernel-$VER-$BRD.txt"
TAG_FILE="$BUILD_PATH/tags"
STRIP="$COMPILER""strip"
IMG_PATH="$BUILD_PATH/arch/$ARCH/boot"

if [ ! -d $BUILD_PATH ]; then
    echo "mkdir $BUILD_PATH"
    mkdir $BUILD_PATH
    chmod 755 $BUILD_PATH
    mkdir "$BUILD_PATH/modules"
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

if [ ! $CMD ]; then
    make -j$CPU_CNT O=$BUILD_PATH CROSS_COMPILE=$COMPILER ARCH=$ARCH

elif [ $CMD == "log" ]; then
    make O=$BUILD_PATH/ CROSS_COMPILE=$COMPILER ARCH=$ARCH > $LOG_FILE

elif [ $CMD == "logfast" ]; then
    make -j$CPU_CNT O=$BUILD_PATH CROSS_COMPILE=$COMPILER ARCH=$ARCH > $LOG_FILE

elif [ $CMD == "tag" ]; then
    make tags -j$CPU_CNT O=$BUILD_PATH CROSS_COMPILE=$COMPILER ARCH=$ARCH
#    echo "set tags=$TAG_PATH" > ~/.vimrc

elif [ $CMD == "modules_install" ]; then
    make -j$CPU_CNT O=$BUILD_PATH CROSS_COMPILE=$COMPILER ARCH=$ARCH \
                INSTALL_MOD_PATH=modules modules_install

elif [ $CMD == "strip" ]; then
    STRIP -o $BUILD_PATH/vmlinux-stripped $BUILD_PATH/vmlinux
else
    make -j$CPU_CNT O=$BUILD_PATH CROSS_COMPILE=$COMPILER ARCH=$ARCH $CMD
fi

cp $IMG_PATH/Image.gz "$INS_PATH/Image-$VER-$BRD.gz"
echo  "copy to $INS_PATH/Image-$VER-$BRD.gz"

cp $IMG_PATH/Image "$INS_PATH/Image-$VER-$BRD"
echo  "copy to $INS_PATH/Image-$VER-$BRD"

END_TIME=`date +%s`
echo "Total build-time is $((($END_TIME-$START_TIME)/60)) minutes $((($END_TIME-$START_TIME)%60)) seconds"
