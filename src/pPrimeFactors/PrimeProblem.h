#ifndef PrimeProblem_HEADER
#define PrimeProblem_HEADER

#include <cstdint>
#include <vector>
#include <cstdlib>

using namespace std;
// TODO: const correctness
class PrimeProblem {
public:
    PrimeProblem(uint64_t number, int received, double start_time);
    bool factorize();
    string make_response(int calculated, int end_time);

    uint64_t m_orig;

private:
    vector<uint64_t> m_factors;
    // TODO: Getters and setters?
    double m_start_time;
    uint64_t m_factor;
    int m_received;
    uint64_t m_number;
};

#endif
