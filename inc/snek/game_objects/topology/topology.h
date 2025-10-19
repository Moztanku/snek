//
// Created by adame on 10/18/2025.
//

#ifndef TOPOLOGY_H
#define TOPOLOGY_H

#include <vector>
#include <expected>
#include "snek/game_objects/game_object.h"

namespace top {

/** piece of space on a topology object **/
class tile {
private:
    const size_t id = -1; //unique id assigned as the index of top::topology::surface_points
    std::optional<const game_object*> content_ref;
    std::array<const tile*, 4> neighbours; // wsad order - top bottom left right

public:
    tile(size_t _id): id(_id) {};
    ~tile() = default;
    enum class Direction : unsigned char {
        top    = 0,
        bottom = 1,
        left   = 2,
        right  = 3,
    };
    auto operator<=>(const tile&) const = delete;
    auto is_occupied(void) -> bool; // or is_occupied_by_obstacle
    auto get_neighbour() -> std::optional<const tile*>;
};

enum class MoveError {
    TileOccupied,     // we have hit an obstacle
    NotASurfacePoint, // we should probably randomly choose any other valid move
    LargeJump         // we can only move one tile at a time
};

/** base class of all topological surfaces **/
class topology {
private:
    /**
     * discrete (later virtually multidimentional) set of all available grid points a game_object could occupy
     * equivalently all available points of the discrete topology' surface
     * the idea is that the points will not necessarily appear contiguous
     * so thats why we implement **topology class** instead of **game_board class** with usual NxM matrix of grid points
     **/
    std::vector<tile> surface_points;

public:
    topology(); // abstract thus cannot manage directly
    virtual ~topology();
    auto moveEntity(const tile& from, const tile& to) -> std::expected<void, MoveError>;

    /**
    * takes length of a sight ray (arbitrary metric), direction angle and tile of the camera/snake head
    * returns game_objects in the specified range of sight
    * packed into tuples with their manhattan distance from the camera tile
    **/
    auto get_obesrvable_objects(const tile& pov, size_t ray_length, size_t angle, size_t range)
        -> std::vector<std::tuple<game_object* const, int,int>>; // TODO check if returning by value is actually moving

    /**
    * returns all game_objects in a ball of a fixed radius around the tile of the camera/snake head
    */
    auto get_objects_in_range(const tile& pov, size_t radious)
        -> std::vector<std::tuple<game_object* const, int,int>>; // TODO check if returning by value is actually moving
};


} // top

#endif //TOPOLOGY_H
