# 🎧 Shazam-Like Audio Fingerprinting in C++

This project is a minimal Shazam-style audio fingerprinting system built in **C++** with support for **PostgreSQL** storage. It allows you to:

- Add a song by generating fingerprints and storing them in a database.
- Record audio from the mic or use a file to find the closest match.
- Convert MP3 files to WAV automatically for processing.

---
##  Demo 
i will add soon
---

## 🔧 Features

- 🎙 Record from microphone (`./shazam record`)
- ➕ Add a song from `.wav` or `.mp3` file (`./shazam add`)
- 🔍 Match a query audio snippet to a stored song (`./shazam find`)
- 💾 Fingerprints stored in PostgreSQL (using `libpqxx`)
- 🎶 MP3 → WAV conversion using FFmpeg (auto-handled)

---

## 🧠 How It Works

### 1. Convert MP3 to WAV
Since **MP3 is compressed**, we convert it to **WAV (PCM format)** using FFmpeg:

```bash
ffmpeg -y -i input.mp3 data/output.wav
```

**Why WAV?**  
WAV stores raw PCM audio, ideal for analysis. MP3 discards some info (lossy), which can affect fingerprinting. FFT requires linear, uncompressed data.

---

### 2. Load and Normalize Audio

We load the WAV file and extract PCM data (raw amplitudes).

Example:
```cpp
[ 0.0, 0.7, 1.0, 0.7, 0.0, -0.7, -1.0, ... ]
```

❗️**Normalization isn't mandatory**  
We don't care about loudness. What matters is the **pattern of frequencies**.

---

### 3. Compute FFT Spectrogram

We slide a window over the waveform and apply the **Fast Fourier Transform** (FFT) to convert time-domain signal into frequency-domain.

Each window gives us the **dominant frequencies** at that time.

Result: A **spectrogram** (2D map of frequency vs time)

---

### 4. Find Spectral Peaks

From the spectrogram, we extract **local peaks** — frequency bins that stand out relative to neighbors.

Even if the audio is quiet, peaks still show up in the same places (just with smaller magnitude).

---

### 5. Generate Fingerprints

We hash frequency-time pairs like:

```cpp
hash = hash_function(f1, f2, time_delta)
```

This results in a set of unique IDs (fingerprints) per song.

---

### 6. Match Against Database

Fingerprints are stored in PostgreSQL:

```sql
CREATE TABLE songs (
  id SERIAL PRIMARY KEY,
  name TEXT NOT NULL
);

CREATE TABLE fingerprints (
  hash BIGINT NOT NULL,
  offset INTEGER NOT NULL,
  song_id INTEGER REFERENCES songs(id),
  PRIMARY KEY (hash, song_id, offset)
);
```

Matching is done by counting hash overlaps and identifying the best match.

---

## ❓ FAQ

### ❔ Why convert MP3 to WAV?
- MP3 is compressed and not suitable for FFT.
- WAV provides raw PCM data.
- We use: `ffmpeg -i input.mp3 output.wav`

---

### ❔ What is PCM audio?
- PCM = Pulse Code Modulation
- It's the raw format of digital sound (amplitude over time).
- WAV files typically use PCM.

---

### ❔ Does fingerprinting depend on volume?
**No.**
- FFT discards amplitude info.
- Peaks are relative.
- Fingerprints use only frequency + timing.

Even quiet recordings can be matched.

---

### ❔ What's the order of processing?

```
Audio (.wav or .mp3)
   ↓
Convert to PCM (if needed)
   ↓
FFT → Spectrogram
   ↓
Peak Finding
   ↓
Fingerprint Generation
   ↓
Save or Query via Database
```

---

### ❔ Example Walkthrough

1. Record or load this waveform:
   ```
   [0, 0.5, 1, 0.5, 0, -0.5, -1, ...]  // A4 tone (440 Hz)
   ```

2. FFT on window → sees 440 Hz dominant.

3. Spectrogram:
   ```
   Time →     t1    t2    t3 ...
   Freq ↓
   440 Hz     0.9   0.8   ...
   500 Hz     0.1   0.2
   ```

4. Peaks:
   ```
   t1: 440 Hz, t2: 440 Hz
   ```

5. Fingerprints:
   ```
   hash(440, 440, delta=1)
   ```

---

## 🚀 Getting Started

### ✅ Dependencies
- FFmpeg (`sudo pacman -S ffmpeg`)
- PortAudio (`sudo pacman -S portaudio`)
- libsndfile (`sudo pacman -S libsndfile`)
- libpqxx (`sudo pacman -S libpqxx`)

### 🧱 Build

```bash
mkdir build && cd build
cmake ..
make
```

### 🛠 PostgreSQL Setup

```sql
-- Save as data/database.sql
CREATE TABLE songs (
  id SERIAL PRIMARY KEY,
  name TEXT NOT NULL
);

CREATE TABLE fingerprints (
  hash BIGINT NOT NULL,
  offset INTEGER NOT NULL,
  song_id INTEGER NOT NULL REFERENCES songs(id),
  PRIMARY KEY (hash, song_id, offset)
);

CREATE INDEX idx_fingerprint_hash ON fingerprints(hash);
```

Run:
```bash
psql -U <user> -d shazam -f data/database.sql
```

---

## 📦 Example Usage

```bash
# Add a song
./shazam add path/to/song.mp3 "Song Title"

# Query with a sample
./shazam query sample.mp3

# Record + identify
./shazam find
```

---

## 📂 Folder Structure

```
shazam/
│
├── main.cpp                # Entry point
├── audio_loader.cpp        # WAV loading
├── fft_processor.cpp       # FFT + spectrogram
├── peak_finder.cpp         # Peak detection
├── fingerprint.cpp         # Hashing logic
├── postgresql.cpp          # DB operations
├── mic_recorder.cpp        # PortAudio mic recording
├── CMakeLists.txt
├── data/
│   ├── samples/            # Mic recordings
│   └── output.wav          # Converted audio
```

---

