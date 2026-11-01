// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjDecompressHeader3 at turbojpeg.c:1874:15 in turbojpeg.h
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
// tjDecompressToYUV2 at turbojpeg.c:2401:15 in turbojpeg.h
// tj3DecompressToYUV8 at turbojpeg.c:2338:15 in turbojpeg.h
// tjDecompressHeader2 at turbojpeg.c:1903:15 in turbojpeg.h
// tjDecompress at turbojpeg.c:2108:15 in turbojpeg.h
// tjDecompress2 at turbojpeg.c:2059:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <turbojpeg.h>

static void *safe_malloc(size_t size) {
    void *ptr = malloc(size);
    if (ptr) {
        memset(ptr, 0, size);
    }
    return ptr;
}

int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
    if (Size < 10) {
        return 0;
    }

    tjhandle handle = tjInitDecompress();
    if (!handle) {
        return 0;
    }

    const unsigned char *jpegBuf = Data;
    unsigned long jpegSize = Size;

    int width = 0, height = 0, jpegSubsamp = 0, jpegColorspace = 0;
    int ret = 0;

    // 1. Parse header to get dimensions
    ret = tjDecompressHeader3(handle, jpegBuf, jpegSize, &width, &height, &jpegSubsamp, &jpegColorspace);
    if (ret == 0) {
        // Ensure dimensions are within reasonable bounds to avoid OOM
        if (width > 0 && height > 0 && width < 8192 && height < 8192) {
            // Allocate buffers
            size_t yuvSize = tj3YUVBufSize(width, 4, height, jpegSubsamp);
            unsigned char *yuvBuf = safe_malloc(yuvSize);
            
            // Calculate RGB size
            int pixelSize = 4; // Assume BGRX/RGBA max
            size_t rgbSize = (size_t)width * height * pixelSize;
            unsigned char *rgbBuf = safe_malloc(rgbSize);

            if (yuvBuf && rgbBuf) {
                int align = 4; // Power of 2
                
                // 2. Fuzz tjDecompressToYUV2
                // Try original size (0, 0) and scaled down
                int w1 = (Size % 2 == 0) ? 0 : width / 2;
                int h1 = (Size % 3 == 0) ? 0 : height / 2;
                tjDecompressToYUV2(handle, jpegBuf, jpegSize, yuvBuf, w1, align, h1, 0);

                // 3. Fuzz tj3DecompressToYUV8
                tj3DecompressToYUV8(handle, jpegBuf, jpegSize, yuvBuf, align);

                // 4. Fuzz tjDecompressHeader2
                int w2, h2, subsamp2;
                tjDecompressHeader2(handle, (unsigned char *)jpegBuf, jpegSize, &w2, &h2, &subsamp2);

                // 5. Fuzz tjDecompress
                int pitch = width * pixelSize;
                int flags = (Size % 2 == 0) ? TJ_YUV : 0;
                tjDecompress(handle, (unsigned char *)jpegBuf, jpegSize, rgbBuf, width, pitch, height, pixelSize, flags);

                // 6. Fuzz tjDecompress2
                int pixelFormat = (Size % 3 == 0) ? TJPF_RGB : TJPF_BGR;
                tjDecompress2(handle, jpegBuf, jpegSize, rgbBuf, width, pitch, height, pixelFormat, 0);
            }

            free(yuvBuf);
            free(rgbBuf);
        }
    }

    tjDestroy(handle);
    return 0;
}