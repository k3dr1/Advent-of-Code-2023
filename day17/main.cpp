#include <array>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

using std::ifstream;
using std::string;
using std::vector;

struct Node {
    int y;
    int x;
    std::array<int, 4> jmps;
    int g;
    int f;

    Node() = default;

    Node(int y_val, int x_val, std::array<int, 4> jmps_val, int g_val, int f_val)
        : y(y_val)
        , x(x_val)
        , jmps(jmps_val)
        , g(g_val)
        , f(f_val)
    {
    }

    bool operator<(const Node& rhs) const
    {
        return f > rhs.f;
    }

    bool operator==(const Node& rhs) const
    {
        bool res = true;
        res &= (y == rhs.y);
        res &= (x == rhs.x);
        res &= (jmps[1] == rhs.jmps[1]);
        res &= (jmps[2] == rhs.jmps[2]);
        res &= (jmps[3] == rhs.jmps[3]);
        res &= (jmps[4] == rhs.jmps[4]);
        res &= (g == rhs.g);
        res &= (f == rhs.f);
        return res;
    }
};

template <>
struct std::hash<Node> {
    std::size_t operator()(const Node& n) const
    {
        std::string s = "" + to_string(n.y) + " " + to_string(n.x) + " "
            + to_string(n.jmps[0]) + " " + to_string(n.jmps[1]) + " " + to_string(n.jmps[2]) + " " + to_string(n.jmps[3]);
        return hash<string>()(s);
    }
};

int heuristic(const int y, const int x, const vector<string>& grid)
{
    return std::abs(int(grid.size() - 1 - y)) + std::abs(int(grid.at(0).size() - 1 - x));
}

bool inside(Node& p, vector<string>& grid)
{
    bool res = ((0 <= p.y && p.y < grid.size()) && (0 <= p.x && p.x < grid.at(0).size()));
    return res;
}

void grid_print(const vector<string>& grid)
{
    for (auto& line : grid) {
        std::cout << line << '\n';
    }
    return;
}

std::vector<Node> neighbors(Node u, vector<string>& grid)
{
    // refresh the fucking jumps
    vector<Node> res;
    Node n0 = Node(u.y - 1, u.x, u.jmps, u.g, 0);
    Node n1 = Node(u.y, u.x - 1, u.jmps, u.g, 0);
    Node n2 = Node(u.y + 1, u.x, u.jmps, u.g, 0);
    Node n3 = Node(u.y, u.x + 1, u.jmps, u.g, 0);
    if (inside(n0, grid) && u.jmps[2] == 3) {
        n0.jmps[0] -= 1;
        if (0 <= n0.jmps[0]) {
            n0.jmps[1] = 3;
            n0.jmps[2] = 3;
            n0.jmps[3] = 3;
            n0.g += grid.at(n0.y).at(n0.x) - '0';
            n0.f = n0.g + heuristic(n0.y, n0.x, grid);
            res.push_back(n0);
        }
    }
    if (inside(n1, grid) && u.jmps[3] == 3) {
        n1.jmps[1] -= 1;
        if (0 <= n1.jmps[1]) {
            n1.jmps[0] = 3;
            n1.jmps[2] = 3;
            n1.jmps[3] = 3;
            n1.g += grid.at(n1.y).at(n1.x) - '0';
            n1.f = n1.g + heuristic(n1.y, n1.x, grid);
            res.push_back(n1);
        }
    }
    if (inside(n2, grid) && u.jmps[0] == 3) {
        n2.jmps[2] -= 1;
        if (0 <= n2.jmps[2]) {
            n2.jmps[0] = 3;
            n2.jmps[1] = 3;
            n2.jmps[3] = 3;
            n2.g += grid.at(n2.y).at(n2.x) - '0';
            n2.f = n2.g + heuristic(n2.y, n2.x, grid);
            res.push_back(n2);
        }
    }
    if (inside(n3, grid) && u.jmps[1] == 3) {
        n3.jmps[3] -= 1;
        if (0 <= n3.jmps[3]) {
            n3.jmps[0] = 3;
            n3.jmps[1] = 3;
            n3.jmps[2] = 3;
            n3.g += grid.at(n3.y).at(n3.x) - '0';
            n3.f = n3.g + heuristic(n3.y, n3.x, grid);
            res.push_back(n3);
        }
    }
    return res;
}

#define WIDTH 13
#define HEIGHT 13

int pathfind(vector<string> grid)
{
    std::priority_queue<Node> queue;
    std::unordered_map<Node, Node> visited;
    bool stop = false;
    // std::array<std::array<bool, WIDTH>, HEIGHT> unvisited;

    // Start node
    queue.push(Node(0, 0, std::array<int, 4> { 3, 3, 3, 3 }, 0, 0));

    while (!queue.empty() && !stop) {
        Node u = queue.top();
        queue.pop();

        // std::cout << "x=" << u.x << " y=" << u.y << " f=" << u.f << '\n';
        if (u.y == grid.size() - 1 && u.x == grid.at(0).size() - 1) {
            stop = true;
            return u.g;
        }

        for (Node v : neighbors(u, grid)) {
            if (visited.find(v) == visited.end()) {
                // if (v.y == 12 && v.x == 12) stop = true;
                visited[v] = v;
                queue.push(v);
            } else {
                // Already in the visited
                // v and alt_v have the same jmps, only different weights
                Node alt_v = visited.at(v);
                if (v.f < alt_v.f) {
                    visited.at(v) = v;
                }
            }
        }
    }

    return -1;
}

char asciiToNum(char c)
{
    switch (c) {
    case ('1'):
        return 1;
    case ('2'):
        return 2;
    case ('3'):
        return 3;
    case ('4'):
        return 4;
    case ('5'):
        return 5;
    case ('6'):
        return 6;
    case ('7'):
        return 7;
    case ('8'):
        return 8;
    case ('9'):
        return 9;
    case ('0'):
        return 0;
    default:
        return -1;
    }
}

// PART 2 STUFF

enum Dir {
    Up = 0,
    Right = 1,
    Down = 2,
    Left = 3
};

constexpr const std::array<Dir, 2> getOrthogonalDirections(const Dir d)
{
    switch (d) {
    case (Dir::Up):  return { Dir::Left, Dir::Right };
    case (Dir::Down): return { Dir::Left, Dir::Right };
    case (Dir::Right): return { Dir::Up, Dir::Down };
    case (Dir::Left): return { Dir::Up, Dir::Down };
    }
    std::unreachable();
}

struct vec2 {
    int x;
    int y;
};

vec2 dirToVec(Dir d)
{
    switch (d) {
    case (Dir::Up): return { 0, -1 };
    case (Dir::Down): return { 0, 1 };
    case (Dir::Right): return { 1, 0 };
    case (Dir::Left): return { -1, 0 };
    }
    std::unreachable();
}

struct Cell {
    vec2 coord;
    Dir lastMovedDirection;
    int numMoves;
    int heatLoss;
};

bool inside(const vec2& v, const int width, const int height)
{
    return ((0 <= v.x)
        && (v.x < width)
        && (0 <= v.y)
        && (v.y < height));
}

const std::vector<Cell> generateNeighbors(const Cell& cell, const std::vector<std::vector<char>>& grid)
{
    static const int H = grid.size();
    static const int W = grid.at(0).size();
    constexpr int minJump = 4;
    constexpr int maxJump = 10;
    std::vector<Cell> generated {};

    if (cell.numMoves < maxJump) {
        const auto movementVector = dirToVec(cell.lastMovedDirection);
        const vec2 newPos = { cell.coord.x + movementVector.x, cell.coord.y + movementVector.y };
        if (inside(newPos, W, H))
            generated.emplace_back(newPos, 
                                   cell.lastMovedDirection, 
                                   cell.numMoves + 1, 
                                   cell.heatLoss + grid.at(newPos.y).at(newPos.x));
    }

    for (const auto d : getOrthogonalDirections(cell.lastMovedDirection)) {
        const auto movementVector = dirToVec(d);
        vec2 newPos { cell.coord.x, cell.coord.y };
        int incurredHeatLoss = cell.heatLoss;
        bool outside = false;
        for (int i = 1; i <= minJump; i++) {
            newPos.x = newPos.x + movementVector.x;
            newPos.y = newPos.y + movementVector.y;
            if (!inside(newPos, W, H)) {
                outside = true;
                break;
            };
            incurredHeatLoss += grid.at(newPos.y).at(newPos.x);
        }
        if (!outside)
            generated.emplace_back(newPos, d, minJump, incurredHeatLoss);
    }

    return generated;
}

template <>
struct std::hash<Cell> {
    std::size_t operator()(const Cell& cell) const
    {
        return ((cell.coord.x << 24) | (cell.coord.y << 16) | (cell.numMoves<<8) | (cell.lastMovedDirection));
    }
};

struct Equal {
    bool operator()(const Cell& a, const Cell& b) const
    {
        return (a.coord.x == b.coord.x
            && a.coord.y == b.coord.y);
    }
};

int findMindHeatLoss2(const std::vector<std::vector<char>>& grid)
{
    auto startHorizontal = Cell { 0, 0, Dir::Right, 10, 0 };
    auto startVertical =   Cell { 0, 0, Dir::Down,  10, 0 };
    auto dijkstraCmp = [](const Cell& a, const Cell& b) { return a.heatLoss >= b.heatLoss; };
    auto heuristicCmp = [&grid](const Cell& a, const Cell& b) {
        const int h_a = ((grid.size() - 1) - a.coord.y) + ((grid.at(0).size() - 1) - a.coord.x);
        const int f_a = (a.heatLoss) + (h_a);

        const int h_b = ((grid.size() - 1) - b.coord.y) + ((grid.at(0).size() - 1) - b.coord.x);
        const int f_b = (b.heatLoss) + (h_b);

        return f_a >= f_b;
    };

    std::unordered_map<Cell, int, std::hash<Cell>, Equal> visited;
    std::priority_queue<Cell, std::vector<Cell>, decltype(heuristicCmp)> pq(heuristicCmp);

    pq.push(startHorizontal);
    pq.push(startVertical);
    visited[startHorizontal] = 0;
    visited[startVertical] = 0;

    static long long unsigned int count = 0;

    while (!pq.empty()) {
        count++;
        const auto pos = pq.top();
        pq.pop();

        if (count % 1'000'000 == 0) {
            std::cout << "Front of the queue: x=" << std::setw(3) << pos.coord.x
                      << " y=" << std::setw(3) << pos.coord.y
                      << " lastMovedDirection=" << pos.lastMovedDirection
                      << " numMoves=" << pos.numMoves
                      << " heatLoss=" << pos.heatLoss
                      << " distanceToEnd=" << ((grid.size() - 1) - pos.coord.y) + ((grid.at(0).size() - 1) - pos.coord.x)
                      << " iteration=" << count
                      << '\n';
        }

        if ((pos.coord.y == grid.size() - 1) && (pos.coord.x == grid.at(0).size() - 1)) {
            return pos.heatLoss;
        }

        // std::cout << "Not the end position\n";

        visited[pos] = pos.heatLoss;

        // std::cout << "Added it to the visited hashmap\n";

        for (const auto& cell : generateNeighbors(pos, grid)) {

            //std::cout << "Generated jumps, checking\n";
            // const auto genKey = cellToKey(cell);

            if (auto const valIt = visited.find(cell);
                (valIt == visited.end()) || (valIt->second > cell.heatLoss)) {
                pq.push(cell);
            }
        }
    }
    return -1;
}

int main()
{

    // ifstream f("example.txt");

    // if (!f.is_open()) {
    //     std::cout << "Couldnt open the file\n";
    //     return 1;
    // }

    // vector<string> grid;
    // string line;
    // while (getline(f, line)) {
    //     std::cout << line << '\n';
    //     grid.push_back(line);
    // }

    // int res = min_heat_loss(grid);
    // int res = pathfind(grid);
    // std::cout << "The answer for part 1 is " << res << '\n';

    std::cout << "==[Starting part 2]==" << '\n';

    ifstream f("input.txt");

    if (!f.is_open()) {
        std::cout << "Couldnt open the file\n";
        return 1;
    }

    vector<vector<char>> grid {};
    string line;
    while (std::getline(f, line)) {
        std::vector<char> row;
        // std::cout << "Processing " << line << '\n';
        for (int i = 0; i < line.size(); i++) {
            // std::cout << "i=" << i << " line[i]=" << line[i] << '\n';
            if (asciiToNum(line[i]) != -1) {
                row.push_back(asciiToNum(line[i]));
            }
        }
        // std::cout << "\n";
        grid.push_back(row);
    }

    std::cout << "Printing the grid...\n";

    for (auto r : grid) {
        for (auto num : r) {
            std::cout << (int)num;
        }
        std::cout << "\n";
    }

    try {
        const int res2 = findMindHeatLoss2(grid);
        std::cout << "The answer for part 2 is " << res2 << '\n';
    } catch (std::exception e) {
        std::cout << e.what() << '\n';
    }

    return 0;
}
