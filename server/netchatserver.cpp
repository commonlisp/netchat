#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

#include <pthread.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
	 
#include "../lib/netchat.h"
#include "../lib/buddylist.h"
using namespace std;

BuddyList buddies;
pthread_mutex_t BuddiesMutex = PTHREAD_MUTEX_INITIALIZER;

void* handleClient(void* cptr);

int main (int argc, char * const argv[]) {

	signal(SIGPIPE,SIG_IGN);	
	int s = socket(AF_INET, SOCK_STREAM, 0);
	if (s == -1) {
		perror("netchatserver[socket]");
		exit(1);
	}
	cout << "Created TCP server socket" << endl;
	int on = 1;
	setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
	
	struct sockaddr_in addr;
	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	if (argc < 2)
		addr.sin_port = htons(SERVER_PORT);
	else 
		addr.sin_port = htons(atoi(argv[1]));
	
	unsigned int realPort = ntohs(addr.sin_port);
	
	cout << "Binding server on port " << realPort << endl;
	if (bind(s, (struct sockaddr*) &addr, sizeof(addr)) == -1) {
		perror("netchatserver[bind]");
		exit(1);
	} 
	cout << "Bound server on port " << realPort << endl;;
	
	if (listen(s, 1) == -1) {
		perror("netchatserver[listen]");
		exit(1);
	}
	cout << "Listening on port " << realPort << endl;

	// Dispatch children to handle connections
	while (1) {
		socklen_t sz = sizeof(addr);
		int c = accept(s, (struct sockaddr*) &addr, &sz);
		if (c == -1) {
			perror("netchatserver[accept]");
			exit(1);
		}
		
		pthread_t clientHandler;	
		pthread_create(&clientHandler, NULL, handleClient, (void*) &c);
		pthread_mutex_lock(&BuddiesMutex);
		string lst = buddies.asString();
		pthread_mutex_unlock(&BuddiesMutex);
		cout << "Current Buddies online: " << lst << endl;
	}
}

void* handleClient(void* cptr) {
	int c = * ((int*) cptr);
	char buf[MAX_SIZE_HANDSHAKE];

	cout << "Writing Greeting. " << endl;
	// Handshake -- Get client info (IP, hostname, screen name)
	if (write(c, SERVER_GREETING, strlen(SERVER_GREETING)) == -1) {
		perror("netchatserver[send(greeting)]");
		close(c);
		pthread_exit(NULL);
	}
	cout << "Sent greeting. " << SERVER_GREETING << " [" << strlen(SERVER_GREETING) 
		<< " bytes]" << endl;
	
	struct sockaddr clt_addr_st;
	bzero(&clt_addr_st, sizeof(clt_addr_st));
	int clt_addr_st_sz = sizeof(clt_addr_st);
	if (getpeername(c, &clt_addr_st, (socklen_t*) &clt_addr_st_sz) == -1) {
		perror("netchatserver[getpeername]");
		close(c);
		pthread_exit(NULL);
	}
	
	struct sockaddr_in * sa_in = (struct sockaddr_in*) &clt_addr_st;
	cout << "[netchat-server]Client " 
		<< inet_ntoa(*((struct in_addr *) &(sa_in->sin_addr.s_addr)))
		<< " connected." << endl;
	
	unsigned int hdshk_sz = 0;
	if (read(c, &hdshk_sz, sizeof(unsigned int)) == -1) {
		perror("netchatserver[recv(handshake-preamble)]");
		close(c);
		pthread_exit(NULL);
	}
	hdshk_sz = ntohl(hdshk_sz);
	
	cout << "[netchat-server]Expecting " << hdshk_sz << " bytes of handshake" << endl;
	// Must receive at least the client address, client (to client) port, and screenname  
	int actualHandshakeSize = 0;
	if ((actualHandshakeSize = read(c, buf, hdshk_sz)) == -1) {
		perror("netchatserver[recv(handshake)]");
		close(c);
		pthread_exit(NULL);
	}

	string bufstr = buf;
	Buddy b(bufstr);
	// The bufstr address is a dummy address. The server figures out the real IP 
	// using getpeername.
	b.setAddr(ntohl(sa_in->sin_addr.s_addr));
	
	while (1) {
		pthread_mutex_lock(&BuddiesMutex);
		if (!buddies.find(b.name().c_str())) {
			buddies.add(b);
		}
		string buddiesData = buddies.asString();
		pthread_mutex_unlock(&BuddiesMutex);
		
		unsigned int listLen = htonl(buddiesData.size());
		
		// Loop: Update client on current server info until client quits
		if (write(c, &listLen, sizeof(listLen)) == -1) {
			perror("netchatserver[send(preamble)]");
			pthread_mutex_lock(&BuddiesMutex);
			buddies.erase(b.name().c_str());
			pthread_mutex_unlock(&BuddiesMutex);
			close(c);
			pthread_exit(NULL);
		}		
		
		if (write(c, buddiesData.c_str(), buddiesData.size()) == -1) {
			perror("netchatserver[send(buddyList)]");
			pthread_mutex_lock(&BuddiesMutex);
			buddies.erase(b.name().c_str());
			pthread_mutex_unlock(&BuddiesMutex);
			close(c);
			pthread_exit(NULL);
		}
		
		sleep(SERVER_SLEEP);
		
	}
	close (c);
	return NULL;
}
