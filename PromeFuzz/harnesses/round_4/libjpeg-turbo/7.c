// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3Decompress16 at turbojpeg-mp.c:148:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Compress16 at turbojpeg-mp.c:71:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3JPEGBufSize at turbojpeg.c:903:18 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3SaveImage16 at turbojpeg-mp.c:442:15 in turbojpeg.h
// tj3LoadImage16 at turbojpeg-mp.c:293:21 in turbojpeg.h
// tj3Compress16 at turbojpeg-mp.c:71:15 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
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
#include <fcntl.h>

static const char* DUMMY_FILE = "./dummy_file";

static void write_dummy_file(const uint8_t* data, size_t size) {
    int fd = open(DUMMY_FILE, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        write(fd, data, size);
        close(fd);
    }
}

static void remove_dummy_file(void) {
    unlink(DUMMY_FILE);
}

int LLVMFuzzerTestOneInput_7(const uint8_t* Data, size_t Size) {
    if (Size < 32) return 0;

    // Initialize handles
    tjhandle comp_handle = tj3Init(TJINIT_COMPRESS);
    tjhandle decomp_handle = tj3Init(TJINIT_DECOMPRESS);
    if (!comp_handle || !decomp_handle) {
        if (comp_handle) tj3Destroy(comp_handle);
        if (decomp_handle) tj3Destroy(decomp_handle);
        return 0;
    }

    // Extract parameters from fuzz data
    int width = (Data[0] << 8) | Data[1];
    int height = (Data[2] << 8) | Data[3];
    int pixel_format = Data[4] % 13; // TJPF values 0-12
    int align = 1 << (Data[5] % 4); // Power of 2 alignment: 1,2,4,8
    int param = Data[6] % 10; // Some TJPARAM values
    int value = Data[7];
    
    // Ensure reasonable dimensions
    if (width <= 0) width = 1;
    if (height <= 0) height = 1;
    if (width > 4096) width = 4096;
    if (height > 4096) height = 4096;

    // Calculate buffer sizes
    int pixel_size = tjPixelSize[pixel_format];
    size_t sample_buf_size = width * height * pixel_size * sizeof(unsigned short);
    size_t jpeg_buf_size = tj3JPEGBufSize(width, height, pixel_format);
    
    // Test tj3Alloc
    void* alloc_buf = tj3Alloc(sample_buf_size);
    if (!alloc_buf) {
        tj3Destroy(comp_handle);
        tj3Destroy(decomp_handle);
        return 0;
    }
    
    // Fill buffer with fuzz data
    size_t copy_size = sample_buf_size < Size - 8 ? sample_buf_size : Size - 8;
    memcpy(alloc_buf, Data + 8, copy_size);
    
    // Test tj3Set
    tj3Set(comp_handle, param, value);
    tj3Set(decomp_handle, param, value);
    
    // Test tj3SaveImage16 with dummy file
    write_dummy_file(Data, Size);
    int save_ret = tj3SaveImage16(comp_handle, DUMMY_FILE, 
                                  (const unsigned short*)alloc_buf,
                                  width, 0, height, pixel_format);
    (void)save_ret; // Ignore return value
    
    // Test tj3LoadImage16
    int loaded_width, loaded_height, loaded_pixel_format = pixel_format;
    unsigned short* loaded_buf = tj3LoadImage16(decomp_handle, DUMMY_FILE,
                                                &loaded_width, align,
                                                &loaded_height, &loaded_pixel_format);
    
    // Test tj3Compress16 with different buffer strategies
    unsigned char* jpeg_buf = NULL;
    size_t jpeg_size = jpeg_buf_size;
    
    // Strategy 1: Let TurboJPEG allocate
    int comp_ret = tj3Compress16(comp_handle, (const unsigned short*)alloc_buf,
                                 width, 0, height, pixel_format,
                                 &jpeg_buf, &jpeg_size);
    
    if (comp_ret == 0 && jpeg_buf && jpeg_size > 0) {
        // Test tj3Decompress16
        unsigned short* decomp_buf = (unsigned short*)tj3Alloc(sample_buf_size);
        if (decomp_buf) {
            tj3Decompress16(decomp_handle, jpeg_buf, jpeg_size,
                           decomp_buf, 0, pixel_format);
            tj3Free(decomp_buf);
        }
        
        // Strategy 2: Pre-allocated buffer
        unsigned char* prealloc_buf = (unsigned char*)tj3Alloc(jpeg_buf_size);
        size_t prealloc_size = jpeg_buf_size;
        if (prealloc_buf) {
            tj3Compress16(comp_handle, (const unsigned short*)alloc_buf,
                         width, 0, height, pixel_format,
                         &prealloc_buf, &prealloc_size);
            tj3Free(prealloc_buf);
        }
    }
    
    // Cleanup
    if (jpeg_buf) tj3Free(jpeg_buf);
    if (loaded_buf) tj3Free(loaded_buf);
    tj3Free(alloc_buf);
    remove_dummy_file();
    tj3Destroy(comp_handle);
    tj3Destroy(decomp_handle);
    
    return 0;
}