/*
 *  buddylist.cpp
 *  netchat
 *
 */

#include "buddylist.h"
#include "netchat.h"

#include <iostream>


BuddyList::BuddyList(string serializedList) {
	fromString(serializedList);
}

void BuddyList::fromString(string serializedList) {
	unsigned int blpos = 0;
	
	buddies.clear();
	
	while (blpos < serializedList.size()) {
		unsigned int i = blpos;
		for (; serializedList[i] != BUDDY_LIST_INTERNAL_DELIMITER && i < serializedList.size(); i++) {
		}
		//cout << "Substring " << serializedList.substr(blpos, i) << endl;
		Buddy b(serializedList.substr(blpos, i));
		buddies[b.name().c_str()] = b;
		blpos = i + 1;
	}
}

bool BuddyList::find(const char* sn) const {
	return (buddies.count(sn) != 0);
}

void BuddyList::add(Buddy b) {
	buddies[b.name().c_str()] = b;
}

void BuddyList::erase(const char* sn) {
	buddies.erase(sn);
}

Buddy BuddyList::get(const char* sn) {
	return buddies[sn];
}

vector<string> BuddyList::getNamesVector() const {
	vector<string> names;
	
	for (map<const char*, Buddy, ltstr>::const_iterator i = buddies.begin(); i != buddies.end(); i++) {
		string nextname = i->first;
		names.push_back(nextname);
	}
	
	return names;
}

string BuddyList::asString() const {
	string s = "";
	for (map<const char*, Buddy, ltstr>::const_iterator i = buddies.begin(); i != buddies.end(); i++) {
		s += (i->second).asString() + BUDDY_LIST_INTERNAL_DELIMITER;
	}
	s += '\0';
	return s;
}

/*
int main(void) {

  BuddyList bl1;
  cout << "bl1: " << bl1.asString() << endl;
  bl1.fromString("lisp#2132412111#20000#20001#$");
  cout << "bl1: " << bl1.asString() << endl;
    BuddyList bl("john#2156373114#8098#8100#$joe#2156373114#8098#8100#$tim#2156373114#8098#8100#$");
    
    if (!bl.find("john")) cout << "cannot find john" << endl;
    if (!bl.find("joe")) cout << "cannot find joe" << endl;
    if (!bl.find("tim")) cout << "cannot find tim" << endl;
    if (bl.find("candace")) cout << "error: found nonexistent entry" << endl;
    
    cout << bl.asString();
    return 0;
}
*/


