// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjGetErrorStr2 at turbojpeg.c:630:17 in turbojpeg.h
// tjGetErrorCode at turbojpeg.c:652:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tj3GetErrorStr at turbojpeg.c:618:17 in turbojpeg.h
// tjGetErrorStr at turbojpeg.c:636:17 in turbojpeg.h
// tjGetErrorStr2 at turbojpeg.c:630:17 in turbojpeg.h
// tjGetErrorCode at turbojpeg.c:652:15 in turbojpeg.h
// tjSaveImage at turbojpeg.c:3125:15 in turbojpeg.h
// tjGetErrorStr at turbojpeg.c:636:17 in turbojpeg.h
// tjGetErrorCode at turbojpeg.c:652:15 in turbojpeg.h
// tj3SaveImage16 at turbojpeg-mp.c:442:15 in turbojpeg.h
// tj3GetErrorStr at turbojpeg.c:618:17 in turbojpeg.h
// tjGetErrorCode at turbojpeg.c:652:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <turbojpeg.h>

int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    // Extract parameters from input data
    int width = Data[0];
    int height = Data[1];
    int pixelFormat = Data[2];
    int flags = Data[3];

    // Sanity checks to avoid crashes with extreme values
    if (width <= 0 || height <= 0 || width > 4096 || height > 4096) {
        width = 64;
        height = 64;
    }

    // Clamp pixelFormat to valid range [0, 5]
    if (pixelFormat < 0 || pixelFormat > 5) pixelFormat = 0;

    // Allocate buffers for image data
    int pitch = width * tjPixelSize[pixelFormat];
    size_t bufSize = (size_t)height * pitch;
    
    // Limit buffer size to prevent OOM
    if (bufSize > 1024 * 1024) bufSize = 1024 * 1024;

    unsigned char *buffer8 = (unsigned char *)malloc(bufSize);
    unsigned short *buffer16 = (unsigned short *)malloc(bufSize * 2);

    if (!buffer8 || !buffer16) {
        free(buffer8);
        free(buffer16);
        return 0;
    }

    // Initialize buffers with fuzz data if available, otherwise zeros
    if (Size > 4) {
        size_t dataLen = Size - 4;
        size_t copyLen8 = (dataLen < bufSize) ? dataLen : bufSize;
        size_t copyLen16 = (dataLen < bufSize * 2) ? dataLen : bufSize * 2;
        memcpy(buffer8, Data + 4, copyLen8);
        // Create 16-bit data by duplicating bytes or using raw input
        for (size_t i = 0; i < copyLen16 / 2; i++) {
            buffer16[i] = (unsigned short)(Data[4 + (i % dataLen)] << 8) | (Data[4 + ((i + 1) % dataLen)]);
        }
    } else {
        memset(buffer8, 0, bufSize);
        memset(buffer16, 0, bufSize * 2);
    }

    // Create a TurboJPEG instance
    tjhandle handle = tjInitCompress();

    // Test tj3GetErrorStr with NULL (global error)
    char *errStr = tj3GetErrorStr(NULL);
    (void)errStr;

    // Test tjGetErrorStr (legacy global)
    char *errStrLegacy = tjGetErrorStr();
    (void)errStrLegacy;

    // Test tjGetErrorStr2 with valid handle
    if (handle) {
        char *errStr2 = tjGetErrorStr2(handle);
        (void)errStr2;
        
        // Test tjGetErrorCode with valid handle
        int errorCode = tjGetErrorCode(handle);
        (void)errorCode;
    }

    // Test tjSaveImage
    // We write to a dummy file. If it fails, we check error strings.
    int retSave = tjSaveImage("./dummy_file", buffer8, width, pitch, height, pixelFormat, flags);
    if (retSave != 0) {
        char *err = tjGetErrorStr();
        (void)err;
        if (handle) {
            int code = tjGetErrorCode(handle);
            (void)code;
        }
    }

    // Test tj3SaveImage16
    // Only if handle is valid
    if (handle) {
        int retSave16 = tj3SaveImage16(handle, "./dummy_file", buffer16, width, 0, height, pixelFormat);
        if (retSave16 != 0) {
            char *err = tj3GetErrorStr(handle);
            (void)err;
            int code = tjGetErrorCode(handle);
            (void)code;
        }
    }

    // Test error retrieval with invalid handle (NULL) for instance-specific functions
    // tjGetErrorStr2(NULL) is valid for global errors
    char *errNull = tjGetErrorStr2(NULL);
    (void)errNull;

    // Test tjGetErrorCode with NULL (might return generic or specific depending on impl)
    int codeNull = tjGetErrorCode(NULL);
    (void)codeNull;

    // Cleanup
    if (handle) {
        tjDestroy(handle);
    }
    free(buffer8);
    free(buffer16);

    // Clean up dummy file
    remove("./dummy_file");

    return 0;
}