/************************************************************/
/*    NAME: Erik Boesen                                              */
/*    ORGN: MIT                                             */
/*    FILE: BHV_ZigLeg.h                                      */
/*    DATE:                                                 */
/************************************************************/

#ifndef ZigLeg_HEADER
#define ZigLeg_HEADER

#include <string>
#include "IvPBehavior.h"

class BHV_ZigLeg : public IvPBehavior {
public:
  BHV_ZigLeg(IvPDomain);
  ~BHV_ZigLeg() {};

  bool         setParam(std::string, std::string);
  void         onSetParamComplete();
  void         onCompleteState();
  void         onIdleState();
  void         onHelmStart();
  void         postConfigStatus();
  void         onRunToIdleState();
  void         onIdleToRunState();
  IvPFunction* onRunState();

protected: // Local Utility functions
  bool poll();

protected: // Configuration parameters
  double m_pulse_radius;
  double m_pulse_duration;
  int m_waypoint_index;
  bool m_waiting;
  int m_end_time;

protected: // State variables
};

#define IVP_EXPORT_FUNCTION

extern "C" {
  IVP_EXPORT_FUNCTION IvPBehavior * createBehavior(std::string name, IvPDomain domain)
  {return new BHV_ZigLeg(domain);}
}
#endif
