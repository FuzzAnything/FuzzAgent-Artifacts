// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3GetErrorStr at turbojpeg.c:618:17 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjDecompressHeader at turbojpeg.c:1914:15 in turbojpeg.h
// tjGetErrorCode at turbojpeg.c:652:15 in turbojpeg.h
// tjGetErrorStr at turbojpeg.c:636:17 in turbojpeg.h
// tjGetErrorStr2 at turbojpeg.c:630:17 in turbojpeg.h
// tj3GetErrorStr at turbojpeg.c:618:17 in turbojpeg.h
// tj3GetErrorStr at turbojpeg.c:618:17 in turbojpeg.h
// tjDecompress2 at turbojpeg.c:2059:15 in turbojpeg.h
// tjGetErrorCode at turbojpeg.c:652:15 in turbojpeg.h
// tjGetErrorStr at turbojpeg.c:636:17 in turbojpeg.h
// tjGetErrorStr2 at turbojpeg.c:630:17 in turbojpeg.h
// tj3GetErrorStr at turbojpeg.c:618:17 in turbojpeg.h
// tjGetErrorCode at turbojpeg.c:652:15 in turbojpeg.h
// tjGetErrorStr at turbojpeg.c:636:17 in turbojpeg.h
// tjGetErrorStr2 at turbojpeg.c:630:17 in turbojpeg.h
// tj3GetErrorStr at turbojpeg.c:618:17 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "turbojpeg.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void cleanup_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_34(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    tjhandle handle = NULL;
    int width = 0, height = 0;
    int ret = 0;
    unsigned char *dstBuf = NULL;
    unsigned long dstBufSize = 0;
    int pixelFormat = TJPF_RGB;
    int flags = 0;

    handle = tjInitDecompress();
    if (!handle) {
        return 0;
    }

    write_dummy_file(Data, Size);

    ret = tjDecompressHeader(handle, (unsigned char *)Data, (unsigned long)Size, &width, &height);
    if (ret == 0 && width > 0 && height > 0) {
        int errorCode = tjGetErrorCode(handle);
        (void)errorCode;

        const char *errorStr1 = tjGetErrorStr();
        (void)errorStr1;

        const char *errorStr2 = tjGetErrorStr2(handle);
        (void)errorStr2;

        const char *errorStr3 = tj3GetErrorStr(handle);
        (void)errorStr3;

        const char *errorStr4 = tj3GetErrorStr(NULL);
        (void)errorStr4;

        pixelFormat = Data[0] % (TJPF_CMYK + 1);
        flags = Data[1] % 4;

        int pitch = width * tjPixelSize[pixelFormat];
        dstBufSize = (unsigned long)pitch * (unsigned long)height;
        if (dstBufSize > 0 && dstBufSize < 100 * 1024 * 1024) {
            dstBuf = (unsigned char *)malloc(dstBufSize);
            if (dstBuf) {
                ret = tjDecompress2(handle, Data, (unsigned long)Size, dstBuf, width, pitch, height, pixelFormat, flags);
                (void)ret;

                errorCode = tjGetErrorCode(handle);
                (void)errorCode;

                errorStr1 = tjGetErrorStr();
                (void)errorStr1;

                errorStr2 = tjGetErrorStr2(handle);
                (void)errorStr2;

                errorStr3 = tj3GetErrorStr(handle);
                (void)errorStr3;

                free(dstBuf);
            }
        }
    } else {
        int errorCode = tjGetErrorCode(handle);
        (void)errorCode;

        const char *errorStr1 = tjGetErrorStr();
        (void)errorStr1;

        const char *errorStr2 = tjGetErrorStr2(handle);
        (void)errorStr2;

        const char *errorStr3 = tj3GetErrorStr(handle);
        (void)errorStr3;

        const char *errorStr4 = tj3GetErrorStr(NULL);
        (void)errorStr4;
    }

    tjDestroy(handle);
    cleanup_dummy_file();
    return 0;
}