#include "DefaultMap.h"

static std::vector<simVec2> DefaultMapSpawnPoints = {
    {1.5, 2.1},
    {18.5, 2.1},
};

const std::vector<simVec2>& DefaultMap::getSpawnPoints() const {
    return DefaultMapSpawnPoints;
}
