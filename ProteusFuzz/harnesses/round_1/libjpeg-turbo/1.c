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
    if (Size < 1) return 0;

    tjhandle handle = tj3Init(TJINIT_DECOMPRESS);
    if (handle == NULL) return 0;

    const unsigned char *jpegBuf = Data;
    size_t jpegSize = Size;

    if (tj3DecompressHeader(handle, jpegBuf, jpegSize) != 0) {
        // Header read might fail, proceed to exercise other APIs
    }

    (void)tj3Get(handle, TJPARAM_SUBSAMP);
    (void)tj3Get(handle, TJPARAM_JPEGWIDTH);
    (void)tj3Get(handle, TJPARAM_JPEGHEIGHT);

    (void)tj3Set(handle, TJPARAM_FASTDCT, 1);
    (void)tj3Set(handle, TJPARAM_LOSSLESS, 0);
    (void)tj3Set(handle, TJPARAM_SAVEMARKERS, 0);

    (void)tj3Get(handle, TJPARAM_LOSSLESS);

    (void)tj3Set(handle, TJPARAM_COLORSPACE, TJCS_RGB);

    tjscalingfactor sf = {1, 1};
    (void)tj3SetScalingFactor(handle, sf);
    
    tjscalingfactor sf2 = {1, 8};
    (void)tj3SetScalingFactor(handle, sf2);

    tjregion crop = {0, 0, 0, 0};
    (void)tj3SetCroppingRegion(handle, crop);
    
    tjregion crop2 = {0, 0, 16, 16};
    (void)tj3SetCroppingRegion(handle, crop2);

    size_t allocSize = 64 * 64 * 3 * sizeof(unsigned short);
    if (allocSize > 1024 * 1024) allocSize = 1024 * 1024;

    unsigned short *dstBuf = (unsigned short *)tj3Alloc(allocSize);
    if (dstBuf == NULL) {
        tj3Destroy(handle);
        return 0;
    }

    int pitch = 64 * 3 * sizeof(unsigned short);
    (void)tj3Decompress16(handle, jpegBuf, jpegSize, dstBuf, pitch, TJPF_RGB);

    tj3Free(dstBuf);
    tj3Destroy(handle);

    return 0;
}