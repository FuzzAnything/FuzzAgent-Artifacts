// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjGetErrorCode at turbojpeg.c:652:15 in turbojpeg.h
// tjGetErrorStr at turbojpeg.c:636:17 in turbojpeg.h
// tj3GetErrorStr at turbojpeg.c:618:17 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjGetErrorStr at turbojpeg.c:636:17 in turbojpeg.h
// tjGetErrorCode at turbojpeg.c:652:15 in turbojpeg.h
// tjSaveImage at turbojpeg.c:3125:15 in turbojpeg.h
// tjGetErrorStr at turbojpeg.c:636:17 in turbojpeg.h
// tjGetErrorCode at turbojpeg.c:652:15 in turbojpeg.h
// tj3SaveImage16 at turbojpeg-mp.c:442:15 in turbojpeg.h
// tj3GetErrorStr at turbojpeg.c:618:17 in turbojpeg.h
// tjGetErrorStr2 at turbojpeg.c:630:17 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <turbojpeg.h>

static void cleanup_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Initialize TurboJPEG instance
    tjhandle handle = tjInitCompress();
    
    // If handle creation fails, we can still test global error functions
    if (!handle) {
        char *err_str = tjGetErrorStr();
        (void)err_str; // Consume result
        int err_code = tjGetErrorCode(NULL);
        (void)err_code; // Consume result
        return 0;
    }

    // Ensure cleanup happens
    atexit(cleanup_dummy_file);

    // Extract parameters from fuzzed data
    int width = (Size >= 2) ? (Data[0] * 256 + Data[1]) : 0;
    int height = (Size >= 4) ? (Data[2] * 256 + Data[3]) : 0;
    int pixelFormat = (Size >= 5) ? (Data[4] % 5) : TJPF_RGB; // 0-4 range for safety
    int precision = (Size >= 6) ? (Data[5] % 4) + 13 : 16; // 13-16 range

    // Clamp dimensions to reasonable limits to avoid huge allocations or crashes
    if (width > 4096) width = 4096;
    if (height > 4096) height = 4096;
    if (width <= 0) width = 1;
    if (height <= 0) height = 1;

    // Calculate pitch
    int pixelSize = tjPixelSize[pixelFormat];
    int pitch = width * pixelSize;

    // Allocate buffers
    size_t bufferSize8 = (size_t)height * pitch;
    size_t bufferSize16 = (size_t)height * pitch * 2; // 16-bit samples

    unsigned char *buffer8 = NULL;
    unsigned short *buffer16 = NULL;

    if (bufferSize8 > 0 && bufferSize8 < (1024 * 1024 * 100)) { // Limit 100MB
        buffer8 = (unsigned char *)malloc(bufferSize8);
        if (buffer8) {
            memset(buffer8, 0, bufferSize8);
            if (Size > 0) {
                size_t copyLen = (Size < bufferSize8) ? Size : bufferSize8;
                memcpy(buffer8, Data, copyLen);
            }
        }
    }

    if (bufferSize16 > 0 && bufferSize16 < (1024 * 1024 * 100)) { // Limit 100MB
        buffer16 = (unsigned short *)malloc(bufferSize16);
        if (buffer16) {
            memset(buffer16, 0, bufferSize16);
            if (Size > 0) {
                size_t copyLen = (Size < bufferSize16) ? Size : bufferSize16;
                // Fill with pattern based on input
                for (size_t i = 0; i < copyLen; i += 2) {
                    if (i + 1 < copyLen) {
                        buffer16[i / 2] = (unsigned short)(Data[i] << 8 | Data[i+1]);
                    } else {
                        buffer16[i / 2] = (unsigned short)(Data[i] << 8);
                    }
                }
            }
        }
    }

    // Test tjSaveImage
    if (buffer8) {
        tjSaveImage("./dummy_file", buffer8, width, pitch, height, pixelFormat, 0);
        char *err = tjGetErrorStr();
        (void)err;
        int code = tjGetErrorCode(handle);
        (void)code;
    }

    // Test tj3SaveImage16
    if (buffer16) {
        tj3SaveImage16(handle, "./dummy_file", buffer16, width, pitch, height, pixelFormat);
        char *err = tj3GetErrorStr(handle);
        (void)err;
        char *err2 = tjGetErrorStr2(handle);
        (void)err2;
        int code = tjGetErrorCode(handle);
        (void)code;
    }

    // Test global error retrieval
    char *globalErr = tjGetErrorStr();
    (void)globalErr;
    char *globalErr3 = tj3GetErrorStr(NULL);
    (void)globalErr3;

    // Cleanup
    if (buffer8) free(buffer8);
    if (buffer16) free(buffer16);
    
    if (handle) {
        tjDestroy(handle);
    }

    return 0;
}