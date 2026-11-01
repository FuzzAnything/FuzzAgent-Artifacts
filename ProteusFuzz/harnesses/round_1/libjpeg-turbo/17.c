// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjCompress at turbojpeg.c:1235:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// TJBUFSIZE at turbojpeg.c:948:25 in turbojpeg.h
// tjPlaneSizeYUV at turbojpeg.c:1048:25 in turbojpeg.h
// tj3YUVPlaneWidth at turbojpeg.c:1057:15 in turbojpeg.h
// tjBufSizeYUV2 at turbojpeg.c:999:25 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <turbojpeg.h>

static int extract_int(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + sizeof(int) > size) return 0;
    int val = *(int *)(data + *offset);
    *offset += sizeof(int);
    return val;
}

static int extract_uint(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + sizeof(unsigned int) > size) return 0;
    unsigned int val = *(unsigned int *)(data + *offset);
    *offset += sizeof(unsigned int);
    return val;
}

int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    size_t offset = 0;
    int width = extract_int(Data, Size, &offset);
    int height = extract_int(Data, Size, &offset);
    int subsamp = (Size > offset + 4) ? extract_int(Data, Size, &offset) : 0;
    int flags = (Size > offset + 4) ? extract_int(Data, Size, &offset) : 0;
    int quality = (Size > offset + 4) ? extract_int(Data, Size, &offset) : 85;
    int pixelSize = (Size > offset + 4) ? extract_int(Data, Size, &offset) : 3;
    int align = (Size > offset + 4) ? extract_int(Data, Size, &offset) : 1;
    int componentID = (Size > offset + 4) ? extract_int(Data, Size, &offset) : 0;
    int stride = (Size > offset + 4) ? extract_int(Data, Size, &offset) : 0;

    // Clamp values to reasonable ranges to avoid immediate crashes or huge allocations
    if (width < 0) width = 0;
    if (height < 0) height = 0;
    if (width > 65535) width = 65535;
    if (height > 65535) height = 65535;
    if (quality < 0) quality = 0;
    if (quality > 100) quality = 100;
    if (pixelSize < 0) pixelSize = 0;
    if (pixelSize > 4) pixelSize = 4;
    if (align < 0) align = 0;
    if (align > 16) align = 16;
    if (componentID < 0) componentID = 0;
    if (componentID > 2) componentID = 2;

    // 1. Test TJBUFSIZE
    unsigned long jpegBufSize = TJBUFSIZE(width, height);
    if (jpegBufSize == (unsigned long)-1) {
        // Invalid args, skip compression
    }

    // 2. Test tjPlaneSizeYUV
    unsigned long planeSizeYUV = tjPlaneSizeYUV(componentID, width, stride, height, subsamp);
    (void)planeSizeYUV;

    // 3. Test tj3YUVPlaneWidth
    int planeWidth = tj3YUVPlaneWidth(componentID, width, subsamp);
    (void)planeWidth;

    // 4. Test tjBufSizeYUV2
    unsigned long yuvBufSize = tjBufSizeYUV2(width, align, height, subsamp);
    if (yuvBufSize == (unsigned long)-1) {
        // Invalid args
    }

    // 5. Test tj3YUVPlaneSize
    size_t planeSize3 = tj3YUVPlaneSize(componentID, width, stride, height, subsamp);
    (void)planeSize3;

    // 6. Test tjCompress
    if (width > 0 && height > 0 && jpegBufSize != (unsigned long)-1 && yuvBufSize != (unsigned long)-1) {
        tjhandle handle = tjInitCompress();
        if (!handle) return 0;

        // Allocate source buffer (RGB or YUV depending on flags)
        unsigned long srcBufSize;
        if (flags & TJ_YUV) {
            srcBufSize = yuvBufSize;
        } else {
            srcBufSize = (unsigned long)width * height * pixelSize;
        }

        // Safety check to prevent huge allocations
        if (srcBufSize > 100 * 1024 * 1024) {
            tjDestroy(handle);
            return 0;
        }

        unsigned char *srcBuf = malloc(srcBufSize);
        unsigned char *dstBuf = malloc(jpegBufSize);

        if (!srcBuf || !dstBuf) {
            free(srcBuf);
            free(dstBuf);
            tjDestroy(handle);
            return 0;
        }

        // Fill source buffer with fuzz data
        if (Size > 0) {
            size_t copySize = (Size < srcBufSize) ? Size : srcBufSize;
            memcpy(srcBuf, Data, copySize);
            if (copySize < srcBufSize) {
                memset(srcBuf + copySize, 0, srcBufSize - copySize);
            }
        } else {
            memset(srcBuf, 0, srcBufSize);
        }

        int pitch = width * pixelSize;
        if (flags & TJ_YUV) {
            // For YUV, pitch logic might differ, but we use a safe approximation
            // In real usage, pitch for YUV is often planeWidth * bytesPerPixel
            // Here we just pass width * pixelSize or 0 if stride was used
            if (stride > 0) pitch = stride;
        }

        unsigned long compressedSize = 0;
        int ret = tjCompress(handle, srcBuf, width, pitch, height, pixelSize,
                             dstBuf, &compressedSize, subsamp, quality, flags);
        (void)ret;

        free(srcBuf);
        free(dstBuf);
        tjDestroy(handle);
    }

    return 0;
}