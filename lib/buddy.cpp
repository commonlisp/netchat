/*
 *  buddy.cpp
 *  netchat
 *
 */
#include "netchat.h"
#include "buddy.h"
#include <iostream>

Buddy::Buddy() 
    : addr(0), port(0), fileXferPort(0), screenName("")
{
}

Buddy::Buddy(unsigned int addr_, unsigned int port_, 
			 unsigned int fileXferPort_, string screenName_) 
: addr(addr_), port(port_), fileXferPort(fileXferPort_), screenName(screenName_)
{
}

Buddy::Buddy(string serializedBuddy) {
	unsigned int snlen = 0; // screen name last index
	for (snlen = 0; (snlen < serializedBuddy.size()) && 
					 serializedBuddy[snlen] != BUDDY_INTERNAL_DELIMITER; 
		 snlen++) {
	}
	screenName = serializedBuddy.substr(0, snlen);
	string buf;
	for (snlen++; (snlen < serializedBuddy.size()) && 
				 serializedBuddy[snlen] != BUDDY_INTERNAL_DELIMITER;
		 snlen++) {
		buf += serializedBuddy[snlen];	
	}
	addr = (unsigned int) strtoul(buf.c_str(), NULL, 10);

	buf = "";
	snlen++;
	for (; (snlen < serializedBuddy.size()) && 
			serializedBuddy[snlen] != BUDDY_INTERNAL_DELIMITER;
		 snlen++) {
		buf += serializedBuddy[snlen];
	}

	port = (unsigned int) strtoul(buf.c_str(), NULL, 10);
	
	buf = "";
	snlen++;
	for (; (snlen < serializedBuddy.size()) && 
			serializedBuddy[snlen] != BUDDY_INTERNAL_DELIMITER;
		snlen++) {
		buf += serializedBuddy[snlen];
	}
	
	fileXferPort = (unsigned int) strtoul(buf.c_str(), NULL, 10);
    }

Buddy::Buddy(const Buddy& b)
  : addr(b.addr), port(b.port), fileXferPort(b.fileXferPort),
    screenName(b.screenName)
{
}

void Buddy::setAddr(unsigned int addr_) {
    addr = addr_;
}

string Buddy::asString() const {
	string serializedBuddy;
	char buf[sizeof(addr)*3 + sizeof(port)*3 + sizeof(fileXferPort)*3 + screenName.size() + 3];
	bzero(buf, sizeof(addr)*3 + sizeof(port)*3 + sizeof(fileXferPort)*3 + screenName.size() + 3);
	
	sprintf(buf, "%s%c%u%c%u%c%u%c", screenName.c_str(), BUDDY_INTERNAL_DELIMITER, addr, 
			BUDDY_INTERNAL_DELIMITER, port, BUDDY_INTERNAL_DELIMITER,
			fileXferPort, BUDDY_INTERNAL_DELIMITER);
	serializedBuddy = buf;
	return serializedBuddy;  
}

Buddy& Buddy::operator= (const Buddy& b) {
	addr = b.addr;
	port = b.port;
	fileXferPort = b.fileXferPort;
	screenName = b.screenName;
	
	return *this;
}

