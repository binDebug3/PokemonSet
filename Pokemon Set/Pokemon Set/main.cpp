#include "Set.h"
#include "HashMap.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#ifdef _MSC_VER
#define _CRTDBG_MAP_ALLOC  
#include <crtdbg.h>
#define VS_MEM_CHECK _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#else
#define VS_MEM_CHECK
#endif

using namespace std;

int main(int argc, char* argv[]) {
	VS_MEM_CHECK

	//open files
	if (argc < 3) {
		cerr << "Please provide name of input and output files";
		return 1;
	}
	ifstream in(argv[1]);
	if (!in.is_open()) {
		cout << "Uh oh there was an inputFile error..." << endl;
		return 2;
	}
	ofstream out(argv[2]);
	if (!out.is_open()) {
		cout << "Uh oh there was an outputFile error..." << endl;
		return 3;
	}

	//set maps and input variables
	HashMap<string, string> pokemon;
	HashMap<string, string> move;
	HashMap<string, Set<string>> effective;
	HashMap<string, Set<string>> ineffective;
	string input, line;
	string pokemonName, baseType;
	string moveName, moveType;
	int count = 0;

	while (in >> input) {
		count++;
		if (input == "Set:") {
			Set<string> item;
			getline(in, line);
			istringstream os(line);
			while (os >> pokemonName) {
				item.insert(pokemonName);
			}
			out << input << " " << line << endl;
			out << "  [" << item << "]" << endl;
		}
		else if (input == "Pokemon:") {
			in >> pokemonName;
			in >> moveType;
			pokemon[pokemonName] = moveType;
			out << input << " " << pokemonName << " " << moveType << endl;
		}
		else if (input == "Move:") {
			in >> moveName;
			in >> moveType;
			move[moveName] = moveType;
			out << input << " " << moveName << " " << moveType << endl;
		}
		else if (input == "Effective:") {
			in >> baseType;
			getline(in, line);
			istringstream os(line);
			while (os >> moveType) {
				effective[baseType].insert(moveType);
			}
			out << input << " " << baseType << line << endl;
		}
		else if (input == "Ineffective:") {
			in >> baseType;
			getline(in, line);
			istringstream os(line);
			while (os >> moveType) {
				ineffective[baseType].insert(moveType);
			}
			out << input << " " << baseType << line << endl;
		}
		else if (input == "Pokemon") {
			out << endl << input << ": " << pokemon.size() << "/" << pokemon.max_size();
			out << pokemon;
		}
		else if (input == "Moves") {
			out << endl << endl << input << ": " << move.size() << "/" << move.max_size();
			out << move;
		}
		else if (input == "Effectivities") {
			out << endl << endl << input << ": " << effective.size() << "/" << effective.max_size();
			out << effective;
		}
		else if (input == "Ineffectivities") {
			out << endl << endl << input << ": " << ineffective.size() << "/" << ineffective.max_size();
			out << ineffective;
		}
		else if (input == "Battle:") {
			//set a bunch of variables
			string pokemonName2, moveName2, pokemonType, pokemonType2, moveType2;
			in >> pokemonName;
			in >> moveName;
			in >> pokemonName2;
			in >> moveName2;
			pokemonType = pokemon[pokemonName];
			pokemonType2 = pokemon[pokemonName2];
			moveType = move[moveName];
			moveType2 = move[moveName2];
			//------------------------------------------------------------------------------------------------------------------
			//check if the the SECOND pokemon's type is on the list of the supereffective types of the attack of the FIRST pokemon
			stringstream listMove;
			listMove << effective[moveType];
			string checkWeak;
			while (listMove >> checkWeak) {
				if (checkWeak.substr(0, 3) == pokemonType2.substr(0, 3)) {
					checkWeak = "super effective";
					break;
				}
			}
			//check the ineffective list from the FIRST pokemon against the SECOND
			if (checkWeak != "super effective") {
				stringstream().swap(listMove);
				listMove << ineffective[moveType];
				while (listMove >> checkWeak) {
					if (checkWeak.substr(0, 3) == pokemonType2.substr(0, 3)) {
						checkWeak = "ineffective";
						break;
					}
				}
			}
			//if neither, it is boring
			if (checkWeak != "super effective" && checkWeak != "ineffective") {
				checkWeak = "effective";
			}
			//check if the the FIRST pokemon's type is on the list of the supereffective types of the attack of the SECOND pokemon
			stringstream listMove2;
			listMove2 << effective[moveType2];
			string checkWeak2;
			while (listMove2 >> checkWeak2) {
				if (checkWeak2.substr(0, 3) == pokemonType.substr(0, 3)) {
					checkWeak2 = "super effective";
					break;
				}
			}
			//check the ineffective list from the SECOND pokemon against the FIRST
			if (checkWeak2 != "super effective") {
				stringstream().swap(listMove2);
				listMove2 << ineffective[moveType2];
				while (listMove2 >> checkWeak2) {
					if (checkWeak2.substr(0, 3) == pokemonType.substr(0, 3)) {
						checkWeak2 = "ineffective";
						break;
					}
				}
			}
			//if neither, it is boring
			if (checkWeak2 != "super effective" && checkWeak2 != "ineffective") {
				checkWeak2 = "effective";
			}
			//------------------------------------------------------------------------------------------------------------------
			//look for all possible combinations of effectiveness and report winner
			string winner;
			if (checkWeak == "super effective") {
				if (checkWeak2 == "super effective")
					winner = " is a tie.";
				else
					winner = ", " + pokemonName + " wins!";
			}
			else if (checkWeak2 == "super effective") {
				winner = ", " + pokemonName2 + " wins!";
			}
			else if (checkWeak == "ineffective") {
				if (checkWeak2 == "ineffective") 
					winner = " is a tie.";
				else
					winner = ", " + pokemonName2 + " wins!";
			}
			else if (checkWeak == "effective") {
				if (checkWeak2 == "effective") 
					winner = " is a tie.";
				else
					winner = ", " + pokemonName + " wins!";
			}
			//------------------------------------------------------------------------------------------------------------------
			out << endl <<	endl << input << " " << pokemonName << " " << moveName << " " << pokemonName2 << " " << moveName2 << endl;
			out << "  " << pokemonName << " (" << moveName << ") vs " << pokemonName2 << " (" << moveName2 << ")" << endl;
			out << "  " << pokemonName << " is a " << pokemonType << " type Pokemon." << endl;
			out << "  " << pokemonName2 << " is a " << pokemonType2 << " type Pokemon." << endl;
			out << "  " << moveName << " is a " << moveType << " type move." << endl;
			out << "  " << moveName2 << " is a " << moveType2 << " type move." << endl;
			out << "  " << moveName << " is super effective against [" << effective[moveType] << "] type Pokemon." << endl;
			out << "  " << moveName << " is ineffective against [" << ineffective[moveType] << "] type Pokemon." << endl;
			out << "  " << pokemonName << "'s " << moveName << " is " << checkWeak << " against " << pokemonName2 << endl;
			out << "  " << moveName2 << " is super effective against [" << effective[moveType2] << "] type Pokemon." << endl;
			out << "  " << moveName2 << " is ineffective against [" << ineffective[moveType2] << "] type Pokemon." << endl;
			out << "  " << pokemonName2 << "'s " << moveName2 << " is " << checkWeak2 << " against " << pokemonName << endl;
			if (winner == " is a tie.")
				out << "  The battle between " << pokemonName << " and " << pokemonName2 << winner << endl;
			else
				out << "  In the battle between " << pokemonName << " and " << pokemonName2 << winner << endl;
		}
		else {
			out << "uh oh" << endl;
		}
	}

	in.close(); 
	out.close(); 
	return 0;
}