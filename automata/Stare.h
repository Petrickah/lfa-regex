#pragma once

#ifndef _STARE_H
#define _STARE_H

#include <iostream>
#include <map>

class Stare
{
protected:
	int ID;
	std::map<int, Stare*> subStari;
public:
	Stare(int _id = 0) : ID(_id) {};

	int getID() { return ID; }
	void setID(int _id) { ID = _id; }
	void addSubStare(int _id) { subStari[_id] = new Stare(_id); }

	Stare& operator[](int i) { return *(subStari[i]); }
	friend std::ostream& operator<<(std::ostream& os, const Stare& myStare) {
		os << myStare.ID; return os;
	}
};

#endif // !_STARE_H


