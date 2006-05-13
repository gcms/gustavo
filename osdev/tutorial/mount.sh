#!/bin/sh
mdconfig -a -t vnode -u 0 -f fs.boot
mount_ext2fs /dev/md0 /mnt
cp kernel.bin /mnt
umount /mnt
mdconfig -d -u 0
cat fs.boot > fda
