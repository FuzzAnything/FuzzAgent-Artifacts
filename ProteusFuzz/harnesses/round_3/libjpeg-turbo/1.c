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
// tj3TransformBufSize at turbojpeg.c:2828:18 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Transform at turbojpeg.c:2867:15 in turbojpeg.h
// tj3TransformBufSize at turbojpeg.c:2828:18 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Transform at turbojpeg.c:2867:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Transform at turbojpeg.c:2867:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <turbojpeg.h>

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    tjhandle handle = tj3Init(TJINIT_DECOMPRESS | TJINIT_TRANSFORM);
    if (handle == NULL) return 0;

    // Use the input data as the JPEG buffer. 
    int ret = tj3DecompressHeader(handle, Data, Size);
    // Continue even if header decompression fails to test error paths

    // tj3Get calls using valid parameters
    (void)tj3Get(handle, TJPARAM_QUALITY);
    (void)tj3Get(handle, TJPARAM_SUBSAMP);
    (void)tj3Get(handle, TJPARAM_SAVEMARKERS);

    // tj3Set calls using valid parameters
    (void)tj3Set(handle, TJPARAM_SAVEMARKERS, 0);
    (void)tj3Set(handle, TJPARAM_SAVEMARKERS, 1);
    (void)tj3Set(handle, TJPARAM_SAVEMARKERS, 2);
    (void)tj3Set(handle, TJPARAM_SAVEMARKERS, 4);

    // Prepare transform structure
    tjtransform transform = {0};
    transform.op = TJXOP_NONE;
    transform.options = 0;
    transform.r = (tjregion){0, 0, 0, 0};
    transform.customFilter = NULL;
    transform.data = NULL;

    // tj3TransformBufSize
    size_t bufSize = tj3TransformBufSize(handle, &transform);
    
    // tj3Alloc
    unsigned char *dstBuf = NULL;
    if (bufSize > 0) {
        dstBuf = (unsigned char *)tj3Alloc(bufSize);
    }
    size_t dstSize = bufSize;

    // tj3Set before transform
    (void)tj3Set(handle, TJPARAM_NOREALLOC, 0);

    // tj3Transform
    if (dstBuf != NULL) {
        tj3Transform(handle, Data, Size, 1, &dstBuf, &dstSize, &transform);
    }

    // Second round of transform logic
    bufSize = tj3TransformBufSize(handle, &transform);
    unsigned char *dstBuf2 = NULL;
    if (bufSize > 0) {
        dstBuf2 = (unsigned char *)tj3Alloc(bufSize);
    }
    size_t dstSize2 = bufSize;
    if (dstBuf2 != NULL) {
        tj3Transform(handle, Data, Size, 1, &dstBuf2, &dstSize2, &transform);
    }

    // Third round
    bufSize = tj3TransformBufSize(handle, &transform);
    unsigned char *dstBuf3 = NULL;
    if (bufSize > 0) {
        dstBuf3 = (unsigned char *)tj3Alloc(bufSize);
    }
    size_t dstSize3 = bufSize;
    if (dstBuf3 != NULL) {
        tj3Transform(handle, Data, Size, 1, &dstBuf3, &dstSize3, &transform);
    }

    // tj3Set after transforms
    (void)tj3Set(handle, TJPARAM_SAVEMARKERS, 2);

    // Final transform attempt
    if (dstBuf3 != NULL) {
        tj3Transform(handle, Data, Size, 1, &dstBuf3, &dstSize3, &transform);
    }

    // Cleanup
    if (dstBuf) tj3Free(dstBuf);
    if (dstBuf2) tj3Free(dstBuf2);
    if (dstBuf3) tj3Free(dstBuf3);
    tj3Destroy(handle);

    return 0;
}