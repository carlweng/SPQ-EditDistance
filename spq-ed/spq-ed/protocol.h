#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <iostream>
#include "sketch.h"
#include "emp-sh2pc/emp-sh2pc.h"
#include "prepare_data.h"

using namespace emp;

class Protocol {
	
public:
	int SIZE, MEDIAN, bitL;
	int party;
	NetIO *io;

	Protocol(int party, NetIO *io) {
		this->party = party;
		this->io = io;
	}

	void run(std::string filename) {
		this->SIZE = 2048;
		this->MEDIAN = 3;

		Sketch sketch(SIZE, MEDIAN);
				
		// transmit random integer of sketch.sys
		if(party == ALICE) {
			io->send_data((void*)sketch.sks, MEDIAN*sizeof(long));
		} else {
			io->recv_data((void*)sketch.sks, MEDIAN*sizeof(long));
		}

		// data compression (bucketing)
		PrepareData prepareData;
		prepareData.inputData(filename, sketch);

		if(party == BOB) {
			for(int j = 0; j < MEDIAN; ++j) {
				for(int i = 0; i < SIZE; ++i) {
					sketch.bs[j][i] *= -1;
				}
			}
			bitL = sketch.MaxInt() * 2;
			io->send_data((void*)(&bitL), sizeof(int));
		} else {
			io->recv_data((void*)(&bitL), sizeof(int));
		}

		// input data for secure computation
		Integer aliceBF[MEDIAN][SIZE];
		Integer bobBF[MEDIAN][SIZE];
		for(int i = 0; i < MEDIAN; i++) {
			for(int j = 0; j < SIZE; j++) {
				aliceBF[i][j] = Integer(32, sketch.bs[i][j], ALICE);
				bobBF[i][j] = Integer(32, sketch.bs[i][j], BOB);
			}
		}

		// computation
		Integer med[MEDIAN];
		for(int i = 0; i < MEDIAN; i++) {
			med[i] = Integer(32, 0, PUBLIC);
			for(int j = 0; j < SIZE; j++) {
				aliceBF[i][j] = aliceBF[i][j] + bobBF[i][j];
				aliceBF[i][j] = aliceBF[i][j] * aliceBF[i][j];
			}
			
			int count = SIZE;
			while(count > 1) {
				for(int j = 0; j < count/2; j++)
					aliceBF[i][j] = aliceBF[i][2*j] + aliceBF[i][2*j+1];
				if((count%2) == 1) {
					aliceBF[i][count/2] = aliceBF[i][count-1];
					count = count / 2 + 1;
				} else count = count / 2;
			}

			med[i] = aliceBF[i][0];
		}
		
		Integer res;
		findMedian3(med, res);
		
		int approx = res.reveal<int>(BOB);
		if(party == BOB) std::cout << "result: " << approx << std::endl;

	}

	void findMedian3(Integer med[], Integer& res) {
		Bit d01 = med[0].geq(med[1]);
		Bit d02 = med[0].geq(med[2]);
		Bit d0 = d01 ^ d02;

		//Bit d10 = !d01;
		//Bit d12 = med[1].geq(med[2]);
		//Bit d1 = d10 ^ d12;

		Bit d20 = med[2].geq(med[0]);
		Bit d21 = med[2].geq(med[1]);
		Bit d2 = d20 ^ d21;

		res = med[1].select(d0, med[0]);
		res = res.select(d2, med[2]);
	}
};
#endif
