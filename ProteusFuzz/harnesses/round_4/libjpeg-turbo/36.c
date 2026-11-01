// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjTransform at turbojpeg.c:3041:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tjGetErrorCode at turbojpeg.c:652:15 in turbojpeg.h
// tj3GetErrorCode at turbojpeg.c:643:15 in turbojpeg.h
// tjTransform at turbojpeg.c:3041:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tj3GetICCProfile at turbojpeg.c:1926:15 in turbojpeg.h
// tj3GetICCProfile at turbojpeg.c:1926:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3SetICCProfile at turbojpeg.c:1164:15 in turbojpeg.h
// tj3GetErrorCode at turbojpeg.c:643:15 in turbojpeg.h
// tj3SetICCProfile at turbojpeg.c:1164:15 in turbojpeg.h
// tj3GetErrorCode at turbojpeg.c:643:15 in turbojpeg.h
// tj3GetICCProfile at turbojpeg.c:1926:15 in turbojpeg.h
// tj3GetICCProfile at turbojpeg.c:1926:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tjDecompressHeader3 at turbojpeg.c:1874:15 in turbojpeg.h
// tjGetErrorCode at turbojpeg.c:652:15 in turbojpeg.h
// tj3GetErrorCode at turbojpeg.c:643:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <turbojpeg.h>

static void cleanup_handle(tjhandle handle) {
    if (handle) {
        tjDestroy(handle);
    }
}

static int fuzz_icc_profile(const uint8_t *Data, size_t Size, tjhandle handle) {
    unsigned char *iccBuf = NULL;
    size_t iccSize = 0;
    int ret;

    // Try to get size first
    ret = tj3GetICCProfile(handle, NULL, &iccSize);
    if (ret != 0) {
        // Error is expected if no profile or handle issue, continue
    }

    // Try to get buffer if size > 0
    if (iccSize > 0) {
        iccBuf = (unsigned char *)malloc(iccSize);
        if (!iccBuf) {
            return -1; // Allocation failure
        }
        ret = tj3GetICCProfile(handle, &iccBuf, &iccSize);
        if (ret == 0 && iccBuf) {
            // Profile retrieved, free it
            tj3Free(iccBuf);
            iccBuf = NULL;
        } else {
            free(iccBuf);
            iccBuf = NULL;
        }
    }

    // Try to set a profile using input data
    if (Size > 0) {
        ret = tj3SetICCProfile(handle, (unsigned char *)Data, Size);
        if (ret != 0) {
            int errCode = tj3GetErrorCode(handle);
            (void)errCode; // Suppress unused warning
        }
    }

    // Try to set NULL profile
    ret = tj3SetICCProfile(handle, NULL, 0);
    if (ret != 0) {
        int errCode = tj3GetErrorCode(handle);
        (void)errCode;
    }

    // Try to get profile again after setting
    iccSize = 0;
    ret = tj3GetICCProfile(handle, NULL, &iccSize);
    if (ret == 0 && iccSize > 0) {
        iccBuf = (unsigned char *)malloc(iccSize);
        if (iccBuf) {
            ret = tj3GetICCProfile(handle, &iccBuf, &iccSize);
            if (ret == 0) {
                tj3Free(iccBuf);
            } else {
                free(iccBuf);
            }
        }
    }

    return 0;
}

static int fuzz_transform(const uint8_t *Data, size_t Size, tjhandle handle) {
    if (Size < 10) return 0; // Need enough data for a minimal JPEG header

    int width, height, subsamp, colorspace;
    int ret;

    // Initialize output vars
    width = 0; height = 0; subsamp = 0; colorspace = 0;

    // Parse header
    ret = tjDecompressHeader3(handle, Data, Size, &width, &height, &subsamp, &colorspace);
    if (ret != 0) {
        // Invalid JPEG, try to get error code
        int errCode = tjGetErrorCode(handle);
        int errSeverity = tj3GetErrorCode(handle);
        (void)errCode; (void)errSeverity;
        return 0;
    }

    if (width <= 0 || height <= 0) return 0;

    // Prepare transform structure
    tjtransform transform;
    memset(&transform, 0, sizeof(transform));
    transform.r.w = width;
    transform.r.h = height;
    transform.r.x = 0;
    transform.r.y = 0;
    transform.op = TJXOP_NONE;
    transform.options = 0;
    transform.data = NULL;
    transform.customFilter = NULL;

    unsigned char *dstBuf = NULL;
    unsigned long dstSize = 0;
    
    // Fix: tjTransform expects arrays of pointers to buffers and pointers to sizes
    unsigned char *dstBufs[1];
    unsigned long *dstSizes[1];
    
    dstBufs[0] = &dstBuf;
    dstSizes[0] = &dstSize;

    // Try transform
    ret = tjTransform(handle, Data, Size, 1, dstBufs, dstSizes, &transform, 0);
    if (ret == 0 && dstBuf) {
        tj3Free(dstBuf);
        dstBuf = NULL;
    } else {
        int errCode = tjGetErrorCode(handle);
        int errSeverity = tj3GetErrorCode(handle);
        (void)errCode; (void)errSeverity;
    }

    // Try with different flags
    ret = tjTransform(handle, Data, Size, 1, dstBufs, dstSizes, &transform, TJXOPT_CROP);
    if (ret == 0 && dstBuf) {
        tj3Free(dstBuf);
        dstBuf = NULL;
    }

    return 0;
}

int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    tjhandle handle = tjInitDecompress();
    if (!handle) {
        return 0;
    }

    // Fuzz ICC profile functions
    fuzz_icc_profile(Data, Size, handle);

    // Fuzz Transform functions
    fuzz_transform(Data, Size, handle);

    cleanup_handle(handle);

    // Create compression handle to test tj3SetICCProfile specifically in compression context
    tjhandle compHandle = tjInitCompress();
    if (compHandle) {
        fuzz_icc_profile(Data, Size, compHandle);
        cleanup_handle(compHandle);
    }

    return 0;
}