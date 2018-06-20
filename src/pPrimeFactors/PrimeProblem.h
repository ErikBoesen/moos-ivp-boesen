#ifndef PrimeProblem_HEADER
#define PrimeProblem_HEADER

#include <cstdint>
#include <vector>
#include <cstdlib>

using namespace std;
// TODO: const correctness
class PrimeProblem {
public:
    PrimeProblem(int number, int recieved);
    bool factorize();
    string make_response(int calculated);

private:
    vector<uint64_t> m_factors;
    // TODO: Getters and setters?
    int m_solve_time;
    int m_factor;
    int m_recieved;
    int m_orig;
    int m_number;
};

#endif
