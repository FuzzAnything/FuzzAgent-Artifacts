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
//<ID> 128
//<Prompt> []
/*<Combination>: [
*/
//<score> 19, nr_unique_branch: 0
//<Quality> {"density":19,"unique_branches":{},"library_calls":["aom_codec_av1_cx","aom_img_alloc","aom_img_plane_height","aom_img_plane_height","aom_img_plane_height","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_img_free","aom_codec_set_option","aom_codec_set_option","aom_codec_set_option","aom_codec_encode","aom_codec_get_cx_data","aom_codec_set_cx_data_buf","aom_uleb_encode","aom_uleb_encode_fixed_size","aom_img_plane_width","aom_img_plane_height","aom_img_add_metadata","aom_img_get_metadata","aom_img_remove_metadata","aom_codec_error_detail","aom_codec_destroy","aom_img_free"],"critical_calls":["aom_codec_av1_cx","aom_img_alloc","aom_img_plane_height","aom_img_plane_height","aom_img_plane_height","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_option","aom_codec_set_option","aom_codec_set_option","aom_codec_encode","aom_codec_get_cx_data","aom_codec_set_cx_data_buf","aom_uleb_encode","aom_uleb_encode_fixed_size","aom_img_plane_width","aom_img_plane_height","aom_img_add_metadata","aom_img_get_metadata","aom_img_remove_metadata","aom_codec_error_detail","aom_codec_destroy","aom_img_free"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_47(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data for processing

    // Create a FILE* from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	

    // 1. Create AV1 encoder interface
    aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
    if (!encoder_iface) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // 2. Allocate image
    aom_image_t *img = aom_img_alloc(NULL, AOM_IMG_FMT_I420, 64, 64, 1);
    if (!img) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // 3. Read some data for image planes
    fseek(in_file, 0, SEEK_SET);
    size_t bytes_read = fread(img->planes[0], 1, 
                             img->stride[0] * aom_img_plane_height(img, 0), in_file);
    if (bytes_read > 0 && fseek(in_file, bytes_read, SEEK_CUR) == 0) {
        // Read more for other planes if available
        if (img->fmt == AOM_IMG_FMT_I420) {
            fread(img->planes[1], 1, 
                  img->stride[1] * aom_img_plane_height(img, 1), in_file);
            fread(img->planes[2], 1, 
                  img->stride[2] * aom_img_plane_height(img, 2), in_file);
        }
    }

    // 4. Create encoder context
    aom_codec_ctx_t encoder;
    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(encoder_iface, &cfg, 0);
    cfg.g_w = 64;
    cfg.g_h = 64;
    
    if (aom_codec_enc_init_ver(&encoder, encoder_iface, &cfg, 0, AOM_ENCODER_ABI_VERSION)) {
        aom_img_free(img);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // 5. Set encoder options
    aom_codec_set_option(&encoder, "cpu-used", "1");
    aom_codec_set_option(&encoder, "end-usage", "vbr");
    aom_codec_set_option(&encoder, "cq-level", "32");

    // 6. Encode the image
    aom_codec_encode(&encoder, img, 0, 1, 0);

    // 7. Get compressed data
    aom_codec_iter_t iter = NULL;
    const aom_codec_cx_pkt_t *pkt;
    std::vector<uint8_t> compressed_data;
    
    while ((pkt = aom_codec_get_cx_data(&encoder, &iter)) != NULL) {
        if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
            compressed_data.insert(compressed_data.end(),
                                  (uint8_t*)pkt->data.frame.buf,
                                  (uint8_t*)pkt->data.frame.buf + pkt->data.frame.sz);
        }
    }

    // 8. Set compressed data buffer for potential reuse
    aom_fixed_buf_t buf;
    buf.buf = compressed_data.data();
    buf.sz = compressed_data.size();
    aom_codec_set_cx_data_buf(&encoder, &buf, 0, 0);

    // 9. ULEB encoding operations
    uint64_t uleb_value = 12345;
    uint8_t uleb_buffer[10];
    size_t coded_size = 0;
    
    // Regular ULEB encoding
    aom_uleb_encode(uleb_value, sizeof(uleb_buffer), uleb_buffer, &coded_size);
    
    // Fixed size ULEB encoding
    size_t fixed_coded_size = 0;
    aom_uleb_encode_fixed_size(uleb_value, sizeof(uleb_buffer), 5, 
                              uleb_buffer, &fixed_coded_size);

    // 10. Check image plane dimensions
    int plane_width = aom_img_plane_width(img, 0);
    int plane_height = aom_img_plane_height(img, 0);

    // 11. Metadata operations
    // Add some metadata
    uint32_t metadata_type = 1;
    uint8_t metadata_data[] = {1, 2, 3, 4, 5};
    aom_img_add_metadata(img, metadata_type, metadata_data, 
                        sizeof(metadata_data), AOM_MIF_ANY_FRAME);

    // Get metadata
    const aom_metadata_t *metadata = aom_img_get_metadata(img, 0);
    
    // Remove metadata
    aom_img_remove_metadata(img);

    // 12. Get error details (if any)
    const char *error_detail = aom_codec_error_detail(&encoder);

    // 13. Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // Write compressed data to file
        if (!compressed_data.empty()) {
            fwrite(compressed_data.data(), 1, compressed_data.size(), out_file);
        }
        
        // Write ULEB encoded data
        fwrite(uleb_buffer, 1, coded_size, out_file);
        
        assert_file_closed(&out_file);;
    }

    // 14. Cleanup
    aom_codec_destroy(&encoder);
    aom_img_free(img);
    assert_file_closed(&in_file);;

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}