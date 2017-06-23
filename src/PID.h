#ifndef PID_H
#define PID_H

#include <string>
#include <vector>

using std::string;
using std::vector;

class PID {
public:
  /*
  * Errors
  */
  double p_error;
  double i_error;
  double d_error;

  /*
  * Coefficients
  */ 
  double Kp;
  double Ki;
  double Kd;
  /*
  *Other Varaibles
  */
  int n_steps;
  int cur_step;
  double best_error;
  double err;
  int p_idx; // Index of tuning parameter
  bool twiddle; // Should Twiddle run and train
  string next_state; // Next Step Name -used in Twiddle

  /*
  * Constructor
  */
  PID();

  /*
  * Destructor.
  */
  virtual ~PID();

  /*
  * Initialize PID.
  */
  void Init(double Kp, double Ki, double Kd);

  /*
  * Update the PID error variables given cross track error.
  */
  void UpdateError(double cte);

  /*
  * Calculate the total PID error.
  */
  double TotalError();
  
  /*
  *Find Kp,Ki and Kd using Twiddle
  */
  void Twiddle(vector <double> &params,vector <double> &dparams);
  
 
};


#endif /* PID_H */
