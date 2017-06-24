# PID Tuning for Drving car in Track with CTE error as Feedback #

For finding the PID parameters(Kp,Kd,Ki)) to drive the car in the Track ,used a combination of Twiddle and manual fine tuning. I used Twiddle to get an approximate parameter and then fine tuned to get the final parameters.The final parameters that i used are 	`Kp = 0.1, Kd = 0.0, Ki = 2.08`. I am able to achieve this with a throttle of 30 mph.

  Twidle algorithm is explained by Sebasian in the course lesson.A sample can be found here .It automatically shifts the parameters either to increment or decrement and automatically tune the Gains of Proportional , Integral or Differential gains.

### Tuning the Parameters: ###
   The main challenge in implementing the twiddle algorithm one in the lesson and in practical is how we calculate the error and use it for the Twiddle function and second the twiddle function itself which is a loop over there where the control is given and robot position is rest within the loop.. In the original algorithm, the system run a few hundred loops to get the error before starting the twiddle function.However, in the actual implementation here , i calculated the error with the number of steps as the base and doing it in realtime along with twiddle.*(line 40,pid.cpp)*
    `if (cur_step > 2*n_steps){
		err = (pow(p_error,2))/n_steps;
	}`
   Initially i set all the PID gains at zero (main.cpp,line 16) `std::vector <double> params = {0.0,0,0}`.I set 1.0 to all the dparams values(main.cpp,line 18) `std::vector <double> dparams`. First i started with the proportional gain. I set number of steps to twiddle to 10000 `n_steps = 10000;` (pid.cpp,line 22) and to tune the proprtional gain, i set `p_idx = 0;` (pid.cpp,line 28).When i set dparams to 1.0, car was running randomly and pid  gain Kp-was showing much larger value.I reduced the dparams to 0.1 and again run the simulator. This time i got **Kp** around **0.108**.

  Next i started tuning the Differential Gain , keeping the Kp at 0.108.Twiddling gave an approximate of **Kd- 3.08**. 
  
  I tried to tune the Integral gain. Integral error is the sum of all the previous errors. Obviously the gain Ki should be low, as it will be multiplied with the large Integral error. I wasn't able to get any good Integral gain and later found that the car can drive on the track even with **Ki -Zero**.

   Kp the proportional gain,tries to induce oscillations , if Kp goes high, and the car will be continually oscillate and eventually veers out of the road at some turn. The Differential gain, Kd tries to offset the oscillation induced by the Proportional gain.I wasn't able to notice the effect of Ki as i kept it zero.

  After further manual tuning , i settled with the 	`Kp = 0.1, Kd = 0.0, Ki = 2.08`.
  
### Next Steps: ###

Implement a pid control for throttle too along with the steering. With the angle and the speed input we can run the car at high speed at the same time,taking care of the speed and angle at turns.
 