// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjGetScalingFactors at turbojpeg.c:1974:28 in turbojpeg.h
// tj3GetScalingFactors at turbojpeg.c:1959:28 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjDecompressHeader2 at turbojpeg.c:1903:15 in turbojpeg.h
// tj3SetScalingFactor at turbojpeg.c:1981:15 in turbojpeg.h
// tj3SetCroppingRegion at turbojpeg.c:2006:15 in turbojpeg.h
// tj3SetCroppingRegion at turbojpeg.c:2006:15 in turbojpeg.h
// tjDecompress2 at turbojpeg.c:2059:15 in turbojpeg.h
// tjDecompress2 at turbojpeg.c:2059:15 in turbojpeg.h
// tjDecompress2 at turbojpeg.c:2059:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "turbojpeg.h"

int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
    // Write fuzzer input to dummy file for functions that might need it
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Test tjGetScalingFactors and tj3GetScalingFactors
    int num_scaling_factors1 = 0;
    tjscalingfactor *factors1 = tjGetScalingFactors(&num_scaling_factors1);
    
    int num_scaling_factors2 = 0;
    tjscalingfactor *factors2 = tj3GetScalingFactors(&num_scaling_factors2);
    
    // Create decompressor handle
    tjhandle handle = tjInitDecompress();
    if (!handle) return 0;
    
    // Try to read JPEG header if we have enough data
    if (Size > 0) {
        int width = 0, height = 0, jpegSubsamp = 0;
        unsigned char *jpegBuf = (unsigned char *)Data;
        unsigned long jpegSize = (unsigned long)Size;
        
        // Test tjDecompressHeader2
        int header_result = tjDecompressHeader2(handle, jpegBuf, jpegSize, 
                                                &width, &height, &jpegSubsamp);
        
        // If header was read successfully, test other functions
        if (header_result == 0 && width > 0 && height > 0) {
            // Test scaling factor setting with valid factors
            if (factors2 && num_scaling_factors2 > 0) {
                // Try different scaling factors
                for (int i = 0; i < num_scaling_factors2 && i < 3; i++) {
                    tj3SetScalingFactor(handle, factors2[i]);
                }
            }
            
            // Test cropping region if we have valid dimensions
            if (width > 16 && height > 16) {
                tjregion crop_region;
                crop_region.x = 0;
                crop_region.y = 0;
                crop_region.w = width / 2;
                crop_region.h = height / 2;
                
                // Align to typical MCU boundaries (8x8 for most subsampling)
                crop_region.w &= ~7;
                crop_region.h &= ~7;
                
                if (crop_region.w > 0 && crop_region.h > 0) {
                    tj3SetCroppingRegion(handle, crop_region);
                }
                
                // Also test with uncropped
                tj3SetCroppingRegion(handle, TJUNCROPPED);
            }
            
            // Test decompression with various parameters
            int pixel_format = TJPF_RGB;
            int pitch = width * tjPixelSize[pixel_format];
            unsigned char *dstBuf = NULL;
            
            // Allocate destination buffer if dimensions are reasonable
            if (width > 0 && height > 0 && width < 10000 && height < 10000) {
                size_t buf_size = (size_t)pitch * (size_t)height;
                if (buf_size > 0 && buf_size < 100 * 1024 * 1024) { // Limit to 100MB
                    dstBuf = (unsigned char *)malloc(buf_size);
                    if (dstBuf) {
                        // Try decompression with different flags
                        tjDecompress2(handle, jpegBuf, jpegSize, dstBuf, 
                                     width, pitch, height, pixel_format, 0);
                        
                        tjDecompress2(handle, jpegBuf, jpegSize, dstBuf,
                                     width, pitch, height, pixel_format, TJFLAG_FASTDCT);
                        
                        tjDecompress2(handle, jpegBuf, jpegSize, dstBuf,
                                     width, pitch, height, pixel_format, TJFLAG_ACCURATEDCT);
                        
                        free(dstBuf);
                    }
                }
            }
        }
    }
    
    // Clean up
    tjDestroy(handle);
    
    return 0;
}