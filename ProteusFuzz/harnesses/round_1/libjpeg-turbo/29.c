// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3GetErrorStr at turbojpeg.c:618:17 in turbojpeg.h
// tjGetErrorStr2 at turbojpeg.c:630:17 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3SaveImage16 at turbojpeg-mp.c:442:15 in turbojpeg.h
// tj3GetErrorStr at turbojpeg.c:618:17 in turbojpeg.h
// tjGetErrorStr2 at turbojpeg.c:630:17 in turbojpeg.h
// tjGetErrorStr at turbojpeg.c:636:17 in turbojpeg.h
// tj3LoadImage16 at turbojpeg-mp.c:293:21 in turbojpeg.h
// tj3SaveImage16 at turbojpeg-mp.c:442:15 in turbojpeg.h
// tj3GetErrorStr at turbojpeg.c:618:17 in turbojpeg.h
// tjGetErrorStr2 at turbojpeg.c:630:17 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3GetErrorStr at turbojpeg.c:618:17 in turbojpeg.h
// tjGetErrorStr2 at turbojpeg.c:630:17 in turbojpeg.h
// tjGetErrorStr at turbojpeg.c:636:17 in turbojpeg.h
// tj3GetErrorStr at turbojpeg.c:618:17 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <turbojpeg.h>

int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
    if (Size < 5) return 0;

    int width = Data[0];
    int height = Data[1];
    int pixelFormatIdx = Data[2] % 4;
    int initType = Data[3] % 2;
    int alignShift = Data[4] % 8;

    int pf = TJPF_RGB;
    if (pixelFormatIdx == 1) pf = TJPF_GRAY;
    else if (pixelFormatIdx == 2) pf = TJPF_CMYK;
    else if (pixelFormatIdx == 3) pf = TJPF_XBGR;

    if (width == 0) width = 1;
    if (height == 0) height = 1;
    if (width > 8192) width = 8192;
    if (height > 8192) height = 8192;

    tjhandle handle = tj3Init(initType);
    if (!handle) {
        tj3GetErrorStr(handle);
        tjGetErrorStr2(handle);
        return 0;
    }

    size_t channels = 3;
    if (pf == TJPF_GRAY) channels = 1;
    else if (pf == TJPF_CMYK) channels = 4;
    else if (pf == TJPF_XBGR) channels = 4;

    size_t required_size = (size_t)width * height * channels;
    unsigned short *buffer = (unsigned short *)malloc(required_size * sizeof(unsigned short));
    if (!buffer) {
        tj3Destroy(handle);
        return 0;
    }

    if (Size >= required_size * 2) {
        for (size_t i = 0; i < required_size; i++) {
            size_t idx = i * 2;
            if (idx + 1 < Size) {
                buffer[i] = (unsigned short)Data[idx] | ((unsigned short)Data[idx + 1] << 8);
            } else {
                buffer[i] = 0;
            }
        }
    } else {
        memset(buffer, 0, required_size * sizeof(unsigned short));
    }

    int pitch = 0;
    int ret = tj3SaveImage16(handle, "./dummy_file", buffer, width, pitch, height, pf);
    if (ret != 0) {
        tj3GetErrorStr(handle);
        tjGetErrorStr2(handle);
        tjGetErrorStr();
    }
    free(buffer);

    int w = 0, h = 0, pf_out = TJPF_UNKNOWN;
    int align = 1 << alignShift;

    unsigned short *loaded = tj3LoadImage16(handle, "./dummy_file", &w, align, &h, &pf_out);
    if (loaded) {
        if (w > 0 && h > 0 && pf_out != TJPF_UNKNOWN) {
            int ret2 = tj3SaveImage16(handle, "./dummy_file2", loaded, w, 0, h, pf_out);
            if (ret2 != 0) {
                tj3GetErrorStr(handle);
                tjGetErrorStr2(handle);
            }
        }
        tj3Free(loaded);
    } else {
        tj3GetErrorStr(handle);
        tjGetErrorStr2(handle);
        tjGetErrorStr();
    }

    tj3GetErrorStr(NULL);

    tj3Destroy(handle);

    return 0;
}