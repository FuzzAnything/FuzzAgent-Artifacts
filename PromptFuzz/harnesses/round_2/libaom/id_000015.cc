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
//<score> 9, nr_unique_branch: 0
//<Quality> {"density":9,"unique_branches":{},"library_calls":["aom_uleb_decode","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_set_option","aom_codec_set_frame_buffer_functions","aom_codec_decode","aom_codec_get_stream_info","aom_codec_control","aom_codec_get_preview_frame","aom_img_alloc","aom_img_set_rect","aom_img_remove_metadata","aom_img_free","aom_codec_destroy"],"critical_calls":["aom_uleb_decode","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_set_option","aom_codec_set_frame_buffer_functions","aom_codec_decode","aom_codec_get_stream_info","aom_codec_control","aom_codec_get_preview_frame","aom_img_alloc","aom_img_set_rect","aom_img_remove_metadata","aom_img_free","aom_codec_destroy"],"visited":0}
/**/


// libaom headers
extern "C" {
}

// Frame buffer callback context
struct FrameBufferContext {
    std::vector<uint8_t> buffer;
    size_t buffer_size = 0;
};

// Frame buffer get callback
static int  id_000015_get_frame_buffer(void* cb_priv, size_t min_size, 
                          aom_codec_frame_buffer_t* fb) {
    FrameBufferContext* ctx = static_cast<FrameBufferContext*>(cb_priv);
    
    if (!ctx || !fb) {
        return -1;
    }
    
    // Resize buffer if needed
    if (ctx->buffer.size() < min_size) {
        ctx->buffer.resize(min_size);
        ctx->buffer_size = min_size;
    }
    
    fb->data = ctx->buffer.data();
    fb->size = ctx->buffer_size;
    return 0;
}

// Frame buffer release callback
static int  id_000015_release_frame_buffer(void* cb_priv, 
                               aom_codec_frame_buffer_t* fb) {
    // Nothing to do here - we keep the buffer allocated
    (void)cb_priv;
    (void)fb;
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_15(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
        // Need minimum data for meaningful fuzzing
        return 0;
    }
    
    // Initialize variables
    aom_codec_ctx_t decoder;
    aom_codec_stream_info_t stream_info;
    memset(&stream_info, 0, sizeof(stream_info));
    memset(&decoder, 0, sizeof(decoder));
    
    // Create a FILE* for input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Read some data for ULEB decoding
    uint8_t uleb_buffer[16];
    size_t bytes_read = fread(uleb_buffer, 1, sizeof(uleb_buffer), in_file);
    if (bytes_read == 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Use aom_uleb_decode
    uint64_t uleb_value = 0;
    size_t uleb_length = 0;
    int uleb_ret = aom_uleb_decode(uleb_buffer, bytes_read, 
                                   &uleb_value, &uleb_length);
    
    // Rewind to use remaining data for decoder
    rewind(in_file);
    
    // Read remaining data for decoder
    std::vector<uint8_t> decoder_data(size);
    size_t decoder_data_size = fread(decoder_data.data(), 1, size, in_file);
    assert_file_closed(&in_file);;
    
    // Initialize decoder with AV1
    aom_codec_iface_t* decoder_iface = aom_codec_av1_dx();
    aom_codec_dec_cfg_t cfg = {0};
    cfg.threads = 1;
    
    aom_codec_err_t ret = aom_codec_dec_init_ver(&decoder, decoder_iface, &cfg, 
                                                 0, AOM_DECODER_ABI_VERSION);
    if (ret != AOM_CODEC_OK) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Use aom_codec_set_option - set decoder option
    aom_codec_set_option(&decoder, "postproc", "1");
    
    // Use aom_codec_set_frame_buffer_functions
    FrameBufferContext fb_ctx;
    aom_codec_set_frame_buffer_functions(&decoder, 
                                          id_000015_get_frame_buffer, 
                                          id_000015_release_frame_buffer, 
                                         &fb_ctx);
    
    // Decode the data
    aom_codec_decode(&decoder, decoder_data.data(), decoder_data_size, nullptr);
    
    // Use aom_codec_get_stream_info
    aom_codec_get_stream_info(&decoder, &stream_info);
    
    // Use aom_codec_control - try to get decoder control
    int value = 0;
    aom_codec_control(&decoder, AOMD_GET_FRAME_CORRUPTED, &value);
    
    // Use aom_codec_get_preview_frame
    const aom_image_t* preview_frame = aom_codec_get_preview_frame(&decoder);
    
    // Create a test image for other operations
    aom_image_t img;
    memset(&img, 0, sizeof(img));
    
    // Allocate a small image
    if (aom_img_alloc(&img, AOM_IMG_FMT_I420, 64, 64, 1)) {
        // Use aom_img_set_rect - set a rectangle in the image
        unsigned int rect_x = stream_info.w % 32;
        unsigned int rect_y = stream_info.h % 32;
        unsigned int rect_w = 16;
        unsigned int rect_h = 16;
        
        aom_img_set_rect(&img, rect_x, rect_y, rect_w, rect_h, 0);
        
        // Use aom_img_remove_metadata
        aom_img_remove_metadata(&img);
        
        // Free the image
        aom_img_free(&img);
    }
    
    // Clean up decoder
    aom_codec_destroy(&decoder);
    
    assert_file_closed(&in_file);
	return 0;
}