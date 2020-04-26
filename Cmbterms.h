#pragma once
#ifndef CMBTERM_H
#define CMBTERM_H

#include <iostream>
#include <set>
#include <vector>
#include <algorithm>
#include <iterator>
using namespace std;

class Cmbterms{
	static const int group[16];//group for uncombined terms
	static const int init_pos1[16][4];//pos of 1s for uncombined terms
    static const char c[4];
private:
	set<int> minterm;//terms combined
	set<int> pos_1;//1's position in the 4-bit term(0~3bit)
	set<int> pos_x;//don't care's position in the 4-bit term(0~3bit)
	bool checked;//whether term is checked when using QuineMcCluskey method

public:
	//functions
	Cmbterms();//default constructor
	Cmbterms(int);//constructor for uncombined terms
	Cmbterms(const Cmbterms &);//copy constructor
	int getGroup() const;
	set<int> getPos_1();
	set<int> getPos_x();
	set<int> getMinterm() const;
	bool getChecked();
	bool canCombine(const Cmbterms &);//check if two Cmbterms can combine
	Cmbterms operator+(Cmbterms &);//combine two Cmbterms if possible
	void print() const;
    void printC() const;
	void printCombined() const;
	void printBinary() const;
	void printChecked() const;
};

#endif
