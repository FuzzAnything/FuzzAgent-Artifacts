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
//<ID> 72
//<Prompt> []
/*<Combination>: [
*/
//<score> 19, nr_unique_branch: 0
//<Quality> {"density":38,"unique_branches":{},"library_calls":["tj3InitVersion","tj3InitVersion","tj3InitVersion","tj3Destroy","tj3Destroy","tj3Destroy","tjDecompressHeader","tj3JPEGBufSize","tj3Alloc","tjTransform","tj3Alloc","tj3Decompress12","tj3YUVBufSize","tj3Alloc","tj3CompressFromYUV8","tj3Free","tj3YUVPlaneWidth","tj3YUVPlaneHeight","tj3YUVPlaneSize","tj3YUVPlaneSize","tj3YUVPlaneSize","tj3Alloc","tj3Alloc","tj3Alloc","tjCompressFromYUVPlanes","tjFree","tj3Free","tj3Free","tj3Free","tj3Free","tj3Free","tj3Free","tj3LoadImage8","tj3Free","tj3LoadImage16","tj3Free","tj3Destroy","tj3Destroy","tj3Destroy"],"critical_calls":["tj3InitVersion","tj3InitVersion","tj3InitVersion","tjDecompressHeader","tj3JPEGBufSize","tj3Alloc","tjTransform","tj3Alloc","tj3Decompress12","tj3YUVBufSize","tj3Alloc","tj3CompressFromYUV8","tj3Free","tj3YUVPlaneWidth","tj3YUVPlaneHeight","tj3YUVPlaneSize","tj3YUVPlaneSize","tj3YUVPlaneSize","tj3Alloc","tj3Alloc","tj3Alloc","tjCompressFromYUVPlanes","tjFree","tj3Free","tj3Free","tj3Free","tj3Free","tj3Free","tj3Free","tj3LoadImage8","tj3Free","tj3LoadImage16","tj3Free","tj3Destroy","tj3Destroy","tj3Destroy"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_12(const uint8_t* f_data, size_t f_size) {
	if(f_size<48) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_10, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_11, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_12, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 1) {
	return 0;
	}
	
    
    // Create a temporary file with the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Initialize handles
    tjhandle handle_compress = tj3Init(TJINIT_COMPRESS);
    tjhandle handle_decompress = tj3Init(TJINIT_DECOMPRESS);
    tjhandle handle_transform = tj3Init(TJINIT_TRANSFORM);
    
    if (!handle_compress || !handle_decompress || !handle_transform) {
        if (handle_compress) tj3Destroy(handle_compress);
        if (handle_decompress) tj3Destroy(handle_decompress);
        if (handle_transform) tj3Destroy(handle_transform);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 1. First use tjDecompressHeader to get image dimensions
    unsigned char *jpegBuf = (unsigned char *)data;
    unsigned long jpegSize = (unsigned long)size;
    int width = 0, height = 0;
    
    int result = tjDecompressHeader(handle_decompress, jpegBuf, jpegSize, &width, &height);
    if (result == 0 && width > 0 && height > 0) {
        // 2. Use tjTransform to apply transformations
        tjtransform transform;
        memset(&transform, 0, sizeof(transform));
        transform.op = TJXOP_NONE;
        transform.options = TJXOPT_CROP;
        transform.r.x = 0;
        transform.r.y = 0;
        transform.r.w = width / 2;
        transform.r.h = height / 2;
        
        unsigned char *dstBuf = nullptr;
        unsigned long dstSize = 0;
        
        // Calculate buffer size for transformed image
        dstSize = tj3JPEGBufSize(transform.r.w, transform.r.h, TJSAMP_444);
        if (dstSize > 0) {
            dstBuf = (unsigned char *)tj3Alloc(dstSize);
            if (dstBuf) {
                // Perform transformation
                result = tjTransform(handle_transform, jpegBuf, jpegSize, 1, &dstBuf, &dstSize, &transform, fuzz_int32_t_1);
                
                if (result == 0 && dstBuf && dstSize > 0) {
                    // 3. Use tj3Decompress12 to decompress the transformed image
                    short *dstBuf12 = nullptr;
                    int pitch = transform.r.w * tjPixelSize[TJPF_RGB];
                    
                    // Allocate buffer for 12-bit decompression
                    dstBuf12 = (short *)tj3Alloc(transform.r.w * transform.r.h * tjPixelSize[TJPF_RGB] * sizeof(short));
                    if (dstBuf12) {
                        result = tj3Decompress12(handle_decompress, dstBuf, dstSize, dstBuf12, 
                                                pitch, TJPF_RGB);
                        
                        // 4. Create YUV data for compression
                        if (result == 0) {
                            // Create a simple YUV buffer (dummy data for testing)
                            int align = 1;
                            size_t yuvSize = tj3YUVBufSize(transform.r.w, fuzz_int32_t_2, transform.r.h, TJSAMP_444);
                            unsigned char *yuvBuf = nullptr;
                            
                            if (yuvSize > 0) {
                                yuvBuf = (unsigned char *)tj3Alloc(yuvSize);
                                if (yuvBuf) {
                                    // Fill with some data
                                    memset(yuvBuf, 128, yuvSize);
                                    
                                    // 5. Use tj3CompressFromYUV8
                                    unsigned char *compressedBuf = nullptr;
                                    size_t compressedSize = 0;
                                    
                                    result = tj3CompressFromYUV8(handle_compress, yuvBuf, 
                                                                transform.r.w, fuzz_int32_t_3, transform.r.h,
                                                                &compressedBuf, &compressedSize);
                                    
                                    if (compressedBuf) {
                                        tj3Free(compressedBuf);
                                    }
                                    
                                    // 6. Use tjCompressFromYUVPlanes
                                    // Create planar YUV data
                                    const unsigned char *srcPlanes[3];
                                    int strides[3];
                                    int planeWidth = tj3YUVPlaneWidth(fuzz_int32_t_4, transform.r.w, TJSAMP_444);
                                    int planeHeight = tj3YUVPlaneHeight(fuzz_int32_t_5, transform.r.h, TJSAMP_444);
                                    
                                    size_t planeSizeY = tj3YUVPlaneSize(fuzz_int32_t_6, transform.r.w, planeWidth, transform.r.h, TJSAMP_444);
                                    size_t planeSizeU = tj3YUVPlaneSize(fuzz_int32_t_7, transform.r.w, planeWidth/2, transform.r.h/2, TJSAMP_444);
                                    size_t planeSizeV = tj3YUVPlaneSize(fuzz_int32_t_8, transform.r.w, planeWidth/2, transform.r.h/2, TJSAMP_444);
                                    
                                    unsigned char *planeY = (unsigned char *)tj3Alloc(planeSizeY);
                                    unsigned char *planeU = (unsigned char *)tj3Alloc(planeSizeU);
                                    unsigned char *planeV = (unsigned char *)tj3Alloc(planeSizeV);
                                    
                                    if (planeY && planeU && planeV) {
                                        memset(planeY, 128, planeSizeY);
                                        memset(planeU, 128, planeSizeU);
                                        memset(planeV, 128, planeSizeV);
                                        
                                        srcPlanes[0] = planeY;
                                        srcPlanes[1] = planeU;
                                        srcPlanes[2] = planeV;
                                        strides[0] = planeWidth;
                                        strides[1] = planeWidth/2;
                                        strides[2] = planeWidth/2;
                                        
                                        unsigned char *jpegBuf2 = nullptr;
                                        unsigned long jpegSize2 = 0;
                                        
                                        result = tjCompressFromYUVPlanes(handle_compress, srcPlanes, 
                                                                        transform.r.w, strides, transform.r.h,
                                                                        TJSAMP_444, &jpegBuf2, &jpegSize2,
                                                                        fuzz_int32_t_9, fuzz_int32_t_10);
                                        
                                        if (jpegBuf2) {
                                            tjFree(jpegBuf2);
                                        }
                                    }
                                    
                                    if (planeY) tj3Free(planeY);
                                    if (planeU) tj3Free(planeU);
                                    if (planeV) tj3Free(planeV);
                                    
                                    tj3Free(yuvBuf);
                                }
                            }
                        }
                        
                        tj3Free(dstBuf12);
                    }
                }
                
                if (dstBuf) {
                    tj3Free(dstBuf);
                }
            }
        }
    }
    
    // 7. Try to load images from file (using the input data as file content)
    // Write input data to a temporary file
    FILE *temp_file = fopen("input_file", "wb");
    if (temp_file) {
        fwrite(data, 1, size, temp_file);
        assert_file_closed(&temp_file);;
        
        // Try to load as 8-bit image
        int width8 = 0, height8 = 0, pixelFormat8 = 0;
        unsigned char *img8 = tj3LoadImage8(handle_decompress, "input_file", 
                                           &width8, fuzz_int32_t_11, &height8, &pixelFormat8);
        if (img8) {
            tj3Free(img8);
        }
        
        // Try to load as 16-bit image
        int width16 = 0, height16 = 0, pixelFormat16 = 0;
        unsigned short *img16 = tj3LoadImage16(handle_decompress, "input_file",
                                              &width16, fuzz_int32_t_12, &height16, &pixelFormat16);
        if (img16) {
            tj3Free(img16);
        }
        
        // Clean up temporary file
        remove("input_file");
    }
    
    // Clean up
    tj3Destroy(handle_compress);
    tj3Destroy(handle_decompress);
    tj3Destroy(handle_transform);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&temp_file);
	assert_file_closed(&in_file);
	return 0;
}