//
// Created by adame on 10/19/2025.
//

#ifndef MATRIXTOPOLOGY_H
#define MATRIXTOPOLOGY_H

#include "topology.h"

/** simple 2D surface bounded by a rectangle */
class MatrixTopology : public top::topology {
  public:
    struct TilePosition {
        size_t x;
        size_t y;
    };
    MatrixTopology(const size_t width, const size_t height): width(width), height(height) {};
    ~MatrixTopology() override;
  private:
    const size_t width;
    const size_t height;
    auto toIndex(const size_t x, const size_t y) const -> size_t;
    auto toIndex(const TilePosition position) const -> size_t { return toIndex(position.x, position.y); };
    auto toPosition(const size_t index) const -> TilePosition;
};

#endif //MATRIXTOPOLOGY_H
