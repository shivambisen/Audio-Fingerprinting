CREATE TABLE songs (
    id SERIAL PRIMARY KEY,
    name TEXT NOT NULL,
    created_at TIMESTAMP DEFAULT NOW()
);

CREATE TABLE fingerprints (
    hash BIGINT NOT NULL,
    time_offset INTEGER NOT NULL,
    song_id INTEGER NOT NULL REFERENCES songs(id),
    PRIMARY KEY (hash, song_id, time_offset)
);

CREATE INDEX idx_fingerprint_hash ON fingerprints(hash);
