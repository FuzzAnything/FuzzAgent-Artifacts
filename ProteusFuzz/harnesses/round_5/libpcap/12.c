// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_freecode at gencode.c:1291:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_open_live at pcap.c:2749:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_setnonblock at pcap.c:3599:1 in pcap.h
// pcap_dispatch at pcap.c:2893:1 in pcap.h
// pcap_stats at pcap.c:3848:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>

static void dummy_callback(u_char *user, const struct pcap_pkthdr *header, const u_char *packet) {
    (void)user;
    (void)header;
    (void)packet;
}

int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = NULL;
    struct bpf_program fp;
    struct pcap_stat stats;
    int nonblock_val;
    int ret;

    if (Size < 1) {
        return 0;
    }

    // Initialize error buffer
    memset(errbuf, 0, sizeof(errbuf));

    // Step 1: Invoke pcap_geterr on NULL
    (void)pcap_geterr(NULL);

    // Attempt to open a pcap handle.
    handle = pcap_open_live("lo", 65535, 1, 1000, errbuf);
    
    if (handle == NULL) {
        // If live open fails, try opening a dummy file
        FILE *f = fopen("./dummy_file", "wb");
        if (f) {
            uint32_t magic = 0xa1b2c3d4;
            fwrite(&magic, sizeof(magic), 1, f);
            uint16_t ver_major = 2;
            uint16_t ver_minor = 4;
            fwrite(&ver_major, sizeof(ver_major), 1, f);
            fwrite(&ver_minor, sizeof(ver_minor), 1, f);
            int32_t zone = 0;
            int32_t sigfigs = 0;
            int32_t snaplen = 65535;
            int32_t network = 1; 
            fwrite(&zone, sizeof(zone), 1, f);
            fwrite(&sigfigs, sizeof(sigfigs), 1, f);
            fwrite(&snaplen, sizeof(snaplen), 1, f);
            fwrite(&network, sizeof(network), 1, f);
            fclose(f);
            
            handle = pcap_open_offline("./dummy_file", errbuf);
        }
    }

    if (handle == NULL) {
        return 0;
    }

    // Sequence: pcap_geterr -> pcap_setnonblock -> pcap_dispatch -> pcap_stats -> pcap_geterr -> pcap_geterr -> pcap_close -> pcap_freecode

    // 1. pcap_geterr (on handle)
    (void)pcap_geterr(handle);

    // 2. pcap_setnonblock
    nonblock_val = (Data[0] % 2) == 0 ? 0 : 1;
    ret = pcap_setnonblock(handle, nonblock_val, errbuf);
    (void)ret;

    // 3. pcap_dispatch
    int cnt = 1;
    if (Size > 1) {
        cnt = Data[1] % 100;
        if (cnt == 0) cnt = 1;
    }
    
    ret = pcap_dispatch(handle, cnt, dummy_callback, NULL);
    (void)ret;

    // 4. pcap_stats
    memset(&stats, 0, sizeof(stats));
    ret = pcap_stats(handle, &stats);
    (void)ret;

    // 5. pcap_geterr
    (void)pcap_geterr(handle);

    // 6. pcap_geterr (again)
    (void)pcap_geterr(handle);

    // 7. pcap_close
    pcap_close(handle);
    handle = NULL;

    // 8. pcap_freecode
    fp.bf_len = 0;
    fp.bf_insns = NULL;
    
    // Check bounds before accessing Data[2] and Data[3]
    if (Size > 3) {
        if (Data[2] % 2 == 1) {
            size_t num_insns = (Data[3] % 10) + 1;
            fp.bf_insns = malloc(num_insns * sizeof(struct bpf_insn));
            if (fp.bf_insns) {
                fp.bf_len = num_insns;
                memset(fp.bf_insns, 0, fp.bf_len * sizeof(struct bpf_insn));
            }
        }
    }

    pcap_freecode(&fp);

    // Cleanup dummy file
    remove("./dummy_file");

    return 0;
}