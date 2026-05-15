// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3LoadImage16 at turbojpeg-mp.c:293:21 in turbojpeg.h
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
#include <stdio.h>

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

int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    if (Size < 8) return 0;
    
    write_dummy_file(Data, Size);
    
    tjhandle handle = tj3Init(TJINIT_COMPRESS);
    if (!handle) {
        remove_dummy_file();
        return 0;
    }
    
    tj3Set(handle, TJPARAM_SUBSAMP, Data[0] % 6);
    tj3Set(handle, TJPARAM_QUALITY, Data[1] % 101);
    tj3Set(handle, TJPARAM_LOSSLESS, Data[2] % 2);
    tj3Set(handle, TJPARAM_ARITHMETIC, Data[3] % 2);
    tj3Set(handle, TJPARAM_OPTIMIZE, Data[4] % 2);
    
    int width = 0, height = 0, pixelFormat = TJPF_UNKNOWN;
    unsigned short *imageBuf = tj3LoadImage16(handle, "./dummy_file", 
                                              &width, 1, &height, &pixelFormat);
    
    if (imageBuf && width > 0 && height > 0) {
        size_t bufSize = tj3JPEGBufSize(width, height, TJSAMP_444);
        tj3Get(handle, TJPARAM_SUBSAMP);
        
        unsigned char *jpegBuf = NULL;
        if (bufSize > 0) {
            jpegBuf = (unsigned char *)tj3Alloc(bufSize);
        }
        
        tj3Set(handle, TJPARAM_PRECISION, 13 + (Data[5] % 4));
        tj3Set(handle, TJPARAM_NOREALLOC, Data[6] % 2);
        tj3Set(handle, TJPARAM_PROGRESSIVE, Data[7] % 2);
        
        if (jpegBuf) {
            size_t jpegSize = bufSize;
            tj3Compress16(handle, imageBuf, width, 0, height, pixelFormat, 
                          &jpegBuf, &jpegSize);
            tj3Free(jpegBuf);
        }
        
        tj3Free(imageBuf);
    }
    
    tj3Destroy(handle);
    remove_dummy_file();
    
    return 0;
}