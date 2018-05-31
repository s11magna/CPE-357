#ifndef HTTPD_H
#define HTTPD_H

#define BUFFSIZE 2048

#include <signal.h>
extern int queueSize;
extern int socketFD;
extern int clientFD;
extern int portNumber;
extern int child_count;
extern int fileSze;
extern char buff[BUFFSIZE];
/* defintions for copy and strp */
extern char *strdup(const char *s);

extern char *strsep(char **stringp, const char *delim);
void cgi_like(int port, int client_fd, char* cmd);
void determineRequest(int port, int clientID, char* fileName);
void createPath(char* fileName, char*path);
int printHeader(int clientFD, int port, char* fileName);
int fileSize(char *path);
int fileExists(char *file);
void sigchld_handler(int s);
int printFile(int clientFD,int FD);
void errorHandeling(int clientFD,int errorCode);
#endif
