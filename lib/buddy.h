/*
 *  buddy.h
 *  netchat
 *
 */
#include <string>
using namespace std;

class Buddy {	
	public:
		string asString() const;
		
		Buddy();
		Buddy(unsigned int addr_, unsigned int port_, 
			  unsigned int fileXferPort_, string screenName_);
		Buddy(string serializedBuddy);
		
		void setAddr(unsigned int addr_);
		string name() const { return screenName; }
		unsigned int getPort() const { return port; }
		unsigned int getFileXferPort() const { return fileXferPort; }
		unsigned int getAddr() const { return addr; }
		Buddy(const Buddy&);
		
		Buddy& operator=(const Buddy&);
		
private:
    unsigned int addr; // IP Address of Buddy
    unsigned int port; 
    unsigned int fileXferPort;
    string screenName;		
};	

