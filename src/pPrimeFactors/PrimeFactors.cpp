/************************************************************/
/*    NAME: Erik Boesen                                              */
/*    ORGN: MIT                                             */
/*    FILE: PrimeFactors.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include <cstdint>
#include <cstdlib>
#include <sstream>
#include "MBUtils.h"
#include "PrimeFactors.h"
#include "PrimeProblem.h"

using namespace std;

//---------------------------------------------------------
// Constructor

PrimeFactors::PrimeFactors() {
    int m_processed = 0;
    int m_received = 0;
}

//---------------------------------------------------------
// Destructor

PrimeFactors::~PrimeFactors()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool PrimeFactors::OnNewMail(MOOSMSG_LIST &NewMail) {
  MOOSMSG_LIST::iterator p;

  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;

    string key = msg.GetKey();
    if (key == "NUM_VALUE") {
        uint64_t in = strtoull(msg.GetString().c_str(), NULL, 0);
        cout << "Number recieved: " << in << endl;
        PrimeProblem prob(in, m_received++);
        m_queue.push_front(move(prob));
        cout << "The PrimeProblem object has " << prob.m_orig << endl;
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

bool PrimeFactors::OnConnectToServer() {
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

bool PrimeFactors::Iterate() {
    if (!m_queue.empty()) {
        cout << "Found one!" << m_queue.front().m_orig << endl;
        if (m_queue.front().factorize()) {
            cout << "Found solution." << endl;
            m_Comms.Notify("PRIME_RESULT", m_queue.front().make_response(m_processed++));
            m_queue.pop_front();
        } else {
            cout << "Didn't finish this time, trying again later" << endl;
            // Didn't finish in desired number of iterations,
            // will continue next iteration or at a later time.
        }
    }
    return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool PrimeFactors::OnStartUp() {
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

void PrimeFactors::RegisterVariables() {
    Register("NUM_VALUE", 0);
}
