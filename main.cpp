#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <cstdint>
using namespace std;

// Function declarations from other files
 vector<float> load_audio(const  string& path);
 vector< vector<float>> compute_spectrogram( vector<float>& samples, int window_size, int hop_size);
 vector< pair<int, int>> find_peaks(const  vector< vector<float>>& spec);
 vector< pair<uint64_t, int>> generate_fingerprints(const  vector< pair<int, int>>& peaks);
void save_to_database(const  string& path, const  string& song, const  vector< pair<uint64_t, int>>& fps);
 unordered_map<uint64_t,  pair< string, int>> load_database(const  string& path);
 string match_fingerprints(const  vector< pair<uint64_t, int>>& query, const  unordered_map<uint64_t,  pair< string, int>>& db);

int main(int argc, char* argv[]) {
    if (argc < 3) {
         cout << "Usage:\n";
         cout << "  ./shazam add <wav file> <song name>\n";
         cout << "  ./shazam query <wav file>\n";
        return 1;
    }

     string command = argv[1];
     string filepath = argv[2];

     vector<float> samples = load_audio(filepath);
    if (samples.empty()) {
         cerr << "Error loading audio.\n";
        return 1;
    }

    auto spec = compute_spectrogram(samples, 4096, 2048);
    auto peaks = find_peaks(spec);
    auto fingerprints = generate_fingerprints(peaks);

     cout << "Generated " << fingerprints.size() << " fingerprints.\n";

    if (command == "add") {
        if (argc < 4) {
             cerr << "Please provide song name.\n";
            return 1;
        }
         string song_name = argv[3];
        save_to_database("data/db/fingerprints.txt", song_name, fingerprints);
         cout << "✅ Song added: " << song_name <<  endl;

    } else if (command == "query") {
        auto db = load_database("data/db/fingerprints.txt");
         string match = match_fingerprints(fingerprints, db);
         cout << "🎯 Best Match: " << (match.empty() ? "None Found" : match) <<  endl;
    } else {
         cerr << "❌ Unknown command: " << command << "\n";
        return 1;
    }

    return 0;
}
