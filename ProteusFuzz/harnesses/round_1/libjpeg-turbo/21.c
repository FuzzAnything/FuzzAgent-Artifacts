// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjDecompressHeader at turbojpeg.c:1914:15 in turbojpeg.h
// tj3GetErrorCode at turbojpeg.c:643:15 in turbojpeg.h
// tj3GetICCProfile at turbojpeg.c:1926:15 in turbojpeg.h
// tj3GetErrorCode at turbojpeg.c:643:15 in turbojpeg.h
// tj3GetICCProfile at turbojpeg.c:1926:15 in turbojpeg.h
// tjGetErrorCode at turbojpeg.c:652:15 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tj3SetICCProfile at turbojpeg.c:1164:15 in turbojpeg.h
// tj3GetErrorCode at turbojpeg.c:643:15 in turbojpeg.h
// tj3SetICCProfile at turbojpeg.c:1164:15 in turbojpeg.h
// tj3GetErrorCode at turbojpeg.c:643:15 in turbojpeg.h
// tjTransform at turbojpeg.c:3041:15 in turbojpeg.h
// tjGetErrorCode at turbojpeg.c:652:15 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <turbojpeg.h>

int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    tjhandle handle = NULL;
    unsigned char *jpegBuf = NULL;
    unsigned char *dstBuf = NULL;
    unsigned long jpegSize = 0;
    unsigned long dstSize = 0;
    int width = 0, height = 0;
    int ret;
    int errorCode;
    unsigned char *iccBuf = NULL;
    size_t iccSize = 0;

    // Use input data to determine operations
    uint8_t op = Data[0] % 4;
    uint8_t subOp = Data[1] % 4;

    // Initialize handle for compression or decompression based on op
    if (op == 0 || op == 1) {
        // Compression path
        handle = tjInitCompress();
        if (!handle) return 0;

        // Prepare a dummy JPEG buffer if needed for transformation
        // We need a valid JPEG to transform. If input is too small, we skip transform.
        if (Size > 100) {
            jpegBuf = (unsigned char *)malloc(Size);
            if (jpegBuf) {
                memcpy(jpegBuf, Data, Size);
                jpegSize = Size;
            }
        }

        // Fuzz tj3SetICCProfile
        // Use part of input as ICC profile
        if (Size > 10) {
            unsigned char *iccData = (unsigned char *)malloc(Size - 10);
            if (iccData) {
                memcpy(iccData, Data + 10, Size - 10);
                ret = tj3SetICCProfile(handle, iccData, Size - 10);
                if (ret == -1) {
                    errorCode = tj3GetErrorCode(handle);
                    (void)errorCode; // consume
                }
                free(iccData);
            }
            // Test removal
            ret = tj3SetICCProfile(handle, NULL, 0);
            if (ret == -1) {
                errorCode = tj3GetErrorCode(handle);
                (void)errorCode;
            }
        }

        // Fuzz tjTransform (requires a valid JPEG source)
        if (jpegBuf && jpegSize > 0) {
            tjtransform transform;
            memset(&transform, 0, sizeof(transform));
            transform.op = subOp % 4; // 0: none, 1: rotate 90, etc.
            transform.options = 0;
            transform.data = NULL;
            transform.customFilter = NULL;

            ret = tjTransform(handle, jpegBuf, jpegSize, 1, &dstBuf, &dstSize, &transform, 0);
            if (ret == -1) {
                errorCode = tjGetErrorCode(handle);
                (void)errorCode;
            }
            if (dstBuf) {
                tjFree(dstBuf);
                dstBuf = NULL;
            }
        }

        tjDestroy(handle);
        handle = NULL;
        if (jpegBuf) free(jpegBuf);
        jpegBuf = NULL;

    } else if (op == 2 || op == 3) {
        // Decompression path
        handle = tjInitDecompress();
        if (!handle) return 0;

        // Need a valid JPEG to decompress header
        if (Size > 10) {
            jpegBuf = (unsigned char *)malloc(Size);
            if (jpegBuf) {
                memcpy(jpegBuf, Data, Size);
                jpegSize = Size;

                // Fuzz tjDecompressHeader
                ret = tjDecompressHeader(handle, jpegBuf, jpegSize, &width, &height);
                if (ret == -1) {
                    errorCode = tj3GetErrorCode(handle);
                    (void)errorCode;
                }

                // Fuzz tj3GetICCProfile
                // First try to get size only
                ret = tj3GetICCProfile(handle, NULL, &iccSize);
                if (ret == -1) {
                    errorCode = tj3GetErrorCode(handle);
                    (void)errorCode;
                }

                // Then try to get buffer
                if (iccSize > 0) {
                    ret = tj3GetICCProfile(handle, &iccBuf, &iccSize);
                    if (ret == -1) {
                        errorCode = tjGetErrorCode(handle);
                        (void)errorCode;
                    }
                    if (iccBuf) {
                        tjFree(iccBuf);
                        iccBuf = NULL;
                    }
                }
            }
        }

        tjDestroy(handle);
        handle = NULL;
        if (jpegBuf) free(jpegBuf);
        jpegBuf = NULL;
    }

    return 0;
}