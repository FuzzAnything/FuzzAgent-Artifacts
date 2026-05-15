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
//<ID> 906
//<Prompt> []
/*<Combination>: [
*/
//<score> 46, nr_unique_branch: 1
//<Quality> {"density":23,"unique_branches":{"tj3Compress16":[[87,7,87,35,0,0,4,0]]},"library_calls":["tj3InitVersion","tj3LoadImage12","tj3Alloc","tj3Compress16","tj3SetICCProfile","tj3Alloc","tj3Decompress16","tjPlaneSizeYUV","tj3Free","tj3Free","tj3Free","tj3Free","tjInitDecompress","tjDecompressHeader2","tj3Alloc","tj3Decompress16","tj3Compress16","tj3SetICCProfile","tjPlaneSizeYUV","tj3Free","tj3Free","tjDestroy","tj3Destroy"],"critical_calls":["tj3InitVersion","tj3LoadImage12","tj3Alloc","tj3Compress16","tj3SetICCProfile","tj3Alloc","tj3Decompress16","tjPlaneSizeYUV","tj3Free","tj3Free","tj3Free","tj3Free","tj3Destroy"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_42(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Write input data to a temporary file for tj3LoadImage12
    FILE *in_file = fopen("input_file", "wb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    fwrite(data, 1, size, in_file);
    assert_file_closed(&in_file);;

    tjhandle handle = tj3Init(TJINIT_DECOMPRESS);
    if (!handle) {
        remove("input_file");
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }

    // Try to load as 12-bit image first (will likely fail for JPEG data)
    int width, height, pixelFormat;
    short *image12 = tj3LoadImage12(handle, "input_file", &width, 4, &height, &pixelFormat);
    if (image12) {
        // If loading succeeded, we have a 12-bit image
        // Try to compress it as 16-bit (promote to 16-bit)
        unsigned short *src16 = (unsigned short *)tj3Alloc(width * height * 3 * sizeof(unsigned short));
        if (src16) {
            // Promote 12-bit to 16-bit by shifting left 4 bits
            for (size_t i = 0; i < width * height * 3; i++) {
                src16[i] = (unsigned short)(image12[i] << 4);
            }
            
            unsigned char *jpegBuf = nullptr;
            size_t jpegSize = 0;
            
            // Compress the 16-bit image
            int compressResult = tj3Compress16(handle, src16, width, width * 3 * sizeof(unsigned short), 
                                              height, TJPF_RGB, &jpegBuf, &jpegSize);
            
            if (compressResult == 0 && jpegBuf && jpegSize > 0) {
                // Set a dummy ICC profile
                unsigned char iccBuf[] = {0,1,2,3,4,5,6,7,8,9};
                tj3SetICCProfile(handle, iccBuf, sizeof(iccBuf));
                
                // Decompress back to 16-bit
                unsigned short *dst16 = (unsigned short *)tj3Alloc(width * height * 3 * sizeof(unsigned short));
                if (dst16) {
                    tj3Decompress16(handle, jpegBuf, jpegSize, dst16, 
                                   width * 3 * sizeof(unsigned short), TJPF_RGB);
                    
                    // Calculate YUV plane size for this image
                    unsigned long planeSize = tjPlaneSizeYUV(0, width, width, height, TJSAMP_444);
                    
                    tj3Free(dst16);
                }
                tj3Free(jpegBuf);
            }
            
            tj3Free(src16);
        }
        tj3Free(image12);
    } else {
        // If loading as 12-bit failed, try to process as JPEG
        // Use old API for decompress header
        tjhandle old_handle = tjInitDecompress();
        if (old_handle) {
            int width2, height2, jpegSubsamp;
            int headerResult = tjDecompressHeader2(old_handle, 
                                                   const_cast<unsigned char*>(data), 
                                                   size, 
                                                   &width2, 
                                                   &height2, 
                                                   &jpegSubsamp);
            
            if (headerResult == 0 && width2 > 0 && height2 > 0) {
                // Try to decompress as 16-bit
                unsigned short *dst16 = (unsigned short *)tj3Alloc(width2 * height2 * 3 * sizeof(unsigned short));
                if (dst16) {
                    tj3Decompress16(handle, data, size, dst16, 
                                   width2 * 3 * sizeof(unsigned short), TJPF_RGB);
                    
                    // Try to compress back to JPEG
                    unsigned char *jpegBuf = nullptr;
                    size_t jpegSize = 0;
                    
                    int compressResult = tj3Compress16(handle, dst16, width2, 
                                                      width2 * 3 * sizeof(unsigned short), 
                                                      height2, TJPF_RGB, &jpegBuf, &jpegSize);
                    
                    if (compressResult == 0 && jpegBuf && jpegSize > 0) {
                        // Set a dummy ICC profile
                        unsigned char iccBuf[] = {0,1,2,3,4,5,6,7,8,9};
                        tj3SetICCProfile(handle, iccBuf, sizeof(iccBuf));
                        
                        // Calculate YUV plane size
                        unsigned long planeSize = tjPlaneSizeYUV(0, width2, width2, height2, jpegSubsamp);
                        
                        tj3Free(jpegBuf);
                    }
                    
                    tj3Free(dst16);
                }
            }
            tjDestroy(old_handle);
        }
    }

    // Clean up
    remove("input_file");
    tj3Destroy(handle);
    
    assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
}