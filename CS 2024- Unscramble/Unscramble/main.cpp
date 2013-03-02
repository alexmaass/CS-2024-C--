/**
	The final project for CS 2024: C++. The purpose of the project is to create
	a word unscrambling game. The game is designed to be played through the
	command line and has numerous options built in. Note: there is a large load time
	of around 30 seconds before the main menu becomes available. 
*/


#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <vector>
#include <algorithm>
#include <sstream>

using namespace std;

// The global dictionary to use.
static vector<string> dictionary;
static int size;

// The score class which contains the name and scores of high scorers.
class Score{
private:
	string name;
	int score;
public:
	Score(string n, int s){
		name = n;
		score = s;
	}
	int getscore(){
		return score;
	}
	string getname(){
		return name;
	}
	void print(){
		cout << name << ":" << score << endl;
	}
};

// Global variable to store the high scores. 
static vector<Score> highscores;

// Method to generate the dictionary array. It reads a file
// and stores the words in memory in the dictionary vectory. 
void generateDictionary(string file){
	string line;

	// Open the input file stream for conversion.
	ifstream in(file);
	if (!in.is_open()){
		cout << "Unable to open dictionary source file. Program will now exit." << endl;
		system("Pause");
		exit(1);
	}
			
	// Main loop to convert words to fixed length. 
	while(in.good()){
		getline(in,line);
		dictionary.push_back(line);
	}
	in.close();

	// Update the size variable.
	size = dictionary.size();
}

// A simple function to return a random word from the dictionary vector. 
string getWord(int maxsize){
	string word;
	// Random access of the vector.
	do{
		int random = rand()*rand() % (int)size;
		word = dictionary[random];
	} while(word.length() > maxsize);
	return word;
}

// The method that scrambles the input word and returns it. 
string scramble(string word){
	int index;
	string scrambled = word;
	while (scrambled.compare(word) == 0){
		for(int i=0; i<word.length(); i++){
			index = rand() % word.length();
			char t = scrambled[i];
			scrambled[i] = scrambled[index];
			scrambled[index] = t;
		}
	}
	return scrambled;
}

// The method to display the various options of the main menu. 
int displayoptions(){
	string selection="";
	int result=0;
	cout << "" << endl;
	cout << "Please select from the follow options below." << endl;
	cout << "1. Start a round of Unscramble" << endl;
	cout << "2. Read the instructions" << endl;
	cout << "3. View high scores" << endl;
	cout << "4. Change settings" << endl;
	cout << "5. Quit" << endl;
	cout << "Please input the number of your selection (1,2,3...): ";
	cin >> selection;
	// Parse user input and execute desired action.
	stringstream(selection) >> result;
	return result;
}

// The method that allows for setting changes. 
int changesettings(){
	string response="";
	int result=0;
	cout << "" << endl;
	cout << "Which setting would you like to do?" << endl;
	cout << "1. Change max length of word." << endl;
	cout << "2. Change time limit." << endl;
	cout << "3. Reset high scores." << endl;
	cout << "4. Return to main menu." << endl;
	cout << "Please input your selection based on the option number (1,2,..): ";
	cin >> response;
	stringstream(response) >> result;
	return result;
}

// The method that prints out the high scores stored in memory. 
void displayscores(){
	int max = min(10,(int)highscores.size());
	int i = 0;
	cout << "" << endl;
	cout << "The top 10 high scores are: " << endl;
	while(i<max){
		highscores[i].print();
		i++;
	}
}

// A comparator method that compares two Score instances. 
bool compare(Score a, Score b){
	return (a.getscore() > b.getscore());
}

// Adds a Score instance to the highscores array. 
void addscore(int count){
	string response;
	int score = count;
	cout << "Please input your three letter intials." << endl;
	cin >> response;
	if (response.length() > 3){
		response = response[0,2];
	}
	while(response.length() < 3){
		response = response + " ";
	}
	Score toadd = Score(response,score);
	highscores.push_back(toadd);
	sort(highscores.begin(),highscores.end(),compare);
}

// The main method of the project. It makes function calls
// to generate the dictionary and prompt the user for options.
int main(){
	time_t start, end;
	double dif,limit;
	
	vector<string> dictionary;
	int result, maxsize, count;
	string response, word;
	bool repeat1, repeat2, cont;

	// Initialize variables
	maxsize = 30;
	limit = 60.0;
	count = 0;

	
	// Generate the dictionary.
	srand(time(NULL));
	cout << "Loading..." << endl;
	generateDictionary("enable.txt");

	while(true){
		// Prompt user for option input.
		result = displayoptions();

		// Execute desired action.
		switch(result){
		// Option 1: Play the game.
		case 1:
			cout << "Starting the game..." << endl;
			cont = true;
			count = 0;
			time(&start);
			while(cont){
				// Generate random word.
				word = getWord(maxsize);
	
				// Scramble the word.
				string scrambled = scramble(word);
		
				// Display scrambled word and ask user for thier answer.
				do{
					bool flag = true;
					cout << "" << endl;
					cout << "The scrambled word is " << scrambled << endl;
					cout << "What is your guess?" << endl;
					cin >> response;
					time(&end);
					dif = difftime(end,start);
					if(dif >= limit){
						cout << "" << endl;
						cout << "TIME'S UP!" << endl;
						cont = false;
						break;						
					}
				} while(response.compare(word) != 0 && response.compare("giveup") != 0 && dif < limit);
				if (response.compare("giveup") == 0){
					cout << "Skipping the current word.." << endl;
				} 
				else{
					count++;
					cout << "Nice job! Moving onto the next word.." << endl;
				}
			}
			cout << "GAME OVER" << endl;
			cout << "You managed to decipher " << count << " words." << endl;
			addscore(count);
			break;
		// Option 2: Display the instructions.
		case 2:
			cout << "" << endl;
			cout << "The Instructions:" << endl;
			cout << "The point of this game is to unscramble as many words as possible in a given " << endl;
			cout << "time limit. The word will be presented in a scrambled format, you will have  " << endl;
			cout << "to type in the unscrambled word. If you ever get stuck, type 'giveup' to skip" << endl;
			cout << "the current word. You will have 60 seconds to try and decipher as many words " << endl;
			cout << "as possible. Good luck!" << endl;
			system("Pause");
			break;
		// Option 3: Diplay the high scores.
		case 3:
			displayscores();
			break;
		// Option 4: Change Settings.
		case 4:
			repeat1 = true;
			while(repeat1){
				repeat2 = true;
				result = changesettings();
				switch(result){
				// Option 1: Change max word length.
				case 1:
					while (repeat2){
						cout << "What would you like to set new max lenth of words to be?" << endl;
						cout << "Please input your option as a positive integer greater than 2.";
						cin >> response;
						stringstream(response) >> result;
						if (result > 2){
							maxsize = result;
							repeat2 = false;
							cout << "The max word size is " << maxsize << endl;
						}
					}
					break;
				// Option 2: Change time limit.
				case 2:
					while (repeat2){
						cout << "What would you like to set new time limit to be?" << endl;
						cout << "Please input your option as a positive integer greater than 0.";
						cin >> response;
						stringstream(response) >> result;
						if (result > 0){
							limit = result;
							repeat2 = false;
							cout << "The max time limit is " << limit << endl;
						}
					}
					break;

				// Option 3: Reset high scores.
				case 3:
					break;
				
				// Option 4: Return to main menu.
				case 4:
					repeat1 = false;
					break;

				// Default case
				default:
					cout << "Input invalid. Please try again." << endl;
					system("Pause");
					break;
				}
			}
			break;
		
		// Option 5: Quit. 
		case 5:
			cout << "" << endl;
			cout << "Thanks for playing!" << endl;
			exit(0);
		
		// Default: Invalid input. 
		default:
			cout << "Input invalid. Please try again." << endl;
			system("Pause");
			break;
		}
	}

	system("Pause");
}


