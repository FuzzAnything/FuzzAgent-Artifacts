// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3SaveImage16 at turbojpeg-mp.c:442:15 in turbojpeg.h
// tj3GetErrorStr at turbojpeg.c:618:17 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3GetErrorStr at turbojpeg.c:618:17 in turbojpeg.h
// tj3SaveImage16 at turbojpeg-mp.c:442:15 in turbojpeg.h
// tj3GetErrorStr at turbojpeg.c:618:17 in turbojpeg.h
// tj3SaveImage16 at turbojpeg-mp.c:442:15 in turbojpeg.h
// tj3GetErrorStr at turbojpeg.c:618:17 in turbojpeg.h
// tj3LoadImage16 at turbojpeg-mp.c:293:21 in turbojpeg.h
// tj3GetErrorStr at turbojpeg.c:618:17 in turbojpeg.h
// tjGetErrorStr at turbojpeg.c:636:17 in turbojpeg.h
// tjGetErrorStr2 at turbojpeg.c:630:17 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3GetErrorStr at turbojpeg.c:618:17 in turbojpeg.h
// tj3LoadImage16 at turbojpeg-mp.c:293:21 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "turbojpeg.h"

static void write_dummy_ppm(const char *filename, int width, int height, int pixelFormat) {
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return;

    int samplesPerPixel = 1;
    if (pixelFormat == TJPF_RGB || pixelFormat == TJPF_CMYK) samplesPerPixel = 3;
    else if (pixelFormat == TJPF_GRAY) samplesPerPixel = 1;
    else samplesPerPixel = 3; // Default to 3 for unknown or other formats

    const char *header = (pixelFormat == TJPF_GRAY) ? "P5" : "P6";
    char buf[256];
    int len = snprintf(buf, sizeof(buf), "%s %d %d 65535\n", header, width, height);
    write(fd, buf, len);

    size_t totalSamples = width * height * samplesPerPixel;
    size_t dataSize = totalSamples * sizeof(unsigned short);
    
    unsigned short *data = malloc(dataSize);
    if (data) {
        memset(data, 0, dataSize);
        for (size_t i = 0; i < dataSize / sizeof(unsigned short); i++) {
            data[i] = (unsigned short)(i % 65536);
        }
        write(fd, data, dataSize);
        free(data);
    }
    
    close(fd);
}

static void write_dummy_pgm(const char *filename, int width, int height) {
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return;

    char buf[256];
    int len = snprintf(buf, sizeof(buf), "P5 %d %d 65535\n", width, height);
    write(fd, buf, len);

    size_t totalSamples = width * height;
    size_t dataSize = totalSamples * sizeof(unsigned short);
    unsigned short *data = malloc(dataSize);
    if (data) {
        memset(data, 0, dataSize);
        for (size_t i = 0; i < dataSize / sizeof(unsigned short); i++) {
            data[i] = (unsigned short)(i % 65536);
        }
        write(fd, data, dataSize);
        free(data);
    }
    close(fd);
}

int LLVMFuzzerTestOneInput_37(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    int width = Data[0] + 1;
    int height = Data[1] + 1;
    int pixelFormat = Data[2] % 10; 
    int align = Data[3] & 0x0F; 
    int initType = Data[4] & 0x03; 
    
    if (align == 0) align = 1;
    else if (align == 1) align = 1;
    else if (align == 2) align = 2;
    else if (align == 3) align = 4;
    else if (align == 4) align = 8;
    else if (align == 5) align = 16;
    else align = 1;

    int validPixelFormats[] = {TJPF_GRAY, TJPF_RGB, TJPF_CMYK, TJPF_UNKNOWN};
    int numFormats = sizeof(validPixelFormats) / sizeof(validPixelFormats[0]);
    int actualPixelFormat = validPixelFormats[pixelFormat % numFormats];

    tjhandle handle = tj3Init(initType);
    if (!handle) {
        tj3GetErrorStr(handle);
        return 0;
    }

    const char *filename = "./dummy_file";
    if (actualPixelFormat == TJPF_GRAY) {
        write_dummy_pgm(filename, width, height);
    } else {
        write_dummy_ppm(filename, width, height, actualPixelFormat);
    }

    int loadWidth, loadHeight, loadPixelFormat = TJPF_UNKNOWN;
    unsigned short *loadedBuffer = tj3LoadImage16(handle, filename, &loadWidth, align, &loadHeight, &loadPixelFormat);
    
    if (loadedBuffer) {
        int saveWidth = loadWidth;
        int saveHeight = loadHeight;
        int savePitch = 0;
        int savePixelFormat = loadPixelFormat;
        
        if (savePixelFormat == TJPF_UNKNOWN) savePixelFormat = TJPF_RGB;

        int saveResult = tj3SaveImage16(handle, filename, loadedBuffer, saveWidth, savePitch, saveHeight, savePixelFormat);
        if (saveResult != 0) {
            tj3GetErrorStr(handle);
        }
        
        tj3Free(loadedBuffer);
    } else {
        tj3GetErrorStr(handle);
    }

    int synthWidth = width;
    int synthHeight = height;
    int synthPitch = 0;
    int synthPixelFormat = actualPixelFormat;
    
    int samplesPerPixel = 1;
    if (synthPixelFormat == TJPF_RGB || synthPixelFormat == TJPF_CMYK) {
        samplesPerPixel = 3;
    }
    
    size_t synthBufferSize = (size_t)synthWidth * synthHeight * samplesPerPixel * sizeof(unsigned short);
    unsigned short *synthBuffer = malloc(synthBufferSize);
    
    if (synthBuffer) {
        if (Size > 10) {
            size_t dataLen = Size - 10;
            if (dataLen > synthBufferSize) dataLen = synthBufferSize;
            memcpy(synthBuffer, Data + 10, dataLen);
            if (dataLen < synthBufferSize) {
                memset((char*)synthBuffer + dataLen, 0, synthBufferSize - dataLen);
            }
        } else {
            memset(synthBuffer, 0, synthBufferSize);
        }

        int saveResult = tj3SaveImage16(handle, filename, synthBuffer, synthWidth, synthPitch, synthHeight, synthPixelFormat);
        if (saveResult != 0) {
            tj3GetErrorStr(handle);
        }
        
        free(synthBuffer);
    }

    int errResult = tj3SaveImage16(handle, filename, NULL, synthWidth, synthPitch, synthHeight, synthPixelFormat);
    if (errResult != 0) {
        tj3GetErrorStr(handle);
    }
    
    unsigned short *nullLoad = tj3LoadImage16(handle, filename, NULL, align, NULL, NULL);
    if (!nullLoad) {
        tj3GetErrorStr(handle);
    }

    char *globalErr = tjGetErrorStr();
    char *handleErr = tjGetErrorStr2(handle);
    (void)globalErr;
    (void)handleErr;

    tj3Free(handle);
    unlink(filename);

    return 0;
}