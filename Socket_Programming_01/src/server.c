#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

#define BUF_SIZE 500

void server_process (int fd);


char * get_ip_str ( const struct sockaddr *sa , char *s, size_t maxlen )
{
  switch (sa -> sa_family ) {
    case AF_INET :
      inet_ntop ( AF_INET , &((( struct sockaddr_in *) sa) -> sin_addr ),
      s, maxlen );
      printf("port: ", ntohs(&((( struct sockaddr_in *) sa) -> sin_addr )));// --> ntohs byteorder
      break;
    default :
      strncpy (s, " Unknown AF", maxlen );
     //return NULL ;
  }
  return s;
}

int main(int argc, char *argv[])
{
    struct addrinfo hints;
    struct addrinfo *result, *rp;
    int sfd, s;
    struct sockaddr_storage peer_addr;
    socklen_t peer_addr_len;
    ssize_t nread;
    char buf[BUF_SIZE];

   /* if (argc != 2) {
        fprintf(stderr, "Usage: %s port\n", argv[0]);
        exit(EXIT_FAILURE);
    }*/

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_STREAM; /* Datagram socket */
    hints.ai_flags = AI_PASSIVE;    /* For wildcard IP address */
    hints.ai_protocol = 0;          /* Any protocol */
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;

    // node = NULL
    // service = argv[1]
    // hints = &hints --> points to detailed specification
    // res = &reselt --> pointer pointer to addrinfo
    // returns s (int)
    s = getaddrinfo(NULL, "3490", &hints, &result);

    if (s != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
        exit(EXIT_FAILURE);
    }

    /* getaddrinfo() returns a list of address structures.
      Try each address until we successfully bind(2).
      If socket(2) (or bind(2)) fails, we (close the socket
      and) try the next address. */

    // iterate over getaddrinfo returned address data
    for (rp = result; rp != NULL; rp = rp->ai_next) {
        // allocate socket
          // socket type
          // transport service (sock_stream, sock_dgram, sock_raw)
          // special protocol
        sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);

        // ******** Aufgabe 3
        // necessary to use same port
        int on = 1;
        setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
        // ********

        if (sfd == -1)
            continue;

        // bind socket to local port
        if (bind(sfd, rp->ai_addr, rp->ai_addrlen) == 0)
          break;

        close(sfd);
    }

    if (rp == NULL) {               /* No address succeeded */
        fprintf(stderr, "Could not bind\n");
        exit(EXIT_FAILURE);
    }

    freeaddrinfo(result);           /* No longer needed */

    /* Read datagrams and echo them back to sender */
    if(listen(sfd,3) < 0) {
      printf("could not listen!\n");
      exit(666);
    }
   
    int new_sfd = accept(sfd, rp -> ai_addr, &rp -> ai_addrlen);

    char stri[INET_ADDRSTRLEN];

    //******* Aufgabe 4
    printf("address data: %s\n", get_ip_str(rp->ai_addr, stri, INET_ADDRSTRLEN));
    //*******s
    server_process(new_sfd);
    close(new_sfd);   

}