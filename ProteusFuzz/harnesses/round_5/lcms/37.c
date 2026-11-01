// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDoTransform at cmsxform.c:192:16 in lcms2.h
// cmsDoTransformLineStride at cmsxform.c:229:16 in lcms2.h
// cmsCreateRGBProfile at cmsvirt.c:217:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsGetTransformInputFormat at cmsxform.c:1424:27 in lcms2.h
// cmsGetTransformOutputFormat at cmsxform.c:1432:27 in lcms2.h
// _cmsGetTransformWorkerFlags at cmsxform.c:801:27 in lcms2_plugin.h
// _cmsGetTransformFlags at cmsxform.c:780:27 in lcms2_plugin.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

static cmsContext create_dummy_context(void) {
    return cmsCreateContext(NULL, NULL);
}

static cmsHPROFILE create_dummy_profile(cmsContext ctx) {
    // Attempt to create a basic RGB profile. 
    // Passing NULLs may result in NULL return or default profile depending on lcms2 version.
    cmsHPROFILE h = cmsCreateRGBProfile(NULL, NULL, NULL);
    return h;
}

static cmsHTRANSFORM create_dummy_transform(cmsContext ctx) {
    cmsHPROFILE hIn = create_dummy_profile(ctx);
    cmsHPROFILE hOut = create_dummy_profile(ctx);
    
    if (!hIn || !hOut) {
        if (hIn) cmsCloseProfile(hIn);
        if (hOut) cmsCloseProfile(hOut);
        return NULL;
    }

    // Create transform with RGB input and output
    cmsHTRANSFORM trans = cmsCreateTransform(hIn, cmsSigRgbData, hOut, cmsSigRgbData, 0, 0);
    
    cmsCloseProfile(hIn);
    cmsCloseProfile(hOut);
    
    return trans;
}

static void cleanup_context(cmsContext ctx) {
    if (ctx) {
        cmsDeleteContext(ctx);
    }
}

static void cleanup_transform(cmsHTRANSFORM trans) {
    if (trans) {
        // cmsDeleteTransform takes only the transform handle
        cmsDeleteTransform(trans);
    }
}

int LLVMFuzzerTestOneInput_37(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    cmsContext ctx = create_dummy_context();
    if (!ctx) return 0;

    cmsHTRANSFORM trans = create_dummy_transform(ctx);
    if (!trans) {
        cleanup_context(ctx);
        return 0;
    }

    // Allocate buffers for transformation
    size_t buf_size = (Size > 1024) ? 1024 : Size;
    uint8_t *input_buf = (uint8_t *)malloc(buf_size);
    uint8_t *output_buf = (uint8_t *)malloc(buf_size);
    if (!input_buf || !output_buf) {
        if (input_buf) free(input_buf);
        if (output_buf) free(output_buf);
        cleanup_transform(trans);
        cleanup_context(ctx);
        return 0;
    }

    // Initialize buffers with fuzz data
    memcpy(input_buf, Data, buf_size);
    memset(output_buf, 0, buf_size);

    // Test 1: cmsGetTransformInputFormat
    cmsUInt32Number in_fmt = cmsGetTransformInputFormat(trans);
    (void)in_fmt;

    // Test 2: cmsGetTransformOutputFormat
    cmsUInt32Number out_fmt = cmsGetTransformOutputFormat(trans);
    (void)out_fmt;

    // Test 3: _cmsGetTransformWorkerFlags (internal, requires struct pointer)
    struct _cmstransform_struct *internal_trans = (struct _cmstransform_struct *)trans;
    if (internal_trans) {
        cmsUInt32Number worker_flags = _cmsGetTransformWorkerFlags(internal_trans);
        (void)worker_flags;

        // Test 4: _cmsGetTransformFlags
        cmsUInt32Number flags = _cmsGetTransformFlags(internal_trans);
        (void)flags;
    }

    // Test 5: cmsDoTransform
    cmsUInt32Number pixel_count = (Size > 0) ? (Size / 4) : 1;
    if (pixel_count > 0) {
        cmsDoTransform(trans, input_buf, output_buf, pixel_count);
    }

    // Test 6: cmsDoTransformLineStride
    cmsUInt32Number pixels_per_line = (Size > 0) ? (Size / 8) : 1;
    cmsUInt32Number line_count = 1;
    cmsUInt32Number bytes_per_line_in = pixels_per_line * 3; // Assuming RGB
    cmsUInt32Number bytes_per_line_out = pixels_per_line * 3;
    cmsUInt32Number bytes_per_plane_in = 0;
    cmsUInt32Number bytes_per_plane_out = 0;

    if (pixels_per_line > 0 && line_count > 0) {
        cmsDoTransformLineStride(trans, input_buf, output_buf, pixels_per_line, line_count,
                                 bytes_per_line_in, bytes_per_line_out,
                                 bytes_per_plane_in, bytes_per_plane_out);
    }

    // Cleanup
    free(input_buf);
    free(output_buf);
    cleanup_transform(trans);
    cleanup_context(ctx);

    return 0;
}