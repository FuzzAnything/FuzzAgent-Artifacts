// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjAlloc at turbojpeg.c:883:26 in turbojpeg.h
// tjCompress at turbojpeg.c:1235:15 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3CompressFromYUV8 at turbojpeg.c:1429:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// TJBUFSIZE at turbojpeg.c:948:25 in turbojpeg.h
// tjBufSize at turbojpeg.c:933:25 in turbojpeg.h
// tj3JPEGBufSize at turbojpeg.c:903:18 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tjAlloc at turbojpeg.c:883:26 in turbojpeg.h
// tjCompress2 at turbojpeg.c:1204:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "turbojpeg.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int get_valid_subsamp(int value) {
    switch (value % 6) {
        case 0: return TJSAMP_444;
        case 1: return TJSAMP_422;
        case 2: return TJSAMP_420;
        case 3: return TJSAMP_GRAY;
        case 4: return TJSAMP_440;
        case 5: return TJSAMP_411;
        default: return TJSAMP_444;
    }
}

static int get_valid_pixel_format(int value) {
    switch (value % 10) {
        case 0: return TJPF_RGB;
        case 1: return TJPF_BGR;
        case 2: return TJPF_RGBX;
        case 3: return TJPF_BGRX;
        case 4: return TJPF_XRGB;
        case 5: return TJPF_XBGR;
        case 6: return TJPF_GRAY;
        case 7: return TJPF_RGBA;
        case 8: return TJPF_BGRA;
        case 9: return TJPF_ABGR;
        default: return TJPF_RGB;
    }
}

static int get_valid_align(int value) {
    int align = 1 << (value % 4);
    return align > 0 ? align : 1;
}

int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    if (Size < 20) return 0;
    
    // Parse parameters from input data
    int width = *(int*)Data % 1024 + 1;
    int height = *(int*)(Data + 4) % 1024 + 1;
    int align = get_valid_align(*(int*)(Data + 8));
    int pixel_format = get_valid_pixel_format(*(int*)(Data + 12));
    int jpeg_subsamp = get_valid_subsamp(*(int*)(Data + 16));
    int jpeg_qual = *(int*)(Data + 20) % 100 + 1;
    int flags = *(int*)(Data + 24) % 256;
    
    // Test TJBUFSIZE
    unsigned long buf_size1 = TJBUFSIZE(width, height);
    
    // Test tjBufSize
    unsigned long buf_size2 = tjBufSize(width, height, jpeg_subsamp);
    
    // Test tj3JPEGBufSize
    size_t buf_size3 = tj3JPEGBufSize(width, height, jpeg_subsamp);
    
    // Create handles for compression
    tjhandle handle_old = tjInitCompress();
    tjhandle handle_new = tj3Init(TJINIT_COMPRESS);
    
    if (handle_old && handle_new) {
        // Prepare source buffer for compression
        size_t src_size = width * height * 3;
        unsigned char* src_buf = NULL;
        if (Size > 28 && Size - 28 >= src_size) {
            src_buf = (unsigned char*)Data + 28;
        } else {
            src_buf = (unsigned char*)malloc(src_size);
            if (src_buf) {
                size_t copy_size = Size > 28 ? Size - 28 : 0;
                if (copy_size > 0 && copy_size <= src_size) {
                    memcpy(src_buf, Data + 28, copy_size);
                }
                if (copy_size < src_size) {
                    memset(src_buf + copy_size, 0, src_size - copy_size);
                }
            }
        }
        
        if (src_buf) {
            // Test tjCompress2
            unsigned char* jpeg_buf1 = NULL;
            unsigned long jpeg_size1 = 0;
            if (buf_size2 > 0) {
                jpeg_buf1 = (unsigned char*)tjAlloc(buf_size2);
            }
            
            int result1 = tjCompress2(handle_old, src_buf, width, 0, height, 
                                      pixel_format, &jpeg_buf1, &jpeg_size1, 
                                      jpeg_subsamp, jpeg_qual, flags);
            
            if (jpeg_buf1) {
                tjFree(jpeg_buf1);
            }
            
            // Test tjCompress
            unsigned char* dst_buf = NULL;
            unsigned long compressed_size = 0;
            if (buf_size2 > 0) {
                dst_buf = (unsigned char*)tjAlloc(buf_size2);
                compressed_size = buf_size2;
                
                int result2 = tjCompress(handle_old, src_buf, width, 0, height, 
                                         3, dst_buf, &compressed_size, 
                                         jpeg_subsamp, jpeg_qual, flags);
                
                if (dst_buf) {
                    tjFree(dst_buf);
                }
            }
            
            // Test tj3CompressFromYUV8
            if (handle_new) {
                // Set parameters for new API
                tj3Set(handle_new, TJPARAM_SUBSAMP, jpeg_subsamp);
                tj3Set(handle_new, TJPARAM_QUALITY, jpeg_qual);
                
                // Prepare YUV buffer
                size_t yuv_size = width * height * 2;
                unsigned char* yuv_buf = NULL;
                if (Size > 28 && Size - 28 >= yuv_size) {
                    yuv_buf = (unsigned char*)Data + 28;
                } else {
                    yuv_buf = (unsigned char*)malloc(yuv_size);
                    if (yuv_buf) {
                        size_t copy_size = Size > 28 ? Size - 28 : 0;
                        if (copy_size > 0 && copy_size <= yuv_size) {
                            memcpy(yuv_buf, Data + 28, copy_size);
                        }
                        if (copy_size < yuv_size) {
                            memset(yuv_buf + copy_size, 0, yuv_size - copy_size);
                        }
                    }
                }
                
                if (yuv_buf) {
                    unsigned char* jpeg_buf2 = NULL;
                    size_t jpeg_size2 = 0;
                    
                    int result3 = tj3CompressFromYUV8(handle_new, yuv_buf, 
                                                     width, align, height,
                                                     &jpeg_buf2, &jpeg_size2);
                    
                    if (jpeg_buf2) {
                        tj3Free(jpeg_buf2);
                    }
                    
                    if (yuv_buf != Data + 28) {
                        free(yuv_buf);
                    }
                }
            }
            
            if (src_buf != Data + 28) {
                free(src_buf);
            }
        }
    }
    
    if (handle_old) {
        tjDestroy(handle_old);
    }
    if (handle_new) {
        tj3Destroy(handle_new);
    }
    
    return 0;
}