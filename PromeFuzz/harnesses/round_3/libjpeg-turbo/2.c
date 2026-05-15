// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Transform at turbojpeg.c:2867:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Transform at turbojpeg.c:2867:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3DecompressHeader at turbojpeg.c:1815:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
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
// tj3TransformBufSize at turbojpeg.c:2828:18 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Transform at turbojpeg.c:2867:15 in turbojpeg.h
// tj3TransformBufSize at turbojpeg.c:2828:18 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "src/turbojpeg.h"

static int dummy_filter(short *coeffs, tjregion arrayRegion,
                       tjregion planeRegion, int componentID, int transformID,
                       struct tjtransform *transform) {
    return 0;
}

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    tjhandle handle = NULL;
    unsigned char *jpegBuf = NULL;
    size_t jpegSize = Size;
    int width, height, subsamp;
    size_t bufSize1, bufSize2, bufSize3;
    unsigned char *dstBuf1 = NULL, *dstBuf2 = NULL, *dstBuf3 = NULL;
    size_t dstSize1 = 0, dstSize2 = 0, dstSize3 = 0;
    tjtransform transform1, transform2, transform3;
    int ret = 0;
    
    if (Size < 1) return 0;
    
    handle = tj3Init(TJINIT_TRANSFORM);
    if (!handle) return 0;
    
    jpegBuf = (unsigned char *)Data;
    
    ret = tj3DecompressHeader(handle, jpegBuf, jpegSize);
    if (ret != 0) {
        tj3Destroy(handle);
        return 0;
    }
    
    width = tj3Get(handle, TJPARAM_JPEGWIDTH);
    height = tj3Get(handle, TJPARAM_JPEGHEIGHT);
    subsamp = tj3Get(handle, TJPARAM_SUBSAMP);
    
    tj3Set(handle, TJPARAM_QUALITY, 85);
    tj3Set(handle, TJPARAM_SUBSAMP, TJSAMP_420);
    tj3Set(handle, TJPARAM_FASTUPSAMPLE, 1);
    tj3Set(handle, TJPARAM_FASTDCT, 1);
    
    memset(&transform1, 0, sizeof(tjtransform));
    transform1.op = TJXOP_NONE;
    transform1.options = TJXOPT_COPYNONE;
    transform1.r.x = 0;
    transform1.r.y = 0;
    transform1.r.w = width > 0 ? width / 2 : 0;
    transform1.r.h = height > 0 ? height / 2 : 0;
    transform1.customFilter = dummy_filter;
    
    bufSize1 = tj3TransformBufSize(handle, &transform1);
    if (bufSize1 > 0) {
        dstBuf1 = (unsigned char *)tj3Alloc(bufSize1);
        dstSize1 = bufSize1;
    }
    
    tj3Set(handle, TJPARAM_NOREALLOC, 1);
    
    if (dstBuf1) {
        ret = tj3Transform(handle, jpegBuf, jpegSize, 1, &dstBuf1, &dstSize1, &transform1);
    }
    
    memset(&transform2, 0, sizeof(tjtransform));
    transform2.op = TJXOP_ROT90;
    transform2.options = TJXOPT_TRIM;
    transform2.r.x = 0;
    transform2.r.y = 0;
    transform2.r.w = width > 0 ? width : 0;
    transform2.r.h = height > 0 ? height : 0;
    transform2.customFilter = NULL;
    
    bufSize2 = tj3TransformBufSize(handle, &transform2);
    if (bufSize2 > 0) {
        dstBuf2 = (unsigned char *)tj3Alloc(bufSize2);
        dstSize2 = bufSize2;
    }
    
    if (dstBuf2) {
        ret = tj3Transform(handle, jpegBuf, jpegSize, 1, &dstBuf2, &dstSize2, &transform2);
    }
    
    memset(&transform3, 0, sizeof(tjtransform));
    transform3.op = TJXOP_HFLIP;
    transform3.options = TJXOPT_GRAY;
    transform3.r.x = width > 0 ? width / 4 : 0;
    transform3.r.y = height > 0 ? height / 4 : 0;
    transform3.r.w = width > 0 ? width / 2 : 0;
    transform3.r.h = height > 0 ? height / 2 : 0;
    transform3.customFilter = dummy_filter;
    
    bufSize3 = tj3TransformBufSize(handle, &transform3);
    if (bufSize3 > 0) {
        dstBuf3 = (unsigned char *)tj3Alloc(bufSize3);
        dstSize3 = bufSize3;
    }
    
    if (dstBuf3) {
        ret = tj3Transform(handle, jpegBuf, jpegSize, 1, &dstBuf3, &dstSize3, &transform3);
    }
    
    tj3Set(handle, TJPARAM_BOTTOMUP, 1);
    
    if (dstBuf1) {
        ret = tj3Transform(handle, jpegBuf, jpegSize, 1, &dstBuf1, &dstSize1, &transform1);
    }
    
    if (dstBuf1) tj3Free(dstBuf1);
    if (dstBuf2) tj3Free(dstBuf2);
    if (dstBuf3) tj3Free(dstBuf3);
    
    tj3Destroy(handle);
    
    return 0;
}