#pragma once

#include "symbol.h"

const int SYMTBL_NUM_BUCKETS = 1000;

class symTbl {
public:
	symTbl();
	void      print();
	symTblRef search(string lex);
	symTblRef insert(string newlex);
private:
	symTblRef bucket[SYMTBL_NUM_BUCKETS];
	int hash(string s);
	
};