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

#define MAXSIZE 256
#define STDOUT 1
#define STDIN 0

// poll function
void comm(int tfd, int nfd);

int main(int argc, char *argv[])
{
  struct addrinfo hints;
  struct addrinfo *result, *rp;
  int sfd, s;

  // check correct number of parameters
  if (argc < 3) {
    fprintf(stderr, "Usage: %s host port msg...\n", argv[0]);
    return EXIT_FAILURE;
  }

  memset(&hints, 0, sizeof(struct addrinfo)); // empty struct
  hints.ai_family = AF_INET;                  // Allow IPv4 or IPv6 
  hints.ai_socktype = SOCK_STREAM;            // STREAM TCP
                                              // AI PASSIVE not necessary, Address gets passed via an argument

  // result points on linked list
  s = getaddrinfo(argv[1], argv[2], &hints, &result);

  // catch getaddrinfo error
  if (s != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
    return EXIT_FAILURE;
  }

  // iterate through list
  for (rp = result; rp != NULL; rp = rp->ai_next) {
    sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);

    // not valid
    if (sfd == -1)
      continue;

    // valid
    if (connect(sfd, rp->ai_addr, rp->ai_addrlen) != -1)
      break;                  

    close(sfd);
  }

  // if end of list --> no address succeeded
  if (rp == NULL) {
      fprintf(stderr, "Could not connect\n");
      return EXIT_FAILURE;
  }

  // free memory of linked list
  freeaddrinfo(result);

  /* ==== communication ==== */
  
  comm(STDIN,sfd);

	close(sfd);

  return EXIT_SUCCESS;
}
