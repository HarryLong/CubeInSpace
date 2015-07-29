#ifndef FLOOD_FILL_TRACKER_H
#define FLOOD_FILL_TRACKER_H

#include <unordered_map>
#include <glm/common.hpp>

class FloodFillTracker
{
public:
    FloodFillTracker();
    ~FloodFillTracker();

    bool tracked(const glm::ivec2 & point);
    bool setTracked(const glm::ivec2 & point);

    void reset(int size_x, int size_y);
private:
    int get_index(const glm::ivec2 & point);
    int m_size_x;
    int m_size_y;
    bool * m_tracker;
};

#endif //FLOOD_FILL_TRACKER_H
