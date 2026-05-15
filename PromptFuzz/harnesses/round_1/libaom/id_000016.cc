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
//<ID> 27
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 0
//<Quality> {"density":18,"unique_branches":{},"library_calls":["aom_uleb_decode","aom_uleb_encode_fixed_size","aom_codec_av1_dx","aom_codec_get_caps","aom_codec_peek_stream_info","aom_codec_dec_init_ver","aom_codec_err_to_string","aom_codec_set_option","aom_codec_decode","aom_codec_err_to_string","aom_codec_destroy","aom_codec_get_stream_info","aom_codec_get_frame","aom_img_alloc","aom_img_add_metadata","aom_img_flip","aom_img_alloc_with_border","aom_img_free","aom_codec_destroy"],"critical_calls":["aom_uleb_decode","aom_uleb_encode_fixed_size","aom_codec_av1_dx","aom_codec_get_caps","aom_codec_peek_stream_info","aom_codec_dec_init_ver","aom_codec_set_option","aom_codec_decode","aom_codec_get_stream_info","aom_codec_get_frame","aom_img_alloc","aom_img_add_metadata","aom_img_flip","aom_img_alloc_with_border","aom_img_free","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_16(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // Variables for ULEB operations
    uint64_t uleb_value = 0;
    size_t uleb_length = 0;
    size_t coded_size = 0;
    const size_t MAX_ULEB_BYTES = 16;
    uint8_t uleb_buffer[MAX_ULEB_BYTES];

    // 1. Use aom_uleb_decode on a portion of the input data
    if (size >= 4) {
        size_t decode_len = size < 10 ? size : 10;
        aom_uleb_decode(data, decode_len, &uleb_value, &uleb_length);
    }

    // 2. Use aom_uleb_encode_fixed_size to encode the decoded value (or a constant)
    size_t available_size = sizeof(uleb_buffer);
    aom_uleb_encode_fixed_size(uleb_value, available_size, 8, uleb_buffer, &coded_size);

    // 3. Get the AV1 decoder interface
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (!iface) {
        return 0;
    }

    // 4. Get codec capabilities
    aom_codec_caps_t caps = aom_codec_get_caps(iface);

    // 5. Peek stream info using the input data
    aom_codec_stream_info_t stream_info;
    memset(&stream_info, 0, sizeof(stream_info));
    aom_codec_peek_stream_info(iface, data, size, &stream_info);

    // 6. Initialize decoder
    aom_codec_ctx_t codec_ctx;
    memset(&codec_ctx, 0, sizeof(codec_ctx));
    aom_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 2;
    dec_cfg.allow_lowbitdepth = 1;

    aom_codec_err_t ret = aom_codec_dec_init_ver(&codec_ctx, iface, &dec_cfg, 0, AOM_DECODER_ABI_VERSION);
    if (ret != AOM_CODEC_OK) {
        const char* error_str = aom_codec_err_to_string(ret);
        (void)error_str; // Avoid unused warning
        return 0;
    }

    // 7. Set a decoder option (example: enable error resilient mode)
    aom_codec_set_option(&codec_ctx, "error-resilient", "1");

    // 8. Decode the input data
    ret = aom_codec_decode(&codec_ctx, data, size, nullptr);
    if (ret != AOM_CODEC_OK) {
        const char* error_str = aom_codec_err_to_string(ret);
        (void)error_str; // Avoid unused warning
        aom_codec_destroy(&codec_ctx);
        return 0;
    }

    // 9. Get stream info again after decoding
    aom_codec_get_stream_info(&codec_ctx, &stream_info);

    // 10. Get decoded frames
    aom_codec_iter_t iter = nullptr;
    const aom_image_t *decoded_img = nullptr;
    std::vector<aom_image_t*> allocated_images;

    while ((decoded_img = aom_codec_get_frame(&codec_ctx, &iter)) != nullptr) {
        // 11. Allocate a new image using aom_img_alloc
        aom_image_t *img1 = aom_img_alloc(nullptr, AOM_IMG_FMT_I420, 
                                         decoded_img->d_w, decoded_img->d_h, 16);
        if (img1) {
            allocated_images.push_back(img1);
            
            // 12. Add metadata to the image (use some data from input)
            size_t metadata_size = size > 100 ? 100 : size;
            uint32_t metadata_type = 1; // Example type
            aom_img_add_metadata(img1, metadata_type, data, metadata_size, 
                                AOM_MIF_ANY_FRAME);
            
            // 13. Flip the image
            aom_img_flip(img1);
        }

        // 14. Allocate another image with border using aom_img_alloc_with_border
        aom_image_t *img2 = aom_img_alloc_with_border(nullptr, AOM_IMG_FMT_I420,
                                                     decoded_img->d_w, decoded_img->d_h,
                                                     16, 32, 8);
        if (img2) {
            allocated_images.push_back(img2);
        }

        // Break after processing first frame to avoid infinite loop
        break;
    }

    // 15. Clean up allocated images
    for (aom_image_t *img : allocated_images) {
        aom_img_free(img);
    }

    // 16. Destroy codec context
    aom_codec_destroy(&codec_ctx);

    // 17. Use FILE* operations as requested
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        // Read a small portion
        uint8_t file_buffer[64];
        fread(file_buffer, 1, sizeof(file_buffer) < size ? sizeof(file_buffer) : size, in_file);
        assert_file_closed(&in_file);;
    }

    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        fwrite(uleb_buffer, 1, coded_size, out_file);
        assert_file_closed(&out_file);;
    }

    // 18. Use file descriptors if needed
    if (in_file) {
        int fd_in = fuzz_fileno(in_file);
        (void)fd_in; // Avoid unused warning
    }
    if (out_file) {
        int fd_out = fuzz_fileno(out_file);
        (void)fd_out; // Avoid unused warning
    }

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}