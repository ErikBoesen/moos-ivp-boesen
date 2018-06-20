/************************************************************/
/*    NAME: Erik Boesen                                              */
/*    ORGN: MIT                                             */
/*    FILE: PrimeFactors.h                                          */
/*    DATE:                                                 */
/************************************************************/
#ifndef PrimeFactors_HEADER
#define PrimeFactors_HEADER

#include <list>
#include "PrimeProblem.h"

#include "MOOS/libMOOS/MOOSLib.h"
using namespace std;

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
        int m_processed;
        int m_received;
        list<PrimeProblem> m_queue;

    private: // State variables
};

#endif
