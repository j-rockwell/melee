//
// Created by John Rockwell on 2/6/25.
//

#ifndef NODE_H
#define NODE_H

enum NodeType {
    AIR,
    GRASS,
    DIRT,
    STONE
};

struct Node {
    NodeType type;
};

#endif //NODE_H
