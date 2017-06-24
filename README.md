#**PID Tuning for Driving car in Track with CTE error as Feedback**
##Writeup for Project Submission

For finding the PID parameters(Kp,Kd,Ki)) to drive the car in the Track ,used a combination of Twiddle and manual fine tuning. 

CTE - Cross Track error. How much the vehicle veers away from the centre of the road.

I used Twiddle to get an approximate parameter and then fine tuned to get the final parameters.The final parameters that i used are 	`Kp = 0.108, Kd = 0.001, Ki = 2.08`. I am able to achieve this with a throttle of 30 mph.


[//]: # (Vide0 References)

[video1]: ./Videos/PID_PAlone_P_1.0.wmv "P Alone01"
[video2]: ./Videos/PID_PAlone_P0.3.wmv "P Alone03"
[video3]: ./Videos/fPID_0.3_0_3.08.wmv "PD"
[video4]: ./Videos/PI_0.1_I_0.001_0.wmv  "PI"
[video5]: ./Videos/PID_0.1_0_2.08.wmv "PID without I"
[video6]: ./Videos/PID_Final_0.1_0.001_2.08_V2.wmv "Final PID"

### Effect of each of the P,I,D components on the system: ###
 As mentioned in previous section, Proportional Gain Kp is tuned first.
As we know, the system will begin to oscillate and after a time, the system wont settle down and the Car will begin to Wobble a lot and move away from the path 
#### Proprtional Effect ####
   

1. [Kp = 0.1,Ki =0;Ki =0](https://github.com/mniju/CarND-PID-Control-Project/blob/master/Videos/PID_PAlone_P_0.1.wmv)
      

2. [Kp = 0.3,Ki =0;Ki =0](https://github.com/mniju/CarND-PID-Control-Project/blob/master/Videos/PID_PAlone_P0.3.wmv)


As seen from the videos, the P gain tries to bring the system in control but it continues to oscillates and then overshoots. The systems isn't stable after any steep change in value such as corners.

#### Differential Effect ####

 **The Diffrential gain tries to dampen the oscillations created by the Proportional component and brings the system in operable limits without overshoots**.After adding the Integral component, we can still see the action of P trying to oscillate . However the Integral components brings the system to stable condition and doesnt allow to over shoot. The system is almost stable.
 Kp = 0.308, Ki =0, Kd = 3.08 . The car is able to drive without going outside the road.

[Kp = 0.308, Ki =0, Kd = 3.08](https://github.com/mniju/CarND-PID-Control-Project/blob/master/Videos/PID_0.3_0_3.08.wmv)


   
#### Integral Effect :
PI Controller is one of the mostly used controller in the industry may be after pid control. **The Inegral error is the sum of the error over a period of time.It is used to offset  the bias added over a period of time in the system.**However, in this system i was not able to make the system work with the PI control alone.WE could see that the system starts to oscillate and since there is no differential gain to dampen the system the system overshoots and moves away from the path.

[PI-Alone-Kp=0.1,Ki=0.001](https://github.com/mniju/CarND-PID-Control-Project/blob/master/Videos/PI_0.1_I_0.001_0%20.wmv)

However The effect of 'I' can be little felt in the complete PID control.When I  component is there, the car moves to the centre bit quickly after a turn .May be this helps the car to be always in track even in turns.This is the only Integral effect. Integral component effect is not much pronunced in this system.  

1. [Without I component](https://github.com/mniju/CarND-PID-Control-Project/blob/master/Videos/PID_0.1_0_2.08.wmv)
2. [With I Component](https://github.com/mniju/CarND-PID-Control-Project/blob/master/Videos/PID_Final_0.1_0.001_2.08_V2.wmv)


### How Hyper parameters Chosen: ###
   I used a combination of Twiddle and Manual Tuning.
   
  Twiddle algorithm explained [here](https://www.youtube.com/watch?v=2uQ2BSzDvXs)   automatically shifts the parameters either to increment or decrement and tunes the Gains of Proportional , Integral or Differential gains.

   The main challenge in implementing the twiddle algorithm one in the lesson and in practical is how we calculate the error and use it for the Twiddle function and second the twiddle function itself which is a loop over there where the control is given and robot position is rest within the loop.. In the original algorithm, the system run a few hundred loops to get the error before starting the twiddle function.However, in the actual implementation here , i calculated the error with the number of steps as the base and doing it in realtime along with twiddle.*(line 40,pid.cpp)*
    `if (cur_step > 2*n_steps){
		err = (pow(p_error,2))/n_steps;
	}`
   Initially i set all the PID gains at zero *(main.cpp,line 16)* `std::vector <double> params = {0.0,0,0}`.I set 1.0 to all the dparams values(main.cpp,line 18) `std::vector <double> dparams`. First i started with the proportional gain. I set number of steps to twiddle to 10000 `n_steps = 10000;` *(pid.cpp,line 22)* and to tune the proprtional gain, i set `p_idx = 0;` *(pid.cpp,line 28)*.When i set dparams to 1.0, car was running randomly and pid  gain Kp-was showing much larger value.I reduced the dparams to 0.1 and again run the simulator. This time i got **Kp** around **0.108**.

  Next i started tuning the Differential Gain , keeping the Kp at 0.108.Twiddling gave an approximate of **Kd= 3.08**. 
  
  I tried to tune the Integral gain. Integral error is the sum of all the previous errors. Obviously the gain Ki should be low, as it will be multiplied with the large Integral error. I wasn't able to get any good Integral gain and later found that the car can drive on the track even with **Ki -Zero**.

   Kp the proportional gain,tries to induce oscillations , if Kp goes high, and the car will be continually oscillate and eventually veers out of the road at some turn. The Differential gain, Kd tries to offset/dampen the oscillation induced by the Proportional gain.

  After further manual tuning , i settled with the 	`Kp = 0.108, Kd = 0.001, Ki = 2.08`.

[Final PID Run With All Components](https://github.com/mniju/CarND-PID-Control-Project/blob/master/Videos/PID_Final_0.1_0.001_2.08_V2.wmv)

### Next Steps: ###

Implement a pid control for throttle too along with the steering. With the angle and the speed input we can run the car at high speed at the same time,taking care of the speed and angle at turns.
 