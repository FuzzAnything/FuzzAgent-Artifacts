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
//<ID> 70
//<Prompt> []
/*<Combination>: [
*/
//<score> 111, nr_unique_branch: 2
//<Quality> {"density":37,"unique_branches":{"encoder_set_config":[[1681,9,1681,34,0,0,4,0]],"align_image_dimension":[[30,11,30,35,0,0,4,0]]},"library_calls":["aom_codec_av1_dx","aom_codec_get_caps","aom_codec_dec_init_ver","aom_codec_destroy","aom_codec_decode","aom_codec_destroy","aom_codec_get_stream_info","aom_codec_destroy","aom_codec_get_frame","aom_codec_destroy","aom_img_alloc_with_border","aom_codec_destroy","aom_img_plane_height","aom_img_plane_width","aom_img_add_metadata","aom_img_num_metadata","aom_img_remove_metadata","aom_codec_av1_cx","aom_codec_enc_config_default","aom_img_free","aom_codec_destroy","aom_codec_enc_init_ver","aom_img_free","aom_codec_destroy","aom_codec_enc_config_set","aom_codec_destroy","aom_img_free","aom_codec_destroy","aom_codec_control","aom_codec_destroy","aom_img_free","aom_codec_destroy","aom_codec_encode","aom_codec_get_cx_data","aom_codec_destroy","aom_img_free","aom_codec_destroy"],"critical_calls":["aom_codec_av1_dx","aom_codec_get_caps","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_get_stream_info","aom_codec_get_frame","aom_img_alloc_with_border","aom_img_plane_height","aom_img_plane_width","aom_img_add_metadata","aom_img_num_metadata","aom_img_remove_metadata","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_enc_config_set","aom_codec_control","aom_codec_encode","aom_codec_get_cx_data","aom_codec_destroy","aom_img_free","aom_codec_destroy"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified libaom APIs to achieve the event of decoding a compressed bitstream, manipulating metadata on the decoded image, and re-encoding it:

*/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_27(const uint8_t* f_data, size_t f_size) {
	if(f_size<64) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_6, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_7, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_8, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_9, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_10, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_11, fdp);
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data for meaningful test
    
    // Initialize variables
    aom_codec_ctx_t decoder = {0};
    aom_codec_ctx_t encoder = {0};
    aom_image_t *image = nullptr;
    aom_codec_stream_info_t stream_info = {0};
    aom_codec_enc_cfg_t enc_cfg = {0};
    FILE *in_file = nullptr;
    FILE *out_file = nullptr;
    
    // 1. Create input file from fuzzer data
    in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 2. Get decoder interface capabilities
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    aom_codec_caps_t caps = aom_codec_get_caps(decoder_iface);
    (void)caps; // Use caps to satisfy requirement
    
    // 3. Initialize decoder
    aom_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    
    if (aom_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, fuzz_int64_t_1, 
                               AOM_DECODER_ABI_VERSION) != AOM_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 4. Read some data for decoding
    std::vector<uint8_t> buffer(size);
    size_t bytes_read = fread(buffer.data(), 1, size, in_file);
    assert_file_closed(&in_file);;
    in_file = nullptr;
    
    if (bytes_read == 0) {
        aom_codec_destroy(&decoder);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 5. Decode the data
    if (aom_codec_decode(&decoder, buffer.data(), bytes_read, nullptr) != AOM_CODEC_OK) {
        aom_codec_destroy(&decoder);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 6. Get stream information
    if (aom_codec_get_stream_info(&decoder, &stream_info) != AOM_CODEC_OK) {
        aom_codec_destroy(&decoder);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 7. Get decoded frame
    aom_codec_iter_t iter = nullptr;
    image = aom_codec_get_frame(&decoder, &iter);
    if (!image) {
        aom_codec_destroy(&decoder);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 8. Create a new image with border for re-encoding
    aom_image_t *new_image = aom_img_alloc_with_border(nullptr, image->fmt, 
                                                      image->d_w, image->d_h,
                                                      fuzz_uint32_t_2, fuzz_uint32_t_3, fuzz_uint32_t_4);
    if (!new_image) {
        aom_codec_destroy(&decoder);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 9. Copy image data (simplified - in reality would copy plane by plane)
    // This is a simplified copy - actual implementation would need to handle
    // different formats and planes properly
    if (image->fmt == AOM_IMG_FMT_I420 || image->fmt == AOM_IMG_FMT_I42016) {
        for (int plane = 0; plane < 3; plane++) {
            int src_stride = image->stride[plane];
            int dst_stride = new_image->stride[plane];
            int height = aom_img_plane_height(image, plane);
            int width = aom_img_plane_width(image, fuzz_int32_t_5);
            
            for (int y = 0; y < height; y++) {
                memcpy(new_image->planes[plane] + y * dst_stride,
                       image->planes[plane] + y * src_stride,
                       width * (image->fmt == AOM_IMG_FMT_I42016 ? 2 : 1));
            }
        }
    }
    
    // 10. Add metadata to the image
    uint32_t metadata_type = 1; // Example metadata type
    std::vector<uint8_t> metadata_data = {'T', 'E', 'S', 'T', 0x01, 0x02, 0x03};
    int add_result = aom_img_add_metadata(new_image, fuzz_uint32_t_6, 
                                         metadata_data.data(), 
                                         metadata_data.size(),
                                         AOM_MIF_ANY_FRAME);
    (void)add_result; // Use result to satisfy requirement
    
    // 11. Check number of metadata entries
    size_t metadata_count = aom_img_num_metadata(new_image);
    (void)metadata_count; // Use count to satisfy requirement
    
    // 12. Remove metadata
    aom_img_remove_metadata(new_image);
    
    // 13. Initialize encoder
    aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
    
    if (aom_codec_enc_config_default(encoder_iface, &enc_cfg, fuzz_uint32_t_7) != AOM_CODEC_OK) {
        aom_img_free(new_image);
        aom_codec_destroy(&decoder);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Set basic encoder configuration
    enc_cfg.g_w = new_image->d_w;
    enc_cfg.g_h = new_image->d_h;
    enc_cfg.g_timebase.num = 1;
    enc_cfg.g_timebase.den = 30;
    enc_cfg.rc_target_bitrate = 1000;
    enc_cfg.g_error_resilient = 0;
    
    // 14. Set encoder configuration
    if (aom_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, fuzz_int64_t_8,
                               AOM_ENCODER_ABI_VERSION) != AOM_CODEC_OK) {
        aom_img_free(new_image);
        aom_codec_destroy(&decoder);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 15. Set encoder configuration again (for testing)
    if (aom_codec_enc_config_set(&encoder, &enc_cfg) != AOM_CODEC_OK) {
        aom_codec_destroy(&encoder);
        aom_img_free(new_image);
        aom_codec_destroy(&decoder);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 16. Use codec control to set a parameter
    int ctrl_value = 8; // CPU used
    if (aom_codec_control(&encoder, AOME_SET_CPUUSED, ctrl_value) != AOM_CODEC_OK) {
        // Continue even if control fails
    }
    
    // 17. Create output file
    out_file = fopen("output_file", "wb");
    if (!out_file) {
        aom_codec_destroy(&encoder);
        aom_img_free(new_image);
        aom_codec_destroy(&decoder);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 18. Encode the image
    aom_codec_pts_t pts = 0;
    unsigned long duration = 1;
    aom_enc_frame_flags_t flags = 0;
    
    if (aom_codec_encode(&encoder, new_image, fuzz_int64_t_9, fuzz_uint64_t_10, fuzz_int64_t_11) != AOM_CODEC_OK) {
        // Continue to cleanup even if encode fails
    }
    
    // 19. Get encoded data
    aom_codec_iter_t enc_iter = nullptr;
    const aom_codec_cx_pkt_t *pkt = nullptr;
    
    while ((pkt = aom_codec_get_cx_data(&encoder, &enc_iter)) != nullptr) {
        if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
            fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
        }
    }
    
    // 20. Cleanup
    assert_file_closed(&out_file);;
    out_file = nullptr;
    
    // Remove the output file
    remove("output_file");
    
    aom_codec_destroy(&encoder);
    aom_img_free(new_image);
    aom_codec_destroy(&decoder);
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}