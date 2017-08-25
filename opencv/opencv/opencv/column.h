#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>

class column_window{
public:
	column_window(int column_num, int max_value, int width,int heigh, std::string wondow_name);
	~column_window();
	bool update_data(std::vector<double> datalist);

private:
	void show_framework();
	void show_column(int column_id, int percent);


	int column_num;
	int max_value;
	int width;
	int heigh;
	std::string window_name;

	int column_interval;
	int column_width;

	cv::Mat mat;
};

