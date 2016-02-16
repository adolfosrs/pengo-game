//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

#ifndef TIMER_H
#define TIMER_H


#include <time.h>


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
class CTimer {

//-----------------------------------------------------------------------------
protected:

  /// time since initialization
  double m_fTimeSeconds;

  /// time since last update in seconds
  double m_fDeltaTime;

  /// last clock
  clock_t m_nLastClock;

//-----------------------------------------------------------------------------
public:

  /// constructor
  CTimer();

  /// destructor
  ~CTimer() {}

  /// initialize the clock
  bool Init();

  /// update the clock - called once per frame
  bool Update();

  /// return the delta time in seconds
  double GetDeltaTimeSeconds() { return m_fDeltaTime; }

  /// return the time passed since initialization in seconds
  double GetTimeSeconds() { return m_fTimeSeconds; }


};


#endif //TIMER_H
