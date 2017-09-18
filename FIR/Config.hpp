#pragma once

#include<Siv3D.hpp>
#include<iostream>
#include<vector>
#include<unordered_map>
#include<memory>
#include<numeric>
#include<HamFramework.hpp>
#include <iomanip>

class Config{

private:

	GUI gui;

	Sound sound_base;
	Sound sound_fir;
	std::string sound_name;
	
	Wave wave_base;
	Wave wave_fir;

	std::vector<double>a;
	std::pair<bool, int>parameter;
	
	double resolution_fir;
	Font font;

	bool normalization;

public:

	Config();
	void update();

	void isReaction();

	void setText();
	std::pair<bool, int> calcArea(int, int);
	void setCoefficient(bool, int);
	
	void draw();
	
	void convolution();
	

};