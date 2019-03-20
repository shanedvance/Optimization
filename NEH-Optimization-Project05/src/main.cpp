#include <iostream>
#include "FSS.h"
#include "FSSB.h"
#include "FSSNW.h"
#include "NEH.h"

using namespace std;

int main(int argc, char **argv) {

	if (argc > 2) {
		perror("Too many arguments");
		exit(1);
	}
	if (argc < 2) {
		perror("Too few arguments");
		exit(1);
	}

	//! Initialization of the FSS
    cFSS* FSS = new cFSS(argv[1]);
    //! Initialization of the FSS with blocking
    cFSSB* FSSB = new cFSSB(argv[1]);
    //! Initialization of the FSS with no wait
    cFSSNW* FSSNW = new cFSSNW(argv[1]);
	//! Initialization of the NEH heuristic algorithm
	NEH *neh = new NEH();
	
    //! Calculate a simple schedule for flowshop
	neh->FSSNEH(FSS);
    //! Calculate a simple schedule for flowshop with blocking
    neh->FSSBNEH(FSSB);
    //! Calculate a simple schedule for flowshop with no wait
    neh->FSSNWNEH(FSSNW);

    return 0;
}