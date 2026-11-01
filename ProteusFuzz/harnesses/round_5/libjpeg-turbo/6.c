// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tj3DecompressHeader at turbojpeg.c:1815:15 in turbojpeg.h
// tj3TransformBufSize at turbojpeg.c:2828:18 in turbojpeg.h
// tj3Transform at turbojpeg.c:2867:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <turbojpeg.h>

int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Determine init type based on input byte to explore different states
    int initType = (Data[0] % 3);
    if (initType == 0) initType = TJINIT_COMPRESS;
    else if (initType == 1) initType = TJINIT_DECOMPRESS;
    else initType = TJINIT_TRANSFORM;

    tjhandle handle = tj3Init(initType);
    if (handle == NULL) {
        return 0;
    }

    // Fuzz tj3Set and tj3Get
    // Define a static array of valid parameters to avoid compilation errors
    static const int params[] = {
        TJPARAM_QUALITY, 
        TJPARAM_SUBSAMP, 
        TJPARAM_PROGRESSIVE, 
        TJPARAM_ARITHMETIC, 
        TJPARAM_NOREALLOC, 
        TJPARAM_SAVEMARKERS
    };
    const int numParams = sizeof(params) / sizeof(params[0]);
    
    for (int i = 0; i < numParams; i++) {
        int param = params[i];
        // Fuzz value from input data
        int value = 0;
        if (Size > (size_t)(i + 1)) {
            value = (int)Data[i + 1];
        }
        tj3Set(handle, param, value);
        
        int retrieved = tj3Get(handle, param);
        (void)retrieved; // Use return value to ensure state is checked
    }

    // Prepare data for header decompression and transform
    // We need a valid JPEG buffer. Since we are fuzzing, we pass the input data
    // which might be invalid, triggering error paths.
    const unsigned char *jpegBuf = Data;
    size_t jpegSize = Size;

    // If we initialized for decompress or transform, try to read header
    if (initType == TJINIT_DECOMPRESS || initType == TJINIT_TRANSFORM) {
        // Ensure we have at least a few bytes to avoid trivial empty checks if library is strict
        if (jpegSize > 0) {
            tj3DecompressHeader(handle, jpegBuf, jpegSize);
        }
    }

    // Prepare transform structure
    tjtransform transform = {0};
    transform.op = (Size > 1) ? (int)Data[1] % 7 : 0; // TJXOP_NONE to TJXOP_TRANSPOSE
    transform.options = (Size > 2) ? (int)Data[2] : 0;
    
    // Fuzz crop region if data allows
    if (Size > 6) {
        transform.r.x = (int)Data[3];
        transform.r.y = (int)Data[4];
        transform.r.w = (int)Data[5];
        transform.r.h = (int)Data[6];
    }

    // Fuzz tj3TransformBufSize
    if (initType == TJINIT_TRANSFORM) {
        size_t bufSize = tj3TransformBufSize(handle, &transform);
        (void)bufSize;
    }

    // Fuzz tj3Transform
    // We need an array of transforms and buffers
    int n = 1;
    unsigned char *dstBufs[1] = {NULL};
    size_t dstSizes[1] = {0};
    tjtransform transforms[1];
    transforms[0] = transform;

    if (initType == TJINIT_TRANSFORM && jpegSize > 0) {
        tj3Transform(handle, jpegBuf, jpegSize, n, dstBufs, dstSizes, transforms);
        
        // Cleanup allocated buffer if any
        if (dstBufs[0] != NULL) {
            tj3Free(dstBufs[0]);
        }
    }

    // Cleanup
    tj3Destroy(handle);

    return 0;
}