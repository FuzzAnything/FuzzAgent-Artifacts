// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_compile at gencode.c:1106:1 in pcap.h
// pcap_setfilter at pcap.c:3807:1 in pcap.h
// pcap_freecode at gencode.c:1291:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_get_selectable_fd at pcap.c:3530:1 in pcap.h
// pcap_get_required_select_timeout at pcap.c:3536:1 in pcap.h
// pcap_setnonblock at pcap.c:3599:1 in pcap.h
// pcap_setnonblock at pcap.c:3599:1 in pcap.h
// pcap_get_required_select_timeout at pcap.c:3536:1 in pcap.h
// pcap_dispatch at pcap.c:2893:1 in pcap.h
// pcap_get_required_select_timeout at pcap.c:3536:1 in pcap.h
// pcap_dispatch at pcap.c:2893:1 in pcap.h
// pcap_dispatch at pcap.c:2893:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_live at pcap.c:2749:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_compile at gencode.c:1106:1 in pcap.h
// pcap_compile at gencode.c:1106:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_setfilter at pcap.c:3807:1 in pcap.h
// pcap_freecode at gencode.c:1291:1 in pcap.h
// pcap_compile at gencode.c:1106:1 in pcap.h
// pcap_setfilter at pcap.c:3807:1 in pcap.h
// pcap_freecode at gencode.c:1291:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>
#include <time.h>

static void packet_callback(u_char *user, const struct pcap_pkthdr *header, const u_char *pkt_data) {
    (void)user;
    (void)header;
    (void)pkt_data;
}

static pcap_t *create_dummy_pcap(const uint8_t *data, size_t size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle;
    struct bpf_program fp;
    int ret;

    // Attempt to open a loopback device or a dummy file
    handle = pcap_open_live("lo", 65535, 1, 1000, errbuf);
    
    if (handle == NULL) {
        // Try opening a dummy file if live capture fails
        FILE *f = fopen("./dummy_file", "wb");
        if (f) {
            // Write minimal pcap header
            uint32_t magic = 0xa1b2c3d4;
            uint16_t ver_major = 2;
            uint16_t ver_minor = 4;
            int32_t thiszone = 0;
            uint32_t sigfigs = 0;
            uint32_t snaplen = 65535;
            uint32_t network = 1; // Ethernet
            fwrite(&magic, sizeof(magic), 1, f);
            fwrite(&ver_major, sizeof(ver_major), 1, f);
            fwrite(&ver_minor, sizeof(ver_minor), 1, f);
            fwrite(&thiszone, sizeof(thiszone), 1, f);
            fwrite(&sigfigs, sizeof(sigfigs), 1, f);
            fwrite(&snaplen, sizeof(snaplen), 1, f);
            fwrite(&network, sizeof(network), 1, f);
            // Write some dummy packet data if available
            if (size > 0) {
                fwrite(data, size, 1, f);
            }
            fclose(f);
            
            handle = pcap_open_offline("./dummy_file", errbuf);
        }
    }

    if (handle == NULL) {
        return NULL;
    }

    // Compile a simple filter
    const char *filter = "ip";
    int net = 0;
    
    if (pcap_compile(handle, &fp, filter, 0, net) == -1) {
        // Try fallback
        if (pcap_compile(handle, &fp, "host 127.0.0.1", 0, net) == -1) {
            pcap_close(handle);
            return NULL;
        }
    }

    ret = pcap_setfilter(handle, &fp);
    pcap_freecode(&fp);

    if (ret == -1) {
        // Filter setting failed, but we continue to test error handling paths
    }

    return handle;
}

int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle;
    struct bpf_program fp;
    int ret;
    int net = 0;

    // Step 1: Prepare environment
    handle = create_dummy_pcap(Data, Size);
    
    if (handle == NULL) {
        return 0;
    }

    // Re-compile a filter to ensure we call pcap_setfilter as the first step in the sequence
    const char *filter_expr = "tcp";
    if (pcap_compile(handle, &fp, filter_expr, 0, net) != -1) {
        pcap_setfilter(handle, &fp);
        pcap_freecode(&fp);
    } else {
        // Fallback filter
        if (pcap_compile(handle, &fp, "host 0.0.0.0", 0, net) != -1) {
            pcap_setfilter(handle, &fp);
            pcap_freecode(&fp);
        }
    }

    // Sequence: pcap_setfilter -> pcap_geterr -> pcap_get_selectable_fd -> ...
    
    // 2. pcap_geterr
    (void)pcap_geterr(handle);

    // 3. pcap_get_selectable_fd
    (void)pcap_get_selectable_fd(handle);

    // 4. pcap_get_required_select_timeout
    (void)pcap_get_required_select_timeout(handle);

    // 5. pcap_setnonblock
    pcap_setnonblock(handle, 1, errbuf);
    pcap_setnonblock(handle, 0, errbuf);

    // 6. pcap_get_required_select_timeout (again)
    (void)pcap_get_required_select_timeout(handle);

    // 7. pcap_dispatch
    int cnt = (Size > 1000) ? 1000 : (Size > 0 ? Size : 1);
    ret = pcap_dispatch(handle, cnt, packet_callback, NULL);
    (void)ret;

    // 8. pcap_get_required_select_timeout (again)
    (void)pcap_get_required_select_timeout(handle);

    // 9. pcap_dispatch (again)
    ret = pcap_dispatch(handle, cnt, packet_callback, NULL);
    (void)ret;

    // 10. pcap_dispatch (again)
    ret = pcap_dispatch(handle, cnt, packet_callback, NULL);
    (void)ret;

    // 11. pcap_geterr
    (void)pcap_geterr(handle);

    // 12. pcap_close
    pcap_close(handle);

    return 0;
}