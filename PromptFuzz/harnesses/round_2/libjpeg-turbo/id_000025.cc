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
//<ID> 373
//<Prompt> []
/*<Combination>: [
*/
//<score> 82, nr_unique_branch: 12
//<Quality> {"density":41,"unique_branches":{"tj3YUVPlaneSize":[[1092,7,1092,18,0,0,4,1]],"tjPlaneHeight":[[1178,10,1178,23,0,0,4,1]],"tj3CompressFromYUVPlanes8":[[1334,15,1334,33,0,0,4,0],[1334,15,1334,33,0,0,4,1],[1338,7,1338,35,0,0,4,0],[1433,7,1433,41,0,0,4,1],[1435,7,1435,41,0,0,4,1],[1435,45,1435,57,0,0,4,0],[1437,15,1437,33,0,0,4,0],[1437,15,1437,33,0,0,4,1],[1442,7,1442,25,0,0,4,1],[317,7,317,12,2,0,4,1]]},"library_calls":["tj3InitVersion","tj3LoadImage12","tj3Destroy","tj3Alloc","tj3Free","tj3Destroy","tj3DecompressHeader","tj3Free","tj3Free","tj3Destroy","tj3Get","tj3Get","tj3Alloc","tj3Decompress16","tj3Free","tjPlaneHeight","tjPlaneHeight","tjPlaneHeight","tj3Alloc","tj3YUVPlaneWidth","tj3YUVPlaneWidth","tj3YUVPlaneWidth","tj3YUVPlaneSize","tj3Alloc","tj3EncodeYUVPlanes8","tj3CompressFromYUVPlanes8","tj3YUVPlaneSize","tj3Alloc","tj3DecompressToYUVPlanes8","tj3Free","tj3Free","tj3Free","tj3Free","tjInitDecompress","tjAlloc","tjDecompress2","tjFree","tjDestroy","tj3Free","tj3Free","tj3Destroy"],"critical_calls":["tj3InitVersion","tj3LoadImage12","tj3Alloc","tj3DecompressHeader","tj3Get","tj3Get","tj3Alloc","tj3Decompress16","tj3Free","tjPlaneHeight","tjPlaneHeight","tjPlaneHeight","tj3Alloc","tj3YUVPlaneWidth","tj3YUVPlaneWidth","tj3YUVPlaneWidth","tj3YUVPlaneSize","tj3Alloc","tj3EncodeYUVPlanes8","tj3CompressFromYUVPlanes8","tj3YUVPlaneSize","tj3Alloc","tj3DecompressToYUVPlanes8","tj3Free","tj3Free","tj3Free","tj3Free","tjInitDecompress","tjAlloc","tjDecompress2","tjFree","tjDestroy","tj3Free","tj3Free","tj3Destroy"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_25(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 1. Use tj3LoadImage12 to load 12-bit image from input file
    // First write input data to a temporary file since tj3LoadImage12 expects file input
    FILE *temp_file = fopen("temp_input_file", "wb");
    if (!temp_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&temp_file);
	assert_file_closed(&in_file);
	return 0;
    }
    fwrite(data, 1, size, temp_file);
    assert_file_closed(&temp_file);;
    
    tjhandle handle = tj3Init(TJINIT_DECOMPRESS);
    if (!handle) {
        assert_file_closed(&in_file);;
        assert_file_closed(&temp_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    int width = 0, height = 0, pixelFormat = 0;
    short *image12 = tj3LoadImage12(handle, "temp_input_file", &width, 1, &height, &pixelFormat);
    
    // Clean up temp file
    remove("temp_input_file");
    
    if (!image12 || width <= 0 || height <= 0) {
        tj3Destroy(handle);
        assert_file_closed(&in_file);;
        assert_file_closed(&temp_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // For demonstration, we'll create a simple 16-bit buffer from the 12-bit data
    // by shifting and duplicating bits
    unsigned short *image16 = (unsigned short *)tj3Alloc(width * height * sizeof(unsigned short) * 3); // RGB
    if (!image16) {
        tj3Free(image12);
        tj3Destroy(handle);
        assert_file_closed(&in_file);;
        assert_file_closed(&temp_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Convert 12-bit to 16-bit (scale 12-bit 0-4095 to 16-bit 0-65535)
    size_t pixelCount = width * height * 3;
    for (size_t i = 0; i < pixelCount; i++) {
        image16[i] = (unsigned short)((image12[i] << 4) | (image12[i] & 0xF));
    }
    
    // 2. Use tj3Decompress16 with the original JPEG data
    // First get JPEG header info
    if (tj3DecompressHeader(handle, data, size) != 0) {
        tj3Free(image16);
        tj3Free(image12);
        tj3Destroy(handle);
        assert_file_closed(&in_file);;
        assert_file_closed(&temp_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    int jpegWidth = tj3Get(handle, TJPARAM_JPEGWIDTH);
    int jpegHeight = tj3Get(handle, TJPARAM_JPEGHEIGHT);
    
    if (jpegWidth > 0 && jpegHeight > 0) {
        unsigned short *decompressed16 = (unsigned short *)tj3Alloc(jpegWidth * jpegHeight * sizeof(unsigned short) * 3);
        if (decompressed16) {
            // Try to decompress with 16-bit precision
            tj3Decompress16(handle, data, size, decompressed16, jpegWidth * 3, TJPF_RGB);
            tj3Free(decompressed16);
        }
    }
    
    // 3. Use tjPlaneHeight to get YUV plane height
    int subsamp = TJSAMP_420;
    int yHeight = tjPlaneHeight(0, height, subsamp);
    int uHeight = tjPlaneHeight(1, height, subsamp);
    int vHeight = tjPlaneHeight(2, height, subsamp);
    
    // 4. Use tj3EncodeYUVPlanes8 to encode RGB to YUV planes
    // First convert 16-bit to 8-bit for encoding
    unsigned char *image8 = (unsigned char *)tj3Alloc(width * height * 3);
    if (image8) {
        for (size_t i = 0; i < width * height * 3; i++) {
            image8[i] = (unsigned char)(image16[i] >> 8); // Scale 16-bit to 8-bit
        }
        
        // Prepare YUV plane buffers
        unsigned char *yPlanes[3] = {nullptr};
        int strides[3] = {0, 0, 0};
        
        // Calculate plane sizes
        strides[0] = tj3YUVPlaneWidth(0, width, subsamp);
        strides[1] = tj3YUVPlaneWidth(1, width, subsamp);
        strides[2] = tj3YUVPlaneWidth(2, width, subsamp);
        
        for (int i = 0; i < 3; i++) {
            size_t planeSize = tj3YUVPlaneSize(i, width, strides[i], height, subsamp);
            yPlanes[i] = (unsigned char *)tj3Alloc(planeSize);
        }
        
        if (yPlanes[0] && yPlanes[1] && yPlanes[2]) {
            // Encode to YUV planes
            tj3EncodeYUVPlanes8(handle, image8, width, width * 3, height, 
                                TJPF_RGB, yPlanes, strides);
            
            // 5. Use tj3CompressFromYUVPlanes8 to compress YUV planes to JPEG
            unsigned char *jpegBuf = nullptr;
            size_t jpegSize = 0;
            
            if (tj3CompressFromYUVPlanes8(handle, (const unsigned char *const *)yPlanes, 
                                         width, strides, height, &jpegBuf, &jpegSize) == 0) {
                
                // 6. Use tj3DecompressToYUVPlanes8 to decompress JPEG back to YUV planes
                unsigned char *yuvPlanes[3] = {nullptr};
                int decompStrides[3] = {0, 0, 0};
                
                for (int i = 0; i < 3; i++) {
                    size_t planeSize = tj3YUVPlaneSize(i, width, strides[i], height, subsamp);
                    yuvPlanes[i] = (unsigned char *)tj3Alloc(planeSize);
                }
                
                if (yuvPlanes[0] && yuvPlanes[1] && yuvPlanes[2]) {
                    tj3DecompressToYUVPlanes8(handle, jpegBuf, jpegSize, yuvPlanes, decompStrides);
                    
                    // Clean up YUV planes
                    for (int i = 0; i < 3; i++) {
                        tj3Free(yuvPlanes[i]);
                    }
                }
                
                tj3Free(jpegBuf);
            }
        }
        
        // Clean up Y planes
        for (int i = 0; i < 3; i++) {
            if (yPlanes[i]) tj3Free(yPlanes[i]);
        }
        
        tj3Free(image8);
    }
    
    // 7. Use tjDecompress2 (legacy API)
    tjhandle legacyHandle = tjInitDecompress();
    if (legacyHandle && width > 0 && height > 0) {
        unsigned char *legacyBuf = (unsigned char *)tjAlloc(width * height * 3);
        if (legacyBuf) {
            // We'll use the original input data for decompression
            unsigned long jpegSize = (unsigned long)size;
            tjDecompress2(legacyHandle, data, jpegSize, legacyBuf, 
                         width, width * 3, height, TJPF_RGB, 0);
            tjFree(legacyBuf);
        }
        tjDestroy(legacyHandle);
    }
    
    // Clean up
    tj3Free(image16);
    tj3Free(image12);
    tj3Destroy(handle);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&temp_file);
	assert_file_closed(&in_file);
	return 0;
}