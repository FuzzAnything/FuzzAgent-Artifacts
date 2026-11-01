// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_dump_fopen at sf-pcap.c:988:1 in pcap.h
// pcap_dump_ftell at sf-pcap.c:1207:1 in pcap.h
// pcap_dump_file at sf-pcap.c:1201:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_dump_close at sf-pcap.c:1262:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_file at pcap.c:3495:1 in pcap.h
// pcap_dump_open at sf-pcap.c:902:1 in pcap.h
// pcap_dump_ftell at sf-pcap.c:1207:1 in pcap.h
// pcap_dump_file at sf-pcap.c:1201:1 in pcap.h
// pcap_dump_open_append at sf-pcap.c:1005:1 in pcap.h
// pcap_dump_ftell at sf-pcap.c:1207:1 in pcap.h
// pcap_dump_file at sf-pcap.c:1201:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>
#include <time.h>

static void cleanup_pcap(pcap_t *handle) {
    if (handle != NULL) {
        pcap_close(handle);
    }
}

static void cleanup_dumper(pcap_dumper_t *dumper) {
    if (dumper != NULL) {
        pcap_dump_close(dumper);
    }
}

static pcap_t* create_offline_handle(const uint8_t *data, size_t size) {
    const char *tmpfile = "./dummy_file";
    FILE *fp = fopen(tmpfile, "wb");
    if (fp == NULL) {
        return NULL;
    }

    uint32_t magic = 0xa1b2c3d4;
    uint16_t version_major = 2;
    uint16_t version_minor = 4;
    int32_t timezone = 0;
    uint32_t sigfigs = 0;
    uint32_t snaplen = 65535;
    uint32_t network = 1; // Ethernet

    fwrite(&magic, sizeof(magic), 1, fp);
    fwrite(&version_major, sizeof(version_major), 1, fp);
    fwrite(&version_minor, sizeof(version_minor), 1, fp);
    fwrite(&timezone, sizeof(timezone), 1, fp);
    fwrite(&sigfigs, sizeof(sigfigs), 1, fp);
    fwrite(&snaplen, sizeof(snaplen), 1, fp);
    fwrite(&network, sizeof(network), 1, fp);

    if (size > 0) {
        struct pcap_pkthdr hdr;
        struct timeval tv;
        tv.tv_sec = time(NULL);
        tv.tv_usec = 0;
        hdr.ts = tv;
        hdr.caplen = size;
        hdr.len = size;

        fwrite(&hdr, sizeof(hdr), 1, fp);
        fwrite(data, 1, size, fp);
    }

    fclose(fp);

    pcap_t *handle = pcap_open_offline(tmpfile, NULL);
    return handle;
}

int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    pcap_t *handle = create_offline_handle(Data, Size);
    if (handle == NULL) {
        return 0;
    }

    FILE *fp = pcap_file(handle);
    if (fp != NULL) {
        fseek(fp, 0, SEEK_SET);
        char dummy;
        fread(&dummy, 1, 1, fp);
    }

    pcap_dumper_t *dumper = pcap_dump_open(handle, "./dummy_dump_file");
    if (dumper != NULL) {
        long pos = pcap_dump_ftell(dumper);
        (void)pos;

        FILE *dump_fp = pcap_dump_file(dumper);
        if (dump_fp != NULL) {
            char dummy = 0;
            fwrite(&dummy, 1, 1, dump_fp);
        }

        cleanup_dumper(dumper);
    }

    pcap_dumper_t *dumper_append = pcap_dump_open_append(handle, "./dummy_append_file");
    if (dumper_append != NULL) {
        long pos_append = pcap_dump_ftell(dumper_append);
        (void)pos_append;

        FILE *dump_fp_append = pcap_dump_file(dumper_append);
        if (dump_fp_append != NULL) {
            char dummy = 0;
            fwrite(&dummy, 1, 1, dump_fp_append);
        }

        cleanup_dumper(dumper_append);
    }

    FILE *fp_fopen = fopen("./dummy_fopen_file", "wb");
    if (fp_fopen != NULL) {
        pcap_dumper_t *dumper_fopen = pcap_dump_fopen(handle, fp_fopen);
        if (dumper_fopen != NULL) {
            long pos_fopen = pcap_dump_ftell(dumper_fopen);
            (void)pos_fopen;

            FILE *dump_fp_fopen = pcap_dump_file(dumper_fopen);
            if (dump_fp_fopen != NULL) {
                char dummy = 0;
                fwrite(&dummy, 1, 1, dump_fp_fopen);
            }

            // pcap_dump_close closes the underlying FILE* as well,
            // so we must not call fclose(fp_fopen) here.
            cleanup_dumper(dumper_fopen);
        } else {
            // If pcap_dump_fopen failed, we own the FILE* and must close it.
            fclose(fp_fopen);
        }
    }

    cleanup_pcap(handle);

    return 0;
}