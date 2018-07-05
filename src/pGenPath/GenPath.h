/************************************************************/
/*    NAME: Erik Boesen                                              */
/*    ORGN: MIT                                             */
/*    FILE: GenPath.h                                          */
/*    DATE:                                                 */
/************************************************************/

#ifndef GenPath_HEADER
#define GenPath_HEADER

#include <GeomUtils.h>
#include "MOOS/libMOOS/MOOSLib.h"

class GenPath : public CMOOSApp
{
 public:
   GenPath();
   ~GenPath();

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();

 protected:
   void RegisterVariables();

 private: // Configuration variables
   unsigned int m_visit_radius = 5;

 private: // State variables
   XYSegList points;
   XYSegList needed_points;
   bool embark();
   bool m_regenerate;
   double m_prev_x;
   double m_prev_y;

   double distance(double x1, double y1, double x2, double y2);
   bool within_range(double x, double y);
};

#endif 
