#ifndef SKETCH_H
#define SKETCH_H

#include <cstdlib>
#include <cmath>
#include <random>
#include <algorithm>
#include "hasher.h"

class Sketch {
public:
	int l, k;
	int **bs;	// k*l matrix
	long *sks;	// k random integer

	Sketch(int m, int l) {
		this->l = m;
		this->k = l;
		bs = (int**)malloc(l*sizeof(int*));
		for(int i = 0; i < l; i++)
			bs[i] = (int*)malloc(m*sizeof(int));;
		sks = (long*)malloc(l*sizeof(long));;
		
		// PRG
		std::random_device rd;
		std::uniform_int_distribution<long> dist;
		for(int i = 0; i < l; ++i)
			sks[i] = dist(rd);
	}

	void test_rnd() {
		for(int i = 0; i < l; i++)
			printf("%ld\n", sks[i]);
	}

	// insert binary hash value in a filter
	void insert(long s) {
		int i = k;	
		Hasher hasher;
		while(i-- > 0) {
			long h = hasher.fasthash64(s, sks[i]);
			int pos = (int)((h & 0xFFFFFF) % l);
			bs[i][pos] += (((h>>29)&1) == 1) ? 1:-1;
		}
	}

	double size() {
		double res[k];
		for(int i = 0; i < k; ++i) {
			for(int j = 0; j < l; ++j) {
				res[i] += ((bs[i][j]*bs[i][j]));
			}
		}
		std::sort(res, res+k);
		return res[k/2];
	}

	void set_diff(Sketch a) {
		for(int j = 0; j < k; ++j)
			for(int i = 0; i < l; ++i) 
				bs[j][i]-=a.bs[j][i];
	}

	int MaxInt() {
		int res = -1;
		for(int i = 0; i < k; ++i) {
			for(int j = 0; j < l; ++j) {
				int comp = std::abs(bs[i][j]);
				res = res>comp?res:comp;
			}
		}
		return (int)(std::ceil(std::log(res) / std::log(2)));
	}
};
#endif
