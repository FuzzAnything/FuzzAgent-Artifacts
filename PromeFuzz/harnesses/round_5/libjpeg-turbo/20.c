// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjBufSizeYUV2 at turbojpeg.c:999:25 in turbojpeg.h
// tjCompressFromYUV at turbojpeg.c:1476:15 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3YUVPlaneWidth at turbojpeg.c:1057:15 in turbojpeg.h
// tj3YUVPlaneWidth at turbojpeg.c:1057:15 in turbojpeg.h
// tj3YUVPlaneWidth at turbojpeg.c:1057:15 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tj3CompressFromYUVPlanes8 at turbojpeg.c:1259:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tjPlaneWidth at turbojpeg.c:1083:15 in turbojpeg.h
// tjPlaneWidth at turbojpeg.c:1083:15 in turbojpeg.h
// tjPlaneWidth at turbojpeg.c:1083:15 in turbojpeg.h
// tjPlaneSizeYUV at turbojpeg.c:1048:25 in turbojpeg.h
// tjPlaneSizeYUV at turbojpeg.c:1048:25 in turbojpeg.h
// tjPlaneSizeYUV at turbojpeg.c:1048:25 in turbojpeg.h
// tjCompressFromYUVPlanes at turbojpeg.c:1394:15 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
// tj3CompressFromYUV8 at turbojpeg.c:1429:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3YUVPlaneWidth at turbojpeg.c:1057:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "src/turbojpeg.h"

static int read_int(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + sizeof(int) > size) return 1;
    int value = *(const int *)(data + *offset);
    *offset += sizeof(int);
    return value;
}

static size_t read_size_t(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + sizeof(size_t) > size) return 1;
    size_t value = *(const size_t *)(data + *offset);
    *offset += sizeof(size_t);
    return value;
}

static unsigned long read_ulong(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + sizeof(unsigned long) > size) return 1;
    unsigned long value = *(const unsigned long *)(data + *offset);
    *offset += sizeof(unsigned long);
    return value;
}

int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    size_t offset = 0;
    int function_selector = Data[offset++] % 6;
    
    tjhandle handle = tj3Init(TJINIT_COMPRESS);
    if (!handle) return 0;
    
    int width = 0, height = 0, align = 0, subsamp = 0, jpegQual = 0, flags = 0;
    size_t jpegSize = 0;
    unsigned long jpegSizeOld = 0;
    unsigned char *jpegBuf = NULL;
    unsigned char *jpegBufOld = NULL;
    
    switch (function_selector) {
        case 0: { // tj3CompressFromYUV8
            if (Size - offset < 4 * sizeof(int)) break;
            width = read_int(Data, Size, &offset);
            height = read_int(Data, Size, &offset);
            align = read_int(Data, Size, &offset);
            subsamp = read_int(Data, Size, &offset);
            
            width = abs(width) % 4096 + 1;
            height = abs(height) % 4096 + 1;
            align = (align & 0x0F) | 1;
            subsamp = abs(subsamp) % 6;
            
            tj3Set(handle, TJPARAM_SUBSAMP, subsamp);
            
            size_t yuvSize = tj3YUVBufSize(width, align, height, subsamp);
            if (yuvSize == 0) break;
            
            unsigned char *srcBuf = (unsigned char *)malloc(yuvSize);
            if (!srcBuf) break;
            
            size_t copy_size = Size - offset;
            if (copy_size > yuvSize) copy_size = yuvSize;
            if (copy_size > 0) memcpy(srcBuf, Data + offset, copy_size);
            
            tj3CompressFromYUV8(handle, srcBuf, width, align, height, &jpegBuf, &jpegSize);
            
            free(srcBuf);
            if (jpegBuf) tj3Free(jpegBuf);
            break;
        }
        
        case 1: { // tj3YUVPlaneWidth
            if (Size - offset < 3 * sizeof(int)) break;
            int componentID = read_int(Data, Size, &offset);
            width = read_int(Data, Size, &offset);
            subsamp = read_int(Data, Size, &offset);
            
            componentID = abs(componentID) % 3;
            width = abs(width) % 4096 + 1;
            subsamp = abs(subsamp) % 6;
            
            tj3YUVPlaneWidth(componentID, width, subsamp);
            break;
        }
        
        case 2: { // tjCompressFromYUV
            if (Size - offset < 6 * sizeof(int) + sizeof(unsigned long)) break;
            width = read_int(Data, Size, &offset);
            height = read_int(Data, Size, &offset);
            align = read_int(Data, Size, &offset);
            subsamp = read_int(Data, Size, &offset);
            jpegQual = read_int(Data, Size, &offset);
            flags = read_int(Data, Size, &offset);
            jpegSizeOld = read_ulong(Data, Size, &offset);
            
            width = abs(width) % 4096 + 1;
            height = abs(height) % 4096 + 1;
            align = (align & 0x0F) | 1;
            subsamp = abs(subsamp) % 6;
            jpegQual = (abs(jpegQual) % 100) + 1;
            flags &= TJFLAG_BOTTOMUP | TJFLAG_FASTUPSAMPLE | TJFLAG_FASTDCT | TJFLAG_ACCURATEDCT;
            
            size_t yuvSize = tjBufSizeYUV2(width, align, height, subsamp);
            if (yuvSize == 0) break;
            
            unsigned char *srcBuf = (unsigned char *)malloc(yuvSize);
            if (!srcBuf) break;
            
            size_t copy_size = Size - offset;
            if (copy_size > yuvSize) copy_size = yuvSize;
            if (copy_size > 0) memcpy(srcBuf, Data + offset, copy_size);
            
            tjCompressFromYUV(handle, srcBuf, width, align, height, subsamp,
                             &jpegBufOld, &jpegSizeOld, jpegQual, flags);
            
            free(srcBuf);
            if (jpegBufOld) tjFree(jpegBufOld);
            break;
        }
        
        case 3: { // tj3YUVPlaneSize
            if (Size - offset < 4 * sizeof(int) + sizeof(size_t)) break;
            int componentID = read_int(Data, Size, &offset);
            width = read_int(Data, Size, &offset);
            int stride = read_int(Data, Size, &offset);
            height = read_int(Data, Size, &offset);
            subsamp = read_int(Data, Size, &offset);
            
            componentID = abs(componentID) % 3;
            width = abs(width) % 4096 + 1;
            stride = abs(stride) % 4096;
            height = abs(height) % 4096 + 1;
            subsamp = abs(subsamp) % 6;
            
            tj3YUVPlaneSize(componentID, width, stride, height, subsamp);
            break;
        }
        
        case 4: { // tj3CompressFromYUVPlanes8
            if (Size - offset < 3 * sizeof(int)) break;
            width = read_int(Data, Size, &offset);
            height = read_int(Data, Size, &offset);
            subsamp = read_int(Data, Size, &offset);
            
            width = abs(width) % 4096 + 1;
            height = abs(height) % 4096 + 1;
            subsamp = abs(subsamp) % 6;
            
            tj3Set(handle, TJPARAM_SUBSAMP, subsamp);
            
            int yPlaneWidth = tj3YUVPlaneWidth(0, width, subsamp);
            int uPlaneWidth = tj3YUVPlaneWidth(1, width, subsamp);
            int vPlaneWidth = tj3YUVPlaneWidth(2, width, subsamp);
            
            size_t yPlaneSize = tj3YUVPlaneSize(0, width, 0, height, subsamp);
            size_t uPlaneSize = tj3YUVPlaneSize(1, width, 0, height, subsamp);
            size_t vPlaneSize = tj3YUVPlaneSize(2, width, 0, height, subsamp);
            
            if (yPlaneSize == 0 || uPlaneSize == 0 || vPlaneSize == 0) break;
            
            unsigned char *yPlane = (unsigned char *)malloc(yPlaneSize);
            unsigned char *uPlane = (unsigned char *)malloc(uPlaneSize);
            unsigned char *vPlane = (unsigned char *)malloc(vPlaneSize);
            
            if (!yPlane || !uPlane || !vPlane) {
                free(yPlane); free(uPlane); free(vPlane);
                break;
            }
            
            size_t remaining = Size - offset;
            if (remaining > 0) {
                size_t y_copy = (remaining > yPlaneSize) ? yPlaneSize : remaining;
                memcpy(yPlane, Data + offset, y_copy);
                offset += y_copy;
                remaining = Size - offset;
            }
            
            if (remaining > 0) {
                size_t u_copy = (remaining > uPlaneSize) ? uPlaneSize : remaining;
                memcpy(uPlane, Data + offset, u_copy);
                offset += u_copy;
                remaining = Size - offset;
            }
            
            if (remaining > 0) {
                size_t v_copy = (remaining > vPlaneSize) ? vPlaneSize : remaining;
                memcpy(vPlane, Data + offset, v_copy);
            }
            
            const unsigned char *srcPlanes[3] = {yPlane, uPlane, vPlane};
            tj3CompressFromYUVPlanes8(handle, srcPlanes, width, NULL, height, &jpegBuf, &jpegSize);
            
            free(yPlane); free(uPlane); free(vPlane);
            if (jpegBuf) tj3Free(jpegBuf);
            break;
        }
        
        case 5: { // tjCompressFromYUVPlanes
            if (Size - offset < 6 * sizeof(int) + sizeof(unsigned long)) break;
            width = read_int(Data, Size, &offset);
            height = read_int(Data, Size, &offset);
            subsamp = read_int(Data, Size, &offset);
            jpegQual = read_int(Data, Size, &offset);
            flags = read_int(Data, Size, &offset);
            jpegSizeOld = read_ulong(Data, Size, &offset);
            
            width = abs(width) % 4096 + 1;
            height = abs(height) % 4096 + 1;
            subsamp = abs(subsamp) % 6;
            jpegQual = (abs(jpegQual) % 100) + 1;
            flags &= TJFLAG_BOTTOMUP | TJFLAG_FASTUPSAMPLE | TJFLAG_FASTDCT | TJFLAG_ACCURATEDCT;
            
            int yPlaneWidth = tjPlaneWidth(0, width, subsamp);
            int uPlaneWidth = tjPlaneWidth(1, width, subsamp);
            int vPlaneWidth = tjPlaneWidth(2, width, subsamp);
            
            size_t yPlaneSize = tjPlaneSizeYUV(0, width, 0, height, subsamp);
            size_t uPlaneSize = tjPlaneSizeYUV(1, width, 0, height, subsamp);
            size_t vPlaneSize = tjPlaneSizeYUV(2, width, 0, height, subsamp);
            
            if (yPlaneSize == 0 || uPlaneSize == 0 || vPlaneSize == 0) break;
            
            unsigned char *yPlane = (unsigned char *)malloc(yPlaneSize);
            unsigned char *uPlane = (unsigned char *)malloc(uPlaneSize);
            unsigned char *vPlane = (unsigned char *)malloc(vPlaneSize);
            
            if (!yPlane || !uPlane || !vPlane) {
                free(yPlane); free(uPlane); free(vPlane);
                break;
            }
            
            size_t remaining = Size - offset;
            if (remaining > 0) {
                size_t y_copy = (remaining > yPlaneSize) ? yPlaneSize : remaining;
                memcpy(yPlane, Data + offset, y_copy);
                offset += y_copy;
                remaining = Size - offset;
            }
            
            if (remaining > 0) {
                size_t u_copy = (remaining > uPlaneSize) ? uPlaneSize : remaining;
                memcpy(uPlane, Data + offset, u_copy);
                offset += u_copy;
                remaining = Size - offset;
            }
            
            if (remaining > 0) {
                size_t v_copy = (remaining > vPlaneSize) ? vPlaneSize : remaining;
                memcpy(vPlane, Data + offset, v_copy);
            }
            
            const unsigned char *srcPlanes[3] = {yPlane, uPlane, vPlane};
            tjCompressFromYUVPlanes(handle, srcPlanes, width, NULL, height, subsamp,
                                   &jpegBufOld, &jpegSizeOld, jpegQual, flags);
            
            free(yPlane); free(uPlane); free(vPlane);
            if (jpegBufOld) tjFree(jpegBufOld);
            break;
        }
    }
    
    tj3Destroy(handle);
    return 0;
}