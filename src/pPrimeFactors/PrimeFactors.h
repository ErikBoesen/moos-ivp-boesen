/************************************************************/
/*    NAME: Erik Boesen                                              */
/*    ORGN: MIT                                             */
/*    FILE: PrimeFactors.h                                          */
/*    DATE:                                                 */
/************************************************************/

#ifndef PrimeFactors_HEADER
#define PrimeFactors_HEADER

#include "MOOS/libMOOS/MOOSLib.h"

class PrimeFactors : public CMOOSApp
{
 public:
   PrimeFactors();
   ~PrimeFactors();

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();

 protected:
   void RegisterVariables();

 private: // Configuration variables

 private: // State variables
};

#endif 
