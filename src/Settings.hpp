#pragma once
#include <vector>
#include "ScoreStore.hpp"

struct Settings {
    bool showFPS = false;
    std::vector<int> scores;

    Settings() : scores(ScoreStore::load()) {}          // ← load at boot
    ~Settings()               { ScoreStore::save(scores); } // ← save on exit
};
