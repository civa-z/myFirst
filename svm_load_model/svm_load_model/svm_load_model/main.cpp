#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

#include "svm.h"





typedef struct SVMData
{
    double label;
    std::vector<std::pair<int, double>> nodes;
} SVMData;


svm_node* AssignSVMNode(const SVMData svmData)
{
	int noneZeroFeatureNum = svmData.nodes.size();
	svm_node* node = new svm_node[noneZeroFeatureNum + 1];

    for (unsigned int index=0; index < svmData.nodes.size(); index++)
    {
        if (svmData.nodes[index].first != 0)
        {
            node[index].index = svmData.nodes[index].first;
            node[index].value = svmData.nodes[index].second;
        }
    }
	node[noneZeroFeatureNum].index = -1;
    node[noneZeroFeatureNum].value = 0;

    return node;
}

bool analyze_features(const std::string line, SVMData &svmData){
	std::istringstream iss(line);
	

	std::string feature;
	int index;
	double value;

	iss>>svmData.label;
	do{
		iss>>feature;
		if (iss.fail())
			return false;

		if (2 != sscanf(feature.c_str(), "%d:%lf", &index, &value))
			return false;
		svmData.nodes.push_back(std::pair<int, double>(index, value));
	}while(!iss.eof());

	return true;
}

std::vector<SVMData> read_data(std::string file_name){
	std::vector<SVMData> output;

	std::ifstream f_in(file_name, std::ios_base::in);
	if(!f_in.good())
		return output;

	char line[256];
	while(!f_in.eof() && f_in.getline(line, 256)){
		SVMData svmData;
		if (analyze_features(line, svmData))
			output.push_back(svmData);
	}

	return output;
}


int main(){
	auto test_data = read_data("1_3");
	svm_model* model = svm_load_model("1_3_b_model");
	int nr_class = svm_get_nr_class(model);

	for (auto svmData : test_data){
		svm_node* nodes = AssignSVMNode(svmData);
		double* probEstimates = new double[nr_class];
		svm_predict_probability(model, nodes, probEstimates);
		for (int i = 0; i < nr_class; ++i){
			std::cout<<i<<":"<<probEstimates[i]<<"  ";
		}
		std::cout<<std::endl;
	}

	getchar();

	return 0;
}