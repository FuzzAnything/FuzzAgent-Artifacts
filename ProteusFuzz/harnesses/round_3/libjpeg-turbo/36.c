// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3LoadImage16 at turbojpeg-mp.c:293:21 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjLoadImage at turbojpeg.c:3104:26 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tj3SaveImage16 at turbojpeg-mp.c:442:15 in turbojpeg.h
// tjGetErrorCode at turbojpeg.c:652:15 in turbojpeg.h
// tjGetErrorStr2 at turbojpeg.c:630:17 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjSaveImage at turbojpeg.c:3125:15 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3SaveImage16 at turbojpeg-mp.c:442:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <turbojpeg.h>

static int fuzzer_test_tjSaveImage(const uint8_t *Data, size_t Size) {
    if (Size < 12) return 0;
    
    // Ensure alignment for int reading
    if ((size_t)Data % sizeof(int) != 0) return 0;

    int width = *(const int*)(Data);
    int height = *(const int*)(Data + 4);
    int pixelFormat = *(const int*)(Data + 8);
    
    if (width <= 0 || height <= 0 || width > 10000 || height > 10000) return 0;
    if (pixelFormat < 0 || pixelFormat > 10) return 0;
    
    // Calculate pitch based on pixel format to ensure buffer size calculation is safe
    int bytesPerPixel = 3; // Default for RGB
    if (pixelFormat == TJPF_GRAY) bytesPerPixel = 1;
    else if (pixelFormat == TJPF_CMYK) bytesPerPixel = 4;
    else if (pixelFormat == TJPF_XRGB || pixelFormat == TJPF_RGBX) bytesPerPixel = 4;
    
    int pitch = width * bytesPerPixel;
    size_t bufferSize = (size_t)height * pitch;
    
    // Ensure we have enough data for the buffer
    if (bufferSize > Size - 12) return 0;
    
    const unsigned char *buffer = Data + 12;
    
    tjSaveImage("./dummy_file", (unsigned char *)buffer, width, pitch, height, pixelFormat, 0);
    
    return 0;
}

static int fuzzer_test_tj3SaveImage16(const uint8_t *Data, size_t Size) {
    if (Size < 12) return 0;
    
    tjhandle handle = tjInitCompress();
    if (!handle) return 0;
    
    // Ensure alignment for int reading
    if ((size_t)Data % sizeof(int) != 0) {
        tjDestroy(handle);
        return 0;
    }

    int width = *(const int*)(Data);
    int height = *(const int*)(Data + 4);
    int pixelFormat = *(const int*)(Data + 8);
    
    if (width <= 0 || height <= 0 || width > 10000 || height > 10000) {
        tjDestroy(handle);
        return 0;
    }
    if (pixelFormat < 0 || pixelFormat > 10) {
        tjDestroy(handle);
        return 0;
    }
    
    // 16-bit images use 2 bytes per sample
    int pitch = width * 2; 
    size_t bufferSize = (size_t)height * pitch;
    
    if (bufferSize > Size - 12) {
        tjDestroy(handle);
        return 0;
    }
    
    // Ensure buffer alignment for unsigned short
    const unsigned short *buffer = (const unsigned short *)(Data + 12);
    
    tj3SaveImage16(handle, "./dummy_file", buffer, width, pitch, height, pixelFormat);
    
    tjDestroy(handle);
    return 0;
}

static int fuzzer_test_tj3LoadImage16(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    tjhandle handle = tjInitDecompress();
    if (!handle) return 0;
    
    int width = 0;
    int height = 0;
    int pixelFormat = 0;
    
    if ((size_t)Data % sizeof(int) == 0) {
        pixelFormat = *(const int*)(Data);
    }
    
    // Clamp pixelFormat to safe range
    if (pixelFormat < 0 || pixelFormat > 10) pixelFormat = TJPF_UNKNOWN;
    
    int align = 1;
    
    unsigned short *buffer = tj3LoadImage16(handle, "./dummy_file", &width, align, &height, &pixelFormat);
    
    if (buffer) {
        tj3Free(buffer);
    }
    
    tjDestroy(handle);
    return 0;
}

static int fuzzer_test_tjLoadImage(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    int width = 0;
    int height = 0;
    int pixelFormat = 0;
    int align = 1;
    int flags = 0;
    
    if ((size_t)Data % sizeof(int) == 0) {
        pixelFormat = *(const int*)(Data);
    }
    
    // Clamp pixelFormat to safe range
    if (pixelFormat < 0 || pixelFormat > 10) pixelFormat = TJPF_UNKNOWN;
    
    unsigned char *buffer = tjLoadImage("./dummy_file", &width, align, &height, &pixelFormat, flags);
    
    if (buffer) {
        free(buffer);
    }
    
    return 0;
}

static int fuzzer_test_error_handling(const uint8_t *Data, size_t Size) {
    tjhandle handle = tjInitCompress();
    if (!handle) return 0;
    
    // Trigger an error by passing invalid arguments
    tj3SaveImage16(handle, "./dummy_file", NULL, -1, 0, -1, -1);
    
    int errorCode = tjGetErrorCode(handle);
    char *errorStr = tjGetErrorStr2(handle);
    
    if (errorStr) {
        size_t len = strlen(errorStr);
        (void)len; // Use length to avoid warning
    }
    
    tjDestroy(handle);
    return 0;
}

int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Use first byte to select which function to fuzz
    int choice = Data[0] % 5;
    
    switch (choice) {
        case 0:
            fuzzer_test_tjSaveImage(Data, Size);
            break;
        case 1:
            fuzzer_test_tj3SaveImage16(Data, Size);
            break;
        case 2:
            fuzzer_test_tj3LoadImage16(Data, Size);
            break;
        case 3:
            fuzzer_test_tjLoadImage(Data, Size);
            break;
        case 4:
            fuzzer_test_error_handling(Data, Size);
            break;
        default:
            break;
    }
    
    return 0;
}