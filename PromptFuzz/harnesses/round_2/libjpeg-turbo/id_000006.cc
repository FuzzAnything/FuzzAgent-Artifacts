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
//<ID> 42
//<Prompt> []
/*<Combination>: [
*/
//<score> 19.666666, nr_unique_branch: 0
//<Quality> {"density":59,"unique_branches":{},"library_calls":["tjInitDecompress","tj3GetScalingFactors","tjDestroy","tjDecompressHeader3","tjDestroy","tjDestroy","tj3YUVBufSize","tjDestroy","tjAlloc","tjDestroy","tjDecompressToYUV2","tjFree","tjDestroy","tj3YUVPlaneWidth","tj3YUVPlaneHeight","tjAlloc","tjFree","tjDestroy","tjDecodeYUVPlanes","tjFree","tjFree","tjDestroy","tjInitCompress","tjFree","tjFree","tjDestroy","tj3YUVBufSize","tjAlloc","tjDestroy","tjFree","tjFree","tjDestroy","tj3InitVersion","tjFree","tjDestroy","tjFree","tjFree","tjDestroy","tj3EncodeYUV8","tj3Destroy","tjFree","tjDestroy","tjFree","tjFree","tjDestroy","tj3JPEGBufSize","tjCompressFromYUV","tj3Destroy","tjFree","tjDestroy","tjFree","tjFree","tjDestroy","tjFree","tj3Destroy","tjFree","tjDestroy","tjFree","tjFree","tjDestroy"],"critical_calls":["tjInitDecompress","tj3GetScalingFactors","tjDecompressHeader3","tj3YUVBufSize","tjAlloc","tjDecompressToYUV2","tj3YUVPlaneWidth","tj3YUVPlaneHeight","tjAlloc","tjDecodeYUVPlanes","tjInitCompress","tj3YUVBufSize","tjAlloc","tj3InitVersion","tj3EncodeYUV8","tj3JPEGBufSize","tjCompressFromYUV","tjFree","tj3Destroy","tjFree","tjDestroy","tjFree","tjFree","tjDestroy"],"visited":2}
/*Here's a C++ fuzz driver that uses the specified libjpeg-turbo APIs to process JPEG input data:

*/


extern "C" int LLVMFuzzerTestOneInput_6(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimum data for JPEG
    
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create decompressor handle using old API
    tjhandle decompressor = tjInitDecompress();
    if (!decompressor) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Get scaling factors (new API)
    int numScalingFactors;
    tjscalingfactor *scalingFactors = tj3GetScalingFactors(&numScalingFactors);
    if (!scalingFactors || numScalingFactors <= 0) {
        tjDestroy(decompressor);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Read JPEG dimensions
    int width, height, jpegSubsamp, jpegColorspace;
    unsigned long jpegSize = size;
    
    // Decompress header using old API
    if (tjDecompressHeader3(decompressor, data, jpegSize, 
                            &width, &height, &jpegSubsamp, &jpegColorspace) != 0) {
        tjDestroy(decompressor);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    if (width <= 0 || height <= 0) {
        tjDestroy(decompressor);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Calculate YUV buffer size
    int align = 4;  // Common alignment
    size_t yuvBufSize = tj3YUVBufSize(width, align, height, jpegSubsamp);
    if (yuvBufSize == 0) {
        tjDestroy(decompressor);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Allocate buffers using old API
    unsigned char *yuvBuffer = (unsigned char *)tjAlloc(yuvBufSize);
    if (!yuvBuffer) {
        tjDestroy(decompressor);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Decompress to YUV using old API
    if (tjDecompressToYUV2(decompressor, data, jpegSize, 
                           yuvBuffer, width, align, height, 0) != 0) {
        tjFree(yuvBuffer);
        tjDestroy(decompressor);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Calculate plane dimensions (new API)
    int planeWidth = tj3YUVPlaneWidth(0, width, jpegSubsamp);
    int planeHeight = tj3YUVPlaneHeight(0, height, jpegSubsamp);
    
    // Prepare for YUV plane decoding
    const unsigned char *srcPlanes[3];
    int strides[3];
    
    // Calculate strides for each plane
    for (int i = 0; i < 3; i++) {
        srcPlanes[i] = yuvBuffer + (i * planeWidth * planeHeight);
        strides[i] = planeWidth;
    }
    
    // Allocate RGB buffer
    int pixelFormat = TJPF_RGB;
    int rgbPitch = width * 3;  // 3 bytes per pixel for RGB
    unsigned char *rgbBuffer = (unsigned char *)tjAlloc(width * height * 3);
    if (!rgbBuffer) {
        tjFree(yuvBuffer);
        tjDestroy(decompressor);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Decode YUV planes to RGB using old API
    if (tjDecodeYUVPlanes(decompressor, srcPlanes, strides, 
                          jpegSubsamp, rgbBuffer, width, rgbPitch, 
                          height, pixelFormat, 0) != 0) {
        tjFree(rgbBuffer);
        tjFree(yuvBuffer);
        tjDestroy(decompressor);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create compressor handle using old API
    tjhandle compressor = tjInitCompress();
    if (!compressor) {
        tjFree(rgbBuffer);
        tjFree(yuvBuffer);
        tjDestroy(decompressor);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Allocate destination YUV buffer for encoding
    size_t encYuvSize = tj3YUVBufSize(width, align, height, jpegSubsamp);
    unsigned char *encYuvBuffer = (unsigned char *)tjAlloc(encYuvSize);
    if (!encYuvBuffer) {
        tjDestroy(compressor);
        tjFree(rgbBuffer);
        tjFree(yuvBuffer);
        tjDestroy(decompressor);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Encode RGB to YUV using new API (needs new handle)
    tjhandle encoder = tj3Init(TJINIT_COMPRESS);
    if (!encoder) {
        tjFree(encYuvBuffer);
        tjDestroy(compressor);
        tjFree(rgbBuffer);
        tjFree(yuvBuffer);
        tjDestroy(decompressor);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Use new API for YUV encoding
    if (tj3EncodeYUV8(encoder, rgbBuffer, width, rgbPitch, height, 
                      pixelFormat, encYuvBuffer, align) != 0) {
        tj3Destroy(encoder);
        tjFree(encYuvBuffer);
        tjDestroy(compressor);
        tjFree(rgbBuffer);
        tjFree(yuvBuffer);
        tjDestroy(decompressor);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Allocate JPEG buffer using new API size calculation
    size_t jpegBufSize = tj3JPEGBufSize(width, height, jpegSubsamp);
    unsigned char *jpegBuf = nullptr;
    unsigned long finalJpegSize = 0;
    
    // Compress from YUV to JPEG using old API
    if (tjCompressFromYUV(compressor, encYuvBuffer, width, align, 
                          height, jpegSubsamp, &jpegBuf, 
                          &finalJpegSize, 85, 0) != 0) {
        tj3Destroy(encoder);
        tjFree(encYuvBuffer);
        tjDestroy(compressor);
        tjFree(rgbBuffer);
        tjFree(yuvBuffer);
        tjDestroy(decompressor);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Verify buffer size matches calculation
    if (finalJpegSize > jpegBufSize) {
        // Handle size mismatch
    }
    
    // Write output to file
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        fwrite(jpegBuf, 1, finalJpegSize, out_file);
        assert_file_closed(&out_file);;
    }
    
    // Clean up all allocated resources
    if (jpegBuf) tjFree(jpegBuf);
    tj3Destroy(encoder);
    tjFree(encYuvBuffer);
    tjDestroy(compressor);
    tjFree(rgbBuffer);
    tjFree(yuvBuffer);
    tjDestroy(decompressor);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}