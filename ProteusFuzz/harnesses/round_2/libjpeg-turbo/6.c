// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjDecompressHeader3 at turbojpeg.c:1874:15 in turbojpeg.h
// tjDecompressHeader2 at turbojpeg.c:1903:15 in turbojpeg.h
// tjDecompressHeader at turbojpeg.c:1914:15 in turbojpeg.h
// tjDecompress2 at turbojpeg.c:2059:15 in turbojpeg.h
// tjDecompress at turbojpeg.c:2108:15 in turbojpeg.h
// tjDecompressToYUV2 at turbojpeg.c:2401:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <turbojpeg.h>

static void cleanup_buffer(unsigned char *buf) {
    if (buf) {
        free(buf);
    }
}

static void cleanup_handle(tjhandle handle) {
    if (handle) {
        tjDestroy(handle);
    }
}

int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    tjhandle handle = tjInitDecompress();
    if (!handle) {
        return 0;
    }

    // Ensure we have enough data for a minimal JPEG structure if needed
    // though the library should handle malformed data gracefully.
    
    int width = 0, height = 0, jpegSubsamp = 0, jpegColorspace = 0;
    int ret;

    // 1. Test tjDecompressHeader3
    ret = tjDecompressHeader3(handle, Data, Size, &width, &height, &jpegSubsamp, &jpegColorspace);
    // Ignore return value for fuzzing, we want to see if it crashes

    // 2. Test tjDecompressHeader2
    width = 0; height = 0; jpegSubsamp = 0;
    ret = tjDecompressHeader2(handle, Data, Size, &width, &height, &jpegSubsamp);

    // 3. Test tjDecompressHeader
    width = 0; height = 0;
    ret = tjDecompressHeader(handle, Data, Size, &width, &height);

    // Prepare buffers for decompression tests
    // Allocate a large buffer to prevent out-of-bounds writes if the library misbehaves
    // We use a safe upper bound based on input size or a fixed large size
    size_t max_size = (Size > 0) ? Size * 4 : 0;
    if (max_size == 0) max_size = 1024 * 1024; // Fallback
    unsigned char *dstBuf = (unsigned char *)malloc(max_size);
    if (!dstBuf) {
        cleanup_handle(handle);
        return 0;
    }

    // 4. Test tjDecompress2
    // width=0, height=0 means use original dimensions
    // pitch=0 means default pitch
    ret = tjDecompress2(handle, Data, Size, dstBuf, 0, 0, 0, TJPF_RGB, 0);

    // 5. Test tjDecompress
    // Reset buffer or reuse
    ret = tjDecompress(handle, Data, Size, dstBuf, 0, 0, 0, 0, 0);

    // 6. Test tjDecompressToYUV2
    ret = tjDecompressToYUV2(handle, Data, Size, dstBuf, 0, 0, 0, 0);

    cleanup_buffer(dstBuf);
    cleanup_handle(handle);

    return 0;
}