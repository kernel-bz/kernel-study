#!/bin/bash
# $1: ver [v5.x]
# $2: cmd [/menuconfig/log/tag/modules_install/install/clean/distclean/]
# $3: arch [x86_64/riscv/arm64]
# $4: board [origin/hifiveu/..]

# author: JaeJoon Jung <rgbi3307@nate.com> on the www.kernel.bz
# refer: https://www.raspberrypi.com/documentation/computers/linux_kernel.html

# echo "set tags=$BUILD_PATH/tags" > ~/.vimrc
# Makefile, KBUILD_CFLAGS, -save-temps=obj
# x86_64: CC_HAVE_ASM_GOTO := 1, -DCC_HAVE_ASM_GOTO

CPU_CNT=$(grep processor /proc/cpuinfo | awk '{field=$NF};END{print field+2}')
START_TIME=`date +%s`

VER=$1
CMD=$2
#CMD="build"
SDEV=$3

ARCH="arm64"
BOARD="rpi-goodix"
COMPILER="aarch64-linux-gnu-"

# For Raspberry Pi 3, 3+, 4, 400 and Zero 2 W, and Raspberry Pi Compute Modules 3, 3+ and 4:
KERNEL=kernel8

BUILD_PATH="../build/build-$VER-$ARCH-$BOARD"
LOG_PATH="../build/log"
LOG_FILE="$LOG_PATH/log-$VER-$ARCH-$BOARD.txt"
TAG_PATH="$BUILD_PATH/tags"
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
    echo "make bcm2711_defconfig"
    make ARCH=$ARCH O=$BUILD_PATH bcm2711_defconfig 
fi

if [ $CMD == "log" ]; then
    make O=$BUILD_PATH/ CROSS_COMPILE=$COMPILER ARCH=$ARCH > $LOG_FILE

elif [ $CMD == "logfast" ]; then
    make -j$CPU_CNT O=$BUILD_PATH/ CROSS_COMPILE=$COMPILER ARCH=$ARCH > $LOG_FILE

elif [ $CMD == "tag" ]; then
    make tags -j$CPU_CNT O=$BUILD_PATH/ CROSS_COMPILE=$COMPILER ARCH=$ARCH
    echo "set tags=$TAG_PATH" > ~/.vimrc

elif [ $CMD == "build" ]; then
    make -j$CPU_CNT O=$BUILD_PATH/ CROSS_COMPILE=$COMPILER ARCH=$ARCH Image modules dtbs

elif [ $CMD == "strip" ]; then
    $STRIP -o $BUILD_PATH/vmlinux-stripped $BUILD_PATH/vmlinux

elif [ $CMD == "install" ]; then                                                
                                                                                
# $lsblk
# sdb      8:16   1  29.7G  0 disk
# ├─sdb1   8:17   1   256M  0 part /media/jungjaejoon/bootfs (fat32)
# └─sdb2   8:18   1  29.5G  0 part /media/jungjaejoon/rootfs (ext4)

mkdir $BUILD_PATH/mnt                                                           
mkdir $BUILD_PATH/mnt/bootfs                                                     
mkdir $BUILD_PATH/mnt/rootfs                                                      
sudo mount /dev/${SDEV}1 $BUILD_PATH/mnt/bootfs                                      
sudo mount /dev/${SDEV}2 $BUILD_PATH/mnt/rootfs                                       
                                                                                
sudo make -j$CPU_CNT O=$BUILD_PATH/ CROSS_COMPILE=$COMPILER ARCH=$ARCH \
                INSTALL_MOD_PATH=mnt/rootfs modules_install           

sudo cp $BUILD_PATH/mnt/bootfs/$KERNEL.img $BUILD_PATH/mnt/bootfs/$KERNEL-backup.img
sudo cp $BUILD_PATH/arch/$ARCH/boot/Image $BUILD_PATH/mnt/bootfs/$KERNEL.img     
sudo cp $BUILD_PATH/arch/$ARCH/boot/dts/broadcom/*.dtb $BUILD_PATH/mnt/bootfs/   
sudo cp $BUILD_PATH/arch/$ARCH/boot/dts/overlays/*.dtb* $BUILD_PATH/mnt/bootfs/overlays/
sudo cp $BUILD_PATH/arch/$ARCH/boot/dts/overlays/README $BUILD_PATH/mnt/bootfs/overlays/

sudo umount $BUILD_PATH/mnt/bootfs                                               
sudo umount $BUILD_PATH/mnt/rootfs

else
    make -j$CPU_CNT O=$BUILD_PATH/ CROSS_COMPILE=$COMPILER ARCH=$ARCH $CMD
fi

END_TIME=`date +%s`
echo "Total build-time is $((($END_TIME-$START_TIME)/60)) minutes $((($END_TIME-$START_TIME)%60)) seconds"
