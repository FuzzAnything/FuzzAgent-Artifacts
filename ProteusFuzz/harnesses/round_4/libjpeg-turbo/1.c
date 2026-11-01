// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3DecompressHeader at turbojpeg.c:1815:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
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

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    tjhandle handle = tj3Init(TJINIT_DECOMPRESS);
    if (handle == NULL) return 0;

    if (tj3DecompressHeader(handle, Data, Size) != 0) {
        tj3Destroy(handle);
        return 0;
    }

    // tj3Get calls
    int val1 = tj3Get(handle, TJPARAM_SUBSAMP);
    int val2 = tj3Get(handle, TJPARAM_JPEGWIDTH);
    int val3 = tj3Get(handle, TJPARAM_JPEGHEIGHT);
    (void)val1; (void)val2; (void)val3;

    // tj3Set calls
    tj3Set(handle, TJPARAM_FASTDCT, 1);
    tj3Set(handle, TJPARAM_PROGRESSIVE, 1);
    tj3Set(handle, TJPARAM_STOPONWARNING, 1);

    // Another tj3Get
    int val4 = tj3Get(handle, TJPARAM_FASTDCT);
    (void)val4;

    // More tj3Set calls
    tj3Set(handle, TJPARAM_SAVEMARKERS, 0);
    tj3Set(handle, TJPARAM_LOSSLESS, 0);

    // tj3SetScalingFactor calls
    tjscalingfactor sf1 = {1, 1};
    tjscalingfactor sf2 = {1, 8};
    tj3SetScalingFactor(handle, sf1);
    tj3SetScalingFactor(handle, sf2);

    // tj3SetCroppingRegion calls
    tjregion cr1 = {0, 0, 0, 0}; // TJUNCROPPED
    tjregion cr2 = {0, 0, 100, 100};
    tj3SetCroppingRegion(handle, cr1);
    tj3SetCroppingRegion(handle, cr2);

    // Allocate buffer for decompression
    void *dstBuf = tj3Alloc(Size * 4); // Over-allocate to be safe
    if (dstBuf == NULL) {
        tj3Destroy(handle);
        return 0;
    }

    // Decompress 16-bit
    int ret = tj3Decompress16(handle, Data, Size, dstBuf, 0, TJPF_RGB);
    (void)ret;

    // Cleanup
    tj3Free(dstBuf);
    tj3Destroy(handle);

    return 0;
}