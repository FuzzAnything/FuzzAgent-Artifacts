// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3JPEGBufSize at turbojpeg.c:903:18 in turbojpeg.h
// tj3Compress16 at turbojpeg-mp.c:71:15 in turbojpeg.h
// tj3CompressFromYUV8 at turbojpeg.c:1429:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <turbojpeg.h>

static void cleanup_instance(tjhandle handle) {
    if (handle) {
        tj3Destroy(handle);
    }
}

static int fuzz_tj3Get(tjhandle handle, int param) {
    return tj3Get(handle, param);
}

static int fuzz_tj3Set(tjhandle handle, int param, int value) {
    return tj3Set(handle, param, value);
}

static tjhandle fuzz_tj3Init(int initType) {
    return tj3Init(initType);
}

static size_t fuzz_tj3JPEGBufSize(int width, int height, int jpegSubsamp) {
    return tj3JPEGBufSize(width, height, jpegSubsamp);
}

static int fuzz_tj3Compress16(tjhandle handle, const unsigned short *srcBuf,
                              int width, int pitch, int height, int pixelFormat,
                              unsigned char **jpegBuf, size_t *jpegSize) {
    return tj3Compress16(handle, srcBuf, width, pitch, height, pixelFormat,
                         jpegBuf, jpegSize);
}

static int fuzz_tj3CompressFromYUV8(tjhandle handle,
                                    const unsigned char *srcBuf, int width,
                                    int align, int height,
                                    unsigned char **jpegBuf, size_t *jpegSize) {
    return tj3CompressFromYUV8(handle, srcBuf, width, align, height,
                               jpegBuf, jpegSize);
}

int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    if (Size < 8) return 0;

    int initType = Data[0];
    int param = Data[1];
    int value = (int)Data[2];
    int width = (int)Data[3];
    int height = (int)Data[4];
    int jpegSubsamp = Data[5];
    int pixelFormat = Data[6];
    int align = Data[7];

    if (width <= 0 || height <= 0) return 0;

    tjhandle handle = fuzz_tj3Init(initType);
    if (!handle) return 0;

    fuzz_tj3Set(handle, param, value);
    fuzz_tj3Get(handle, param);

    size_t jpegBufSize = fuzz_tj3JPEGBufSize(width, height, jpegSubsamp);
    if (jpegBufSize == 0) {
        cleanup_instance(handle);
        return 0;
    }

    unsigned char *jpegBuf = NULL;
    size_t jpegSize = 0;

    unsigned short *srcBuf16 = NULL;
    if (Size >= 8 + width * height * 2) {
        srcBuf16 = (unsigned short *)malloc(width * height * sizeof(unsigned short));
        if (srcBuf16) {
            memcpy(srcBuf16, Data + 8, width * height * sizeof(unsigned short));
            int pitch = width * 2;
            fuzz_tj3Compress16(handle, srcBuf16, width, pitch, height, pixelFormat,
                               &jpegBuf, &jpegSize);
            free(srcBuf16);
        }
    }

    if (jpegBuf) {
        free(jpegBuf);
        jpegBuf = NULL;
        jpegSize = 0;
    }

    unsigned char *srcBufYUV = NULL;
    if (Size >= 8 + width * height * 2) {
        srcBufYUV = (unsigned char *)malloc(width * height * 2);
        if (srcBufYUV) {
            memcpy(srcBufYUV, Data + 8, width * height * 2);
            fuzz_tj3CompressFromYUV8(handle, srcBufYUV, width, align, height,
                                     &jpegBuf, &jpegSize);
            free(srcBufYUV);
        }
    }

    if (jpegBuf) {
        free(jpegBuf);
    }

    cleanup_instance(handle);
    return 0;
}