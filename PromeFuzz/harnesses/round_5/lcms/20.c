// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsGetProfileIOhandler at cmsio0.c:517:25 in lcms2.h
// _cmsReadTypeBase at cmsplugin.c:421:31 in lcms2_plugin.h
// _cmsWriteTypeBase at cmsplugin.c:434:20 in lcms2_plugin.h
// _cmsWriteAlignment at cmsplugin.c:462:19 in lcms2_plugin.h
// _cmsWriteUInt64Number at cmsplugin.c:324:20 in lcms2_plugin.h
// _cmsReadAlignment at cmsplugin.c:445:19 in lcms2_plugin.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
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

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void cleanup_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    write_dummy_file(Data, Size);
    
    cmsHPROFILE hProfile = cmsOpenProfileFromFile("./dummy_file", "r");
    if (hProfile) {
        cmsIOHANDLER* io = cmsGetProfileIOhandler(hProfile);
        if (io) {
            cmsTagTypeSignature sig_read = _cmsReadTypeBase(io);
            
            cmsTagTypeSignature sig_write = 0x6D6D6F74;
            _cmsWriteTypeBase(io, sig_write);
            
            _cmsWriteAlignment(io);
            
            cmsUInt64Number num = 0x1234567890ABCDEF;
            _cmsWriteUInt64Number(io, &num);
            
            _cmsReadAlignment(io);
        }
        cmsCloseProfile(hProfile);
    }
    
    cleanup_dummy_file();
    
    return 0;
}