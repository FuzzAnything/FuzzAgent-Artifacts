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
//<ID> 676
//<Prompt> []
/*<Combination>: [
*/
//<score> 16, nr_unique_branch: 2
//<Quality> {"density":24,"unique_branches":{"examine_app14":[[691,7,691,22,0,0,4,0],[692,7,692,22,0,0,4,1]]},"library_calls":["tjInitDecompress","tj3InitVersion","tjDestroy","tj3Destroy","tj3LoadImage12","tjDecompressHeader","tj3DecompressHeader","tj3Get","tj3Get","tj3YUVBufSize","tjPlaneSizeYUV","tj3Alloc","tj3DecompressToYUV8","tj3Alloc","tj3Alloc","tjDecodeYUVPlanes","tj3DecodeYUV8","tj3SaveImage8","tj3Free","tj3Free","tj3Free","tj3Free","tjDestroy","tj3Destroy"],"critical_calls":["tjInitDecompress","tj3InitVersion","tj3LoadImage12","tjDecompressHeader","tj3DecompressHeader","tj3Get","tj3Get","tj3YUVBufSize","tjPlaneSizeYUV","tj3Alloc","tj3DecompressToYUV8","tj3Alloc","tj3Alloc","tjDecodeYUVPlanes","tj3DecodeYUV8","tj3SaveImage8","tj3Free","tj3Free","tj3Free","tj3Free","tjDestroy","tj3Destroy"],"visited":2}
/**/


// Include libjpeg-turbo headers
extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_38(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





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
    
    // Initialize handles
    tjhandle handle_old = tjInitDecompress();
    tjhandle handle_new = tj3Init(TJINIT_DECOMPRESS);
    
    if (!handle_old || !handle_new) {
        if (handle_old) tjDestroy(handle_old);
        if (handle_new) tj3Destroy(handle_new);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Try to load as 12-bit image (tj3LoadImage12)
    int width_load = 0, height_load = 0, pixelFormat_load = 0;
    fseek(in_file, 0, SEEK_SET);
    short *image12 = tj3LoadImage12(handle_new, "input_file", &width_load, 4, &height_load, &pixelFormat_load);
    
    // If loading failed, try to process as JPEG data
    if (!image12 && size > 0) {
        // Reset file pointer
        fseek(in_file, 0, SEEK_SET);
        
        // Read JPEG data from memory
        std::vector<unsigned char> jpeg_data(size);
        fread(jpeg_data.data(), 1, size, in_file);
        
        if (jpeg_data.size() > 0) {
            // Use old API to decompress header (tjDecompressHeader)
            int width_old = 0, height_old = 0;
            tjDecompressHeader(handle_old, jpeg_data.data(), jpeg_data.size(), &width_old, &height_old);
            
            // Use new API to get header info
            int width = 0, height = 0;
            tj3DecompressHeader(handle_new, jpeg_data.data(), jpeg_data.size());
            width = tj3Get(handle_new, TJPARAM_JPEGWIDTH);
            height = tj3Get(handle_new, TJPARAM_JPEGHEIGHT);
            
            // Use width and height from whichever succeeded
            int final_width = (width > 0) ? width : width_old;
            int final_height = (height > 0) ? height : height_old;
            
            if (final_width > 0 && final_height > 0) {
                // Calculate YUV buffer size (tj3YUVBufSize)
                int align = 4;
                int subsamp = TJSAMP_420;
                size_t yuv_buf_size = tj3YUVBufSize(final_width, align, final_height, subsamp);
                
                // Calculate plane size (tjPlaneSizeYUV)
                int componentID = 0;
                int stride = final_width;
                unsigned long plane_size = tjPlaneSizeYUV(componentID, final_width, stride, final_height, subsamp);
                
                if (yuv_buf_size > 0 && plane_size > 0) {
                    // Allocate YUV buffer
                    unsigned char *yuv_buf = (unsigned char *)tj3Alloc(yuv_buf_size);
                    if (yuv_buf) {
                        // Decompress to YUV
                        tj3DecompressToYUV8(handle_new, jpeg_data.data(), jpeg_data.size(), yuv_buf, align);
                        
                        // Allocate destination buffer for RGB
                        int pixel_format = TJPF_RGB;
                        int pitch = final_width * tjPixelSize[pixel_format];
                        unsigned char *rgb_buf1 = (unsigned char *)tj3Alloc(final_height * pitch);
                        unsigned char *rgb_buf2 = (unsigned char *)tj3Alloc(final_height * pitch);
                        
                        if (rgb_buf1 && rgb_buf2) {
                            // Use old API to decode YUV planes (tjDecodeYUVPlanes)
                            // First, prepare plane pointers for planar YUV
                            const unsigned char *planes[3];
                            int strides[3];
                            
                            // Calculate plane offsets (simplified - actual layout depends on subsampling)
                            strides[0] = final_width;  // Y plane
                            strides[1] = final_width / 2;  // U plane (420 subsampling)
                            strides[2] = final_width / 2;  // V plane (420 subsampling)
                            
                            planes[0] = yuv_buf;
                            planes[1] = yuv_buf + final_width * final_height;
                            planes[2] = planes[1] + (final_width/2) * (final_height/2);
                            
                            tjDecodeYUVPlanes(handle_old, planes, strides, subsamp, 
                                            rgb_buf1, final_width, pitch, final_height, 
                                            pixel_format, 0);
                            
                            // Use new API to decode YUV (tj3DecodeYUV8)
                            tj3DecodeYUV8(handle_new, yuv_buf, align, rgb_buf2, 
                                        final_width, pitch, final_height, pixel_format);
                            
                            // Save one of the decoded images (tj3SaveImage8)
                            tj3SaveImage8(handle_new, "output_file", rgb_buf1, 
                                        final_width, pitch, final_height, pixel_format);
                            
                            tj3Free(rgb_buf2);
                            tj3Free(rgb_buf1);
                        }
                        
                        tj3Free(yuv_buf);
                    }
                }
            }
        }
    }
    
    // Clean up
    if (image12) {
        tj3Free(image12);
    }
    
    tjDestroy(handle_old);
    tj3Destroy(handle_new);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove temporary files
    remove("input_file");
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
}