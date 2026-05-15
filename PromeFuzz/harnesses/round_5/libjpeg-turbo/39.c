// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjCompress2 at turbojpeg.c:1204:15 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Compress16 at turbojpeg-mp.c:71:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// TJBUFSIZE at turbojpeg.c:948:25 in turbojpeg.h
// tj3JPEGBufSize at turbojpeg.c:903:18 in turbojpeg.h
// tjBufSize at turbojpeg.c:933:25 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "turbojpeg.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int extract_int(const uint8_t **data, size_t *size, int min_val, int max_val) {
    if (*size < sizeof(int)) {
        return min_val;
    }
    int value = *(const int *)(*data);
    *data += sizeof(int);
    *size -= sizeof(int);
    
    if (value < min_val) value = min_val;
    if (value > max_val) value = max_val;
    return value;
}

static int extract_subsamp(const uint8_t **data, size_t *size) {
    if (*size == 0) {
        return TJSAMP_444;
    }
    int subsamp = **data % (TJSAMP_GRAY + 1);
    (*data)++;
    (*size)--;
    return subsamp;
}

static int extract_pixel_format(const uint8_t **data, size_t *size) {
    if (*size == 0) {
        return TJPF_RGB;
    }
    int format = **data % (TJPF_CMYK + 1);
    (*data)++;
    (*size)--;
    return format;
}

static int extract_quality(const uint8_t **data, size_t *size) {
    if (*size == 0) {
        return 85;
    }
    int quality = **data;
    (*data)++;
    (*size)--;
    if (quality < 1) quality = 1;
    if (quality > 100) quality = 100;
    return quality;
}

int LLVMFuzzerTestOneInput_39(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(int) * 2) {
        return 0;
    }
    
    const uint8_t *data_ptr = Data;
    size_t data_size = Size;
    
    int width = extract_int(&data_ptr, &data_size, 1, 4096);
    int height = extract_int(&data_ptr, &data_size, 1, 4096);
    int subsamp = extract_subsamp(&data_ptr, &data_size);
    int pixel_format = extract_pixel_format(&data_ptr, &data_size);
    int quality = extract_quality(&data_ptr, &data_size);
    
    unsigned long bufsize1 = TJBUFSIZE(width, height);
    size_t bufsize2 = tj3JPEGBufSize(width, height, subsamp);
    unsigned long bufsize3 = tjBufSize(width, height, subsamp);
    
    if (bufsize1 > 0 && bufsize1 < 1024 * 1024 * 1024) {
        void *alloc_buf = tj3Alloc(bufsize1);
        if (alloc_buf) {
            free(alloc_buf);
        }
    }
    
    if (bufsize2 > 0 && bufsize2 < 1024 * 1024 * 1024) {
        void *alloc_buf = tj3Alloc(bufsize2);
        if (alloc_buf) {
            free(alloc_buf);
        }
    }
    
    tjhandle handle = tjInitCompress();
    if (handle) {
        unsigned char *jpegBuf = NULL;
        unsigned long jpegSize = 0;
        
        size_t src_size = width * height * tjPixelSize[pixel_format];
        if (src_size > 0 && src_size <= data_size) {
            int pitch = width * tjPixelSize[pixel_format];
            
            int flags = 0;
            if (data_size > 0) {
                flags = *data_ptr % 4;
                data_ptr++;
                data_size--;
            }
            
            tjCompress2(handle, data_ptr, width, pitch, height, pixel_format,
                       &jpegBuf, &jpegSize, subsamp, quality, flags);
            
            if (jpegBuf) {
                tjFree(jpegBuf);
            }
        }
        
        tjDestroy(handle);
    }
    
    handle = tj3Init(TJINIT_COMPRESS);
    if (handle) {
        unsigned char *jpegBuf = NULL;
        size_t jpegSize = 0;
        
        size_t src_size = width * height * tjPixelSize[pixel_format];
        if (src_size > 0 && src_size * 2 <= data_size) {
            const unsigned short *src16 = (const unsigned short *)data_ptr;
            int pitch = width * tjPixelSize[pixel_format];
            
            tj3Set(handle, TJPARAM_SUBSAMP, subsamp);
            tj3Set(handle, TJPARAM_LOSSLESS, 1);
            
            tj3Compress16(handle, src16, width, pitch, height, pixel_format,
                         &jpegBuf, &jpegSize);
            
            if (jpegBuf) {
                tj3Free(jpegBuf);
            }
        }
        
        tj3Destroy(handle);
    }
    
    return 0;
}