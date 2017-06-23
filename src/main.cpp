#include <uWS/uWS.h>
#include <iostream>
#include "json.hpp"
#include "PID.h"
#include <math.h>


// for convenience
using json = nlohmann::json;

// For converting back and forth between radians and degrees.
constexpr double pi() { return M_PI; }
double deg2rad(double x) { return x * pi() / 180; }
double rad2deg(double x) { return x * 180 / pi(); }
//PID Values
  std::vector <double> params = {0.1,0,2.08};//Kp,Ki,Kd - 0,0.0002,3.0
//0.308,0,2.08 - > Noisy but okay
  //std::vector <double> dparams = {0.01,0.05,0.1}; //Starting Prob Parameters -used for Twiddle

// Checks if the SocketIO event has JSON data.
// If there is data the JSON object in string format will be returned,
// else the empty string "" will be returned.
std::string hasData(std::string s) {
  auto found_null = s.find("null");
  auto b1 = s.find_first_of("[");
  auto b2 = s.find_last_of("]");
  if (found_null != std::string::npos) {
    return "";
  }
  else if (b1 != std::string::npos && b2 != std::string::npos) {
    return s.substr(b1, b2 - b1 + 1);
  }
  return "";
}

int main()
{
  uWS::Hub h;

  PID pid;
//Initialize the PID variable  
  pid.Init(params[0], params[1], params[2]);
  //std::cout << "PID Vals Top:"<<pid.Kp<<" ,"<<pid.Ki<<","<<pid.Kd<<std::endl;
  //std::cout << "PID params Top:"<<params[0]<<" ,"<<params[1]<<","<<params[2]<<std::endl;


  h.onMessage([&pid](uWS::WebSocket<uWS::SERVER> ws, char *data, size_t length, uWS::OpCode opCode) {
    // "42" at the start of the message means there's a websocket message event.
    // The 4 signifies a websocket message
    // The 2 signifies a websocket event
    if (length && length > 2 && data[0] == '4' && data[1] == '2')
    {
      auto s = hasData(std::string(data).substr(0, length));
      if (s != "") {
        auto j = json::parse(s);
        std::string event = j[0].get<std::string>();
        if (event == "telemetry") {
          // j[1] is the data JSON object
          double cte = std::stod(j[1]["cte"].get<std::string>());
          double speed = std::stod(j[1]["speed"].get<std::string>());
          double angle = std::stod(j[1]["steering_angle"].get<std::string>());
          double steer_value;
          /*
          * TODO: Calcuate steering value here, remember the steering value is
          * [-1, 1].
          * NOTE: Feel free to play around with the throttle and speed. Maybe use
          * another PID controller to control the speed!
          */
		   
		  pid.UpdateError(cte);//Update the PID errors first
          
		  /* Used in Twiddle
      double err = pid.TotalError();
		  pid.Twiddle(params,dparams);
      */
		  
      double p_error = pid.p_error;		  
		  double i_error = pid.i_error;	
		  double d_error = pid.d_error;
		  
		  double Kp = pid.Kp;
		  double Ki = pid.Ki;
		  double Kd = pid.Kd;
          //std::cout << "PID params:"<<params[0]<<" ,"<<params[1]<<","<<params[2]<<std::endl;
          //std::cout << "PID Vals:"<<Kp<<" ,"<<Ki<<","<<Kd<<std::endl;
          
         // std::cout <<"Errors:"<< p_error <<" , "<< i_error<<" , "<< d_error << std::endl;
		  steer_value = (-Kp * p_error) - (Kd * d_error) - (Ki * i_error);
      ///std::cout << "Raw Steer" << steer_value <<std::endl;
          //Steering value is between [1,-1]
		  steer_value = std::max(std::min(steer_value,1.0),-1.0);

          // DEBUG
          std::cout << "CTE: " << cte << " Steering Value: " << steer_value << std::endl;

          json msgJson;
          msgJson["steering_angle"] = steer_value;
          msgJson["throttle"] = 0.3;
          auto msg = "42[\"steer\"," + msgJson.dump() + "]";
          std::cout << msg << std::endl;
          ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
		  
        }
      } else {
        // Manual driving
        std::string msg = "42[\"manual\",{}]";
        ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
      }
    }
  });

  // We don't need this since we're not using HTTP but if it's removed the program
  // doesn't compile :-(
  h.onHttpRequest([](uWS::HttpResponse *res, uWS::HttpRequest req, char *data, size_t, size_t) {
    const std::string s = "<h1>Hello world!</h1>";
    if (req.getUrl().valueLength == 1)
    {
      res->end(s.data(), s.length());
    }
    else
    {
      // i guess this should be done more gracefully?
      res->end(nullptr, 0);
    }
  });

  h.onConnection([&h](uWS::WebSocket<uWS::SERVER> ws, uWS::HttpRequest req) {
    std::cout << "Connected!!!" << std::endl;
  });

  h.onDisconnection([&h](uWS::WebSocket<uWS::SERVER> ws, int code, char *message, size_t length) {
    ws.close();
    std::cout << "Disconnected" << std::endl;
  });

  int port = 4567;
  if (h.listen(port))
  {
    std::cout << "Listening to port " << port << std::endl;
  }
  else
  {
    std::cerr << "Failed to listen to port" << std::endl;
    return -1;
  }
  h.run();
}
