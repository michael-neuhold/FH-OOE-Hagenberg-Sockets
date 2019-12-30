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

// client counter
volatile int counter = 0;

// WordCheck.c
void server_process (int fd);

// for signal handler
void waiter() {
  int cpid, stat;
  cpid = wait (&stat);      // wait for child termination 
  signal (SIGCHLD, waiter); // signal handler can be reinstalled
  counter--;                // client counter can be decreased
}

void get_ip_str ( const struct sockaddr *sa , char *s, size_t maxlen ) {
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

int main(int argc, char *argv[]) {

  // getaddrinfo 
  struct addrinfo hints;
  struct addrinfo *result, *rp;

  // new client
  struct sockaddr_in *client;
  int client_len;
  int sfd, s;

  // install signal handler
  signal (SIGCHLD, waiter);

  // check correct number of parameters
  if (argc != 2) {
      fprintf(stderr, "Usage: %s port\n", argv[0]);
      return EXIT_FAILURE;
  }

  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_INET;                  // Allow IPv4 or IPv6
  hints.ai_socktype = SOCK_STREAM;            // STREAM TCP
  hints.ai_flags = AI_PASSIVE;                // AI PASSIVE getaddrinfo first param

  // result points to linked list
  s = getaddrinfo(NULL, argv[1], &hints, &result);

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

      // necessary to use same port
      int on = 1;
      setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

      // valid
      if (bind(sfd, rp->ai_addr, rp->ai_addrlen) == 0)
        break;

      close(sfd);
  }

  // if end of list --> no address succeeded
  if (rp == NULL) {           
      fprintf(stderr, "Could not bind\n");
      return EXIT_FAILURE;
  }

  // free memory of linked list
  freeaddrinfo(result);

  // listen with error handling
  if(listen(sfd,3) < 0) {
    printf("<< error (listen) >>\n");
    return EXIT_FAILURE;
  }

  int new_sfd;
  pid_t pid;

  while(1) {
    new_sfd = accept(sfd, (struct sockaddr*)&client, (socklen_t*)&client_len);
    // catch accept error
    if(new_sfd == -1) 
      return EXIT_FAILURE;
    // client counter
    counter++;
    pid = fork(); // create sub/child process
    if(pid == 0) {
      printf("# clients: %d\n", counter);
      close(sfd);
      server_process(new_sfd);
      close(new_sfd); // is not used any more
      exit(0);
    } else if(pid > 0) {
      printf("# pid: %d\n", pid);
      close(new_sfd); // is not used any more
    } else {
      printf("fork faild");
      return EXIT_FAILURE;
    }
  }
  close(sfd);
}