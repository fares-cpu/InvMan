#include<iostream>
#include<dbb.h>
#include<pqxx\pqxx>
#include<string>


void handle_output(int iterator);
void handle_input(const char& input){}

void app(const char& input) {
	for (int i = 0; input != 3; i++) {
		handle_input(input);
		handle_output(i);
	}
}

void handle_output(int iterator)
{
	if (iterator == 0) {
		std::cout << "Surprize, Motherfucker!";
	}
}
