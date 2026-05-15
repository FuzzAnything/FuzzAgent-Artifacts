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
//<ID> 282
//<Prompt> []
/*<Combination>: [
*/
//<score> 44, nr_unique_branch: 0
//<Quality> {"density":44,"unique_branches":{},"library_calls":["tjInitDecompress","tjInitCompress","tjInitTransform","tj3InitVersion","tjDestroy","tjDestroy","tjDestroy","tj3Destroy","tjDecompressHeader3","tjDecompressHeader2","tjDestroy","tjDestroy","tjDestroy","tj3Destroy","tjAlloc","tjDecompress2","tjGetErrorCode","tjFree","tjBufSizeYUV","tjAlloc","tjFree","tjEncodeYUV","tjGetErrorCode","tjPlaneWidth","tjPlaneHeight","tjPlaneSizeYUV","tjAlloc","tjFree","tjFree","tjFree","tjEncodeYUVPlanes","tj3CompressFromYUV8","tjTransform","tjFree","tj3Alloc","tj3SaveImage16","tj3Free","tj3Free","tjPlaneWidth","tjFree","tjFree","tjFree","tjDestroy","tjDestroy","tjDestroy","tj3Destroy"],"critical_calls":["tjInitDecompress","tjInitCompress","tjInitTransform","tj3InitVersion","tjDecompressHeader3","tjDecompressHeader2","tjAlloc","tjDecompress2","tjGetErrorCode","tjBufSizeYUV","tjAlloc","tjEncodeYUV","tjGetErrorCode","tjPlaneWidth","tjPlaneHeight","tjPlaneSizeYUV","tjAlloc","tjEncodeYUVPlanes","tj3CompressFromYUV8","tjTransform","tjFree","tj3Alloc","tj3SaveImage16","tj3Free","tj3Free","tjPlaneWidth","tjFree","tjFree","tjFree","tjDestroy","tjDestroy","tjDestroy","tj3Destroy"],"visited":0}
/**/



extern "C" int LLVMFuzzerTestOneInput_20(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Too small to be meaningful
    
    // Initialize handles
    tjhandle handle_decompress = tjInitDecompress();
    tjhandle handle_compress = tjInitCompress();
    tjhandle handle_transform = tjInitTransform();
    tjhandle handle_new = tj3InitVersion(TJINIT_COMPRESS, 3000000);
    
    if (!handle_decompress || !handle_compress || !handle_transform || !handle_new) {
        if (handle_decompress) tjDestroy(handle_decompress);
        if (handle_compress) tjDestroy(handle_compress);
        if (handle_transform) tjDestroy(handle_transform);
        if (handle_new) tj3Destroy(handle_new);
        return 0;
    }
    
    // 1. First decompress the input JPEG using tjDecompress2
    int width, height, jpegSubsamp, jpegColorspace;
    
    // Try to get header info
    if (tjDecompressHeader3(handle_decompress, data, size, &width, &height, 
                           &jpegSubsamp, &jpegColorspace) != 0) {
        // If header fails, try with older API
        if (tjDecompressHeader2(handle_decompress, const_cast<unsigned char*>(data), 
                               size, &width, &height, &jpegSubsamp) != 0) {
            cleanup:
            tjDestroy(handle_decompress);
            tjDestroy(handle_compress);
            tjDestroy(handle_transform);
            tj3Destroy(handle_new);
            return 0;
        }
    }
    
    // Clamp dimensions to reasonable values
    if (width <= 0 || height <= 0 || width > 4096 || height > 4096) {
        width = 64;
        height = 64;
        jpegSubsamp = TJSAMP_420;
    }
    
    // Calculate pitch
    int pitch = width * tjPixelSize[TJPF_RGB];
    unsigned long dstSize = pitch * height;
    
    // Allocate destination buffer for decompression
    unsigned char* dstBuf = tjAlloc(dstSize);
    if (!dstBuf) goto cleanup;
    
    // Decompress using tjDecompress2
    int decompress_result = tjDecompress2(handle_decompress, data, size, dstBuf,
                                         width, pitch, height, TJPF_RGB, 0);
    
    // Check error code
    int error_code = tjGetErrorCode(handle_decompress);
    if (decompress_result != 0) {
        tjFree(dstBuf);
        goto cleanup;
    }
    
    // 2. Encode to YUV using tjEncodeYUV
    unsigned long yuvSize = tjBufSizeYUV(width, height, jpegSubsamp);
    unsigned char* yuvBuf = tjAlloc(yuvSize);
    if (!yuvBuf) {
        tjFree(dstBuf);
        goto cleanup;
    }
    
    int encode_result = tjEncodeYUV(handle_compress, dstBuf, width, pitch,
                                   height, 3, yuvBuf, jpegSubsamp, 0);
    
    // Check error code
    error_code = tjGetErrorCode(handle_compress);
    
    // 3. Encode to YUV planes using tjEncodeYUVPlanes
    int strides[3];
    unsigned char* yuvPlanes[3];
    
    // Calculate plane sizes and allocate
    for (int i = 0; i < 3; i++) {
        strides[i] = tjPlaneWidth(i, width, jpegSubsamp);
        int planeHeight = tjPlaneHeight(i, height, jpegSubsamp);
        unsigned long planeSize = tjPlaneSizeYUV(i, width, strides[i], height, jpegSubsamp);
        yuvPlanes[i] = tjAlloc(planeSize);
        if (!yuvPlanes[i]) {
            for (int j = 0; j < i; j++) tjFree(yuvPlanes[j]);
            tjFree(yuvBuf);
            tjFree(dstBuf);
            goto cleanup;
        }
    }
    
    int encode_planes_result = tjEncodeYUVPlanes(handle_compress, dstBuf, width,
                                                pitch, height, TJPF_RGB,
                                                yuvPlanes, strides, jpegSubsamp, 0);
    
    // 4. Compress from YUV using tj3CompressFromYUV8 (new API)
    unsigned char* jpegBuf = nullptr;
    size_t jpegSize = 0;
    
    // For tj3CompressFromYUV8, we need to determine alignment
    int align = 4; // Typical alignment
    
    int compress_result = tj3CompressFromYUV8(handle_new, yuvBuf, width, align,
                                             height, &jpegBuf, &jpegSize);
    
    if (compress_result == 0 && jpegBuf && jpegSize > 0) {
        // 5. Transform the compressed JPEG using tjTransform
        unsigned char* dstBufs[1];
        unsigned long dstSizes[1];
        tjtransform transforms[1];
        
        // Set up a simple crop transformation
        transforms[0].r.x = 0;
        transforms[0].r.y = 0;
        transforms[0].r.w = (width > 16) ? 16 : width;
        transforms[0].r.h = (height > 16) ? 16 : height;
        transforms[0].op = TJXOP_NONE;
        transforms[0].options = 0;
        transforms[0].data = nullptr;
        transforms[0].customFilter = nullptr;
        
        int transform_result = tjTransform(handle_transform, jpegBuf, jpegSize,
                                          1, dstBufs, dstSizes, transforms, 0);
        
        if (transform_result == 0 && dstBufs[0]) {
            tjFree(dstBufs[0]);
        }
        
        // 6. Save as 16-bit image using tj3SaveImage16
        // First create a 16-bit version of our decompressed buffer
        int pixelFormat = TJPF_RGB;
        unsigned short* buffer16 = (unsigned short*)tj3Alloc(width * height * 
                                                           tjPixelSize[pixelFormat] * 2);
        if (buffer16) {
            // Convert 8-bit to 16-bit (simple scale)
            for (int i = 0; i < width * height * 3; i++) {
                buffer16[i] = dstBuf[i] * 257; // Scale 8-bit to 16-bit
            }
            
            // Save to file (we'll use a temporary filename)
            tj3SaveImage16(handle_new, "output_file", buffer16, 
                          width, pitch, height, pixelFormat);
            
            tj3Free(buffer16);
        }
        
        tj3Free(jpegBuf);
    }
    
    // 7. Use tjPlaneWidth to get plane widths
    for (int i = 0; i < 3; i++) {
        int plane_width = tjPlaneWidth(i, width, jpegSubsamp);
        // Use the result to prevent optimization
        (void)plane_width;
    }
    
    // Cleanup
    for (int i = 0; i < 3; i++) {
        if (yuvPlanes[i]) tjFree(yuvPlanes[i]);
    }
    
    tjFree(yuvBuf);
    tjFree(dstBuf);
    
    tjDestroy(handle_decompress);
    tjDestroy(handle_compress);
    tjDestroy(handle_transform);
    tj3Destroy(handle_new);
    
    // Remove output file if created
    remove("output_file");
    
    assert_file_name_closed("output_file");
	return 0;
}