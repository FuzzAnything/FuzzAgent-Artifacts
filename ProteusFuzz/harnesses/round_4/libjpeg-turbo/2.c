// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3LoadImage16 at turbojpeg-mp.c:293:21 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tj3JPEGBufSize at turbojpeg.c:903:18 in turbojpeg.h
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Compress16 at turbojpeg-mp.c:71:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <turbojpeg.h>

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        // Write a simple PGM header and then the raw data
        // P5 is the magic number for PGM binary
        // We assume width=100, height=100 for the header, but data might be different
        const char *header = "P5\n100 100\n65535\n";
        fwrite(header, 1, strlen(header), f);
        fwrite(Data, 1, Size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    if (Size < 6) return 0;

    // 1. Prepare the environment
    tjhandle handle = tj3Init(TJINIT_COMPRESS);
    if (!handle) return 0;

    // 2. Invoke tj3Set multiple times to explore states
    // Using data from input to vary parameters, with bounds checking
    int p1 = Data[0] % 100; // Quality
    int p2 = Data[1] % 5;   // Subsampling
    // TJPARAM_ACCURACY is not a valid parameter in libjpeg-turbo, using TJPARAM_FASTDCT instead
    int p3 = (Data[0] + Data[1]) % 5; // FastDCT (0-4)
    int p4 = (Data[0] ^ Data[1]) % 5; // FastDCT again or another valid param
    int p5 = Data[0] % 100; // Another valid param

    tj3Set(handle, TJPARAM_QUALITY, p1);
    tj3Set(handle, TJPARAM_SUBSAMP, p2);
    tj3Set(handle, TJPARAM_FASTDCT, p3);
    tj3Set(handle, TJPARAM_FASTDCT, p4);
    // Using TJPARAM_PRECISION instead of TJPARAM_CMYK as it is a valid parameter
    // Precision range is typically 13-16, so we map p5 to that range
    int precision = 13 + (p5 % 4); 
    tj3Set(handle, TJPARAM_PRECISION, precision);

    // Prepare for loading image
    int width = 0;
    int height = 0;
    int pixelFormat = TJPF_UNKNOWN;
    int align = 1 << (Data[2] % 5); // Power of 2 alignment (1, 2, 4, 8, 16)

    // Create dummy file for tj3LoadImage16
    write_dummy_file(Data, Size);

    // 3. Load Image
    unsigned short *srcBuf = tj3LoadImage16(handle, "./dummy_file", &width, align, &height, &pixelFormat);
    
    // If loading failed or dimensions are invalid, try to proceed with minimal checks or return
    if (!srcBuf || width <= 0 || height <= 0) {
        tj3Destroy(handle);
        return 0;
    }

    // Calculate JPEG buffer size
    int subsamp = tj3Get(handle, TJPARAM_SUBSAMP);
    size_t jpegSize = tj3JPEGBufSize(width, height, subsamp);
    
    // Get a parameter to ensure state exploration
    int quality = tj3Get(handle, TJPARAM_QUALITY);
    (void)quality;

    // Allocate buffer for JPEG output
    unsigned char *jpegBuf = NULL;
    size_t jpegBufSize = 0;
    
    if (jpegSize > 0) {
        jpegBuf = (unsigned char *)tj3Alloc(jpegSize);
        jpegBufSize = jpegSize;
    }

    // Set more parameters before compression
    int p6 = Data[3] % 100;
    int p7 = Data[4] % 5;
    int p8 = Data[5] % 5; // Use FastDCT range
    
    tj3Set(handle, TJPARAM_QUALITY, p6);
    tj3Set(handle, TJPARAM_SUBSAMP, p7);
    tj3Set(handle, TJPARAM_FASTDCT, p8);

    // Calculate pitch
    int pitch = width * tjPixelSize[pixelFormat];
    if (pitch == 0) pitch = width; // Fallback

    // 4. Compress
    tj3Compress16(handle, srcBuf, width, pitch, height, pixelFormat, &jpegBuf, &jpegBufSize);

    // 5. Cleanup
    tj3Free(srcBuf);
    tj3Free(jpegBuf);
    tj3Destroy(handle);

    return 0;
}