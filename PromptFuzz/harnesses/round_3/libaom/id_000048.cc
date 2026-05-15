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
//<ID> 129
//<Prompt> []
/*<Combination>: [
*/
//<score> 30, nr_unique_branch: 0
//<Quality> {"density":30,"unique_branches":{},"library_calls":["aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_error_detail","aom_codec_destroy","aom_codec_get_frame","aom_codec_destroy","aom_img_num_metadata","aom_img_get_metadata","aom_img_remove_metadata","aom_img_plane_width","aom_img_plane_height","aom_img_alloc","aom_codec_destroy","aom_img_plane_width","aom_img_plane_height","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_img_free","aom_codec_destroy","aom_codec_set_option","aom_codec_set_option","aom_codec_set_cx_data_buf","aom_uleb_encode","aom_uleb_encode_fixed_size","aom_codec_encode","aom_codec_error_detail","aom_codec_get_cx_data","aom_img_free","aom_codec_destroy","aom_codec_destroy"],"critical_calls":["aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_get_frame","aom_img_num_metadata","aom_img_get_metadata","aom_img_remove_metadata","aom_img_plane_width","aom_img_plane_height","aom_img_alloc","aom_img_plane_width","aom_img_plane_height","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_option","aom_codec_set_option","aom_codec_set_cx_data_buf","aom_uleb_encode","aom_uleb_encode_fixed_size","aom_codec_encode","aom_codec_error_detail","aom_codec_get_cx_data","aom_img_free","aom_codec_destroy","aom_codec_destroy"],"visited":0}
/*Here's a C++ fuzz driver that uses all the specified libaom APIs to process AV1 encoded data:

*/


extern "C" int LLVMFuzzerTestOneInput_48(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&out_file);
	return 0;
    }

    // 2. Create input file from memory buffer
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);
	assert_file_closed(&out_file);
	return 0;
    }

    // 3. Get file descriptor for input file
    int in_fd = fuzz_fileno(in_file);
    (void)in_fd; // Use to avoid unused variable warning

    // 4. Initialize decoder context
    aom_codec_ctx_t decoder;
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    aom_codec_dec_cfg_t dec_cfg = aom_codec_dec_cfg_t();
    dec_cfg.threads = 1;
    
    aom_codec_err_t res = aom_codec_dec_init(&decoder, decoder_iface, &dec_cfg, 0);
    if (res != AOM_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);
	assert_file_closed(&out_file);
	assert_fd_closed(in_fd);
	return 0;
    }

    // 5. Decode the input data
    res = aom_codec_decode(&decoder, data, size, nullptr);
    if (res != AOM_CODEC_OK) {
        const char *error_detail = aom_codec_error_detail(&decoder);
        (void)error_detail; // Use to avoid unused variable warning
        
        aom_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);
	assert_file_closed(&out_file);
	assert_fd_closed(in_fd);
	return 0;
    }

    // 6. Get decoded frame
    aom_codec_iter_t iter = nullptr;
    aom_image_t *decoded_img = aom_codec_get_frame(&decoder, &iter);
    
    if (!decoded_img) {
        aom_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);
	assert_file_closed(&out_file);
	assert_fd_closed(in_fd);
	return 0;
    }

    // 7. Use aom_img_get_metadata to check for metadata
    size_t num_metadata = aom_img_num_metadata(decoded_img);
    for (size_t i = 0; i < num_metadata; i++) {
        const aom_metadata_t *metadata = aom_img_get_metadata(decoded_img, i);
        (void)metadata; // Use to avoid unused variable warning
    }

    // 8. Remove metadata from image
    aom_img_remove_metadata(decoded_img);

    // 9. Get plane dimensions
    int plane_width = aom_img_plane_width(decoded_img, 0);
    int plane_height = aom_img_plane_height(decoded_img, 0);
    (void)plane_width; // Use to avoid unused variable warning
    (void)plane_height; // Use to avoid unused variable warning

    // 10. Allocate new image for encoding
    aom_image_t *img_to_encode = aom_img_alloc(nullptr, decoded_img->fmt, 
                                               decoded_img->d_w, decoded_img->d_h, 16);
    if (!img_to_encode) {
        aom_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);
	assert_file_closed(&out_file);
	assert_fd_closed(in_fd);
	return 0;
    }

    // 11. Copy image data (simplified - in real code you'd copy plane by plane)
    // This is just to demonstrate the API usage
    for (int plane = 0; plane < 3; plane++) {
        int src_stride = decoded_img->stride[plane];
        int dst_stride = img_to_encode->stride[plane];
        int copy_width = aom_img_plane_width(decoded_img, plane);
        int copy_height = aom_img_plane_height(decoded_img, plane);
        
        for (int y = 0; y < copy_height; y++) {
            memcpy(img_to_encode->planes[plane] + y * dst_stride,
                   decoded_img->planes[plane] + y * src_stride,
                   copy_width);
        }
    }

    // 12. Initialize encoder context using aom_codec_av1_cx
    aom_codec_ctx_t encoder;
    aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
    
    aom_codec_enc_cfg_t enc_cfg;
    aom_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
    enc_cfg.g_w = decoded_img->d_w;
    enc_cfg.g_h = decoded_img->d_h;
    enc_cfg.g_timebase.num = 1;
    enc_cfg.g_timebase.den = 30;
    enc_cfg.rc_target_bitrate = 1000;
    enc_cfg.g_pass = AOM_RC_ONE_PASS;
    enc_cfg.g_lag_in_frames = 0;
    enc_cfg.g_error_resilient = 0;

    res = aom_codec_enc_init(&encoder, encoder_iface, &enc_cfg, 0);
    if (res != AOM_CODEC_OK) {
        aom_img_free(img_to_encode);
        aom_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);
	assert_file_closed(&out_file);
	assert_fd_closed(in_fd);
	return 0;
    }

    // 13. Set encoder options
    aom_codec_set_option(&encoder, "cpu-used", "5");
    aom_codec_set_option(&encoder, "end-usage", "vbr");

    // 14. Prepare output buffer for encoded data
    std::vector<uint8_t> output_buffer(4096);
    aom_fixed_buf_t buf;
    buf.buf = output_buffer.data();
    buf.sz = output_buffer.size();
    
    aom_codec_set_cx_data_buf(&encoder, &buf, 0, 0);

    // 15. Test ULEB encoding functions
    uint64_t test_value = 12345;
    uint8_t uleb_buffer[16];
    size_t coded_size = 0;
    
    // Regular ULEB encoding
    int uleb_res = aom_uleb_encode(test_value, sizeof(uleb_buffer), 
                                   uleb_buffer, &coded_size);
    (void)uleb_res; // Use to avoid unused variable warning
    
    // Fixed size ULEB encoding
    size_t fixed_coded_size = 0;
    aom_uleb_encode_fixed_size(test_value, sizeof(uleb_buffer), 8,
                               uleb_buffer, &fixed_coded_size);

    // 16. Encode the image
    res = aom_codec_encode(&encoder, img_to_encode, 0, 1, 0);
    if (res != AOM_CODEC_OK) {
        const char *error_detail = aom_codec_error_detail(&encoder);
        (void)error_detail; // Use to avoid unused variable warning
    }

    // 17. Get encoded data
    iter = nullptr;
    const aom_codec_cx_pkt_t *pkt;
    while ((pkt = aom_codec_get_cx_data(&encoder, &iter)) != nullptr) {
        if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
            fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
        }
    }

    // 18. Cleanup
    aom_img_free(img_to_encode);
    aom_codec_destroy(&encoder);
    aom_codec_destroy(&decoder);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_closed(&in_file);
	assert_file_closed(&out_file);
	assert_fd_closed(in_fd);
	return 0;
}