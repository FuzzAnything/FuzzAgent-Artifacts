// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsD50_XYZ at cmswtpnt.c:31:28 in lcms2.h
// cmsXYZ2Lab at cmspcs.c:143:16 in lcms2.h
// cmsXYZ2Lab at cmspcs.c:143:16 in lcms2.h
// cmsXYZ2xyY at cmspcs.c:91:16 in lcms2.h
// cmsAdaptToIlluminant at cmswtpnt.c:328:19 in lcms2.h
// cmsLab2XYZ at cmspcs.c:161:16 in lcms2.h
// cmsLab2XYZ at cmspcs.c:161:16 in lcms2.h
// cmsxyY2XYZ at cmspcs.c:102:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <string.h>

static void fill_struct_from_data(void* dest, const uint8_t* data, size_t size, size_t struct_size) {
    size_t copy_size = (size < struct_size) ? size : struct_size;
    memcpy(dest, data, copy_size);
    if (copy_size < struct_size) {
        memset((uint8_t*)dest + copy_size, 0, struct_size - copy_size);
    }
}

int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(double) * 3) {
        return 0;
    }

    cmsCIEXYZ xyz1, xyz2, xyz3, whitePoint, resultXYZ;
    cmsCIELab lab1, lab2;
    cmsCIExyY xyY1, xyY2;
    cmsBool adaptResult;

    fill_struct_from_data(&xyz1, Data, Size, sizeof(cmsCIEXYZ));
    fill_struct_from_data(&xyz2, Data + sizeof(cmsCIEXYZ), 
                         (Size > sizeof(cmsCIEXYZ)) ? Size - sizeof(cmsCIEXYZ) : 0, 
                         sizeof(cmsCIEXYZ));
    fill_struct_from_data(&xyz3, Data + 2 * sizeof(cmsCIEXYZ),
                         (Size > 2 * sizeof(cmsCIEXYZ)) ? Size - 2 * sizeof(cmsCIEXYZ) : 0,
                         sizeof(cmsCIEXYZ));

    fill_struct_from_data(&lab1, Data, Size, sizeof(cmsCIELab));
    fill_struct_from_data(&lab2, Data + sizeof(cmsCIELab),
                         (Size > sizeof(cmsCIELab)) ? Size - sizeof(cmsCIELab) : 0,
                         sizeof(cmsCIELab));

    fill_struct_from_data(&xyY1, Data, Size, sizeof(cmsCIExyY));
    fill_struct_from_data(&xyY2, Data + sizeof(cmsCIExyY),
                         (Size > sizeof(cmsCIExyY)) ? Size - sizeof(cmsCIExyY) : 0,
                         sizeof(cmsCIExyY));

    const cmsCIEXYZ* d50 = cmsD50_XYZ();

    cmsXYZ2Lab(d50, &lab1, &xyz1);
    cmsXYZ2Lab(NULL, &lab2, &xyz2);

    cmsXYZ2xyY(&xyY1, &xyz1);

    adaptResult = cmsAdaptToIlluminant(&resultXYZ, &xyz1, &xyz2, &xyz3);

    cmsLab2XYZ(d50, &xyz3, &lab1);
    cmsLab2XYZ(NULL, &xyz2, &lab2);

    cmsxyY2XYZ(&xyz1, &xyY1);

    return 0;
}