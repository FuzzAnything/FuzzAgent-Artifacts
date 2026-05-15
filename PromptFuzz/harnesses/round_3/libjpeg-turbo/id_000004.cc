#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
extern "C" {
#include <turbojpeg.h>
}
//<ID> 35
//<Prompt> []
/*<Combination>: [
*/
//<score> 87.5, nr_unique_branch: 42
//<Quality> {"density":35,"unique_branches":{"jpeg_quality_scaling":[[139,7,139,19,0,0,4,0],[148,7,148,19,0,0,4,0]],"tj3EncodeYUVPlanes8":[[1592,7,1592,38,0,0,4,1],[1594,7,1594,31,0,0,4,1],[1597,7,1597,17,0,0,4,1],[1611,7,1611,42,0,0,4,1],[1622,7,1622,73,0,0,4,1],[1624,15,1624,25,0,0,4,0],[1624,15,1624,25,0,0,4,1],[1625,9,1625,23,0,0,4,1],[1630,7,1630,19,0,0,4,1],[1633,15,1633,40,0,0,4,0],[1633,15,1633,40,0,0,4,1],[1639,9,1639,20,0,0,4,1],[1643,9,1643,19,0,0,4,1],[1645,19,1645,49,0,0,4,0],[1645,19,1645,49,0,0,4,1],[1656,9,1656,21,0,0,4,1],[1659,9,1659,20,0,0,4,1],[1661,19,1661,47,0,0,4,0],[1661,19,1661,47,0,0,4,1],[1671,9,1671,19,0,0,4,1],[1674,19,1674,30,0,0,4,0],[1674,19,1674,30,0,0,4,1],[1676,15,1676,22,0,0,4,0],[1676,26,1676,41,0,0,4,0],[1682,17,1682,26,0,0,4,0],[1682,17,1682,26,0,0,4,1],[1686,45,1686,70,0,0,4,0],[1686,45,1686,70,0,0,4,1]],"add_huff_table":[[26,7,26,23,0,0,4,1],[28,12,28,34,0,0,4,1]],"jpeg_add_quant_table":[[60,27,60,38,0,0,4,0]],"tj3SaveImage12":[[340,7,340,12,2,0,4,0]],"tjDecompress2":[[2155,7,2155,41,0,0,4,0],[2156,7,2156,25,0,0,4,0],[2156,7,2156,25,0,0,4,1]],"jpeg_set_defaults":[[196,7,196,31,0,0,4,1]],"tjEncodeYUVPlanes":[[1720,7,1720,18,0,0,4,1],[1720,22,1720,46,0,0,4,1],[340,7,340,12,1,0,4,1]],"jpeg_read_header":[[289,9,289,22,0,0,4,0]]},"library_calls":["tjInitCompress","tjInitDecompress","tjDestroy","tjDestroy","tjDecompressHeader3","tjAlloc","tjDestroy","tjDestroy","tjDecompress2","tjGetErrorStr2","tj3Get","tjPlaneHeight","tjBufSizeYUV2","tjAlloc","tjEncodeYUV2","tjPlaneWidth","tjPlaneWidth","tjPlaneWidth","tjPlaneWidth","tjPlaneHeight","tjAlloc","tjEncodeYUVPlanes","tj3InitVersion","tj3Alloc","tj3DecodeYUVPlanes8","tj3Free","tj3Destroy","tjCompress2","tj3Alloc","tj3SaveImage12","tj3Free","tjFree","tjFree","tjFree","tjFree","tjDestroy","tjDestroy"],"critical_calls":["tjInitCompress","tjInitDecompress","tjDecompressHeader3","tjAlloc","tjDecompress2","tjGetErrorStr2","tj3Get","tjPlaneHeight","tjBufSizeYUV2","tjAlloc","tjEncodeYUV2","tjPlaneWidth","tjPlaneWidth","tjPlaneWidth","tjPlaneWidth","tjPlaneHeight","tjAlloc","tjEncodeYUVPlanes","tj3InitVersion","tj3Alloc","tj3DecodeYUVPlanes8","tj3Free","tj3Destroy","tjCompress2","tj3Alloc","tj3SaveImage12","tj3Free","tjFree","tjFree","tjFree","tjFree","tjDestroy","tjDestroy"],"visited":3}
/**/


extern "C" int LLVMFuzzerTestOneInput_4(const uint8_t* f_data, size_t f_size) {
	if(f_size<24) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
	//fuzzer shim end}




    // Create a file pointer from the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Initialize TurboJPEG handles
    tjhandle handle_compress = tjInitCompress();
    tjhandle handle_decompress = tjInitDecompress();
    if (!handle_compress || !handle_decompress) {
        assert_file_closed(&in_file);;
        if (handle_compress) tjDestroy(handle_compress);
        if (handle_decompress) tjDestroy(handle_decompress);
        assert_file_closed(&in_file);
	return 0;
    }

    // Read JPEG header from input data
    int width, height, jpegSubsamp, jpegColorspace;
    unsigned char *jpegBuf = (unsigned char *)data;
    unsigned long jpegSize = size;
    
    if (tjDecompressHeader3(handle_decompress, jpegBuf, jpegSize, 
                           &width, &height, &jpegSubsamp, &jpegColorspace) != 0) {
        // If header decompression fails, try to use the data as raw image data
        // For this example, we'll use a default size if header reading fails
        width = 64;
        height = 64;
        jpegSubsamp = TJSAMP_420;
    }

    // Allocate source buffer for encoding
    int pixelFormat = TJPF_RGB;
    int srcPitch = width * tjPixelSize[pixelFormat];
    unsigned char *srcBuf = (unsigned char *)tjAlloc(width * height * tjPixelSize[pixelFormat]);
    if (!srcBuf) {
        assert_file_closed(&in_file);;
        tjDestroy(handle_compress);
        tjDestroy(handle_decompress);
        assert_file_closed(&in_file);
	return 0;
    }

    // Try to decompress the input as JPEG
    if (tjDecompress2(handle_decompress, jpegBuf, jpegSize, srcBuf, 
                      width, srcPitch, height, pixelFormat, fuzz_int32_t_1) == 0) {
        // Successfully decompressed, now we can use the image data
    } else {
        // If decompression fails, fill with pattern
        for (int i = 0; i < width * height * tjPixelSize[pixelFormat]; i++) {
            srcBuf[i] = (i % 256);
        }
    }

    // Get error string using tjGetErrorStr2
    char *errorStr = tjGetErrorStr2(handle_decompress);
    // We don't use it but we call the API as required

    // Get a parameter using tj3Get (note: tj3Get is TurboJPEG 3.x API, we need to use appropriate handle)
    // For compatibility, we'll use the decompression handle
    int paramValue = tj3Get(handle_decompress, TJPARAM_SUBSAMP);
    // paramValue contains the subsampling parameter

    // Calculate plane height using tjPlaneHeight
    int planeHeight = tjPlaneHeight(fuzz_int32_t_2, height, jpegSubsamp);

    // Encode to YUV using tjEncodeYUV2
    size_t yuvBufSize = tjBufSizeYUV2(width, 4, height, jpegSubsamp);
    unsigned char *yuvBuf = (unsigned char *)tjAlloc(yuvBufSize);
    if (yuvBuf) {
        tjEncodeYUV2(handle_compress, srcBuf, width, srcPitch, height, 
                     pixelFormat, yuvBuf, jpegSubsamp, fuzz_int32_t_3);
    }

    // Prepare for YUV planes encoding
    int strides[3];
    unsigned char *dstPlanes[3];
    
    // Calculate strides and allocate planes
    strides[0] = tjPlaneWidth(0, width, jpegSubsamp);
    strides[1] = tjPlaneWidth(1, width, jpegSubsamp);
    strides[2] = tjPlaneWidth(2, width, jpegSubsamp);
    
    for (int i = 0; i < 3; i++) {
        int planeW = tjPlaneWidth(i, width, jpegSubsamp);
        int planeH = tjPlaneHeight(i, height, jpegSubsamp);
        dstPlanes[i] = (unsigned char *)tjAlloc(planeW * planeH);
    }

    // Encode to YUV planes using tjEncodeYUVPlanes
    if (dstPlanes[0] && dstPlanes[1] && dstPlanes[2]) {
        tjEncodeYUVPlanes(handle_compress, srcBuf, width, srcPitch, height,
                          pixelFormat, dstPlanes, strides, jpegSubsamp, fuzz_int32_t_4);
    }

    // Decode from YUV planes using tj3DecodeYUVPlanes8
    // First, we need a TurboJPEG 3.x handle
    tjhandle handle3 = tj3Init(TJINIT_DECOMPRESS);
    if (handle3) {
        unsigned char *decodedBuf = (unsigned char *)tj3Alloc(width * height * tjPixelSize[pixelFormat]);
        if (decodedBuf && dstPlanes[0] && dstPlanes[1] && dstPlanes[2]) {
            const unsigned char *srcPlanes[3] = {dstPlanes[0], dstPlanes[1], dstPlanes[2]};
            tj3DecodeYUVPlanes8(handle3, srcPlanes, strides, decodedBuf, 
                                width, srcPitch, height, pixelFormat);
            tj3Free(decodedBuf);
        }
        tj3Destroy(handle3);
    }

    // Compress using tjCompress2
    unsigned char *compressedBuf = nullptr;
    unsigned long compressedSize = 0;
    if (tjCompress2(handle_compress, srcBuf, width, srcPitch, height,
                    pixelFormat, &compressedBuf, &compressedSize, 
                    jpegSubsamp, fuzz_int32_t_5, fuzz_int32_t_6) == 0) {
        // Compression successful
    }

    // Save as 12-bit image using tj3SaveImage12
    // We need to convert 8-bit data to 12-bit
    short *buffer12 = (short *)tj3Alloc(width * height * sizeof(short));
    if (buffer12) {
        // Convert 8-bit to 12-bit by shifting
        for (int i = 0; i < width * height; i++) {
            buffer12[i] = srcBuf[i * tjPixelSize[pixelFormat]] << 4;
        }
        
        // Save as 12-bit image
        tj3SaveImage12(nullptr, "output_file", buffer12, width, width * sizeof(short), height, TJPF_GRAY);
        
        tj3Free(buffer12);
    }

    // Clean up
    assert_file_closed(&in_file);;
    
    // Free allocated buffers
    tjFree(srcBuf);
    if (yuvBuf) tjFree(yuvBuf);
    for (int i = 0; i < 3; i++) {
        if (dstPlanes[i]) tjFree(dstPlanes[i]);
    }
    if (compressedBuf) tjFree(compressedBuf);
    
    // Destroy handles
    tjDestroy(handle_compress);
    tjDestroy(handle_decompress);

    assert_file_closed(&in_file);
	return 0;
}