#ifndef PREPARE_DATA_H
#define PREPARE_DATA_H

#include "data_preprocessor.h"
#include "sketch.h"

class PrepareData {
public:
	void inputData(std::string filename, Sketch& sketch) {
		DataPreprocessor dp(filename);
		dp.readFile();
		for(int i = 0; i < dp.data_cnt; i++)
			sketch.insert(dp.data_pre[i]);
	}
	
	double setDiffApprox(std::string s1, std::string s2, int size, int l) {
		Sketch sa(size, l);
		Sketch sb(size, l);
		sb.sks = sa.sks;
		inputData(s1, sa);
		inputData(s2, sb);
		
		// ???
		for(int i = 0; i < 100000; ++i)
			sa.insert(i);

		printf("set sa size: %f\n", sa.size());
		printf("set sb size: %f\n", sb.size());

		sa.set_diff(sb);
		return sa.size();
	}

};
#endif
