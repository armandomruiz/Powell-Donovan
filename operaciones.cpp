#include "iostream"
#include "vector"
#include "operaciones.h"
#include "math.h"

float operacion(std::string funcion, std::vector<float>& params) {

	float r = 0;
	if(funcion == "+" || funcion == "*") {
		if(funcion == "+") {
			
			r = params[0]+ params[1];
			//return r;
		}
		else if(funcion == "*") {
			r = params[0]*params[1];
		}
	return r;
	}
	else if(funcion == "-") {
			r = params[0] - params[1];
	}
	else if(funcion == "sin"){
			r = sin(params[0]);
	}
	else if(funcion == "cos"){
			r = cos(params[0]);
	}
	return r;

}
