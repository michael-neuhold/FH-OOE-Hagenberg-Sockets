/*
 * service.c -- event oriented version of the
 * hangman game
 * $Id: service.c 2891 2019-11-19 19:08:36Z p20018 $
 *
 * gj, 110403: created
 * gj, 170329: included unistd.h to avoid compiler's warnings
 * gj, 191112: sprintf -> snprintf, still some warnings from the compiler
 * gj, 191119: added handling of malloc failures
 */

#pragma GCC diagnostic ignored "-Wformat-truncation"

#include<errno.h>
#include<syslog.h>
#include<time.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

#include"service.h"

/*
 * remove or comment the following define if you are not
 * interested in debug output
 */
//#define DEBUG

#define WORDLEN 		80
#define INCOMPLETE 		1
#define WON 			2
#define LOST 			3


typedef struct state {
	char	*whole_word;	/* the word to be guessed */
	int	word_len;
	char 	part_word[WORDLEN];	/* the part guessed already */
	int	lives;

	int	fd;		/* file descriptor of client */
	struct state*next;		/* pointer to next client */
} state;

static state *clients = NULL;

static char  *words []= { 		/* the words to be guessed */
		"applicationlayer",
		"presentationlayer",
		"sessionlayer",
		"transportlayer",
		"datalinklayer",
		"networklayer",
		"physicallayer",
		"transmissioncontrolprotocol",
		"userdatagramprotocol",

		"arpa",
		"internet",
		"rfc",
		"addressresolutionprotocol"
		"reverseaddressresolutionprotocol",
		"fragmentation",
		"networkaccesslayer",
		"internetcontrolmessageprotocol",
		"filetransferprotocol",
		"hypertexttransferprotocol",
		"simplemailtransferprotocol",
		"networknewsprotocol",


		"asterix",
		"obelix",
		"miraculix",
		"idefix",
		"majestix",
		"gutemine",
		"methusalix",
		"verleihnix",
		"troubardix"
        };
static char	outbuff[WORDLEN];


/*
 * handle a fatal error
 */
#define FATAL(msg) fatal(msg, __FILE__, __LINE__)
static void fatal(char* msg, char* file, int line)
{
	fprintf(stderr, "FATAL ERROR in %s:%d: %s,\n\twill die the hard way.\n", 
		file, line, msg);
	exit(42);
}


/*
 * debug print of list of clients
 */
static void print(){
#ifdef DEBUG
	state	*act;

	printf("*** print: now contains the following states:\n");
	for (act = clients; act != NULL; act = act->next) {
		printf("\t%p: %d %s %s, %d lives\n", act, act->fd, act->whole_word,
			act->part_word, act->lives);
	}
#endif
} /* print */


/*
 * store a new client's data
 */
static state* store(int fd){
	state 		*aState = (state*) malloc(sizeof (state));
#ifdef DEBUG
	printf("*** store client %d in address %p\n", fd, aState);
#endif
	if (!aState){
		FATAL("could not malloc state for a new client");
	}

	aState->fd = fd;

	aState->next = clients;
	clients = aState;
  print();
	return clients;
} /* store */


/*
 * retreive the state of a client
 */
static state* get(int fd){
	state	*aState;

	print();
	for (aState=clients; (aState!=NULL) && (aState->fd != fd);
			aState = aState->next);
	return aState;
}

/*
 * free state of a client
 */
static void removeClient(int fd){
	state	*act=clients;
	state 	*prev=NULL;

	while ((act!=NULL) && (act->fd!=fd)){
		prev = act;
		act = act->next;
	}

	if (act == NULL){
		//perror("cannot free client for %d: not in list", fd);
		perror("cannot free client");
		exit(1);
	}

	if (prev == NULL){
		clients = clients->next;
	} else {
		prev->next = act->next;
	}

	free (act);
	print();
} /* remove */

/*
 * Insert a new client for service
 */
void service_init(int fd) {
	time_t		timer;
	static struct tm *t = NULL;

	state		*act;
	int 		i;


	act = store(fd);
	act->lives = 10;

	if (t == NULL){
		t = (struct tm*) malloc (sizeof (struct tm));
	}

	/*
	 * pick up a random word
	 */
 	time (&timer);
 	t = localtime (&timer);
 	act->whole_word = words [
	  (t->tm_sec + rand()) %
	  (sizeof (words) / sizeof (char*))
	  ];
 	act->word_len = strlen (act->whole_word);

	/*
	 * initialize empty word
	 */
 	for (i=0; i<act->word_len; i++)
		act->part_word [i]='-';
 	act->part_word [i]= '\0';

	/*
	 * output empty word
	 */
 	snprintf (outbuff, WORDLEN, "%s  lives:%d \n", act->part_word, act->lives);
 	write (fd, outbuff, strlen (outbuff));
} /* service_init */


/*
 * do a service on client fd
 */
int service_do(int fd) {
	state	*act;
	int	hits, i, readCount;
	char	guess_word[WORDLEN];
	int	game_status = INCOMPLETE;


	act = get(fd);

	readCount = read(fd, guess_word, WORDLEN);

	hits = 0;
	for (i=0; i<act->word_len; i++) {
		if (guess_word[0] == act->whole_word [i]) {
		hits=1;
		act->part_word[i] = act->whole_word[i];
		} /* if */
	} /* for */

	/*
	* check for end of game
	*/
	if (!hits) {
		act->lives--;
		if (act->lives == 0) {
			game_status = LOST;
		} /* game is over */
	} /* lost one life */
	if (strcmp (act->part_word, act->whole_word) == 0) {
		/*
		* he did it
		*/
		game_status = WON;
		snprintf (outbuff, WORDLEN, "You won!\n");
		write (fd, outbuff, strlen(outbuff));
		return 0;
	} else if (act->lives == 0) {
		game_status = LOST;
		strcpy (act->part_word, act->whole_word);
	}
	/*
	* show word
	*/
	snprintf (outbuff, WORDLEN, "%s  lives: %d \n", act->part_word, act->lives);
	write (fd, outbuff, strlen (outbuff));
	if (game_status == LOST) {
		snprintf (outbuff, WORDLEN, "\nGame over.\n");
		write (fd, outbuff, strlen (outbuff));
	} /* he looses */

	if (game_status == INCOMPLETE) {
		return readCount;
	} else {
		return 0;
	}
	} /* service_do */


/*
 * remove the client fd from service
 */
void service_exit(int fd) {
	removeClient(fd);
} /* service_exit */
