// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Decompress16 at turbojpeg-mp.c:148:15 in turbojpeg.h
// tj3DecompressHeader at turbojpeg.c:1815:15 in turbojpeg.h
// tj3DecompressHeader at turbojpeg.c:1815:15 in turbojpeg.h
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tj3Transform at turbojpeg.c:2867:15 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Decompress16 at turbojpeg-mp.c:148:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Transform at turbojpeg.c:2867:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <turbojpeg.h>

int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    if (Size < 5) return 0;

    // Use first byte to select operation type
    int opType = Data[0] % 10;
    
    tjhandle handle = NULL;
    
    // Initialize handle based on operation needs
    if (opType >= 3 && opType <= 6) {
        // Operations 3-6 need decompression handle
        handle = tj3Init(TJINIT_DECOMPRESS);
    } else if (opType == 2 || opType == 8) {
        // Transform needs transform handle
        handle = tj3Init(TJINIT_TRANSFORM);
    } else {
        // Default to decompress for set/get tests
        handle = tj3Init(TJINIT_DECOMPRESS);
    }
    
    if (!handle) return 0;

    // Ensure we have enough data for a minimal JPEG-like buffer
    // Create a minimal valid JPEG header if input is too small
    unsigned char *jpegBuf = NULL;
    size_t jpegSize = Size - 2;
    
    if (jpegSize < 10) {
        // Create a minimal fake JPEG buffer
        jpegBuf = (unsigned char *)malloc(20);
        if (!jpegBuf) {
            tj3Destroy(handle);
            return 0;
        }
        // Minimal JPEG SOI + APP0 + SOF0 + DHT + SOS
        jpegBuf[0] = 0xFF; jpegBuf[1] = 0xD8; // SOI
        jpegBuf[2] = 0xFF; jpegBuf[3] = 0xE0; // APP0
        jpegBuf[4] = 0x00; jpegBuf[5] = 0x10; // Length
        // Fill rest with zeros
        memset(jpegBuf + 6, 0, 14);
        jpegSize = 20;
    } else {
        jpegBuf = (unsigned char *)malloc(jpegSize);
        if (!jpegBuf) {
            tj3Destroy(handle);
            return 0;
        }
        memcpy(jpegBuf, Data + 2, jpegSize);
        // Ensure it starts with JPEG SOI
        if (jpegBuf[0] != 0xFF || jpegBuf[1] != 0xD8) {
            jpegBuf[0] = 0xFF;
            jpegBuf[1] = 0xD8;
        }
    }

    switch (opType) {
        case 0: {
            // Test tj3Get
            int param = Data[2] % 10;
            int result = tj3Get(handle, param);
            (void)result;
            break;
        }
        case 1: {
            // Test tj3Set
            int param = Data[2] % 10;
            int value = (Data[3] << 8) | Data[4];
            int result = tj3Set(handle, param, value);
            (void)result;
            break;
        }
        case 2: {
            // Test tj3Transform
            if (Size < 10) break;
            
            // Create transform structure
            tjtransform *transforms = (tjtransform *)malloc(sizeof(tjtransform));
            if (!transforms) break;
            
            transforms->r.w = Data[2] % 100 + 1;
            transforms->r.h = Data[3] % 100 + 1;
            transforms->r.x = 0;
            transforms->r.y = 0;
            transforms->op = Data[4] % 8; // TJXOP constants
            transforms->options = Data[5] % 16;
            transforms->data = NULL;
            transforms->customFilter = NULL;
            
            unsigned char *dstBuf = NULL;
            size_t dstSize = 0;
            
            int result = tj3Transform(handle, jpegBuf, jpegSize, 1, &dstBuf, &dstSize, transforms);
            (void)result;
            
            if (dstBuf) {
                tj3Free(dstBuf);
            }
            free(transforms);
            break;
        }
        case 3: {
            // Test tj3Decompress16
            if (Size < 10) break;
            
            int width = Data[2] % 100 + 1;
            int height = Data[3] % 100 + 1;
            int pitch = width * 3; // RGB 16-bit: 2 bytes per channel * 3 channels
            int pixelFormat = Data[4] % 4; // TJPF_GRAY, TJPF_RGB, TJPF_CMYK, TJPF_YUVPacked
            
            size_t dstSize = (size_t)height * pitch * 2; // 16-bit samples
            unsigned short *dstBuf = (unsigned short *)malloc(dstSize);
            if (!dstBuf) break;
            
            int result = tj3Decompress16(handle, jpegBuf, jpegSize, dstBuf, pitch, pixelFormat);
            (void)result;
            
            free(dstBuf);
            break;
        }
        case 4: {
            // Test tj3DecompressHeader
            int result = tj3DecompressHeader(handle, jpegBuf, jpegSize);
            (void)result;
            break;
        }
        case 5: {
            // Test tj3Get after tj3DecompressHeader
            tj3DecompressHeader(handle, jpegBuf, jpegSize);
            int param = Data[2] % 10;
            int result = tj3Get(handle, param);
            (void)result;
            break;
        }
        case 6: {
            // Test tj3Set with invalid params
            int param = Data[2] % 100; // Wider range to hit invalid params
            int value = (Data[3] << 8) | Data[4];
            int result = tj3Set(handle, param, value);
            (void)result;
            break;
        }
        case 7: {
            // Test tj3Get with invalid params
            int param = Data[2] % 100;
            int result = tj3Get(handle, param);
            (void)result;
            break;
        }
        case 8: {
            // Test tj3Transform with multiple transforms
            if (Size < 15) break;
            
            int n = (Data[2] % 3) + 1; // 1 to 3 transforms
            tjtransform *transforms = (tjtransform *)malloc(n * sizeof(tjtransform));
            if (!transforms) break;
            
            unsigned char **dstBufs = (unsigned char **)malloc(n * sizeof(unsigned char *));
            size_t *dstSizes = (size_t *)malloc(n * sizeof(size_t));
            if (!dstBufs || !dstSizes) {
                free(transforms);
                free(dstBufs);
                free(dstSizes);
                break;
            }
            
            for (int i = 0; i < n; i++) {
                transforms[i].r.w = (Data[3 + i*3] % 100) + 1;
                transforms[i].r.h = (Data[4 + i*3] % 100) + 1;
                transforms[i].r.x = 0;
                transforms[i].r.y = 0;
                transforms[i].op = Data[5 + i*3] % 8;
                transforms[i].options = 0;
                transforms[i].data = NULL;
                transforms[i].customFilter = NULL;
                dstBufs[i] = NULL;
                dstSizes[i] = 0;
            }
            
            int result = tj3Transform(handle, jpegBuf, jpegSize, n, dstBufs, dstSizes, transforms);
            (void)result;
            
            for (int i = 0; i < n; i++) {
                if (dstBufs[i]) {
                    tj3Free(dstBufs[i]);
                }
            }
            free(dstBufs);
            free(dstSizes);
            free(transforms);
            break;
        }
        case 9: {
            // Test tj3Decompress16 with different pixel formats
            if (Size < 5) break;
            
            int width = Data[2] % 100 + 1;
            int height = Data[3] % 100 + 1;
            int pixelFormat = Data[4] % 4;
            int pitch = width * 3; // RGB
            
            size_t dstSize = (size_t)height * pitch * 2;
            unsigned short *dstBuf = (unsigned short *)malloc(dstSize);
            if (!dstBuf) break;
            
            int result = tj3Decompress16(handle, jpegBuf, jpegSize, dstBuf, pitch, pixelFormat);
            (void)result;
            
            free(dstBuf);
            break;
        }
    }

    tj3Destroy(handle);
    free(jpegBuf);
    
    return 0;
}