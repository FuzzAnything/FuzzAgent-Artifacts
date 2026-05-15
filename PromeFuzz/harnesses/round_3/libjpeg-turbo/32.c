// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjLoadImage at turbojpeg.c:3104:26 in turbojpeg.h
// tjSaveImage at turbojpeg.c:3125:15 in turbojpeg.h
// tjDecodeYUVPlanes at turbojpeg.c:2649:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Compress16 at turbojpeg-mp.c:71:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3LoadImage16 at turbojpeg-mp.c:293:21 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "turbojpeg.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static int write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) return 0;
    size_t written = fwrite(data, 1, size, f);
    fclose(f);
    return written == size;
}

static void cleanup_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size) {
    if (Size < 64) return 0;
    
    // Initialize handles
    tjhandle comp_handle = tj3Init(TJINIT_COMPRESS);
    tjhandle decomp_handle = tj3Init(TJINIT_DECOMPRESS);
    if (!comp_handle || !decomp_handle) {
        if (comp_handle) tj3Destroy(comp_handle);
        if (decomp_handle) tj3Destroy(decomp_handle);
        return 0;
    }
    
    // Extract parameters from fuzzer input
    size_t offset = 0;
    int width = (Data[offset] << 8) | Data[offset + 1];
    offset += 2;
    int height = (Data[offset] << 8) | Data[offset + 1];
    offset += 2;
    int pixelFormat = Data[offset] % 12; // TJPF values 0-11
    offset += 1;
    int align = 1 << (Data[offset] % 5); // 1,2,4,8,16
    offset += 1;
    int subsamp = Data[offset] % 4; // TJCS values 0-3
    offset += 1;
    int flags = Data[offset] & 0xFF;
    offset += 1;
    int param = Data[offset] % 32; // TJPARAM values
    offset += 1;
    
    // Ensure reasonable dimensions
    if (width <= 0) width = 16;
    if (height <= 0) height = 16;
    if (width > 4096) width = 4096;
    if (height > 4096) height = 4096;
    
    // Calculate buffer sizes
    int pixelSize = tjPixelSize[pixelFormat];
    size_t srcBufSize = width * height * pixelSize * sizeof(unsigned short);
    size_t yuvBufSize = width * height * 3 / 2;
    
    // Ensure we have enough data
    if (Size - offset < srcBufSize + yuvBufSize + 1024) {
        tj3Destroy(comp_handle);
        tj3Destroy(decomp_handle);
        return 0;
    }
    
    // Test tj3Compress16
    unsigned short *srcBuf16 = (unsigned short*)&Data[offset];
    offset += srcBufSize;
    
    unsigned char *jpegBuf = NULL;
    size_t jpegSize = 0;
    int pitch = width * pixelSize;
    
    tj3Set(comp_handle, TJPARAM_PRECISION, 16);
    tj3Set(comp_handle, TJPARAM_SUBSAMP, subsamp);
    
    int ret = tj3Compress16(comp_handle, srcBuf16, width, pitch, height, 
                           pixelFormat, &jpegBuf, &jpegSize);
    if (jpegBuf) tj3Free(jpegBuf);
    
    // Test tj3LoadImage16
    if (write_dummy_file(&Data[offset], Size - offset)) {
        int loadedWidth, loadedHeight, loadedPixelFormat;
        loadedPixelFormat = pixelFormat;
        
        unsigned short *loadedBuf = tj3LoadImage16(comp_handle, "./dummy_file", 
                                                   &loadedWidth, align, 
                                                   &loadedHeight, &loadedPixelFormat);
        if (loadedBuf) tj3Free(loadedBuf);
        cleanup_dummy_file();
    }
    
    // Test tj3Get
    int value = tj3Get(comp_handle, param);
    (void)value; // Suppress unused warning
    
    // Test tjLoadImage
    if (write_dummy_file(&Data[offset], Size - offset)) {
        int imgWidth, imgHeight, imgPixelFormat;
        imgPixelFormat = pixelFormat;
        
        unsigned char *imgBuf = tjLoadImage("./dummy_file", &imgWidth, align, 
                                           &imgHeight, &imgPixelFormat, flags);
        if (imgBuf) free(imgBuf);
        
        // Test tjSaveImage with the loaded image if we got one
        if (imgBuf) {
            tjSaveImage("./dummy_file", imgBuf, imgWidth, imgWidth * tjPixelSize[imgPixelFormat], 
                       imgHeight, imgPixelFormat, flags);
        }
        cleanup_dummy_file();
    }
    
    // Test tjDecodeYUVPlanes
    const unsigned char *srcPlanes[3];
    int strides[3] = {width, width/2, width/2};
    
    size_t planeSizes[3] = {width * height, width * height / 4, width * height / 4};
    if (Size - offset >= planeSizes[0] + planeSizes[1] + planeSizes[2]) {
        srcPlanes[0] = &Data[offset];
        offset += planeSizes[0];
        srcPlanes[1] = &Data[offset];
        offset += planeSizes[1];
        srcPlanes[2] = &Data[offset];
        offset += planeSizes[2];
        
        unsigned char *dstBuf = (unsigned char*)malloc(width * height * pixelSize);
        if (dstBuf) {
            tjDecodeYUVPlanes(decomp_handle, srcPlanes, strides, subsamp,
                             dstBuf, width, width * pixelSize, height, 
                             pixelFormat, flags);
            free(dstBuf);
        }
    }
    
    // Cleanup
    tj3Destroy(comp_handle);
    tj3Destroy(decomp_handle);
    
    return 0;
}