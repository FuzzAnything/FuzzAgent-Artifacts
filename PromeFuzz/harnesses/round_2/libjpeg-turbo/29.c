// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjSaveImage at turbojpeg.c:3125:15 in turbojpeg.h
// tjDecodeYUVPlanes at turbojpeg.c:2649:15 in turbojpeg.h
// tjLoadImage at turbojpeg.c:3104:26 in turbojpeg.h
// tj3LoadImage16 at turbojpeg-mp.c:293:21 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tj3JPEGBufSize at turbojpeg.c:903:18 in turbojpeg.h
// tj3Compress16 at turbojpeg-mp.c:71:15 in turbojpeg.h
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

#define MAX_DIMENSION 256
#define DUMMY_FILENAME "./dummy_file"

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen(DUMMY_FILENAME, "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static void cleanup_dummy_file(void) {
    remove(DUMMY_FILENAME);
}

int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
    if (Size < 16) return 0;
    
    // Initialize some parameters from fuzzer data
    int width = (Data[0] % MAX_DIMENSION) + 1;
    int height = (Data[1] % MAX_DIMENSION) + 1;
    int pixelFormat = Data[2] % 12; // TJPF_ values 0-11
    int align = 1 << (Data[3] % 4); // 1, 2, 4, or 8
    int flags = Data[4] & 0xFF;
    int param = Data[5] % 32; // TJPARAM values
    int subsamp = Data[6] % 4; // TJSAMP_ values 0-3
    
    // Create TurboJPEG instances
    tjhandle comp_handle = tj3Init(TJINIT_COMPRESS);
    tjhandle decomp_handle = tj3Init(TJINIT_DECOMPRESS);
    
    if (!comp_handle || !decomp_handle) {
        if (comp_handle) tj3Destroy(comp_handle);
        if (decomp_handle) tj3Destroy(decomp_handle);
        return 0;
    }
    
    // Set some parameters
    tj3Set(comp_handle, TJPARAM_QUALITY, 85);
    tj3Set(comp_handle, TJPARAM_SUBSAMP, subsamp);
    
    // Test tj3Get
    int quality = tj3Get(comp_handle, TJPARAM_QUALITY);
    int retrieved_subsamp = tj3Get(comp_handle, TJPARAM_SUBSAMP);
    (void)quality;
    (void)retrieved_subsamp;
    
    // Test tj3Compress16
    {
        int pitch = width * tjPixelSize[pixelFormat];
        size_t src_size = pitch * height;
        unsigned short *src_buf = NULL;
        unsigned char *jpeg_buf = NULL;
        size_t jpeg_size = 0;
        
        if (src_size > 0 && src_size <= Size * 2) {
            src_buf = (unsigned short *)malloc(src_size * sizeof(unsigned short));
            if (src_buf) {
                // Copy fuzzer data to source buffer
                size_t copy_size = (Size > src_size * sizeof(unsigned short)) ? 
                                   src_size * sizeof(unsigned short) : Size;
                memcpy(src_buf, Data, copy_size);
                
                jpeg_size = tj3JPEGBufSize(width, height, subsamp);
                jpeg_buf = (unsigned char *)malloc(jpeg_size);
                
                if (jpeg_buf) {
                    tj3Compress16(comp_handle, src_buf, width, pitch, height, 
                                 pixelFormat, &jpeg_buf, &jpeg_size);
                }
                
                free(src_buf);
                if (jpeg_buf) free(jpeg_buf);
            }
        }
    }
    
    // Test tjSaveImage
    {
        int pitch = width * tjPixelSize[pixelFormat];
        size_t buf_size = pitch * height;
        unsigned char *buffer = NULL;
        
        if (buf_size > 0 && buf_size <= Size) {
            buffer = (unsigned char *)malloc(buf_size);
            if (buffer) {
                memcpy(buffer, Data, (Size < buf_size) ? Size : buf_size);
                write_dummy_file(Data, (Size < 4096) ? Size : 4096);
                tjSaveImage(DUMMY_FILENAME, buffer, width, pitch, height, 
                           pixelFormat, flags);
                cleanup_dummy_file();
                free(buffer);
            }
        }
    }
    
    // Test tjDecodeYUVPlanes
    {
        int pitch = width * tjPixelSize[pixelFormat];
        size_t dst_size = pitch * height;
        unsigned char *dst_buf = NULL;
        const unsigned char *src_planes[3];
        int strides[3];
        
        if (dst_size > 0 && dst_size <= Size) {
            dst_buf = (unsigned char *)malloc(dst_size);
            if (dst_buf) {
                // Initialize YUV planes with fuzzer data
                size_t y_size = width * height;
                size_t uv_size = y_size / 4; // For 4:2:0
                
                if (y_size + uv_size * 2 <= Size) {
                    src_planes[0] = Data;
                    src_planes[1] = Data + y_size;
                    src_planes[2] = Data + y_size + uv_size;
                    
                    strides[0] = width;
                    strides[1] = width / 2;
                    strides[2] = width / 2;
                    
                    tjDecodeYUVPlanes(decomp_handle, src_planes, strides, 
                                     subsamp, dst_buf, width, pitch, height, 
                                     pixelFormat, flags);
                }
                free(dst_buf);
            }
        }
    }
    
    // Test tjLoadImage
    {
        write_dummy_file(Data, (Size < 65536) ? Size : 65536);
        int loaded_width, loaded_height, loaded_pixelFormat;
        unsigned char *loaded_buffer = tjLoadImage(DUMMY_FILENAME, &loaded_width, 
                                                   align, &loaded_height, 
                                                   &loaded_pixelFormat, flags);
        if (loaded_buffer) {
            free(loaded_buffer);
        }
        cleanup_dummy_file();
    }
    
    // Test tj3LoadImage16
    {
        write_dummy_file(Data, (Size < 65536) ? Size : 65536);
        int loaded_width, loaded_height, loaded_pixelFormat = pixelFormat;
        unsigned short *loaded_buffer = tj3LoadImage16(comp_handle, DUMMY_FILENAME, 
                                                       &loaded_width, align, 
                                                       &loaded_height, 
                                                       &loaded_pixelFormat);
        if (loaded_buffer) {
            tj3Free(loaded_buffer);
        }
        cleanup_dummy_file();
    }
    
    // Cleanup
    tj3Destroy(comp_handle);
    tj3Destroy(decomp_handle);
    
    return 0;
}