// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3EncodeYUV8 at turbojpeg.c:1688:15 in turbojpeg.h
// tj3CompressFromYUV8 at turbojpeg.c:1429:15 in turbojpeg.h
// tj3DecodeYUV8 at turbojpeg.c:2675:15 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
// tj3JPEGBufSize at turbojpeg.c:903:18 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "turbojpeg.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static tjhandle create_compressor() {
    return tj3Init(TJINIT_COMPRESS);
}

static tjhandle create_decompressor() {
    return tj3Init(TJINIT_DECOMPRESS);
}

static void cleanup_handle(tjhandle handle) {
    if (handle) {
        tj3Destroy(handle);
    }
}

static void cleanup_buffer(unsigned char* buf) {
    if (buf) {
        tj3Free(buf);
    }
}

int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size) {
    if (Size < 32) return 0;

    // Extract parameters from fuzzer input
    int width = (Data[0] << 8) | Data[1];
    int height = (Data[2] << 8) | Data[3];
    int align = 1 << (Data[4] & 0x7); // Power of 2, 1-128
    int subsamp = Data[5] % 6; // 0-5 for TJSAMP values
    int pixelFormat = Data[6] % 12; // 0-11 for TJPF values
    int param = Data[7] % 13; // 0-12 for TJPARAM values
    int value = Data[8];
    
    // Ensure reasonable dimensions
    if (width <= 0) width = 16;
    if (height <= 0) height = 16;
    if (width > 4096) width = 4096;
    if (height > 4096) height = 4096;
    if (align < 1) align = 1;
    if (align > 128) align = 128;

    // Test tj3YUVBufSize
    size_t yuvBufSize = tj3YUVBufSize(width, align, height, subsamp);
    if (yuvBufSize == 0) return 0;

    // Test tj3JPEGBufSize
    size_t jpegBufSize = tj3JPEGBufSize(width, height, subsamp);
    if (jpegBufSize == 0) return 0;

    // Create handles
    tjhandle compHandle = create_compressor();
    tjhandle decompHandle = create_decompressor();
    if (!compHandle || !decompHandle) {
        cleanup_handle(compHandle);
        cleanup_handle(decompHandle);
        return 0;
    }

    // Test tj3Set
    tj3Set(compHandle, param, value);
    tj3Set(decompHandle, param, value);

    // Allocate buffers
    unsigned char* yuvBuf = (unsigned char*)tj3Alloc(yuvBufSize);
    unsigned char* rgbSrcBuf = (unsigned char*)tj3Alloc(width * height * 4);
    unsigned char* rgbDstBuf = (unsigned char*)tj3Alloc(width * height * 4);
    unsigned char* jpegBuf = NULL;
    size_t jpegSize = jpegBufSize;

    if (!yuvBuf || !rgbSrcBuf || !rgbDstBuf) {
        cleanup_buffer(yuvBuf);
        cleanup_buffer(rgbSrcBuf);
        cleanup_buffer(rgbDstBuf);
        cleanup_handle(compHandle);
        cleanup_handle(decompHandle);
        return 0;
    }

    // Initialize buffers with fuzzer data
    size_t dataToCopy = Size - 9;
    if (dataToCopy > 0) {
        size_t yuvCopySize = dataToCopy > yuvBufSize ? yuvBufSize : dataToCopy;
        memcpy(yuvBuf, Data + 9, yuvCopySize);
        
        size_t rgbCopySize = dataToCopy > (width * height * 4) ? 
                            (width * height * 4) : dataToCopy;
        memcpy(rgbSrcBuf, Data + 9, rgbCopySize);
    }

    // Test tj3EncodeYUV8
    tj3EncodeYUV8(compHandle, rgbSrcBuf, width, 0, height, 
                  pixelFormat, yuvBuf, align);

    // Test tj3CompressFromYUV8
    tj3CompressFromYUV8(compHandle, yuvBuf, width, align, height, 
                        &jpegBuf, &jpegSize);

    // Test tj3DecodeYUV8
    tj3DecodeYUV8(decompHandle, yuvBuf, align, rgbDstBuf, 
                  width, 0, height, pixelFormat);

    // Cleanup
    cleanup_buffer(yuvBuf);
    cleanup_buffer(rgbSrcBuf);
    cleanup_buffer(rgbDstBuf);
    cleanup_buffer(jpegBuf);
    cleanup_handle(compHandle);
    cleanup_handle(decompHandle);

    return 0;
}