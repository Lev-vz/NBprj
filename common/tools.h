#ifndef TOOLS_H
#define TOOLS_H
#include <set>
#include <list>

using namespace std;

int rcv(int id, char *buf, int port);
timeval setTimeOut(int s, int us);
int readServ(int socketID, char *buf, int ta = 5);
//void globInit();
bool isKbInput(string &s);
//int readSockets(set<int> &clients, fd_set &readset);
int readSockets(list<int> &sockArr, list<string> buf, int ta = 5);
int getSocketId(char*, int);
int read2Sockets(int *sockArr, char **buf, int ta = 5);
int getSocketForData(int socketID);
int sendFile(string pathFile, int socketID, int socketForFile);

#endif
