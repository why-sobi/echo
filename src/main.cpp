#include "echo.hpp"
#include <vector>
#include <cmath>
#include <thread>

int main() {
    using namespace echo;
    using namespace echo::ThreeD;

    hide_cursor();
    clear_screen();

    // 1. Setup Window (Full Terminal)
    auto [tw, th] = get_terminal_size();
    Window win(1, 1, tw - 1, th - 1, " ECHO 3D ENGINE ");

    // 2. Define Cube Vertices (Centered at 0,0,0)
    // Scale is 10 units
    float s = 10.0f;
    std::vector<Point3D> vertices = {
        {-s, -s, -s}, {s, -s, -s}, {s, s, -s}, {-s, s, -s},
        {-s, -s,  s}, {s, -s,  s}, {s, s,  s}, {-s, s,  s}
    };

    // 3. Define Edges (Pairs of vertex indices)
    std::vector<std::pair<int, int>> edges = {
        {0, 1}, {1, 2}, {2, 3}, {3, 0}, // Back face
        {4, 5}, {5, 6}, {6, 7}, {7, 4}, // Front face
        {0, 4}, {1, 5}, {2, 6}, {3, 7}  // Connecting lines
    };

    float angle = 0.0f;
    while (true) {
        win.clean_buffer();
        
        // Center of the window for projection
        int centerX = win.get_w() / 2;
        int centerY = win.get_h() / 2;

        // 4. Rotate and Project
        std::vector<Point3D> rotated;
        for (auto& v : vertices) {
            // Use your existing rotate function
            Point3D r = v.rotate(angle);
            
            // Simple Orthographic Projection + Offset to center
            // (Terminal chars are taller than wide, so we multiply X by 2 for aspect ratio)
            r.x = (r.x * 2.0f) + centerX;
            r.y = r.y + centerY;
            rotated.push_back(r);
        }

        // 5. Draw Edges using your Bresenham line algorithm
        for (auto& edge : edges) {
            Visualizer::Primitive::draw_line(
                win, 
                (int)rotated[edge.first].x, (int)rotated[edge.first].y,
                (int)rotated[edge.second].x, (int)rotated[edge.second].y,
                COLOR(COLOR::CYAN), '*'
            );
        }

        win.render();
        angle += 2.0f; 
        std::this_thread::sleep_for(60_FPS);
    }

    reset_cursor();
    return 0;
}