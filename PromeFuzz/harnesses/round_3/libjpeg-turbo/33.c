// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3JPEGBufSize at turbojpeg.c:903:18 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Compress16 at turbojpeg-mp.c:71:15 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Decompress16 at turbojpeg-mp.c:148:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3SaveImage16 at turbojpeg-mp.c:442:15 in turbojpeg.h
// tj3LoadImage16 at turbojpeg-mp.c:293:21 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "turbojpeg.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static void remove_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size) {
    if (Size < 16) return 0;
    
    tjhandle handle_comp = tj3Init(TJINIT_COMPRESS);
    tjhandle handle_decomp = tj3Init(TJINIT_DECOMPRESS);
    if (!handle_comp || !handle_decomp) {
        if (handle_comp) tj3Destroy(handle_comp);
        if (handle_decomp) tj3Destroy(handle_decomp);
        return 0;
    }
    
    int width = (Data[0] << 8) | Data[1];
    int height = (Data[2] << 8) | Data[3];
    int pixel_format = Data[4] % 12;
    int subsamp = Data[5] % 6;
    int align = 1 << (Data[6] % 5);
    
    if (width <= 0) width = 16;
    if (height <= 0) height = 16;
    if (width > 1024) width = 1024;
    if (height > 1024) height = 1024;
    if (align < 1) align = 1;
    
    size_t sample_size = sizeof(unsigned short);
    size_t pixel_size = tjPixelSize[pixel_format];
    size_t pitch = width * pixel_size;
    size_t src_buf_size = pitch * height;
    
    unsigned short *src_buf = NULL;
    if (src_buf_size > 0 && src_buf_size <= Size - 16) {
        src_buf = (unsigned short *)tj3Alloc(src_buf_size * sample_size);
        if (src_buf) {
            memcpy(src_buf, Data + 16, src_buf_size * sample_size);
        }
    }
    
    if (src_buf) {
        unsigned char *jpeg_buf = NULL;
        size_t jpeg_size = 0;
        
        size_t max_buf_size = tj3JPEGBufSize(width, height, subsamp);
        if (max_buf_size > 0) {
            jpeg_buf = (unsigned char *)tj3Alloc(max_buf_size);
            if (jpeg_buf) {
                jpeg_size = max_buf_size;
                
                int ret = tj3Compress16(handle_comp, src_buf, width, pitch, height,
                                       pixel_format, &jpeg_buf, &jpeg_size);
                if (ret == 0 && jpeg_buf && jpeg_size > 0) {
                    unsigned short *dst_buf = (unsigned short *)tj3Alloc(src_buf_size * sample_size);
                    if (dst_buf) {
                        tj3Decompress16(handle_decomp, jpeg_buf, jpeg_size, dst_buf,
                                       pitch, pixel_format);
                        tj3Free(dst_buf);
                    }
                }
                
                if (jpeg_buf) tj3Free(jpeg_buf);
            }
        }
        
        write_dummy_file(Data, Size);
        tj3SaveImage16(handle_comp, "./dummy_file", src_buf, width, pitch, height, pixel_format);
        
        int loaded_width, loaded_height, loaded_pixel_format;
        unsigned short *loaded_buf = tj3LoadImage16(handle_decomp, "./dummy_file",
                                                   &loaded_width, align, &loaded_height,
                                                   &loaded_pixel_format);
        if (loaded_buf) {
            tj3Free(loaded_buf);
        }
        
        remove_dummy_file();
        tj3Free(src_buf);
    }
    
    tj3Destroy(handle_comp);
    tj3Destroy(handle_decomp);
    
    return 0;
}