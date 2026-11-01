// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjDecompressHeader at turbojpeg.c:1914:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjDecompressHeader3 at turbojpeg.c:1874:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDecompressToYUVPlanes at turbojpeg.c:2288:15 in turbojpeg.h
// tjDecompressHeader2 at turbojpeg.c:1903:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjDecompressHeader2 at turbojpeg.c:1903:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDecompress at turbojpeg.c:2108:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjDecompressHeader3 at turbojpeg.c:1874:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjDecompressHeader3 at turbojpeg.c:1874:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3DecompressToYUVPlanes8 at turbojpeg.c:2122:15 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <turbojpeg.h>

static int fuzz_tjDecompressHeader2(const uint8_t *Data, size_t Size) {
    tjhandle handle = tjInitDecompress();
    if (!handle) return 0;

    int width = 0, height = 0, subsamp = 0;
    if (Size > 0) {
        tjDecompressHeader2(handle, (unsigned char *)Data, Size, &width, &height, &subsamp);
    }
    tjDestroy(handle);
    return 0;
}

static int fuzz_tjDecompress(const uint8_t *Data, size_t Size) {
    tjhandle handle = tjInitDecompress();
    if (!handle) return 0;

    int width = 0, height = 0, subsamp = 0;
    if (Size > 0) {
        if (tjDecompressHeader2(handle, (unsigned char *)Data, Size, &width, &height, &subsamp) != 0) {
            tjDestroy(handle);
            return 0;
        }
        if (width <= 0 || height <= 0) {
            tjDestroy(handle);
            return 0;
        }

        // tjPixelSize is a macro or array in some versions, use manual calculation for safety
        // TJPF_RGB = 3 bytes per pixel
        int pixelSize = 3; 
        int pitch = width * pixelSize;
        size_t dstSize = (size_t)height * pitch;
        unsigned char *dstBuf = (unsigned char *)malloc(dstSize);
        if (!dstBuf) {
            tjDestroy(handle);
            return 0;
        }

        tjDecompress(handle, (unsigned char *)Data, Size, dstBuf, width, pitch, height, pixelSize, 0);
        free(dstBuf);
    }
    tjDestroy(handle);
    return 0;
}

static int fuzz_tjDecompressHeader3(const uint8_t *Data, size_t Size) {
    tjhandle handle = tjInitDecompress();
    if (!handle) return 0;

    int width = 0, height = 0, subsamp = 0, cs = 0;
    if (Size > 0) {
        tjDecompressHeader3(handle, Data, Size, &width, &height, &subsamp, &cs);
    }
    tjDestroy(handle);
    return 0;
}

static int fuzz_tj3DecompressToYUVPlanes8(const uint8_t *Data, size_t Size) {
    tjhandle handle = tjInitDecompress();
    if (!handle) return 0;

    int width = 0, height = 0, subsamp = 0, cs = 0;
    if (Size > 0) {
        if (tjDecompressHeader3(handle, Data, Size, &width, &height, &subsamp, &cs) != 0) {
            tjDestroy(handle);
            return 0;
        }
        if (width <= 0 || height <= 0) {
            tjDestroy(handle);
            return 0;
        }

        int strides[3] = {0, 0, 0};
        unsigned char *planes[3] = {NULL, NULL, NULL};

        // Allocate planes based on subsampling
        size_t ySize = (size_t)width * height;
        size_t uvSize = (size_t)(width / 2) * (height / 2); // Approximate for 4:2:0

        planes[0] = (unsigned char *)malloc(ySize);
        if (subsamp == TJSAMP_420 || subsamp == TJSAMP_422 || subsamp == TJSAMP_411) {
            planes[1] = (unsigned char *)malloc(uvSize);
            planes[2] = (unsigned char *)malloc(uvSize);
        }

        if (planes[0]) {
            tj3DecompressToYUVPlanes8(handle, Data, Size, planes, strides);
        }

        free(planes[0]);
        free(planes[1]);
        free(planes[2]);
    }
    tjDestroy(handle);
    return 0;
}

static int fuzz_tjDecompressHeader(const uint8_t *Data, size_t Size) {
    tjhandle handle = tjInitDecompress();
    if (!handle) return 0;

    int width = 0, height = 0;
    if (Size > 0) {
        tjDecompressHeader(handle, (unsigned char *)Data, Size, &width, &height);
    }
    tjDestroy(handle);
    return 0;
}

static int fuzz_tjDecompressToYUVPlanes(const uint8_t *Data, size_t Size) {
    tjhandle handle = tjInitDecompress();
    if (!handle) return 0;

    int width = 0, height = 0, subsamp = 0, cs = 0;
    if (Size > 0) {
        if (tjDecompressHeader3(handle, Data, Size, &width, &height, &subsamp, &cs) != 0) {
            tjDestroy(handle);
            return 0;
        }
        if (width <= 0 || height <= 0) {
            tjDestroy(handle);
            return 0;
        }

        int strides[3] = {0, 0, 0};
        unsigned char *planes[3] = {NULL, NULL, NULL};

        size_t ySize = (size_t)width * height;
        size_t uvSize = (size_t)(width / 2) * (height / 2);

        planes[0] = (unsigned char *)malloc(ySize);
        if (subsamp == TJSAMP_420 || subsamp == TJSAMP_422 || subsamp == TJSAMP_411) {
            planes[1] = (unsigned char *)malloc(uvSize);
            planes[2] = (unsigned char *)malloc(uvSize);
        }

        if (planes[0]) {
            tjDecompressToYUVPlanes(handle, Data, Size, planes, width, strides, height, 0);
        }

        free(planes[0]);
        free(planes[1]);
        free(planes[2]);
    }
    tjDestroy(handle);
    return 0;
}

int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Run all fuzzing functions
    fuzz_tjDecompressHeader2(Data, Size);
    fuzz_tjDecompress(Data, Size);
    fuzz_tjDecompressHeader3(Data, Size);
    fuzz_tj3DecompressToYUVPlanes8(Data, Size);
    fuzz_tjDecompressHeader(Data, Size);
    fuzz_tjDecompressToYUVPlanes(Data, Size);

    return 0;
}