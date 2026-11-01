// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSwabShort at tif_swab.c:33:6 in tiffio.h
// TIFFSwabArrayOfShort at tif_swab.c:81:6 in tiffio.h
// TIFFSwabArrayOfLong at tif_swab.c:117:6 in tiffio.h
// TIFFSwabArrayOfFloat at tif_swab.c:180:6 in tiffio.h
// TIFFSwabArrayOfDouble at tif_swab.c:222:6 in tiffio.h
// TIFFSwabArrayOfTriples at tif_swab.c:99:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <tiffio.h>

static void fuzz_swab_short(uint16_t *data, size_t count) {
    if (count == 0) return;
    TIFFSwabShort(data);
    TIFFSwabArrayOfShort(data, count);
}

static void fuzz_swab_long(uint32_t *data, size_t count) {
    if (count == 0) return;
    TIFFSwabArrayOfLong(data, count);
}

static void fuzz_swab_float(float *data, size_t count) {
    if (count == 0) return;
    TIFFSwabArrayOfFloat(data, count);
}

static void fuzz_swab_double(double *data, size_t count) {
    if (count == 0) return;
    TIFFSwabArrayOfDouble(data, count);
}

static void fuzz_swab_triples(uint8_t *data, size_t count) {
    if (count == 0) return;
    TIFFSwabArrayOfTriples(data, count);
}

int LLVMFuzzerTestOneInput_69(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    // Extract a count from the input to determine array sizes
    // Ensure count is not too large to avoid allocation issues
    uint32_t count = *(const uint32_t *)Data;
    if (count > Size / 8) {
        count = Size / 8;
    }
    if (count == 0) {
        count = 1;
    }

    // Allocate buffers based on the extracted count
    uint16_t *short_buf = (uint16_t *)malloc(count * sizeof(uint16_t));
    uint32_t *long_buf = (uint32_t *)malloc(count * sizeof(uint32_t));
    float *float_buf = (float *)malloc(count * sizeof(float));
    double *double_buf = (double *)malloc(count * sizeof(double));
    uint8_t *triple_buf = (uint8_t *)malloc(count * 3 * sizeof(uint8_t));

    if (!short_buf || !long_buf || !float_buf || !double_buf || !triple_buf) {
        free(short_buf);
        free(long_buf);
        free(float_buf);
        free(double_buf);
        free(triple_buf);
        return 0;
    }

    // Initialize buffers with input data or random patterns if input is insufficient
    size_t available_data = Size - 4;
    size_t copy_len = available_data;

    // Copy input data to buffers safely
    if (copy_len > 0) {
        memcpy(short_buf, Data + 4, (count * sizeof(uint16_t) < copy_len) ? count * sizeof(uint16_t) : copy_len);
        memcpy(long_buf, Data + 4, (count * sizeof(uint32_t) < copy_len) ? count * sizeof(uint32_t) : copy_len);
        memcpy(float_buf, Data + 4, (count * sizeof(float) < copy_len) ? count * sizeof(float) : copy_len);
        memcpy(double_buf, Data + 4, (count * sizeof(double) < copy_len) ? count * sizeof(double) : copy_len);
        memcpy(triple_buf, Data + 4, (count * 3 < copy_len) ? count * 3 : copy_len);
    }

    // Fuzz TIFFSwabShort (single value)
    if (count > 0) {
        fuzz_swab_short(short_buf, count);
    }

    // Fuzz TIFFSwabArrayOfLong
    fuzz_swab_long(long_buf, count);

    // Fuzz TIFFSwabArrayOfFloat
    fuzz_swab_float(float_buf, count);

    // Fuzz TIFFSwabArrayOfDouble
    fuzz_swab_double(double_buf, count);

    // Fuzz TIFFSwabArrayOfTriples
    fuzz_swab_triples(triple_buf, count);

    // Cleanup
    free(short_buf);
    free(long_buf);
    free(float_buf);
    free(double_buf);
    free(triple_buf);

    return 0;
}