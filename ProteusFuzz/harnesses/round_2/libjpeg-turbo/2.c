// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3JPEGBufSize at turbojpeg.c:903:18 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3EncodeYUV8 at turbojpeg.c:1688:15 in turbojpeg.h
// tj3CompressFromYUV8 at turbojpeg.c:1429:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <turbojpeg.h>

static int fuzz_helper(const uint8_t *Data, size_t Size) {
    if (Size < 8) return 0;

    int width = Data[0];
    int height = Data[1];
    int pixelFormat = Data[2] & 0x03; 
    int subsamp = Data[3] & 0x03;     
    int align = Data[4];
    int quality = Data[5];
    int param2 = Data[6];
    int param3 = Data[7];

    if (align == 0) align = 1;
    if (width <= 0 || width > 4096) width = 1;
    if (height <= 0 || height > 4096) height = 1;

    tjhandle handle = tj3Init(TJINIT_COMPRESS);
    if (!handle) return 0;

    tj3Set(handle, TJPARAM_QUALITY, quality);
    tj3Set(handle, TJPARAM_SUBSAMP, subsamp);
    tj3Set(handle, TJPARAM_JPEGWIDTH, width);
    tj3Set(handle, TJPARAM_JPEGHEIGHT, height);
    tj3Set(handle, TJPARAM_FASTDCT, param2);
    tj3Set(handle, TJPARAM_PROGRESSIVE, param3);
    tj3Set(handle, TJPARAM_NOREALLOC, 0);
    tj3Set(handle, TJPARAM_FASTDCT, 0);
    tj3Set(handle, TJPARAM_FASTDCT, 0);

    size_t jpegBufSize = tj3JPEGBufSize(width, height, subsamp);
    unsigned char *jpegBuf = NULL;
    size_t jpegSize = 0;

    if (jpegBufSize > 0) {
        jpegBuf = (unsigned char *)tj3Alloc(jpegBufSize);
        jpegSize = jpegBufSize;
    }

    size_t yuvBufSize = tj3YUVBufSize(width, align, height, subsamp);
    unsigned char *yuvBuf = NULL;
    if (yuvBufSize > 0) {
        yuvBuf = (unsigned char *)tj3Alloc(yuvBufSize);
        if (yuvBuf) {
            memset(yuvBuf, 0, yuvBufSize);
        }
    }

    tj3Set(handle, TJPARAM_FASTDCT, 0);
    tj3Set(handle, TJPARAM_FASTDCT, 0);

    int pitch = width * 3; 
    size_t srcSize = (size_t)pitch * height;
    unsigned char *srcBuf = NULL;
    if (srcSize > 0 && srcSize < Size) {
        srcBuf = (unsigned char *)malloc(srcSize);
        if (srcBuf) {
            memcpy(srcBuf, Data, srcSize);
        }
    } else if (srcSize > 0) {
        srcBuf = (unsigned char *)malloc(srcSize);
        if (srcBuf) {
            memset(srcBuf, 0x55, srcSize);
        }
    }

    if (srcBuf && yuvBuf) {
        tj3EncodeYUV8(handle, srcBuf, width, pitch, height, pixelFormat, yuvBuf, align);
    }

    if (yuvBuf && jpegBuf) {
        tj3CompressFromYUV8(handle, yuvBuf, width, align, height, &jpegBuf, &jpegSize);
    }

    if (srcBuf) free(srcBuf);
    tj3Free(yuvBuf);
    tj3Free(jpegBuf);
    tj3Destroy(handle);

    return 0;
}

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    return fuzz_helper(Data, Size);
}