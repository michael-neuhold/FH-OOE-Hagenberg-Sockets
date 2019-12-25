#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>

#define BUF_SIZE 500

#define MAXSIZE 256
#define STDOUT 1
#define STDIN 0


int main(int argc, char *argv[])
{
    struct addrinfo hints;
    struct addrinfo *result, *rp;
    int sfd, s, j;
    size_t len;
    ssize_t nread;
    char buf[BUF_SIZE];

    int cSocket, nrRead, nrWrite, r;   
    int in, out;  

    if (argc < 3) {
        fprintf(stderr, "Usage: %s host port msg...\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    /* Obtain address(es) matching host/port */

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;    /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_STREAM; /* STREAM TCP */
    hints.ai_flags = 0;
    hints.ai_protocol = 0;          /* Any protocol */

    s = getaddrinfo(argv[1], argv[2], &hints, &result);

    if (s != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
        exit(EXIT_FAILURE);
    }

    /* getaddrinfo() returns a list of address structures.
      Try each address until we successfully connect(2).
      If socket(2) (or connect(2)) fails, we (close the socket
      and) try the next address. */

    for (rp = result; rp != NULL; rp = rp->ai_next) {
        sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (sfd == -1)
            continue;

        if (connect(sfd, rp->ai_addr, rp->ai_addrlen) != -1)
            break;                  /* Success */

        close(sfd);
    }

    if (rp == NULL) {               /* No address succeeded */
        fprintf(stderr, "Could not connect\n");
        exit(EXIT_FAILURE);
    }

    freeaddrinfo(result);           /* No longer needed */

    in = sfd;
	  out = STDOUT;

   while (1) {
		/* read from input */
		nrRead = read(in, buf, MAXSIZE);
		if (nrRead < 0) {
			perror("error reading from input");
			exit(1);
		} else if (nrRead == 0) {
			/* EOF */
			break;
		}

		/* write to output */
		nrWrite = write(out, buf, nrRead);
		if (nrWrite != nrRead) {
			perror("error writing to output");
			exit(1); 
		} /* if */

		/* switch input/output */
		if (in==sfd) {
			in = STDIN;
			out = sfd;
		}
		else {
			in = sfd;
			out = STDOUT;
		}
	} /* while */

	close(cSocket);

    //exit(EXIT_SUCCESS);
    return 0;
}
