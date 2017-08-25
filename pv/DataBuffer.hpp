#include <iostream>
#include <mutex>
#include <condition_variable>
#include <vector>

class DataBuffer{
public:
	DataBuffer(int size):size(size){}
	~DataBuffer(){}
	void produce(std::vector<double> dataset){
		std::unique_lock<std::mutex> locker(m_mutex);
		if (databuf.size() == size){
			std::cout<<"DataBuffer wait for data buffer clear";
			empty.wait(locker);
		}
		//produce start
		databuf.push_back(dataset);
		//produce end

		if(databuf.size() == size){
			full.notify_one();
		}
	}

	void consume(std::vector<std::vector<double>> &output){
		std::unique_lock<std::mutex> locker(m_mutex);
		if (databuf.size() < size){
			full.wait(locker);
		}

		//consume begin
		output.insert(output.begin(), databuf.begin(), databuf.end());
		databuf.clear();
		//consume end

		empty.notify_one();
	}

	void reset(){
		std::unique_lock<std::mutex> locker(m_mutex);
		databuf.clear();
	}


private:
	std::mutex m_mutex;
	std::condition_variable empty, full;

	const unsigned int size;
	std::vector<std::vector<double>> databuf;
};


