/************************************************************/
/*    NAME: Erik Boesen                                              */
/*    ORGN: MIT                                             */
/*    FILE: BHV_ZigLeg.cpp                                    */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include <cstdlib>
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

  m_pulse_radius = 10;
  m_pulse_duration = 50;
  m_waypoint_index = 0;
  m_waiting = false;
  m_end_time = 0;
}

//---------------------------------------------------------------
// Procedure: setParam()

bool BHV_ZigLeg::setParam(string param, string val)
{
  // Convert the parameter to lower case for more general matching
  param = tolower(param);

  // Get the numerical value of the param argument for convenience once
  double double_val = atof(val.c_str());

  if((param == "pulse_radius") && isNumber(val)) {
      m_pulse_radius = double_val;
      return(true);
  }
  else if (param == "pulse_duration") {
      m_pulse_duration = double_val;
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

  // Check if the waypoint has changed since last time
  if (waypoint_valid && current_waypoint_index != m_waypoint_index) {
      m_waypoint_index = current_waypoint_index;
      m_waiting = true;
      m_end_time = getBufferCurrTime() + 5;
  }

  poll();

  // Part N: Prior to returning the IvP function, apply the priority wt
  // Actual weight applied may be some value different than the configured
  // m_priority_wt, depending on the behavior author's insite.
  if(ipf)
    ipf->setPWT(m_priority_wt);

  return(ipf);
}

// TODO: Nonsensical procedure name
bool BHV_ZigLeg::poll() {
    int curr_time;
    if (m_waiting && (curr_time = getBufferCurrTime()) >= m_end_time) {
        XYRangePulse pulse;
        m_waiting = false;
        bool position_ok = true;
        if (position_ok) pulse.set_x(getBufferDoubleVal("NAV_X", position_ok));
        if (position_ok) pulse.set_y(getBufferDoubleVal("NAV_Y", position_ok));
        pulse.set_label("bhv_pulse");
        pulse.set_rad(m_pulse_radius);
        pulse.set_duration(m_pulse_duration);
        pulse.set_time(curr_time);
        pulse.set_color("edge", "yellow");
        pulse.set_color("fill", "yellow");

        string spec = pulse.get_spec();
        postMessage("VIEW_RANGE_PULSE", spec);

    }
}
