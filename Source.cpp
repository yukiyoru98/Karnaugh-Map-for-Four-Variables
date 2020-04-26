#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cmath>
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>
#include "print.h"
#include "Cmbterms.h"
using namespace std;

//func. prototype
void mintermExpansion(int[], int, int, int[], int);
void getSolution(vector<Cmbterms> &, vector<int>, const int[], const char &, const char &);

//main function
int main() {
    
    int min_sop[16] = { 0 };
    
    //open file
    ifstream read("/Users/christine/Desktop/HW/106學年度/下學期/KarnaughMap/KarnaughMap/input.txt", ios::in);
    if (!read) {//failed to read file
        cerr << "File cannot be opened.\n";
        exit(1);
    }
    //read a line
    string s;
    getline(read, s);
    s += ' ';//add to the end of the string to avoid out of range
    
    int output = 1;//output of the term (1 for minterms and -1 for don't care terms)
    int term[4] = { -1, -1, -1, -1 };//save term
    
    for (int i = 0; i < s.length(); i++) {//scan the string to find terms
        //read variable
        int value = 1;//represents voltage high
        //need to take compliment
        if (s[i + 1] == '\'') {
            value = 0;
        }
        
        switch (s[i]) {//convert into binary
            case 'a':
                term[3] = value;
                break;
                
            case 'b':
                term[2] = value;
                break;
                
            case 'c':
                term[1] = value;
                break;
                
            case 'd':
                term[0] = value;
                break;
                
            case '('://don't care
                output = -1;
                break;
                
            default:
                break;
        }
        
        //term is completed when s[i] is char & s[i+1] is not
        //start expanding the term
        if (((isalpha(s[i])) || (s[i] == '\''))
            && (!isalpha(s[i + 1])) && (s[i + 1] != '(') && (s[i + 1] != '\''))
        {
            mintermExpansion(min_sop, output, -1, term, 3);
            //reset term[4]
            for (int i = 0; i < 4; i++) {
                term[i] = -1;
            }
            //reset output
            output = 1;
        }
    }
    
    //record only minterms
    vector<int> minterms;
    //print minterms and don't cares
    cout << "Minterms:";
    for (int i = 0; i < 16; i++) {
        if (min_sop[i] == 1) {
            cout << i << ',';
            minterms.push_back(i);
        }
    }
    cout << endl;
    cout << "Don't care:";
    for (int i = 0; i < 16; i++) {
        if (min_sop[i] == -1) {
            cout << i << ',';
        }
    }
    cout << endl << endl;
    
    //print Kmap
    printKmap(min_sop);
    
    //USER selection:
    cout << "Please select...\n";
    cout << "(1)View solution for Karnaugh Map\n";
    cout << "(2)View other methods' solution(step-by-step)\n";
    char choice, enter;
    cin >> choice;
    //illegal input
    while (choice != '1' && choice != '2') {
        cout << "Please try again...\n";
        cin >> choice;
    }
    
    //Preparation for Quine McClasky or Petrick's method
    
    vector< vector<Cmbterms> > G(5);
    //sort minterms into groups first
    for (int i = 0; i < 16; i++) {
        if ((min_sop[i] == 1) || (min_sop[i] == -1)) {
            Cmbterms c(i);
            G.at(c.getGroup()).push_back(c);
        }
    }
    
    if (choice == '2') {//print step-by-step
        //print Group Table
        system("cls");
        cout << "* Step1: Group the minterms according to the number of 1's.\n\n";
        printGroup(G);
        cin.ignore();//ignore \n from previous input
        cout << "\nPress ENTER to see the next step...\n";
        cin.get(enter);
    }
    
    
    //combine minterms into a set-> put into new group
    for (int i = 0; i < G.size() - 1; i++) {//repeat until reaching the last group
        vector<Cmbterms> temp;//temporarily save the sets that are combined for g[i] and g[i+1]
        //pick a minterm set from group[i] and one from group[i+1]
        for (int j = 0; j < G[i].size(); j++) {
            for (int k = 0; k < G[i + 1].size(); k++) {
                if (G[i][j].canCombine(G[i + 1][k])) {//if two sets can combine
                    Cmbterms new_combine = G[i][j] + G[i + 1][k];//operator+
                    //put combined set into temp
                    bool repeat = false;//avoid inserting a repeated combined minterm set
                    for (int i = 0; i < temp.size(); i++) {
                        if (temp[i].getMinterm() == new_combine.getMinterm()) {//set repeated
                            repeat = true;
                        }
                    }
                    if (repeat == false) {//set not repeated
                        temp.push_back(new_combine);//insert into temp
                    }
                }
                
            }
        }
        if (temp.size() != 0) {//temp is not empty
            G.push_back(temp);//put into G and become a new group
            
            if (choice == '2') {//print step-by-step
                system("cls");
                cout << "* Step 2:\nComparison of adjacent groups:\n";
                cout << "Combine terms that have the same '-' and differ one in the number of 1's.\n";
                cout << "Check off the combined terms.\n";
                printGroup(G);
                cout << "\nPress ENTER to see the next step...\n";
                cin.get(enter);
            }
        }
    }
    
    //make list of PIs
    //look for prime implicants(unchecked Cmbterms)
    vector< Cmbterms > pi_list;
    for (int i = 0; i < G.size(); i++) {
        for (int j = 0; j < G[i].size(); j++) {
            if (G[i][j].getChecked() == false) {
                pi_list.push_back(G[i][j]);
            }
        }
    }
    
    //print pilist
    system("cls");
    if (choice == '2') {
        cout << "* Step3:The remaining unchecked terms are prime implicants:\n";
        for (int i = 0; i < pi_list.size(); i++) {
            cout << "[P" << i << "] ";
            pi_list[i].printCombined();
            cout << "->";
            pi_list[i].printC();
            cout << endl;
        }
    }
    
    if (choice == '2') {
        cout << "\nDraw the Prime Implicant Chart.\n";
        vector<int> isPruned_pi(pi_list.size(), -1);
        //print Prime Implicant Chart
        printPiChart(pi_list, minterms, isPruned_pi);
    }
    
    //USER select: apply QM or Petrick's method
    char choice2;
    if (choice == '2') {
        cout << "\nPlease select a method...\n";
        cout << "(1)Quine MacClasky Method\n";
        cout << "(2)Petrick's Method\n";
        
        cin >> choice2;
        //illegal input
        while (choice2 != '1' && choice2 != '2') {
            cout << "Please try again...\n";
            cin >> choice2;
        }
    }
    //find solution
    getSolution(pi_list, minterms, min_sop, choice, choice2);
    
    system("pause");
    return 0;
}

//func. def.

//recursive func. to find mintermExpansion
void mintermExpansion(int min_sop[16], int output, int minterm, int term[4], int b) {
    
    if (b < 0) {//reach final literal
        
        if (min_sop[minterm] != 1) {//avoid don't care terms covering minterms
            min_sop[minterm] = output;
        }
        
    }
    else {//not yet reach final literal
        
        for (int i = 0; i <= 1; i++) {
            if (term[b] != 1 - i) {//need to execute
                
                //add value to midterm
                if (minterm == -1) {
                    //term[b] is the first appeared literal in the term
                    minterm = static_cast<int>(i * pow(2, b));//use assignment operation
                }
                else {
                    minterm += static_cast<int>(i * pow(2, b));//use addition
                }
                mintermExpansion(min_sop, output, minterm, term, b - 1);//recurse:expand next literal
            }
        }
    }
}


void getSolution(vector<Cmbterms> &pi_list, vector<int> pruned_minterms, const int min_sop[16], const char &choice, const char &choice2) {
    vector<int> isPruned_pi(pi_list.size(), 0);//for printing solution step-by-step
    
    //message to USER
    char enter;
    system("cls");
    if (choice == '2') {
        if (choice2 == '1') {
            cout << "\nApply Quine McClasky Method.\n";
        }
        else {
            cout << "\nApply Petrick's Method.\n";
        }
        printPiChart(pi_list, pruned_minterms, isPruned_pi);
        cin.ignore();
        cout << "\nPress ENTER to see the next step...\n";
        cin.get(enter);
    }
    
    //make a table with minterms and the prime implicants
    //the set<int> in min_pi[i] records the prime implicants that includes the i-th minterm
    //but non-minterms are still in the table for conveniency, they will be pruned later
    vector<set<int>> min_pi(16);
    for (int i = 0; i < pi_list.size(); i++) {
        std::set<int>::iterator it;
        set<int> m = pi_list[i].getMinterm();//make a copy for convenience
        for (it = m.begin(); it != m.end(); it++) {//put the minterm set of Cmbterms into table
            if (min_sop[*it] == 1) {//*it is a minterm
                min_pi[*it].insert(i);//put all minterms in the minterm set sequentially into table
            }
        }
    }
    //erase non-minterms from min_pi table
    std::vector<set<int>>::iterator it_min_pi;
    int i_min_pi;
    for (it_min_pi = min_pi.begin(), i_min_pi = 0; it_min_pi != min_pi.end();) {
        if (min_pi[i_min_pi].empty()) {
            it_min_pi = min_pi.erase(it_min_pi);
        }
        else {
            it_min_pi++;
            i_min_pi++;
        }
    }
    
    vector<set<int>> all_solutions;
    set<int> solution;
    
    //apply QM
    if (choice == '2') {
        if (choice2 == '1') {
            for (int i = 0; i < min_pi.size();) {//look for essential prime implicant in the remaining pi chart
                int ess_pi;
                if (min_pi[i].size() == 1) {
                    ess_pi = *(min_pi[i].begin());
                    solution.insert(ess_pi);//record ess_pi in solution
                    //mark ess_pi as pruned in isPruned_pi
                    isPruned_pi[ess_pi] = 1;
                    cout << "\nMinterm" << pruned_minterms[i] << " is only covered by " << " P" << ess_pi << "\n";
                    cout << "P" << ess_pi << " is a essential prime implicant. Delete P" << ess_pi << " and the other minterms it covers\n\n";
                }
                else {
                    i++;
                    continue;
                }
                
                //prune min_pi table
                std::vector<set<int>>::iterator it_min_pi;
                std::set<int>::iterator it_solution;
                std::vector<int>::iterator itv;
                int i_min_pi;//counter in the for-loop
                for (it_min_pi = min_pi.begin(), i_min_pi = 0, itv = pruned_minterms.begin(); it_min_pi != min_pi.end(); ) {
                    if (min_pi[i_min_pi].count(ess_pi)) {//find minterms that are covered by ess_pi
                        //erase it from min_pi and in pruned_minterms
                        
                        it_min_pi = min_pi.erase(it_min_pi);
                        itv = pruned_minterms.erase(itv);
                    }
                    else {
                        it_min_pi++;
                        i_min_pi++;
                        itv++;
                    }
                }
                
                printPiChart(pi_list, pruned_minterms, isPruned_pi);
                char enter;
                cout << "\nPress any key to see the next step...\n";
                cin.get(enter);
            }
            
            if (pruned_minterms.empty()) {//no more minterms uncovered
                all_solutions.push_back(solution);
            }
        }
    }//end of QM
    cout << "QM:";
    printSet(solution);
    cout << endl;
    //apply petrick's method
    if (all_solutions.size() == 0) {//QM reaches here if it doesn't generate a complete sol((because the remaining chart is cyclic
        
        //message to USER who selects QM
        if (choice == '2') {
            if (choice2 == '1') {
                cout << "The remaining chart is CYCLIC, apply Petrick's Method to find solution.\n";
            }
        }
        
        set< set< set<int> > > sum;//used to save the equation(SOP)
        
        for (int i = 0; i < min_pi.size(); i++) {//form the initial petrick's method equation EX:(P1+P2)(P2+P3(P3+P4)
            set<int> s = min_pi[i];//s is the set of prime implicants that covers the i-th  minterm
            
            set<set<int>> proterm;//product term (P1+P2+...Pn)
            std::set<int>::iterator it;
            for (it = s.begin(); it != s.end(); it++) {//insert the prime implicants in s seperately to form a product term
                set<int> pi;//save as set<int> because each P inside the product term might not be a single prime implicant after consequent calculation
                pi.insert(*it);
                proterm.insert(pi);
            }
            sum.insert(proterm);
        }
        if (choice == '2') {
            cout << "\n< Equation >\n   ";
            printSum(sum);
            cout << "\nPress ENTER to see the next step...\n";
            cin.get(enter);
        }
        
        //iterators declaration
        std::set< set< set<int> > >::iterator its;
        std::set< set< set<int> > >::iterator its2;
        std::set< set<int> >::iterator itp;
        std::set< set<int> >::iterator itp2;
        
        for (its = sum.begin(); its != sum.end();) {
            bool modified = false;
            for (its2 = its; its2 != sum.end(); ) {
                if (its2 == its) {//skip itself
                    its2++;
                    continue;
                }
                cout << "1: ";printSset(*its);cout << endl;
                cout << "2: ";printSset(*its2);cout << endl;
                //reduce redundance term(two conditions because in the function "includes(a_begin, a_end, b_begin, b_end)", a must be larger than b)
                //condition1:(X+Y) X = X
                if (includes(its->begin(), its->end(), its2->begin(), its2->end())) {
                    cout << "1-erase :";printSset(*its);cout << endl;
                    its = sum.erase(its);
                    modified = true;
                    its = sum.begin();
                    
                    if (choice == '2') {
                        cout << " = ";
                        printSum(sum);
                        cout << "\nPress ENTER to see the next step...\n";
                        cin.get(enter);
                    }
                    break;
                }
                //condition2:X (X+Y) = X
                if (includes(its2->begin(), its2->end(), its->begin(), its->end())) {
                    cout << "2-erase :";printSset(*its2);cout << endl;
                    its2 = sum.erase(its2);
                    modified = true;
                    its = sum.begin();
                    
                    if (choice == '2') {
                        cout << " = ";
                        printSum(sum);
                        cout << "\nPress ENTER to see the next step...\n";
                        cin.get(enter);
                    }
                    break;
                }
                //or else
                //factorization: (X+Y)(X+Z)=X+YZ
                //X is intersection part, YZ is product of different parts
                //partA: find X
                set< set<int> > inter;
                set_intersection((*its).begin(), (*its).end(), (*its2).begin(), (*its2).end(), inserter(inter, inter.begin()));
                
                if (!inter.empty()) {//can factorize the two sets if intersection is not empty
                    //partB:find YZ
                    //remove intersection from origin to get the remaining different parts
                    set< set<int> > dif;
                    set< set<int> > dif2;
                    set_difference((*its).begin(), (*its).end(), inter.begin(), inter.end(), inserter(dif, dif.begin()));
                    set_difference((*its2).begin(), (*its2).end(), inter.begin(), inter.end(), inserter(dif2, dif2.begin()));
                    
                    for (itp = dif.begin(); itp != dif.end(); itp++) {//merge dif "one by one"(because Y and Z could be POS) EX: Y=(A+B)-> YZ=(A+B)Z=AZ+BZ
                        for (itp2 = dif2.begin(); itp2 != dif2.end(); itp2++) {
                            set< set<int> > merge_dif;
                            set<int> merge_pi;
                            
                            set_union((*itp).begin(), (*itp).end(), (*itp2).begin(), (*itp2).end(), inserter(merge_pi, merge_pi.begin()));
                            //convert into set< set<int> > form
                            merge_dif.insert(merge_pi);
                            //merge partA and partB to get (X+YZ)
                            set< set<int> > lastMerge;
                            set_union(merge_dif.begin(), merge_dif.end(), inter.begin(), inter.end(), inserter(lastMerge, lastMerge.begin()));//unite inter and merge dif and put into sum
                            //put the result into sum
                            sum.insert(lastMerge);
                            
                            if (choice == '2') {
                                cout << " = ";
                                printSum(sum);
                                cout << "\nPress ENTER to see the next step...\n";
                                cin.get(enter);
                            }
                        }
                    }
                    //remove original pos term (X+Y)and (X+Z) after factorizing
                    its2 = sum.erase(its2);
                    its = sum.erase(its);
                    modified = true;
                    if (choice == '2') {
                        cout << " = ";
                        printSum(sum);
                        cout << "\nPress ENTER to see the next step...\n";
                        cin.get(enter);
                    }
                    its = sum.begin();//check from the beginning again since sum has been modified
                    break;
                }
                else {//nothing erased->iterator move on
                    its2++;
                }
            }
            if (modified == false) {//nothing erased->iterator move on
                its++;
            }
        }//factorization is done
        
        //next: multiply all out to get all possible solutions: (W+X)(Y+Z)=WY+WZ+XY+XZ
        for (its = sum.begin(); its != sum.end(); its = sum.begin()) {
            its2 = its;
            its2++;//don't multiply itself
            
            if (its2 == sum.end()) {//multiplying out procedure is done
                break;
            }
            
            set< set<int> > lastMerge;
            for (itp = (*its).begin(); itp != (*its).end(); itp++) {//merge different parts
                for (itp2 = (*its2).begin(); itp2 != (*its2).end(); itp2++) {
                    set<int> merge_pi;
                    set_union((*itp).begin(), (*itp).end(), (*itp2).begin(), (*itp2).end(), inserter(merge_pi, merge_pi.begin()));
                    //convert into set< set<int> > form
                    lastMerge.insert(merge_pi);
                }
            }
            //erase after multiplying out
            its2 = sum.erase(its2);
            its = sum.erase(its);
            
            //put back into sum
            sum.insert(lastMerge);
            //print sum
            if (choice == '2') {
                cout << " = ";
                printSum(sum);
                cout << "\nPress ENTER to see the next step...\n";
                cin.get(enter);
            }
        }
        
        //find solution with fewest product terms
        cout << "\n< SOLUTION >\n";
        
        if (choice == '2') {
            cout << "(Pick the terms with fewest prime implicants.)\n";
        }
        its = sum.begin();
        itp = its->begin();
        size_t min_size = itp->size();
        for (int i = 0; itp != its->end(); i++, itp++) {
            
            if (itp->size() < min_size) {//found solution with smaller size
                min_size = itp->size();
                all_solutions.clear();//clear all bigger solutions recorded
            }
            
            if (itp->size() == min_size) {
                //merge with solution found in QM if neccessary
                if (choice2 == '1') {
                    cout << "MERGE\n";
                    set<int> merge_pi;
                    set_union((*itp).begin(), (*itp).end(), solution.begin(), solution.end(), inserter(merge_pi, merge_pi.begin()));
                    all_solutions.push_back(merge_pi);//record solutions with the currrently minimum size
                }
                else {
                    all_solutions.push_back(*itp);//record solutions with the currrently minimum size
                }
            }
        }
        
    }//solution found, end of Petrick's Method
    
    
    //print solution
    //print grouping result
    if (choice == '1') {
        printKmap(min_sop);
        cout << endl;
    }
    
    //print func.sol
    for (int i = 0; i < all_solutions.size(); i++) {
        if (i > 0) {
            cout << "\n\n or\n\n";
        }
        if (choice == '1') {//print the following if USER chooses to view Karnaugh Map solution
            std::set<int>::iterator it;
            int j;
            for (it = all_solutions[i].begin(), j = 1; it != all_solutions[i].end(); it++, j++) {
                cout << "group " << j << ":";
                pi_list[*it].printCombined();
                cout << endl;
                cout << "simplification of group " << j << "->";
                pi_list[*it].printC();
                cout << endl << endl;
            }
        }
        cout << "F(a,b,c,d) = ";
        std::set<int>::iterator it;
        for (it = all_solutions[i].begin(); it != all_solutions[i].end(); it++) {
            
            pi_list[*it].printC();
            if (it != --all_solutions[i].end()) {
                cout << '+';
            }
        }
    }
    cout << endl << endl;
}
