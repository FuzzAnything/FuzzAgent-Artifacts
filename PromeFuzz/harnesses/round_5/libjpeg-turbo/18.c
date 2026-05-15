// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3DecompressHeader at turbojpeg.c:1815:15 in turbojpeg.h
// tjGetErrorCode at turbojpeg.c:652:15 in turbojpeg.h
// tj3GetICCProfile at turbojpeg.c:1926:15 in turbojpeg.h
// tj3SetICCProfile at turbojpeg.c:1164:15 in turbojpeg.h
// tj3TransformBufSize at turbojpeg.c:2828:18 in turbojpeg.h
// tj3Transform at turbojpeg.c:2867:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "turbojpeg.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int customFilter(short *coeffs, tjregion arrayRegion,
                       tjregion planeRegion, int componentID, int transformID,
                       struct tjtransform *transform) {
    // Simple custom filter that does nothing
    (void)coeffs;
    (void)arrayRegion;
    (void)planeRegion;
    (void)componentID;
    (void)transformID;
    (void)transform;
    return 0;
}

int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    if (Size < 100) {
        return 0;
    }

    // Initialize handles
    tjhandle decompHandle = tj3Init(TJINIT_DECOMPRESS);
    tjhandle transHandle = tj3Init(TJINIT_TRANSFORM);
    if (!decompHandle || !transHandle) {
        if (decompHandle) tj3Destroy(decompHandle);
        if (transHandle) tj3Destroy(transHandle);
        return 0;
    }

    // Set parameters to extract ICC profile
    tj3Set(decompHandle, TJPARAM_SAVEMARKERS, 2);

    // Test tj3DecompressHeader
    int headerResult = tj3DecompressHeader(decompHandle, Data, Size);
    
    // Test tjGetErrorCode
    int errorCode = tjGetErrorCode(decompHandle);
    (void)errorCode;

    // Test tj3GetICCProfile
    unsigned char *iccBuf = NULL;
    size_t iccSize = 0;
    if (headerResult == 0) {
        tj3GetICCProfile(decompHandle, &iccBuf, &iccSize);
    }

    // Test tj3SetICCProfile
    if (Size > 200) {
        // Use part of the input as ICC profile
        size_t profileSize = Size % 100 + 50;
        if (profileSize < Size) {
            tj3SetICCProfile(transHandle, (unsigned char*)Data, profileSize);
        }
    }

    // Test tj3Transform with dummy transform
    tjtransform transform;
    transform.r.x = 0;
    transform.r.y = 0;
    transform.r.w = 100;
    transform.r.h = 100;
    transform.op = TJXOP_NONE;
    transform.options = 0;
    transform.data = NULL;
    transform.customFilter = customFilter;

    // Test tj3TransformBufSize (requires valid header)
    if (headerResult == 0) {
        size_t bufSize = tj3TransformBufSize(transHandle, &transform);
        (void)bufSize;
    }

    // Test tj3Transform
    unsigned char *dstBuf = NULL;
    size_t dstSize = 0;
    if (headerResult == 0) {
        tj3Transform(transHandle, Data, Size, 1, &dstBuf, &dstSize, &transform);
    }

    // Cleanup
    if (iccBuf) {
        tj3Free(iccBuf);
    }
    if (dstBuf) {
        tj3Free(dstBuf);
    }
    tj3Destroy(decompHandle);
    tj3Destroy(transHandle);

    return 0;
}