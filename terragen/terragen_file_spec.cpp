#include "terragen_file_spec.h"
#include <iostream>

TerragenFile::TerragenFile() : m_height_data(NULL)
{

}

TerragenFile::~TerragenFile()
{

}

void TerragenFile::summarize()
{
    std::cout << "***** TERRAGEN FILE SUMMARY *****" << std::endl;
    std::cout << "Width: " << m_header_data.width << std::endl;
    std::cout << "Height: " << m_header_data.depth << std::endl;
    std::cout << "Minimum height: " << m_header_data.min_height << std::endl;
    std::cout << "Max height: " << m_header_data.max_height << std::endl;
    std::cout << "Raw Scale: " << m_header_data.scale << std::endl;
    std::cout << "HeightScale: " << m_header_data.height_scale << std::endl;
    std::cout << "BaseHeight: " << m_header_data.base_height << std::endl;
    std::cout << "*********************************" << std::endl;
}
