// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjDecompressHeader2 at turbojpeg.c:1903:15 in turbojpeg.h
// tjDecompressHeader at turbojpeg.c:1914:15 in turbojpeg.h
// tjDecompress at turbojpeg.c:2108:15 in turbojpeg.h
// tjDecompressToYUV2 at turbojpeg.c:2401:15 in turbojpeg.h
// tj3SetScalingFactor at turbojpeg.c:1981:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "turbojpeg.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    tjhandle handle = tjInitDecompress();
    if (!handle) return 0;
    
    /* Prepare JPEG buffer from fuzzer input */
    unsigned char *jpegBuf = (unsigned char *)Data;
    unsigned long jpegSize = Size;
    
    /* Variables for decompression */
    int width = 0, height = 0, jpegSubsamp = 0;
    int pitch = 0, pixelSize = 3, flags = 0;
    unsigned char *dstBuf = NULL;
    
    /* Try tjDecompressHeader2 first to get dimensions */
    if (tjDecompressHeader2(handle, jpegBuf, jpegSize, &width, &height, &jpegSubsamp) == 0) {
        /* Try tjDecompressHeader as well */
        int width2 = 0, height2 = 0;
        tjDecompressHeader(handle, jpegBuf, jpegSize, &width2, &height2);
        
        /* Allocate destination buffer if dimensions are valid */
        if (width > 0 && height > 0) {
            pitch = width * pixelSize;
            dstBuf = (unsigned char *)malloc(pitch * height);
            if (dstBuf) {
                /* Try decompression with different flags */
                for (int i = 0; i < 4; i++) {
                    flags = i;
                    tjDecompress(handle, jpegBuf, jpegSize, dstBuf, width, pitch, height, pixelSize, flags);
                }
                free(dstBuf);
            }
            
            /* Try tjDecompressToYUV2 */
            int align = 1;
            unsigned long yuvSize = width * height * 2; /* Conservative estimate */
            unsigned char *yuvBuf = (unsigned char *)malloc(yuvSize);
            if (yuvBuf) {
                for (int i = 0; i < 4; i++) {
                    flags = i;
                    tjDecompressToYUV2(handle, jpegBuf, jpegSize, yuvBuf, width, align, height, flags);
                }
                free(yuvBuf);
            }
        }
    }
    
    /* Try tj3SetScalingFactor with different scaling factors */
    tjscalingfactor scalingFactors[] = {
        {1, 1}, {1, 2}, {3, 4}, {1, 4}, {1, 8}
    };
    for (int i = 0; i < 5; i++) {
        tj3SetScalingFactor(handle, scalingFactors[i]);
    }
    
    /* Write to dummy file and try file-based operations */
    write_dummy_file(Data, Size);
    
    /* Clean up */
    tjDestroy(handle);
    
    return 0;
}