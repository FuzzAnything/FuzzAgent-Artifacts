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
//<ID> 14
//<Prompt> []
/*<Combination>: [
*/
//<score> 14, nr_unique_branch: 0
//<Quality> {"density":28,"unique_branches":{},"library_calls":["tj3InitVersion","tj3SetScalingFactor","tj3Destroy","tj3DecompressHeader","tj3LoadImage12","tj3Alloc","tj3SaveImage16","tj3Free","tj3Free","tj3LoadImage16","tj3SaveImage16","tj3Free","tj3Get","tj3Get","tj3Get","tj3YUVPlaneWidth","tj3YUVPlaneHeight","tj3YUVPlaneSize","tj3Alloc","tj3Free","tj3Destroy","tj3DecompressToYUVPlanes8","tj3LoadImage12","tj3Free","tj3LoadImage16","tj3SaveImage16","tj3Free","tj3Free","tj3Destroy"],"critical_calls":["tj3InitVersion","tj3SetScalingFactor","tj3DecompressHeader","tj3Get","tj3Get","tj3Get","tj3YUVPlaneWidth","tj3YUVPlaneHeight","tj3YUVPlaneSize","tj3Alloc","tj3DecompressToYUVPlanes8","tj3LoadImage12","tj3Free","tj3LoadImage16","tj3SaveImage16","tj3Free","tj3Free","tj3Destroy"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_4(const uint8_t* f_data, size_t f_size) {
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
        return 0;  // Too small to be meaningful
    }

    // Write input data to a file for loading functions
    FILE *input_file = fopen("input_file", "wb");
    if (!input_file) {
        assert_file_closed(&input_file);
	return 0;
    }
    fwrite(data, 1, size, input_file);
    assert_file_closed(&input_file);;

    tjhandle handle = tj3InitVersion(TJINIT_DECOMPRESS, 0);
    if (!handle) {
        remove("input_file");
        assert_file_closed(&input_file);
	assert_file_name_closed("input_file");
	return 0;
    }

    // Set scaling factor (1:2 scaling)
    tjscalingfactor scaling;
    scaling.num = 1;
    scaling.denom = 2;
    if (tj3SetScalingFactor(handle, scaling) != 0) {
        tj3Destroy(handle);
        remove("input_file");
        assert_file_closed(&input_file);
	assert_file_name_closed("input_file");
	return 0;
    }

    // First, try to decompress the input as JPEG to YUV planes
    unsigned char *jpeg_buf = (unsigned char *)data;
    size_t jpeg_size = size;

    // Get header information first
    if (tj3DecompressHeader(handle, jpeg_buf, jpeg_size) != 0) {
        // Not a valid JPEG, try other operations with the file
        int width, height, pixel_format;
        
        // Try loading as 12-bit image
        short *img12 = tj3LoadImage12(handle, "input_file", &width, 1, &height, &pixel_format);
        if (img12) {
            // Save as 16-bit image (converting from 12-bit)
            unsigned short *img16 = (unsigned short *)tj3Alloc(width * height * sizeof(unsigned short) * 3);
            if (img16) {
                // Simple conversion: copy and shift
                for (size_t i = 0; i < width * height * 3; i++) {
                    img16[i] = (unsigned short)(img12[i] << 4);
                }
                tj3SaveImage16(handle, "output_file", img16, width, width * sizeof(unsigned short) * 3, 
                              height, TJPF_RGB);
                tj3Free(img16);
            }
            tj3Free(img12);
        }

        // Try loading as 16-bit image
        unsigned short *img16_loaded = tj3LoadImage16(handle, "input_file", &width, 1, &height, &pixel_format);
        if (img16_loaded) {
            tj3SaveImage16(handle, "output_file2", img16_loaded, width, width * sizeof(unsigned short) * 3, 
                          height, TJPF_RGB);
            tj3Free(img16_loaded);
        }
    } else {
        // Valid JPEG header, decompress to YUV planes
        int width = tj3Get(handle, TJPARAM_JPEGWIDTH);
        int height = tj3Get(handle, TJPARAM_JPEGHEIGHT);
        int subsamp = tj3Get(handle, TJPARAM_SUBSAMP);

        // Calculate plane sizes and allocate
        int strides[3];
        unsigned char *planes[3];
        
        for (int i = 0; i < 3; i++) {
            int plane_width = tj3YUVPlaneWidth(i, width, subsamp);
            int plane_height = tj3YUVPlaneHeight(i, height, subsamp);
            strides[i] = plane_width;
            size_t plane_size = tj3YUVPlaneSize(i, plane_width, strides[i], plane_height, subsamp);
            planes[i] = (unsigned char *)tj3Alloc(plane_size);
            if (!planes[i]) {
                for (int j = 0; j < i; j++) {
                    tj3Free(planes[j]);
                }
                tj3Destroy(handle);
                remove("input_file");
                remove("output_file");
                remove("output_file2");
                assert_file_closed(&input_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	return 0;
            }
        }

        // Decompress to YUV planes
        if (tj3DecompressToYUVPlanes8(handle, jpeg_buf, jpeg_size, planes, strides) == 0) {
            // Successfully decompressed to YUV planes
            
            // Also try loading as 12-bit and 16-bit images
            int width2, height2, pixel_format;
            
            // Load as 12-bit image
            short *img12 = tj3LoadImage12(handle, "input_file", &width2, 1, &height2, &pixel_format);
            if (img12) {
                tj3Free(img12);
            }
            
            // Load as 16-bit image
            unsigned short *img16 = tj3LoadImage16(handle, "input_file", &width2, 1, &height2, &pixel_format);
            if (img16) {
                tj3SaveImage16(handle, "output_file3", img16, width2, width2 * sizeof(unsigned short) * 3, 
                              height2, pixel_format);
                tj3Free(img16);
            }
        }

        // Free YUV planes
        for (int i = 0; i < 3; i++) {
            tj3Free(planes[i]);
        }
    }

    // Cleanup
    tj3Destroy(handle);
    remove("input_file");
    remove("output_file");
    remove("output_file2");
    remove("output_file3");
    
    assert_file_closed(&input_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	return 0;
}