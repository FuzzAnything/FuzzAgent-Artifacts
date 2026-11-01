// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjGetErrorCode at turbojpeg.c:652:15 in turbojpeg.h
// tj3GetErrorCode at turbojpeg.c:643:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3SetICCProfile at turbojpeg.c:1164:15 in turbojpeg.h
// tj3SetICCProfile at turbojpeg.c:1164:15 in turbojpeg.h
// tj3GetICCProfile at turbojpeg.c:1926:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tjTransform at turbojpeg.c:3041:15 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjGetErrorCode at turbojpeg.c:652:15 in turbojpeg.h
// tj3GetErrorCode at turbojpeg.c:643:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <turbojpeg.h>

int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size) {
    if (Size < 10) {
        return 0;
    }

    tjhandle handle = NULL;
    int ret = 0;

    // Use the first byte to decide initialization mode
    int mode = Data[0] % 3;

    if (mode == 0) {
        // Initialize for compression
        handle = tjInitCompress();
    } else if (mode == 1) {
        // Initialize for decompression
        handle = tjInitDecompress();
    } else {
        // Do not initialize, test error paths with NULL handle
        handle = NULL;
    }

    if (handle == NULL) {
        // Test error code retrieval on invalid handle
        (void)tjGetErrorCode(NULL);
        (void)tj3GetErrorCode(NULL);
        (void)tjDestroy(NULL);
        return 0;
    }

    // Test tj3SetICCProfile
    // Use remaining data as potential ICC profile
    size_t iccSize = Size - 1;
    if (iccSize > 0) {
        // Only set ICC if initialized for compression
        if (mode == 0) {
            ret = tj3SetICCProfile(handle, (unsigned char *)&Data[1], iccSize);
        }
    } else {
        // Try to remove ICC profile
        if (mode == 0) {
            ret = tj3SetICCProfile(handle, NULL, 0);
        }
    }

    // Test tj3GetICCProfile
    unsigned char *iccBuf = NULL;
    size_t retrievedSize = 0;
    if (mode == 1) {
        // Only valid for decompression handles with markers saved
        // We don't have a valid JPEG with markers here, but we test the API call
        ret = tj3GetICCProfile(handle, &iccBuf, &retrievedSize);
        if (iccBuf != NULL) {
            tj3Free(iccBuf);
        }
    }

    // Test tjTransform
    // Prepare a dummy transform spec
    tjtransform transform = {0};
    transform.op = Data[1] % 6; // TJXOP_NONE to TJXOP_TRANSPOSE
    transform.options = (Data[2] % 2) * TJXOPT_CROP;
    
    unsigned char *dstBuf = NULL;
    unsigned long dstSize = 0;
    
    // We need a valid JPEG buffer for transform, but we don't have one.
    // We pass the input data as a potential JPEG buffer.
    // This will likely fail, which is fine for fuzzing.
    ret = tjTransform(handle, Data, Size, 1, &dstBuf, &dstSize, &transform, 0);
    
    if (dstBuf != NULL) {
        tjFree(dstBuf);
    }

    // Test error codes after potential failures
    int errCode = tjGetErrorCode(handle);
    int errCode3 = tj3GetErrorCode(handle);
    (void)errCode;
    (void)errCode3;

    // Cleanup
    ret = tjDestroy(handle);
    (void)ret;

    return 0;
}