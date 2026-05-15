// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjDecompressToYUVPlanes at turbojpeg.c:2288:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tj3SetScalingFactor at turbojpeg.c:1981:15 in turbojpeg.h
// tj3DecompressHeader at turbojpeg.c:1815:15 in turbojpeg.h
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tj3SetCroppingRegion at turbojpeg.c:2006:15 in turbojpeg.h
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
// tj3DecompressToYUV8 at turbojpeg.c:2338:15 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "turbojpeg.h"

static tjhandle init_decompressor() {
    return tj3Init(TJINIT_DECOMPRESS);
}

static tjhandle init_compressor() {
    return tj3Init(TJINIT_COMPRESS);
}

int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    tjhandle decomp_handle = init_decompressor();
    tjhandle comp_handle = init_compressor();
    
    if (!decomp_handle || !comp_handle) {
        if (decomp_handle) tj3Destroy(decomp_handle);
        if (comp_handle) tj3Destroy(comp_handle);
        return 0;
    }
    
    // Write data to dummy file for potential file-based operations
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }
    
    // Use first byte to determine parameter values
    uint8_t param_selector = Data[0];
    
    // Test tj3Set with various parameters
    int params_to_test[] = {
        TJPARAM_QUALITY, TJPARAM_SUBSAMP, TJPARAM_FASTDCT,
        TJPARAM_OPTIMIZE, TJPARAM_PROGRESSIVE, TJPARAM_SCANLIMIT,
        TJPARAM_ARITHMETIC, TJPARAM_LOSSLESS, TJPARAM_LOSSLESSPSV,
        TJPARAM_FASTUPSAMPLE, TJPARAM_STOPONWARNING
    };
    
    int num_params = sizeof(params_to_test) / sizeof(params_to_test[0]);
    int param_idx = param_selector % num_params;
    int value = (param_selector * 37) % 256; // Simple hash for value
    
    // Test tj3Set on both handles
    tj3Set(decomp_handle, params_to_test[param_idx], value);
    tj3Set(comp_handle, params_to_test[param_idx], value);
    
    // Test tj3Get on both handles
    for (int i = 0; i < 3; i++) {
        int get_idx = (param_selector + i) % num_params;
        tj3Get(decomp_handle, params_to_test[get_idx]);
        tj3Get(comp_handle, params_to_test[get_idx]);
    }
    
    // Test scaling factor setting
    tjscalingfactor scaling_factor;
    scaling_factor.num = 1 + (param_selector % 8);
    scaling_factor.denom = 1 + ((param_selector * 3) % 8);
    tj3SetScalingFactor(decomp_handle, scaling_factor);
    
    // Try to decompress header if we have enough data
    int width, height, jpegSubsamp, jpegColorspace;
    if (Size > 100) { // Need minimum data for header
        tj3DecompressHeader(decomp_handle, Data, Size);
        
        // Get dimensions after header decompression
        width = tj3Get(decomp_handle, TJPARAM_JPEGWIDTH);
        height = tj3Get(decomp_handle, TJPARAM_JPEGHEIGHT);
        jpegSubsamp = tj3Get(decomp_handle, TJPARAM_SUBSAMP);
        
        if (width > 0 && height > 0) {
            // Test cropping region
            tjregion crop_region;
            crop_region.x = (param_selector * 7) % (width > 0 ? width : 100);
            crop_region.y = (param_selector * 11) % (height > 0 ? height : 100);
            crop_region.w = 10 + (param_selector * 13) % 100;
            crop_region.h = 10 + (param_selector * 17) % 100;
            
            // Set cropping region (may fail if not aligned properly)
            tj3SetCroppingRegion(decomp_handle, crop_region);
            
            // Test tj3DecompressToYUV8
            int align = 1 << (param_selector % 4); // 1, 2, 4, or 8
            unsigned long yuv_buf_size = tj3YUVBufSize(width, align, height, jpegSubsamp);
            
            if (yuv_buf_size > 0 && yuv_buf_size < 10 * 1024 * 1024) { // Limit buffer size
                unsigned char *yuv_buffer = (unsigned char*)malloc(yuv_buf_size);
                if (yuv_buffer) {
                    tj3DecompressToYUV8(decomp_handle, Data, Size, yuv_buffer, align);
                    free(yuv_buffer);
                }
            }
            
            // Test tjDecompressToYUVPlanes (legacy API)
            int strides[3] = {0, 0, 0};
            unsigned char *planes[3] = {NULL, NULL, NULL};
            
            // Calculate plane sizes
            int y_width = width;
            int y_height = height;
            int uv_width = (jpegSubsamp == TJSAMP_444) ? width : 
                          (jpegSubsamp == TJSAMP_422) ? width/2 : 
                          (jpegSubsamp == TJSAMP_420) ? width/2 : width/4;
            int uv_height = (jpegSubsamp == TJSAMP_444) ? height : 
                           (jpegSubsamp == TJSAMP_422) ? height : 
                           (jpegSubsamp == TJSAMP_420) ? height/2 : height/4;
            
            // Allocate planes if dimensions are reasonable
            if (y_width > 0 && y_height > 0 && uv_width > 0 && uv_height > 0 &&
                y_width < 4096 && y_height < 4096) {
                
                planes[0] = (unsigned char*)malloc(y_width * y_height);
                planes[1] = (unsigned char*)malloc(uv_width * uv_height);
                planes[2] = (unsigned char*)malloc(uv_width * uv_height);
                
                if (planes[0] && planes[1] && planes[2]) {
                    tjDecompressToYUVPlanes(decomp_handle, Data, Size,
                                           planes, width, strides, height, 0);
                }
                
                if (planes[0]) free(planes[0]);
                if (planes[1]) free(planes[1]);
                if (planes[2]) free(planes[2]);
            }
        }
    }
    
    // Cleanup
    tj3Destroy(decomp_handle);
    tj3Destroy(comp_handle);
    
    // Clean up dummy file
    remove("./dummy_file");
    
    return 0;
}