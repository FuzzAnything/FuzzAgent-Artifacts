// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3TransformBufSize at turbojpeg.c:2828:18 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3DecompressHeader at turbojpeg.c:1815:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3DecompressHeader at turbojpeg.c:1815:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tjInitTransform at turbojpeg.c:2748:20 in turbojpeg.h
// tjTransform at turbojpeg.c:3041:15 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Transform at turbojpeg.c:2867:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3DecompressHeader at turbojpeg.c:1815:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <turbojpeg.h>

int LLVMFuzzerTestOneInput_39(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    tjhandle handle = NULL;
    tjhandle transformHandle = NULL;
    unsigned char *dstBuf = NULL;
    size_t dstSize = 0;
    tjtransform transform;
    
    // Initialize transform structure
    memset(&transform, 0, sizeof(transform));
    transform.r.w = 0;
    transform.r.h = 0;
    transform.r.x = 0;
    transform.r.y = 0;
    transform.op = 0;
    transform.options = 0;
    transform.customFilter = NULL;
    transform.data = NULL;

    // Determine operation type from input data
    int opType = Data[0] % 5;
    int initType = (Data[1] % 2) ? TJINIT_COMPRESS : TJINIT_DECOMPRESS;

    switch (opType) {
        case 0:
            // Test tj3Init and tj3DecompressHeader
            handle = tj3Init(initType);
            if (handle) {
                tj3DecompressHeader(handle, Data, Size);
                tj3Destroy(handle);
                handle = NULL;
            }
            break;

        case 1:
            // Test tjInitTransform and tjTransform (legacy API)
            transformHandle = tjInitTransform();
            if (transformHandle) {
                // Set transform parameters based on input
                transform.op = (Data[0] % 8); // 0-7 valid ops
                transform.options = (Data[1] % 4); // 0-3 valid options
                
                // Ensure cropping region is within bounds if data is large enough
                if (Size > 4) {
                    transform.r.w = (Data[2] % 100) + 1;
                    transform.r.h = (Data[3] % 100) + 1;
                    transform.r.x = 0;
                    transform.r.y = 0;
                }

                tjTransform(transformHandle, Data, Size, 1, &dstBuf, &dstSize, &transform, 0);
                if (dstBuf) {
                    tjFree(dstBuf);
                    dstBuf = NULL;
                }
                tjDestroy(transformHandle);
                transformHandle = NULL;
            }
            break;

        case 2:
            // Test tj3Init (transform) and tj3Transform
            handle = tj3Init(TJINIT_TRANSFORM);
            if (handle) {
                // Set transform parameters
                transform.op = (Data[0] % 8);
                transform.options = (Data[1] % 4);
                
                if (Size > 4) {
                    transform.r.w = (Data[2] % 100) + 1;
                    transform.r.h = (Data[3] % 100) + 1;
                    transform.r.x = 0;
                    transform.r.y = 0;
                }

                // Try tj3Transform with NULL dstBuf to let it allocate
                tj3Transform(handle, Data, Size, 1, &dstBuf, &dstSize, &transform);
                if (dstBuf) {
                    tj3Free(dstBuf);
                    dstBuf = NULL;
                }
                tj3Destroy(handle);
                handle = NULL;
            }
            break;

        case 3:
            // Test tj3Init, tj3DecompressHeader, and tj3TransformBufSize
            handle = tj3Init(TJINIT_TRANSFORM);
            if (handle) {
                tj3DecompressHeader(handle, Data, Size);
                
                // Set transform parameters
                transform.op = (Data[0] % 8);
                transform.options = (Data[1] % 4);
                
                if (Size > 4) {
                    transform.r.w = (Data[2] % 100) + 1;
                    transform.r.h = (Data[3] % 100) + 1;
                    transform.r.x = 0;
                    transform.r.y = 0;
                }

                size_t bufSize = tj3TransformBufSize(handle, &transform);
                if (bufSize > 0) {
                    unsigned char *tempBuf = tj3Alloc(bufSize);
                    if (tempBuf) {
                        tj3Free(tempBuf);
                    }
                }
                tj3Destroy(handle);
                handle = NULL;
            }
            break;

        case 4:
            // Test error handling with invalid inputs
            handle = tj3Init(TJINIT_TRANSFORM);
            if (handle) {
                // Pass NULL buffer
                tj3DecompressHeader(handle, NULL, 0);
                tj3Destroy(handle);
                handle = NULL;
            }
            break;
    }

    return 0;
}