// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_set_buffer_size at pcap.c:2625:1 in pcap.h
// pcap_set_promisc at pcap.c:2544:1 in pcap.h
// pcap_setsampling at pcap.c:5067:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_set_buffer_size at pcap.c:2625:1 in pcap.h
// pcap_set_promisc at pcap.c:2544:1 in pcap.h
// pcap_setsampling at pcap.c:5067:1 in pcap.h
// pcap_open at pcap.c:4949:1 in pcap.h
// pcap_breakloop at pcap.c:2932:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <pcap.h>

static void cleanup_pcap(pcap_t *p) {
    if (p != NULL) {
        pcap_close(p);
    }
}

int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *p = NULL;
    struct pcap_rmtauth auth;
    memset(&auth, 0, sizeof(auth));

    // Use input data to construct a source string
    char source[256];
    size_t src_len = (Size < 255) ? Size : 255;
    memcpy(source, Data, src_len);
    source[src_len] = '\0';

    // Fuzz pcap_create
    p = pcap_create(source, errbuf);
    if (p != NULL) {
        // Fuzz pcap_set_buffer_size
        if (Size > 1) {
            int buf_size = (int)Data[1] * 1024;
            if (buf_size <= 0) buf_size = 1024;
            pcap_set_buffer_size(p, buf_size);
        }

        // Fuzz pcap_set_promisc
        if (Size > 2) {
            int promisc = (Data[2] % 2) ? 1 : 0;
            pcap_set_promisc(p, promisc);
        }

        // Fuzz pcap_setsampling (requires ENABLE_REMOTE, will fail locally but safe to call)
        if (Size > 3) {
            struct pcap_samp *samp = pcap_setsampling(p);
            if (samp != NULL) {
                samp->method = Data[3] % 3; // 0, 1, or 2
                samp->value = (int)Data[4] * 10;
            }
        }

        // Attempt activation via pcap_open logic or direct activation if supported
        // Since pcap_create returns an inactive handle, we can try to activate it.
        // However, pcap_activate is not in the target list, so we rely on pcap_open for full flow.
        // We will close this handle and try pcap_open in a separate block.
        cleanup_pcap(p);
        p = NULL;
    }

    // Fuzz pcap_open
    // Construct a potentially valid source for pcap_open
    char open_source[256];
    if (Size > 5) {
        memcpy(open_source, Data + 5, src_len > 255 ? 255 : src_len);
        open_source[src_len > 255 ? 255 : src_len] = '\0';
    } else {
        strcpy(open_source, "any");
    }

    int snaplen = (Size > 6) ? (int)Data[6] * 64 : 65535;
    if (snaplen <= 0) snaplen = 65535;

    int flags = (Size > 7) ? (int)Data[7] % 8 : 0;
    int timeout = (Size > 8) ? (int)Data[8] * 100 : 1000;

    // Setup auth if data allows
    if (Size > 9) {
        auth.type = Data[9] % 3;
        // We don't allocate dynamic strings for username/password to avoid leaks in fuzzing loop
        // If type requires them, pcap_open will likely fail gracefully or we pass NULL
        auth.username = NULL;
        auth.password = NULL;
    }

    p = pcap_open(open_source, snaplen, flags, timeout, &auth, errbuf);

    if (p != NULL) {
        // Fuzz pcap_breakloop
        // This is safe to call even if not in a loop, it just sets a flag
        pcap_breakloop(p);

        // Cleanup
        cleanup_pcap(p);
        p = NULL;
    }

    // Fuzz pcap_create again with different parameters to explore states
    p = pcap_create(NULL, errbuf); // NULL means "any"
    if (p != NULL) {
        // Try setting options again
        pcap_set_buffer_size(p, 102400);
        pcap_set_promisc(p, 1);
        pcap_setsampling(p); // Will likely return NULL locally
        cleanup_pcap(p);
    }

    return 0;
}