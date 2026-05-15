// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_add_metadata at aom_image.c:380:5 in aom_image.h
// aom_img_num_metadata at aom_image.c:429:8 in aom_image.h
// aom_img_get_metadata at aom_image.c:419:23 in aom_image.h
// aom_img_remove_metadata at aom_image.c:412:6 in aom_image.h
// aom_img_remove_metadata at aom_image.c:412:6 in aom_image.h
// aom_img_remove_metadata at aom_image.c:412:6 in aom_image.h
// aom_img_get_metadata at aom_image.c:419:23 in aom_image.h
// aom_img_metadata_alloc at aom_image.c:327:17 in aom_image.h
// aom_img_metadata_free at aom_image.c:345:6 in aom_image.h
// aom_img_num_metadata at aom_image.c:429:8 in aom_image.h
// aom_img_metadata_free at aom_image.c:345:6 in aom_image.h
// aom_img_metadata_alloc at aom_image.c:327:17 in aom_image.h
// aom_img_metadata_free at aom_image.c:345:6 in aom_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "aom_image.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void initialize_image(aom_image_t *img) {
    memset(img, 0, sizeof(aom_image_t));
    img->fmt = AOM_IMG_FMT_I420;
    img->cp = AOM_CICP_CP_BT_709;
    img->tc = AOM_CICP_TC_BT_709;
    img->mc = AOM_CICP_MC_BT_709;
    img->range = AOM_CR_STUDIO_RANGE;
    img->csp = AOM_CSP_VERTICAL;
    img->w = 64;
    img->h = 64;
    img->d_w = 64;
    img->d_h = 64;
    img->r_w = 64;
    img->r_h = 64;
    img->bit_depth = 8;
    img->x_chroma_shift = 1;
    img->y_chroma_shift = 1;
    img->bps = 12;
    img->sz = 64 * 64 * 3 / 2;
    img->metadata = NULL;
}

int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    aom_image_t img;
    initialize_image(&img);

    size_t num_ops = Data[0] % 8;
    size_t data_offset = 1;

    for (size_t op = 0; op < num_ops && data_offset < Size; op++) {
        uint8_t op_type = Data[data_offset++] % 6;

        switch (op_type) {
            case 0: {
                aom_img_remove_metadata(&img);
                break;
            }
            case 1: {
                if (data_offset >= Size) break;
                size_t index = Data[data_offset++] % 16;
                const aom_metadata_t *md = aom_img_get_metadata(&img, index);
                (void)md;
                break;
            }
            case 2: {
                if (data_offset + 4 >= Size) break;
                uint32_t type = *(const uint32_t*)(Data + data_offset);
                data_offset += 4;
                if (data_offset >= Size) break;
                size_t sz = Data[data_offset++] % 256;
                if (data_offset >= Size) break;
                aom_metadata_insert_flags_t flag = (aom_metadata_insert_flags_t)(Data[data_offset] % 19);
                if (data_offset + sz > Size) sz = Size - data_offset;
                aom_metadata_t *md = aom_img_metadata_alloc(type, Data + data_offset, sz, flag);
                if (md) {
                    aom_img_metadata_free(md);
                }
                if (sz > 0) data_offset += sz;
                break;
            }
            case 3: {
                size_t num = aom_img_num_metadata(&img);
                (void)num;
                break;
            }
            case 4: {
                aom_img_metadata_free(NULL);
                if (data_offset >= Size) break;
                size_t sz = Data[data_offset++] % 256;
                if (data_offset >= Size) break;
                uint32_t type = 0;
                if (data_offset + 4 <= Size) {
                    type = *(const uint32_t*)(Data + data_offset);
                    data_offset += 4;
                }
                if (data_offset >= Size) break;
                aom_metadata_insert_flags_t flag = (aom_metadata_insert_flags_t)(Data[data_offset] % 19);
                if (data_offset + sz > Size) sz = Size - data_offset;
                aom_metadata_t *md = aom_img_metadata_alloc(type, Data + data_offset, sz, flag);
                aom_img_metadata_free(md);
                if (sz > 0) data_offset += sz;
                break;
            }
            case 5: {
                if (data_offset + 4 >= Size) break;
                uint32_t type = *(const uint32_t*)(Data + data_offset);
                data_offset += 4;
                if (data_offset >= Size) break;
                size_t sz = Data[data_offset++] % 256;
                if (data_offset >= Size) break;
                aom_metadata_insert_flags_t flag = (aom_metadata_insert_flags_t)(Data[data_offset] % 19);
                if (data_offset + sz > Size) sz = Size - data_offset;
                int ret = aom_img_add_metadata(&img, type, Data + data_offset, sz, flag);
                (void)ret;
                if (sz > 0) data_offset += sz;
                break;
            }
        }
    }

    size_t final_num = aom_img_num_metadata(&img);
    for (size_t i = 0; i < final_num; i++) {
        const aom_metadata_t *md = aom_img_get_metadata(&img, i);
        (void)md;
    }

    aom_img_remove_metadata(&img);
    aom_img_remove_metadata(&img);

    return 0;
}