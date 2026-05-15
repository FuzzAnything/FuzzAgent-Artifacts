// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3SetScalingFactor at turbojpeg.c:1981:15 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3DecompressToYUV8 at turbojpeg.c:2338:15 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3DecodeYUV8 at turbojpeg.c:2675:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
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
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) return;
    fwrite(data, 1, size, f);
    fclose(f);
}

int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size) {
    if (Size < 100) return 0;
    
    write_dummy_file(Data, Size);
    
    tjhandle handle = tj3Init(TJINIT_DECOMPRESS);
    if (!handle) return 0;
    
    tjscalingfactor scaling_factor;
    scaling_factor.num = 1;
    scaling_factor.denom = 1;
    
    tj3SetScalingFactor(handle, scaling_factor);
    
    void *alloc1 = tj3Alloc(Size);
    void *alloc2 = tj3Alloc(Size * 2);
    
    int width = 64;
    int height = 64;
    int align = 4;
    int subsamp = TJSAMP_420;
    
    size_t yuv_size = tj3YUVBufSize(width, align, height, subsamp);
    if (yuv_size == 0) {
        tj3Free(alloc1);
        tj3Free(alloc2);
        tj3Destroy(handle);
        return 0;
    }
    
    unsigned char *yuv_buf = (unsigned char *)tj3Alloc(yuv_size);
    if (!yuv_buf) {
        tj3Free(alloc1);
        tj3Free(alloc2);
        tj3Destroy(handle);
        return 0;
    }
    
    int decompress_ret = tj3DecompressToYUV8(handle, Data, Size, yuv_buf, align);
    
    if (decompress_ret == 0) {
        int pixel_format = TJPF_RGB;
        int pitch = width * tjPixelSize[pixel_format];
        unsigned char *rgb_buf = (unsigned char *)tj3Alloc(pitch * height);
        if (rgb_buf) {
            tj3DecodeYUV8(handle, yuv_buf, align, rgb_buf, width, pitch, height, pixel_format);
            tj3Free(rgb_buf);
        }
    }
    
    tj3Free(yuv_buf);
    tj3Free(alloc1);
    tj3Free(alloc2);
    tj3Destroy(handle);
    
    return 0;
}