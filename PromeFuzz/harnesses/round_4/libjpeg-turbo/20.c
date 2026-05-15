// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjDecompress2 at turbojpeg.c:2059:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjDecompressHeader at turbojpeg.c:1914:15 in turbojpeg.h
// tjDecompressHeader2 at turbojpeg.c:1903:15 in turbojpeg.h
// tjDecompressHeader3 at turbojpeg.c:1874:15 in turbojpeg.h
// tjDecompressToYUV at turbojpeg.c:2449:15 in turbojpeg.h
// tjDecompressToYUV2 at turbojpeg.c:2401:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "src/turbojpeg.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DIMENSION 4096
#define DUMMY_FILE "./dummy_file"

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen(DUMMY_FILE, "wb");
    if (!fp) return;
    fwrite(data, 1, size, fp);
    fclose(fp);
}

static void cleanup_dummy_file(void) {
    remove(DUMMY_FILE);
}

int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    // Write input to dummy file for any potential file-based operations
    write_dummy_file(Data, Size);
    
    tjhandle handle = tjInitDecompress();
    if (!handle) {
        cleanup_dummy_file();
        return 0;
    }
    
    // Variables for header extraction
    int width = 0, height = 0, jpegSubsamp = 0, jpegColorspace = 0;
    unsigned long jpegSize = Size;
    
    // Test tjDecompressHeader
    if (Size > 0) {
        tjDecompressHeader(handle, (unsigned char *)Data, jpegSize, &width, &height);
    }
    
    // Test tjDecompressHeader2
    if (Size > 0) {
        tjDecompressHeader2(handle, (unsigned char *)Data, jpegSize, &width, &height, &jpegSubsamp);
    }
    
    // Test tjDecompressHeader3
    if (Size > 0) {
        tjDecompressHeader3(handle, Data, jpegSize, &width, &height, &jpegSubsamp, &jpegColorspace);
    }
    
    // Cap dimensions to avoid excessive memory allocation
    if (width > MAX_DIMENSION) width = MAX_DIMENSION;
    if (height > MAX_DIMENSION) height = MAX_DIMENSION;
    if (width < 1) width = 1;
    if (height < 1) height = 1;
    
    // Test tjDecompressToYUV if we have valid dimensions
    if (Size > 0 && width > 0 && height > 0) {
        // Calculate buffer size for YUV (planar format)
        unsigned long yuvSize = width * height * 3 / 2;
        if (yuvSize > 0 && yuvSize < 100 * 1024 * 1024) { // Limit to 100MB
            unsigned char *dstBuf = (unsigned char *)malloc(yuvSize);
            if (dstBuf) {
                tjDecompressToYUV(handle, (unsigned char *)Data, jpegSize, dstBuf, 0);
                free(dstBuf);
            }
        }
    }
    
    // Test tjDecompressToYUV2
    if (Size > 0 && width > 0 && height > 0) {
        unsigned long yuvSize = width * height * 3 / 2;
        if (yuvSize > 0 && yuvSize < 100 * 1024 * 1024) {
            unsigned char *dstBuf = (unsigned char *)malloc(yuvSize);
            if (dstBuf) {
                tjDecompressToYUV2(handle, Data, jpegSize, dstBuf, width, 1, height, 0);
                free(dstBuf);
            }
        }
    }
    
    // Test tjDecompress2 with different pixel formats
    if (Size > 0 && width > 0 && height > 0) {
        // Try different pixel formats
        int pixelFormats[] = {TJPF_RGB, TJPF_BGR, TJPF_RGBX, TJPF_BGRX, TJPF_XRGB, TJPF_XBGR, TJPF_GRAY};
        int numFormats = sizeof(pixelFormats) / sizeof(pixelFormats[0]);
        
        for (int i = 0; i < numFormats; i++) {
            int pixelSize = 3; // Default for RGB/BGR
            if (pixelFormats[i] == TJPF_RGBX || pixelFormats[i] == TJPF_BGRX || 
                pixelFormats[i] == TJPF_XRGB || pixelFormats[i] == TJPF_XBGR) {
                pixelSize = 4;
            } else if (pixelFormats[i] == TJPF_GRAY) {
                pixelSize = 1;
            }
            
            unsigned long bufSize = width * height * pixelSize;
            if (bufSize > 0 && bufSize < 100 * 1024 * 1024) {
                unsigned char *dstBuf = (unsigned char *)malloc(bufSize);
                if (dstBuf) {
                    int pitch = width * pixelSize;
                    tjDecompress2(handle, Data, jpegSize, dstBuf, width, pitch, height, pixelFormats[i], 0);
                    free(dstBuf);
                }
            }
        }
    }
    
    // Cleanup
    tjDestroy(handle);
    cleanup_dummy_file();
    
    return 0;
}