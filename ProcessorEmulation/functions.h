#include <iostream>
#include <bitset>
#include <conio.h>
#include <vector>

#include <fstream>
#include <sstream>
#include <string>
using namespace std;

class Reg {
public: 
	Reg();
	Reg(const string&);

	void setName(const string&);
	string getName();
	long getNum();

	void load(const Reg&);
	void load(const int&);

	operator string();
	
	unsigned long ms(const Reg&);
	unsigned long ms(const int&);

	void customFunc(const int a);
	bool getPS();

private:
	string regName;
	static const unsigned int bitSize = 20; //16
	bitset<bitSize> bitForm;
	bool PS;

	unsigned long sum_byte();
};

class Processor {
public: 
	Processor();
	void process();

private:
	void tactPass();
	void parseFile(ifstream&);
	inline bool isInteger(const string&s);
	void printRegs();

	size_t getRegPos(const string&);
	bool hasReg(const string&);
	
	int tactNum;
	int position;
	int commandNum;
	
	static const char delimeter = 32;
	
	vector<string> commandsVect;
	vector<Reg> regsVect;
};

