#include "..\automata\regex\regexp.h"
#include <string.h>

int precedence(char op) {
	switch (op)
	{
	case '(': return 1;
	case '|':
	case '^': return 2;
	case '*': return 3;
	}
	return 0;
}
std::string Regex::addConcat()
{
	std::string alfabet("0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
	std::list<char> output;
	std::string result = "";
	for (std::string::iterator it = regexp.begin(); it<regexp.end()-1; it++)
	{
		output.push_back(*it);
		if(alfabet.find(*it) != std::string::npos || *it == '*' || *it == ')')
		{
			if(!(*(it+1) == '|' || *(it+1) == ')' || *(it+1) == '*'))
				output.push_back('^');
		}
	}
	output.push_back(*(regexp.end() - 1));

	for (char token : output)
		result += token;
	return result;
}

std::string Regex::toPostFix()
{
	std::string alfabet("0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
	std::stack<char> opStack;
	std::list<char> output;
	std::string result = addConcat();

	for (char token : result) {
		if (alfabet.find(token) != std::string::npos) {
			output.push_back(token);
		}
		else if (token == '(') opStack.push(token);
		else if (token == ')') {
			char topOperator = opStack.top(); opStack.pop();
			while (topOperator != '(') {
				output.push_back(topOperator);
				topOperator = opStack.top(); opStack.pop();
			}
		}
		else {
			while (!opStack.empty() && precedence(opStack.top()) >= precedence(token))
			{ output.push_back(opStack.top()); opStack.pop(); }
			opStack.push(token);
		}
	}

	while( !opStack.empty())
	{ output.push_back(opStack.top()); opStack.pop(); }
	result.clear();
	for (char token : output)
		result += token;
	return result;
}

NFA Regex::constructNFA()
{
	std::string alfabet("0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
	std::string _regexp = toPostFix();
	for (int i = 0; i < _regexp.size(); i++) {
		if (alfabet.find(_regexp[i]) != std::string::npos)
		{
			NFA* op = new NFA;
			op->addLitera(_regexp[i]); op->createStare(1);
			op->setStariFinale({ 1 });
			op->insertTranzitie(0, _regexp[i], 1);
			nfaStack.push(op);
		}
		else {
			switch (_regexp[i]) {
				case '|': {
					NFA& op1 = *nfaStack.top(); nfaStack.pop();
					NFA& op2 = *nfaStack.top(); nfaStack.pop();
					nfaStack.push(new NFA(op2 | op1));
					break;
				}
				case '*': {
					NFA& op = *nfaStack.top(); nfaStack.pop();
					nfaStack.push(new NFA(*op));
					break;
				}
				case '^': {
					NFA& op1 = *nfaStack.top(); nfaStack.pop();
					NFA& op2 = *nfaStack.top(); nfaStack.pop();
					nfaStack.push(new NFA(op2 ^ op1));
					break;
				}
			}
		}
	}
	NFA& result = *nfaStack.top(); nfaStack.pop();
	return result.flip();
}

bool Regex::validate(std::string word)
{
	this->convertToNonLambdaNFA();
	std::vector<int> currStari({ myNFA.getStareInitiala() });
	for (int i = 0; i < word.size(); i++) {
		currStari = Helper::nextStari(word[i], currStari, myNFA.getTabelTranzitii());
	}
	for (int stare : myNFA.getStariFinale()) {
		if (find(currStari.begin(), currStari.end(), stare) != currStari.end()) return true;
	}
	return false;
}

vector<string> Regex::findAll(std::string ph)
{
	vector<string> myVector;
	char* c_ph = (char*)ph.c_str(), *word;
#pragma warning(suppress : 4996)
	word = strtok(c_ph, " ");
	while (word != nullptr) {
		if (this->validate(word)) {
			if(find(myVector.begin(), myVector.end(), string(word)) == myVector.end())
				myVector.push_back(string(word));
		}
#pragma warning(suppress : 4996)
		word = strtok(nullptr, " ");
	}
	return myVector;
}
