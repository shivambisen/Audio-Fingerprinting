#include <unordered_map>
#include <string>
#include <fstream>
#include <cstdint>
#include <vector>
using namespace std;

 unordered_map<uint64_t,  pair< string, int>> load_database(const  string& path) {
     unordered_map<uint64_t,  pair< string, int>> db;
     ifstream file(path);
    uint64_t h;
     string song;
    int time;
    while (file >> h >> song >> time) {
        db[h] = {song, time};
    }
    return db;
}

void save_to_database(const  string& path, const  string& song, const  vector< pair<uint64_t, int>>& fps) {
     ofstream file(path,  ios::app);
    for (auto& [h, t] : fps) {
        file << h << " " << song << " " << t << "\n";
    }
}
