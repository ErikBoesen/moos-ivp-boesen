/************************************************************/
/*    NAME: Erik Boesen                                              */
/*    ORGN: MIT                                             */
/*    FILE: BHV_ZigLeg.cpp                                    */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include <cstdlib>
#include <ZAIC_PEAK.h>
#include "MBUtils.h"
#include "BuildUtils.h"
#include "BHV_ZigLeg.h"
#include "XYRangePulse.h"

using namespace std;

//---------------------------------------------------------------
// Constructor

BHV_ZigLeg::BHV_ZigLeg(IvPDomain domain) :
  IvPBehavior(domain)
{
  // Provide a default behavior name
  IvPBehavior::setParam("name", "defaultname");

  // Declare the behavior decision space
  m_domain = subDomain(m_domain, "course,speed");

  // Add any variables this behavior needs to subscribe for
  addInfoVars("NAV_X, NAV_Y");
  addInfoVars("WPT_INDEX", "no_warning");
  addInfoVars("DESIRED_HEADING", "no_warning");

  /*m_pulse_radius = 10;
  m_pulse_duration = 50;
  m_waiting = false;
  m_end_time = 0;*/
  m_waypoint_index = 0;

  m_zig_duration = 10;
  m_zig_angle = 45;
}

//---------------------------------------------------------------
// Procedure: setParam()

bool BHV_ZigLeg::setParam(string param, string val)
{
  // Convert the parameter to lower case for more general matching
  param = tolower(param);

  // Get the numerical value of the param argument for convenience once
  double double_val = atof(val.c_str());

  if((param == "zig_duration") && isNumber(val)) {
      m_zig_duration = double_val;
      return(true);
  }
  else if (param == "zig_angle") {
      m_zig_angle = double_val;
      return(true);
  }

  // If parameter not handled above
  return(false);
}

//---------------------------------------------------------------
// Procedure: onSetParamComplete()
//   Purpose: Invoked once after all parameters have been handled.
//            Good place to ensure all required params have are set.
//            Or any inter-param relationships like a<b.

void BHV_ZigLeg::onSetParamComplete()
{
}

//---------------------------------------------------------------
// Procedure: onHelmStart()
//   Purpose: Invoked once upon helm start, even if this behavior
//            is a template and not spawned at startup

void BHV_ZigLeg::onHelmStart()
{
}

//---------------------------------------------------------------
// Procedure: onIdleState()
//   Purpose: Invoked on each helm iteration if conditions not met.

void BHV_ZigLeg::onIdleState()
{
}

//---------------------------------------------------------------
// Procedure: onCompleteState()

void BHV_ZigLeg::onCompleteState()
{
}

//---------------------------------------------------------------
// Procedure: postConfigStatus()
//   Purpose: Invoked each time a param is dynamically changed

void BHV_ZigLeg::postConfigStatus()
{
}

//---------------------------------------------------------------
// Procedure: onIdleToRunState()
//   Purpose: Invoked once upon each transition from idle to run state

void BHV_ZigLeg::onIdleToRunState()
{
}

//---------------------------------------------------------------
// Procedure: onRunToIdleState()
//   Purpose: Invoked once upon each transition from run to idle state

void BHV_ZigLeg::onRunToIdleState()
{
}

//---------------------------------------------------------------
// Procedure: onRunState()
//   Purpose: Invoked each iteration when run conditions have been met.

IvPFunction* BHV_ZigLeg::onRunState()
{
  // Part 1: Build the IvP function
  IvPFunction *ipf = 0;

  bool waypoint_valid;
  int current_waypoint_index = (int)getBufferDoubleVal("WPT_INDEX", waypoint_valid);

  if (current_waypoint_index != m_waypoint_index) {
    m_waypoint_index = current_waypoint_index;

    m_zig_start_time = getBufferCurrTime() + 5;
    m_zig_end_time = m_zig_start_time + m_zig_duration;

    m_zig_effective_angle = m_zig_angle + getBufferDoubleVal("DESIRED_HEADING", waypoint_valid);
  }

  if (getBufferCurrTime() >= m_zig_start_time && getBufferCurrTime() <= m_zig_end_time) {
    ipf = getMotionFunction();
    if(ipf == 0) postWMessage("Problem Creating the IvP Function");
  } else {
    //m_zig_effective_angle = 0;
    ipf = 0;
  }

  // Part N: Prior to returning the IvP function, apply the priority wt
  // Actual weight applied may be some value different than the configured
  // m_priority_wt, depending on the behavior author's insite.

  if(ipf)
    ipf->setPWT(m_priority_wt);

  return(ipf);
}

IvPFunction *BHV_ZigLeg::getMotionFunction() {
    ZAIC_PEAK zig(m_domain, "course");
    zig.setSummit(m_zig_effective_angle);
    zig.setPeakWidth(0);
    zig.setBaseWidth(180.0);
    zig.setSummitDelta(0);
    zig.setValueWrap(true);
    if (!zig.stateOK()) return(0);
    IvPFunction *f = zig.extractIvPFunction();
    return f;
}
