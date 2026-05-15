// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSwabArrayOfLong at tif_swab.c:117:6 in tiffio.h
// TIFFSwabArrayOfLong8 at tif_swab.c:138:6 in tiffio.h
// TIFFSwabArrayOfFloat at tif_swab.c:180:6 in tiffio.h
// TIFFSwabArrayOfShort at tif_swab.c:81:6 in tiffio.h
// TIFFSwabFloat at tif_swab.c:165:6 in tiffio.h
// TIFFSwabArrayOfDouble at tif_swab.c:222:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int LLVMFuzzerTestOneInput_73(const uint8_t *Data, size_t Size) {
    if (Size < 8) {
        return 0;
    }

    // For array functions, we'll use part of the input as array data
    // and part to determine array size
    size_t max_elements = 1024;
    size_t element_size;
    tmsize_t n;
    
    // TIFFSwabArrayOfLong8
    element_size = sizeof(uint64_t);
    if (Size >= element_size) {
        size_t array_size = Size % (max_elements * element_size);
        if (array_size > 0) {
            n = (tmsize_t)(array_size / element_size);
            if (n > 0) {
                uint64_t *arr = (uint64_t *)malloc(array_size);
                if (arr) {
                    memcpy(arr, Data, array_size);
                    TIFFSwabArrayOfLong8(arr, n);
                    free(arr);
                }
            }
        }
    }
    
    // TIFFSwabArrayOfFloat
    element_size = sizeof(float);
    if (Size >= element_size) {
        size_t array_size = Size % (max_elements * element_size);
        if (array_size > 0) {
            n = (tmsize_t)(array_size / element_size);
            if (n > 0) {
                float *arr = (float *)malloc(array_size);
                if (arr) {
                    memcpy(arr, Data, array_size);
                    TIFFSwabArrayOfFloat(arr, n);
                    free(arr);
                }
            }
        }
    }
    
    // TIFFSwabArrayOfShort
    element_size = sizeof(uint16_t);
    if (Size >= element_size) {
        size_t array_size = Size % (max_elements * element_size);
        if (array_size > 0) {
            n = (tmsize_t)(array_size / element_size);
            if (n > 0) {
                uint16_t *arr = (uint16_t *)malloc(array_size);
                if (arr) {
                    memcpy(arr, Data, array_size);
                    TIFFSwabArrayOfShort(arr, n);
                    free(arr);
                }
            }
        }
    }
    
    // TIFFSwabFloat
    if (Size >= sizeof(float)) {
        float f;
        memcpy(&f, Data, sizeof(float));
        TIFFSwabFloat(&f);
    }
    
    // TIFFSwabArrayOfDouble
    element_size = sizeof(double);
    if (Size >= element_size) {
        size_t array_size = Size % (max_elements * element_size);
        if (array_size > 0) {
            n = (tmsize_t)(array_size / element_size);
            if (n > 0) {
                double *arr = (double *)malloc(array_size);
                if (arr) {
                    memcpy(arr, Data, array_size);
                    TIFFSwabArrayOfDouble(arr, n);
                    free(arr);
                }
            }
        }
    }
    
    // TIFFSwabArrayOfLong
    element_size = sizeof(uint32_t);
    if (Size >= element_size) {
        size_t array_size = Size % (max_elements * element_size);
        if (array_size > 0) {
            n = (tmsize_t)(array_size / element_size);
            if (n > 0) {
                uint32_t *arr = (uint32_t *)malloc(array_size);
                if (arr) {
                    memcpy(arr, Data, array_size);
                    TIFFSwabArrayOfLong(arr, n);
                    free(arr);
                }
            }
        }
    }
    
    return 0;
}