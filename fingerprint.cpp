#include <vector>
#include <unordered_map>
#include <cstdint>

using namespace std;

uint64_t custom_hash(int f1, int f2, int dt) {
    return ((uint64_t)f1 << 32) | ((uint64_t)f2 << 16) | (uint64_t)dt;
}

 vector< pair<uint64_t, int>> generate_fingerprints(const  vector< pair<int, int>>& peaks) {
     vector< pair<uint64_t, int>> fingerprints;
    for (size_t i = 0; i < peaks.size(); i++) {
        for (size_t j = i + 1; j <  min(i + 10, peaks.size()); j++) {
            int t1 = peaks[i].first;
            int f1 = peaks[i].second;
            int t2 = peaks[j].first;
            int f2 = peaks[j].second;
            fingerprints.push_back({custom_hash(f1, f2, t2 - t1), t1});
        }
    }
    return fingerprints;
}
