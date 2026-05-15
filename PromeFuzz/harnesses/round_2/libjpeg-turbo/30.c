// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjDecodeYUVPlanes at turbojpeg.c:2649:15 in turbojpeg.h
// tjLoadImage at turbojpeg.c:3104:26 in turbojpeg.h
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tj3LoadImage16 at turbojpeg-mp.c:293:21 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3JPEGBufSize at turbojpeg.c:903:18 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Compress16 at turbojpeg-mp.c:71:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tjSaveImage at turbojpeg.c:3125:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "turbojpeg.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define DUMMY_FILE "./dummy_file"
#define WIDTH 64
#define HEIGHT 64
#define PIXEL_FORMAT TJPF_RGB

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen(DUMMY_FILE, "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void cleanup_dummy_file(void) {
    remove(DUMMY_FILE);
}

int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size) {
    if (Size < 16) return 0;
    
    write_dummy_file(Data, Size);
    
    tjhandle comp_handle = tj3Init(TJINIT_COMPRESS);
    tjhandle decomp_handle = tj3Init(TJINIT_DECOMPRESS);
    
    if (!comp_handle || !decomp_handle) {
        if (comp_handle) tj3Destroy(comp_handle);
        if (decomp_handle) tj3Destroy(decomp_handle);
        cleanup_dummy_file();
        return 0;
    }
    
    /* Test tj3Compress16 */
    {
        unsigned short *src_buf = NULL;
        unsigned char *jpeg_buf = NULL;
        size_t jpeg_size = 0;
        
        /* Use a small fixed size for source buffer */
        size_t src_size = WIDTH * HEIGHT * 3 * sizeof(unsigned short);
        src_buf = (unsigned short *)malloc(src_size);
        if (src_buf) {
            /* Fill with fuzzer data, repeating if needed */
            for (size_t i = 0; i < src_size / sizeof(unsigned short); i++) {
                src_buf[i] = (i < Size) ? Data[i] : 0x8000;
            }
            
            jpeg_size = tj3JPEGBufSize(WIDTH, HEIGHT, TJSAMP_444);
            jpeg_buf = tj3Alloc(jpeg_size);
            
            if (jpeg_buf) {
                tj3Compress16(comp_handle, src_buf, WIDTH, 0, HEIGHT, 
                             PIXEL_FORMAT, &jpeg_buf, &jpeg_size);
            }
            
            free(src_buf);
            if (jpeg_buf) tj3Free(jpeg_buf);
        }
    }
    
    /* Test tjSaveImage */
    {
        unsigned char *buffer = NULL;
        size_t buf_size = WIDTH * HEIGHT * 3;
        buffer = (unsigned char *)malloc(buf_size);
        
        if (buffer) {
            /* Fill buffer with fuzzer data */
            for (size_t i = 0; i < buf_size; i++) {
                buffer[i] = (i < Size) ? Data[i] : 0;
            }
            
            tjSaveImage(DUMMY_FILE, buffer, WIDTH, 0, HEIGHT, 
                       PIXEL_FORMAT, TJFLAG_BOTTOMUP);
            
            free(buffer);
        }
    }
    
    /* Test tjDecodeYUVPlanes - FIXED */
    {
        const unsigned char *src_planes[3] = {NULL, NULL, NULL};
        int strides[3] = {0, 0, 0};
        unsigned char *dst_buf = NULL;
        size_t dst_size = WIDTH * HEIGHT * 3;
        
        dst_buf = (unsigned char *)malloc(dst_size);
        if (dst_buf) {
            /* Use fuzzer data for YUV planes with proper size calculation */
            size_t y_size = WIDTH * HEIGHT;
            size_t uv_size = (WIDTH/2) * (HEIGHT/2);
            
            /* Allocate proper buffers for YUV planes */
            unsigned char *y_plane = (unsigned char *)malloc(y_size);
            unsigned char *u_plane = (unsigned char *)malloc(uv_size);
            unsigned char *v_plane = (unsigned char *)malloc(uv_size);
            
            if (y_plane && u_plane && v_plane) {
                /* Fill Y plane with fuzzer data */
                for (size_t i = 0; i < y_size; i++) {
                    y_plane[i] = (i < Size) ? Data[i] : 0;
                }
                
                /* Fill U plane with fuzzer data */
                size_t u_offset = y_size;
                for (size_t i = 0; i < uv_size; i++) {
                    u_plane[i] = (u_offset + i < Size) ? Data[u_offset + i] : 0;
                }
                
                /* Fill V plane with fuzzer data */
                size_t v_offset = y_size + uv_size;
                for (size_t i = 0; i < uv_size; i++) {
                    v_plane[i] = (v_offset + i < Size) ? Data[v_offset + i] : 0;
                }
                
                src_planes[0] = y_plane;
                src_planes[1] = u_plane;
                src_planes[2] = v_plane;
                
                strides[0] = WIDTH;
                strides[1] = WIDTH/2;
                strides[2] = WIDTH/2;
                
                tjDecodeYUVPlanes(decomp_handle, src_planes, strides, TJSAMP_420,
                                 dst_buf, WIDTH, 0, HEIGHT, PIXEL_FORMAT, 0);
                
                free(y_plane);
                free(u_plane);
                free(v_plane);
            }
            
            free(dst_buf);
        }
    }
    
    /* Test tjLoadImage */
    {
        int width = 0, height = 0, pixel_format = 0;
        unsigned char *buffer = tjLoadImage(DUMMY_FILE, &width, 1, 
                                           &height, &pixel_format, 0);
        if (buffer) free(buffer);
    }
    
    /* Test tj3Get */
    {
        if (comp_handle) {
            tj3Get(comp_handle, TJPARAM_SUBSAMP);
            tj3Get(comp_handle, TJPARAM_QUALITY);
            tj3Get(comp_handle, TJPARAM_FASTDCT);
        }
        if (decomp_handle) {
            tj3Get(decomp_handle, TJPARAM_SCANLIMIT);
            tj3Get(decomp_handle, TJPARAM_BOTTOMUP);
        }
    }
    
    /* Test tj3LoadImage16 */
    {
        int width = 0, height = 0, pixel_format = TJPF_UNKNOWN;
        unsigned short *buffer = tj3LoadImage16(decomp_handle, DUMMY_FILE, 
                                               &width, 1, &height, &pixel_format);
        if (buffer) tj3Free(buffer);
    }
    
    /* Cleanup */
    if (comp_handle) tj3Destroy(comp_handle);
    if (decomp_handle) tj3Destroy(decomp_handle);
    cleanup_dummy_file();
    
    return 0;
}