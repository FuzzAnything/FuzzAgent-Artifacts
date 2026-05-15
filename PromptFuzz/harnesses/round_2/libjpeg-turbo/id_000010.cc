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
//<ID> 75
//<Prompt> []
/*<Combination>: [
*/
//<score> 66, nr_unique_branch: 20
//<Quality> {"density":22,"unique_branches":{"tj3GetICCProfile":[[1981,7,1981,22,0,0,4,1],[1984,7,1984,30,0,0,4,0],[1985,9,1985,28,0,0,4,0],[1986,11,1986,17,0,0,4,0],[340,7,340,12,1,0,4,1]],"tj3SetCroppingRegion":[[2065,7,2065,37,0,0,4,1],[2068,7,2068,28,0,0,4,0],[2068,7,2068,28,0,0,4,1],[2068,32,2068,53,0,0,4,0],[2068,32,2068,53,0,0,4,1],[2069,7,2069,28,0,0,4,0],[2069,7,2069,28,0,0,4,1],[2069,32,2069,53,0,0,4,0],[2069,32,2069,53,0,0,4,1],[2074,7,2074,27,0,0,4,1],[2074,31,2074,51,0,0,4,1],[2074,55,2074,75,0,0,4,1],[2075,7,2075,27,0,0,4,1],[2077,7,2077,26,0,0,4,0],[340,7,340,12,1,0,4,1]]},"library_calls":["tj3InitVersion","tj3LoadImage8","tj3SetCroppingRegion","tj3GetICCProfile","TJBUFSIZE","tj3EncodeYUVPlanes8","tjPlaneSizeYUV","tjPlaneSizeYUV","tjPlaneSizeYUV","tjCompressFromYUVPlanes","tj3Alloc","tj3YUVBufSize","tj3DecompressToYUV8","tj3Alloc","tj3DecodeYUV8","tj3Free","tj3Free","tj3CompressFromYUVPlanes8","tj3Free","tj3Free","tj3Free","tjFree","tj3Destroy"],"critical_calls":["tj3InitVersion","tj3LoadImage8","tj3SetCroppingRegion","tj3GetICCProfile","TJBUFSIZE","tj3EncodeYUVPlanes8","tjPlaneSizeYUV","tjPlaneSizeYUV","tjPlaneSizeYUV","tjCompressFromYUVPlanes","tj3Alloc","tj3YUVBufSize","tj3DecompressToYUV8","tj3Alloc","tj3DecodeYUV8","tj3Free","tj3Free","tj3CompressFromYUVPlanes8","tj3Free","tj3Free","tj3Free","tjFree","tj3Destroy"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_10(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 100) {
	return 0;
	}
	 // Too small to be meaningful

    // Create a temporary file with the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    tjhandle handle = nullptr;
    int width = 0, height = 0, pixelFormat = TJPF_RGB;
    int subsamp = TJSAMP_420;
    unsigned char *jpegBuf = nullptr;
    unsigned long jpegSize = 0;
    int result = 0;
    
    // Initialize TurboJPEG 3.x handle for loading
    handle = tj3Init(TJINIT_DECOMPRESS);
    if (!handle) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Write data to a file for loading (tj3LoadImage8 requires filename)
    // We'll use the input file we created
    int align = 1;
    unsigned char *loadedImage = tj3LoadImage8(handle, "input_file", &width, align, &height, &pixelFormat);
    
    // Reset file pointer
    fseek(in_file, 0, SEEK_SET);
    
    if (loadedImage && width > 0 && height > 0) {
        // Set cropping region (crop middle 50% of image)
        tjregion cropRegion;
        cropRegion.x = width / 4;
        cropRegion.y = height / 4;
        cropRegion.w = width / 2;
        cropRegion.h = height / 2;
        tj3SetCroppingRegion(handle, cropRegion);
        
        // Get ICC profile if exists
        unsigned char *iccBuf = nullptr;
        size_t iccSize = 0;
        tj3GetICCProfile(handle, &iccBuf, &iccSize);
        
        // Calculate buffer sizes
        unsigned long bufSize = TJBUFSIZE(width, height);
        
        // Encode to YUV planes using new API
        int strides[3] = {0};
        unsigned char *yuvPlanes[3] = {nullptr};
        result = tj3EncodeYUVPlanes8(handle, loadedImage, width, 0, height, pixelFormat, yuvPlanes, strides);
        
        if (result == 0 && yuvPlanes[0]) {
            // Calculate plane sizes
            unsigned long planeSizeY = tjPlaneSizeYUV(0, width, strides[0], height, subsamp);
            unsigned long planeSizeU = tjPlaneSizeYUV(1, width, strides[1], height, subsamp);
            unsigned long planeSizeV = tjPlaneSizeYUV(2, width, strides[2], height, subsamp);
            
            // Compress from YUV planes using old API
            const unsigned char *srcPlanes[3] = {yuvPlanes[0], yuvPlanes[1], yuvPlanes[2]};
            result = tjCompressFromYUVPlanes(handle, srcPlanes, width, strides, height, 
                                             subsamp, &jpegBuf, &jpegSize, 85, 0);
            
            if (result == 0 && jpegBuf && jpegSize > 0) {
                // Decompress the JPEG to YUV buffer using new API
                unsigned char *yuvBuffer = (unsigned char*)tj3Alloc(tj3YUVBufSize(width, align, height, subsamp));
                if (yuvBuffer) {
                    result = tj3DecompressToYUV8(handle, jpegBuf, jpegSize, yuvBuffer, align);
                    
                    if (result == 0) {
                        // Decode YUV back to RGB using new API
                        unsigned char *decodedImage = (unsigned char*)tj3Alloc(width * height * tjPixelSize[pixelFormat]);
                        if (decodedImage) {
                            result = tj3DecodeYUV8(handle, yuvBuffer, align, decodedImage, 
                                                   width, 0, height, pixelFormat);
                            tj3Free(decodedImage);
                        }
                    }
                    tj3Free(yuvBuffer);
                }
                
                // Also compress from YUV planes using new API
                unsigned char *jpegBuf2 = nullptr;
                size_t jpegSize2 = 0;
                result = tj3CompressFromYUVPlanes8(handle, srcPlanes, width, strides, height, 
                                                   &jpegBuf2, &jpegSize2);
                
                if (jpegBuf2) {
                    tj3Free(jpegBuf2);
                }
            }
            
            // Free YUV planes
            for (int i = 0; i < 3; i++) {
                if (yuvPlanes[i]) {
                    tj3Free(yuvPlanes[i]);
                }
            }
        }
        
        if (iccBuf) {
            free(iccBuf);
        }
        
        tj3Free(loadedImage);
    }
    
    // Cleanup
    if (jpegBuf) {
        tjFree(jpegBuf);
    }
    
    tj3Destroy(handle);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
}