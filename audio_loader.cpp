#include <iostream>
#include <vector>
#include <sndfile.h>

using namespace std;

vector<float> load_audio(const string& path){
    SF_INFO sfinfo;
    SNDFILE* file= sf_open(path.c_str(), SFM_READ, &sfinfo);
    if (!file) {
        cerr << "Error opening audio file: " << sf_strerror(file) << endl;
        return {};
    }
    vector<float> sample(sfinfo.frames * sfinfo.channels);
    sf_read_float(file, sample.data(), sfinfo.frames * sfinfo.channels);
    sf_close(file);
    return sample;
}