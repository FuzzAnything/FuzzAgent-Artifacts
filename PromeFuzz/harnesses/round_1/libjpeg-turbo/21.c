// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3LoadImage16 at turbojpeg-mp.c:293:21 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Compress16 at turbojpeg-mp.c:71:15 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Compress16 at turbojpeg-mp.c:71:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Compress16 at turbojpeg-mp.c:71:15 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Decompress16 at turbojpeg-mp.c:148:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3SaveImage16 at turbojpeg-mp.c:442:15 in turbojpeg.h
// tj3LoadImage16 at turbojpeg-mp.c:293:21 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "turbojpeg.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static void remove_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    tjhandle handle = NULL;
    unsigned char *jpegBuf = NULL;
    size_t jpegSize = 0;
    unsigned short *imageBuf = NULL;
    int width, height, pixelFormat;
    int ret = 0;
    
    /* Initialize handle for both compression and decompression */
    handle = tj3Init(TJINIT_COMPRESS | TJINIT_DECOMPRESS);
    if (!handle) return 0;
    
    /* Use first byte to determine pixel format (mod 13 for valid formats) */
    int pixelFormatChoice = Data[0] % 13;
    /* Valid pixel formats: TJPF_RGB(0) through TJPF_CMYK(12) */
    pixelFormat = pixelFormatChoice;
    
    /* Use next bytes to determine dimensions (ensure reasonable values) */
    width = (Size > 1) ? (Data[1] % 64 + 1) : 16;  /* 1-64 pixels */
    height = (Size > 2) ? (Data[2] % 64 + 1) : 16; /* 1-64 pixels */
    
    /* Calculate required buffer size for 16-bit samples */
    int samplesPerPixel = 0;
    switch (pixelFormat) {
        case TJPF_RGB: samplesPerPixel = 3; break;
        case TJPF_BGR: samplesPerPixel = 3; break;
        case TJPF_RGBX: samplesPerPixel = 4; break;
        case TJPF_BGRX: samplesPerPixel = 4; break;
        case TJPF_XBGR: samplesPerPixel = 4; break;
        case TJPF_XRGB: samplesPerPixel = 4; break;
        case TJPF_GRAY: samplesPerPixel = 1; break;
        case TJPF_RGBA: samplesPerPixel = 4; break;
        case TJPF_BGRA: samplesPerPixel = 4; break;
        case TJPF_ABGR: samplesPerPixel = 4; break;
        case TJPF_ARGB: samplesPerPixel = 4; break;
        case TJPF_CMYK: samplesPerPixel = 4; break;
        default: samplesPerPixel = 3; break;
    }
    
    size_t imageBufSize = width * height * samplesPerPixel;
    
    /* Allocate and initialize image buffer with fuzzer data */
    imageBuf = (unsigned short *)tj3Alloc(imageBufSize * sizeof(unsigned short));
    if (!imageBuf) goto cleanup;
    
    /* Fill buffer with fuzzer data, repeating if needed */
    for (size_t i = 0; i < imageBufSize; i++) {
        imageBuf[i] = (i < Size - 3) ? 
            ((unsigned short)Data[3 + i] << 8) | Data[3 + (i + 1) % (Size - 3)] : 
            0x8000; /* Middle value for 16-bit */
    }
    
    /* Test 1: Compress image */
    jpegBuf = NULL;
    jpegSize = 0;
    ret = tj3Compress16(handle, imageBuf, width, 0, height, pixelFormat, 
                       &jpegBuf, &jpegSize);
    /* Don't check ret - we want to test error paths too */
    
    /* Test 2: Decompress if compression succeeded and we have JPEG data */
    if (jpegBuf && jpegSize > 0) {
        unsigned short *decompBuf = (unsigned short *)tj3Alloc(imageBufSize * sizeof(unsigned short));
        if (decompBuf) {
            tj3Decompress16(handle, jpegBuf, jpegSize, decompBuf, 0, pixelFormat);
            tj3Free(decompBuf);
        }
    }
    
    /* Test 3: Save image to file */
    write_dummy_file(Data, Size);
    tj3SaveImage16(handle, "./dummy_file", imageBuf, width, 0, height, pixelFormat);
    
    /* Test 4: Load image from file */
    int loadedWidth = 0, loadedHeight = 0, loadedPixelFormat = pixelFormat;
    unsigned short *loadedBuf = tj3LoadImage16(handle, "./dummy_file", 
                                              &loadedWidth, 1, &loadedHeight, 
                                              &loadedPixelFormat);
    if (loadedBuf) {
        tj3Free(loadedBuf);
    }
    
    /* Test 5: Try different alignments for loading */
    if (Size > 3) {
        int align = 1 << (Data[3] % 4); /* 1, 2, 4, or 8 */
        loadedBuf = tj3LoadImage16(handle, "./dummy_file", &loadedWidth, 
                                  align, &loadedHeight, &loadedPixelFormat);
        if (loadedBuf) {
            tj3Free(loadedBuf);
        }
    }
    
    /* Test 6: Try compression with different pitch values */
    if (imageBuf) {
        int pitch = width * samplesPerPixel + (Data[0] % 16); /* Add some padding */
        jpegBuf = NULL;
        jpegSize = 0;
        tj3Compress16(handle, imageBuf, width, pitch, height, pixelFormat, 
                     &jpegBuf, &jpegSize);
    }
    
    /* Test 7: Try with pre-allocated buffer */
    if (Size > 4) {
        size_t preallocSize = (Data[4] % 1024) + 1024;
        unsigned char *preallocBuf = (unsigned char *)tj3Alloc(preallocSize);
        if (preallocBuf) {
            jpegSize = preallocSize;
            tj3Compress16(handle, imageBuf, width, 0, height, pixelFormat, 
                         &preallocBuf, &jpegSize);
            tj3Free(preallocBuf);
        }
    }
    
cleanup:
    if (jpegBuf) tj3Free(jpegBuf);
    if (imageBuf) tj3Free(imageBuf);
    if (handle) tj3Destroy(handle);
    remove_dummy_file();
    
    return 0;
}