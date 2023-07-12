// vim: set ts=4 sw=4 tw=99 noet:
//
// AMX Mod X, based on AMX Mod by Aleksander Naszko ("OLO").
// Copyright (C) The AMX Mod X Development Team.
//
// This software is licensed under the GNU General Public License, version 3 or higher.
// Additional exceptions apply. For full license details, see LICENSE.txt or visit:
//     https://alliedmods.net/amxmodx-license

//
// CSX Module
//

#ifndef CRANK_H
#define CRANK_H

#define RANK_VERSION 11

#include "amxxmodule.h"

// *****************************************************
// class Stats
// *****************************************************

struct Stats {
	int hits;
	int shots;
	int damage;
	int hs;
	int tks;
	int kills;
	int deaths;
	int bodyHits[9]; ////////////////////
	
	// SiDLuke start
	int bPlants;
	int bExplosions;
	int bDefusions;
	int bDefused;
	// SiDLuke end :D

	Stats();
	void commit(Stats* a);
};

// *****************************************************
// class RankSystem
// *****************************************************

class RankSystem
{
public:
	class RankStats;
	friend class RankStats;
	class iterator;

	class RankStats : public Stats {
		friend class RankSystem;
		friend class iterator;
		RankSystem*	parent;
		RankStats*	next;
		RankStats*	prev;
		char*		unique;
		short int	uniquelen;
		char*		name;
		short int	namelen;
		int			score;
		int			id;
		RankStats( const char* uu, const char* nn,  RankSystem* pp );
		~RankStats();
		void setUnique( const char* nn  );
		inline void goDown() {++id;}
		inline void goUp() {--id;}
		inline void addStats(Stats* a) { commit( a ); }
	public:
		void setName( const char* nn  );
		inline const char* getName() const { return name ? name : ""; }
		inline const char* getUnique() const { return unique ? unique : ""; }
		inline int getPosition() const { return id; }
		inline void updatePosition( Stats* points ) {
			parent->updatePos( this , points );
		}
		inline void deletePosition() { 
			parent->deletePos( this );
		}
};

private:
	RankStats* head;
	RankStats* tail;
	int rankNum;

	struct scoreCalc{
		AMX amx;
		void* code;
		int func;
		cell amxAddr1;
		cell amxAddr2;
		cell *physAddr1;
		cell *physAddr2;
	} calc;

	void put_before( RankStats* a, RankStats* ptr );
	void put_after( RankStats* a, RankStats* ptr );
	void unlink( RankStats* ptr );
	void updatePos( RankStats* r ,  Stats* s );
	void deletePos( RankStats* r );
	
public:

	RankSystem();
	~RankSystem();

	void saveRank( const char* filename );
	void loadRank( const char* filename );
	RankStats* findEntryInRank(const char* unique, const char* name, bool isip=false);
	RankStats* newEntryInRank(const char* unique, const char* name);
	
	bool loadCalc(const char* filename, char* error, size_t maxLength);
	inline int getRankNum( ) const { return rankNum; }
	void clear();
	void unloadCalc();

	class iterator {
		RankStats* ptr;
	public:		
		iterator() {}
		iterator(RankStats* a): ptr(a){}
		inline iterator& operator++() {	ptr = ptr->next; return *this; }
		inline iterator& operator--() { ptr = ptr->prev; return *this;}
		inline RankStats& operator*() {	return *ptr;}operator bool () { return (ptr != 0); }
		iterator subs( const iterator& it, int a );
		iterator add( const iterator& it , int a );
		void setEntryByRank( const int rank );
	};

	inline iterator front() {  return iterator(head);  }
	inline iterator begin() {  return iterator(tail);  }
};


#endif

