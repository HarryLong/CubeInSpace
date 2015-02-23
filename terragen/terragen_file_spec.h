#ifndef TERRAGEN_FILE_SPEC_H
#define TERRAGEN_FILE_SPEC_H

struct TerragenFileHeaderData{
    int width, depth;
    float scale, height_scale, base_height, min_height, max_height;
};

class TerragenFile
{
public:
    void summarize();

    float & operator()(int x, int y){
        return m_height_data[y * m_header_data.width + x];
    }

    TerragenFileHeaderData m_header_data;
    float* m_height_data;
};

#endif // TERRAGEN_FILE_SPEC_H
