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
#include <aom/aom.h>
}
extern "C" {
#include <aom/aom_codec.h>
}
extern "C" {
#include <aom/aom_decoder.h>
}
extern "C" {
#include <aom/aom_encoder.h>
}
extern "C" {
#include <aom/aom_ext_ratectrl.h>
}
extern "C" {
#include <aom/aom_external_partition.h>
}
extern "C" {
#include <aom/aom_frame_buffer.h>
}
extern "C" {
#include <aom/aom_image.h>
}
extern "C" {
#include <aom/aomcx.h>
}
extern "C" {
#include <aom/aomdx.h>
}
//<ID> 0
//<Prompt> []
/*<Combination>: [
*/
//<score> 6, nr_unique_branch: 0
//<Quality> {"density":6,"unique_branches":{},"library_calls":["aom_codec_version_str","aom_img_alloc","aom_uleb_size_in_bytes","aom_codec_av1_dx","aom_codec_iface_name","aom_codec_get_caps","aom_img_add_metadata","aom_img_num_metadata","aom_img_metadata_alloc","aom_img_metadata_free","aom_img_free"],"critical_calls":["aom_codec_version_str","aom_img_alloc","aom_uleb_size_in_bytes","aom_codec_av1_dx","aom_codec_iface_name","aom_codec_get_caps","aom_img_add_metadata","aom_img_num_metadata","aom_img_metadata_alloc","aom_img_metadata_free","aom_img_free"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_0(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 16) {
        return 0;
    }

    // 1. Get codec version string
    const char *version_str = aom_codec_version_str();
    if (version_str) {
        // Use the version string to avoid optimization
        volatile const char *v = version_str;
        (void)v;
    }

    // 2. Create a FILE* from input data for reading
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // 3. Get file descriptor for reading
    int fd_in = fuzz_fileno(in_file);
    if (fd_in < 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
    }

    // 4. Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
    }

    // 5. Get file descriptor for writing
    int fd_out = fuzz_fileno(out_file);
    if (fd_out < 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }

    // 6. Allocate an image
    aom_image_t img;
    memset(&img, 0, sizeof(img));
    
    // Use first 4 bytes as width, next 4 as height
    unsigned int width = 64;
    unsigned int height = 64;
    if (size >= 8) {
        width = *reinterpret_cast<const unsigned int*>(data) % 256 + 16;
        height = *reinterpret_cast<const unsigned int*>(data + 4) % 256 + 16;
    }
    
    aom_image_t *image = aom_img_alloc(&img, AOM_IMG_FMT_I420, width, height, 16);
    if (!image) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }

    // 7. Calculate ULEB size for a value from input
    uint64_t uleb_value = 0;
    if (size >= 16) {
        uleb_value = *reinterpret_cast<const uint64_t*>(data + 8);
    }
    size_t uleb_size = aom_uleb_size_in_bytes(uleb_value);
    
    // 8. Get codec interface and capabilities
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (iface) {
        const char *iface_name = aom_codec_iface_name(iface);
        if (iface_name) {
            volatile const char *n = iface_name;
            (void)n;
        }
        
        aom_codec_caps_t caps = aom_codec_get_caps(iface);
        volatile aom_codec_caps_t c = caps;
        (void)c;
    }

    // 9. Add metadata to the image using remaining input data
    if (size > 20) {
        size_t metadata_size = size - 20;
        if (metadata_size > 0) {
            uint32_t metadata_type = 1; // Arbitrary type
            aom_metadata_insert_flags_t insert_flag = AOM_MIF_ANY_FRAME;
            
            int result = aom_img_add_metadata(image, metadata_type, 
                                             data + 20, metadata_size, insert_flag);
            volatile int r = result;
            (void)r;
            
            // 10. Get metadata count and free metadata if any
            size_t num_metadata = aom_img_num_metadata(image);
            if (num_metadata > 0) {
                // Allocate a separate metadata structure to test aom_img_metadata_free
                aom_metadata_t *metadata = aom_img_metadata_alloc(
                    metadata_type, data + 20, 
                    metadata_size > 1024 ? 1024 : metadata_size, 
                    insert_flag);
                
                if (metadata) {
                    aom_img_metadata_free(metadata);
                }
            }
        }
    }

    // 11. Clean up
    aom_img_free(image);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
}