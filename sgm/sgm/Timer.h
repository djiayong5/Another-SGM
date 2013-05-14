#pragma once
#include <string>   
#include <iostream> 
#include <iomanip>  

using namespace std;

class Timer {
public:
	Timer(){
		last = 0;
	}
	void start(std::string s){
		mess = s;
		last = clock();	
	}
	void finish(){
		clock_t now = clock();
		if(last != 0){
			long diff = ((now - last) * 1000) / CLOCKS_PER_SEC;
			std::cout << mess << " in " << diff << " millis\n";
		}
	}
private:
	string mess;
	clock_t last;
};

Timer timer;

