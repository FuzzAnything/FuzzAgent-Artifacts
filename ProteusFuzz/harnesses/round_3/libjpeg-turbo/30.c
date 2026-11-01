// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3LoadImage16 at turbojpeg-mp.c:293:21 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3GetErrorStr at turbojpeg.c:618:17 in turbojpeg.h
// tj3SaveImage16 at turbojpeg-mp.c:442:15 in turbojpeg.h
// tjGetErrorStr at turbojpeg.c:636:17 in turbojpeg.h
// tjGetErrorCode at turbojpeg.c:652:15 in turbojpeg.h
// tjGetErrorStr2 at turbojpeg.c:630:17 in turbojpeg.h
// tj3GetErrorStr at turbojpeg.c:618:17 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3SaveImage16 at turbojpeg-mp.c:442:15 in turbojpeg.h
// tj3GetErrorStr at turbojpeg.c:618:17 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <turbojpeg.h>

static void write_dummy_ppm_file(const char *filename, int width, int height, const unsigned short *data, int pixelFormat) {
    if (width <= 0 || height <= 0 || !data) return;
    
    FILE *f = fopen(filename, "wb");
    if (!f) return;

    int channels = 1;
    if (pixelFormat == TJPF_GRAY) channels = 1;
    else if (pixelFormat == TJPF_RGB || pixelFormat == TJPF_BGR || pixelFormat == TJPF_CMYK) channels = 3;
    else if (pixelFormat == TJPF_RGBA || pixelFormat == TJPF_BGRA || pixelFormat == TJPF_ABGR) channels = 4;
    else channels = 3;

    if (channels == 1) {
        fprintf(f, "P5\n%d %d\n65535\n", width, height);
    } else {
        fprintf(f, "P6\n%d %d\n65535\n", width, height);
    }

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            for (int c = 0; c < channels; c++) {
                int idx = (y * width + x) * channels + c;
                if (idx >= 0 && idx < (width * height * channels)) {
                    unsigned short val = data[idx];
                    fputc((val >> 8) & 0xFF, f);
                    fputc(val & 0xFF, f);
                }
            }
        }
    }
    fclose(f);
}

int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    tjhandle handle = tjInitCompress();
    if (!handle) return 0;

    int width = (Data[0] % 100) + 1;
    int height = (Data[1] % 100) + 1;
    int pixelFormatIdx = Data[2] % 6;
    int align = 1 << (Data[3] % 5);

    int validFormats[] = {TJPF_GRAY, TJPF_RGB, TJPF_BGR, TJPF_CMYK, TJPF_RGBA, TJPF_BGRA};
    int fmt = validFormats[pixelFormatIdx];

    int channels = (fmt == TJPF_GRAY) ? 1 : (fmt == TJPF_CMYK ? 4 : 3);
    if (fmt == TJPF_RGBA || fmt == TJPF_BGRA || fmt == TJPF_ABGR) channels = 4;
    
    // Prevent integer overflow
    if (width > 10000 || height > 10000) {
        tjDestroy(handle);
        return 0;
    }

    size_t rowSize = (size_t)width * channels;
    if (rowSize > 10000000) {
        tjDestroy(handle);
        return 0;
    }
    
    size_t bufferSize = rowSize * (size_t)height;
    if (bufferSize > 100000000) {
        tjDestroy(handle);
        return 0;
    }

    unsigned short *buffer = (unsigned short *)malloc(bufferSize);
    if (!buffer) {
        tjDestroy(handle);
        return 0;
    }

    // Initialize buffer with fuzz data safely
    size_t dataUsed = (Size > bufferSize) ? bufferSize : Size;
    for (size_t i = 0; i < dataUsed; i++) {
        buffer[i] = Data[i] * 256 + Data[(i + 1) % Size];
    }
    
    // Fill rest with 0 safely
    for (size_t i = dataUsed; i < bufferSize; i++) {
        buffer[i] = 0;
    }

    const char *filename = "./dummy_file";

    // Test 1: tj3SaveImage16
    int pitch = 0; 
    int ret = tj3SaveImage16(handle, filename, buffer, width, pitch, height, fmt);
    if (ret != 0) {
        char *err = tj3GetErrorStr(handle);
        if (err) (void)strlen(err);
    }

    // Test 2: tj3LoadImage16
    int loadWidth = 0, loadHeight = 0, loadFmt = TJPF_UNKNOWN;
    unsigned short *loadedBuffer = tj3LoadImage16(handle, filename, &loadWidth, align, &loadHeight, &loadFmt);
    
    if (loadedBuffer) {
        if (loadWidth > 0 && loadHeight > 0 && loadFmt != TJPF_UNKNOWN) {
            int loadChannels = (loadFmt == TJPF_GRAY) ? 1 : 3;
            if (loadFmt == TJPF_CMYK) loadChannels = 4;
            if (loadFmt == TJPF_RGBA || loadFmt == TJPF_BGRA || loadFmt == TJPF_ABGR) loadChannels = 4;
            
            size_t loadedSize = (size_t)loadWidth * (size_t)loadHeight * loadChannels;
            if (loadedSize > 0 && loadedSize < 100000000) {
                loadedBuffer[0] = 0xFFFF;
            }
        }
        tj3Free(loadedBuffer);
    } else {
        char *err = tj3GetErrorStr(handle);
        if (err) (void)strlen(err);
    }

    // Test 3: Error handling functions
    tj3SaveImage16(handle, NULL, buffer, -1, 0, -1, -1);
    char *errStr = tjGetErrorStr();
    if (errStr) (void)strlen(errStr);
    
    int errCode = tjGetErrorCode(handle);
    (void)errCode;

    char *errStr2 = tjGetErrorStr2(handle);
    if (errStr2) (void)strlen(errStr2);

    char *errStr3 = tj3GetErrorStr(handle);
    if (errStr3) (void)strlen(errStr3);

    // Cleanup
    free(buffer);
    tjDestroy(handle);
    return 0;
}