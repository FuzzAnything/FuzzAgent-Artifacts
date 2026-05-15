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
//<ID> 77
//<Prompt> []
/*<Combination>: [
*/
//<score> 80, nr_unique_branch: 35
//<Quality> {"density":16,"unique_branches":{"tj3YUVPlaneWidth":[[1121,7,1121,16,0,0,4,0]],"tj3YUVPlaneHeight":[[1155,7,1155,17,0,0,4,0]],"j16init_write_ppm":[[324,7,324,46,0,0,4,1],[325,7,325,50,0,0,4,1],[339,7,339,46,0,0,4,1],[340,7,340,47,0,0,4,1],[366,9,366,32,0,0,4,0],[78,4,78,17,11,0,4,1],[78,22,78,39,11,0,4,0],[78,43,78,61,11,0,4,0]],"tj3YUVPlaneSize":[[1085,7,1085,16,0,0,4,0]],"tj3SaveImage16":[[511,7,511,16,0,0,4,1],[511,20,511,27,0,0,4,1],[511,31,511,40,0,0,4,1],[511,44,511,53,0,0,4,1],[511,57,511,67,0,0,4,1],[512,7,512,22,0,0,4,1],[512,26,512,49,0,0,4,1],[518,7,518,53,0,0,4,1],[526,7,526,45,0,0,4,1],[539,7,539,10,0,0,4,1],[546,14,546,17,0,0,4,1],[572,9,572,47,0,0,4,1],[576,9,576,48,0,0,4,1],[587,7,587,17,0,0,4,1],[589,10,589,55,0,0,4,0],[589,10,589,55,0,0,4,1],[592,9,592,15,0,0,4,1],[607,7,607,11,0,0,4,0],[340,7,340,12,2,0,4,1]],"tj3GetScalingFactors":[[2017,7,2017,32,0,0,4,1]],"processFlags":[[554,7,554,28,0,0,4,1]],"tjSaveImage":[[3180,7,3180,52,0,0,4,1]],"tj3SaveImage8":[[539,7,539,10,0,0,4,1],[546,14,546,17,0,0,4,1]]},"library_calls":["tj3InitVersion","tj3LoadImage8","tj3LoadImage12","tj3GetScalingFactors","tjInitDecompress","tjDecompressHeader2","tjBufSizeYUV2","tjAlloc","tjDecompressToYUV2","tjFree","tjDestroy","tj3Destroy","tj3InitVersion","tj3DecompressHeader","tj3Get","tj3Get","tj3Get","tj3YUVPlaneWidth","tj3YUVPlaneHeight","tj3YUVPlaneWidth","tj3YUVPlaneSize","tj3Alloc","tj3Free","tj3DecompressToYUVPlanes8","tj3Free","tj3JPEGBufSize","tj3Alloc","tj3SaveImage16","tj3Free","tjSaveImage","tj3Free","tj3Free","tj3Destroy"],"critical_calls":["tj3InitVersion","tj3LoadImage8","tj3LoadImage12","tj3GetScalingFactors","tjInitDecompress","tjDecompressHeader2","tjBufSizeYUV2","tjAlloc","tjDecompressToYUV2","tjFree","tjDestroy","tj3Destroy","tj3InitVersion","tj3DecompressHeader","tj3Get","tj3Get","tj3Get","tj3YUVPlaneWidth","tj3YUVPlaneHeight","tj3YUVPlaneWidth","tj3YUVPlaneSize","tj3Alloc","tj3Free","tj3DecompressToYUVPlanes8","tj3Free","tj3JPEGBufSize","tj3Alloc","tj3SaveImage16","tj3Free","tjSaveImage","tj3Free","tj3Free","tj3Destroy"],"visited":1}
/**/


// Include libjpeg-turbo headers
extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_12(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Early return if input is too small
    if (size < 100) {
	return 0;
	}
	
    
    tjhandle handle = nullptr;
    int ret = 0;
    FILE *in_file = nullptr;
    FILE *out_file = nullptr;
    int width = 0, height = 0, pixelFormat = 0;
    int align = 4; // Default alignment
    
    // Create input file from fuzzer data
    in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Initialize libjpeg-turbo
    handle = tj3Init(TJINIT_DECOMPRESS);
    if (!handle) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Part 1: Load image using both 8-bit and 12-bit loaders
    // First try loading as 8-bit image
    unsigned char *img8 = tj3LoadImage8(handle, "input_file", &width, align, &height, &pixelFormat);
    
    // Save the position in the file to reset for 12-bit loading
    fseek(in_file, 0, SEEK_SET);
    
    // Try loading as 12-bit image
    short *img12 = tj3LoadImage12(handle, "input_file", &width, align, &height, &pixelFormat);
    
    // Reset file position for decompression operations
    fseek(in_file, 0, SEEK_SET);
    
    // Part 2: Get scaling factors
    int numScalingFactors = 0;
    tjscalingfactor *scalingFactors = tj3GetScalingFactors(&numScalingFactors);
    
    // Part 3: Decompress JPEG data to YUV format using old API
    // First get JPEG dimensions
    int jpegWidth, jpegHeight, jpegSubsamp;
    unsigned long jpegSize = (unsigned long)size;
    
    // Create a handle for old API
    tjhandle oldHandle = tjInitDecompress();
    if (oldHandle) {
        // Try to read JPEG header
        if (tjDecompressHeader2(oldHandle, (unsigned char *)data, jpegSize, 
                                &jpegWidth, &jpegHeight, &jpegSubsamp) == 0) {
            
            // Calculate buffer size for YUV
            unsigned long yuvBufSize = tjBufSizeYUV2(jpegWidth, align, jpegHeight, jpegSubsamp);
            if (yuvBufSize > 0 && yuvBufSize < 100 * 1024 * 1024) { // Limit to 100MB
                unsigned char *yuvBuf = (unsigned char *)tjAlloc(yuvBufSize);
                if (yuvBuf) {
                    // Decompress to YUV using old API
                    tjDecompressToYUV2(oldHandle, (unsigned char *)data, jpegSize, 
                                      yuvBuf, jpegWidth, align, jpegHeight, 0);
                    tjFree(yuvBuf);
                }
            }
        }
        tjDestroy(oldHandle);
    }
    
    // Part 4: Decompress to YUV planes using new API
    // Reset handle for new API operations
    tj3Destroy(handle);
    handle = tj3Init(TJINIT_DECOMPRESS);
    
    if (handle) {
        // Try to decompress header with new API
        if (tj3DecompressHeader(handle, data, size) == 0) {
            width = tj3Get(handle, TJPARAM_JPEGWIDTH);
            height = tj3Get(handle, TJPARAM_JPEGHEIGHT);
            int subsamp = tj3Get(handle, TJPARAM_SUBSAMP);
            
            // Allocate planes for YUV
            int strides[3];
            unsigned char *planes[3];
            
            for (int i = 0; i < 3; i++) {
                int planeWidth = tj3YUVPlaneWidth(i, width, subsamp);
                int planeHeight = tj3YUVPlaneHeight(i, height, subsamp);
                strides[i] = tj3YUVPlaneWidth(i, width, subsamp) * tjPixelSize[TJPF_GRAY];
                size_t planeSize = tj3YUVPlaneSize(i, width, strides[i], height, subsamp);
                
                if (planeSize > 0 && planeSize < 50 * 1024 * 1024) { // Limit to 50MB
                    planes[i] = (unsigned char *)tj3Alloc(planeSize);
                    if (!planes[i]) {
                        // Free already allocated planes
                        for (int j = 0; j < i; j++) {
                            tj3Free(planes[j]);
                        }
                        break;
                    }
                } else {
                    planes[i] = nullptr;
                }
            }
            
            // Decompress to YUV planes if all planes were allocated
            if (planes[0] && planes[1] && planes[2]) {
                tj3DecompressToYUVPlanes8(handle, data, size, planes, strides);
            }
            
            // Free YUV planes
            for (int i = 0; i < 3; i++) {
                if (planes[i]) {
                    tj3Free(planes[i]);
                }
            }
        }
    }
    
    // Part 5: Calculate JPEG buffer size
    if (width > 0 && height > 0) {
        size_t jpegBufSize = tj3JPEGBufSize(width, height, TJSAMP_444);
        (void)jpegBufSize; // Use variable to avoid unused warning
    }
    
    // Part 6: Save images if we successfully loaded them
    out_file = fopen("output_file", "wb");
    if (out_file) {
        // Save 16-bit image if we have 12-bit data
        if (img12 && width > 0 && height > 0) {
            // Convert 12-bit to 16-bit by shifting
            int pitch = width * tjPixelSize[pixelFormat];
            unsigned short *img16 = (unsigned short *)tj3Alloc(width * height * sizeof(unsigned short) * 3);
            if (img16) {
                // Simple conversion: shift 12-bit to 16-bit
                for (size_t i = 0; i < width * height * 3; i++) {
                    img16[i] = (unsigned short)(img12[i] << 4);
                }
                tj3SaveImage16(handle, "output_file", img16, width, pitch, height, pixelFormat);
                tj3Free(img16);
            }
        }
        
        // Also save using old API if we have 8-bit data
        if (img8 && width > 0 && height > 0) {
            int pitch = width * tjPixelSize[pixelFormat];
            tjSaveImage("output_file", img8, width, pitch, height, pixelFormat, 0);
        }
        
        assert_file_closed(&out_file);;
    }
    
    // Cleanup
    if (img8) tj3Free(img8);
    if (img12) tj3Free(img12);
    if (handle) tj3Destroy(handle);
    if (in_file) assert_file_closed(&in_file);;
    
    // Remove temporary files
    remove("input_file");
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
}