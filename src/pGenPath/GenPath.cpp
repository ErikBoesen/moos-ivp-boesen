/************************************************************/
/*    NAME: Erik Boesen                                              */
/*    ORGN: MIT                                             */
/*    FILE: GenPath.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "GenPath.h"

using namespace std;

GenPath::GenPath() {}
GenPath::~GenPath() {}

bool GenPath::OnNewMail(MOOSMSG_LIST &NewMail)
{
  MOOSMSG_LIST::iterator p;
   
  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;
    string key   = msg.GetKey();

    if (key == "NAV_X") {
        m_x = msg.GetDouble();
    } else if (key == "NAV_Y") {
        m_y = msg.GetDouble();
    } else if (key == "GENPATH_REGENERATE" && msg.GetString() == "true") {
        points = needed_points;
        embark();
    } else if (key == "VISIT_POINT") {
        if (msg.GetString() == "lastpoint") {
            embark();
        } else {
            float x = std::stof(tokStringParse(msg.GetString(), "x", ',', '=').c_str());
            float y = std::stof(tokStringParse(msg.GetString(), "y", ',', '=').c_str());
            points.add_vertex(x, y);
        }
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

bool GenPath::embark() {
    XYSegList new_points;
    unsigned int num_points = points.size();
    double prev_x = m_x,
           prev_y = m_y;

    for (int i = 0; i < num_points; i++) {
        unsigned int closest_vertex = points.closest_vertex(prev_x, prev_y);
        prev_x = points.get_vx(closest_vertex);
        prev_y = points.get_vy(closest_vertex);
        new_points.add_vertex(prev_x, prev_y);
        points.delete_vertex(closest_vertex);
    }
    new_points.delete_vertex(0, 0);

    string return_string = "points=";
    if (new_points.size()) {
        return_string += new_points.get_spec();
        m_Comms.Notify("SURVEY_POINTS", points_string);
    } else {
        m_Comms.Notify("SURVEY", "false");
        m_Comms.Notify("RETURN", "true");
    }
    needed_points = points = new_points;
    return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool GenPath::OnConnectToServer()
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

double GenPath::distance(double x1, double y1, double x2, double y2) {
    return sqrt(pow(x2-x1, 2) + pow(y2-y1, 2));
}
bool GenPath::within_range(double x, y) {
    return distance(m_x, m_y, x, y) <= m_visit_radius;
}

bool GenPath::Iterate()
{   
    for (int i = 0; i < needed_points.size(); ++i) {
        double x = needed_points.get_vx(i);
        double y = needed_points.get_vy(i);
        if (within_range(m_x, m_y, x, y))
            needed_points.delete_vertex(x, y);
    }
    return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool GenPath::OnStartUp()
{
  list<string> sParams;
  m_MissionReader.EnableVerbatimQuoting(false);
  if(m_MissionReader.GetConfiguration(GetAppName(), sParams)) {
    list<string>::iterator p;
    for(p=sParams.begin(); p!=sParams.end(); p++) {
      string original_line = *p;
      string param = stripBlankEnds(toupper(biteString(*p, '=')));
      string value = stripBlankEnds(*p);
      
      if (param == "visit_radius") {
        m_visit_radius = stoi(value);
      }
    }
  }
  
  RegisterVariables();	
  return(true);
}

//---------------------------------------------------------
// Procedure: RegisterVariables

void GenPath::RegisterVariables()
{
    Register("NAV_X", 0);
    Register("NAV_Y", 0);
    Register("GENPATH_REGENERATE", 0);
    Register("VISIT_POINT", 0);
}

