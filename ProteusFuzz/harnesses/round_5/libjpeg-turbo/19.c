// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Compress16 at turbojpeg-mp.c:71:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3LoadImage16 at turbojpeg-mp.c:293:21 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tj3JPEGBufSize at turbojpeg.c:903:18 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <turbojpeg.h>

static int write_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) return -1;
    
    int width = 1;
    int height = 1;
    
    if (Size > 0) {
        width = Size > 1000 ? 1000 : Size;
        height = 1;
    }
    
    // Write PGM header (P5 is 16-bit grayscale)
    fprintf(f, "P5\n%d %d\n65535\n", width, height);
    
    size_t bytes_needed = (size_t)width * (size_t)height * 2;
    if (bytes_needed > 0) {
        unsigned char buf[2] = {0xFF, 0xFF};
        if (Size >= 2) {
            buf[0] = Data[0];
            buf[1] = Data[1];
        }
        fwrite(buf, 1, 2, f);
        
        size_t remaining = bytes_needed - 2;
        if (remaining > 0) {
            // Allocate buffer on heap to avoid stack overflow for large images
            unsigned char *zero_buf = (unsigned char *)malloc(remaining);
            if (zero_buf) {
                memset(zero_buf, 0, remaining);
                fwrite(zero_buf, 1, remaining, f);
                free(zero_buf);
            }
        }
    }
    
    fclose(f);
    return 0;
}

int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    tjhandle handle = tj3Init(TJINIT_COMPRESS);
    if (!handle) {
        return 0;
    }

    int val1 = Size > 0 ? Data[0] % 100 : 75;
    int val2 = Size > 1 ? Data[1] % 100 : 75;
    int val3 = Size > 2 ? Data[2] % 100 : 75;
    int val4 = Size > 3 ? Data[3] % 100 : 75;
    int val5 = Size > 4 ? Data[4] % 100 : 75;

    // tj3Set calls (first batch)
    tj3Set(handle, TJPARAM_QUALITY, val1);
    tj3Set(handle, TJPARAM_SUBSAMP, val2);
    tj3Set(handle, TJPARAM_PROGRESSIVE, val3); 
    #ifdef TJPARAM_ACCURACY
    tj3Set(handle, TJPARAM_ACCURACY, val4);
    #else
    tj3Set(handle, TJPARAM_PROGRESSIVE, val4);
    #endif
    tj3Set(handle, TJPARAM_PRECISION, val5);

    if (write_dummy_file(Data, Size) != 0) {
        tj3Destroy(handle);
        return 0;
    }

    int width = 0;
    int height = 0;
    int pixelFormat = TJPF_UNKNOWN;
    unsigned short *srcBuf = tj3LoadImage16(handle, "./dummy_file", &width, 1, &height, &pixelFormat);
    
    if (!srcBuf || width <= 0 || height <= 0) {
        tj3Destroy(handle);
        return 0;
    }

    // Get subsampling value for buffer size calculation
    int subsamp = tj3Get(handle, TJPARAM_SUBSAMP);
    if (subsamp < 0) subsamp = TJSAMP_444;

    size_t jpegSize = tj3JPEGBufSize(width, height, subsamp);
    if (jpegSize == 0) {
        tj3Free(srcBuf);
        tj3Destroy(handle);
        return 0;
    }

    int currentQuality = tj3Get(handle, TJPARAM_QUALITY);
    (void)currentQuality;

    unsigned char *jpegBuf = (unsigned char *)tj3Alloc(jpegSize);
    if (!jpegBuf) {
        tj3Free(srcBuf);
        tj3Destroy(handle);
        return 0;
    }

    int val6 = Size > 5 ? Data[5] % 100 : 75;
    int val7 = Size > 6 ? Data[6] % 100 : 75;
    int val8 = Size > 7 ? Data[7] % 100 : 75;

    // tj3Set calls (second batch)
    tj3Set(handle, TJPARAM_PROGRESSIVE, val6);
    #ifdef TJPARAM_ACCURACY
    tj3Set(handle, TJPARAM_ACCURACY, val7);
    #else
    tj3Set(handle, TJPARAM_PROGRESSIVE, val7);
    #endif
    tj3Set(handle, TJPARAM_PRECISION, val8);

    int pixelSize = tjPixelSize[pixelFormat];
    int pitch = width * pixelSize;
    
    unsigned char *jpegBufPtr = jpegBuf;
    size_t jpegSizePtr = jpegSize;

    int ret = tj3Compress16(handle, srcBuf, width, pitch, height, pixelFormat, &jpegBufPtr, &jpegSizePtr);
    (void)ret;

    tj3Free(srcBuf);
    tj3Free(jpegBufPtr);

    tj3Destroy(handle);

    return 0;
}