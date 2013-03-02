#include <iostream>;
#include <string>;
using namespace std; 

class Towers_of_Hanoi{
private: 
	int discs;
	Pegs opeg;
	Pegs fpeg;
	Pegs tpeg;

public:
	Towers_of_Hanoi(int d, string o, string f, string t){
		discs = d;
		opeg = Pegs(o, d);
		fpeg = Pegs(f,0);
		tpeg = Pegs(t,0);			
	}
	void solve(){
		if(opeg.height() == 1){
			cout << "Move a peg from " << opeg.ID() << " to " << fpeg.ID() << endl;
		}
		else{
			solve
		}
	}
};

class Pegs{
private:
	string name;
	int number_of_discs;
public:
	Pegs(string n, int discs){
		name = n;
		number_of_discs = discs;
	}
	string ID(){
		return name;
	}
	int height(){
		return number_of_discs;
	}
};

int main(int argc, char *argv[]){

}

