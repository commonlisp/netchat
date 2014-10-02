/*
 *  netchat-client.cpp
 *  netchat
 *
 */

#include <qapplication.h>
#include <qmessagebox.h>
#include <qstring.h>
#include "netchat-client.h"

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <string>
#include <fstream>
#include <sys/stat.h>
#include "../lib/netchat.h"
#include "../lib/buddylist.h"

using namespace std;

const int on = 1;

pthread_mutex_t BuddiesMutex = PTHREAD_MUTEX_INITIALIZER;
BuddyList bl;

pthread_mutex_t nameMutex = PTHREAD_MUTEX_INITIALIZER;
string screenName;

pthread_mutex_t chatPortSockMutex = PTHREAD_MUTEX_INITIALIZER;
int chatPortSock = 0;

BuddyListWndImpl* mainwndptr = NULL;

unsigned int myServerPort = SERVER_PORT;
unsigned int myClientChatPort = CLIENT_CHAT_PORT;
unsigned int myClientXferPort = CLIENT_FILE_XFER_PORT;

int main( int argc, char **argv )
{

  switch (argc) { 
    case 4:
      myClientXferPort = strtoul(argv[3], NULL, 10);
    case 3:
      myClientChatPort = strtoul(argv[2], NULL, 10);
    case 2:
      myServerPort = strtoul(argv[1], NULL, 10);
  }
  cout << "Initializing client to use server port " 
       << myServerPort
       << ", UDP chat port " << myClientChatPort
       << ", TCP file xfer port " << myClientXferPort << endl;
 
    signal(SIGPIPE,SIG_IGN);
    QApplication a( argc, argv );
    BuddyListWndImpl myBuddyListWnd;
		
    a.setMainWidget( &myBuddyListWnd );
    myBuddyListWnd.show();

    mainwndptr = &myBuddyListWnd;

    pthread_mutex_lock(&chatPortSockMutex);	
    setupListener();
    pthread_mutex_unlock(&chatPortSockMutex);
	
    pthread_t clientHandlerT, clientFileXferHandlerT;		
    pthread_create(&clientHandlerT, NULL, (void* (*) (void*))clientHandler, NULL);
    pthread_create(&clientFileXferHandlerT, NULL, ( void* (*) (void*)) listenFileXfer, NULL);
    
    int ret = a.exec();

    pthread_kill(clientHandlerT, SIGQUIT);
    
    pthread_mutex_lock(&chatPortSockMutex);
    close(chatPortSock);
    pthread_mutex_unlock(&chatPortSockMutex);
	
    return ret; 
}

void setupListener() {
	chatPortSock = socket(AF_INET, SOCK_DGRAM, 0);
	if (chatPortSock == -1) {
		perror("netchat-client[socket]");
		exit(1);
	}
	
	setsockopt(chatPortSock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
	
	struct sockaddr_in addr = makesockaddr(INADDR_ANY, myClientChatPort, false);

	if (bind(chatPortSock, (const struct sockaddr*) &addr, sizeof(addr)) == -1) {
		perror("netchat-client[bind]");
		exit(1);
	} 
	cout << "Receiver bound to port " << myClientChatPort << endl;
}

void listenFileXfer() {
	int s = socket(AF_INET, SOCK_STREAM, 0);
	if (s == -1) {
		perror("netchat-client[socket]");
		return;
	}
	
	setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

	struct sockaddr_in addr = makesockaddr(INADDR_ANY, myClientXferPort, false);
	
	cout << "File Receiver process started on port " << myClientXferPort << endl;
	if (bind(s, (const struct sockaddr*) &addr, sizeof(addr)) == -1) {
		perror("netchat-client[bind]");
		return;
	} 
	
	if (listen(s, 1) == -1) {
		perror("netchat-client[listen-file-xfer]");
		return;
	}
	
	while (1) {
		socklen_t sz = sizeof(addr);
		int c = accept(s, (struct sockaddr*) &addr, &sz);
		if (c == -1) {
			perror("netchat-client[accept-file-xfer]");
			return;
		}
		
		int* cp = new int;
		*cp = c;
		pthread_t fileRcvrT;
		pthread_create(&fileRcvrT, NULL, ( void* (*) (void*)) recvFileFromSock, 
			       (void*) cp);
	}
}

void recvFileFromSock(int* c_) {
	int c = *c_;
	delete c_;
	
	cout << "Ready to receive file xfer preamble." << endl;
	unsigned long file_sz = 0;
	if (read(c, &file_sz, sizeof(unsigned long)) == -1) {
		perror("netchat-client[read-file-sz]");
		close(c);
		return;
	}
	
	file_sz = ntohl(file_sz);
	
	unsigned int filename_sz = 0;
	if (read(c, &filename_sz, sizeof(unsigned int)) == -1) {
		perror("netchat-client[read-filename-sz]");
		close(c);
		return;
	}
	filename_sz = ntohl(filename_sz);
	char filename[filename_sz];
	if (read(c, filename, filename_sz) == -1) {
		perror("netchat-client[read-filename]");
		close(c);
		return;
	}
	
	cout << "netchat-client[file-recvr]Receiving " << filename << " [" << file_sz 
		<< " bytes]" << endl;
	
	unsigned int read_sz = 0;
	if (file_sz > FILE_RW_CHUNK) {
		read_sz = FILE_RW_CHUNK;
	} else {
		read_sz = file_sz;
	}
	
	ofstream f (filename, ios::out | ofstream::binary);
	
	char buf[read_sz];
	
	unsigned long bytesLeft = file_sz;
	
	for (; bytesLeft > 0 && f.good(); ) {
		if (bytesLeft < read_sz) {
			bzero(buf, read_sz);
			read_sz = bytesLeft;
		}
		
		int actualBytesRead = 0;
		if ((actualBytesRead = read(c, buf, read_sz)) == -1) {
			perror("netchat-client[read-file]");
			close(c);
			f.close();
			return;
		}
		f.write(buf, actualBytesRead);
		bytesLeft = bytesLeft - actualBytesRead;
		cout << "Saved " << actualBytesRead << " bytes of " << filename
			 << " [" << bytesLeft << " bytes remaining]" << endl;
	}
	cout << "Bytes left to be written " << bytesLeft << endl;
	f.flush();
	f.close();
	close(c);
}

void handleServerConnection(QStringList* qp) {
	struct hostent* hinfo;
	string server;
	
	QStringList q = *qp;
	delete qp;

	QStringList::Iterator itr = q.begin(); 
	pthread_mutex_lock(&nameMutex);
	screenName = (*itr).ascii();
	pthread_mutex_unlock(&nameMutex);
	itr++;
	server = (*itr).ascii();
		 
	if ((hinfo=gethostbyname(server.c_str()))==NULL) {
	    perror("netchat-client[gethostbyname--unable to resolve name]");
		return;
	}
	
	int ss = socket(AF_INET, SOCK_STREAM, 0);
	if (ss == -1) {
		perror("socket");
		return;
	}
	setsockopt(ss, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
		
	unsigned int saddr = 0;
	memcpy(&saddr, hinfo->h_addr, sizeof(unsigned int)); //h_addr is already in network byte order
	struct sockaddr_in p_addr = makesockaddr(ntohl(saddr),myServerPort,false);
	
	cout << "Connecting to server " << inet_ntoa(*((struct in_addr *) &(p_addr.sin_addr.s_addr))) << " port " << myServerPort << endl;	
	if (connect(ss, (struct sockaddr*) &p_addr, sizeof(p_addr)) == -1) {
		perror("netchat-client[connect]");
		close(ss);
		return;
	} 
	cout << "Successfully \"connected\" to " << " [" 
		<< inet_ntoa(*((struct in_addr *) &(p_addr.sin_addr.s_addr))) << "]\n" << endl;
	
	char welcome_buf[strlen(SERVER_GREETING) + 1];
	int bytesread = 0;
	if ((bytesread = read(ss, welcome_buf, strlen(SERVER_GREETING))) == -1) {
		perror("netchat-client[recv-greeting]");
		close(ss);
		return;
	}
	welcome_buf[bytesread] = '\0';
	cout << "Received [" << bytesread << " bytes]: " <<  welcome_buf << endl;
	
	// Send the chat port , file xfer port, and screen name info, but let server figure out my IP
	pthread_mutex_lock(&nameMutex);
	Buddy dummy_buddy_self(htonl(INADDR_ANY), myClientChatPort, 
			       myClientXferPort, screenName);
	pthread_mutex_unlock(&nameMutex);
	
	unsigned int selfsz = htonl(dummy_buddy_self.asString().size()); 
	if (write(ss, &selfsz, sizeof(selfsz)) == -1) {
		perror("netchat-client[send-handshake-preamble]");
		close(ss);
		return;
	}
	cout << "Sending handshake " << dummy_buddy_self.asString().c_str() << endl;
	
	int bytesXferred = 0;
	if ((bytesXferred = write(ss, dummy_buddy_self.asString().c_str(), 
			dummy_buddy_self.asString().size())) == -1) {
		perror("netchat-client[send-handshake]");
		close(ss);
		return;
	}
	cout << "Sent handshake. [" << bytesXferred << " bytes]" << endl;
	
	while (1) {
		unsigned int listLen = 0;
		
		if ((bytesXferred = read(ss, &listLen, sizeof(listLen))) == -1) {
			perror("netchat-client[read-preamble]");
			close(ss);
			return;
		}
		listLen = ntohl(listLen);
		char buddiesData[listLen];
		if ((bytesXferred = read(ss, buddiesData, listLen)) == -1) {
			perror("netchat-client[read-data]");
			close(ss);
			return;
		}
		
		pthread_mutex_lock(&BuddiesMutex);
		bl.fromString(buddiesData);
		vector<string> names = bl.getNamesVector();
		pthread_mutex_unlock(&BuddiesMutex);
		
		if (mainwndptr != NULL) {
		  mainwndptr->refreshBuddyList(names);
		} 
	    }		
	close(ss);
}

void clientHandler() {
    unsigned int header_sz = 1 + sizeof(unsigned int) + 
      sizeof(time_t) + sizeof(unsigned int);  // x86 Linux header_sz is 17 bytes
    char preamble_buf[header_sz];
    time_t last_packet_time = 0;
    
    while (1) {
	struct sockaddr_in bogus_addr;
	bzero(&bogus_addr, sizeof(bogus_addr));
	socklen_t sz = sizeof(bogus_addr);
	
	// Get the incoming packet header
	// Header Format
	// OPCODE: 1 byte
	// Reply Port: 4 bytes
	// Timestamp: 8 bytes
	// (Total) Packet Length: 4 bytes 

	pthread_mutex_lock(&chatPortSockMutex);
	if (recvfrom(chatPortSock, preamble_buf, header_sz, 
		     MSG_PEEK, (struct sockaddr*) &bogus_addr, &sz) == -1) {
	    perror("netchat-client[recvfrom]");
	    pthread_mutex_unlock(&chatPortSockMutex);
	    continue;
	}
	pthread_mutex_unlock(&chatPortSockMutex);
	
	if (preamble_buf[0] != OP_PREAMBLE) {
	  // Skip this current packet because it isn't a msg packet
	  // It is probably an acknowledgement
	  continue;
	}
	
	unsigned int reply_port = 0;
	memcpy(&reply_port, preamble_buf + 1, sizeof(reply_port));
	// Sender's timestamp (for use in acknowledging)
	time_t timestamp;
	memcpy(&timestamp, preamble_buf + 1 + sizeof(reply_port), sizeof(time_t));
	cout << "Timestamp: " << ntohl(timestamp) << endl;
	
	// Total packet length (header_sz + payload_len)
	unsigned int packlen = 0;
	memcpy(&packlen, preamble_buf + 1 + sizeof(reply_port) + sizeof(time_t), sizeof(unsigned int));
	unsigned int msgsz = ntohl(packlen);
	cout << "Expecting to receive message of size " << msgsz << endl;
	
	// Get the entire packet (including both header and payload).
	char msg[msgsz];
	pthread_mutex_lock(&chatPortSockMutex);
	if (recvfrom(chatPortSock, msg, msgsz, 0, 
		     (struct sockaddr*) &bogus_addr, &sz) == -1) {
	    perror("netchat-client[msg-recvfrom]");
	    pthread_mutex_unlock(&chatPortSockMutex);
	    sleep(CLIENT_RETRY_DELAY);
	    continue;
	}
	pthread_mutex_unlock(&chatPortSockMutex);
	
	// Check to see if packet is too old (to avoid duplicates)
	if (last_packet_time > (time_t) ntohl(timestamp)) {
	    // Discard out dated packet
	    cerr << "Discarding old packet." << endl;
	    continue;
	}
	last_packet_time = ntohl(timestamp);
	
	// Parse the packet payload
	string sender = "";
	unsigned int i = 0;
	for (i = header_sz; i < msgsz && 
	     msg[i] != BUDDY_INTERNAL_DELIMITER; i++) {
	    sender += msg[i];
	}
	char textbuf[msgsz - i];
	memcpy(textbuf, msg + i + 1, msgsz - i - 1);
	textbuf[msgsz - i - 1] = '\0';
	string textstr = textbuf;
	cout << "Sender: " << sender << endl;
	cout << "Message: " << textstr << endl;
	
	if (mainwndptr != NULL) {
	  mainwndptr->newMsgFrom(sender, textstr);
	  cout << "GUI updated." << endl;
	} else {
	  cerr << "netchat-client[No main window pointer]" << endl;
	}
	// Acknowledge the packet
	// Look for ACK format: [a] - 1 char
	//                                [timestamp of sent msg (above)] long, time_t
	//	               [ack'ed packet length] unsigned int 
	//                                [SN length] unsigned int
	//                                [char* SN] SN length	
	// recvfrom only filled bogus_addr with the correct return address but not port,
	// we have to fill in the port manually.
	pthread_mutex_lock(&BuddiesMutex);
	if (!bl.find(sender.c_str())) {
	    cerr << "Cannot find " << sender << endl;
	    bogus_addr.sin_port = reply_port;
	} else {
	    Buddy bb = bl.get(sender.c_str());
	    cout << "Found " << bb.name() << " with port " << bb.getPort() << endl;
	    bogus_addr.sin_port = htons(bb.getPort());
	}
	pthread_mutex_unlock(&BuddiesMutex);
	
	printIPAddrPort(bogus_addr, "netchat-client-IM ACK " + sender);
	
	unsigned int snlen = htonl(sender.size());
	unsigned int acklen = 1 + sizeof(time_t) + 2*sizeof(unsigned int) + sender.size();
	
	char ack[acklen];
	ack[0] = OP_ACK;
	memcpy(ack + 1, &timestamp, sizeof(time_t));
	// packlen is the network order version of msgsz
	memcpy(ack + 1 + sizeof(time_t), &packlen, sizeof(unsigned int));
	memcpy(ack + 1 + sizeof(time_t) + sizeof(unsigned int), &snlen, sizeof(unsigned int));
	memcpy(ack + 1 + sizeof(time_t) + 2*sizeof(unsigned int), sender.c_str(), sender.size());
	
	pthread_mutex_lock(&chatPortSockMutex);
	if (sendto(chatPortSock, ack, acklen, 0, 
		   (struct sockaddr*) &bogus_addr, sizeof(bogus_addr)) == -1) {
	    perror("netchat-client[problem-sending-ack]");
	} else {
	    cerr << "netchat-client[ACK-sent-successful]" << endl;
	}
	pthread_mutex_unlock(&chatPortSockMutex);
    }
}

void sendIM(string sn, string msg)
{
    //string s[2] = {sn, msg};
    string* s = new string[2];
    s[0] = sn;
    s[1] = msg;
    pthread_t sendim_t;
    pthread_create(&sendim_t, NULL, (void * (*) (void*))sendIMThread, s);
}

void sendIMThread(string* str) {
    string sn = str[0];
    string msg = str[1];
    delete [] str;
    
    pthread_mutex_lock(&BuddiesMutex);
    if (!bl.find(sn.c_str())) {
	pthread_mutex_unlock(&BuddiesMutex);
	cout << "User " << sn << " is not currently online." << endl;
	return;
    }	
    pthread_mutex_unlock(&BuddiesMutex);
 
	// UDP setup
	int s = socket(AF_INET, SOCK_DGRAM, 0);
	if (s == -1) {
		perror("netchat-client[sendmsg-socket]");
		return;
	}
	
	int on = 1;
	setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

	pthread_mutex_lock(&BuddiesMutex);
	cout << "Desperate stuff: " << bl.asString() << endl;
	struct sockaddr_in addr = makesockaddr(bl.get(sn.c_str()).getAddr(),
					       bl.get(sn.c_str()).getPort(), false);
	pthread_mutex_unlock(&BuddiesMutex);
	
	pthread_mutex_lock(&nameMutex);
	string sender = screenName;
	pthread_mutex_unlock(&nameMutex);
	
	printIPAddrPort(addr, "[netchat-client]Send IM");
	// Packaging Message
	// [OPCODE 'p'] 1 char
	// [reply port] unsigned int
	// [timestamp (network order)] time_t (usually long)
	// [packet size (network order)] unsigned int
	// [sender name] 
	// [BUDDY_INTERNAL_DELIMITER] 1 char
	// [msg]
	unsigned int pktsize = 1 + sizeof(unsigned int) + sizeof(time_t) + sizeof(unsigned int) 
			       + sender.size() + 1 + msg.size();
	char msgpkt[pktsize];
	bzero(msgpkt, pktsize);
	msgpkt[0] = OP_PREAMBLE;
	unsigned int chatport_netorder = htons(myClientChatPort);
	memcpy(msgpkt + 1, &chatport_netorder, sizeof(chatport_netorder));
	time_t timestamp = htonl(time(NULL));
	unsigned int pktsize_in_netorder = htonl(pktsize);
	memcpy(msgpkt + 1 + sizeof(chatport_netorder), &timestamp, sizeof(time_t));
	memcpy(msgpkt + 1 + sizeof(chatport_netorder) + sizeof(time_t), &pktsize_in_netorder, sizeof(unsigned int));
	sprintf(msgpkt + 1 + sizeof(chatport_netorder) + sizeof(time_t) + sizeof(unsigned int), "%s%c%s", 
			sender.c_str(), BUDDY_INTERNAL_DELIMITER, msg.c_str());
	
	bool ack = false;
	int numtries = 0;
	while (!ack && numtries  <= MAX_RETRIES) {
	    numtries++;
		
		// Send the packet
		cout << "Sending msg " << msgpkt + 1 + sizeof(chatport_netorder) + sizeof(time_t) 
			+ sizeof(unsigned int) << " [" << pktsize << " bytes] to " << sn << endl;
		if (sendto(s, msgpkt, pktsize, 0, (struct sockaddr*) &addr, sizeof(addr)) == -1) {
			perror("netchat-client[sendto-sending-msg]");
			continue;
		}
		
		// Look for ACK format: [a] - 1 char
		//                                [timestamp of sent msg (above)] long, time_t
		//	               [ack'ed packet length] unsigned int 
		//                                [SN length] unsigned int
		//                                [char* SN] SN length
		int ackbuflen = 1 + sizeof(time_t) + 2*sizeof(unsigned int);
		char ackbuf[ackbuflen];
		
		// The number of packets we have examined for ACKs thus far.
		int pktnum = 0;

		struct sockaddr dummy;
		bzero(&dummy, sizeof(dummy));
		socklen_t dummy_sz = sizeof(dummy);
					
		while (pktnum <= MAX_WAIT_FOR_ACK) {
			pktnum++;
			//cout << "Waiting for ack. Attempt " << pktnum << endl;
			pthread_mutex_lock(&chatPortSockMutex);
			if (recvfrom(chatPortSock, ackbuf, ackbuflen, 
				MSG_PEEK, &dummy, &dummy_sz) == -1) {
				perror("netchat-client[recvfrom-ack]");
				pthread_mutex_unlock(&chatPortSockMutex);
				continue;
			} else if (ackbuf[0] != OP_ACK) {
				//cerr << "Not an ACK. May be a regular message." << endl;
			    pthread_mutex_unlock(&chatPortSockMutex);
			    sleep(CLIENT_RETRY_DELAY);
				continue;
			} else {
			    pthread_mutex_unlock(&chatPortSockMutex);
			    time_t ack_time;
			    memcpy(&ack_time, ackbuf + 1, sizeof(time_t));
			    cout << "ACK Timestamp " << ack_time << ", " << timestamp << endl;
			    if (ack_time != timestamp) {
				cerr << "netchat-client[ACK-wrong-timestamp]" << endl;
				sleep(CLIENT_RETRY_DELAY);
				continue;
			    }
			    // The packet length the ack acknowledges receiving. (a crude checksum)
			    unsigned int acklen = 0;
			    memcpy(&acklen, ackbuf + 1 + sizeof(time_t), sizeof(unsigned int)); 
			    if (ntohl(acklen) != pktsize) {
				cerr << "netchat-client[ACK-wrong-packet-length]" << endl;
				continue;
			    }
			    // Length of ACK receipt SN (the client which received the message we just sent)
			    unsigned int acksnlen = 0;
			    memcpy(&acksnlen, ackbuf + 1 + sizeof(time_t) + sizeof(unsigned int),
				   sizeof(unsigned int));
			    acksnlen = ntohl(acksnlen);
			    if (acksnlen != sender.size()) {
				cerr << "netchat-client[ACK-SN-LEN-mismatch]" << endl;
				continue;
			    }
			    // Whole ack pkt including SN
			    char wholeackpkt[acksnlen + ackbuflen];
			    char acksn[acksnlen];
			    pthread_mutex_lock(&chatPortSockMutex);
			    if (recvfrom(chatPortSock, wholeackpkt, acksnlen + ackbuflen, 
					 MSG_PEEK, &dummy, &dummy_sz) == -1) {
				perror("netchat-client[recvfrom-ack-sn]");
				pthread_mutex_unlock(&chatPortSockMutex);
				continue;
			    }
			    pthread_mutex_unlock(&chatPortSockMutex);
			    memcpy(acksn,wholeackpkt + ackbuflen, acksnlen);
			    if (!strncmp(acksn,sender.c_str(),acksnlen)) {
				ack = true;
				//Otherwise we have received an expected ACK
				// Now that we know the ack was good, we can remove the ack
				pthread_mutex_lock(&chatPortSockMutex);
				if (recvfrom(chatPortSock, wholeackpkt, acksnlen + ackbuflen,
					     0, &dummy, &dummy_sz) == -1) {
				    pthread_mutex_unlock(&chatPortSockMutex);
				    perror("netchat-client[recvfrom-ack-final]");
				}
				pthread_mutex_unlock(&chatPortSockMutex);
				cout << "Removed an ACK packet." << endl;
				break;
			    }
			}
		}
			
	} 
	if (!ack) 
	    cerr << "Giving up!" << endl;
}

void sendFile(string user, string filename) 
{
	string* s = new string[2];
	s[0] = user;
	s[1] = filename;
	pthread_t sendfile_t;
	pthread_create(&sendfile_t, NULL, (void * (*) (void*))sendFileThread, s);
}

void sendFileThread(string* s)
{
	string user = s[0];
	string filename = s[1];
	delete [] s;

	pthread_mutex_lock(&BuddiesMutex);
	if (!bl.find(user.c_str())) {
		pthread_mutex_unlock(&BuddiesMutex);
		cout << "User " << user << " is not currently online." << endl;
		return;
	}
	pthread_mutex_unlock(&BuddiesMutex);
	
	pthread_mutex_lock(&BuddiesMutex);
	Buddy b = bl.get(user.c_str());
	pthread_mutex_unlock(&BuddiesMutex);
	
	struct stat fstats;
	
	if (stat(filename.c_str(), &fstats) != 0) {
		cerr << "Unable to stat file " << filename << endl;
		return;
	}
	
	unsigned long file_sz = fstats.st_size;
	unsigned int fns = filename.rfind(PATH_SEPARATOR);
	string basefilename;
	if (fns != string::npos) {
		basefilename = filename.substr(fns + 1, filename.size() - 1);
	} else {
		basefilename = filename;
	}
	
	unsigned int filename_sz = basefilename.size();
	
	cout << "Want to open file " << filename << " to " << user <<  endl
		<< " [" << file_sz << " bytes] and base filename " << basefilename 
		<< " [" << filename_sz << " bytes]" << endl
	     << "Trying address " << b.getAddr() << " on port " << b.getFileXferPort() << endl;;

	int ss = socket(AF_INET, SOCK_STREAM, 0);
	if (ss == -1) {
		perror("socket");
		return;
	}
	setsockopt(ss, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
	
	struct sockaddr_in p_addr = makesockaddr(b.getAddr(), b.getFileXferPort(), false);
	
	if (connect(ss, (struct sockaddr*) &p_addr, sizeof(p_addr)) == -1) {
		perror("netchat-client[connect]");
		close(ss);
		return;
	} 

	unsigned long file_sz_ = htonl(file_sz);
	if (write(ss, &file_sz_, sizeof(unsigned long)) == -1) {
		perror("netchat-client[write-file-sz]");
		close(ss);
		return;
	}

	unsigned int filename_sz_ = htonl(filename_sz);	
	if (write(ss, &filename_sz_, sizeof(unsigned int)) == -1) {
		perror("netchat-client[write-filename-sz]");
		close(ss);
		return;
	}
	
	if (write(ss, basefilename.c_str(), basefilename.size()) == -1) {
		perror("netchat-client[write-filename]");
		close(ss);
		return;
	}
	
	unsigned int write_sz = 0;
	if (file_sz > FILE_RW_CHUNK) {
		write_sz = FILE_RW_CHUNK;
	} else {
		write_sz = file_sz;
	}
	
	char buf[write_sz];
	ifstream f (filename.c_str(), ios::in | ifstream::binary);
		
	for (; f.good() && file_sz > 0; ) {
		if (file_sz < write_sz) {
			bzero(buf,write_sz);
			write_sz = file_sz;
		}

		f.read(buf, write_sz);
		if (write(ss,buf,write_sz) == -1) {
			perror("netchat-client[write-file-chunk]");			
			close(ss);
			f.close();
			return;
		} 
		file_sz = file_sz - write_sz;
		cout << "Wrote " << write_sz << " bytes of " << filename
			 << " [" << file_sz << " bytes still remaining]" << endl;
	}
	cout << "sendFileThread() : File size " << file_sz << endl;
	f.close();
	close(ss);
}

struct sockaddr_in makesockaddr(unsigned int addr, unsigned int port, bool isNetworkOrder)
{
    struct sockaddr_in saddr;
    bzero(&saddr, sizeof(saddr));
    saddr.sin_family = AF_INET;
    if (!isNetworkOrder) {
      saddr.sin_addr.s_addr = htonl(addr);
      saddr.sin_port = htons(port);     
    } else {
      saddr.sin_addr.s_addr = addr;
      saddr.sin_port = port;
    }
    return saddr;
}

void printIPAddrPort(struct sockaddr_in addr, string msg)
{
    unsigned int port = addr.sin_port;
    const char* ipaddr = inet_ntoa(*((struct in_addr *) &(addr.sin_addr.s_addr))); 
    //inet_ntoa expects s_addr to be in network byte order
    cout << "[" << msg << "] IP Address : " 
	    << ipaddr << ", port " << ntohs(port) << endl;  
}
