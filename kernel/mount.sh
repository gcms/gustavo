#!/bin/sh

if ! [ $1 ]; then
    echo "kernel not specified"
    exit
fi

mdconfig -a -t vnode -u 0 -f /fatd/text/os/osdev/tutorial/fs.boot
mount_ext2fs /dev/md0 /mnt
cp "$1" /mnt
umount /mnt
mdconfig -d -u 0
cat /fatd/text/os/osdev/tutorial/fs.boot > fda
