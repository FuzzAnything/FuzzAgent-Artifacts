// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3LoadImage16 at turbojpeg-mp.c:293:21 in turbojpeg.h
// tj3JPEGBufSize at turbojpeg.c:903:18 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Compress16 at turbojpeg-mp.c:71:15 in turbojpeg.h
// tj3Decompress16 at turbojpeg-mp.c:148:15 in turbojpeg.h
// tj3SaveImage16 at turbojpeg-mp.c:442:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3LoadImage16 at turbojpeg-mp.c:293:21 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3JPEGBufSize at turbojpeg.c:903:18 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "turbojpeg.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static void remove_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    write_dummy_file(Data, Size);
    
    tjhandle handle = tj3Init(TJINIT_COMPRESS);
    if (!handle) {
        remove_dummy_file();
        return 0;
    }
    
    // Test tj3LoadImage16
    int width = 0, height = 0, pixelFormat = TJPF_UNKNOWN;
    unsigned short *loadedImage = tj3LoadImage16(handle, "./dummy_file", &width, 1, &height, &pixelFormat);
    
    if (loadedImage) {
        // Test tj3JPEGBufSize with various parameters
        size_t bufSize = tj3JPEGBufSize(width, height, TJSAMP_444);
        if (bufSize > 0 && bufSize < 100 * 1024 * 1024) { // Reasonable size limit
            // Test tj3Alloc
            unsigned char *jpegBuf = (unsigned char *)tj3Alloc(bufSize);
            size_t jpegSize = bufSize;
            
            if (jpegBuf) {
                // Test tj3Compress16
                int compressResult = tj3Compress16(handle, loadedImage, width, 0, height, pixelFormat, &jpegBuf, &jpegSize);
                
                if (compressResult == 0 && jpegBuf && jpegSize > 0 && jpegSize <= bufSize) {
                    // Test tj3Decompress16
                    unsigned short *decompressedBuf = NULL;
                    if (width > 0 && height > 0) {
                        int pitch = width * tjPixelSize[pixelFormat];
                        decompressedBuf = (unsigned short *)malloc(pitch * height * sizeof(unsigned short));
                        if (decompressedBuf) {
                            int decompressResult = tj3Decompress16(handle, jpegBuf, jpegSize, decompressedBuf, pitch, pixelFormat);
                            (void)decompressResult; // Result ignored for fuzzing
                            free(decompressedBuf);
                        }
                    }
                    
                    // Test tj3SaveImage16
                    tj3SaveImage16(handle, "./dummy_file", loadedImage, width, 0, height, pixelFormat);
                }
                
                tj3Free(jpegBuf);
            }
        }
        
        tj3Free(loadedImage);
    }
    
    // Test with different pixel formats and alignments
    for (int i = 0; i < 4 && Size > 20; i++) {
        int testWidth = (Data[i] << 8) | Data[i+1];
        int testHeight = (Data[i+2] << 8) | Data[i+3];
        int testAlign = 1 << (Data[i+4] % 4); // 1, 2, 4, or 8
        int testPixelFormat = Data[i+5] % (TJPF_CMYK + 1);
        
        if (testWidth > 0 && testWidth < 4096 && testHeight > 0 && testHeight < 4096) {
            width = 0; height = 0; pixelFormat = testPixelFormat;
            unsigned short *testImage = tj3LoadImage16(handle, "./dummy_file", &width, testAlign, &height, &pixelFormat);
            if (testImage) {
                tj3Free(testImage);
            }
        }
    }
    
    // Test tj3JPEGBufSize with various subsampling modes
    for (int subsamp = TJSAMP_444; subsamp <= TJSAMP_411; subsamp++) {
        size_t testBufSize = tj3JPEGBufSize(100, 100, subsamp);
        (void)testBufSize; // Result ignored for fuzzing
    }
    
    tj3Destroy(handle);
    remove_dummy_file();
    
    return 0;
}