#include "functions.h"

Reg::Reg():PS(false) {}
Reg::Reg(const string& a) : regName(a), PS(false) {};

void Reg::load(const Reg& a) {
	bitForm = a.bitForm;
}

void Reg::load(const int& a) {
	bitForm = a;
}

Reg::operator string(){
	return bitForm.to_string();
}

bool Reg::getPS() {
	if (bitForm.to_string()[0] == '1') PS = 1;
	else PS = 0;
	return PS;
}

string Reg::getName(){
	return regName;
}

void Reg::setName(const string& a) {
	regName = a;
}

Processor::Processor() :commandNum(1), tactNum(0), position(0) {
	Reg R1("R1"), R2("R2"), R3("R3");
	regsVect.push_back(R1);
	regsVect.push_back(R2);
	regsVect.push_back(R3);
}


void Processor::parseFile(ifstream &source) {
	string line;
	string rest;
	size_t pos;
	while (getline(source, line, delimeter)) {
		pos = line.find("\n");
		if (pos != std::string::npos) {
			rest = line.substr(pos + 1, line.size());
			line = line.substr(0, pos);
			commandsVect.push_back(line);
			commandsVect.push_back(rest);
		}
		else 
			commandsVect.push_back(line);
	}
}

long Reg::getNum() {
	return bitForm.to_ulong();
};

void Processor::tactPass() {
	if (position == commandsVect.size())
		return;

	bool isDigit;
	tactNum = 0;
	string command, op1, op2;
	int op;
	command = commandsVect[position];
	op1 = commandsVect[position + 1];
	op2 = commandsVect[position + 2];

	if (!hasReg(op1)) {
		Reg temp(op1);
		regsVect.push_back(temp);
	}

	isDigit = isInteger(op2);

	if (isDigit) {
		op = atoi(op2.c_str());
	}
	else if (!hasReg(op2)) {
		Reg temp(op2);
		regsVect.push_back(temp);
	}

	cout << ">>> COMMAND BEGIN <<<\n";
	cout << command << " " << op1 << " " << op2 << endl;
	printRegs();
	cout << "PS: " << regsVect[getRegPos(op1)].getPS() << endl;
	cout << "PC: " << commandNum << endl;
	cout << "TC: " << ++tactNum << endl << endl;
	_getch();
	if (command == "LOAD") {
		if (isDigit)
			regsVect[getRegPos(op1)].load(op);
		else
			regsVect[getRegPos(op1)].load(regsVect[getRegPos(op2)]);
	}
	if (command == "F12") {
		if (isDigit)
			regsVect[getRegPos(op1)].customFunc(op);
		else 
			regsVect[getRegPos(op1)].customFunc(regsVect[getRegPos(op2)].getNum());
	}
	printRegs();
	cout << "PS: " << regsVect[getRegPos(op1)].getPS() << endl;
	cout << "PC: " << commandNum << endl;
	cout << "TC: " << ++tactNum << endl;
	cout << ">>> COMMAND END <<<\n\n";
	_getch();
	++commandNum;
	position += 3;
}

size_t Processor::getRegPos(const string&name){
	size_t i = 0;
	for (; i < regsVect.size(); ++i) {
		if (regsVect[i].getName() == name)
			return i;
	}
}

bool Processor::hasReg(const string&name){
	for (size_t i = 0; i < regsVect.size(); ++i) {
		if (regsVect[i].getName() == name)
			return true;
	}
	return false;
}

void Processor::printRegs(){
	for (auto it = regsVect.begin(); it != regsVect.end(); ++it) {
		cout << (*it).getName() << ": " << string(*it) << endl;
	}
	cout << endl;
}

bool Processor::isInteger(const std::string & s){
	if (s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+')))
		return false;

	char * p;
	strtol(s.c_str(), &p, 10);

	return (*p == 0);
}

void Processor::process() {
	ifstream stream("input.txt");
	parseFile(stream);
	while (position != commandsVect.size()) {
		tactPass();
	}
}


/*-------------------------------------------------------------*/

void Reg::customFunc(const int a) {
	if (a != 1 && a != 0)
		return;
	string t = bitForm.to_string();
	for (int i = 0; i < t.length() - 1; i += 2) {
		if (t[i] - '0' == a)
			swap(t[i], t[i + 1]);
	}
	bitset<bitSize> temp(t);
	bitForm = temp;
}


unsigned long Reg::ms(const Reg& mod) {
	unsigned long res = sum_byte() % mod.bitForm.to_ulong();
	load(int(res));
	return res;
}

unsigned long Reg::ms(const int&mod) {
	unsigned long res = sum_byte() % mod;
	load(int(res));
	return res;
}

unsigned long Reg::sum_byte() {
	unsigned long sum, op1, op2;
	string s = bitForm.to_string();
	string t1, t2;
	for (int i = 0; i < 4; ++i) {
		t1 += s[i + 4];

	}
	for (int i = 0; i < 4; ++i) {
		t2 += s[i + 12];

	}
	bitset<4> tmp1(t1);
	bitset<4> tmp2(t2);
	op1 = tmp1.to_ulong() < 10 ? tmp1.to_ulong() : tmp1.to_ulong() % 10;
	op2 = tmp2.to_ulong() < 10 ? tmp2.to_ulong() : tmp2.to_ulong() % 10;
	sum = op1 + op2;
	return sum;
}