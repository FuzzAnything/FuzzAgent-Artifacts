// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateMultiprofileTransform at cmsxform.c:1311:25 in lcms2.h
// cmsCreateExtendedTransform at cmsxform.c:1123:25 in lcms2.h
// cmsGetTransformInputFormat at cmsxform.c:1424:27 in lcms2.h
// cmsGetTransformInputFormat at cmsxform.c:1424:27 in lcms2.h
// cmsTransform2DeviceLink at cmsvirt.c:1194:23 in lcms2.h
// cmsTransform2DeviceLink at cmsvirt.c:1194:23 in lcms2.h
// cmsGetTransformInputFormat at cmsxform.c:1424:27 in lcms2.h
// cmsGetTransformOutputFormat at cmsxform.c:1432:27 in lcms2.h
// cmsDoTransform at cmsxform.c:192:16 in lcms2.h
// cmsDoTransformStride at cmsxform.c:211:16 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static cmsHPROFILE load_dummy_profile(const uint8_t *data, size_t size) {
    if (size < 128) return NULL;
    uint8_t *buf = malloc(size);
    if (!buf) return NULL;
    memcpy(buf, data, size);
    if (size >= 128) {
        uint32_t *magic = (uint32_t *)(buf + 36);
        if (*magic != 0x61637370) {
            *magic = 0x61637370;
        }
    }
    cmsHPROFILE h = cmsOpenProfileFromMem(buf, size);
    free(buf);
    return h;
}

static void cleanup_profiles(cmsHPROFILE *profiles, int count) {
    for (int i = 0; i < count; i++) {
        if (profiles[i]) {
            cmsCloseProfile(profiles[i]);
            profiles[i] = NULL;
        }
    }
}

static cmsUInt32Number get_pixel_size(cmsUInt32Number format) {
    cmsUInt32Number channels = 0;
    cmsUInt32Number depth = 0;

    if (format & 0xFF000000) {
        switch (format & 0xFF000000) {
            case 0x01000000: depth = 1; break;
            case 0x02000000: depth = 2; break;
            case 0x04000000: depth = 4; break;
            default: return 0;
        }
    } else {
        depth = 1;
    }

    switch (format & 0x00FFFFFF) {
        case 0x00000001: channels = 1; break;
        case 0x00000002: channels = 2; break;
        case 0x00000003: channels = 3; break;
        case 0x00000004: channels = 4; break;
        case 0x00000005: channels = 5; break;
        case 0x00000006: channels = 6; break;
        case 0x00000007: channels = 7; break;
        case 0x00000008: channels = 8; break;
        default:
            return 0;
    }
    return channels * depth;
}

int LLVMFuzzerTestOneInput_125(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) ctx = cmsCreateContext(NULL, NULL);

    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }

    int num_profiles = 1;
    if (Size > 0) num_profiles = Data[0] % 4 + 1;
    if (num_profiles > 4) num_profiles = 4;

    cmsHPROFILE profiles[4] = {0};
    cmsHPROFILE valid_profiles[4] = {0};
    int valid_count = 0;

    size_t chunk_size = Size / num_profiles;
    if (chunk_size < 128) chunk_size = 128;

    for (int i = 0; i < num_profiles; i++) {
        size_t offset = i * chunk_size;
        if (offset + chunk_size > Size) chunk_size = Size - offset;
        if (chunk_size < 128) continue;

        profiles[i] = load_dummy_profile(Data + offset, chunk_size);
        if (profiles[i]) {
            valid_profiles[valid_count++] = profiles[i];
        }
    }

    if (valid_count == 0) {
        profiles[0] = load_dummy_profile(Data, Size);
        if (profiles[0]) valid_profiles[0] = profiles[0];
        valid_count = (profiles[0] ? 1 : 0);
    }

    cmsHTRANSFORM hTransform = NULL;
    cmsHTRANSFORM hTransformExt = NULL;
    cmsHPROFILE hDeviceLink = NULL;

    if (valid_count > 0) {
        cmsUInt32Number in_fmt = (Data[1] % 10);
        cmsUInt32Number out_fmt = (Data[2] % 10);
        cmsUInt32Number intent = Data[3] % 4;
        cmsUInt32Number flags = Data[4] % 8;

        hTransform = cmsCreateMultiprofileTransform(
            valid_profiles,
            valid_count,
            in_fmt,
            out_fmt,
            intent,
            flags
        );
    }

    if (valid_count > 0) {
        cmsBool bpc[4] = {0};
        cmsUInt32Number intents[4] = {0};
        cmsFloat64Number adapt[4] = {0};
        for (int i = 0; i < valid_count; i++) {
            bpc[i] = (Data[5 + i] % 2);
            intents[i] = Data[6 + i] % 4;
            adapt[i] = (Data[7 + i] / 256.0);
        }

        cmsUInt32Number in_fmt_ext = (Data[8] % 10);
        cmsUInt32Number out_fmt_ext = (Data[9] % 10);
        cmsUInt32Number flags_ext = Data[10] % 16;

        hTransformExt = cmsCreateExtendedTransform(
            ctx,
            valid_count,
            valid_profiles,
            bpc,
            intents,
            adapt,
            NULL,
            0,
            in_fmt_ext,
            out_fmt_ext,
            flags_ext
        );
    }

    cmsUInt32Number fmt = 0;
    if (hTransform) {
        fmt = cmsGetTransformInputFormat(hTransform);
    }
    fmt = cmsGetTransformInputFormat(NULL);

    if (hTransform) {
        cmsFloat64Number version = 2.0 + (Data[11] / 100.0);
        hDeviceLink = cmsTransform2DeviceLink(hTransform, version, Data[12] % 4);
    }
    hDeviceLink = cmsTransform2DeviceLink(NULL, 2.0, 0);

    if (hTransform) {
        cmsUInt32Number input_fmt = cmsGetTransformInputFormat(hTransform);
        cmsUInt32Number output_fmt = cmsGetTransformOutputFormat(hTransform);
        
        cmsUInt32Number pixelSize = get_pixel_size(input_fmt);
        if (pixelSize == 0) pixelSize = 4;

        if (pixelSize > 0 && pixelSize < 1024) {
            size_t buffer_size = 100 * pixelSize;
            uint8_t *input_buf = malloc(buffer_size);
            uint8_t *output_buf = malloc(buffer_size);
            if (input_buf && output_buf) {
                memset(input_buf, 0, buffer_size);
                memset(output_buf, 0, buffer_size);
                if (Size > 0) {
                    size_t copy_len = (Size < buffer_size) ? Size : buffer_size;
                    memcpy(input_buf, Data, copy_len);
                }
                
                cmsDoTransform(hTransform, input_buf, output_buf, 100);
                
                cmsDoTransformStride(hTransform, input_buf, output_buf, 100, pixelSize * 2);
                
                free(input_buf);
                free(output_buf);
            }
        }
    }

    if (hTransform) cmsDeleteTransform(hTransform);
    if (hTransformExt) cmsDeleteTransform(hTransformExt);
    if (hDeviceLink) cmsCloseProfile(hDeviceLink);
    
    cleanup_profiles(profiles, num_profiles);
    if (ctx) cmsDeleteContext(ctx);

    return 0;
}