// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tj3GetErrorStr at turbojpeg.c:618:17 in turbojpeg.h
// tjGetErrorCode at turbojpeg.c:652:15 in turbojpeg.h
// tjSaveImage at turbojpeg.c:3125:15 in turbojpeg.h
// tjGetErrorCode at turbojpeg.c:652:15 in turbojpeg.h
// tj3GetErrorStr at turbojpeg.c:618:17 in turbojpeg.h
// tj3SaveImage16 at turbojpeg-mp.c:442:15 in turbojpeg.h
// tjGetErrorCode at turbojpeg.c:652:15 in turbojpeg.h
// tj3GetErrorStr at turbojpeg.c:618:17 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <turbojpeg.h>

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        if (Size > 0 && Data != NULL) {
            fwrite(Data, 1, Size, f);
        }
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Ensure we have enough data for basic parameters
    if (Size < 4) {
        return 0;
    }

    // Parse input to derive parameters
    int width = Data[0] + 1; // 1 to 256
    int height = Data[1] + 1; // 1 to 256
    int pixelFormat = Data[2] % 4; // 0 to 3 (TJPF_GRAY, TJPF_RGB, TJPF_CMYK, TJPF_YUVPacked)
    int flags = Data[3];

    // Limit dimensions to prevent excessive allocation
    if (width > 2048 || height > 2048) {
        width = 2048;
        height = 2048;
    }

    // Calculate buffer sizes based on pixel format
    // For 8-bit: TJPF_GRAY=1, TJPF_RGB=3, TJPF_CMYK=4, TJPF_YUVPacked varies but we use 3 or 4
    int pixelSize8 = 0;
    if (pixelFormat == TJPF_GRAY) pixelSize8 = 1;
    else if (pixelFormat == TJPF_RGB) pixelSize8 = 3;
    else if (pixelFormat == TJPF_CMYK) pixelSize8 = 4;
    else pixelSize8 = 4; // Default for others

    // For 16-bit functions, tj3SaveImage16 expects 2 bytes per sample regardless of format
    // The buffer passed is unsigned short *, so each element is 2 bytes.
    // The pitch parameter is in samples, not bytes.
    int samplesPerPixel = 0;
    if (pixelFormat == TJPF_GRAY) samplesPerPixel = 1;
    else if (pixelFormat == TJPF_RGB) samplesPerPixel = 3;
    else if (pixelFormat == TJPF_CMYK) samplesPerPixel = 4;
    else samplesPerPixel = 4;

    size_t bufSize8 = (size_t)width * height * pixelSize8;
    size_t bufSize16 = (size_t)width * height * samplesPerPixel;

    // Allocate buffers
    unsigned char *buffer8 = NULL;
    unsigned short *buffer16 = NULL;

    // Allocate and initialize 8-bit buffer
    buffer8 = (unsigned char *)malloc(bufSize8);
    if (!buffer8) return 0;
    if (Size > bufSize8) {
        memcpy(buffer8, Data, bufSize8);
    } else {
        // Fill with pattern if input is too small
        for (size_t i = 0; i < bufSize8; i++) {
            buffer8[i] = (unsigned char)(Data[i % Size] ^ 0xAA);
        }
    }

    // Allocate and initialize 16-bit buffer
    buffer16 = (unsigned short *)malloc(bufSize16 * sizeof(unsigned short));
    if (!buffer16) {
        free(buffer8);
        return 0;
    }
    
    // Fill 16-bit buffer with valid data
    for (size_t i = 0; i < bufSize16; i++) {
        // Use input data to generate 16-bit values, ensuring they are within 13-16 bit range if possible
        // or just valid 16-bit values.
        uint16_t val = 0;
        if (Size > 0) {
            val = (uint16_t)Data[i % Size];
            if (Size > 1) {
                val |= ((uint16_t)Data[(i + 1) % Size]) << 8;
            }
        }
        // Ensure value is not 0 if we want to test non-black, but 0 is valid.
        // Let's just use the value.
        buffer16[i] = val;
    }

    // Initialize TurboJPEG handle
    tjhandle handle = tjInitCompress();
    if (handle == NULL) {
        tj3GetErrorStr(NULL);
        tjGetErrorCode(NULL);
        free(buffer8);
        free(buffer16);
        return 0;
    }

    // Test tjSaveImage (8-bit)
    if (buffer8) {
        int pitch8 = width * pixelSize8;
        write_dummy_file(Data, Size);
        tjSaveImage("./dummy_file", buffer8, width, pitch8, height, pixelFormat, flags);
        if (tjGetErrorCode(handle) != 0) {
            tj3GetErrorStr(handle);
        }
    }

    // Test tj3SaveImage16 (16-bit)
    if (buffer16) {
        // Pitch is in samples, not bytes.
        int pitch16 = width * samplesPerPixel;
        write_dummy_file(Data, Size);
        tj3SaveImage16(handle, "./dummy_file", buffer16, width, pitch16, height, pixelFormat);
        if (tjGetErrorCode(handle) != 0) {
            tj3GetErrorStr(handle);
        }
    }

    // Cleanup
    tjDestroy(handle);
    free(buffer8);
    free(buffer16);

    return 0;
}