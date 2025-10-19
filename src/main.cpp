/**
 * @file main.cpp
 *
 * @brief Starting point of the snek game, contains the main loop.
 *
 * @authors Jacek Zub
 */
#include <complex>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>


#include <print>
#include <cstdint>
#include <ranges>
#include <valarray>

#include "snek/events/event_bus.h"
#include "snek/events/input_manager.h"

#include "snek/constants.hpp"

auto pool_events(sf::Window& window) -> void;
auto draw_something(sf::RenderWindow& window) -> void;
auto draw_rectangle(sf::RenderWindow& window) -> void;
auto draw_triangle(sf::RenderWindow& window) -> void;
auto center_window_on_screen(sf::RenderWindow & window) -> void;
auto draw_scene(sf::RenderWindow& window) -> void;


auto main() -> int32_t {
    auto window = sf::RenderWindow(
        sf::VideoMode({snek::WINDOW_WIDTH, snek::WINDOW_HEIGHT}),
        snek::WINDOW_TITLE);

    window.setFramerateLimit(snek::FRAMERATE_LIMIT);
    EventBus event_bus;
    const InputManager input_manager(event_bus);

    center_window_on_screen(window);

    while (window.isOpen()) {

        while (const auto& event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close(); //TODO remove this when we have proper game loop handling
            }
            input_manager.handle_event(*event); // TODO debug if it can be empty optional
        }

        draw_scene(window);

        window.display();
    }

    return 0;
}


auto euclidean_distance_square(const float x, const float y) -> float {
    static constexpr int tile_square_x = snek::tile_size.x * snek::tile_size.x;
    static constexpr int tile_square_y = snek::tile_size.y * snek::tile_size.y;
    return tile_square_x * x*x + tile_square_y * y*y;
}
auto relative_height(const double distance, const double object_height, const size_t vfov = 90) -> double {
    return object_height / (std::sqrt(distance) * std::tan(vfov / 2)); // how much percent of screen height will the project appear to take
}
auto euclidian_distance_square(const sf::Vector2f& vec) -> float {
    return euclidean_distance_square(vec.x, vec.y);
}
auto dot_product(const sf::Vector2f& lhs, const sf::Vector2f& rhs) -> double {
    return lhs.x * rhs.x + lhs.y * rhs.y;
}
auto magnitude(const sf::Vector2f& vector) -> double {
    return std::sqrt(dot_product(vector, vector));
}
auto relative_horizontal_offset(const double distance, const int camera_direction_angle, const sf::Vector2f& relative_object_distance, const size_t hfov) -> double {
    static constexpr double a2r = std::numbers::pi / 360;
    const double camera_direction_angle_in_radians = camera_direction_angle * a2r;
    const sf::Vector2f direction_unit_vec = { static_cast<float>(std::cos(camera_direction_angle)), static_cast<float>(std::sin(camera_direction_angle)) };
    const double angle_between_camera_and_object_in_radians = dot_product(direction_unit_vec, relative_object_distance) / distance; // distance should be equal to magnitude(relative_object_distance)
    return angle_between_camera_and_object_in_radians / hfov / 2;
}
// basic render (mixed with draw)
// here we imagine instance of concrete topology gave us all visible objects
// and their manhattan x y distance from implicitly (0,0) head/camera,
// and we need to render vertex arrays as 3d objects
auto draw_scene(sf::RenderWindow& window) -> void {
    const auto windowSize = window.getSize();
    // DRAW_SKY()
    // DRAW_BACKGROUND()
    window.clear(sf::Color::Black); //for now, it's our sky and background

    // DRAW_FLOOR()
    // simple lattice for every topology?

    // DRAW_SCENE_OBJECTS()
    //input: implicit head at 0,0
    int hfov = 90; // horizontal field of view max 360
    int vfov = 90; // vertical field of view max 360
    int tilt = 0; // tiled camera view
    int camera_horizontal_direction = 90; // direction of view
    int camera_height = snek::tile_size.z * 0.8; // please make our lives simpler and never let camera see the top side of a tile
    int x = 2; // object 2d manhattan grid center x relative distance
    int y = 6; // object grid center y distance

    constexpr float tile_half_x = snek::tile_size.x / 2;
    constexpr float tile_half_y = snek::tile_size.y / 2;

    sf::Vector2f cornerA;  //nearest corner always visible
    sf::Vector2f cornerB;  //second corner always visible
    std::optional<sf::Vector2f> cornerC; // sometimes only two corners are visible

    // all corners
    sf::Vector2f corner1 = {x - tile_half_x, y - tile_half_y};
    sf::Vector2f corner2 = {x + tile_half_x, y - tile_half_y};
    sf::Vector2f corner3 = {x - tile_half_x, y + tile_half_y};
    sf::Vector2f corner4 = {x + tile_half_x, y + tile_half_y};
    std::array<sf::Vector2f, 4> all_corners = {corner1, corner2, corner3, corner4};
    std::ranges::sort(all_corners, [](const sf::Vector2f& a, const sf::Vector2f& b) { return euclidian_distance_square(a) < euclidian_distance_square(b); });

    // maybe this simplification is good enough
    cornerA = all_corners[0];
    cornerB = all_corners[1];
    cornerC = all_corners[2];

    constexpr double object_height = snek::tile_size.z;
    constexpr double cornerA_euclid_dist = euclidian_distance_square(cornerA);
    constexpr double cornerB_euclid_dist = euclidian_distance_square(cornerB);
    constexpr double cornerC_euclid_dist = euclidian_distance_square(*cornerC);
    constexpr double relative_height_A = relative_height(euclidian_distance_square(cornerA), object_height, vfov); // how much percent of screen height will the project appear to take
    constexpr double relative_height_B = relative_height(euclidian_distance_square(cornerB), object_height, vfov);
    constexpr double relative_height_C = relative_height(euclidian_distance_square(*cornerC), object_height, vfov);


    // this direction should be calculated separately for every corner but this is an approximate hack
    const int horizontal_offset_direction = (x / y > (x * y > 0 ? 1 : -1) * tan(camera_horizontal_direction)) ? 1 : -1; //happens to work in all 4 cases
    const double relative_hoffset_A = relative_horizontal_offset(cornerA_euclid_dist, camera_horizontal_direction, cornerA, hfov);
    const double relative_hoffset_B = relative_horizontal_offset(cornerB_euclid_dist, camera_horizontal_direction, cornerB, hfov);
    const double relative_hoffset_C = relative_horizontal_offset(cornerC_euclid_dist, camera_horizontal_direction, *cornerC, hfov);

    const unsigned int cornerA_projected_x = windowSize.x / 2 + horizontal_offset_direction * windowSize.x * relative_hoffset_A;
    const unsigned int cornerB_projected_x = windowSize.x / 2 + horizontal_offset_direction * windowSize.x * relative_hoffset_B;
    const unsigned int cornerC_projected_x = windowSize.x / 2 + horizontal_offset_direction * windowSize.x * relative_hoffset_C;

    const unsigned int cornerA_projected_y = windowSize.y / 2 - camera_height + relative_height_A * windowSize.y;
    const unsigned int cornerB_projected_y = windowSize.y / 2 - camera_height + relative_height_B * windowSize.y;
    const unsigned int cornerC_projected_y = windowSize.y / 2 - camera_height + relative_height_C * windowSize.y;

    // TODO print the Triangle Strip properly
    sf::VertexArray vertex_array{sf::PrimitiveType::TriangleStrip, 6};
    vertex_array[0].position = cornerB;
    vertex_array[1].position = cornerB +
    vertex_array[2].position = sf::Vector2f(100.f, 500.f);
    vertex_array[3].position = sf::Vector2f(700.f, 500.f);
    vertex_array[0].color = sf::Color::Yellow;
    vertex_array[1].color = sf::Color::Cyan;
    vertex_array[2].color = sf::Color::Magenta;
    vertex_array[3].color = sf::Color::White;
    // draw_rectangle(window);v
    // draw_triangle(window);
    window.draw(vertex_array);
}

auto draw_rectangle(sf::RenderWindow& window) -> void {
    sf::RectangleShape rect;
    rect.setSize({400.f, 300.f});
    rect.setOrigin({
        200.f,
        150.f
    });
    rect.setPosition({
        snek::WINDOW_WIDTH / 2.f,
        snek::WINDOW_HEIGHT / 2.f
    });
    rect.setFillColor(sf::Color::Green);
    window.draw(rect);
}

auto draw_triangle(sf::RenderWindow& window) -> void {
    // create an array of 3 vertices that define a triangle primitive
    sf::VertexArray triangle(sf::PrimitiveType::Triangles, 3);

    // define the position of the triangle's points
    triangle[0].position = sf::Vector2f(200.f, 150.f);
    triangle[1].position = sf::Vector2f(600.f, 150.f);
    triangle[2].position = sf::Vector2f(600.f, 450.f);

    // define the color of the triangle's points
    triangle[0].color = sf::Color::Red;
    triangle[1].color = sf::Color::Blue;
    triangle[2].color = sf::Color::Green;

    window.draw(triangle);
}

auto center_window_on_screen(sf::RenderWindow & window) -> void {
    const auto desktop = sf::VideoMode::getDesktopMode();
    const size_t window_width = window.getSize().x;
    const size_t window_height = window.getSize().y;
    const int center_width = static_cast<int>(desktop.size.x - window_width) / 2;
    const int center_height = static_cast<int>(desktop.size.y - window_height) / 2;

    window.setPosition({center_width, center_height});
}
