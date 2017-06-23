#include "PID.h"
#include <limits>
#include <cmath>
#include <iostream>
using namespace std;

/*
* TODO: Complete the PID class.
*/

PID::PID() {}

PID::~PID() {}

void PID::Init(double Kp, double Ki, double Kd) {
	this ->Kp = Kp;
	this ->Ki = Ki;
	this ->Kd = Kd;
	p_error = 0;
	i_error = 0;
	d_error = 0;
	n_steps = 10000;
	best_error = std::numeric_limits<double>::max();
	err = 0;
	next_state = "start";
	cur_step = 0;
	twiddle = false;
	p_idx = 0;
}

void PID::UpdateError(double cte) {
   
	i_error += cte; // Accumulate the error for integration
	// Previous error is the cte from prev iteration (ie p_error!!!)
	d_error = cte - p_error; // Take the difference of the error for differentation
	p_error = cte;
}

double PID::TotalError() {
	if (cur_step > 2*n_steps){
		err = (pow(p_error,2))/n_steps;// p_error is cte - From UpdateError Function;err = cte^2 /number_of_steps
		
	}
	//else err = p_error/n_steps;
	return err;
}

void PID::Twiddle(vector <double> &params,vector <double> &dparams){
	
	if (twiddle == true && cur_step <= 2* n_steps){
		//std::cout <<"next_state:"<<next_state<<std::endl;
		if (next_state == "start"){
				params[p_idx] += dparams[p_idx];
		        //Set the steering and Get error
		        next_state = "errchk1";
		}
		else if (next_state == "errchk1"){//Increase
			if (err < best_error){
				best_error = err;
				dparams[p_idx] *= 1.1;
				params[p_idx] += dparams[p_idx];
				next_state = "errchk1";
				}
				else{//Decrease
					params[p_idx] -= 2*dparams[p_idx];
					next_state = "errchk2";
				}
		}
		else if (next_state == "errchk2"){
			if (err < best_error){
				best_error = err;
				dparams[p_idx] *= 1.1;
				params[p_idx] += dparams[p_idx];
				next_state = "errchk1";
			}
			else{
				params[p_idx] += dparams[p_idx];
				dparams[p_idx] *= 0.9;
				params[p_idx] += dparams[p_idx];
				next_state = "errchk2";
			}
		}
	cur_step += 1;
	Kp = params[0];
	Ki = params[1];
	Kd = params[2];
	//std::cout<<"cur_step:"<<cur_step<<std::endl;
	}

}
	
	
	
	
