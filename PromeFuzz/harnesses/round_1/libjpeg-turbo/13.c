// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjDecompress2 at turbojpeg.c:2059:15 in turbojpeg.h
// tjDecompress2 at turbojpeg.c:2059:15 in turbojpeg.h
// tjDecompress2 at turbojpeg.c:2059:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3GetScalingFactors at turbojpeg.c:1959:28 in turbojpeg.h
// tj3GetScalingFactors at turbojpeg.c:1959:28 in turbojpeg.h
// tjGetScalingFactors at turbojpeg.c:1974:28 in turbojpeg.h
// tjGetScalingFactors at turbojpeg.c:1974:28 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjDecompressHeader at turbojpeg.c:1914:15 in turbojpeg.h
// tjDecompressHeader at turbojpeg.c:1914:15 in turbojpeg.h
// tj3GetScalingFactors at turbojpeg.c:1959:28 in turbojpeg.h
// tj3SetScalingFactor at turbojpeg.c:1981:15 in turbojpeg.h
// tj3SetScalingFactor at turbojpeg.c:1981:15 in turbojpeg.h
// tj3SetScalingFactor at turbojpeg.c:1981:15 in turbojpeg.h
// tj3SetCroppingRegion at turbojpeg.c:2006:15 in turbojpeg.h
// tj3SetCroppingRegion at turbojpeg.c:2006:15 in turbojpeg.h
// tj3SetCroppingRegion at turbojpeg.c:2006:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "src/turbojpeg.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void test_tj3GetScalingFactors(void) {
    int numScalingFactors = 0;
    tjscalingfactor *factors = tj3GetScalingFactors(&numScalingFactors);
    (void)factors;
    (void)numScalingFactors;
    
    // Test with NULL pointer (should trigger error)
    tjscalingfactor *null_factors = tj3GetScalingFactors(NULL);
    (void)null_factors;
}

static void test_tjGetScalingFactors(void) {
    int numscalingfactors = 0;
    tjscalingfactor *factors = tjGetScalingFactors(&numscalingfactors);
    (void)factors;
    (void)numscalingfactors;
    
    // Test with NULL pointer
    tjscalingfactor *null_factors = tjGetScalingFactors(NULL);
    (void)null_factors;
}

static void test_decompression_functions(const uint8_t *jpegBuf, unsigned long jpegSize) {
    // Create decompressor handle
    tjhandle handle = tjInitDecompress();
    if (!handle) {
        return;
    }
    
    // Test tjDecompressHeader
    int width = 0, height = 0;
    int header_result = tjDecompressHeader(handle, (unsigned char*)jpegBuf, jpegSize, &width, &height);
    (void)header_result;
    
    // Test with NULL dimensions
    int null_header_result = tjDecompressHeader(handle, (unsigned char*)jpegBuf, jpegSize, NULL, NULL);
    (void)null_header_result;
    
    if (width > 0 && height > 0) {
        // Get scaling factors for testing
        int numFactors = 0;
        tjscalingfactor *factors = tj3GetScalingFactors(&numFactors);
        
        // Test tj3SetScalingFactor with various scaling factors
        if (factors && numFactors > 0) {
            // Test with first scaling factor
            int set_result = tj3SetScalingFactor(handle, factors[0]);
            (void)set_result;
            
            // Test with TJUNSCALED
            tjscalingfactor unscaled = {1, 1};
            int unscaled_result = tj3SetScalingFactor(handle, unscaled);
            (void)unscaled_result;
            
            // Test with invalid scaling factor
            tjscalingfactor invalid = {3, 7}; // Not a multiple of 1/8
            int invalid_result = tj3SetScalingFactor(handle, invalid);
            (void)invalid_result;
        }
        
        // Test tj3SetCroppingRegion if header was read successfully
        if (header_result == 0) {
            // Test with TJUNCROPPED
            int uncropped_result = tj3SetCroppingRegion(handle, TJUNCROPPED);
            (void)uncropped_result;
            
            // Test with valid cropping region (aligned to MCU boundaries)
            tjregion crop_region;
            crop_region.x = 0;
            crop_region.y = 0;
            crop_region.w = (width > 16) ? 16 : width;
            crop_region.h = (height > 16) ? 16 : height;
            
            int crop_result = tj3SetCroppingRegion(handle, crop_region);
            (void)crop_result;
            
            // Test with invalid cropping region
            tjregion invalid_crop;
            invalid_crop.x = 3; // Not aligned to MCU
            invalid_crop.y = 3;
            invalid_crop.w = 10;
            invalid_crop.h = 10;
            
            int invalid_crop_result = tj3SetCroppingRegion(handle, invalid_crop);
            (void)invalid_crop_result;
        }
        
        // Test tjDecompress2 with various parameters
        int pitch = width * 3; // RGB format
        unsigned char *dstBuf = (unsigned char*)malloc(height * pitch);
        if (dstBuf) {
            // Test with RGB pixel format
            int decompress_result = tjDecompress2(handle, (unsigned char*)jpegBuf, jpegSize,
                                                  dstBuf, width, pitch, height, TJPF_RGB, 0);
            (void)decompress_result;
            
            // Test with BGR pixel format
            int decompress_bgr = tjDecompress2(handle, (unsigned char*)jpegBuf, jpegSize,
                                               dstBuf, width, pitch, height, TJPF_BGR, 0);
            (void)decompress_bgr;
            
            // Test with invalid parameters
            int invalid_decompress = tjDecompress2(handle, (unsigned char*)jpegBuf, jpegSize,
                                                   dstBuf, -1, -1, -1, TJPF_RGB, 0);
            (void)invalid_decompress;
            
            free(dstBuf);
        }
    }
    
    // Cleanup
    tjDestroy(handle);
}

int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    // Test scaling factor functions
    test_tj3GetScalingFactors();
    test_tjGetScalingFactors();
    
    // Test decompression functions with fuzzer input as JPEG data
    test_decompression_functions(Data, (unsigned long)Size);
    
    return 0;
}