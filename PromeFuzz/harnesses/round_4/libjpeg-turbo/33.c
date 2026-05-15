// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjCompress2 at turbojpeg.c:1204:15 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjCompress at turbojpeg.c:1235:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
// tj3CompressFromYUV8 at turbojpeg.c:1429:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// TJBUFSIZE at turbojpeg.c:948:25 in turbojpeg.h
// tj3JPEGBufSize at turbojpeg.c:903:18 in turbojpeg.h
// tjBufSize at turbojpeg.c:933:25 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "src/turbojpeg.h"

static void fill_buffer_with_fuzz_data(unsigned char* buffer, size_t buffer_size, 
                                       const uint8_t* fuzz_data, size_t fuzz_size, 
                                       size_t* offset) {
    if (buffer_size == 0) return;
    
    for (size_t i = 0; i < buffer_size; i++) {
        if (*offset < fuzz_size) {
            buffer[i] = fuzz_data[*offset];
            (*offset)++;
        } else {
            buffer[i] = (i % 256);
        }
    }
}

static int get_subsampling_from_fuzz(const uint8_t* data, size_t size, size_t* offset) {
    if (*offset < size) {
        int subsamp = data[*offset] % 6;
        (*offset)++;
        return subsamp;
    }
    return TJSAMP_444;
}

static int get_quality_from_fuzz(const uint8_t* data, size_t size, size_t* offset) {
    if (*offset < size) {
        int quality = 1 + (data[*offset] % 100);
        (*offset)++;
        return quality;
    }
    return 50;
}

static int get_pixel_format_from_fuzz(const uint8_t* data, size_t size, size_t* offset) {
    if (*offset < size) {
        int pixel_format = data[*offset] % 12;
        (*offset)++;
        return pixel_format;
    }
    return TJPF_RGB;
}

static int get_align_from_fuzz(const uint8_t* data, size_t size, size_t* offset) {
    if (*offset < size) {
        int align = 1 << (data[*offset] % 5);
        if (align == 0) align = 1;
        (*offset)++;
        return align;
    }
    return 1;
}

static int get_width_from_fuzz(const uint8_t* data, size_t size, size_t* offset) {
    if (*offset + 1 < size) {
        int width = 1 + ((data[*offset] << 8) | data[*offset + 1]) % 1024;
        *offset += 2;
        return width;
    }
    return 64;
}

static int get_height_from_fuzz(const uint8_t* data, size_t size, size_t* offset) {
    if (*offset + 1 < size) {
        int height = 1 + ((data[*offset] << 8) | data[*offset + 1]) % 1024;
        *offset += 2;
        return height;
    }
    return 64;
}

int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size) {
    if (Size < 16) return 0;
    
    size_t offset = 0;
    
    int width = get_width_from_fuzz(Data, Size, &offset);
    int height = get_height_from_fuzz(Data, Size, &offset);
    
    if (width <= 0 || height <= 0) {
        width = 64;
        height = 64;
    }
    
    int subsamp = get_subsampling_from_fuzz(Data, Size, &offset);
    int quality = get_quality_from_fuzz(Data, Size, &offset);
    int pixel_format = get_pixel_format_from_fuzz(Data, Size, &offset);
    int align = get_align_from_fuzz(Data, Size, &offset);
    
    unsigned long bufsize1 = TJBUFSIZE(width, height);
    size_t bufsize2 = tj3JPEGBufSize(width, height, subsamp);
    unsigned long bufsize3 = tjBufSize(width, height, subsamp);
    
    tjhandle handle_old = NULL;
    tjhandle handle_new = NULL;
    
    handle_old = tjInitCompress();
    if (handle_old) {
        unsigned char* src_buf = NULL;
        unsigned char* jpeg_buf = NULL;
        unsigned long jpeg_size = 0;
        
        int pitch = width * tjPixelSize[pixel_format];
        size_t src_size = (size_t)pitch * (size_t)height;
        
        if (src_size > 0 && src_size < 100 * 1024 * 1024) {
            src_buf = (unsigned char*)malloc(src_size);
            if (src_buf) {
                fill_buffer_with_fuzz_data(src_buf, src_size, Data, Size, &offset);
                
                tjCompress2(handle_old, src_buf, width, pitch, height, pixel_format,
                           &jpeg_buf, &jpeg_size, subsamp, quality, 0);
                
                if (jpeg_buf) {
                    tjFree(jpeg_buf);
                }
                
                if (bufsize3 > 0 && bufsize3 < 100 * 1024 * 1024) {
                    unsigned char* dst_buf = (unsigned char*)malloc(bufsize3);
                    if (dst_buf) {
                        tjCompress(handle_old, src_buf, width, pitch, height, 
                                  tjPixelSize[pixel_format], dst_buf, &jpeg_size, 
                                  subsamp, quality, 0);
                        free(dst_buf);
                    }
                }
                
                free(src_buf);
            }
        }
        
        tjDestroy(handle_old);
    }
    
    handle_new = tj3Init(TJINIT_COMPRESS);
    if (handle_new) {
        tj3Set(handle_new, TJPARAM_SUBSAMP, subsamp);
        tj3Set(handle_new, TJPARAM_QUALITY, quality);
        
        size_t yuv_buf_size = tj3YUVBufSize(width, align, height, subsamp);
        if (yuv_buf_size > 0 && yuv_buf_size < 100 * 1024 * 1024) {
            unsigned char* yuv_buf = (unsigned char*)malloc(yuv_buf_size);
            
            if (yuv_buf) {
                fill_buffer_with_fuzz_data(yuv_buf, yuv_buf_size, Data, Size, &offset);
                
                unsigned char* jpeg_buf = NULL;
                size_t jpeg_size = 0;
                
                tj3CompressFromYUV8(handle_new, yuv_buf, width, align, height,
                                   &jpeg_buf, &jpeg_size);
                
                if (jpeg_buf) {
                    free(jpeg_buf);
                }
                
                free(yuv_buf);
            }
        }
        
        tj3Destroy(handle_new);
    }
    
    return 0;
}