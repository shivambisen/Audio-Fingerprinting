#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <cstdint>
#include <sys/stat.h>
#include <sys/types.h>

using namespace std;

// External function declarations
vector<float> load_audio(const string& path);
vector<vector<float>> compute_spectrogram(vector<float>& samples, int window_size, int hop_size);
vector<pair<int, int>> find_peaks(const vector<vector<float>>& spec);
vector<pair<uint64_t, int>> generate_fingerprints(const vector<pair<int, int>>& peaks);
void save_to_pg(const string& song_name, const vector<pair<uint64_t, int>>& fps);
string match_from_pg(const vector<pair<uint64_t, int>>& query);
int record_from_mic(const string& output_path);

// Create directory if it doesn't exist
void ensure_directory_exists(const string& dir) {
    struct stat info;
    if (stat(dir.c_str(), &info) != 0) {
        mkdir(dir.c_str(), 0755);
    }
}

// Convert .mp3 to .wav using ffmpeg
bool convert_mp3_to_wav(const string& mp3_path, const string& wav_path) {
    string command = "ffmpeg -y -i \"" + mp3_path + "\" \"" + wav_path + "\"";
    int result = system(command.c_str());
    return result == 0;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Usage:\n";
        cout << "  ./shazam add <audio file> <song name>\n";
        cout << "  ./shazam query <audio file>\n";
        cout << "  ./shazam record\n";
        cout << "  ./shazam find\n";
        return 1;
    }

    string command = argv[1];

    if (command == "record") {
        return record_from_mic("data/samples/mic_input.wav");
    }

    if (command == "find") {
        ensure_directory_exists("data/samples");
        string mic_path = "data/samples/mic_input.wav";
        int result = record_from_mic(mic_path);
        if (result != 0) {
            cerr << "❌ Failed to record from mic.\n";
            return 1;
        }

        vector<float> samples = load_audio(mic_path);
        if (samples.empty()) {
            cerr << "❌ Error loading recorded audio.\n";
            return 1;
        }

        auto spec = compute_spectrogram(samples, 4096, 2048);
        auto peaks = find_peaks(spec);
        auto fingerprints = generate_fingerprints(peaks);
        cout << "Generated " << fingerprints.size() << " fingerprints.\n";

        string match = match_from_pg(fingerprints);
        cout << "🎯 Best Match: " << (match.empty() ? "None Found" : match) << endl;
        return 0;
    }

    if (argc < 3) {
        cerr << "❌ Not enough arguments for '" << command << "'\n";
        return 1;
    }

    string filepath = argv[2];
    string extension = filepath.substr(filepath.find_last_of('.') + 1);
    string wav_path;

    if (extension == "mp3") {
        ensure_directory_exists("data");
        wav_path = "data/output.wav";
        if (!convert_mp3_to_wav(filepath, wav_path)) {
            cerr << "❌ Failed to convert MP3 to WAV.\n";
            return 1;
        }
    } else {
        wav_path = filepath;
    }

    vector<float> samples = load_audio(wav_path);
    if (samples.empty()) {
        cerr << "❌ Error loading audio.\n";
        return 1;
    }

    auto spec = compute_spectrogram(samples, 4096, 2048);
    auto peaks = find_peaks(spec);
    auto fingerprints = generate_fingerprints(peaks);
    cout << "Generated " << fingerprints.size() << " fingerprints.\n";

    if (command == "add") {
        if (argc < 4) {
            cerr << "❌ Please provide song name.\n";
            return 1;
        }
        string song_name = argv[3];
        save_to_pg(song_name, fingerprints);
        cout << "✅ Song added: " << song_name << endl;
    } else if (command == "query") {
        string match = match_from_pg(fingerprints);
        cout << "🎯 Best Match: " << (match.empty() ? "None Found" : match) << endl;
    } else {
        cerr << "❌ Unknown command: " << command << "\n";
        return 1;
    }

    return 0;
}
