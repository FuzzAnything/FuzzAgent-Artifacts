// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDecompressHeader3 at turbojpeg.c:1874:15 in turbojpeg.h
// tj3GetErrorCode at turbojpeg.c:643:15 in turbojpeg.h
// tj3GetICCProfile at turbojpeg.c:1926:15 in turbojpeg.h
// tj3GetICCProfile at turbojpeg.c:1926:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3SetICCProfile at turbojpeg.c:1164:15 in turbojpeg.h
// tjGetErrorCode at turbojpeg.c:652:15 in turbojpeg.h
// tj3SetICCProfile at turbojpeg.c:1164:15 in turbojpeg.h
// tjTransform at turbojpeg.c:3041:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <turbojpeg.h>

int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    tjhandle handle = NULL;
    tjhandle compHandle = NULL;
    int width = 0, height = 0, jpegSubsamp = 0, jpegColorspace = 0;
    unsigned char *iccBuf = NULL;
    size_t iccSize = 0;
    tjtransform transform;
    int errorCode;

    // Initialize decompression handle
    handle = tjInitDecompress();
    if (!handle) return 0;

    // Initialize compression handle for ICC profile tests
    compHandle = tjInitCompress();
    if (!compHandle) {
        tjDestroy(handle);
        return 0;
    }

    // 1. Test tjDecompressHeader3
    // Pass the fuzzed data as the JPEG buffer
    tjDecompressHeader3(handle, Data, Size, &width, &height, &jpegSubsamp, &jpegColorspace);
    errorCode = tj3GetErrorCode(handle);
    (void)errorCode; // Use return value to avoid unused warning

    // 2. Test tj3GetICCProfile (Get size first)
    tj3GetICCProfile(handle, NULL, &iccSize);
    
    // If size > 0, allocate and retrieve
    if (iccSize > 0) {
        iccBuf = (unsigned char *)malloc(iccSize);
        if (iccBuf) {
            tj3GetICCProfile(handle, &iccBuf, &iccSize);
            // Free the buffer as per documentation
            tj3Free(iccBuf);
            iccBuf = NULL;
        }
    }

    // 3. Test tj3SetICCProfile on compression handle
    // Use fuzzed data as ICC profile buffer if size is reasonable
    if (Size > 0 && Size < 1024 * 1024) {
        tj3SetICCProfile(compHandle, (unsigned char *)Data, Size);
        errorCode = tjGetErrorCode(compHandle);
        (void)errorCode;
    }
    
    // Test with NULL/0 to remove profile
    tj3SetICCProfile(compHandle, NULL, 0);

    // 4. Test tjTransform
    // Prepare transform structure
    memset(&transform, 0, sizeof(transform));
    transform.op = TJXOP_NONE; // Default to no-op to avoid complex validation failures
    transform.options = 0;
    transform.customFilter = NULL;
    
    // Attempt transform with fuzzed data as source
    // Note: dstBufs is an array of pointers, we pass a single pointer in an array
    unsigned char *dstBufs[1] = {NULL};
    unsigned long dstSizes[1] = {0};
    
    // We need a valid source JPEG for transform to work well, but fuzzing invalid data is the goal
    // tjTransform will likely fail on invalid JPEG, which is what we want to test
    tjTransform(handle, Data, Size, 1, dstBufs, dstSizes, &transform, 0);
    
    // Clean up destination buffer if allocated by library
    if (dstBufs[0]) {
        tj3Free(dstBufs[0]);
    }

    // Cleanup
    tjDestroy(handle);
    tjDestroy(compHandle);

    return 0;
}