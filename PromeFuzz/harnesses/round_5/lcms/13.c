// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsGetHeaderFlags at cmsio0.c:996:27 in lcms2.h
// cmsGetHeaderModel at cmsio0.c:1026:27 in lcms2.h
// cmsGetHeaderRenderingIntent at cmsio0.c:984:27 in lcms2.h
// cmsSetHeaderFlags at cmsio0.c:1002:16 in lcms2.h
// cmsSetHeaderModel at cmsio0.c:1032:16 in lcms2.h
// cmsSetHeaderRenderingIntent at cmsio0.c:990:16 in lcms2.h
// cmsGetHeaderFlags at cmsio0.c:996:27 in lcms2.h
// cmsGetHeaderModel at cmsio0.c:1026:27 in lcms2.h
// cmsGetHeaderRenderingIntent at cmsio0.c:984:27 in lcms2.h
// cmsSetHeaderFlags at cmsio0.c:1002:16 in lcms2.h
// cmsSetHeaderModel at cmsio0.c:1032:16 in lcms2.h
// cmsSetHeaderRenderingIntent at cmsio0.c:990:16 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsGetHeaderFlags at cmsio0.c:996:27 in lcms2.h
// cmsGetHeaderModel at cmsio0.c:1026:27 in lcms2.h
// cmsGetHeaderRenderingIntent at cmsio0.c:984:27 in lcms2.h
// cmsSetHeaderFlags at cmsio0.c:1002:16 in lcms2.h
// cmsSetHeaderModel at cmsio0.c:1032:16 in lcms2.h
// cmsSetHeaderRenderingIntent at cmsio0.c:990:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "lcms2.h"

static cmsUInt32Number readUint32(const uint8_t *data, size_t size, size_t offset, cmsUInt32Number default_value) {
    if (offset + sizeof(cmsUInt32Number) <= size) {
        cmsUInt32Number value;
        memcpy(&value, data + offset, sizeof(cmsUInt32Number));
        return value;
    }
    return default_value;
}

int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    cmsUInt32Number flags = readUint32(Data, Size, 0, 0);
    cmsUInt32Number model = readUint32(Data, Size, 4, 0);
    cmsUInt32Number intent = readUint32(Data, Size, 8, 0);

    cmsHPROFILE hProfile = cmsCreate_sRGBProfile();
    if (!hProfile) {
        return 0;
    }

    cmsUInt32Number originalFlags = cmsGetHeaderFlags(hProfile);
    cmsUInt32Number originalModel = cmsGetHeaderModel(hProfile);
    cmsUInt32Number originalIntent = cmsGetHeaderRenderingIntent(hProfile);

    cmsSetHeaderFlags(hProfile, flags);
    cmsSetHeaderModel(hProfile, model);
    cmsSetHeaderRenderingIntent(hProfile, intent);

    cmsUInt32Number newFlags = cmsGetHeaderFlags(hProfile);
    cmsUInt32Number newModel = cmsGetHeaderModel(hProfile);
    cmsUInt32Number newIntent = cmsGetHeaderRenderingIntent(hProfile);

    cmsSetHeaderFlags(hProfile, originalFlags);
    cmsSetHeaderModel(hProfile, originalModel);
    cmsSetHeaderRenderingIntent(hProfile, originalIntent);

    if (Size > 12) {
        FILE *file = fopen("./dummy_file", "wb");
        if (file) {
            fwrite(Data, 1, Size, file);
            fclose(file);
            
            file = fopen("./dummy_file", "rb");
            if (file) {
                fseek(file, 0, SEEK_END);
                long file_size = ftell(file);
                fseek(file, 0, SEEK_SET);
                
                if (file_size > 0) {
                    uint8_t *file_data = (uint8_t *)malloc(file_size);
                    if (file_data) {
                        fread(file_data, 1, file_size, file);
                        
                        cmsHPROFILE fileProfile = cmsOpenProfileFromMem(file_data, file_size);
                        if (fileProfile) {
                            cmsUInt32Number fileFlags = cmsGetHeaderFlags(fileProfile);
                            cmsUInt32Number fileModel = cmsGetHeaderModel(fileProfile);
                            cmsUInt32Number fileIntent = cmsGetHeaderRenderingIntent(fileProfile);
                            
                            cmsSetHeaderFlags(fileProfile, flags ^ 0xFFFFFFFF);
                            cmsSetHeaderModel(fileProfile, model ^ 0xFFFFFFFF);
                            cmsSetHeaderRenderingIntent(fileProfile, intent ^ 0xFFFFFFFF);
                            
                            cmsCloseProfile(fileProfile);
                        }
                        free(file_data);
                    }
                }
                fclose(file);
                remove("./dummy_file");
            }
        }
    }

    cmsCloseProfile(hProfile);
    return 0;
}