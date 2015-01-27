/**
 * @file
 *
 * Terragen format import and export.
 */

#ifndef UTS_COMMON_TERRAGEN_H
#define UTS_COMMON_TERRAGEN_H

#include <string>
#include "terragen_file_spec.h"

/**
 * Load a Terragen file.
 */
TerragenFile readTerragen(const std::string &filename, int terrain_dimension);

/**
 * Save a Terragen file.
 */
//void writeTerragen(const uts::string &filename, const MemMap<height_tag> &map, const Region &region);

#endif /* !UTS_COMMON_TERRAGEN_H */
