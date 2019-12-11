/*
 * service.h: define interface of service module
 * gj, 170329: typo
 */

void service_init(int fd);	/* insert a new client for service */
int  service_do(int fd);	/* do a service on client fd */
void service_exit(int fd);	/* remove the client fd from service */

