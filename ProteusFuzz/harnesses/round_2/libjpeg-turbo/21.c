// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjDecompressHeader2 at turbojpeg.c:1903:15 in turbojpeg.h
// tj3GetICCProfile at turbojpeg.c:1926:15 in turbojpeg.h
// tj3GetICCProfile at turbojpeg.c:1926:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tjTransform at turbojpeg.c:3041:15 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjTransform at turbojpeg.c:3041:15 in turbojpeg.h
// tjGetErrorCode at turbojpeg.c:652:15 in turbojpeg.h
// tj3GetErrorCode at turbojpeg.c:643:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3SetICCProfile at turbojpeg.c:1164:15 in turbojpeg.h
// tj3SetICCProfile at turbojpeg.c:1164:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <turbojpeg.h>

int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    // Use input data to seed random choices
    uint32_t seed = 0;
    if (Size >= 4) {
        memcpy(&seed, Data, 4);
    } else {
        seed = Size;
    }

    // Initialize TurboJPEG instance for compression
    tjhandle handleCompress = tjInitCompress();
    if (!handleCompress) return 0;

    // Initialize TurboJPEG instance for decompression
    tjhandle handleDecompress = tjInitDecompress();
    if (!handleDecompress) {
        tjDestroy(handleCompress);
        return 0;
    }

    // Create a dummy JPEG buffer from input data if possible, or minimal valid header
    unsigned char *jpegBuf = NULL;
    unsigned long jpegSize = 0;
    int isRealJpeg = 0;

    // Try to use input as JPEG if it starts with JPEG marker (0xFFD8)
    if (Size >= 2 && Data[0] == 0xFF && Data[1] == 0xD8) {
        jpegBuf = (unsigned char *)malloc(Size);
        if (jpegBuf) {
            memcpy(jpegBuf, Data, Size);
            jpegSize = Size;
            isRealJpeg = 1;
        }
    }

    // If not a valid JPEG, create a minimal valid JPEG (1x1 pixel)
    if (!jpegBuf) {
        // Minimal valid JPEG: 0xFFD8 0xFFE0 ... 0xFFD9
        jpegSize = 20;
        jpegBuf = (unsigned char *)malloc(jpegSize);
        if (!jpegBuf) {
            tjDestroy(handleCompress);
            tjDestroy(handleDecompress);
            return 0;
        }
        // Write minimal JPEG header
        jpegBuf[0] = 0xFF;
        jpegBuf[1] = 0xD8; // SOI
        jpegBuf[2] = 0xFF;
        jpegBuf[3] = 0xE0; // APP0
        jpegBuf[4] = 0x00;
        jpegBuf[5] = 0x04; // Length
        jpegBuf[6] = 'J';
        jpegBuf[7] = 'F';
        jpegBuf[8] = 'I';
        jpegBuf[9] = 'F';
        jpegBuf[10] = 0;
        jpegBuf[11] = 1;
        jpegBuf[12] = 1;
        jpegBuf[13] = 0;
        jpegBuf[14] = 0;
        jpegBuf[15] = 0;
        jpegBuf[16] = 0;
        jpegBuf[17] = 0;
        jpegBuf[18] = 0xFF;
        jpegBuf[19] = 0xD9; // EOI
        isRealJpeg = 1;
    }

    // Test tj3SetICCProfile on compression handle
    unsigned char *iccBuf = NULL;
    size_t iccSize = 0;
    if (Size > 100) {
        iccBuf = (unsigned char *)malloc(Size - 100);
        if (iccBuf) {
            memcpy(iccBuf, Data + 100, Size - 100);
            iccSize = Size - 100;
            tj3SetICCProfile(handleCompress, iccBuf, iccSize);
            tj3SetICCProfile(handleCompress, NULL, 0); // Remove profile
            free(iccBuf);
            iccBuf = NULL;
        }
    }

    // Test tj3GetICCProfile on decompression handle
    // First, try to load header with SAVEMARKERS to enable ICC retrieval
    unsigned long width, height;
    int jpegSubsamp;
    
    // Use tjDecompressHeader2 which is available in older versions or standard API
    // The function signature is: int tjDecompressHeader2(tjhandle handle, unsigned char *jpegBuf,
    // unsigned long jpegSize, int *width, int *height, int *jpegSubsamp);
    if (tjDecompressHeader2(handleDecompress, jpegBuf, jpegSize, (int*)&width, (int*)&height, &jpegSubsamp) == 0) {
        // Try to get ICC profile size
        size_t retrievedSize = 0;
        unsigned char *retrievedBuf = NULL;
        
        // Get size first
        if (tj3GetICCProfile(handleDecompress, NULL, &retrievedSize) == 0 && retrievedSize > 0) {
            retrievedBuf = (unsigned char *)malloc(retrievedSize);
            if (retrievedBuf) {
                tj3GetICCProfile(handleDecompress, &retrievedBuf, &retrievedSize);
                if (retrievedBuf) {
                    tj3Free(retrievedBuf);
                }
            }
        }
    }

    // Test tjTransform
    if (isRealJpeg && jpegBuf) {
        tjtransform transform;
        memset(&transform, 0, sizeof(transform));
        transform.r = (tjregion){0, 0, 0, 0}; // Default crop
        transform.op = seed % 4; // Random transform op
        transform.options = 0;
        transform.customFilter = NULL;
        transform.data = NULL;

        unsigned char *dstBuf = NULL;
        unsigned long dstSize = 0;
        int n = 1;
        int flags = (seed >> 8) % 2; // Random flag

        tjTransform(handleCompress, jpegBuf, jpegSize, n, &dstBuf, &dstSize, &transform, flags);
        if (dstBuf) {
            tjFree(dstBuf);
        }

        // Try with invalid handle
        tjTransform(NULL, jpegBuf, jpegSize, n, &dstBuf, &dstSize, &transform, flags);
    }

    // Test error codes
    int errCode1 = tjGetErrorCode(handleCompress);
    int errCode2 = tj3GetErrorCode(handleDecompress);
    (void)errCode1;
    (void)errCode2;

    // Test tjDestroy
    tjDestroy(handleCompress);
    tjDestroy(handleDecompress);

    // Test tjDestroy on NULL
    tjDestroy(NULL);

    // Cleanup
    if (jpegBuf) {
        free(jpegBuf);
    }

    return 0;
}