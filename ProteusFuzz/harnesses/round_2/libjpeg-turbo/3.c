// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
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
// tj3LoadImage16 at turbojpeg-mp.c:293:21 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tj3JPEGBufSize at turbojpeg.c:903:18 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <turbojpeg.h>

static void write_dummy_file(const char *filename, const uint8_t *data, size_t size) {
    FILE *f = fopen(filename, "wb");
    if (f) {
        // Write a minimal valid PPM header followed by the fuzz data
        // P6 2 2 255 (2x2 RGB image)
        const char *header = "P6\n2 2\n255\n";
        size_t header_len = strlen(header);
        fwrite(header, 1, header_len, f);
        // Write as much data as possible, ensuring at least 12 bytes (2*2*3)
        size_t to_write = (size > 12) ? 12 : size;
        if (to_write > 0) {
            fwrite(data, 1, to_write, f);
        }
        // Pad if necessary to ensure 12 bytes of pixel data
        if (to_write < 12) {
            uint8_t pad = 0;
            for (size_t i = to_write; i < 12; i++) {
                fputc(pad, f);
            }
        }
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    tjhandle handle = NULL;
    unsigned short *srcBuf = NULL;
    unsigned char *jpegBuf = NULL;
    size_t jpegSize = 0;
    int width = 0, height = 0, pixelFormat = TJPF_UNKNOWN;
    int align = 1;
    int ret;

    // 1. Prepare the environment
    // Initialize handle for compression
    handle = tj3Init(TJINIT_COMPRESS);
    if (!handle) return 0;

    // 2. Invoke tj3Set multiple times (5 times as per sequence)
    // Using safe default values or values derived from input to explore states
    ret = tj3Set(handle, TJPARAM_QUALITY, (Size > 0) ? (Data[0] % 100) + 1 : 50);
    ret = tj3Set(handle, TJPARAM_SUBSAMP, (Size > 1) ? (Data[1] % 5) : 0);
    // Using TJPARAM_PROGRESSIVE for the 3rd set
    ret = tj3Set(handle, TJPARAM_PROGRESSIVE, (Size > 2) ? (Data[2] % 2) : 0);
    ret = tj3Set(handle, TJPARAM_PRECISION, (Size > 3) ? (13 + (Data[3] % 4)) : 16);
    ret = tj3Set(handle, TJPARAM_NOREALLOC, (Size > 4) ? (Data[4] % 2) : 0);

    // 3. Write dummy file and load image
    write_dummy_file("./dummy_file", Data, Size);
    
    // Load image 16-bit
    srcBuf = tj3LoadImage16(handle, "./dummy_file", &width, align, &height, &pixelFormat);
    
    if (!srcBuf || width <= 0 || height <= 0) {
        tj3Destroy(handle);
        return 0;
    }

    // Calculate pitch (samples per row)
    // tjPixelSize is not directly accessible without including full header details or defining it,
    // but we can estimate based on pixelFormat.
    // For safety, we assume a reasonable pitch or use width * samples_per_pixel.
    // Since we don't have tjPixelSize array here, we calculate based on format.
    int samplesPerPixel = 1;
    if (pixelFormat == TJPF_GRAY) samplesPerPixel = 1;
    else if (pixelFormat == TJPF_RGB || pixelFormat == TJPF_CMYK) samplesPerPixel = 3;
    else samplesPerPixel = 3; // Default to RGB-like

    int pitch = width * samplesPerPixel;

    // 4. Get buffer size
    int subsamp = tj3Get(handle, TJPARAM_SUBSAMP);
    if (subsamp < 0) subsamp = TJSAMP_444;
    size_t maxBufSize = tj3JPEGBufSize(width, height, subsamp);
    if (maxBufSize == 0) {
        tj3Free(srcBuf);
        tj3Destroy(handle);
        return 0;
    }

    // 5. Get parameter value
    int quality = tj3Get(handle, TJPARAM_QUALITY);
    (void)quality; // Use variable to avoid unused warning

    // 6. Allocate buffer
    jpegBuf = (unsigned char *)tj3Alloc(maxBufSize);
    if (!jpegBuf) {
        tj3Free(srcBuf);
        tj3Destroy(handle);
        return 0;
    }
    jpegSize = maxBufSize;

    // 7. Invoke tj3Set multiple times (3 times as per sequence)
    // Changing settings before compression
    ret = tj3Set(handle, TJPARAM_ARITHMETIC, (Size > 5) ? (Data[5] % 2) : 0);
    // Replaced TJPARAM_TURBO with TJPARAM_FASTDCT which is a valid parameter
    ret = tj3Set(handle, TJPARAM_FASTDCT, (Size > 6) ? (Data[6] % 2) : 0);
    ret = tj3Set(handle, TJPARAM_PROGRESSIVE, (Size > 7) ? (Data[7] % 2) : 0);

    // 8. Compress 16-bit image
    ret = tj3Compress16(handle, srcBuf, width, pitch, height, pixelFormat, &jpegBuf, &jpegSize);
    
    // 9. Cleanup
    tj3Free(srcBuf);
    tj3Free(jpegBuf);
    tj3Destroy(handle);

    return 0;
}