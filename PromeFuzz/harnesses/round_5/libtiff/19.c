// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFFindField at tif_dirinfo.c:795:18 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFWriteScanline at tif_write.c:47:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFCreateGPSDirectory at tif_dir.c:1742:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFWriteCustomDirectory at tif_dirwrite.c:303:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static void cleanup_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
    if (Size < 100) return 0;
    
    write_dummy_file(Data, Size);
    
    TIFF *tif = TIFFOpen("./dummy_file", "w");
    if (!tif) {
        cleanup_file();
        return 0;
    }
    
    uint32_t tag;
    uint16_t sample;
    uint32_t row;
    void *buf;
    uint64_t offset;
    int ret;
    const TIFFField *field;
    
    buf = malloc(1024);
    if (!buf) {
        TIFFClose(tif);
        cleanup_file();
        return 0;
    }
    memcpy(buf, Data, Size > 1024 ? 1024 : Size);
    
    offset = 0;
    
    tag = *(uint32_t*)Data;
    TIFFSetField(tif, tag, *(uint32_t*)(Data+4));
    
    tag = *(uint32_t*)(Data+8);
    TIFFSetField(tif, tag, *(uint32_t*)(Data+12));
    
    tag = *(uint32_t*)(Data+16);
    TIFFSetField(tif, tag, *(uint32_t*)(Data+20));
    
    tag = *(uint32_t*)(Data+24);
    TIFFSetField(tif, tag, *(uint32_t*)(Data+28));
    
    tag = *(uint32_t*)(Data+32);
    TIFFSetField(tif, tag, *(uint32_t*)(Data+36));
    
    tag = *(uint32_t*)(Data+40);
    TIFFSetField(tif, tag, *(uint32_t*)(Data+44));
    
    tag = *(uint32_t*)(Data+48);
    TIFFSetField(tif, tag, *(uint32_t*)(Data+52));
    
    tag = *(uint32_t*)(Data+56);
    TIFFSetField(tif, tag, *(uint32_t*)(Data+60));
    
    tag = *(uint32_t*)(Data+64);
    TIFFSetField(tif, tag, *(uint32_t*)(Data+68));
    
    tag = *(uint32_t*)(Data+72);
    TIFFSetField(tif, tag, *(uint32_t*)(Data+76));
    
    tag = *(uint32_t*)(Data+80);
    TIFFSetField(tif, tag, *(uint32_t*)(Data+84));
    
    tag = *(uint32_t*)(Data+88);
    TIFFSetField(tif, tag, *(uint32_t*)(Data+92));
    
    tag = *(uint32_t*)(Data+96);
    TIFFSetField(tif, tag, *(uint32_t*)(Data+100));
    
    tag = *(uint32_t*)(Data+104);
    TIFFSetField(tif, tag, *(uint32_t*)(Data+108));
    
    tag = *(uint32_t*)(Data+112);
    TIFFSetField(tif, tag, *(uint32_t*)(Data+116));
    
    tag = *(uint32_t*)(Data+120);
    TIFFSetField(tif, tag, *(uint32_t*)(Data+124));
    
    tag = *(uint32_t*)(Data+128);
    TIFFSetField(tif, tag, *(uint32_t*)(Data+132));
    
    tag = *(uint32_t*)(Data+136);
    TIFFSetField(tif, tag, *(uint32_t*)(Data+140));
    
    tag = *(uint32_t*)(Data+144);
    TIFFSetField(tif, tag, *(uint32_t*)(Data+148));
    
    tag = *(uint32_t*)(Data+152);
    TIFFSetField(tif, tag, *(uint32_t*)(Data+156));
    
    field = TIFFFindField(tif, *(uint32_t*)(Data+160), *(TIFFDataType*)(Data+164));
    
    tag = *(uint32_t*)(Data+168);
    TIFFSetField(tif, tag, *(uint32_t*)(Data+172));
    
    tag = *(uint32_t*)(Data+176);
    TIFFSetField(tif, tag, *(uint32_t*)(Data+180));
    
    row = *(uint32_t*)(Data+184);
    sample = *(uint16_t*)(Data+188);
    ret = TIFFWriteScanline(tif, buf, row, sample);
    
    ret = TIFFWriteDirectory(tif);
    
    ret = TIFFCreateGPSDirectory(tif);
    
    tag = *(uint32_t*)(Data+190);
    TIFFSetField(tif, tag, *(uint32_t*)(Data+194));
    
    tag = *(uint32_t*)(Data+198);
    TIFFSetField(tif, tag, *(uint32_t*)(Data+202));
    
    tag = *(uint32_t*)(Data+206);
    TIFFSetField(tif, tag, *(uint32_t*)(Data+210));
    
    tag = *(uint32_t*)(Data+214);
    TIFFSetField(tif, tag, *(uint32_t*)(Data+218));
    
    tag = *(uint32_t*)(Data+222);
    TIFFSetField(tif, tag, *(uint32_t*)(Data+226));
    
    tag = *(uint32_t*)(Data+230);
    TIFFSetField(tif, tag, *(uint32_t*)(Data+234));
    
    tag = *(uint32_t*)(Data+238);
    TIFFSetField(tif, tag, *(uint32_t*)(Data+242));
    
    tag = *(uint32_t*)(Data+246);
    TIFFSetField(tif, tag, *(uint32_t*)(Data+250));
    
    tag = *(uint32_t*)(Data+254);
    TIFFSetField(tif, tag, *(uint32_t*)(Data+258));
    
    tag = *(uint32_t*)(Data+262);
    TIFFSetField(tif, tag, *(uint32_t*)(Data+266));
    
    tag = *(uint32_t*)(Data+270);
    TIFFSetField(tif, tag, *(uint32_t*)(Data+274));
    
    tag = *(uint32_t*)(Data+278);
    TIFFSetField(tif, tag, *(uint32_t*)(Data+282));
    
    tag = *(uint32_t*)(Data+286);
    TIFFSetField(tif, tag, *(uint32_t*)(Data+290));
    
    tag = *(uint32_t*)(Data+294);
    TIFFSetField(tif, tag, *(uint32_t*)(Data+298));
    
    tag = *(uint32_t*)(Data+302);
    TIFFSetField(tif, tag, *(uint32_t*)(Data+306));
    
    tag = *(uint32_t*)(Data+310);
    TIFFSetField(tif, tag, *(uint32_t*)(Data+314));
    
    tag = *(uint32_t*)(Data+318);
    TIFFSetField(tif, tag, *(uint32_t*)(Data+322));
    
    tag = *(uint32_t*)(Data+326);
    TIFFSetField(tif, tag, *(uint32_t*)(Data+330));
    
    tag = *(uint32_t*)(Data+334);
    TIFFSetField(tif, tag, *(uint32_t*)(Data+338));
    
    tag = *(uint32_t*)(Data+342);
    TIFFSetField(tif, tag, *(uint32_t*)(Data+346));
    
    tag = *(uint32_t*)(Data+350);
    TIFFSetField(tif, tag, *(uint32_t*)(Data+354));
    
    ret = TIFFWriteCustomDirectory(tif, &offset);
    
    free(buf);
    TIFFClose(tif);
    cleanup_file();
    
    return 0;
}