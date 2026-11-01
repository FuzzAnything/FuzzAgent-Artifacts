// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3DecompressHeader at turbojpeg.c:1815:15 in turbojpeg.h
// tj3SetScalingFactor at turbojpeg.c:1981:15 in turbojpeg.h
// tj3SetCroppingRegion at turbojpeg.c:2006:15 in turbojpeg.h
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tj3Decompress16 at turbojpeg-mp.c:148:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <turbojpeg.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Ensure minimum size for basic operations
    if (Size < 4) return 0;

    int initType = Data[0] & 0x03; // Mask to valid range for init types (0-3)
    tjhandle handle = tj3Init(initType);
    if (!handle) return 0;

    // Prepare JPEG buffer (skip first byte used for initType)
    const unsigned char *jpegBuf = Data + 1;
    size_t jpegSize = Size - 1;

    // Fuzz tj3DecompressHeader
    // We try to decompress header with the fuzzed data
    int ret = tj3DecompressHeader(handle, jpegBuf, jpegSize);
    (void)ret; // Ignore return for fuzzing state exploration, but function is called

    // Fuzz tj3SetScalingFactor
    // Construct a scaling factor from data
    tjscalingfactor sf;
    sf.num = (Size > 2) ? (Data[2] & 0x0F) : 1;
    sf.denom = (Size > 3) ? (Data[3] & 0x0F) : 1;
    if (sf.denom == 0) sf.denom = 1;
    
    ret = tj3SetScalingFactor(handle, sf);
    (void)ret;

    // Fuzz tj3SetCroppingRegion
    tjregion cr;
    cr.x = (Size > 4) ? (int)Data[4] : 0;
    cr.y = (Size > 5) ? (int)Data[5] : 0;
    cr.w = (Size > 6) ? (int)Data[6] : 0;
    cr.h = (Size > 7) ? (int)Data[7] : 0;
    
    ret = tj3SetCroppingRegion(handle, cr);
    (void)ret;

    // Fuzz tj3Get
    int param = (Size > 8) ? (int)Data[8] : 0;
    int val = tj3Get(handle, param);
    (void)val;

    // Fuzz tj3Decompress16
    // Requires a destination buffer. Allocate a small one.
    // Pitch and pixelFormat derived from data
    int pitch = (Size > 9) ? (int)Data[9] : 0;
    int pixelFormat = (Size > 10) ? (int)Data[10] : 0;
    
    // Allocate a small buffer for decompression target
    // Limit size to avoid OOM in fuzzer
    size_t maxBufSize = 1024 * 1024; 
    size_t allocSize = (pitch > 0 && cr.h > 0) ? (size_t)pitch * cr.h : 0;
    if (allocSize == 0 || allocSize > maxBufSize) allocSize = 1024;
    
    unsigned short *dstBuf = (unsigned short *)malloc(allocSize);
    if (dstBuf) {
        ret = tj3Decompress16(handle, jpegBuf, jpegSize, dstBuf, pitch, pixelFormat);
        (void)ret;
        free(dstBuf);
    }

    // Cleanup
    tj3Destroy(handle);
    
    return 0;
}