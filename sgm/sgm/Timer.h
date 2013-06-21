#pragma once
#include <string>   
#include <iostream> 
#include <iomanip>  

using namespace std;

class Timer {
public:
	Timer(){
		last = 0;
		on = 0;
	}
	void start(std::string s){
		mess = s;
		last = clock();	
	}
	void finish(){
		clock_t now = clock();
		if(last != 0){
			long diff = ((now - last) * 1000) / CLOCKS_PER_SEC;
			if(on)
				std::cout << mess << " in " << diff << " millis\n";
		}
	}
	void changeState(bool on){
		this->on = on;
	}
private:
	string mess;
	clock_t last;
	bool on;
};

Timer timer;

