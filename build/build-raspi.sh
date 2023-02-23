#!/bin/bash

KERNEL=kernel7
VER_N=0
VER_S=V01

CPU_CNT=$(grep processor /proc/cpuinfo | awk '{field=$NF};END{print field+2}')
START_TIME=`date +%s`

build_version=`date "+zImage_raspi2_k4.1.16_($VER_S)_%Y%m%d_%H%M"`
build_dtb=`date "+raspi2_k4.1.16_dtb_($VER)_%Y%m%d_%H%M"`

build_path="../build_4.1.16_rpi"
release_path="../release"
release_path_ver="../release/$VER_S"
release_path_overlays="../release/$VER_S/overlays"

image_filename="$build_path/arch/arm/boot/zImage"
dtb_filename="$build_path/arch/arm/boot/dts/*.dtb"
dtb_overlays="$build_path/arch/arm/boot/dts/overlays/*.dtb*"
dtb_overlays_readme="$build_path/arch/arm/boot/dts/overlays/README"

target_filename="$release_path/$VER_S/$build_version"
target_dtbname="$release_path/$VER_S/"
target_overlays="$release_path/$VER_S/overlays/"

target_dev_boot="/dev/sda5"
target_mount_boot="/media/pi/boot"
target_dev_root="/dev/sda6"
target_mount_root="/media/pi/root"

echo $VER_N > $build_path/.version
echo "raspi2" > $build_path/version.txt

if [ ! -d $release_path ]; then
	mkdir $release_path
	chmod 777 $release_path
fi

if [ ! -d $release_path_ver ]; then
	mkdir $release_path_ver
	chmod 777 $release_path_ver
	mkdir $release_path_overlays
	chmod 777 $release_path_overlays
fi

if [ ! -d $build_path ]; then
	mkdir $build_path
	chmod 777 $build_path
fi

if [ ! -f $build_path/.config ]; then
	echo ".....raspi2 defconfig"
	make O=$build_path bcm2709_defconfig
fi

if [ "$1" = "make" ]; then
	make -j$CPU_CNT O=$build_path zImage modules dtbs

elif [ "$1" = "install" ]; then

	echo "Installing..."

    if [ ! -d $target_mount_boot/overlays ]; then
        echo "Mounting boot SDCard $target_mount_boot ..."
        mount -t vfat $target_dev_boot $target_mount_boot
    fi

    if [ ! -d $target_mount_root/root ]; then
        echo "Mounting root SDCard $target_mount_root ..."
        mount -t ext4 $target_dev_root $target_mount_root
    fi

	if [ ! -d $target_mount_boot/overlays ]; then
		mkdir $target_mount_boot/overlays
		chmod 777 $target_mount_boot/overlays
	fi

	if [ -f $image_filename ]; then
		echo "Install module to SDCard root($target_mount_root)"
		make -j$CPU_CNT O=$build_path INSTALL_MOD_PATH=$target_mount_root modules_install

		echo "Install kernel to SDCard boot($target_mount_boot)"
		cp $target_mount_boot/$KERNEL.img $target_mount_boot/$KERNEL-backup.img
		scripts/mkknlimg $image_filename $target_mount_boot/$KERNEL.img

		echo "Copy dtb to SDCard boot($target_mount_boot)"
		cp $dtb_filename $target_mount_boot/
		cp $dtb_overlays $target_mount_boot/overlays/
	fi

	echo "Backup to release..."

	if [ -f $image_filename ]; then
	   echo "copy from $image_filename to $target_filename"
	   cp -a $image_filename $target_filename

	   scripts/mkknlimg $image_filename $release_path_ver/$KERNEL.img

	   echo "copy from $dtb_filename to $target_dtbname"
	   cp -a $dtb_filename $target_dtbname

	   echo "copy from $dtb_overlays to $target_overlays"
	   cp -a $dtb_overlays $target_overlays
#	   cp -a $dtb_overlays_readme $target_overlays
	fi

	umount $target_mount_boot
	umount $target_mount_root

else
	make -j$CPU_CNT O=$build_path $1 $2 $3
fi

END_TIME=`date +%s`
echo "Total build-time is $((($END_TIME-$START_TIME)/60)) minutes $((($END_TIME-$START_TIME)%60)) seconds"
