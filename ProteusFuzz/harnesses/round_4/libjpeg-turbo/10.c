// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3Transform at turbojpeg.c:2867:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3DecompressHeader at turbojpeg.c:1815:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3DecompressHeader at turbojpeg.c:1815:15 in turbojpeg.h
// tj3TransformBufSize at turbojpeg.c:2828:18 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <turbojpeg.h>

int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    // Use first 4 bytes to determine test scenario
    uint32_t seed = 0;
    memcpy(&seed, Data, sizeof(uint32_t));
    int scenario = seed % 4;

    tjhandle handle = NULL;
    unsigned char *jpegBuf = NULL;
    unsigned char *dstBuf = NULL;
    size_t jpegSize = 0;
    size_t dstSize = 0;
    int ret = 0;

    // Allocate buffer for input data if needed
    if (Size > sizeof(uint32_t)) {
        jpegSize = Size - sizeof(uint32_t);
        jpegBuf = (unsigned char *)malloc(jpegSize);
        if (!jpegBuf) return 0;
        memcpy(jpegBuf, Data + sizeof(uint32_t), jpegSize);
    }

    switch (scenario) {
        case 0: {
            // Test tj3Init, tj3Set, tj3Get
            handle = tj3Init(TJINIT_DECOMPRESS);
            if (handle) {
                // Try setting various parameters
                int params[] = {TJPARAM_QUALITY, TJPARAM_SUBSAMP, TJPARAM_NOREALLOC};
                for (int i = 0; i < 3; i++) {
                    int val = (seed >> (8 * i)) & 0xFF;
                    tj3Set(handle, params[i], val);
                }
                // Get parameters
                int getParams[] = {TJPARAM_QUALITY, TJPARAM_SUBSAMP, TJPARAM_NOREALLOC};
                for (int i = 0; i < 3; i++) {
                    tj3Get(handle, getParams[i]);
                }
                tj3Destroy(handle);
                handle = NULL;
            }
            break;
        }
        case 1: {
            // Test tj3Init (compress), tj3Set, tj3Get
            handle = tj3Init(TJINIT_COMPRESS);
            if (handle) {
                int params[] = {TJPARAM_QUALITY, TJPARAM_SUBSAMP, TJPARAM_NOREALLOC};
                for (int i = 0; i < 3; i++) {
                    int val = (seed >> (8 * i)) & 0xFF;
                    tj3Set(handle, params[i], val);
                }
                int getParams[] = {TJPARAM_QUALITY, TJPARAM_SUBSAMP, TJPARAM_NOREALLOC};
                for (int i = 0; i < 3; i++) {
                    tj3Get(handle, getParams[i]);
                }
                tj3Destroy(handle);
                handle = NULL;
            }
            break;
        }
        case 2: {
            // Test tj3Init (decompress), tj3DecompressHeader
            handle = tj3Init(TJINIT_DECOMPRESS);
            if (handle && jpegBuf && jpegSize > 0) {
                // Try to set SAVEMARKERS to extract ICC profile
                tj3Set(handle, TJPARAM_SAVEMARKERS, 2);
                ret = tj3DecompressHeader(handle, jpegBuf, jpegSize);
                (void)ret;
            }
            if (handle) {
                tj3Destroy(handle);
                handle = NULL;
            }
            break;
        }
        case 3: {
            // Test tj3Init (transform), tj3DecompressHeader, tj3TransformBufSize, tj3Transform
            handle = tj3Init(TJINIT_TRANSFORM);
            if (handle && jpegBuf && jpegSize > 0) {
                ret = tj3DecompressHeader(handle, jpegBuf, jpegSize);
                if (ret == 0) {
                    tjtransform transform;
                    memset(&transform, 0, sizeof(tjtransform));
                    transform.op = (seed % 6); // TJXOP_NONE to TJXOP_TRANSPOSE
                    transform.options = (seed >> 8) & 0xFF;
                    
                    size_t bufSize = tj3TransformBufSize(handle, &transform);
                    if (bufSize > 0) {
                        dstBuf = (unsigned char *)malloc(bufSize);
                        if (dstBuf) {
                            dstSize = bufSize;
                            tj3Transform(handle, jpegBuf, jpegSize, 1, &dstBuf, &dstSize, &transform);
                            free(dstBuf);
                            dstBuf = NULL;
                        }
                    }
                }
            }
            if (handle) {
                tj3Destroy(handle);
                handle = NULL;
            }
            break;
        }
    }

    if (jpegBuf) {
        free(jpegBuf);
    }
    return 0;
}