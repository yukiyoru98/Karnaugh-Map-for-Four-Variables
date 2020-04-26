#pragma once
#ifndef PRINT_H
#define PRINT_H

#include <vector>
#include <set>
#include "Cmbterms.h"

using namespace std;

//print func. prototype
void printVset(vector< set<int> >);
void printSet(set<int>);
void printSset(set< set<int> >);
void printSum(set< set< set<int> > >);
void printKmap(const int[16]);
void printGroup(const vector< vector<Cmbterms> > &);
void printPiChart(const vector< Cmbterms > &, const vector<int> &, const vector<int> &);

#endif
