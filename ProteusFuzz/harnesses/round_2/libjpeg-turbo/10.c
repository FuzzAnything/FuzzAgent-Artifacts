// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjGetErrorCode at turbojpeg.c:652:15 in turbojpeg.h
// tj3GetErrorStr at turbojpeg.c:618:17 in turbojpeg.h
// tjGetErrorCode at turbojpeg.c:652:15 in turbojpeg.h
// tj3GetErrorStr at turbojpeg.c:618:17 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjSaveImage at turbojpeg.c:3125:15 in turbojpeg.h
// tjGetErrorCode at turbojpeg.c:652:15 in turbojpeg.h
// tj3GetErrorStr at turbojpeg.c:618:17 in turbojpeg.h
// tjGetErrorCode at turbojpeg.c:652:15 in turbojpeg.h
// tj3GetErrorStr at turbojpeg.c:618:17 in turbojpeg.h
// tj3SaveImage16 at turbojpeg-mp.c:442:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <turbojpeg.h>

static void cleanup_handle(tjhandle handle) {
    if (handle != NULL) {
        // Note: tjDestroy is the standard cleanup function for tjhandle
        // though not explicitly listed in the prompt, it is required for memory safety.
        // If strictly restricted to listed functions, we cannot free memory,
        // but a fuzzer must avoid leaks. Assuming standard libjpeg-turbo API availability.
        tjDestroy(handle);
    }
}

int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Extract parameters from input data to diversify states
    int initType = (Size > 0) ? (Data[0] % 3) : 0; // TJINIT_COMPRESS=0, TJINIT_DECOMPRESS=1, TJINIT_TRANSFORM=2
    int pixelFormat = (Size > 1) ? (Data[1] % 5) : 0; // TJPF_RGB, etc.
    int flags = (Size > 2) ? (Data[2] % 100) : 0;
    
    // Dimensions (avoid 0 or negative to prevent immediate crashes in some contexts, but fuzzing needs edge cases)
    int width = (Size > 3) ? (Data[3] % 2000) : 1;
    int height = (Size > 4) ? (Data[4] % 2000) : 1;
    
    // Ensure minimum size for buffer operations
    if (width < 1) width = 1;
    if (height < 1) height = 1;

    // Allocate buffers for 8-bit and 16-bit data
    size_t buffer8Size = width * height * 3; // Assume RGB for max size
    size_t buffer16Size = width * height * 3 * 2; // 16-bit RGB

    unsigned char *buf8 = NULL;
    unsigned short *buf16 = NULL;

    if (buffer8Size > 0 && buffer8Size < (1024 * 1024 * 10)) { // Limit allocation
        buf8 = (unsigned char *)malloc(buffer8Size);
        if (buf8) {
            // Fill with fuzz data or pattern
            if (Size >= buffer8Size) {
                memcpy(buf8, Data, buffer8Size);
            } else {
                memset(buf8, 0, buffer8Size);
                if (Size > 0) memcpy(buf8, Data, Size);
            }
        }
    }

    if (buffer16Size > 0 && buffer16Size < (1024 * 1024 * 10)) {
        buf16 = (unsigned short *)malloc(buffer16Size);
        if (buf16) {
            if (Size >= buffer16Size) {
                // Cast carefully for memcpy
                memcpy(buf16, Data, buffer16Size);
            } else {
                memset(buf16, 0, buffer16Size);
                if (Size > 0) memcpy(buf16, Data, Size);
            }
        }
    }

    tjhandle handle = NULL;

    // Test tj3Init
    if (initType == 0) {
        handle = tj3Init(TJINIT_COMPRESS);
    } else if (initType == 1) {
        handle = tj3Init(TJINIT_DECOMPRESS);
    } else {
        handle = tj3Init(TJINIT_TRANSFORM);
    }

    // Test tjInitCompress (legacy)
    tjhandle handleLegacy = tjInitCompress();

    // Test tjSaveImage (8-bit)
    // Use a dummy file path as requested
    if (buf8 != NULL) {
        tjSaveImage("./dummy_file", buf8, width, 0, height, pixelFormat, flags);
        // Check error after potential failure
        tjGetErrorCode(NULL);
        tj3GetErrorStr(NULL);
        if (handleLegacy) {
            tjGetErrorCode(handleLegacy);
            tj3GetErrorStr(handleLegacy);
        }
    }

    // Test tj3SaveImage16 (16-bit)
    if (buf16 != NULL && handle != NULL) {
        tj3SaveImage16(handle, "./dummy_file", buf16, width, 0, height, pixelFormat);
        tjGetErrorCode(handle);
        tj3GetErrorStr(handle);
    }
    
    // Test error retrieval on NULL handle specifically
    tjGetErrorCode(NULL);
    tj3GetErrorStr(NULL);

    // Cleanup
    cleanup_handle(handle);
    cleanup_handle(handleLegacy);
    free(buf8);
    free(buf16);

    return 0;
}