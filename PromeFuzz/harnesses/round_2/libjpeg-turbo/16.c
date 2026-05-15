// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3LoadImage16 at turbojpeg-mp.c:293:21 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3JPEGBufSize at turbojpeg.c:903:18 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3SaveImage16 at turbojpeg-mp.c:442:15 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Compress16 at turbojpeg-mp.c:71:15 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Decompress16 at turbojpeg-mp.c:148:15 in turbojpeg.h
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

int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    if (Size < 7) return 0;
    
    // Initialize TurboJPEG handles
    tjhandle comp_handle = tj3Init(TJINIT_COMPRESS);
    tjhandle decomp_handle = tj3Init(TJINIT_DECOMPRESS);
    tjhandle generic_handle = tj3Init(TJINIT_DECOMPRESS);
    
    if (!comp_handle || !decomp_handle || !generic_handle) {
        if (comp_handle) tj3Destroy(comp_handle);
        if (decomp_handle) tj3Destroy(decomp_handle);
        if (generic_handle) tj3Destroy(generic_handle);
        return 0;
    }
    
    // Extract parameters from fuzzer input
    int width = (Data[0] << 8) | Data[1];
    int height = (Data[2] << 8) | Data[3];
    int pixel_format = Data[4] % 12;  // TJPF values 0-11
    int align = 1 << (Data[5] % 5);  // 1, 2, 4, 8, 16
    int subsamp = Data[6] % 6;       // TJSAMP values 0-5
    
    // Ensure reasonable dimensions to avoid excessive memory usage
    if (width <= 0) width = 1;
    if (height <= 0) height = 1;
    if (width > 4096) width = 4096;
    if (height > 4096) height = 4096;
    
    // Test tj3JPEGBufSize
    size_t max_buf_size = tj3JPEGBufSize(width, height, subsamp);
    
    // Test tj3Alloc with various sizes
    size_t alloc_size = Size % 65536;
    if (alloc_size == 0) alloc_size = 1;
    void *allocated_buf = tj3Alloc(alloc_size);
    if (allocated_buf) {
        // Use the buffer
        memset(allocated_buf, 0, alloc_size);
        tj3Free(allocated_buf);
    }
    
    // Create a source buffer for 16-bit operations
    int pixel_size = tjPixelSize[pixel_format];
    size_t src_buf_size = width * height * pixel_size * 2;  // 16-bit per sample
    unsigned short *src_buf = NULL;
    
    if (src_buf_size > 0 && src_buf_size < 100 * 1024 * 1024) {  // Limit to 100MB
        src_buf = (unsigned short *)tj3Alloc(src_buf_size);
        if (src_buf) {
            // Fill with data from fuzzer input
            size_t copy_size = Size > src_buf_size ? src_buf_size : Size;
            memcpy(src_buf, Data, copy_size);
            
            // Test tj3SaveImage16
            write_dummy_file(Data, Size % 1024);  // Limit file size
            tj3SaveImage16(generic_handle, "./dummy_file", src_buf, 
                          width, 0, height, pixel_format);
            remove_dummy_file();
            
            // Test tj3Compress16
            unsigned char *jpeg_buf = NULL;
            size_t jpeg_size = 0;
            
            if (max_buf_size > 0 && max_buf_size < 100 * 1024 * 1024) {
                jpeg_buf = (unsigned char *)tj3Alloc(max_buf_size);
                if (jpeg_buf) {
                    jpeg_size = max_buf_size;
                    tj3Compress16(comp_handle, src_buf, width, 0, height, 
                                 pixel_format, &jpeg_buf, &jpeg_size);
                    
                    // Test tj3Decompress16 if compression succeeded and we have JPEG data
                    if (jpeg_size > 0 && jpeg_size <= max_buf_size) {
                        unsigned short *dst_buf = (unsigned short *)tj3Alloc(src_buf_size);
                        if (dst_buf) {
                            tj3Decompress16(decomp_handle, jpeg_buf, jpeg_size, 
                                           dst_buf, 0, pixel_format);
                            tj3Free(dst_buf);
                        }
                    }
                    
                    tj3Free(jpeg_buf);
                }
            }
            
            // Test tj3LoadImage16
            write_dummy_file(Data, Size % 1024);
            int loaded_width = 0, loaded_height = 0, loaded_pixel_format = pixel_format;
            unsigned short *loaded_buf = tj3LoadImage16(generic_handle, "./dummy_file", 
                                                       &loaded_width, align, 
                                                       &loaded_height, &loaded_pixel_format);
            if (loaded_buf) {
                tj3Free(loaded_buf);
            }
            remove_dummy_file();
            
            tj3Free(src_buf);
        }
    }
    
    // Clean up
    tj3Destroy(comp_handle);
    tj3Destroy(decomp_handle);
    tj3Destroy(generic_handle);
    
    return 0;
}