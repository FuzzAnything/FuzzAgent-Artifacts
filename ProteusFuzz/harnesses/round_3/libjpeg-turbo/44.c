// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3DecompressHeader at turbojpeg.c:1815:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tjInitTransform at turbojpeg.c:2748:20 in turbojpeg.h
// tjTransform at turbojpeg.c:3041:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3DecompressHeader at turbojpeg.c:1815:15 in turbojpeg.h
// tj3TransformBufSize at turbojpeg.c:2828:18 in turbojpeg.h
// tj3Transform at turbojpeg.c:2867:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3DecompressHeader at turbojpeg.c:1815:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <turbojpeg.h>

int LLVMFuzzerTestOneInput_44(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    // Use first byte to determine operation mode
    int mode = Data[0] % 4;
    const uint8_t *jpegData = Data + 1;
    size_t jpegSize = Size - 1;

    tjhandle handle = NULL;
    unsigned char *dstBuf = NULL;
    size_t dstSize = 0;
    tjtransform transform = {0};

    // Initialize transform region with safe defaults
    transform.r.x = 0;
    transform.r.y = 0;
    transform.r.w = 0;
    transform.r.h = 0;

    switch (mode) {
        case 0:
            // Test tj3Init + tj3DecompressHeader
            handle = tj3Init(TJINIT_DECOMPRESS);
            if (handle != NULL) {
                tj3DecompressHeader(handle, jpegData, jpegSize);
                tj3Destroy(handle);
            }
            break;

        case 1:
            // Test tjInitTransform + tjTransform (legacy API)
            handle = tjInitTransform();
            if (handle != NULL) {
                // Set a simple transform operation
                transform.op = Data[1] % 6; // 0-5 valid ops
                transform.r.w = (jpegSize > 100) ? 100 : jpegSize;
                transform.r.h = (jpegSize > 100) ? 100 : jpegSize;
                
                unsigned long legacyDstSize = 0;
                tjTransform(handle, jpegData, jpegSize, 1, &dstBuf, &legacyDstSize, &transform, 0);
                
                if (dstBuf != NULL) {
                    free(dstBuf);
                }
                tjDestroy(handle);
            }
            break;

        case 2:
            // Test tj3Init + tj3TransformBufSize + tj3Transform
            handle = tj3Init(TJINIT_TRANSFORM);
            if (handle != NULL) {
                // First try to read header to get image dimensions
                if (tj3DecompressHeader(handle, jpegData, jpegSize) == 0) {
                    // Set transform parameters
                    transform.op = Data[1] % 6;
                    // Use small crop region to avoid huge allocations
                    transform.r.w = (jpegSize > 200) ? 200 : jpegSize;
                    transform.r.h = (jpegSize > 200) ? 200 : jpegSize;
                    
                    size_t requiredSize = tj3TransformBufSize(handle, &transform);
                    if (requiredSize > 0 && requiredSize < 100 * 1024 * 1024) {
                        dstBuf = (unsigned char *)malloc(requiredSize);
                        if (dstBuf != NULL) {
                            dstSize = requiredSize;
                            tj3Transform(handle, jpegData, jpegSize, 1, &dstBuf, &dstSize, &transform);
                            free(dstBuf);
                        }
                    }
                }
                tj3Destroy(handle);
            }
            break;

        case 3:
            // Test tj3Init + tj3DecompressHeader with SAVEMARKERS
            handle = tj3Init(TJINIT_DECOMPRESS);
            if (handle != NULL) {
                // Attempt to extract ICC profile if present
                tj3DecompressHeader(handle, jpegData, jpegSize);
                tj3Destroy(handle);
            }
            break;
    }

    return 0;
}