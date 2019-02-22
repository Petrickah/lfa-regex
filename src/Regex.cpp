// Regex.cpp : Defines the entry point for the console application.

#include "..\automata\regex\regexp.h"
#include <iostream>

int menu() {
	int x;
	std::cout << "1) Citire expresie regulata." << std::endl;
	std::cout << "2) Afisare automat cu lambda miscari." << std::endl;
	std::cout << "3) Afisare automat fara lambda miscari." << std::endl;
	std::cout << "4) Validare cuvant citit de la tastatura." << std::endl;
	std::cout << "5) Cautare cuvant pe baza expresiei regulate." << std::endl;
	std::cout << "6) Terminare program." << std::endl;
	std::cout << ">>> "; std::cin >> x;
	return x;
}

int main()
{
	char regexp[150]; Regex* myRegex = nullptr; bool isNonLambda = false;
	while (true)
	{
		int x = menu();
		switch (x) {
			case 1: {
				std::cout << "Introduceti o expresie regulata: "; std::cin >> regexp;
				myRegex = new Regex(regexp);
				break;
			}
			case 2: {
				if(myRegex) std::cout << myRegex->getNFA() << std::endl;
				else std::cout << "Nu ati citit o expresie regulata. " << std::endl;
				break;
			}
			case 3: {
				if (myRegex) {
					if (!isNonLambda) {
						myRegex->convertToNonLambdaNFA(); isNonLambda = true;
					}
					std::cout << myRegex->getNFA() << std::endl;
				}
				else std::cout << "Nu ati citit o expresie regulata. " << std::endl;
				break;
			}
			case 4: {
				if (myRegex)
				{
					string cuvant;
					std::cout << "Introduceti cuvantul de validat: "; std::cin >> cuvant;
					if (myRegex->validate(cuvant)) std::cout << "Cuvantul " << cuvant << " a fost acceptat!" << std::endl;
					else std::cout << "Cuvantul " << cuvant << " NU a fost acceptat!" << std::endl;
				}
				else std::cout << "Nu ati citit o expresie regulata. " << std::endl;
				break;
			}
			case 5: {
				if (myRegex)
				{
					string paragraf;
					std::cout << "Introduceti un paragraf: " << std::endl; {
						char buff[50] = "";
						do {
							cin.getline(buff, 50);
							if (string(buff) != "$")
							{
								paragraf += buff;
								paragraf += " ";
							}
						} while (string(buff) != "$");
					}
					auto cuvinte = myRegex->findAll(string(paragraf));
					if (cuvinte.size() != 0) std::cout << "Am gasit " << cuvinte.size() << " cuvinte in paragraf." << std::endl;
					for (int i = 0; i < cuvinte.size(); i++)
						std::cout << cuvinte[i] << " ";
					std::cout << std::endl;
				}
				else std::cout << "Nu ati citit o expresie regulata. ";
				break;
			}
			case 6: {
				goto end;
			}
		}
		system("PAUSE");
		system("cls");
	}
	end: 
	delete myRegex;
    return 0;
}

