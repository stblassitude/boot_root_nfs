#include <stdio.h>

#define nmount(iov, niov, flags)	my_nmount(iov, niov, flags)
#define add_mtab					my_add_mtab

#include <mount_nfs.c>

int
my_add_mtab(char *a, char *b)
{
	return (1);
}

char *
format_nfshandle(char *handle, int len)
{
	char *s;
	int slen;
	
	slen = len * 2 + 2 + 1;
	s = calloc(slen, 1);
	strlcat(s, "X", slen);
	for (int i = 0; i < len; i++) {
		snprintf(s + i*2 + 1, 3, "%02x", (unsigned char)handle[i]);
	}
	strlcat(s, "X", slen);
	return s;
}

int
my_nmount(struct iovec *iov, u_int niov, int flags)
{
#if 0	
	printf("niov=%d\n", niov);
	for (int i=0; i<niov; i+=2) {
		if (iov[i+1].iov_len > 0) {
			printf("%02d=%02ld: %s=%s\n", i/2, iov[i+1].iov_len, iov[i].iov_base, iov[i+1].iov_base);
		} else {
			printf("%02d=%02ld: %s\n", i/2, iov[i+1].iov_len, iov[i].iov_base);
		}
	}
#endif
	/* extract relevant parameters from iov */
	for (int i=0; i<niov; i+=2) {
		if (strcmp("addr", iov[i].iov_base) == 0) {
			struct sockaddr_in *sin = iov[i+1].iov_base;
			if (sin->sin_family != AF_INET) {
				fprintf(stderr, "FreeBSD diskless only supports IPv4, but the server address is in a different family");
				exit(1);
			}
			in_addr_t a = ntohl(sin->sin_addr.s_addr);
			char s[256];
			inet_ntop(AF_INET, &(sin->sin_addr), s, sizeof(s));
			printf("-e boot.nfsroot.server=%s\n", s);
		}
		if (strcmp("fh", iov[i].iov_base) == 0) {
			char *p = format_nfshandle(iov[i+1].iov_base, iov[i+1].iov_len);
			printf("-e boot.nfsroot.nfshandle=%s\n", p);
			printf("-e boot.nfsroot.nfshandlelen=%ld\n", iov[i+1].iov_len);
			free(p);
		}
		if (strcmp("hostname", iov[i].iov_base) == 0) {
			char *p = strchr(iov[i+1].iov_base, ':');
			if (p == NULL)
				p = iov[i+1].iov_base;
			else
				p++;
			printf("-e boot.nfsroot.path=%s\n", p);
		}
	}
	return (0);
}