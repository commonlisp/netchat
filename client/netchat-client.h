/*
 *  netchat-client.h
 *  netchat
 *
 */

#ifndef CLIENT_H
#define CLIENT_H 1

#include "buddylistwndimpl.h"

void setupListener();
void listenFileXfer();
void recvFileFromSock(int* c);
void clientHandler();
void sendIM(string user, string msg);
void sendIMThread(string* s);
void sendFile(string user, string filename);
void sendFileThread(string* s);
struct sockaddr_in makesockaddr(unsigned int addr, unsigned int port, bool isNetworkOrder);
void printIPAddrPort(struct sockaddr_in, string msg);

void handleServerConnection(QStringList* ql);


#endif

