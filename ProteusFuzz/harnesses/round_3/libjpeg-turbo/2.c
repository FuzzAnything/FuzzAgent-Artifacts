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
    if (Size < 10) {
        return 0;
    }

    tjhandle handle = tj3Init(TJINIT_DECOMPRESS);
    if (handle == NULL) {
        return 0;
    }

    const unsigned char *jpegBuf = Data;
    size_t jpegSize = Size;

    if (tj3DecompressHeader(handle, jpegBuf, jpegSize) != 0) {
        // Continue even on error to test state handling
    }

    // tj3Get (3 times)
    tj3Get(handle, TJPARAM_SUBSAMP);
    tj3Get(handle, TJPARAM_LOSSLESS);
    tj3Get(handle, TJPARAM_JPEGWIDTH);

    // tj3Set (3 times)
    tj3Set(handle, TJPARAM_FASTDCT, 1);
    tj3Set(handle, TJPARAM_SAVEMARKERS, 0);
    tj3Set(handle, TJPARAM_STOPONWARNING, 0);

    // tj3Get
    tj3Get(handle, TJPARAM_FASTDCT);

    // tj3Set (2 times)
    tj3Set(handle, TJPARAM_SAVEMARKERS, 2);
    tj3Set(handle, TJPARAM_JPEGWIDTH, 0);

    // tj3SetScalingFactor (2 times)
    tjscalingfactor sf1 = {1, 8};
    tj3SetScalingFactor(handle, sf1);
    
    tjscalingfactor sf3 = {1, 1};
    tj3SetScalingFactor(handle, sf3);

    // tj3SetCroppingRegion (2 times)
    tjregion cr1 = {0, 0, 0, 0};
    tj3SetCroppingRegion(handle, cr1);
    
    tjregion cr2 = {16, 16, 32, 32};
    tj3SetCroppingRegion(handle, cr2);

    // tj3Alloc
    size_t allocSize = 1024 * 1024;
    if (Size > 100) {
        allocSize = Size * 2;
    }
    unsigned short *dstBuf = (unsigned short *)tj3Alloc(allocSize);
    if (dstBuf == NULL) {
        tj3Destroy(handle);
        return 0;
    }

    // tj3Decompress16
    int pixelFormat = (Size % 4) == 0 ? TJPF_GRAY : TJPF_RGB;
    int pitch = 0;
    
    tj3Decompress16(handle, jpegBuf, jpegSize, dstBuf, pitch, pixelFormat);

    tj3Free(dstBuf);
    tj3Destroy(handle);

    return 0;
}