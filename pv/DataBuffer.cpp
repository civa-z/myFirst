#include "DataBuffer.hpp"
#include <thread>

int main(){
	DataBuffer databuffer(8);
	std::thread producer([&]()
	{
		std::vector<double> dataset;
		dataset.push_back(1);
		while(1){
			std::this_thread::sleep_for (std::chrono::milliseconds (100));
			databuffer.produce(dataset);
			std::cout<<"producer Thread+++"<<std::endl;
		}
	});

	std::thread consumer([&]()
	{
		std::vector<std::vector<double>> output;
		while(1){
			databuffer.consume(output);
			std::cout<<"consumer Thread-----"<<std::endl;
		}
	});

	producer.join();
	consumer.join();
	std::cout<<"Main Thread"<<std::endl;

	return 0;
}
