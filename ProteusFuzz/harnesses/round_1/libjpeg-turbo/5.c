// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Compress16 at turbojpeg-mp.c:71:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3LoadImage16 at turbojpeg-mp.c:293:21 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3JPEGBufSize at turbojpeg.c:903:18 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "turbojpeg.h"

static int write_dummy_file(const char *filename, const uint8_t *data, size_t size) {
    FILE *f = fopen(filename, "wb");
    if (!f) return -1;
    
    const char *header = "P5\n";
    size_t written = fwrite(header, 1, strlen(header), f);
    if (written != strlen(header)) { fclose(f); return -1; }
    
    int width = 1;
    int height = 1;
    
    if (size >= 4) {
        width = data[0] + 1;
        height = data[1] + 1;
        if (width > 4096) width = 4096;
        if (height > 4096) height = 4096;
    }
    
    char dim_str[64];
    int len = snprintf(dim_str, sizeof(dim_str), "%d %d\n65535\n", width, height);
    if (len <= 0 || len >= (int)sizeof(dim_str)) { fclose(f); return -1; }
    
    written = fwrite(dim_str, 1, len, f);
    if (written != (size_t)len) { fclose(f); return -1; }
    
    size_t needed = (size_t)width * height * 2;
    size_t to_write = (size < needed) ? size : needed;
    
    if (to_write > 0) {
        fwrite(data, 1, to_write, f);
    }
    
    if (to_write < needed) {
        unsigned char zero = 0;
        for (size_t i = to_write; i < needed; i++) {
            fwrite(&zero, 1, 1, f);
        }
    }
    
    fclose(f);
    return 0;
}

int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    tjhandle handle = tj3Init(TJINIT_COMPRESS);
    if (!handle) return 0;

    if (write_dummy_file("./dummy_file", Data, Size) != 0) {
        tj3Destroy(handle);
        return 0;
    }

    int p1 = (Size > 0) ? Data[0] % 100 : 50;
    int p2 = (Size > 1) ? Data[1] % 4 : 0;
    int p3 = (Size > 2) ? Data[2] % 4 : 16;
    int p4 = (Size > 3) ? Data[3] % 2 : 0;
    int p5 = (Size > 4) ? Data[4] % 2 : 0;

    tj3Set(handle, TJPARAM_QUALITY, p1);
    tj3Set(handle, TJPARAM_SUBSAMP, p2);
    tj3Set(handle, TJPARAM_PRECISION, p3);
    tj3Set(handle, TJPARAM_NOREALLOC, p4);
    tj3Set(handle, TJPARAM_PROGRESSIVE, p5);

    int width = 0, height = 0, pixelFormat = TJPF_UNKNOWN;
    int align = (Size > 5) ? (Data[5] % 4) + 1 : 1;
    if (align > 8) align = 1;
    
    unsigned short *srcBuf = tj3LoadImage16(handle, "./dummy_file", &width, align, &height, &pixelFormat);
    
    if (!srcBuf || width <= 0 || height <= 0) {
        tj3Destroy(handle);
        return 0;
    }

    int subsamp = (Size > 6) ? Data[6] % 4 : 0;
    size_t jpegSize = tj3JPEGBufSize(width, height, subsamp);
    if (jpegSize == 0) {
        tj3Free(srcBuf);
        tj3Destroy(handle);
        return 0;
    }

    int retrieved = tj3Get(handle, TJPARAM_QUALITY);
    (void)retrieved;

    unsigned char *jpegBuf = (unsigned char *)tj3Alloc(jpegSize);
    if (!jpegBuf) {
        tj3Free(srcBuf);
        tj3Destroy(handle);
        return 0;
    }

    int p6 = (Size > 7) ? Data[7] % 100 : 90;
    int p7 = (Size > 8) ? Data[8] % 4 : 0;
    int p8 = (Size > 9) ? Data[9] % 2 : 0;
    
    tj3Set(handle, TJPARAM_QUALITY, p6);
    tj3Set(handle, TJPARAM_SUBSAMP, p7);
    tj3Set(handle, TJPARAM_NOREALLOC, p8);

    int samplesPerPixel = tjPixelSize[pixelFormat];
    int pitch = width * samplesPerPixel;
    
    size_t actualSize = 0;
    tj3Compress16(handle, srcBuf, width, pitch, height, pixelFormat, &jpegBuf, &actualSize);

    tj3Free(srcBuf);
    tj3Free(jpegBuf);
    tj3Destroy(handle);

    return 0;
}