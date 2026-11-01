// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjDecompressHeader at turbojpeg.c:1914:15 in turbojpeg.h
// tjDecompressHeader2 at turbojpeg.c:1903:15 in turbojpeg.h
// tjDecompressHeader3 at turbojpeg.c:1874:15 in turbojpeg.h
// tj3GetICCProfile at turbojpeg.c:1926:15 in turbojpeg.h
// tj3GetICCProfile at turbojpeg.c:1926:15 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjTransform at turbojpeg.c:3041:15 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <turbojpeg.h>

int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    tjhandle handle = tjInitDecompress();
    if (!handle) return 0;

    int width = 0, height = 0, jpegSubsamp = 0, jpegColorspace = 0;
    unsigned char *iccBuf = NULL;
    size_t iccSize = 0;

    // Test tjDecompressHeader
    tjDecompressHeader(handle, (unsigned char *)Data, Size, &width, &height);

    // Test tjDecompressHeader2
    tjDecompressHeader2(handle, (unsigned char *)Data, Size, &width, &height, &jpegSubsamp);

    // Test tjDecompressHeader3
    tjDecompressHeader3(handle, (unsigned char *)Data, Size, &width, &height, &jpegSubsamp, &jpegColorspace);

    // Test tj3GetICCProfile (requires TJPARAM_SAVEMARKERS, but we try anyway)
    tj3GetICCProfile(handle, NULL, &iccSize);
    if (iccSize > 0) {
        iccBuf = (unsigned char *)malloc(iccSize);
        if (iccBuf) {
            tj3GetICCProfile(handle, &iccBuf, &iccSize);
            if (iccBuf) {
                tjFree(iccBuf);
            }
        }
    }

    // Test tjTransform
    if (width > 0 && height > 0 && Size > 0) {
        tjtransform transform = {0};
        transform.r.w = width / 2;
        transform.r.h = height / 2;
        transform.r.x = 0;
        transform.r.y = 0;
        transform.op = TJXOP_NONE;
        transform.options = 0;
        transform.data = NULL;
        transform.customFilter = NULL;

        unsigned char *dstBufs[1] = {NULL};
        unsigned long dstSizes[1] = {0};

        tjTransform(handle, (unsigned char *)Data, Size, 1, dstBufs, dstSizes, &transform, 0);
        if (dstBufs[0]) {
            tjFree(dstBufs[0]);
        }
    }

    // Cleanup
    tjDestroy(handle);

    return 0;
}