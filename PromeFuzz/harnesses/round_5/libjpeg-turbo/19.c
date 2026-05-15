// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjDecompress2 at turbojpeg.c:2059:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3GetScalingFactors at turbojpeg.c:1959:28 in turbojpeg.h
// tjGetScalingFactors at turbojpeg.c:1974:28 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjDecompressHeader at turbojpeg.c:1914:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3GetScalingFactors at turbojpeg.c:1959:28 in turbojpeg.h
// tj3SetScalingFactor at turbojpeg.c:1981:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3DecompressHeader at turbojpeg.c:1815:15 in turbojpeg.h
// tj3SetCroppingRegion at turbojpeg.c:2006:15 in turbojpeg.h
// tj3SetCroppingRegion at turbojpeg.c:2006:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "turbojpeg.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void fuzz_tj3GetScalingFactors(void) {
    int numScalingFactors;
    tjscalingfactor *factors = tj3GetScalingFactors(&numScalingFactors);
    (void)factors; // Use result to avoid unused variable warning
}

static void fuzz_tjGetScalingFactors(void) {
    int numScalingFactors;
    tjscalingfactor *factors = tjGetScalingFactors(&numScalingFactors);
    (void)factors; // Use result to avoid unused variable warning
}

static void fuzz_tjDecompressHeader(const uint8_t *data, size_t size) {
    tjhandle handle = tjInitDecompress();
    if (!handle) return;
    
    int width, height;
    unsigned long jpegSize = (unsigned long)size;
    unsigned char *jpegBuf = (unsigned char*)data;
    
    // Try decompress header with provided data
    tjDecompressHeader(handle, jpegBuf, jpegSize, &width, &height);
    
    tjDestroy(handle);
}

static void fuzz_tj3SetScalingFactor(const uint8_t *data, size_t size) {
    tjhandle handle = tj3Init(TJINIT_DECOMPRESS);
    if (!handle) return;
    
    // Get available scaling factors
    int numFactors;
    tjscalingfactor *factors = tj3GetScalingFactors(&numFactors);
    
    if (factors && numFactors > 0 && size > 0) {
        // Use first byte of fuzz data to select scaling factor
        int idx = data[0] % numFactors;
        tj3SetScalingFactor(handle, factors[idx]);
    }
    
    tj3Destroy(handle);
}

static void fuzz_tj3SetCroppingRegion(const uint8_t *data, size_t size) {
    tjhandle handle = tj3Init(TJINIT_DECOMPRESS);
    if (!handle) return;
    
    // Create a dummy JPEG header to allow setting cropping region
    if (size > 0) {
        // Write fuzz data to a dummy file
        FILE *f = fopen("./dummy_file", "wb");
        if (f) {
            fwrite(data, 1, size, f);
            fclose(f);
            
            // Try to read header from dummy file
            FILE *jpegFile = fopen("./dummy_file", "rb");
            if (jpegFile) {
                fseek(jpegFile, 0, SEEK_END);
                long fileSize = ftell(jpegFile);
                fseek(jpegFile, 0, SEEK_SET);
                
                if (fileSize > 0) {
                    unsigned char *jpegBuf = (unsigned char*)malloc(fileSize);
                    if (jpegBuf) {
                        fread(jpegBuf, 1, fileSize, jpegFile);
                        
                        // Read JPEG header
                        if (tj3DecompressHeader(handle, jpegBuf, fileSize) == 0) {
                            // Create cropping region from fuzz data
                            tjregion region;
                            if (size >= 16) {
                                // Use fuzz data to populate region fields
                                region.x = *(int*)(data);
                                region.y = *(int*)(data + 4);
                                region.w = *(int*)(data + 8);
                                region.h = *(int*)(data + 12);
                                
                                // Try setting cropping region
                                tj3SetCroppingRegion(handle, region);
                            } else {
                                // Try with no cropping
                                tj3SetCroppingRegion(handle, TJUNCROPPED);
                            }
                        }
                        
                        free(jpegBuf);
                    }
                }
                fclose(jpegFile);
            }
        }
    }
    
    tj3Destroy(handle);
}

static void fuzz_tjDecompress2(const uint8_t *data, size_t size) {
    tjhandle handle = tjInitDecompress();
    if (!handle) return;
    
    if (size > 0) {
        // Try to decompress with fuzz data as JPEG
        unsigned long jpegSize = (unsigned long)size;
        unsigned char *jpegBuf = (unsigned char*)data;
        
        int width = 100;
        int height = 100;
        int pitch = width * 3; // RGB format
        int pixelFormat = TJPF_RGB;
        unsigned char *dstBuf = (unsigned char*)malloc(pitch * height);
        
        if (dstBuf) {
            tjDecompress2(handle, jpegBuf, jpegSize, dstBuf, 
                         width, pitch, height, pixelFormat, 0);
            free(dstBuf);
        }
    }
    
    tjDestroy(handle);
}

int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
    // Test all API functions with the fuzz input
    
    // 1. Test tj3GetScalingFactors
    fuzz_tj3GetScalingFactors();
    
    // 2. Test tjGetScalingFactors
    fuzz_tjGetScalingFactors();
    
    // 3. Test tjDecompressHeader
    fuzz_tjDecompressHeader(Data, Size);
    
    // 4. Test tj3SetScalingFactor
    fuzz_tj3SetScalingFactor(Data, Size);
    
    // 5. Test tj3SetCroppingRegion
    fuzz_tj3SetCroppingRegion(Data, Size);
    
    // 6. Test tjDecompress2
    fuzz_tjDecompress2(Data, Size);
    
    return 0;
}