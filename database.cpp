#include <pqxx/pqxx>
#include <iostream>
#include <vector>
#include <cstdint>
#include <string>
using namespace std;

void save_to_pg(const string& song_name, const vector<pair<uint64_t, int>>& fps) {
    pqxx::connection c("dbname=shazam user=shivam password=StarLight");
    pqxx::work txn(c);

    auto result = txn.exec_params("INSERT INTO songs (name) VALUES ($1) RETURNING id", song_name);
    int song_id = result[0][0].as<int>();

    for (auto& [hash, time_offset] : fps) {
        txn.exec_params("INSERT INTO fingerprints (hash, time_offset, song_id) VALUES ($1, $2, $3)", hash, time_offset, song_id);
    }

    txn.commit();
    cout << "✅ Saved to Postgres\n";
}

string match_from_pg(const vector<pair<uint64_t, int>>& query_fps) {
    pqxx::connection c("dbname=shazam user=shivam password=StarLight");
    pqxx::work txn(c);

    string query = "SELECT s.name, COUNT(*) as match_count FROM fingerprints f JOIN songs s ON f.song_id = s.id WHERE f.hash IN (";
    for (size_t i = 0; i < query_fps.size(); ++i) {
        query += to_string(query_fps[i].first);
        if (i != query_fps.size() - 1) query += ",";
    }
    query += ") GROUP BY s.name ORDER BY match_count DESC LIMIT 1";

    auto result = txn.exec(query);
    if (result.empty()) return "None Found";
    return result[0][0].as<string>();
    }
