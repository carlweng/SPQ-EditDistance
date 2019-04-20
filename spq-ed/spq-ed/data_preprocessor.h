#ifndef DATA_PREPROCESSOR_H
#define DATA_PREPROCESSOR_H

#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <cstring>

//#include "MessageDigest.h"
#include <openssl/md5.h>

class DataPreprocessor {
	std::string filename;

public:	
	long *data_pre;
	int data_cnt;

	DataPreprocessor(std::string filename) {
		this->filename = filename;
		data_cnt = 0;
		data_pre = new long[150000];
	}

	std::vector<std::string> strSeparator(std::string line) {
		std::vector<std::string> set;
		int front = 0, end = 0;
		end = line.find("\t");
		set.push_back(line.substr(front, end-front));
		front = end + 1;
		while(true) {
			end = line.find("\t", front);
			if(end < 0)
				break;
			set.push_back(line.substr(front, end-front));
			front = end + 1;
		}
		set.push_back(line.substr(front, line.length() - front));
		return set;
	}

	int op2Int(std::string op_str) {
		if(op_str.compare("SUB") == 0 || op_str.compare("SNP") == 0) return 0;
		else if(op_str.compare("DEL") == 0) return 1;
		else if(op_str.compare("INS") == 0) return 2;
		else {std::cout << "unknow operation" << std::endl;abort();} 
	}

	int allel2Int(unsigned char allel) {
		if(allel == 'A') return 0;
		else if(allel == 'T') return 1;
		else if(allel == 'C') return 2;
		else if(allel == 'G') return 3;
		else {std::cout << "unknow allel" << std::endl;abort();}
	}
	
	void getPos(long pos, std::string allel, int op, long *res, int l) {
		const char* al_chr = allel.c_str();
		for(int i = 0; i < l; ++i) {
			res[i] = 0;
			res[i] = op;
			res[i] <<= 2;
			res[i] = res[i] | allel2Int(al_chr[i]);
			res[i] <<= 40;
			res[i] = res[i] | (pos + i);
		}
	}

	void readFile() {
		//MessageDigest md;
		std::ifstream infile(filename);
		std::string line;
		while(std::getline(infile, line)) {
			if(line[0] == '#')
				continue;

			std::vector<std::string> set = strSeparator(line);
			long pos = std::stol(set[1]);
			std::string ref = set[3];
			std::string alt = set[4];
			std::string op_str = set[7].substr(7, 3);

			int op = op2Int(op_str);
			int l = 0;

			long *res;
			if(op == 1) {
				l = ref.length();
				res = (long*)malloc(l*sizeof(long)+1);
				getPos(pos, ref, op, res, l);
			} else {
				l = alt.length();
				res = (long*)malloc(l*sizeof(long)+1);
				getPos(pos, alt, op, res, l);
			}

			for(int i = 0; i < l; i++) {
				unsigned char* msg = (unsigned char*)calloc(32, sizeof(unsigned char));
				memcpy(msg, (unsigned char*)(&res[i]), sizeof(long));

				unsigned char dig[32];
				//unsigned int dig_len = 16;
				//md.digest_message(msg, 16, (unsigned char**)&dig, &dig_len);
				MD5(msg, sizeof(long), dig);
				unsigned long *dig_long = (unsigned long*)dig;
				data_pre[data_cnt++] = *dig_long;
				//printf("%lu %lu\n", res[i], *dig_long);
				free(msg);
			}

			free(res);
		}

		infile.close();
	}
};
#endif
