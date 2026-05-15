// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3LoadImage16 at turbojpeg-mp.c:293:21 in turbojpeg.h
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
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "turbojpeg.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    // Write fuzzer input to dummy file for tj3LoadImage16
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Initialize TurboJPEG instance for compression
    tjhandle handle = tj3Init(TJINIT_COMPRESS);
    if (!handle) {
        remove("./dummy_file");
        return 0;
    }

    // Set various parameters (5 times as specified)
    // Use some common parameter values from fuzzer data
    if (Size > 0) {
        tj3Set(handle, TJPARAM_SUBSAMP, Data[0] % 5); // TJSAMP values 0-4
    } else {
        tj3Set(handle, TJPARAM_SUBSAMP, TJSAMP_444);
    }
    
    if (Size > 1) {
        tj3Set(handle, TJPARAM_QUALITY, Data[1] % 101); // Quality 0-100
    } else {
        tj3Set(handle, TJPARAM_QUALITY, 90);
    }
    
    if (Size > 2) {
        tj3Set(handle, TJPARAM_OPTIMIZE, Data[2] & 1); // Boolean
    }
    
    if (Size > 3) {
        tj3Set(handle, TJPARAM_ARITHMETIC, Data[3] & 1); // Boolean
    }
    
    if (Size > 4) {
        tj3Set(handle, TJPARAM_PROGRESSIVE, Data[4] & 1); // Boolean
    }

    // Load 16-bit image from dummy file
    int width = 0, height = 0, pixelFormat = TJPF_UNKNOWN;
    unsigned short *srcBuf = tj3LoadImage16(handle, "./dummy_file", &width, 1, &height, &pixelFormat);
    
    // Calculate JPEG buffer size (use default subsampling if width/height are valid)
    size_t jpegBufSize = 0;
    if (width > 0 && height > 0) {
        int subsamp = tj3Get(handle, TJPARAM_SUBSAMP);
        if (subsamp < 0) subsamp = TJSAMP_444;
        jpegBufSize = tj3JPEGBufSize(width, height, subsamp);
    }

    // Get a parameter value
    int quality = tj3Get(handle, TJPARAM_QUALITY);

    // Allocate JPEG buffer if size is valid
    unsigned char *jpegBuf = NULL;
    if (jpegBufSize > 0) {
        jpegBuf = (unsigned char *)tj3Alloc(jpegBufSize);
    }

    // Set more parameters (3 times as specified)
    if (Size > 5) {
        tj3Set(handle, TJPARAM_SCANLIMIT, Data[5] % 1000);
    }
    
    if (Size > 6) {
        tj3Set(handle, TJPARAM_BOTTOMUP, Data[6] & 1);
    }
    
    if (Size > 7) {
        tj3Set(handle, TJPARAM_FASTUPSAMPLE, Data[7] & 1);
    }

    // Compress 16-bit image if we have valid source buffer and JPEG buffer
    if (srcBuf && jpegBuf && width > 0 && height > 0 && pixelFormat >= 0) {
        size_t actualJpegSize = jpegBufSize;
        tj3Compress16(handle, srcBuf, width, 0, height, pixelFormat, &jpegBuf, &actualJpegSize);
    }

    // Cleanup
    if (srcBuf) {
        tj3Free(srcBuf);
    }
    if (jpegBuf) {
        tj3Free(jpegBuf);
    }
    tj3Destroy(handle);
    
    // Remove dummy file
    remove("./dummy_file");
    
    return 0;
}