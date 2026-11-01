// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSwabArrayOfTriples at tif_swab.c:99:6 in tiffio.h
// TIFFSwabArrayOfDouble at tif_swab.c:222:6 in tiffio.h
// TIFFSwabArrayOfLong at tif_swab.c:117:6 in tiffio.h
// TIFFSwabArrayOfFloat at tif_swab.c:180:6 in tiffio.h
// TIFFSwabLong8 at tif_swab.c:60:6 in tiffio.h
// TIFFSwabArrayOfLong8 at tif_swab.c:138:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <tiffio.h>

static void safe_free(void *ptr) {
    if (ptr) {
        free(ptr);
    }
}

int LLVMFuzzerTestOneInput_87(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    // Extract count from the first two bytes to avoid huge allocations
    uint16_t count = (Data[0] << 8) | Data[1];
    if (count > 10000) {
        count = 10000;
    }
    if (count == 0) {
        return 0;
    }

    size_t offset = 2;
    size_t remaining = Size - offset;

    // Fuzz TIFFSwabArrayOfTriples
    // Requires 3 bytes per element
    if (remaining >= count * 3) {
        uint8_t *triples = malloc(count * 3);
        if (triples) {
            memcpy(triples, Data + offset, count * 3);
            TIFFSwabArrayOfTriples(triples, count);
            safe_free(triples);
        }
    }

    // Fuzz TIFFSwabArrayOfDouble
    // Requires 8 bytes per element
    if (remaining >= count * sizeof(double)) {
        double *doubles = malloc(count * sizeof(double));
        if (doubles) {
            memcpy(doubles, Data + offset, count * sizeof(double));
            TIFFSwabArrayOfDouble(doubles, count);
            safe_free(doubles);
        }
    }

    // Fuzz TIFFSwabArrayOfLong
    // Requires 4 bytes per element
    if (remaining >= count * sizeof(uint32_t)) {
        uint32_t *longs = malloc(count * sizeof(uint32_t));
        if (longs) {
            memcpy(longs, Data + offset, count * sizeof(uint32_t));
            TIFFSwabArrayOfLong(longs, count);
            safe_free(longs);
        }
    }

    // Fuzz TIFFSwabArrayOfFloat
    // Requires 4 bytes per element
    if (remaining >= count * sizeof(float)) {
        float *floats = malloc(count * sizeof(float));
        if (floats) {
            memcpy(floats, Data + offset, count * sizeof(float));
            TIFFSwabArrayOfFloat(floats, count);
            safe_free(floats);
        }
    }

    // Fuzz TIFFSwabLong8
    // Requires 8 bytes for a single element
    if (remaining >= sizeof(uint64_t)) {
        uint64_t long8;
        memcpy(&long8, Data + offset, sizeof(uint64_t));
        TIFFSwabLong8(&long8);
    }

    // Fuzz TIFFSwabArrayOfLong8
    // Requires 8 bytes per element
    if (remaining >= count * sizeof(uint64_t)) {
        uint64_t *long8s = malloc(count * sizeof(uint64_t));
        if (long8s) {
            memcpy(long8s, Data + offset, count * sizeof(uint64_t));
            TIFFSwabArrayOfLong8(long8s, count);
            safe_free(long8s);
        }
    }

    return 0;
}