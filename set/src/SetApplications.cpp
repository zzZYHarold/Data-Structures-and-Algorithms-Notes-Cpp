#include "set/include/SetApplications.h"
#include "set/include/UFSets.h"

#include <cstddef>
#include <stdexcept>

int CountProvinces(const int* isConnected, int cityCount) {
    if (cityCount < 0 || (cityCount > 0 && isConnected == nullptr))
        throw std::invalid_argument("invalid city adjacency matrix");
    UFSets provinces(cityCount);
    for (int city = 0; city < cityCount; ++city) {
        for (int neighbor = city + 1; neighbor < cityCount; ++neighbor) {
            if (isConnected[static_cast<std::size_t>(city) * cityCount + neighbor]) {
                int cityRoot = provinces.CollapsingFind(city);
                int neighborRoot = provinces.CollapsingFind(neighbor);
                provinces.WeightedUnion(cityRoot, neighborRoot);
            }
        }
    }
    return provinces.SetCount();
}
