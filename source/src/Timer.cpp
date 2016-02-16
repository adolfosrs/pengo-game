//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------


#include "Timer.h"





//-----------------------------------------------------------------------------
// constructor
//-----------------------------------------------------------------------------
CTimer::CTimer() {

  m_fTimeSeconds = 0.0;
  m_fDeltaTime = 0.0;
  m_nLastClock = 0;
}


//-----------------------------------------------------------------------------
// initialize the clock
//-----------------------------------------------------------------------------
bool CTimer::Init() {

  m_nLastClock = clock();
  m_fTimeSeconds = 0.0;
  m_fDeltaTime = 0.0;

  return true;
}


//-----------------------------------------------------------------------------
// update the clock - called once per frame
//-----------------------------------------------------------------------------
bool CTimer::Update() {

  //aux vars
  clock_t nCurrentTime;

  nCurrentTime = clock();

  m_fDeltaTime = (double)(nCurrentTime - m_nLastClock) / (double)CLOCKS_PER_SEC;
  m_fTimeSeconds += m_fDeltaTime;

  m_nLastClock = nCurrentTime;

  // check for a NAN error
  if (m_fTimeSeconds != m_fTimeSeconds)
    return false;

  return true;
}
