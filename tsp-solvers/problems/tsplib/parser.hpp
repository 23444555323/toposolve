#include "../../common/graph.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

namespace tsp {
namespace problems {

class TSPLIBParser {
public:
    static Graph load(const std::string& filepath) {
        Graph g;
        std::ifstream file(filepath);
        if (!file.is_open()) {
            throw std::runtime_error("Could not open file: " + filepath);
        }

        std::string line;
        bool reading_coords = false;
        while (std::getline(file, line)) {
            if (line.find("NODE_COORD_SECTION") != std::string::npos) {
                reading_coords = true;
                continue;
            }
            if (line.find("EOF") != std::string::npos) break;

            if (reading_coords) {
                std::stringstream ss(line);
                int id;
                double x, y;
                if (ss >> id >> x >> y) {
                    g.add_node(x, y);
                }
            }
        }
        std::cout << "Loaded " << g.size() << " nodes from " << filepath << std::endl;
        return g;
    }
};

} // namespace problems
} // namespace tsp
