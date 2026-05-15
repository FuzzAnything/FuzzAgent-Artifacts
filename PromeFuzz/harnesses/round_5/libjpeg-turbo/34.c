// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjDecodeYUVPlanes at turbojpeg.c:2649:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Compress16 at turbojpeg-mp.c:71:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tjLoadImage at turbojpeg.c:3104:26 in turbojpeg.h
// tjSaveImage at turbojpeg.c:3125:15 in turbojpeg.h
// tj3LoadImage16 at turbojpeg-mp.c:293:21 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "src/turbojpeg.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void cleanup_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_34(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Initialize handles
    tjhandle comp_handle = tj3Init(TJINIT_COMPRESS);
    tjhandle decomp_handle = tj3Init(TJINIT_DECOMPRESS);
    
    if (!comp_handle || !decomp_handle) {
        if (comp_handle) tj3Destroy(comp_handle);
        if (decomp_handle) tj3Destroy(decomp_handle);
        return 0;
    }
    
    // Use first byte to choose different code paths
    uint8_t selector = Data[0];
    size_t offset = 1;
    
    // Test tj3Get
    if (selector & 0x01) {
        int params[] = {TJPARAM_SUBSAMP, TJPARAM_QUALITY, TJPARAM_ARITHMETIC, 
                       TJPARAM_OPTIMIZE, TJPARAM_PROGRESSIVE, TJPARAM_COLORSPACE};
        int param_idx = (selector >> 1) % (sizeof(params)/sizeof(params[0]));
        tj3Get(comp_handle, params[param_idx]);
    }
    
    // Test tjLoadImage and tjSaveImage
    if (selector & 0x02) {
        write_dummy_file(Data + offset, Size - offset);
        
        int width = 0, height = 0, pixelFormat = TJPF_UNKNOWN;
        int align = 1 << ((selector >> 2) % 4); // 1, 2, 4, 8
        int flags = selector & 0x0F;
        
        unsigned char *img_buffer = tjLoadImage("./dummy_file", &width, align, 
                                               &height, &pixelFormat, flags);
        
        if (img_buffer && width > 0 && height > 0) {
            // Test tjSaveImage with loaded buffer
            int pitch = width * tjPixelSize[pixelFormat];
            tjSaveImage("./dummy_file2", img_buffer, width, pitch, height, 
                       pixelFormat, flags);
            remove("./dummy_file2");
            
            free(img_buffer);
        }
        
        cleanup_dummy_file();
    }
    
    // Test tj3LoadImage16
    if (selector & 0x04 && Size > offset) {
        write_dummy_file(Data + offset, Size - offset);
        
        int width = 0, height = 0, pixelFormat = TJPF_UNKNOWN;
        int align = 1 << ((selector >> 3) % 4); // 1, 2, 4, 8
        
        unsigned short *img16_buffer = tj3LoadImage16(comp_handle, "./dummy_file", 
                                                     &width, align, &height, 
                                                     &pixelFormat);
        
        if (img16_buffer) {
            tj3Free(img16_buffer);
        }
        
        cleanup_dummy_file();
    }
    
    // Test tjDecodeYUVPlanes
    if (selector & 0x08 && Size > 100) {
        // Parse some parameters from fuzzer input
        int width = (Data[offset] << 8) | Data[offset + 1];
        int height = (Data[offset + 2] << 8) | Data[offset + 3];
        width = width % 1024 + 1;
        height = height % 1024 + 1;
        
        int subsamp = Data[offset + 4] % 5; // 0-4 for different subsampling
        int pixelFormat = Data[offset + 5] % 12; // 0-11 for pixel formats
        int flags = Data[offset + 6];
        
        offset += 7;
        
        // Calculate plane sizes based on subsampling
        int y_plane_size = width * height;
        int uv_width = width, uv_height = height;
        
        switch(subsamp) {
            case TJSAMP_420: uv_width = (width + 1) / 2; uv_height = (height + 1) / 2; break;
            case TJSAMP_422: uv_width = (width + 1) / 2; uv_height = height; break;
            case TJSAMP_440: uv_width = width; uv_height = (height + 1) / 2; break;
            case TJSAMP_444: uv_width = width; uv_height = height; break;
            default: uv_width = 0; uv_height = 0; break;
        }
        
        int uv_plane_size = uv_width * uv_height;
        size_t total_planes_size = y_plane_size + 2 * uv_plane_size;
        
        if (Size - offset >= total_planes_size) {
            // Allocate and setup planes
            const unsigned char *planes[3];
            int strides[3] = {0, 0, 0};
            
            planes[0] = Data + offset; // Y plane
            planes[1] = Data + offset + y_plane_size; // U plane
            planes[2] = Data + offset + y_plane_size + uv_plane_size; // V plane
            
            // Set random strides
            strides[0] = width + (Data[offset] % 16);
            strides[1] = uv_width + (Data[offset + 1] % 16);
            strides[2] = uv_width + (Data[offset + 2] % 16);
            
            // Allocate destination buffer
            int pitch = width * tjPixelSize[pixelFormat];
            unsigned char *dst_buf = (unsigned char*)malloc(height * pitch);
            
            if (dst_buf) {
                tjDecodeYUVPlanes(decomp_handle, planes, strides, subsamp,
                                 dst_buf, width, pitch, height, pixelFormat, flags);
                free(dst_buf);
            }
        }
    }
    
    // Test tj3Compress16
    if (selector & 0x10 && Size > 50) {
        // Parse parameters
        int width = (Data[offset] << 8) | Data[offset + 1];
        int height = (Data[offset + 2] << 8) | Data[offset + 3];
        width = width % 256 + 1;
        height = height % 256 + 1;
        
        int pixelFormat = Data[offset + 4] % 12;
        int pitch = width * tjPixelSize[pixelFormat];
        
        offset += 5;
        
        // Check if we have enough data for the image
        size_t needed_size = pitch * height * sizeof(unsigned short);
        if (Size - offset >= needed_size / 4) { // Use smaller size for speed
            // Allocate source buffer
            unsigned short *src_buf = (unsigned short*)malloc(needed_size);
            if (src_buf) {
                // Fill with fuzzer data (scaled to 16-bit)
                size_t copy_size = (Size - offset) * sizeof(uint8_t);
                if (copy_size > needed_size) copy_size = needed_size;
                memcpy(src_buf, Data + offset, copy_size);
                
                // Setup JPEG buffer
                unsigned char *jpeg_buf = NULL;
                size_t jpeg_size = 0;
                
                // Set some parameters
                tj3Set(comp_handle, TJPARAM_SUBSAMP, TJSAMP_444);
                tj3Set(comp_handle, TJPARAM_LOSSLESS, 1);
                
                tj3Compress16(comp_handle, src_buf, width, pitch, height,
                            pixelFormat, &jpeg_buf, &jpeg_size);
                
                if (jpeg_buf) {
                    tj3Free(jpeg_buf);
                }
                
                free(src_buf);
            }
        }
    }
    
    // Cleanup
    tj3Destroy(comp_handle);
    tj3Destroy(decomp_handle);
    
    return 0;
}