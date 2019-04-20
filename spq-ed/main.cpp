#include <iostream>
#include "protocol.h"
//#include "data_preprocessor.h"
//#include "sketch.h"

int main(int argc, char** argv) {
	int port, party;
	parse_party_and_port(argv, &party, &port);
	NetIO * io = new NetIO(party==ALICE ? nullptr : "127.0.0.1", port);
	setup_semi_honest(io, party);

/*	
	DataPreprocessor dp("hu60_test.vcf");
	dp.readFile();

	Sketch sketch(3, 4);
	sketch.insert(1);
*/
	Protocol protocol(party, io);
	if(party == ALICE)
		protocol.run("data/hu661AD0.vcf");
	else protocol.run("data/hu604D39.vcf");
	
	return 0;
}
