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
//<ID> 30
//<Prompt> []
/*<Combination>: [
*/
//<score> 21, nr_unique_branch: 0
//<Quality> {"density":21,"unique_branches":{},"library_calls":["aom_uleb_encode","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_error_detail","aom_codec_set_frame_buffer_functions","aom_codec_destroy","aom_codec_destroy","aom_img_wrap","aom_codec_destroy","aom_codec_enc_config_set","aom_img_free","aom_codec_destroy","aom_codec_set_cx_data_buf","aom_img_free","aom_codec_destroy","aom_img_add_metadata","aom_codec_encode","aom_codec_error_detail","aom_codec_get_cx_data","aom_img_free","aom_codec_destroy"],"critical_calls":["aom_uleb_encode","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_frame_buffer_functions","aom_img_wrap","aom_codec_enc_config_set","aom_codec_set_cx_data_buf","aom_img_add_metadata","aom_codec_encode","aom_codec_error_detail","aom_codec_get_cx_data","aom_img_free","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Frame buffer callback structures
struct BufferCallbackData {
    uint8_t* buffer;
    size_t size;
    bool used;
};

// Frame buffer get callback
int  id_000021_get_frame_buffer(void* cb_priv, size_t min_size, aom_codec_frame_buffer_t* fb) {
    BufferCallbackData* data = static_cast<BufferCallbackData*>(cb_priv);
    
    if (data->used) {
        // Return error if buffer already used
        return -1;
    }
    
    if (min_size > data->size) {
        // Reallocate buffer if too small
        data->buffer = static_cast<uint8_t*>(realloc(data->buffer, min_size));
        if (!data->buffer) return -1;
        data->size = min_size;
    }
    
    fb->data = data->buffer;
    fb->size = data->size;
    data->used = true;
    
    return 0;
}

// Frame buffer release callback
int  id_000021_release_frame_buffer(void* cb_priv, aom_codec_frame_buffer_t* fb) {
    BufferCallbackData* data = static_cast<BufferCallbackData*>(cb_priv);
    data->used = false;
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_21(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early return if input is too small
    if (size < 100) {
	return 0;
	}
	
    
    // Create input file from fuzzer data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create output file
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 1. Use aom_uleb_encode to encode a value from the input
    size_t coded_size = 0;
    uint8_t coded_value[16];
    uint64_t uleb_value = *reinterpret_cast<const uint64_t*>(data);
    
    if (aom_uleb_encode(uleb_value, sizeof(coded_value), coded_value, &coded_size) != 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 2. Get AV1 encoder interface
    aom_codec_iface_t* iface = aom_codec_av1_cx();
    if (!iface) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 3. Create encoder configuration
    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Configure basic settings
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;
    
    // 4. Initialize encoder context
    aom_codec_ctx_t encoder;
    if (aom_codec_enc_init_ver(&encoder, iface, &cfg, 0, AOM_ENCODER_ABI_VERSION) != AOM_CODEC_OK) {
        // Use aom_codec_error_detail to get error information
        const char* detail = aom_codec_error_detail(&encoder);
        (void)detail; // Use detail to avoid unused variable warning
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 5. Set frame buffer functions
    BufferCallbackData cb_data;
    cb_data.buffer = static_cast<uint8_t*>(malloc(4096));
    cb_data.size = 4096;
    cb_data.used = false;
    
    if (aom_codec_set_frame_buffer_functions(&encoder,  id_000021_get_frame_buffer, 
                                            id_000021_release_frame_buffer, &cb_data) != AOM_CODEC_OK) {
        free(cb_data.buffer);
        aom_codec_destroy(&encoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 6. Create an image wrapper using input data
    aom_image_t img;
    unsigned int img_width = 64;
    unsigned int img_height = 64;
    
    // Allocate image buffer from input data
    size_t img_data_size = img_width * img_height * 3 / 2;
    uint8_t* img_data = static_cast<uint8_t*>(malloc(img_data_size));
    if (!img_data) {
        free(cb_data.buffer);
        aom_codec_destroy(&encoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Read image data from input file
    fseek(in_file, 0, SEEK_SET);
    size_t read_size = fread(img_data, 1, std::min(img_data_size, size), in_file);
    if (read_size < img_data_size) {
        // Pad with zeros if input is smaller
        memset(img_data + read_size, 0, img_data_size - read_size);
    }
    
    aom_image_t* wrapped_img = aom_img_wrap(&img, AOM_IMG_FMT_I420, img_width, img_height, 
                                           1, img_data);
    if (!wrapped_img) {
        free(img_data);
        free(cb_data.buffer);
        aom_codec_destroy(&encoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 7. Update encoder configuration
    cfg.g_pass = AOM_RC_ONE_PASS;
    if (aom_codec_enc_config_set(&encoder, &cfg) != AOM_CODEC_OK) {
        aom_img_free(&img);
        free(img_data);
        free(cb_data.buffer);
        aom_codec_destroy(&encoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 8. Set compressed data buffer
    aom_fixed_buf_t output_buf;
    output_buf.buf = malloc(8192);
    output_buf.sz = 8192;
    
    if (aom_codec_set_cx_data_buf(&encoder, &output_buf, 0, 0) != AOM_CODEC_OK) {
        free(output_buf.buf);
        aom_img_free(&img);
        free(img_data);
        free(cb_data.buffer);
        aom_codec_destroy(&encoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 9. Add metadata to image
    uint32_t metadata_type = 1;
    uint8_t metadata_data[32];
    memcpy(metadata_data, data + 8, std::min(size_t(32), size - 8));
    
    if (aom_img_add_metadata(&img, metadata_type, metadata_data, 
                           std::min(size_t(32), size - 8), 
                           AOM_MIF_ANY_FRAME) != 0) {
        // Continue even if metadata addition fails
    }
    
    // 10. Encode the image
    aom_codec_pts_t pts = 0;
    unsigned long duration = 1;
    aom_enc_frame_flags_t flags = 0;
    
    if (aom_codec_encode(&encoder, &img, pts, duration, flags) != AOM_CODEC_OK) {
        // Try to get error detail
        const char* detail = aom_codec_error_detail(&encoder);
        (void)detail;
    }
    
    // 11. Get encoded data
    aom_codec_iter_t iter = nullptr;
    const aom_codec_cx_pkt_t* pkt;
    while ((pkt = aom_codec_get_cx_data(&encoder, &iter)) != nullptr) {
        if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
            // Write encoded frame to output file
            fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
        }
    }
    
    // Write uleb encoded value to output file
    fwrite(coded_value, 1, coded_size, out_file);
    
    // 12. Clean up image
    aom_img_free(&img);
    free(img_data);
    
    // 13. Clean up encoder
    aom_codec_destroy(&encoder);
    
    // Clean up other resources
    free(output_buf.buf);
    free(cb_data.buffer);
    
    // Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}