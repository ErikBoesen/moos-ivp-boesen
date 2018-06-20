#include <cstdint>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <sstream>
#include <string>
#include <iostream> // TODO: Remove
#include "PrimeProblem.h"

#define MAX_ITER 10000

using namespace std;

PrimeProblem::PrimeProblem(uint64_t number, int received) {
    m_orig = number;
    m_number = number;
    m_received = received;
    m_factor = 2;
}

// Returns boolean, whether or not problem was finished with given number of iterations.
bool PrimeProblem::factorize() {
    int iterations = 0;

    cout << "m_factor:" << m_factor << endl;

    for (; m_factor <= sqrt(m_number); ++m_factor) {
        while (m_number % m_factor == 0) {
            m_factors.push_back(m_factor);
            m_number /= m_factor;
        }
        if (++iterations > MAX_ITER) {
            return false;
        }
    }
    if (m_number > 2) m_factors.push_back(m_number);
    return true;
}

string PrimeProblem::make_response(int calculated) {
    // Response format:
    // "orig=90090,received=34,calculated=33,solve_time=2.03,primes=2:3:3:5:7:11:13,username=jane"
    stringstream ss;
    ss << "orig=" << m_orig << ",received=" << m_received << ",calculated=" << calculated << ",solve_time=" << m_solve_time << ",primes=";
    ss << m_factors[0];
    for (int i = 1; i < m_factors.size(); i++) ss << ':' << m_factors[i];
    ss << ",username=erik";
    return ss.str();
}
