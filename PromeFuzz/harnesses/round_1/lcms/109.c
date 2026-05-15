// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsAdjustEndianess16 at cmsplugin.c:37:28 in lcms2_plugin.h
// cmsXYZEncoded2Float at cmspcs.c:429:16 in lcms2.h
// cmsLabEncoded2Float at cmspcs.c:226:16 in lcms2.h
// cmsLabEncoded2FloatV2 at cmspcs.c:218:16 in lcms2.h
// cmsFloat2LabEncoded at cmspcs.c:298:16 in lcms2.h
// cmsFloat2XYZEncoded at cmspcs.c:374:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include "lcms2_plugin.h"
#include <stdint.h>
#include <string.h>

static void initialize_CIEXYZ(cmsCIEXYZ* xyz, float x, float y, float z) {
    xyz->X = x;
    xyz->Y = y;
    xyz->Z = z;
}

static void initialize_CIELab(cmsCIELab* lab, float L, float a, float b) {
    lab->L = L;
    lab->a = a;
    lab->b = b;
}

int LLVMFuzzerTestOneInput_109(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;
    
    // Test _cmsAdjustEndianess16
    cmsUInt16Number word = (cmsUInt16Number)((Data[0] << 8) | Data[1]);
    cmsUInt16Number adjusted = _cmsAdjustEndianess16(word);
    (void)adjusted;  // Prevent unused variable warning
    
    // Prepare data for other functions
    cmsUInt16Number encoded[3];
    cmsCIEXYZ fxyz;
    cmsCIELab lab;
    
    // Initialize encoded array with data from fuzzer input
    if (Size >= 8) {
        for (int i = 0; i < 3; i++) {
            size_t offset = 2 + i * 2;
            if (offset + 1 < Size) {
                encoded[i] = (cmsUInt16Number)((Data[offset] << 8) | Data[offset + 1]);
            } else {
                encoded[i] = 0;
            }
        }
        
        // Test cmsXYZEncoded2Float
        cmsXYZEncoded2Float(&fxyz, encoded);
        
        // Test cmsLabEncoded2Float
        cmsLabEncoded2Float(&lab, encoded);
        
        // Test cmsLabEncoded2FloatV2
        cmsLabEncoded2FloatV2(&lab, encoded);
    }
    
    // Test cmsFloat2LabEncoded and cmsFloat2XYZEncoded
    if (Size >= 12) {
        // Extract float values from fuzzer input (using first 12 bytes as raw float data)
        float float_data[3];
        memcpy(float_data, Data, (Size < 12) ? Size : 12);
        
        // Test cmsFloat2LabEncoded
        initialize_CIELab(&lab, float_data[0], float_data[1], float_data[2]);
        cmsFloat2LabEncoded(encoded, &lab);
        
        // Test cmsFloat2XYZEncoded
        initialize_CIEXYZ(&fxyz, float_data[0], float_data[1], float_data[2]);
        cmsFloat2XYZEncoded(encoded, &fxyz);
    }
    
    return 0;
}