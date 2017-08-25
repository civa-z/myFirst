#include "column.h"


column_window::column_window(int column_num, int max_value, int width, int heigh, std::string window_name):
		column_num(column_num), max_value(max_value), width(width), heigh(heigh), window_name(window_name){
	int column_base = width / (column_num * 3 + 1);
	this->column_interval = column_base;
	this->column_width = column_base * 2;
	this->mat = cv::Mat::zeros(cv::Size(width, heigh), CV_8UC3);
	cv::imshow(this->window_name, this->mat);
}

column_window::~column_window(){
}


bool column_window::update_data(std::vector<double> datalist){
	return false;
}

void column_window::show_framework(){

}