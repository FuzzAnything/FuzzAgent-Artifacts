// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3SetScalingFactor at turbojpeg.c:1981:15 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
// tj3DecompressToYUV8 at turbojpeg.c:2338:15 in turbojpeg.h
// tj3DecodeYUV8 at turbojpeg.c:2675:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "turbojpeg.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static tjscalingfactor get_scaling_factor_from_data(const uint8_t *data, size_t size) {
    tjscalingfactor factor;
    if (size >= 2) {
        factor.num = (data[0] % 8) + 1;
        factor.denom = (data[1] % 8) + 1;
    } else {
        factor.num = 1;
        factor.denom = 1;
    }
    return factor;
}

int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    tjhandle handle = tj3Init(TJINIT_DECOMPRESS);
    if (!handle) return 0;

    tjscalingfactor scalingFactor = get_scaling_factor_from_data(Data, Size);
    tj3SetScalingFactor(handle, scalingFactor);

    size_t alloc_size1 = Size > 0 ? Data[0] * 1024 : 1024;
    void *buffer1 = tj3Alloc(alloc_size1);

    size_t alloc_size2 = Size > 1 ? Data[1] * 1024 : 2048;
    void *buffer2 = tj3Alloc(alloc_size2);

    int width = Size > 2 ? Data[2] * 4 + 1 : 64;
    int height = Size > 3 ? Data[3] * 4 + 1 : 64;
    int align = Size > 4 ? 1 << (Data[4] % 4) : 4;
    int subsamp = Size > 5 ? Data[5] % 4 : TJSAMP_420;

    size_t yuv_buf_size = tj3YUVBufSize(width, align, height, subsamp);

    if (yuv_buf_size > 0 && buffer1 && buffer2) {
        const char *dummy_file = "./dummy_file";
        FILE *fp = fopen(dummy_file, "wb");
        if (fp) {
            fwrite(Data, 1, Size, fp);
            fclose(fp);
        }

        fp = fopen(dummy_file, "rb");
        if (fp) {
            fseek(fp, 0, SEEK_END);
            long file_size = ftell(fp);
            fseek(fp, 0, SEEK_SET);
            
            unsigned char *jpeg_buf = (unsigned char *)malloc(file_size);
            if (jpeg_buf) {
                fread(jpeg_buf, 1, file_size, fp);
                tj3DecompressToYUV8(handle, jpeg_buf, file_size, (unsigned char *)buffer1, align);
                free(jpeg_buf);
            }
            fclose(fp);
        }

        int pitch = width * 3;
        int pixelFormat = Size > 6 ? Data[6] % 10 : TJPF_RGB;
        tj3DecodeYUV8(handle, (const unsigned char *)buffer1, align, (unsigned char *)buffer2,
                      width, pitch, height, pixelFormat);
    }

    if (buffer1) tj3Free(buffer1);
    if (buffer2) tj3Free(buffer2);
    tj3Destroy(handle);

    return 0;
}