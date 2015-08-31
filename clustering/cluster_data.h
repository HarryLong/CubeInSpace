#ifndef CLUSTER_DATA
#define CLUSTER_DATA

#include <gl3.h>

class ClusterData{
public:
    GLfloat slope;
    GLint temperatures[12];
    GLuint illumination[12];
    GLfloat soil_humidities[12];
    int member_count;
    bool operator==(const ClusterData & other);
};

#endif //CLUSTER_DATA
