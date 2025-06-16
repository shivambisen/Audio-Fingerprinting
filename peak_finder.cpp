#include <vector>
using namespace std;

 vector< pair<int, int>> find_peaks(const  vector< vector<float>>& spec) {
     vector< pair<int, int>> peaks;
    for (int t = 1; t < spec.size() - 1; t++) {
        for (int f = 1; f < spec[t].size() - 1; f++) {
            float val = spec[t][f];
            if (val > spec[t - 1][f] && val > spec[t + 1][f] && val > spec[t][f - 1] && val > spec[t][f + 1]) {
                if (val > 10.0f) peaks.push_back({t, f});
            }
        }
    }
    return peaks;
}
