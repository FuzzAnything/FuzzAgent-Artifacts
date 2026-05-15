// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsMD5alloc at cmsmd5.c:154:21 in lcms2_plugin.h
// cmsMD5add at cmsmd5.c:172:16 in lcms2_plugin.h
// cmsMD5finish at cmsmd5.c:215:16 in lcms2_plugin.h
// cmsSetHeaderProfileID at cmsio0.c:1056:16 in lcms2.h
// cmsGetHeaderProfileID at cmsio0.c:1050:16 in lcms2.h
// cmsMD5computeID at cmsmd5.c:257:19 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include "lcms2_plugin.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cmsHPROFILE create_test_profile_from_data(const uint8_t *data, size_t size) {
    if (size < 128) {
        return cmsCreate_sRGBProfile();
    }
    
    FILE *file = fopen("./dummy_file", "wb");
    if (!file) {
        return cmsCreate_sRGBProfile();
    }
    
    fwrite(data, 1, size, file);
    fclose(file);
    
    cmsHPROFILE profile = cmsOpenProfileFromFile("./dummy_file", "r");
    if (!profile) {
        profile = cmsCreate_sRGBProfile();
    }
    
    remove("./dummy_file");
    return profile;
}

static void test_md5_operations(cmsContext context, const uint8_t *data, size_t size, cmsHPROFILE profile) {
    cmsHANDLE md5_handle = cmsMD5alloc(context);
    if (!md5_handle) {
        return;
    }
    
    if (size > 0) {
        size_t chunk_size = size > 1024 ? 1024 : size;
        cmsMD5add(md5_handle, data, (cmsUInt32Number)chunk_size);
    }
    
    cmsProfileID profile_id;
    cmsMD5finish(&profile_id, md5_handle);
    
    cmsSetHeaderProfileID(profile, (cmsUInt8Number*)&profile_id);
    
    cmsUInt8Number retrieved_id[16];
    cmsGetHeaderProfileID(profile, retrieved_id);
    
    cmsMD5computeID(profile);
}

int LLVMFuzzerTestOneInput_68(const uint8_t *data, size_t size) {
    if (size == 0) {
        return 0;
    }
    
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) {
        return 0;
    }
    
    cmsHPROFILE profile = create_test_profile_from_data(data, size);
    if (!profile) {
        cmsDeleteContext(context);
        return 0;
    }
    
    test_md5_operations(context, data, size, profile);
    
    cmsCloseProfile(profile);
    cmsDeleteContext(context);
    
    return 0;
}