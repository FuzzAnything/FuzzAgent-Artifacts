// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cms8Fixed8toDouble at cmsplugin.c:365:28 in lcms2_plugin.h
// _cmsDoubleTo8Fixed8 at cmsplugin.c:370:27 in lcms2_plugin.h
// _cmsWrite15Fixed16Number at cmsplugin.c:337:20 in lcms2_plugin.h
// _cmsRead15Fixed16Number at cmsplugin.c:224:20 in lcms2_plugin.h
// _cmsRead15Fixed16Number at cmsplugin.c:224:20 in lcms2_plugin.h
// _cmsDoubleTo15Fixed16 at cmsplugin.c:383:31 in lcms2_plugin.h
// _cmsDoubleTo8Fixed8 at cmsplugin.c:370:27 in lcms2_plugin.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// _cms15Fixed16toDouble at cmsplugin.c:377:28 in lcms2_plugin.h
// _cmsDoubleTo15Fixed16 at cmsplugin.c:383:31 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "lcms2_plugin.h"

static cmsContext g_ctx = NULL;

static cmsUInt32Number dummy_read(struct _cms_io_handler* iohandler, void* Buffer,
                                  cmsUInt32Number size, cmsUInt32Number count) {
    (void)iohandler;
    (void)size;
    (void)count;
    (void)Buffer;
    return 0;
}

static cmsBool dummy_seek(struct _cms_io_handler* iohandler, cmsUInt32Number offset) {
    (void)iohandler;
    (void)offset;
    return TRUE;
}

static cmsBool dummy_close(struct _cms_io_handler* iohandler) {
    (void)iohandler;
    return TRUE;
}

static cmsUInt32Number dummy_tell(struct _cms_io_handler* iohandler) {
    (void)iohandler;
    return 0;
}

static cmsBool dummy_write(struct _cms_io_handler* iohandler, cmsUInt32Number size,
                           const void* Buffer) {
    (void)iohandler;
    (void)size;
    (void)Buffer;
    return TRUE;
}

static void init_io_handler(cmsIOHANDLER* io, cmsContext ctx) {
    memset(io, 0, sizeof(cmsIOHANDLER));
    io->ContextID = ctx;
    io->Read = dummy_read;
    io->Seek = dummy_seek;
    io->Close = dummy_close;
    io->Tell = dummy_tell;
    io->Write = dummy_write;
    io->UsedSpace = 0;
    io->ReportedSize = 0;
    io->stream = NULL;
    strncpy(io->PhysicalFile, "./dummy_file", cmsMAX_PATH - 1);
}

static cmsContext create_context(void) {
    if (g_ctx == NULL) {
        g_ctx = cmsCreateContext(NULL, NULL);
    }
    return g_ctx;
}

int LLVMFuzzerTestOneInput_44(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    cmsContext ctx = create_context();
    if (ctx == NULL) {
        return 0;
    }

    cmsIOHANDLER io;
    init_io_handler(&io, ctx);

    // Extract inputs from Data
    cmsS15Fixed16Number fix32_val = *(const cmsS15Fixed16Number*)(Data);
    cmsUInt16Number fixed8_val = *(const cmsUInt16Number*)(Data + 2);
    double double_val = 0.0;
    if (Size >= 12) {
        memcpy(&double_val, Data + 4, sizeof(double));
    } else {
        double_val = (double)fix32_val / 65536.0;
    }

    // Fuzz _cms15Fixed16toDouble
    cmsFloat64Number res1 = _cms15Fixed16toDouble(fix32_val);
    (void)res1;

    // Fuzz _cmsDoubleTo15Fixed16
    cmsS15Fixed16Number res2 = _cmsDoubleTo15Fixed16(double_val);
    (void)res2;

    // Fuzz _cms8Fixed8toDouble
    cmsFloat64Number res3 = _cms8Fixed8toDouble(fixed8_val);
    (void)res3;

    // Fuzz _cmsDoubleTo8Fixed8
    cmsUInt16Number res4 = _cmsDoubleTo8Fixed8(double_val);
    (void)res4;

    // Fuzz _cmsWrite15Fixed16Number
    cmsBool res5 = _cmsWrite15Fixed16Number(&io, double_val);
    (void)res5;

    // Fuzz _cmsRead15Fixed16Number with valid pointer
    cmsFloat64Number read_val = 0.0;
    cmsBool res6 = _cmsRead15Fixed16Number(&io, &read_val);
    (void)res6;

    // Fuzz _cmsRead15Fixed16Number with NULL pointer
    cmsBool res7 = _cmsRead15Fixed16Number(&io, NULL);
    (void)res7;

    // Additional state exploration: randomize double_val slightly for overflow checks
    double_val = (double_val > 32767.5) ? 32767.6 : -32768.0;
    cmsS15Fixed16Number res8 = _cmsDoubleTo15Fixed16(double_val);
    (void)res8;

    double_val = -100.0;
    cmsUInt16Number res9 = _cmsDoubleTo8Fixed8(double_val);
    (void)res9;

    return 0;
}