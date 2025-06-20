#include <portaudio.h>
#include <sndfile.h>
#include <iostream>
#include <vector>
#include <string>

#define SAMPLE_RATE 44100
#define NUM_CHANNELS 1
#define SECONDS 5

int record_from_mic(const std::string& output_path) {
    Pa_Initialize();

    PaStream* stream;
    std::vector<float> recorded(SAMPLE_RATE * SECONDS);

    Pa_OpenDefaultStream(&stream,
                         NUM_CHANNELS,      // input channels
                         0,                 // output channels
                         paFloat32,
                         SAMPLE_RATE,
                         256,               // frames per buffer
                         nullptr,
                         nullptr);

    Pa_StartStream(stream);
    std::cout << "🎤 Recording for " << SECONDS << " seconds...\n";
    Pa_ReadStream(stream, recorded.data(), SAMPLE_RATE * SECONDS);
    Pa_StopStream(stream);
    Pa_CloseStream(stream);
    Pa_Terminate();

    SF_INFO sfinfo;
    sfinfo.channels = NUM_CHANNELS;
    sfinfo.samplerate = SAMPLE_RATE;
    sfinfo.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;

    SNDFILE* file = sf_open(output_path.c_str(), SFM_WRITE, &sfinfo);
    if (!file) {
        std::cerr << "❌ Failed to open file for writing: " << output_path << "\n";
        return 1;
    }
    sf_writef_float(file, recorded.data(), recorded.size());
    sf_close(file);

    std::cout << "✅ Saved recording to " << output_path << "\n";
    return 0;
}
