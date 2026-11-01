// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3DecompressHeader at turbojpeg.c:1815:15 in turbojpeg.h
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3SetScalingFactor at turbojpeg.c:1981:15 in turbojpeg.h
// tj3SetScalingFactor at turbojpeg.c:1981:15 in turbojpeg.h
// tj3SetCroppingRegion at turbojpeg.c:2006:15 in turbojpeg.h
// tj3SetCroppingRegion at turbojpeg.c:2006:15 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Decompress16 at turbojpeg-mp.c:148:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <turbojpeg.h>

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    tjhandle handle = tj3Init(TJINIT_DECOMPRESS);
    if (handle == NULL) return 0;

    const unsigned char *jpegBuf = Data;
    size_t jpegSize = Size;

    if (tj3DecompressHeader(handle, jpegBuf, jpegSize) != 0) {
        // Header reading failed, proceed to test other APIs for robustness
    }

    int val1 = tj3Get(handle, TJPARAM_JPEGWIDTH);
    int val2 = tj3Get(handle, TJPARAM_JPEGHEIGHT);
    int val3 = tj3Get(handle, TJPARAM_SUBSAMP);
    (void)val1; (void)val2; (void)val3;

    tj3Set(handle, TJPARAM_QUALITY, 90);
    // Replaced TJPARAM_ACCURACY with TJPARAM_FASTDCT as it is a valid parameter
    tj3Set(handle, TJPARAM_FASTDCT, 0);
    tj3Set(handle, TJPARAM_NOREALLOC, 0);

    int val4 = tj3Get(handle, TJPARAM_FASTDCT);
    (void)val4;

    tj3Set(handle, TJPARAM_SAVEMARKERS, 0);
    tj3Set(handle, TJPARAM_PROGRESSIVE, 0);

    tjscalingfactor sf1 = {1, 1};
    tjscalingfactor sf2 = {1, 8};
    tj3SetScalingFactor(handle, sf1);
    tj3SetScalingFactor(handle, sf2);

    tjregion crop1 = {0, 0, 0, 0}; 
    tjregion crop2 = {10, 10, 100, 100};
    tj3SetCroppingRegion(handle, crop1);
    tj3SetCroppingRegion(handle, crop2);

    size_t allocSize = Size * 4; 
    if (allocSize < 1024) allocSize = 1024;
    if (allocSize > 100 * 1024 * 1024) allocSize = 100 * 1024 * 1024; 

    unsigned short *dstBuf = (unsigned short *)tj3Alloc(allocSize);
    if (dstBuf == NULL) {
        tj3Destroy(handle);
        return 0;
    }

    int pitch = (int)(allocSize / sizeof(unsigned short));
    if (pitch <= 0) pitch = 1;
    
    int pixelFormat = TJPF_RGB; 

    tj3Decompress16(handle, jpegBuf, jpegSize, dstBuf, pitch, pixelFormat);

    tj3Free(dstBuf);
    tj3Destroy(handle);

    return 0;
}