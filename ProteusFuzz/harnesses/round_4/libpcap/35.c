// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_nametoaddrinfo at nametoaddr.c:178:1 in namedb.h
// pcap_ether_hostton at nametoaddr.c:887:1 in namedb.h
// pcap_ether_aton at nametoaddr.c:820:1 in namedb.h
// pcap_nametonetaddr at nametoaddr.c:220:1 in namedb.h
// pcap_next_etherent at etherent.c:73:1 in namedb.h
// pcap_nametoaddr at nametoaddr.c:138:1 in namedb.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/ether.h>
#include <pcap.h>
#include <namedb.h>

static void sanitize_input(const uint8_t *Data, size_t Size, char *out, size_t max_len) {
    size_t copy_len = Size < max_len - 1 ? Size : max_len - 1;
    memcpy(out, Data, copy_len);
    out[copy_len] = '\0';
    
    // Remove null bytes in the middle to ensure valid C strings for API calls
    for (size_t i = 0; i < copy_len; i++) {
        if (out[i] == '\0') {
            memmove(out + i, out + i + 1, copy_len - i);
            out[copy_len - 1] = '\0';
            break;
        }
    }
}

int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    char buffer[256];
    sanitize_input(Data, Size, buffer, sizeof(buffer));

    // 1. pcap_nametoaddrinfo
    struct addrinfo *ai_res = pcap_nametoaddrinfo(buffer);
    if (ai_res != NULL) {
        freeaddrinfo(ai_res);
    }

    // 2. pcap_ether_hostton
    u_char *ether_host_res = pcap_ether_hostton(buffer);
    if (ether_host_res != NULL) {
        free(ether_host_res);
    }

    // 3. pcap_ether_aton
    u_char *ether_aton_res = pcap_ether_aton(buffer);
    if (ether_aton_res != NULL) {
        free(ether_aton_res);
    }

    // 4. pcap_nametonetaddr
    bpf_u_int32 net_addr = pcap_nametonetaddr(buffer);
    (void)net_addr; // Suppress unused variable warning

    // 5. pcap_next_etherent
    // Create a dummy file with potential etherent data
    const char *dummy_file = "./dummy_file";
    FILE *fp = fopen(dummy_file, "w+");
    if (fp != NULL) {
        // Write the fuzzed data as potential content
        fwrite(buffer, 1, Size, fp);
        // Add some newlines to ensure parsing logic is triggered
        fprintf(fp, "\n%s\n", buffer);
        fclose(fp);

        fp = fopen(dummy_file, "r");
        if (fp != NULL) {
            struct pcap_etherent *etherent;
            while ((etherent = pcap_next_etherent(fp)) != NULL) {
                // Use the data to ensure it's processed
                (void)etherent->addr[0];
                (void)etherent->name[0];
            }
            fclose(fp);
        }
        remove(dummy_file);
    }

    // 6. pcap_nametoaddr (Deprecated but tested for coverage)
    // Note: This function returns a pointer to a static array.
    // We cast to void* to suppress warnings about deprecated usage in some compilers
    // but the function signature returns struct in_addr *.
    // Since the return type is struct in_addr *, we can't easily check NULL without casting.
    // We assume the function returns a valid pointer or NULL.
    struct in_addr *addr_res = (struct in_addr *)pcap_nametoaddr(buffer);
    if (addr_res != NULL) {
        // Just access to ensure validity, do not free as it's static
        (void)addr_res->s_addr;
    }

    return 0;
}