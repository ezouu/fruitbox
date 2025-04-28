#pragma once
#include <vector>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

class ScoreStore {
public:
    static fs::path dataPath() {
#ifdef _WIN32
        const char* base = std::getenv("APPDATA");            // e.g. C:\Users\Ed\AppData\Roaming
        fs::path p = base ? base : fs::temp_directory_path();
        return p / "FruitBox";
#elif __APPLE__
        return fs::path(std::getenv("HOME")) / "Library/Application Support/FruitBox";
#else                       // Linux, BSD…
        return fs::path(std::getenv("HOME")) / ".local/share/fruitbox";
#endif
    }

    static fs::path filePath() { return dataPath() / "scores.txt"; }

    /* load existing scores (sorted high→low) */
    static std::vector<int> load() {
        std::vector<int> v;
        std::ifstream f(filePath());
        int x;
        while (f >> x) v.push_back(x);
        std::sort(v.begin(), v.end(), std::greater<int>());
        return v;
    }

    /* append a new score and keep only top N (default 100) */
    static void append(int score, std::vector<int>& vec, std::size_t limit = 100) {
        vec.insert(std::upper_bound(vec.begin(), vec.end(), score, std::greater<int>()), score);
        if (vec.size() > limit) vec.resize(limit);
    }

    static void save(const std::vector<int>& v) {
        fs::create_directories(dataPath());
        std::ofstream f(filePath(), std::ios::trunc);
        for (int s : v) f << s << '\n';
    }
};
