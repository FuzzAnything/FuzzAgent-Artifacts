// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3JPEGBufSize at turbojpeg.c:903:18 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Compress16 at turbojpeg-mp.c:71:15 in turbojpeg.h
// tjSaveImage at turbojpeg.c:3125:15 in turbojpeg.h
// tjDecodeYUVPlanes at turbojpeg.c:2649:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tj3LoadImage16 at turbojpeg-mp.c:293:21 in turbojpeg.h
// tjLoadImage at turbojpeg.c:3104:26 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "turbojpeg.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void cleanup_handles(tjhandle comp_handle, tjhandle decomp_handle) {
    if (comp_handle) tj3Destroy(comp_handle);
    if (decomp_handle) tj3Destroy(decomp_handle);
}

int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    // Write input to dummy file for file-based functions
    write_dummy_file(Data, Size);
    
    tjhandle comp_handle = NULL;
    tjhandle decomp_handle = NULL;
    
    // Initialize handles
    comp_handle = tj3Init(TJINIT_COMPRESS);
    decomp_handle = tj3Init(TJINIT_DECOMPRESS);
    
    if (!comp_handle || !decomp_handle) {
        cleanup_handles(comp_handle, decomp_handle);
        return 0;
    }
    
    // Use first bytes to determine parameters
    int width = (Data[0] << 8) | Data[1];
    int height = (Data[2] << 8) | Data[3];
    int pixel_format = Data[4] % 12; // TJPF values 0-11
    int align = 1 << (Data[5] % 4); // 1, 2, 4, or 8
    int param = Data[6] % 32; // TJPARAM values
    
    // Ensure reasonable dimensions
    if (width <= 0) width = 16;
    if (height <= 0) height = 16;
    if (width > 4096) width = 4096;
    if (height > 4096) height = 4096;
    
    // Test tj3Get
    if (comp_handle) {
        int value = tj3Get(comp_handle, param);
        (void)value; // Use value to avoid unused variable warning
    }
    
    // Test tj3LoadImage16
    int loaded_width, loaded_height, loaded_pixel_format;
    loaded_width = width;
    loaded_height = height;
    loaded_pixel_format = pixel_format;
    
    unsigned short *image16 = NULL;
    if (Size > 100) { // Need enough data for PPM header
        image16 = tj3LoadImage16(comp_handle, "./dummy_file", 
                                 &loaded_width, align, &loaded_height, 
                                 &loaded_pixel_format);
    }
    
    // Test tjLoadImage
    int tj_width, tj_height, tj_pixel_format;
    tj_width = width;
    tj_height = height;
    tj_pixel_format = pixel_format;
    int flags = Data[7] % 4; // TJFLAG values
    
    unsigned char *image8 = NULL;
    if (Size > 100) {
        image8 = tjLoadImage("./dummy_file", &tj_width, align, 
                            &tj_height, &tj_pixel_format, flags);
    }
    
    // Test tj3Compress16 with synthetic data if we couldn't load from file
    unsigned short *src_buf = NULL;
    size_t src_size = 0;
    unsigned char *jpeg_buf = NULL;
    size_t jpeg_size = 0;
    
    if (image16) {
        src_buf = image16;
        src_size = loaded_width * loaded_height * tjPixelSize[loaded_pixel_format] * sizeof(unsigned short);
    } else {
        // Create synthetic 16-bit image data
        int pitch = width * tjPixelSize[pixel_format];
        src_size = pitch * height * sizeof(unsigned short);
        src_buf = (unsigned short*)malloc(src_size);
        if (src_buf && Size > 8) {
            size_t copy_size = Size - 8;
            if (copy_size > src_size) copy_size = src_size;
            memcpy(src_buf, Data + 8, copy_size);
        }
    }
    
    if (src_buf && comp_handle) {
        // Set precision for 16-bit compression
        tj3Set(comp_handle, TJPARAM_PRECISION, 16);
        
        // Try compression with various parameters
        int pitch = width * tjPixelSize[pixel_format];
        jpeg_size = tj3JPEGBufSize(width, height, 0);
        jpeg_buf = tj3Alloc(jpeg_size);
        
        if (jpeg_buf) {
            int result = tj3Compress16(comp_handle, src_buf, width, pitch, 
                                      height, pixel_format, &jpeg_buf, &jpeg_size);
            (void)result; // Use result to avoid unused variable warning
        }
    }
    
    // Test tjSaveImage if we have image data
    if (image8 && Size > 50) {
        int save_width = tj_width;
        int save_height = tj_height;
        int save_pitch = save_width * tjPixelSize[tj_pixel_format];
        
        tjSaveImage("./dummy_file", image8, save_width, save_pitch, 
                   save_height, tj_pixel_format, flags);
    }
    
    // Test tjDecodeYUVPlanes with synthetic YUV data
    if (decomp_handle && Size > 100) {
        const unsigned char *src_planes[3];
        int strides[3] = {width, width/2, width/2};
        int subsamp = Data[8] % 4; // TJSAMP values
        
        // Allocate synthetic YUV planes
        size_t y_size = width * height;
        size_t uv_size = (width/2) * (height/2);
        unsigned char *y_plane = (unsigned char*)malloc(y_size);
        unsigned char *u_plane = (unsigned char*)malloc(uv_size);
        unsigned char *v_plane = (unsigned char*)malloc(uv_size);
        
        if (y_plane && u_plane && v_plane) {
            size_t offset = 9;
            if (offset < Size && offset + y_size + uv_size * 2 <= Size) {
                memcpy(y_plane, Data + offset, y_size);
                memcpy(u_plane, Data + offset + y_size, uv_size);
                memcpy(v_plane, Data + offset + y_size + uv_size, uv_size);
                
                src_planes[0] = y_plane;
                src_planes[1] = u_plane;
                src_planes[2] = v_plane;
                
                int dst_pitch = width * tjPixelSize[pixel_format];
                size_t dst_size = dst_pitch * height;
                unsigned char *dst_buf = (unsigned char*)malloc(dst_size);
                
                if (dst_buf) {
                    tjDecodeYUVPlanes(decomp_handle, src_planes, strides, 
                                     subsamp, dst_buf, width, dst_pitch, 
                                     height, pixel_format, flags);
                    free(dst_buf);
                }
            }
        }
        
        free(y_plane);
        free(u_plane);
        free(v_plane);
    }
    
    // Cleanup
    if (image16) tj3Free(image16);
    if (image8) tjFree(image8);
    if (jpeg_buf) tj3Free(jpeg_buf);
    if (!image16 && src_buf) free(src_buf);
    
    cleanup_handles(comp_handle, decomp_handle);
    
    return 0;
}