#pragma once

#ifndef _REGEX_H
#define _REGEX_H

#include "../nfa/nfa.h"
#include <string>

class Regex
{
	std::string regexp;
	std::string toPostFix();
	std::string addConcat();
	
	NFA myNFA;
	std::stack<NFA*> nfaStack;
	NFA constructNFA();
public:
	Regex(std::string _regexp = "") : regexp(_regexp) { myNFA = this->constructNFA(); };
	~Regex() { regexp = ""; }

	NFA getNFA() { return myNFA; }
	NFA convertToNonLambdaNFA() { return myNFA.convertToNonLambdaNFA(); }

	bool validate(std::string word);
	vector<string> findAll(std::string ph);
};

#endif // !_REGEX_H



