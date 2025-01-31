#ifndef PID_v1_h
#define PID_v1_h
#define LIBRARY_VERSION	1.2.1

class PID
{
  public:

    // Constants used in some of the functions below
    #define AUTOMATIC	1
    #define MANUAL	0
    #define DIRECT  0
    #define REVERSE  1
    #define P_ON_M 0
    #define P_ON_E 1

    //commonly used functions **************************************************************************
    PID(double* const, double* const, double* const,        // * constructor.  links the PID to the Input, Output, and 
        const double, const double, const double,           //   Setpoint.  Initial tuning parameters are also set here.
        const int, const int);                              //   (overload for specifying proportional mode)

    PID(double* const, double* const, double* const,        // * constructor.  links the PID to the Input, Output, and 
        const double, const double, const double,           //   Setpoint.  Initial tuning parameters are also set here
        const int);

    void SetMode(const int Mode);                           // * sets PID to either Manual (0) or Auto (non-0)

    bool Compute();                                         // * performs the PID calculation.  it should be
                                                            //   called every time loop() cycles. ON/OFF and
                                                            //   calculation frequency can be set using SetMode
                                                            //   SetSampleTime respectively

    void SetOutputLimits(const double, const double);       // * clamps the output to a specific range. 0-255 by default, but
                                                            //   it's likely the user will want to change this depending on
                                                            //   the application


    //available but not commonly used functions ********************************************************
    void SetTunings(const double, const double,             // * While most users will set the tunings once in the 
                    const double);         	                //   constructor, this function gives the user the option
                                                            //   of changing tunings during runtime for Adaptive control
    void SetTunings(const double, const double,             // * overload for specifying proportional mode
                    const double, const int);         	  

    void SetControllerDirection(const int);	                // * Sets the Direction, or "Action" of the controller. DIRECT
                                                            //   means the output will increase when error is positive. REVERSE
                                                            //   means the opposite.  it's very unlikely that this will be needed
                                                            //   once it is set in the constructor.
    void SetSampleTime(const int);                          // * sets the frequency, in Milliseconds, with which 
                                                            //   the PID calculation is performed.  default is 100


    /* Status Funcions*************************************************************
    * Just because you set the Kp=-1 doesn't mean it actually happened.  these
    * functions query the internal state of the PID.  they're here for display
    * purposes.  this are the functions the PID Front-end uses for example
    ******************************************************************************/
    inline double GetKp() const { return dispKp; };                         // These functions query the pid for interal values.
    inline double GetKi() const { return dispKi; };                         //  they were created mainly for the pid front-end,
    inline double GetKd() const { return dispKd; };                         // where it's important to know what is actually 
    inline int GetMode() const { return inAuto ? AUTOMATIC : MANUAL; };     //  inside the PID.
    inline int GetDirection() const { return controllerDirection; };

  private:
    void Initialize();

    double dispKp;              // * we'll hold on to the tuning parameters in user-entered 
    double dispKi;              //   format for display purposes
    double dispKd;
    
    double kp;                  // * (P)roportional Tuning Parameter
    double ki;                  // * (I)ntegral Tuning Parameter
    double kd;                  // * (D)erivative Tuning Parameter

    int controllerDirection;
    int pOn;

    double *myInput;            // * Pointers to the Input, Output, and Setpoint variables
    double *myOutput;           //   This creates a hard link between the variables and the 
    double *mySetpoint;         //   PID, freeing the user from having to constantly tell us
                                //   what these values are.  with pointers we'll just know.

    unsigned long lastTime;
    double outputSum, lastInput;

    unsigned long SampleTime;
    double outMin, outMax;
    bool inAuto, pOnE;
};
#endif

