#ifndef TERRAGEN_FILE_SPEC_H
#define TERRAGEN_FILE_SPEC_H

struct TerragenFileHeaderData{
    int width, depth;
    float raw_scale, height_scale, base_height, min_height, max_height, dynamic_scale;
};

class TerragenFile
{
public:
    void summarize();

    TerragenFileHeaderData m_header_data;
    float* m_height_data;
};

#endif // TERRAGEN_FILE_SPEC_H
