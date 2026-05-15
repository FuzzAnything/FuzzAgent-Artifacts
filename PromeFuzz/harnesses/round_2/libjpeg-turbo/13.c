// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjDecompressHeader2 at turbojpeg.c:1903:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tjDecompressHeader2 at turbojpeg.c:1903:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tjBufSizeYUV2 at turbojpeg.c:999:25 in turbojpeg.h
// tjBufSize at turbojpeg.c:933:25 in turbojpeg.h
// tjDecompressToYUVPlanes at turbojpeg.c:2288:15 in turbojpeg.h
// tjDecompressToYUV2 at turbojpeg.c:2401:15 in turbojpeg.h
// tj3DecompressToYUV8 at turbojpeg.c:2338:15 in turbojpeg.h
// tjDecompressToYUV at turbojpeg.c:2449:15 in turbojpeg.h
// tjDecompress2 at turbojpeg.c:2059:15 in turbojpeg.h
// tjDecompressHeader2 at turbojpeg.c:1903:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "turbojpeg.h"

#define MAX_DIMENSION 4096
#define MAX_PLANES 3

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void remove_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    tjhandle handle = NULL;
    tjhandle handle3 = NULL;
    int width = 0, height = 0, jpegSubsamp = 0;
    unsigned char *dstBuf = NULL;
    unsigned char *dstPlanes[MAX_PLANES] = {NULL};
    int strides[MAX_PLANES] = {0};
    int align = 1;
    int pitch = 0;
    int pixelFormat = TJPF_RGB;
    int flags = 0;
    unsigned long jpegSize = Size;
    
    // Initialize handles
    handle = tjInitDecompress();
    if (!handle) goto cleanup;
    
    handle3 = tj3Init(TJINIT_DECOMPRESS);
    if (!handle3) goto cleanup;
    
    // Use first byte to determine some parameters
    align = 1 << (Data[0] % 4);  // 1, 2, 4, or 8
    flags = Data[1] & 0xF;
    pixelFormat = TJPF_RGB + (Data[2] % 12);  // Valid pixel formats
    
    // Try tjDecompressHeader2 first to get dimensions
    if (tjDecompressHeader2(handle, (unsigned char *)Data, jpegSize, 
                           &width, &height, &jpegSubsamp) == 0) {
        // Cap dimensions to avoid excessive memory allocation
        if (width > MAX_DIMENSION) width = MAX_DIMENSION;
        if (height > MAX_DIMENSION) height = MAX_DIMENSION;
        
        // Set parameters for new API
        tj3Set(handle3, TJPARAM_JPEGWIDTH, width);
        tj3Set(handle3, TJPARAM_JPEGHEIGHT, height);
        tj3Set(handle3, TJPARAM_SUBSAMP, jpegSubsamp);
        
        // Calculate buffer sizes
        unsigned long yuvBufSize = tjBufSizeYUV2(width, align, height, jpegSubsamp);
        unsigned long rgbBufSize = tjBufSize(width, height, jpegSubsamp);
        
        // Allocate buffers
        dstBuf = (unsigned char *)malloc(yuvBufSize > rgbBufSize ? yuvBufSize : rgbBufSize);
        if (!dstBuf) goto cleanup;
        
        // Set strides for planes
        for (int i = 0; i < MAX_PLANES; i++) {
            strides[i] = width;
            dstPlanes[i] = (unsigned char *)malloc(width * height);
        }
        
        pitch = width * tjPixelSize[pixelFormat];
        
        // Test various decompression functions
        tjDecompressToYUVPlanes(handle, Data, jpegSize, dstPlanes, width,
                               strides, height, flags);
        
        tjDecompressToYUV2(handle, Data, jpegSize, dstBuf, width, 
                          align, height, flags);
        
        tj3DecompressToYUV8(handle3, Data, jpegSize, dstBuf, align);
        
        tjDecompressToYUV(handle, (unsigned char *)Data, jpegSize, 
                         dstBuf, flags);
        
        tjDecompress2(handle, Data, jpegSize, dstBuf, width, pitch,
                     height, pixelFormat, flags);
    }
    
    // Also test with just the input data without valid header
    tjDecompressHeader2(handle, (unsigned char *)Data, jpegSize, 
                       &width, &height, &jpegSubsamp);
    
    // Test with file-based approach
    write_dummy_file(Data, Size);
    FILE *fp = fopen("./dummy_file", "rb");
    if (fp) {
        fseek(fp, 0, SEEK_END);
        long fileSize = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        
        if (fileSize > 0) {
            unsigned char *fileData = (unsigned char *)malloc(fileSize);
            if (fileData) {
                fread(fileData, 1, fileSize, fp);
                tjDecompressHeader2(handle, fileData, fileSize, 
                                   &width, &height, &jpegSubsamp);
                free(fileData);
            }
        }
        fclose(fp);
    }
    remove_dummy_file();

cleanup:
    // Free allocated memory
    if (dstBuf) free(dstBuf);
    for (int i = 0; i < MAX_PLANES; i++) {
        if (dstPlanes[i]) free(dstPlanes[i]);
    }
    if (handle) tjDestroy(handle);
    if (handle3) tj3Destroy(handle3);
    
    return 0;
}