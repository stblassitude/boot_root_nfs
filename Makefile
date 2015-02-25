#

PROG=	boot_root_nfs
SRCS=	boot_root_nfs.c getmntopts.c mounttab.c
NO_MAN=

CFLAGS+=	-DNFS -I/usr/src/sbin/mount_nfs -I/usr/src/sbin/mount -I/usr/src/usr.sbin/rpc.umntall

.PATH:	/usr/src/sbin/mount_nfs /usr/src/sbin/mount /usr/src/usr.sbin/rpc.umntall

.include <bsd.prog.mk>
