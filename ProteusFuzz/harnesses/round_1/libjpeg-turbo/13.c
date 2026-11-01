// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjTransform at turbojpeg.c:3041:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3GetICCProfile at turbojpeg.c:1926:15 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3GetICCProfile at turbojpeg.c:1926:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3SetICCProfile at turbojpeg.c:1164:15 in turbojpeg.h
// tj3GetErrorCode at turbojpeg.c:643:15 in turbojpeg.h
// tj3SetICCProfile at turbojpeg.c:1164:15 in turbojpeg.h
// tjGetErrorCode at turbojpeg.c:652:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3SetICCProfile at turbojpeg.c:1164:15 in turbojpeg.h
// tj3SetICCProfile at turbojpeg.c:1164:15 in turbojpeg.h
// tj3SetICCProfile at turbojpeg.c:1164:15 in turbojpeg.h
// tj3Transform at turbojpeg.c:2867:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <turbojpeg.h>

int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    tjhandle handle = NULL;
    int ret = 0;
    int op = 0;
    int flags = 0;
    int n = 1;
    unsigned char *dstBuf = NULL;
    size_t dstSize = 0;
    unsigned char *iccBuf = NULL;
    size_t iccSize = 0;
    tjtransform transform;

    // Initialize transform structure
    memset(&transform, 0, sizeof(transform));
    transform.r.x = 0;
    transform.r.y = 0;
    transform.r.w = 0;
    transform.r.h = 0;
    transform.op = TJXOP_NONE;
    transform.options = 0;
    transform.customFilter = NULL;
    transform.data = NULL;

    // Parse input for operation selection and parameters
    uint32_t seed = 0;
    if (Size >= 4) {
        memcpy(&seed, Data, 4);
        op = seed % 6;
        flags = (seed >> 16) & 0xFF;
    }

    // Initialize handle based on operation
    if (op < 3) {
        // Compression or Transform requires compression handle
        handle = tj3Init(TJINIT_COMPRESS);
    } else {
        // Decompression or ICC retrieval requires decompression handle
        handle = tj3Init(TJINIT_DECOMPRESS);
    }

    if (!handle) {
        return 0;
    }

    // Ensure we have enough data for buffer operations
    if (Size < 8) {
        tj3Destroy(handle);
        return 0;
    }

    // Extract potential JPEG data or ICC profile data from input
    size_t dataLen = Size - 4;
    const uint8_t *dataPtr = Data + 4;

    switch (op) {
        case 0: {
            // Fuzz tj3SetICCProfile
            // Try with valid buffer and size
            if (tj3SetICCProfile(handle, (unsigned char *)dataPtr, dataLen) == 0) {
                // Try removing profile
                tj3SetICCProfile(handle, NULL, 0);
            }
            // Try with NULL buffer
            tj3SetICCProfile(handle, NULL, dataLen);
            break;
        }
        case 1: {
            // Fuzz tj3Transform
            // Requires a valid JPEG source. If input is not a valid JPEG, this will likely fail gracefully.
            dstBuf = NULL;
            dstSize = 0;
            transform.op = (dataLen > 0) ? (dataPtr[0] % 6) : 0;
            transform.r.w = (dataLen > 1) ? (dataPtr[1] * 10) : 0;
            transform.r.h = (dataLen > 2) ? (dataPtr[2] * 10) : 0;
            
            ret = tj3Transform(handle, dataPtr, dataLen, 1, &dstBuf, &dstSize, &transform);
            if (dstBuf) {
                tj3Free(dstBuf);
            }
            break;
        }
        case 2: {
            // Fuzz tjTransform (legacy)
            dstBuf = NULL;
            unsigned long uDstSize = 0;
            unsigned long uJpegSize = (dataLen < 0xFFFFFFFFUL) ? (unsigned long)dataLen : 0xFFFFFFFFUL;
            
            transform.op = (dataLen > 0) ? (dataPtr[0] % 6) : 0;
            transform.r.w = (dataLen > 1) ? (dataPtr[1] * 10) : 0;
            transform.r.h = (dataLen > 2) ? (dataPtr[2] * 10) : 0;

            ret = tjTransform(handle, dataPtr, uJpegSize, 1, &dstBuf, &uDstSize, &transform, flags);
            if (dstBuf) {
                tj3Free(dstBuf);
            }
            break;
        }
        case 3: {
            // Fuzz tj3GetICCProfile
            // This usually requires a previous decompress with SAVEMARKERS, but we test the API directly
            unsigned char *tempBuf = NULL;
            size_t tempSize = 0;
            
            // First try to get size
            ret = tj3GetICCProfile(handle, NULL, &tempSize);
            if (ret == 0 && tempSize > 0) {
                tempBuf = (unsigned char *)tj3Alloc(tempSize);
                if (tempBuf) {
                    ret = tj3GetICCProfile(handle, &tempBuf, &tempSize);
                    if (tempBuf) {
                        tj3Free(tempBuf);
                    }
                }
            }
            break;
        }
        case 4: {
            // Fuzz tj3GetErrorCode
            // Trigger an error first by passing invalid data to a transform or set
            tj3SetICCProfile(handle, (unsigned char *)dataPtr, dataLen);
            int code = tj3GetErrorCode(handle);
            (void)code;
            break;
        }
        case 5: {
            // Fuzz tjGetErrorCode
            tj3SetICCProfile(handle, (unsigned char *)dataPtr, dataLen);
            int code = tjGetErrorCode(handle);
            (void)code;
            break;
        }
    }

    tj3Destroy(handle);
    return 0;
}