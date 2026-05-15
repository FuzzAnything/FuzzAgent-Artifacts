// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjDecompress2 at turbojpeg.c:2059:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3GetScalingFactors at turbojpeg.c:1959:28 in turbojpeg.h
// tj3GetScalingFactors at turbojpeg.c:1959:28 in turbojpeg.h
// tjGetScalingFactors at turbojpeg.c:1974:28 in turbojpeg.h
// tjGetScalingFactors at turbojpeg.c:1974:28 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tjDecompressHeader at turbojpeg.c:1914:15 in turbojpeg.h
// tjDecompressHeader at turbojpeg.c:1914:15 in turbojpeg.h
// tj3GetScalingFactors at turbojpeg.c:1959:28 in turbojpeg.h
// tj3SetScalingFactor at turbojpeg.c:1981:15 in turbojpeg.h
// tj3SetScalingFactor at turbojpeg.c:1981:15 in turbojpeg.h
// tjDecompressHeader at turbojpeg.c:1914:15 in turbojpeg.h
// tj3SetCroppingRegion at turbojpeg.c:2006:15 in turbojpeg.h
// tj3SetCroppingRegion at turbojpeg.c:2006:15 in turbojpeg.h
// tj3SetCroppingRegion at turbojpeg.c:2006:15 in turbojpeg.h
// tjDecompressHeader at turbojpeg.c:1914:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "turbojpeg.h"

int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size) {
    // Initialize dummy JPEG buffer from fuzzer input
    unsigned char *jpegBuf = (unsigned char *)Data;
    unsigned long jpegSize = Size;
    
    // Test tj3GetScalingFactors
    {
        int numScalingFactors = 0;
        tjscalingfactor *factors = tj3GetScalingFactors(&numScalingFactors);
        // Also test with NULL pointer
        tj3GetScalingFactors(NULL);
    }
    
    // Test tjGetScalingFactors
    {
        int numscalingfactors = 0;
        tjscalingfactor *old_factors = tjGetScalingFactors(&numscalingfactors);
        tjGetScalingFactors(NULL);
    }
    
    // Create handles for decompression
    tjhandle handle_old = tjInitDecompress();
    tjhandle handle_new = tj3Init(TJINIT_DECOMPRESS);
    
    if (handle_old && handle_new) {
        // Test tjDecompressHeader
        {
            int width = 0, height = 0;
            tjDecompressHeader(handle_old, jpegBuf, jpegSize, &width, &height);
            // Test with NULL pointers
            if (Size > 0) {
                tjDecompressHeader(handle_old, jpegBuf, jpegSize, NULL, NULL);
            }
        }
        
        // Test tj3SetScalingFactor with various scaling factors
        {
            int numFactors = 0;
            tjscalingfactor *factors = tj3GetScalingFactors(&numFactors);
            if (factors && numFactors > 0) {
                // Try different scaling factors based on input size
                for (int i = 0; i < numFactors && i < 3; i++) {
                    tj3SetScalingFactor(handle_new, factors[i]);
                }
                // Try unscaled
                tjscalingfactor unscaled = {1, 1};
                tj3SetScalingFactor(handle_new, unscaled);
            }
        }
        
        // Test tj3SetCroppingRegion
        {
            // Read header first if we have enough data
            int width = 0, height = 0;
            if (tjDecompressHeader(handle_old, jpegBuf, jpegSize, &width, &height) == 0) {
                // Try different cropping regions
                tjregion regions[] = {
                    {0, 0, (unsigned int)width, (unsigned int)height},
                    {0, 0, (unsigned int)(width/2), (unsigned int)(height/2)},
                    {(unsigned int)(width/4), (unsigned int)(height/4), 
                     (unsigned int)(width/2), (unsigned int)(height/2)}
                };
                
                for (int i = 0; i < 3 && i < (int)(sizeof(regions)/sizeof(regions[0])); i++) {
                    if (regions[i].w > 0 && regions[i].h > 0) {
                        tj3SetCroppingRegion(handle_new, regions[i]);
                    }
                }
                
                // Try no cropping
                tj3SetCroppingRegion(handle_new, TJUNCROPPED);
            } else {
                // Try cropping without valid header
                tjregion dummy_region = {0, 0, 100, 100};
                tj3SetCroppingRegion(handle_new, dummy_region);
            }
        }
        
        // Test tjDecompress2
        {
            int width = 0, height = 0;
            if (tjDecompressHeader(handle_old, jpegBuf, jpegSize, &width, &height) == 0 && 
                width > 0 && height > 0 && width < 4096 && height < 4096) {
                
                // Allocate buffer for decompressed image
                int pitch = width * tjPixelSize[TJPF_RGB];
                unsigned char *dstBuf = (unsigned char *)malloc(pitch * height);
                
                if (dstBuf) {
                    // Try different pixel formats
                    int pixelFormats[] = {TJPF_RGB, TJPF_BGR, TJPF_RGBX, TJPF_BGRX, TJPF_GRAY};
                    
                    for (int i = 0; i < 5 && i < (int)(sizeof(pixelFormats)/sizeof(pixelFormats[0])); i++) {
                        pitch = width * tjPixelSize[pixelFormats[i]];
                        tjDecompress2(handle_old, jpegBuf, jpegSize, dstBuf, 
                                     width, pitch, height, pixelFormats[i], 0);
                    }
                    
                    free(dstBuf);
                }
            }
        }
    }
    
    // Clean up
    if (handle_old) {
        tjDestroy(handle_old);
    }
    if (handle_new) {
        tj3Destroy(handle_new);
    }
    
    return 0;
}