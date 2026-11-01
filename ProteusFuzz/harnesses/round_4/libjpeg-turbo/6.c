// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3Decompress16 at turbojpeg-mp.c:148:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Compress16 at turbojpeg-mp.c:71:15 in turbojpeg.h
// tj3DecompressHeader at turbojpeg.c:1815:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <turbojpeg.h>

static void cleanup_handle(tjhandle handle) {
    if (handle) {
        tj3Destroy(handle);
    }
}

int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    tjhandle handle = NULL;
    int initType = Data[0];
    
    // Initialize handle based on first byte
    // 0: Compress, 1: Decompress, others: try compress then decompress
    if (initType == 0) {
        handle = tj3Init(TJINIT_COMPRESS);
    } else if (initType == 1) {
        handle = tj3Init(TJINIT_DECOMPRESS);
    } else {
        handle = tj3Init(TJINIT_COMPRESS);
        if (!handle) {
            handle = tj3Init(TJINIT_DECOMPRESS);
        }
    }

    if (!handle) return 0;

    // Test tj3Get and tj3Set
    // Use second byte as param index to fuzz different parameters
    int param = Data[1];
    int value = 0;
    
    // Fuzz tj3Set with various values
    if (Size > 2) value = Data[2];
    if (Size > 3) value = (value << 8) | Data[3];
    
    // Clamp value to reasonable range for some params, but allow overflow for others
    // to test boundary conditions
    tj3Set(handle, param, value);
    
    // Retrieve the value
    int retrieved = tj3Get(handle, param);
    (void)retrieved; // Suppress unused warning

    // Prepare data for compression/decompression if we have enough data
    if (Size > 10) {
        int width = (Data[4] % 20) + 1;
        int height = (Data[5] % 20) + 1;
        int pixelFormat = Data[6] % 8; // 0-7 covers common formats
        
        // Ensure width/height are not too large to avoid OOM
        if (width > 1000) width = 1000;
        if (height > 1000) height = 1000;

        size_t pitch = width * 3; // Assuming RGB for 16-bit (3 channels * 2 bytes)
        size_t srcSize = pitch * height;
        
        // Allocate source buffer
        unsigned short *srcBuf = (unsigned short *)malloc(srcSize);
        if (!srcBuf) {
            cleanup_handle(handle);
            return 0;
        }

        // Fill source buffer with fuzz data
        if (Size > 10 + srcSize) {
            memcpy(srcBuf, Data + 10, srcSize);
        } else {
            memset(srcBuf, 0, srcSize);
        }

        unsigned char *jpegBuf = NULL;
        size_t jpegSize = 0;

        // Test tj3Compress16 if initialized for compression
        // We rely on the initType variable we set, as TJPARAM_INITTYPE is not a valid param
        if (initType == 0) {
            // Try to set precision if supported
            tj3Set(handle, TJPARAM_PRECISION, 16);
            tj3Compress16(handle, srcBuf, width, (int)pitch, height, pixelFormat, &jpegBuf, &jpegSize);
        }

        // Test tj3DecompressHeader and tj3Decompress16 if we have a JPEG buffer
        if (jpegBuf && jpegSize > 0) {
            // Try decompression header
            tj3DecompressHeader(handle, jpegBuf, jpegSize);
            
            // Prepare destination buffer for 16-bit decompression
            // Pitch for 16-bit RGB is width * 3 samples.
            size_t dstPitch = width * 3;
            size_t dstSize = dstPitch * height;
            unsigned short *dstBuf = (unsigned short *)malloc(dstSize);
            
            if (dstBuf) {
                tj3Decompress16(handle, jpegBuf, jpegSize, dstBuf, (int)dstPitch, pixelFormat);
                free(dstBuf);
            }
        }
        
        // Free allocated buffers
        free(srcBuf);
        if (jpegBuf) {
            tj3Free(jpegBuf);
        }
    }

    cleanup_handle(handle);
    return 0;
}