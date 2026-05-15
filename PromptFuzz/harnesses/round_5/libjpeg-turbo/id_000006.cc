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
//<ID> 37
//<Prompt> []
/*<Combination>: [
*/
//<score> 22, nr_unique_branch: 1
//<Quality> {"density":33,"unique_branches":{"tj3EncodeYUV8":[[1749,7,1749,38,0,0,4,0]]},"library_calls":["tj3InitVersion","tjLoadImage","tjLoadImage","tj3Destroy","tj3YUVBufSize","tj3Alloc","tjFree","tj3Destroy","tj3EncodeYUV8","tj3Free","tjFree","tj3Destroy","tj3YUVPlaneWidth","tj3YUVPlaneHeight","tj3YUVPlaneWidth","tj3YUVPlaneHeight","tj3Alloc","tj3Free","tjFree","tj3Destroy","tj3DecodeYUVPlanes8","tj3Free","tj3Free","tjFree","tj3Destroy","tj3SaveImage8","tj3Alloc","tj3SaveImage16","tj3Free","tj3Alloc","tj3SaveImage12","tj3Free","tj3Free","tj3Free","tjFree","tj3Destroy"],"critical_calls":["tj3InitVersion","tjLoadImage","tjLoadImage","tj3YUVBufSize","tj3Alloc","tj3EncodeYUV8","tj3YUVPlaneWidth","tj3YUVPlaneHeight","tj3YUVPlaneWidth","tj3YUVPlaneHeight","tj3Alloc","tj3DecodeYUVPlanes8","tj3SaveImage8","tj3Alloc","tj3SaveImage16","tj3Free","tj3Alloc","tj3SaveImage12","tj3Free","tj3Free","tj3Free","tjFree","tj3Destroy"],"visited":2}
/**/


extern "C" int LLVMFuzzerTestOneInput_6(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Create temporary files for input/output
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Initialize TurboJPEG handle
    tjhandle handle = tj3Init(TJINIT_COMPRESS);  // Using compress mode since we'll encode
    if (!handle) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Step 1: Load input image using tjLoadImage
    int width, height, pixelFormat;
    unsigned char *srcBuf = tjLoadImage("input_file", &width, 4, &height, &pixelFormat, 0);
    
    // If loading from file fails, try to decode the input data as JPEG
    if (!srcBuf) {
        // Reset file pointer
        fseek(in_file, 0, SEEK_SET);
        
        // Write input data to file for tjLoadImage
        FILE *tmp_file = fopen("input_file", "wb");
        if (tmp_file) {
            fwrite(data, 1, size, tmp_file);
            assert_file_closed(&tmp_file);;
            
            // Try loading again
            srcBuf = tjLoadImage("input_file", &width, 4, &height, &pixelFormat, 0);
        }
    }
    
    if (!srcBuf) {
        tj3Destroy(handle);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // Calculate pitch (bytes per row)
    int pitch = width * tjPixelSize[pixelFormat];
    
    // Step 2: Encode to YUV using tj3EncodeYUV8
    size_t yuvSize = tj3YUVBufSize(width, 4, height, TJSAMP_420);
    unsigned char *yuvBuf = (unsigned char *)tj3Alloc(yuvSize);
    if (!yuvBuf) {
        tjFree(srcBuf);
        tj3Destroy(handle);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    
    int result = tj3EncodeYUV8(handle, srcBuf, width, pitch, height, pixelFormat, yuvBuf, 4);
    if (result != 0) {
        tj3Free(yuvBuf);
        tjFree(srcBuf);
        tj3Destroy(handle);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // Step 3: Decode YUV planes back to RGB using tj3DecodeYUVPlanes8
    // Prepare YUV planes from contiguous buffer
    int strides[3];
    const unsigned char *srcPlanes[3];
    
    // For YUV420, calculate plane sizes and pointers
    int yWidth = tj3YUVPlaneWidth(0, width, TJSAMP_420);
    int yHeight = tj3YUVPlaneHeight(0, height, TJSAMP_420);
    int uvWidth = tj3YUVPlaneWidth(1, width, TJSAMP_420);
    int uvHeight = tj3YUVPlaneHeight(1, height, TJSAMP_420);
    
    strides[0] = yWidth;
    strides[1] = uvWidth;
    strides[2] = uvWidth;
    
    srcPlanes[0] = yuvBuf;  // Y plane
    srcPlanes[1] = yuvBuf + (yWidth * yHeight);  // U plane
    srcPlanes[2] = srcPlanes[1] + (uvWidth * uvHeight);  // V plane
    
    // Allocate destination buffer for decoded image
    unsigned char *dstBuf = (unsigned char *)tj3Alloc(pitch * height);
    if (!dstBuf) {
        tj3Free(yuvBuf);
        tjFree(srcBuf);
        tj3Destroy(handle);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    
    result = tj3DecodeYUVPlanes8(handle, srcPlanes, strides, dstBuf, width, pitch, height, pixelFormat);
    if (result != 0) {
        tj3Free(dstBuf);
        tj3Free(yuvBuf);
        tjFree(srcBuf);
        tj3Destroy(handle);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // Step 4: Save images in different formats
    
    // Save as 8-bit image
    result = tj3SaveImage8(handle, "output8.ppm", dstBuf, width, pitch, height, pixelFormat);
    
    // Create 16-bit version by scaling 8-bit values
    unsigned short *buffer16 = (unsigned short *)tj3Alloc(width * height * tjPixelSize[pixelFormat] * 2);
    if (buffer16) {
        // Scale 8-bit to 16-bit (multiply by 257 for proper scaling)
        for (int i = 0; i < width * height * tjPixelSize[pixelFormat]; i++) {
            buffer16[i] = (unsigned short)(dstBuf[i] * 257);
        }
        result = tj3SaveImage16(handle, "output16.ppm", buffer16, width, width * 2, height, pixelFormat);
        tj3Free(buffer16);
    }
    
    // Create 12-bit version
    short *buffer12 = (short *)tj3Alloc(width * height * tjPixelSize[pixelFormat] * 2);
    if (buffer12) {
        // Scale 8-bit to 12-bit (multiply by 16, maximum 4095)
        for (int i = 0; i < width * height * tjPixelSize[pixelFormat]; i++) {
            buffer12[i] = (short)((dstBuf[i] * 16) & 0xFFF);
        }
        result = tj3SaveImage12(handle, "output12.ppm", buffer12, width, width * 2, height, pixelFormat);
        tj3Free(buffer12);
    }
    
    // Cleanup
    tj3Free(dstBuf);
    tj3Free(yuvBuf);
    tjFree(srcBuf);
    tj3Destroy(handle);
    assert_file_closed(&in_file);;
    
    // Remove temporary files
    remove("input_file");
    remove("output8.ppm");
    remove("output16.ppm");
    remove("output12.ppm");
    
    assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
}