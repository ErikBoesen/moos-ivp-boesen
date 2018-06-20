#ifndef PrimeProblem_HEADER
#define PrimeProblem_HEADER

#include <cstdint>
#include <vector>
#include <cstdlib>

using namespace std;
// TODO: const correctness
class PrimeProblem {
public:
    PrimeProblem(uint64_t number, int received);
    bool factorize();
    string make_response(int calculated);

    uint64_t m_orig;

private:
    vector<uint64_t> m_factors;
    // TODO: Getters and setters?
    int m_solve_time;
    uint64_t m_factor;
    int m_received;
    uint64_t m_number;
};

#endif
