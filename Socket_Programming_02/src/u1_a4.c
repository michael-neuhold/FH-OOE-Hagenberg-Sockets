#include <unistd.h> 
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

#include <signal.h>

#define BUF_SIZE 500

volatile int counter = 0;

void waiter()
{
  int cpid, stat;
  cpid = wait (&stat); /* wait for a child to terminate */
  signal (SIGCHLD, waiter); /* reinstall signal handler */
  counter--;
}

void server_process (int fd);

void get_ip_str ( const struct sockaddr *sa , char *s, size_t maxlen )
{
  switch (sa -> sa_family ) {
    case AF_INET :
      inet_ntop ( AF_INET , &((( struct sockaddr_in *) sa) -> sin_addr ),s, maxlen );
      printf("Clients IP: %s\n", s);
      printf("Clients Port: %d\n", ntohs(&((( struct sockaddr_in *) sa) -> sin_addr )));// --> ntohs byteorder
      break;
    default :
        strncpy (s, " Unknown AF", maxlen );
    }
}

int main(int argc, char *argv[])
{
    // install signal handler
    signal (SIGCHLD, waiter);


    struct addrinfo hints;
    struct addrinfo *result, *rp;

    //***** in loop 
    struct sockaddr_in *client;
    int client_len;
    //*****


    int sfd, s;
    struct sockaddr_storage peer_addr;
    socklen_t peer_addr_len;
    ssize_t nread;
    char buf[BUF_SIZE];

    if (argc != 2) {
        fprintf(stderr, "Usage: %s port\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;    /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_STREAM; /* TCP connection */
    hints.ai_flags = AI_PASSIVE;    /* For wildcard IP address */

    s = getaddrinfo(NULL, argv[1], &hints, &result);

    if (s != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
        exit(EXIT_FAILURE);
    }

    // iterate over getaddrinfo returned address data
    for (rp = result; rp != NULL; rp = rp->ai_next) {
        sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);

        if (sfd == -1)
          continue;

        // necessary to use same port
        int on = 1;
        setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

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
      printf("<< error (listen) >>!\n");
      exit(1);
    }

    int new_sfd;
    pid_t pid;

    while(1) {
      new_sfd = accept(sfd, &client, &client_len);
      counter++;
      pid = fork();
      if(pid == 0) {
        printf("Number of connected clients: %d\n", counter);
        close(sfd);
        server_process(new_sfd);
        close(new_sfd); // is not used any more
        exit(0);
      } else if(pid > 0) {
        printf("dies ist die pid: %d\n", pid);
        close(new_sfd); // is not used any more
      } else {
        printf("fork faild");
        return EXIT_FAILURE;
      }
    }
}