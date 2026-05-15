// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsVEC3init at cmsmtrx.c:34:16 in lcms2_plugin.h
// _cmsVEC3cross at cmsmtrx.c:50:16 in lcms2_plugin.h
// _cmsVEC3distance at cmsmtrx.c:72:28 in lcms2_plugin.h
// _cmsVEC3minus at cmsmtrx.c:42:16 in lcms2_plugin.h
// _cmsVEC3dot at cmsmtrx.c:58:28 in lcms2_plugin.h
// _cmsVEC3length at cmsmtrx.c:64:28 in lcms2_plugin.h
// _cmsVEC3cross at cmsmtrx.c:50:16 in lcms2_plugin.h
// _cmsVEC3distance at cmsmtrx.c:72:28 in lcms2_plugin.h
// _cmsVEC3minus at cmsmtrx.c:42:16 in lcms2_plugin.h
// _cmsVEC3dot at cmsmtrx.c:58:28 in lcms2_plugin.h
// _cmsVEC3length at cmsmtrx.c:64:28 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "lcms2_plugin.h"

static void parse_input(const uint8_t *data, size_t size, cmsVEC3 *vec1, cmsVEC3 *vec2, cmsFloat64Number *scalars) {
    // Use the input data to populate vectors and scalars
    // We need at least 3 doubles for each vector (24 bytes each) and 3 doubles for scalars
    size_t min_size = 9 * sizeof(cmsFloat64Number);
    if (size < min_size) {
        // If not enough data, use default values
        *scalars = 1.0;
        return;
    }
    
    // Cast input data to doubles for vector components
    const cmsFloat64Number *double_data = (const cmsFloat64Number *)data;
    
    // Fill first vector
    vec1->n[0] = double_data[0];
    vec1->n[1] = double_data[1];
    vec1->n[2] = double_data[2];
    
    // Fill second vector
    vec2->n[0] = double_data[3];
    vec2->n[1] = double_data[4];
    vec2->n[2] = double_data[5];
    
    // Fill scalars for _cmsVEC3init
    scalars[0] = double_data[6];
    scalars[1] = double_data[7];
    scalars[2] = double_data[8];
}

int LLVMFuzzerTestOneInput_55(const uint8_t *Data, size_t Size) {
    cmsVEC3 vec1, vec2, vec3, result;
    cmsFloat64Number scalars[3];
    cmsFloat64Number distance_result, dot_result, length_result;
    
    // Parse input data to fill vectors and scalars
    parse_input(Data, Size, &vec1, &vec2, scalars);
    
    // Test _cmsVEC3init
    _cmsVEC3init(&vec3, scalars[0], scalars[1], scalars[2]);
    
    // Test _cmsVEC3cross
    _cmsVEC3cross(&result, &vec1, &vec2);
    
    // Test _cmsVEC3distance
    distance_result = _cmsVEC3distance(&vec1, &vec2);
    
    // Test _cmsVEC3minus
    _cmsVEC3minus(&result, &vec1, &vec2);
    
    // Test _cmsVEC3dot
    dot_result = _cmsVEC3dot(&vec1, &vec2);
    
    // Test _cmsVEC3length
    length_result = _cmsVEC3length(&vec1);
    
    // Also test with the initialized vector
    _cmsVEC3cross(&result, &vec1, &vec3);
    distance_result = _cmsVEC3distance(&vec2, &vec3);
    _cmsVEC3minus(&result, &vec2, &vec3);
    dot_result = _cmsVEC3dot(&vec2, &vec3);
    length_result = _cmsVEC3length(&vec3);
    
    return 0;
}