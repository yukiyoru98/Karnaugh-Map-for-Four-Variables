#include "Cmbterms.h"
#include <iomanip>
using namespace std;

//static 
const int Cmbterms::group[16] = { 0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4 };
const int Cmbterms::init_pos1[16][4] = { {-1, 7, 7, 7}, {3, 7, 7, 7}, {2, 7, 7, 7}, {2, 3, 7, 7}, {1, 7, 7, 7}, {1, 3, 7, 7}, {1, 2, 7, 7}, {1, 2, 3, 7}, {0, 7, 7, 7}, {0,3, 7, 7}, {0,2, 7, 7}, {0, 2, 3, 7}, {0, 1, 7, 7}, {0, 1, 3, 7}, {0, 1, 2, 7}, {0, 1, 2, 3} };
const char Cmbterms::c[4] = { 'a','b','c','d' };

Cmbterms::Cmbterms() //default constructor
	:checked(false)
{
}

Cmbterms::Cmbterms(int minTerm) //constructor for uncombined terms
	:checked(false)
{
	minterm.insert(minTerm);
	for (int i = 0; i < 4; i++) {
		if ( (init_pos1[minTerm][i] < 4) && (init_pos1[minTerm][i] >= -1) ) {
			pos_1.insert(init_pos1[minTerm][i]);
		}
		else {
			break;
		}
	}
	
	pos_x.insert(-1);
}

Cmbterms::Cmbterms(const Cmbterms &another){//copy constructor
	this->minterm = another.minterm;
	this->pos_1 = another.pos_1;
	this->pos_x = another.pos_x;
	checked = false;
}

int Cmbterms::getGroup() const {
	if (pos_1.count(-1)) {//pos_1 contains -1 means there are no 1s
		return 0;
	}
	return pos_1.size();
}

set<int> Cmbterms::getPos_1() {
	return pos_1;
}

set<int> Cmbterms::getPos_x() {
	return pos_x;
}

set<int> Cmbterms::getMinterm() const {
	return minterm;
}

bool Cmbterms::getChecked() {
	return checked;
}

bool Cmbterms::canCombine(const Cmbterms &c2){//check if two Cmbterms can combine

	if (this->pos_x == c2.pos_x) {//positions of don't care must be the same
		if ( includes( c2.pos_1.begin(), c2.pos_1.end(), this->pos_1.begin(), this->pos_1.end() ) ) {
			//the smaller set of positions of 1s must be the subset of the bigger one
			return true;
		}
		//special case:no 1s
        if (this->pos_1.count(-1)) {
			return true;
		}
	}
	return false;
}

Cmbterms Cmbterms::operator+(Cmbterms &c2) {//combine two Cmbterms if possible
	
		Cmbterms new_combined;
		//combine new term
		std::set_union(this->minterm.begin(), this->minterm.end(), c2.minterm.begin(), c2.minterm.end(), inserter(new_combined.minterm, new_combined.minterm.begin()) );
		
		//initialize data members
		new_combined.pos_1 = this->pos_1;
		set<int> newPos_x;
		std::set_difference(c2.pos_1.begin(), c2.pos_1.end(), this->pos_1.begin(), this->pos_1.end(), inserter(newPos_x, newPos_x.begin()));
		std::set_union(newPos_x.begin(), newPos_x.end(), this->pos_x.begin(), this->pos_x.end(), inserter(new_combined.pos_x, new_combined.pos_x.begin()));

		//check the original terms
		this->checked = true;
		c2.checked = true;

		return new_combined;
}

void Cmbterms::print() const {

	//print combined minterms
	printCombined();
	cout << " \t";
	//print binary
	printBinary();

	//print checked
	printChecked();

	/*print pos_1;
	for (it = pos_1.begin(); it != pos_1.end(); it++) {
		cout << *it << ',';
	}
	cout << "\t";

	//print pos_x;
	for (it = pos_x.begin(); it != pos_x.end(); it++) {
		cout << *it << ',';
	}
	cout << "\t";*/
}

void Cmbterms::printC() const{
    //print char
    for (int i = 0; i < 4; i++) {
        if (pos_1.count(i)) {
            cout << c[i];
        }
        else if (pos_x.count(i)) {
            ;
        }
        else {
            cout << c[i] << '\'';
        }
    }
}

void Cmbterms::printCombined() const {
	std::set<int>::iterator it;
	int i;
	//print combined minterm
	for (it = minterm.begin(), i = 0; it != minterm.end(); it++, i++) {
		cout << *it;
		if (i != minterm.size()-1) {
			cout << ",";
		}
	}
	
}

void Cmbterms::printBinary() const {
	std::set<int>::iterator it;
	for (int i = 0; i < 4; i++) {
		if (pos_1.count(i)) {
			cout << '1';
		}
		else if (pos_x.count(i)) {
			cout << '-';
		}
		else {
			cout << '0';
		}
	}
	cout << '\t';
}

void Cmbterms::printChecked() const{
	
	if (checked == true) {
		cout << 'y';
	}
	else {
		cout << 'x';
	}
	cout << endl;
}