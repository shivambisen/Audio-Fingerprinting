#include <unordered_map>
#include <vector>
#include <iostream>
#include <cstdint>

using namespace std;

  string match_fingerprints(
    const  vector< pair<uint64_t, int>>& query,
    const  unordered_map<uint64_t,  pair< string, int>>& db) {
    
     unordered_map< string,  unordered_map<int, int>> offset_votes;

    for (const auto& [h, t_query] : query) {
        auto it = db.find(h);
        if (it != db.end()) {
            const auto& [song, t_db] = it->second;
            int offset = t_db - t_query;
            offset_votes[song][offset]++;
        }
    }

     string best_song;
    int best_score = 0;

    for (const auto& [song, offsets] : offset_votes) {
        for (const auto& [offset, count] : offsets) {
            if (count > best_score) {
                best_score = count;
                best_song = song;
            }
        }
    }

    return best_song;
}
