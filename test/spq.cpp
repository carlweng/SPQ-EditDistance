#include <iostream>
#include "emp-sh2pc/emp-sh2pc.h"
#include "protocol.h"
//#include "data_preprocessor.h"
//#include "sketch.h"
using namespace emp;
using namespace std;

int main(int argc, char** argv) {
	PRG prg;
	int port, party;
	parse_party_and_port(argv, &party, &port);
	NetIO * io = new NetIO(party==ALICE ? nullptr : "127.0.0.1", port);
	setup_semi_honest(io, party);	

	Protocol protocol(party, io);
	if(party == ALICE) {
		protocol.run("spq-ed/data/hu661AD0.vcf");
		//protocol.run("spq-ed/data/hu60_test.vcf");
	}
	else { 
		protocol.run("spq-ed/data/hu604D39.vcf");
		//protocol.run("spq-ed/data/hu661AD0.vcf");
	}
	
	return 0;
}
