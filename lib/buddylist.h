/*
 *  buddylist.h
 *  netchat
 *
 */
#include "buddy.h"
#include <map>
#include <vector>

using namespace std;

struct ltstr
{
  bool operator()(const char* s1, const char* s2) const
  {
    return strcmp(s1, s2) < 0;
  }
};

class BuddyList {
	map<const char*, Buddy, ltstr> buddies;
	
	public:
		BuddyList() {}
		BuddyList(string serializedList);
		
		bool find(const char* sn) const;
		void add(Buddy b);
		void erase(const char* sn);
		Buddy get(const char* sn);
		void fromString(string serializedList);
		string asString() const;
		
		vector<string> getNamesVector() const;
	
	private: 
		BuddyList(const BuddyList&);
		BuddyList& operator=(const BuddyList&);
		
};

