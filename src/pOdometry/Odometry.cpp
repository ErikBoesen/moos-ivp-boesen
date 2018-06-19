/************************************************************/
/*    NAME: Erik Boesen                                     */
/*    ORGN: MIT                                             */
/*    FILE: Odometry.cpp                                    */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include <cmath>
#include "MBUtils.h"
#include "Odometry.h"

using namespace std;


//---------------------------------------------------------
// Constructor

Odometry::Odometry()
{
    bool   m_first_reading = false;
    double m_current_x = 0;
    double m_current_y = 0;
    double m_previous_x = 0;
    double m_previous_y = 0;
    double m_total_distance = 0;
}

//---------------------------------------------------------
// Destructor

Odometry::~Odometry()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool Odometry::OnNewMail(MOOSMSG_LIST &NewMail)
{
  MOOSMSG_LIST::iterator p;

  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;
    string key = msg.GetKey();
    if (key == "NAV_X" && msg.IsDouble()) {
        m_previous_x = m_current_x;
        m_current_x = msg.GetDouble();
    } else if (key == "NAV_Y" && msg.IsDouble()) {
        m_previous_y = m_current_y;
        m_current_y = msg.GetDouble();
    }

#if 0 // Keep these around just for template
    string key   = msg.GetKey();
    string comm  = msg.GetCommunity();
    double dval  = msg.GetDouble();
    string sval  = msg.GetString();
    string msrc  = msg.GetSource();
    double mtime = msg.GetTime();
    bool   mdbl  = msg.IsDouble();
    bool   mstr  = msg.IsString();
#endif
   }

   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool Odometry::OnConnectToServer()
{
   // register for variables here
   // possibly look at the mission file?
   // m_MissionReader.GetConfigurationParam("Name", <string>);
   // m_Comms.Register("VARNAME", 0);

   RegisterVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool Odometry::Iterate()
{
  m_total_distance += sqrt(pow(m_current_x - m_previous_x, 2) + pow(m_current_y - m_previous_y, 2));
  m_Comms.Notify("ODOMETRY_DIST", m_total_distance);
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool Odometry::OnStartUp()
{
  list<string> sParams;
  m_MissionReader.EnableVerbatimQuoting(false);
  if(m_MissionReader.GetConfiguration(GetAppName(), sParams)) {
    list<string>::iterator p;
    for(p=sParams.begin(); p!=sParams.end(); p++) {
      string original_line = *p;
      string param = stripBlankEnds(toupper(biteString(*p, '=')));
      string value = stripBlankEnds(*p);

      if(param == "FOO") {
        //handled
      }
      else if(param == "BAR") {
        //handled
      }
    }
  }

  RegisterVariables();
  return(true);
}

//---------------------------------------------------------
// Procedure: RegisterVariables

void Odometry::RegisterVariables()
{
    Register("NAV_X", 0);
    Register("NAV_Y", 0);
  // Register("FOOBAR", 0);
}
