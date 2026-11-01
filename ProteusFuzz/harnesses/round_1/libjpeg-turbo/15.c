// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tj3SetICCProfile at turbojpeg.c:1164:15 in turbojpeg.h
// tj3SetICCProfile at turbojpeg.c:1164:15 in turbojpeg.h
// tj3SetICCProfile at turbojpeg.c:1164:15 in turbojpeg.h
// tjDecompressHeader at turbojpeg.c:1914:15 in turbojpeg.h
// tjGetErrorCode at turbojpeg.c:652:15 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjDecompressHeader at turbojpeg.c:1914:15 in turbojpeg.h
// tj3GetICCProfile at turbojpeg.c:1926:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3GetICCProfile at turbojpeg.c:1926:15 in turbojpeg.h
// tj3GetErrorCode at turbojpeg.c:643:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjTransform at turbojpeg.c:3041:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <turbojpeg.h>

int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    tjhandle handle = tjInitCompress();
    if (!handle) return 0;

    // Test tj3SetICCProfile with various inputs
    if (Size >= 2) {
        unsigned char *iccBuf = (unsigned char *)malloc(Size);
        if (iccBuf) {
            memcpy(iccBuf, Data, Size);
            tj3SetICCProfile(handle, iccBuf, Size);
            free(iccBuf);
        }
        // Test with NULL buffer
        tj3SetICCProfile(handle, NULL, Size);
        // Test with size 0
        tj3SetICCProfile(handle, Data, 0);
    }

    // Test tjDecompressHeader
    int width = 0, height = 0;
    tjDecompressHeader(handle, (unsigned char *)Data, Size, &width, &height);

    // Check error code after potential failure
    int errCode = tjGetErrorCode(handle);
    (void)errCode;

    // Initialize a decompression handle for ICC profile retrieval
    tjhandle decompHandle = tjInitDecompress();
    if (decompHandle) {
        // Try to decompress header with SAVEMARKERS to potentially extract ICC
        int w = 0, h = 0;
        tjDecompressHeader(decompHandle, (unsigned char *)Data, Size, &w, &h);

        // Test tj3GetICCProfile
        unsigned char *iccBuf = NULL;
        size_t iccSize = 0;
        tj3GetICCProfile(decompHandle, &iccBuf, &iccSize);
        if (iccBuf) {
            tj3Free(iccBuf);
        }

        // Test with NULL iccBuf
        tj3GetICCProfile(decompHandle, NULL, &iccSize);

        // Test tj3GetErrorCode
        int errCodeDecomp = tj3GetErrorCode(decompHandle);
        (void)errCodeDecomp;

        tjDestroy(decompHandle);
    }

    // Prepare for tjTransform test
    // We need a valid JPEG buffer to transform. 
    // If the input data is not a valid JPEG, tjTransform will fail gracefully.
    unsigned char *dstBuf = NULL;
    unsigned long dstSize = 0;
    tjtransform transform;
    memset(&transform, 0, sizeof(transform));
    transform.op = TJXOP_NONE; // No-op transform to minimize failure chances on invalid data
    transform.options = 0;
    transform.data = NULL;
    transform.customFilter = NULL;

    // Attempt transform (will likely fail if input is not a valid JPEG, which is fine for fuzzing)
    tjTransform(handle, (unsigned char *)Data, Size, 1, &dstBuf, &dstSize, &transform, 0);
    if (dstBuf) {
        tj3Free(dstBuf);
    }

    // Cleanup
    tjDestroy(handle);
    return 0;
}