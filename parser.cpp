#include <string>
#include <stack>
#include "scanner.h"
#include "parser.h"
#include <iostream>
#include "grammar.h"
#include "prints.h"
using namespace std;


bool parser::startup(string filename) {
    scan.open(filename);
	if (scan.isError()) {
		cout << "Error opening file " << filename << ". \nPress ENTER to continue..." << endl;
		getchar();
		return false;
	}
    
    return true;
}

bool parser::isNonTerminal(token t) {

    if (t.getId() >= FIRST_NON_TERMINAL && t.getId() <= LAST_NON_TERMINAL)
        return true;
    else
        return false;
}

int parser::selectProd() {
    for (int i = 0; i < GR_NUMPRODS; i++) {
        auto prod = PROD[i];
        if (prod[GR_LHS] != tokStack.top().getId()) 
            continue;
            for (int j = GR_FIRST_SELSET; j <= GR_LAST_SELSET; ++j) 
                if (prod[j] == nextToken.getId() || prod[j] == TOK_DEFAULT) 
                    return i;
            
    }
    return -1;
}

void parser::pushProdRHS(int prodNum) {
    for (int i = GR_LAST_RHS; i >= GR_FIRST_RHS ; i--)
        if(PROD[prodNum][i] != E) {
            nextToken.setId(PROD[prodNum][i]);
            tokStack.push(nextToken);
        }
}

string parser::compile(string fn) {
    
    if (startup(fn)) {
        
        nextToken = scan.getNextToken();
        tokStack.push(nextToken);
     
        while ((!scan.isError()) && (!tokStack.empty()) && (nextToken.getId() != TOK_EOS)) { 
            
            top = tokStack.top(); 
            tokStack.pop();
            
            if (isNonTerminal(top))
                if (selectProd() != -1){
                    pushProdRHS(selectProd());
                    printProduction(selectProd());
                    
                }
                else
                    cout << "Error" << endl;
            
            else {
                nextToken = scan.getNextToken();
                if (top.getId() == nextToken.getId())  
                    cout << "TEST: " << nextToken.getId() << " consumed" << endl;
                else 
                    cout << endl << "No production found for top= " << top.getId() 
                    << " next token= " << nextToken.getId() << endl;
            }
            nextToken = scan.getNextToken();
            tokStack.push(nextToken);
            
        }
        if ((tokStack.empty()) && (nextToken.getId() == TOK_EOS) && (!scan.isError())) 
            cout << "ACCEPTED" << endl;
        else 
            cout << "REJECTED" << endl;  
    }
}

