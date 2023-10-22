#include "Edge.h"

namespace duck_app {
    Edge::Edge(const int v1, const int v2, const int t1, const int t2) {
        vertex_position_1 = v1;
        vertex_position_2 = v2;

        triangle_index_1 = t1;
        triangle_index_2 = t2;
    }
}
