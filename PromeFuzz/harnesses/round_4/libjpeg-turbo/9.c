// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tj3SetScalingFactor at turbojpeg.c:1981:15 in turbojpeg.h
// tj3SetCroppingRegion at turbojpeg.c:2006:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "turbojpeg.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int read_int(const uint8_t **data, size_t *size, size_t bytes) {
    if (*size < bytes) {
        return 0;
    }
    int value = 0;
    for (size_t i = 0; i < bytes; i++) {
        value = (value << 8) | (*data)[i];
    }
    *data += bytes;
    *size -= bytes;
    return value;
}

int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    const uint8_t *data = Data;
    size_t remaining = Size;

    // Initialize a TurboJPEG instance for decompression
    tjhandle handle = tj3Init(TJINIT_DECOMPRESS);
    if (!handle) {
        return 0;
    }

    // Use some fuzzer data to set parameters
    int param1 = read_int(&data, &remaining, 1) % 10;  // Random parameter
    int value1 = read_int(&data, &remaining, 1);
    tj3Set(handle, param1, value1);

    // Try to get a parameter value
    int param2 = read_int(&data, &remaining, 1) % 10;
    int get_result = tj3Get(handle, param2);
    (void)get_result;  // Use result to avoid unused variable warning

    // Set scaling factor if we have enough data
    if (remaining >= 2) {
        tjscalingfactor scaling_factor;
        scaling_factor.num = read_int(&data, &remaining, 1);
        scaling_factor.denom = read_int(&data, &remaining, 1);
        tj3SetScalingFactor(handle, scaling_factor);
    }

    // Set cropping region if we have enough data
    if (remaining >= 8) {
        tjregion cropping_region;
        cropping_region.x = read_int(&data, &remaining, 2);
        cropping_region.y = read_int(&data, &remaining, 2);
        cropping_region.w = read_int(&data, &remaining, 2);
        cropping_region.h = read_int(&data, &remaining, 2);
        tj3SetCroppingRegion(handle, cropping_region);
    }

    // Clean up
    tjDestroy(handle);

    // Create another handle for compression
    tjhandle handle2 = tj3Init(TJINIT_COMPRESS);
    if (handle2) {
        // Set some parameters on compression handle
        int param3 = read_int(&data, &remaining, 1) % 10;
        int value3 = read_int(&data, &remaining, 1);
        tj3Set(handle2, param3, value3);

        // Get a parameter from compression handle
        int param4 = read_int(&data, &remaining, 1) % 10;
        int get_result2 = tj3Get(handle2, param4);
        (void)get_result2;

        // Destroy compression handle
        tjDestroy(handle2);
    }

    return 0;
}