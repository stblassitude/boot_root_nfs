# Get NFS Handle For Diskless Boot with Bhyve

This utility is a quick and dirty hack based on mount_nfs(8).  Instead
of calling nmount(9) to mount the NFS filesystem on the local machine,
the boot parameters necessary for mounting the filesystem as root are
emitted on stdout.

## Sample Output

````
$ ./boot_root_nfs server:/netboot/10-stable /
-e boot.nfsroot.server=192.0.2.1
-e boot.nfsroot.nfshandle=Xffd8bce2de73f4a90a000400000000005e860b000000000000000000X
-e boot.nfsroot.nfshandlelen=28
-e boot.nfsroot.path=/netboot/10-stable
````

## Starting Bhyve To Boot Diskless

When pxeboot(8) starts a system, it sets a number of boot variables to let
the kernel know which network interface needs to be configured, and where
to mount the root filesystem from.  userboot(8) currently does not include
networking functions, so you need to set the necessary variables yourself.

You pass these variables to bhyveload(8) using the -e option.
boot_root_nfs emits the necessary options directly, so you can just insert
its output into the command line.

````
$ bhyveload -h /netboot/10-stable \
  -e boot.netif.name=vtnet0 \
  -e boot.netif.hwaddr=02:01:02:03:04:05 \
  -e boot.netif.ip=192.0.2.2 \
  -e boot.netif.netmask=255.255.255.0 \
  $(./boot_root_nfs server:/p2/netboot/10-stable /) \
  -m 256 -c /dev/nmdm2A netboot2

````

