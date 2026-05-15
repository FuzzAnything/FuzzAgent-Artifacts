// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsVEC3init at cmsmtrx.c:34:16 in lcms2_plugin.h
// _cmsVEC3init at cmsmtrx.c:34:16 in lcms2_plugin.h
// _cmsVEC3dot at cmsmtrx.c:58:28 in lcms2_plugin.h
// _cmsVEC3init at cmsmtrx.c:34:16 in lcms2_plugin.h
// _cmsVEC3init at cmsmtrx.c:34:16 in lcms2_plugin.h
// _cmsVEC3distance at cmsmtrx.c:72:28 in lcms2_plugin.h
// _cmsVEC3init at cmsmtrx.c:34:16 in lcms2_plugin.h
// _cmsVEC3init at cmsmtrx.c:34:16 in lcms2_plugin.h
// _cmsVEC3cross at cmsmtrx.c:50:16 in lcms2_plugin.h
// _cmsVEC3init at cmsmtrx.c:34:16 in lcms2_plugin.h
// _cmsVEC3init at cmsmtrx.c:34:16 in lcms2_plugin.h
// _cmsVEC3length at cmsmtrx.c:64:28 in lcms2_plugin.h
// _cmsVEC3init at cmsmtrx.c:34:16 in lcms2_plugin.h
// _cmsVEC3init at cmsmtrx.c:34:16 in lcms2_plugin.h
// _cmsVEC3minus at cmsmtrx.c:42:16 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2_plugin.h"
#include <stdint.h>
#include <string.h>

static void parse_input(const uint8_t *data, size_t size, 
                       cmsFloat64Number *values, size_t max_values) {
    size_t i;
    for (i = 0; i < max_values && i * sizeof(cmsFloat64Number) + sizeof(cmsFloat64Number) <= size; i++) {
        memcpy(&values[i], data + i * sizeof(cmsFloat64Number), sizeof(cmsFloat64Number));
    }
    for (; i < max_values; i++) {
        values[i] = 0.0;
    }
}

int LLVMFuzzerTestOneInput_48(const uint8_t *Data, size_t Size) {
    cmsVEC3 vec1, vec2, vec3, result;
    cmsFloat64Number values[12];
    
    if (Size < 1) return 0;
    
    parse_input(Data, Size, values, 12);
    
    uint8_t selector = Data[0] % 6;
    
    switch (selector) {
        case 0:  // _cmsVEC3dot
            _cmsVEC3init(&vec1, values[0], values[1], values[2]);
            _cmsVEC3init(&vec2, values[3], values[4], values[5]);
            {
                volatile cmsFloat64Number dot_result = _cmsVEC3dot(&vec1, &vec2);
                (void)dot_result;
            }
            break;
            
        case 1:  // _cmsVEC3distance
            _cmsVEC3init(&vec1, values[0], values[1], values[2]);
            _cmsVEC3init(&vec2, values[3], values[4], values[5]);
            {
                volatile cmsFloat64Number dist_result = _cmsVEC3distance(&vec1, &vec2);
                (void)dist_result;
            }
            break;
            
        case 2:  // _cmsVEC3cross
            _cmsVEC3init(&vec1, values[0], values[1], values[2]);
            _cmsVEC3init(&vec2, values[3], values[4], values[5]);
            _cmsVEC3cross(&result, &vec1, &vec2);
            break;
            
        case 3:  // _cmsVEC3init
            _cmsVEC3init(&vec1, values[0], values[1], values[2]);
            break;
            
        case 4:  // _cmsVEC3length
            _cmsVEC3init(&vec1, values[0], values[1], values[2]);
            {
                volatile cmsFloat64Number len_result = _cmsVEC3length(&vec1);
                (void)len_result;
            }
            break;
            
        case 5:  // _cmsVEC3minus
            _cmsVEC3init(&vec1, values[0], values[1], values[2]);
            _cmsVEC3init(&vec2, values[3], values[4], values[5]);
            _cmsVEC3minus(&result, &vec1, &vec2);
            break;
    }
    
    return 0;
}