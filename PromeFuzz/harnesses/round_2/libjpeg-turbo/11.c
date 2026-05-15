// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3SetCroppingRegion at turbojpeg.c:2006:15 in turbojpeg.h
// tjDecompress2 at turbojpeg.c:2059:15 in turbojpeg.h
// tjDecompress2 at turbojpeg.c:2059:15 in turbojpeg.h
// tjDecompress2 at turbojpeg.c:2059:15 in turbojpeg.h
// tjDecompress2 at turbojpeg.c:2059:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3GetScalingFactors at turbojpeg.c:1959:28 in turbojpeg.h
// tjGetScalingFactors at turbojpeg.c:1974:28 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjDecompressHeader2 at turbojpeg.c:1903:15 in turbojpeg.h
// tj3SetScalingFactor at turbojpeg.c:1981:15 in turbojpeg.h
// tj3SetScalingFactor at turbojpeg.c:1981:15 in turbojpeg.h
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

static void remove_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    // Write input data to dummy file for functions that require file input
    write_dummy_file(Data, Size);
    
    // Test tj3GetScalingFactors and tjGetScalingFactors
    int num_scaling_factors1 = 0;
    tjscalingfactor *scaling_factors1 = tj3GetScalingFactors(&num_scaling_factors1);
    
    int num_scaling_factors2 = 0;
    tjscalingfactor *scaling_factors2 = tjGetScalingFactors(&num_scaling_factors2);
    
    // Create decompression handle
    tjhandle handle = tjInitDecompress();
    if (!handle) {
        remove_dummy_file();
        return 0;
    }
    
    // Read dummy file for JPEG data
    FILE *f = fopen("./dummy_file", "rb");
    if (f) {
        fseek(f, 0, SEEK_END);
        long file_size = ftell(f);
        fseek(f, 0, SEEK_SET);
        
        if (file_size > 0 && file_size <= (long)Size) {
            unsigned char *jpeg_buf = (unsigned char *)malloc(file_size);
            if (jpeg_buf) {
                fread(jpeg_buf, 1, file_size, f);
                
                // Test tjDecompressHeader2
                int width = 0, height = 0, jpeg_subsamp = 0;
                int header_result = tjDecompressHeader2(handle, jpeg_buf, file_size, 
                                                       &width, &height, &jpeg_subsamp);
                
                // Test tj3SetScalingFactor if we have valid scaling factors
                if (scaling_factors1 && num_scaling_factors1 > 0) {
                    // Try different scaling factors based on input data
                    int idx = Data[0] % num_scaling_factors1;
                    tj3SetScalingFactor(handle, scaling_factors1[idx]);
                    
                    // Also test with TJUNSCALED
                    tjscalingfactor unscaled = {1, 1};
                    tj3SetScalingFactor(handle, unscaled);
                }
                
                // Test tj3SetCroppingRegion if header was read successfully
                if (header_result == 0 && width > 0 && height > 0) {
                    // Create various cropping regions based on input data
                    tjregion regions[3];
                    
                    // Region 1: Full image (uncropped)
                    regions[0] = TJUNCROPPED;
                    
                    // Region 2: Partial region (aligned to typical MCU boundaries)
                    int mcu_width = 8; // Conservative default
                    if (jpeg_subsamp >= 0 && jpeg_subsamp <= 3) {
                        // Common MCU widths for different subsampling
                        const int mcu_widths[] = {8, 16, 16, 8};
                        mcu_width = mcu_widths[jpeg_subsamp];
                    }
                    
                    int crop_x = (Data[1] % (width/mcu_width)) * mcu_width;
                    int crop_y = (Data[2] % (height/8)) * 8;
                    int crop_w = (Data[3] % ((width - crop_x)/mcu_width)) * mcu_width;
                    int crop_h = (Data[4] % ((height - crop_y)/8)) * 8;
                    
                    if (crop_w > 0 && crop_h > 0 && crop_x + crop_w <= width && crop_y + crop_h <= height) {
                        regions[1].x = crop_x;
                        regions[1].y = crop_y;
                        regions[1].w = crop_w;
                        regions[1].h = crop_h;
                    } else {
                        regions[1] = TJUNCROPPED;
                    }
                    
                    // Region 3: Another variation
                    regions[2].x = 0;
                    regions[2].y = 0;
                    regions[2].w = width > 16 ? 16 : width;
                    regions[2].h = height > 16 ? 16 : height;
                    
                    // Try setting different cropping regions
                    for (int i = 0; i < 3; i++) {
                        tj3SetCroppingRegion(handle, regions[i]);
                    }
                }
                
                // Test tjDecompress2 if we have valid dimensions
                if (header_result == 0 && width > 0 && height > 0) {
                    // Calculate pitch
                    int pixel_format = TJPF_RGB; // Try RGB format
                    int pitch = width * tjPixelSize[pixel_format];
                    
                    // Allocate destination buffer
                    unsigned char *dst_buf = (unsigned char *)malloc(pitch * height);
                    if (dst_buf) {
                        // Try decompression with different flags
                        int flags = 0;
                        
                        // Test with FASTDCT flag
                        flags |= TJFLAG_FASTDCT;
                        tjDecompress2(handle, jpeg_buf, file_size, dst_buf, 
                                     width, pitch, height, pixel_format, flags);
                        
                        // Test with ACCURATEDCT flag
                        flags = TJFLAG_ACCURATEDCT;
                        tjDecompress2(handle, jpeg_buf, file_size, dst_buf,
                                     width, pitch, height, pixel_format, flags);
                        
                        // Test with BOTTOMUP flag
                        flags = TJFLAG_BOTTOMUP;
                        tjDecompress2(handle, jpeg_buf, file_size, dst_buf,
                                     width, pitch, height, pixel_format, flags);
                        
                        free(dst_buf);
                    }
                    
                    // Try with different pixel format
                    pixel_format = TJPF_BGR;
                    pitch = width * tjPixelSize[pixel_format];
                    unsigned char *dst_buf2 = (unsigned char *)malloc(pitch * height);
                    if (dst_buf2) {
                        tjDecompress2(handle, jpeg_buf, file_size, dst_buf2,
                                     width, pitch, height, pixel_format, 0);
                        free(dst_buf2);
                    }
                }
                
                free(jpeg_buf);
            }
        }
        fclose(f);
    }
    
    // Clean up
    tjDestroy(handle);
    remove_dummy_file();
    
    return 0;
}