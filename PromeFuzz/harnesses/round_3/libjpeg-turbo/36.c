// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjPlaneSizeYUV at turbojpeg.c:1048:25 in turbojpeg.h
// TJBUFSIZEYUV at turbojpeg.c:1013:25 in turbojpeg.h
// tjBufSizeYUV at turbojpeg.c:1007:25 in turbojpeg.h
// tjBufSizeYUV2 at turbojpeg.c:999:25 in turbojpeg.h
// tjDecompressHeader2 at turbojpeg.c:1903:15 in turbojpeg.h
// tjDecodeYUVPlanes at turbojpeg.c:2649:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "src/turbojpeg.h"

static tjhandle create_decompress_handle(void) {
    return tjInitDecompress();
}

static void destroy_decompress_handle(tjhandle handle) {
    if (handle) {
        tjDestroy(handle);
    }
}

static tjhandle create_decompress_handle_for_yuv(void) {
    return tjInitDecompress();
}

int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    // Extract parameters from fuzzer input
    // We'll use the first few bytes to determine parameters
    int componentID = (Data[0] % 3);  // 0, 1, or 2 for Y, U, V
    int width = (Data[1] % 256) + 1;  // Ensure width > 0
    int stride = (Data[2] % 256) + 1; // Ensure stride > 0
    int height = (Data[3] % 256) + 1; // Ensure height > 0
    int subsamp = (Data[0] % 5);      // 0-4 for valid subsampling values
    
    // For alignment parameter
    int align = (Data[1] % 32) + 1;   // Ensure align > 0
    
    // Test tjPlaneSizeYUV
    tjPlaneSizeYUV(componentID, width, stride, height, subsamp);
    
    // Test TJBUFSIZEYUV macro
    TJBUFSIZEYUV(width, height, subsamp);
    
    // Test tjBufSizeYUV
    tjBufSizeYUV(width, height, subsamp);
    
    // Test tjBufSizeYUV2
    tjBufSizeYUV2(width, align, height, subsamp);
    
    // Test tjDecompressHeader2
    if (Size >= 100) {
        tjhandle handle = create_decompress_handle();
        if (handle) {
            int out_width, out_height, out_subsamp;
            unsigned char *jpegBuf = (unsigned char *)Data;
            unsigned long jpegSize = Size > 65535 ? 65535 : Size;
            
            // Try to decompress header
            tjDecompressHeader2(handle, jpegBuf, jpegSize, 
                               &out_width, &out_height, &out_subsamp);
            
            destroy_decompress_handle(handle);
        }
    }
    
    // Test tjDecodeYUVPlanes
    if (Size >= 200) {
        tjhandle handle = create_decompress_handle_for_yuv();
        if (handle) {
            const unsigned char *srcPlanes[3];
            int strides[3];
            unsigned char dstBuf[256 * 256 * 4]; // Reasonable max buffer
            
            // Set up plane pointers and strides
            for (int i = 0; i < 3; i++) {
                srcPlanes[i] = Data + (i * 64);
                strides[i] = (Data[i] % 64) + 1;
            }
            
            int pixelFormat = (Data[0] % 12); // Various pixel formats
            int flags = (Data[1] % 256);
            
            // Ensure width and height are reasonable for our buffer
            int safe_width = width % 256;
            int safe_height = height % 256;
            int pitch = safe_width * 4; // Assume 4 bytes per pixel
            
            tjDecodeYUVPlanes(handle, srcPlanes, strides, subsamp,
                             dstBuf, safe_width, pitch, safe_height,
                             pixelFormat, flags);
            
            destroy_decompress_handle(handle);
        }
    }
    
    // Also test with a dummy file
    if (Size > 0) {
        FILE *fp = fopen("./dummy_file", "wb");
        if (fp) {
            fwrite(Data, 1, Size > 1024 ? 1024 : Size, fp);
            fclose(fp);
        }
    }
    
    return 0;
}