// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3DecompressHeader at turbojpeg.c:1815:15 in turbojpeg.h
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3TransformBufSize at turbojpeg.c:2828:18 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Transform at turbojpeg.c:2867:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3TransformBufSize at turbojpeg.c:2828:18 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Transform at turbojpeg.c:2867:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3TransformBufSize at turbojpeg.c:2828:18 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Transform at turbojpeg.c:2867:15 in turbojpeg.h
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

static int customFilter(short *coeffs, tjregion arrayRegion,
                       tjregion planeRegion, int componentID, int transformID,
                       struct tjtransform *transform) {
    return 0;
}

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    tjhandle handle = tj3Init(TJINIT_DECOMPRESS);
    if (!handle) {
        return 0;
    }

    int result = tj3DecompressHeader(handle, Data, Size);
    if (result == 0) {
        tj3Get(handle, TJPARAM_JPEGWIDTH);
        tj3Get(handle, TJPARAM_JPEGHEIGHT);
        tj3Get(handle, TJPARAM_SUBSAMP);
        
        tj3Set(handle, TJPARAM_QUALITY, 85);
        tj3Set(handle, TJPARAM_SUBSAMP, TJSAMP_420);
        tj3Set(handle, TJPARAM_FASTUPSAMPLE, 1);
        tj3Set(handle, TJPARAM_NOREALLOC, 0);
        
        tjtransform transform1 = {0};
        transform1.op = TJXOP_NONE;
        transform1.options = TJXOPT_COPYNONE;
        transform1.customFilter = customFilter;
        
        size_t buf_size1 = tj3TransformBufSize(handle, &transform1);
        if (buf_size1 > 0) {
            unsigned char *dstBuf1 = tj3Alloc(buf_size1);
            if (dstBuf1) {
                size_t dstSize1 = buf_size1;
                tj3Set(handle, TJPARAM_NOREALLOC, 1);
                tj3Transform(handle, Data, Size, 1, &dstBuf1, &dstSize1, &transform1);
                tj3Free(dstBuf1);
            }
        }
        
        tjtransform transform2 = {0};
        transform2.op = TJXOP_ROT90;
        transform2.options = TJXOPT_TRIM;
        transform2.r.x = 0;
        transform2.r.y = 0;
        transform2.r.w = 100;
        transform2.r.h = 100;
        
        size_t buf_size2 = tj3TransformBufSize(handle, &transform2);
        if (buf_size2 > 0) {
            unsigned char *dstBuf2 = tj3Alloc(buf_size2);
            if (dstBuf2) {
                size_t dstSize2 = buf_size2;
                tj3Transform(handle, Data, Size, 1, &dstBuf2, &dstSize2, &transform2);
                tj3Free(dstBuf2);
            }
        }
        
        tjtransform transform3 = {0};
        transform3.op = TJXOP_HFLIP;
        transform3.options = TJXOPT_GRAY;
        transform3.customFilter = customFilter;
        
        size_t buf_size3 = tj3TransformBufSize(handle, &transform3);
        if (buf_size3 > 0) {
            unsigned char *dstBuf3 = tj3Alloc(buf_size3);
            if (dstBuf3) {
                size_t dstSize3 = buf_size3;
                tj3Set(handle, TJPARAM_NOREALLOC, 0);
                tj3Transform(handle, Data, Size, 1, &dstBuf3, &dstSize3, &transform3);
                tj3Free(dstBuf3);
            }
        }
    }
    
    tj3Destroy(handle);
    return 0;
}