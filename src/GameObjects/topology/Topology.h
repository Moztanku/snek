//
// Created by adame on 10/18/2025.
//

#ifndef TOPOLOGY_H
#define TOPOLOGY_H

#include <vector>
#include "Topology.h"
#include "GameObjects/Tile.h"

namespace top {

class Topology {
private:
    /** discrete 2d surface with fixed coordinates (not necessarily contiguous) **/
    std::vector<std::vector<Tile>> surface;




};

} // top

#endif //TOPOLOGY_H
