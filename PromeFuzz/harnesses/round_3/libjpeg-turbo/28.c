// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3Decompress16 at turbojpeg-mp.c:148:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3LoadImage16 at turbojpeg-mp.c:293:21 in turbojpeg.h
// tj3SaveImage16 at turbojpeg-mp.c:442:15 in turbojpeg.h
// tj3Compress16 at turbojpeg-mp.c:71:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3JPEGBufSize at turbojpeg.c:903:18 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Compress16 at turbojpeg-mp.c:71:15 in turbojpeg.h
// tj3Compress16 at turbojpeg-mp.c:71:15 in turbojpeg.h
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
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static void cleanup_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
    if (Size < 16) return 0;
    
    // Initialize TurboJPEG instances
    tjhandle handle_compress = tj3Init(TJINIT_COMPRESS);
    tjhandle handle_decompress = tj3Init(TJINIT_DECOMPRESS);
    tjhandle handle_general = tj3Init(TJINIT_COMPRESS); // Can be used for both
    
    if (!handle_compress || !handle_decompress || !handle_general) {
        if (handle_compress) tj3Destroy(handle_compress);
        if (handle_decompress) tj3Destroy(handle_decompress);
        if (handle_general) tj3Destroy(handle_general);
        return 0;
    }
    
    // Set precision to 16-bit
    tj3Set(handle_compress, TJPARAM_PRECISION, 16);
    tj3Set(handle_decompress, TJPARAM_PRECISION, 16);
    tj3Set(handle_general, TJPARAM_PRECISION, 16);
    
    // Extract parameters from fuzzer input
    int width = (Data[0] % 256) + 1;
    int height = (Data[1] % 256) + 1;
    int pixelFormat = Data[2] % 12; // TJPF values 0-11
    int align = 1 << (Data[3] % 5); // 1,2,4,8,16
    int jpegSubsamp = Data[4] % 5; // TJSAMP values 0-4
    
    // Ensure reasonable dimensions
    if (width > 1024) width = 1024;
    if (height > 1024) height = 1024;
    
    // Calculate pixel size
    int pixelSize = tjPixelSize[pixelFormat];
    if (pixelSize <= 0) pixelSize = 3; // Default to RGB
    
    // Create source image buffer
    size_t srcBufSize = width * height * pixelSize * sizeof(unsigned short);
    if (srcBufSize == 0 || srcBufSize > 100 * 1024 * 1024) {
        srcBufSize = 1024 * 1024; // 1MB max
    }
    
    unsigned short *srcBuf = (unsigned short *)malloc(srcBufSize);
    if (!srcBuf) {
        tj3Destroy(handle_compress);
        tj3Destroy(handle_decompress);
        tj3Destroy(handle_general);
        return 0;
    }
    
    // Fill source buffer with fuzzer data
    size_t copySize = Size - 16;
    if (copySize > srcBufSize) copySize = srcBufSize;
    memcpy(srcBuf, Data + 16, copySize);
    
    // Test tj3JPEGBufSize
    size_t maxBufSize = tj3JPEGBufSize(width, height, jpegSubsamp);
    
    // Test tj3Alloc
    size_t allocSize = maxBufSize > 0 ? maxBufSize : 1024;
    unsigned char *jpegBuf = (unsigned char *)tj3Alloc(allocSize);
    
    // Test tj3Compress16
    size_t jpegSize = allocSize;
    int pitch = width * pixelSize;
    
    if (jpegBuf) {
        // Try compression with different buffer strategies
        unsigned char *jpegBuf2 = NULL;
        size_t jpegSize2 = 0;
        
        // Test with pre-allocated buffer
        int result1 = tj3Compress16(handle_compress, srcBuf, width, pitch, 
                                   height, pixelFormat, &jpegBuf, &jpegSize);
        
        // Test with NULL buffer (let library allocate)
        int result2 = tj3Compress16(handle_general, srcBuf, width, pitch, 
                                   height, pixelFormat, &jpegBuf2, &jpegSize2);
        
        // Test tj3Decompress16 if we have valid JPEG data
        if (result1 == 0 && jpegSize > 0 && jpegSize <= allocSize) {
            // Allocate destination buffer for decompression
            size_t dstBufSize = width * height * pixelSize * sizeof(unsigned short);
            unsigned short *dstBuf = (unsigned short *)malloc(dstBufSize);
            if (dstBuf) {
                tj3Decompress16(handle_decompress, jpegBuf, jpegSize, 
                               dstBuf, pitch, pixelFormat);
                free(dstBuf);
            }
        }
        
        // Free library-allocated buffer
        if (jpegBuf2) {
            tj3Free(jpegBuf2);
        }
    }
    
    // Test file operations if we have enough data
    if (Size > 100) {
        write_dummy_file(Data, Size);
        
        // Test tj3LoadImage16
        int loadedWidth, loadedHeight, loadedPixelFormat;
        unsigned short *loadedBuf = tj3LoadImage16(handle_general, "./dummy_file", 
                                                  &loadedWidth, align, 
                                                  &loadedHeight, &loadedPixelFormat);
        
        if (loadedBuf) {
            // Test tj3SaveImage16 with loaded image
            if (loadedWidth > 0 && loadedHeight > 0) {
                tj3SaveImage16(handle_general, "./dummy_file2", loadedBuf, 
                              loadedWidth, 0, loadedHeight, loadedPixelFormat);
                remove("./dummy_file2");
            }
            
            // Test compression with loaded image
            unsigned char *jpegBuf3 = NULL;
            size_t jpegSize3 = 0;
            if (loadedWidth > 0 && loadedHeight > 0) {
                int loadedPitch = loadedWidth * tjPixelSize[loadedPixelFormat];
                if (loadedPitch > 0) {
                    tj3Compress16(handle_compress, loadedBuf, loadedWidth, 
                                 loadedPitch, loadedHeight, loadedPixelFormat, 
                                 &jpegBuf3, &jpegSize3);
                }
            }
            
            if (jpegBuf3) {
                tj3Free(jpegBuf3);
            }
            tj3Free(loadedBuf);
        }
        
        cleanup_dummy_file();
    }
    
    // Cleanup
    if (jpegBuf) {
        tj3Free(jpegBuf);
    }
    free(srcBuf);
    
    tj3Destroy(handle_compress);
    tj3Destroy(handle_decompress);
    tj3Destroy(handle_general);
    
    return 0;
}