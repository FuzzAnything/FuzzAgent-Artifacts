// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjDecompress2 at turbojpeg.c:2059:15 in turbojpeg.h
// tj3SetScalingFactor at turbojpeg.c:1981:15 in turbojpeg.h
// tj3SetScalingFactor at turbojpeg.c:1981:15 in turbojpeg.h
// tj3SetScalingFactor at turbojpeg.c:1981:15 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tj3SetCroppingRegion at turbojpeg.c:2006:15 in turbojpeg.h
// tj3SetCroppingRegion at turbojpeg.c:2006:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjGetScalingFactors at turbojpeg.c:1974:28 in turbojpeg.h
// tj3GetScalingFactors at turbojpeg.c:1959:28 in turbojpeg.h
// tjDecompressHeader2 at turbojpeg.c:1903:15 in turbojpeg.h
// tj3SetScalingFactor at turbojpeg.c:1981:15 in turbojpeg.h
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
#include <stdio.h>

static tjhandle create_decompress_handle(void) {
    return tjInitDecompress();
}

static void cleanup_handle(tjhandle handle) {
    if (handle) {
        tjDestroy(handle);
    }
}

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return;
    fwrite(data, 1, size, fp);
    fclose(fp);
}

int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    // Write input data to dummy file for potential file-based operations
    write_dummy_file(Data, Size);
    
    // Test tjGetScalingFactors (deprecated but still available)
    int num_factors_old = 0;
    tjscalingfactor *factors_old = tjGetScalingFactors(&num_factors_old);
    if (factors_old && num_factors_old > 0) {
        // Access some factor data to ensure it's read
        for (int i = 0; i < (num_factors_old < 2 ? num_factors_old : 2); i++) {
            int dummy = factors_old[i].num * factors_old[i].denom;
            (void)dummy;
        }
    }
    
    // Test tj3GetScalingFactors (new API)
    int num_factors_new = 0;
    tjscalingfactor *factors_new = tj3GetScalingFactors(&num_factors_new);
    if (factors_new && num_factors_new > 0) {
        // Access some factor data
        for (int i = 0; i < (num_factors_new < 2 ? num_factors_new : 2); i++) {
            int dummy = factors_new[i].num * factors_new[i].denom;
            (void)dummy;
        }
    }
    
    // Create decompression handle
    tjhandle handle = create_decompress_handle();
    if (!handle) {
        return 0;
    }
    
    // Test decompression header reading
    int width = 0, height = 0, jpegSubsamp = 0;
    unsigned long jpegSize = Size > 0 ? (unsigned long)Size : 0;
    
    // Use Data as JPEG buffer if size > 0
    if (Size > 0) {
        int header_result = tjDecompressHeader2(handle, (unsigned char *)Data, 
                                                jpegSize, &width, &height, &jpegSubsamp);
        (void)header_result; // Result may be error for invalid data
        
        // If header read succeeded, test scaling and cropping
        if (width > 0 && height > 0) {
            // Test setting scaling factor if we have factors
            if (factors_new && num_factors_new > 0) {
                // Use first scaling factor
                tj3SetScalingFactor(handle, factors_new[0]);
                
                // Test setting cropping region (only if scaling was set)
                tjregion crop_region;
                // Create a simple cropping region (must align with MCU boundaries)
                // Use small region to avoid issues
                crop_region.x = 0;
                crop_region.y = 0;
                crop_region.w = width > 16 ? 16 : width;
                crop_region.h = height > 16 ? 16 : height;
                
                tj3SetCroppingRegion(handle, crop_region);
            }
            
            // Test decompression with dummy buffer
            // Only attempt if we have reasonable dimensions
            if (width <= 4096 && height <= 4096 && width > 0 && height > 0) {
                int pitch = width * 3; // RGB format
                size_t buf_size = (size_t)(pitch * height);
                if (buf_size > 0 && buf_size < 100 * 1024 * 1024) { // Limit to 100MB
                    unsigned char *dstBuf = (unsigned char *)malloc(buf_size);
                    if (dstBuf) {
                        // Try decompression with various pixel formats
                        int pixel_formats[] = {TJPF_RGB, TJPF_BGR, TJPF_RGBX, TJPF_BGRX};
                        for (int i = 0; i < 4; i++) {
                            int decomp_result = tjDecompress2(handle, Data, jpegSize,
                                                             dstBuf, width, pitch, height,
                                                             pixel_formats[i], 0);
                            (void)decomp_result; // Result may be error
                        }
                        free(dstBuf);
                    }
                }
            }
        }
    }
    
    // Test setting scaling factor with various inputs
    if (factors_new && num_factors_new > 0) {
        // Try first factor
        tj3SetScalingFactor(handle, factors_new[0]);
        
        // Try last factor if different
        if (num_factors_new > 1) {
            tj3SetScalingFactor(handle, factors_new[num_factors_new - 1]);
        }
        
        // Try unscaled
        tjscalingfactor unscaled = {1, 1};
        tj3SetScalingFactor(handle, unscaled);
    }
    
    // Test setting cropping region with various inputs
    if (width > 0 && height > 0) {
        // Try different cropping regions based on input data
        tjregion regions[3];
        
        // Region 1: Small region at origin
        regions[0].x = 0;
        regions[0].y = 0;
        regions[0].w = width > 8 ? 8 : width;
        regions[0].h = height > 8 ? 8 : height;
        
        // Region 2: Full region (uncropped)
        regions[1].x = 0;
        regions[1].y = 0;
        regions[1].w = width;
        regions[1].h = height;
        
        // Region 3: Partial region (ensure within bounds)
        regions[2].x = width > 16 ? 8 : 0;
        regions[2].y = height > 16 ? 8 : 0;
        regions[2].w = width > 32 ? 16 : (width > 8 ? width - 8 : width);
        regions[2].h = height > 32 ? 16 : (height > 8 ? height - 8 : height);
        
        for (int i = 0; i < 3; i++) {
            if (regions[i].w > 0 && regions[i].h > 0 &&
                regions[i].x + regions[i].w <= width &&
                regions[i].y + regions[i].h <= height) {
                tj3SetCroppingRegion(handle, regions[i]);
            }
        }
        
        // Try uncropped
        tjregion uncropped = {0, 0, width, height};
        tj3SetCroppingRegion(handle, uncropped);
    }
    
    // Clean up
    cleanup_handle(handle);
    
    return 0;
}