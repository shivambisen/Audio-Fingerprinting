#include "kiss_fft.h"
#include <vector>
#include <cmath>

using namespace std;

 vector< vector<float>> compute_spectrogram( vector<float>& samples, int window_size = 4096, int hop_size = 2048) {
     vector< vector<float>> spectrogram;
    kiss_fft_cfg cfg = kiss_fft_alloc(window_size, 0, 0, 0);

    for (size_t i = 0; i + window_size < samples.size(); i += hop_size) {
         vector<kiss_fft_cpx> in(window_size), out(window_size);
        for (int j = 0; j < window_size; j++) {
            in[j].r = samples[i + j] * (0.5 - 0.5 * cos(2 * M_PI * j / (window_size - 1))); // Hanning
            in[j].i = 0;
        }
        kiss_fft(cfg, in.data(), out.data());

         vector<float> mag(window_size / 2);
        for (int j = 0; j < window_size / 2; j++) {
            mag[j] = sqrt(out[j].r * out[j].r + out[j].i * out[j].i);
        }
        spectrogram.push_back(mag);
    }
    free(cfg);
    return spectrogram;
}
