#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>

class MouseAction{
public:
    void movemouse(int x, int y){
        std::string command = "xdotool mousemove " + std::to_string(x) + " " + std::to_string(y);
        system(command.c_str());
    }

	void mouseleftbuttonclick(){
		system("xdotool click 1");
	}
};

