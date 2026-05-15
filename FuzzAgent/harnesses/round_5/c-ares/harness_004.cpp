#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include "ares.h"

namespace harness_004 {
/* Comprehensive fuzzing harness for c-ares library targeting legacy DNS reply parsing functions
 * identified as critical coverage gaps by CoverageAnalyzerAgent:
 * - ares_parse_caa_reply (182 undiscovered branches)
 * - ares_parse_ns_reply (168 undiscovered branches)
 * - ares_parse_aaaa_reply (166 undiscovered branches)
 * - ares_parse_naptr_reply (166 undiscovered branches)
 * - ares_parse_a_reply (166 undiscovered branches)
 * - ares_parse_soa_reply (164 undiscovered branches)
 * - ares_parse_uri_reply (161 undiscovered branches)
 * - ares_parse_srv_reply (160 undiscovered branches)
 * - ares_parse_mx_reply (160 undiscovered branches)
 * - ares_parse_ptr_reply (140 undiscovered branches)
 * - ares_parse_txt_reply_ext (138 undiscovered branches)
 * - ares_parse_txt_reply (138 undiscovered branches)
 *
 * This harness implements DNS response parsing for all record types with semantic
 * diversity from previous harnesses:
 * - harness_000: getaddrinfo and modern APIs
 * - harness_001: DNS processing pipeline  
 * - harness_002: event processing and channel management
 * - harness_003: server configuration APIs
 *
 * Focus: Generate valid DNS response buffers, call corresponding parsing functions,
 * handle parsed output structures, clean up memory using appropriate free functions.
 */



/* DNS record type constants */
#define DNS_TYPE_A      1
#define DNS_TYPE_NS     2
#define DNS_TYPE_CNAME  5
#define DNS_TYPE_SOA    6
#define DNS_TYPE_PTR    12
#define DNS_TYPE_MX     15
#define DNS_TYPE_TXT    16
#define DNS_TYPE_AAAA   28
#define DNS_TYPE_SRV    33
#define DNS_TYPE_NAPTR  35
#define DNS_TYPE_CAA    257
#define DNS_TYPE_URI    256

/* DNS class constants */
#define DNS_CLASS_IN    1

/* DNS header structure */
struct dns_header {
    uint16_t id;
    uint16_t flags;
    uint16_t qdcount;
    uint16_t ancount;
    uint16_t nscount;
    uint16_t arcount;
};

/* Helper function to push 16-bit value in network byte order */
static void push_uint16(std::vector<uint8_t> &data, uint16_t value) {
    data.push_back((value >> 8) & 0xFF);
    data.push_back(value & 0xFF);
}

/* Helper function to push 32-bit value in network byte order */
static void push_uint32(std::vector<uint8_t> &data, uint32_t value) {
    data.push_back((value >> 24) & 0xFF);
    data.push_back((value >> 16) & 0xFF);
    data.push_back((value >> 8) & 0xFF);
    data.push_back(value & 0xFF);
}

/* Helper to encode DNS name (simple implementation) */
static void encode_name(std::vector<uint8_t> &data, const char *name) {
    const char *p = name;
    while (*p) {
        const char *dot = strchr(p, '.');
        size_t len = dot ? (size_t)(dot - p) : strlen(p);
        data.push_back((uint8_t)len);
        for (size_t i = 0; i < len; i++) {
            data.push_back((uint8_t)p[i]);
        }
        p = dot ? dot + 1 : p + len;
    }
    data.push_back(0); // null terminator
}

/* Generate a simple DNS response for testing */
static std::vector<uint8_t> generate_dns_response(uint16_t type, FuzzedDataProvider &fdp) {
    std::vector<uint8_t> response;
    
    /* DNS header */
    struct dns_header hdr;
    hdr.id = fdp.ConsumeIntegral<uint16_t>();
    hdr.flags = 0x8180; /* Standard response with recursion available */
    hdr.qdcount = 1;    /* One question */
    hdr.ancount = 1;    /* One answer */
    hdr.nscount = 0;
    hdr.arcount = 0;
    
    push_uint16(response, hdr.id);
    push_uint16(response, hdr.flags);
    push_uint16(response, hdr.qdcount);
    push_uint16(response, hdr.ancount);
    push_uint16(response, hdr.nscount);
    push_uint16(response, hdr.arcount);
    
    /* Question section */
    std::string domain = fdp.ConsumeRandomLengthString(63);
    if (domain.empty()) domain = "example.com";
    encode_name(response, domain.c_str());
    push_uint16(response, type);
    push_uint16(response, DNS_CLASS_IN);
    
    /* Answer section - start with pointer to name */
    response.push_back(0xC0); /* Pointer */
    response.push_back(0x0C); /* Offset to name in question section */
    
    push_uint16(response, type);
    push_uint16(response, DNS_CLASS_IN);
    push_uint32(response, fdp.ConsumeIntegral<uint32_t>() & 0x7FFFFFFF); /* TTL */
    
    /* RDATA length and content depends on record type */
    switch (type) {
        case DNS_TYPE_A: {
            /* A record: 4-byte IPv4 address */
            push_uint16(response, 4);
            std::vector<uint8_t> addr = fdp.ConsumeBytes<uint8_t>(4);
            if (addr.size() < 4) {
                for (int i = 0; i < 4; i++) response.push_back(fdp.ConsumeIntegral<uint8_t>());
            } else {
                response.insert(response.end(), addr.begin(), addr.end());
            }
            break;
        }
        case DNS_TYPE_AAAA: {
            /* AAAA record: 16-byte IPv6 address */
            push_uint16(response, 16);
            std::vector<uint8_t> addr = fdp.ConsumeBytes<uint8_t>(16);
            if (addr.size() < 16) {
                for (int i = 0; i < 16; i++) response.push_back(fdp.ConsumeIntegral<uint8_t>());
            } else {
                response.insert(response.end(), addr.begin(), addr.end());
            }
            break;
        }
        case DNS_TYPE_CAA: {
            /* CAA record: flags(1) + tag + value */
            uint8_t critical = fdp.ConsumeBool() ? 0x80 : 0x00;
            std::string tag = fdp.ConsumeRandomLengthString(15);
            std::string value = fdp.ConsumeRandomLengthString(100);
            
            size_t rdata_len = 1 + tag.length() + value.length();
            push_uint16(response, (uint16_t)rdata_len);
            response.push_back(critical);
            response.insert(response.end(), tag.begin(), tag.end());
            response.insert(response.end(), value.begin(), value.end());
            break;
        }
        case DNS_TYPE_TXT: {
            /* TXT record: length-prefixed strings */
            std::string txt_data = fdp.ConsumeRandomLengthString(255);
            push_uint16(response, (uint16_t)(txt_data.length() + 1));
            response.push_back((uint8_t)txt_data.length());
            response.insert(response.end(), txt_data.begin(), txt_data.end());
            break;
        }
        case DNS_TYPE_MX: {
            /* MX record: preference(2) + exchange name */
            std::string mx_exchange = fdp.ConsumeRandomLengthString(63);
            if (mx_exchange.empty()) mx_exchange = "mail.example.com";
            size_t mx_rdata_len = 2 + mx_exchange.length() + 2; /* preference + name */
            push_uint16(response, (uint16_t)mx_rdata_len);
            push_uint16(response, fdp.ConsumeIntegral<uint16_t>()); /* preference */
            encode_name(response, mx_exchange.c_str());
            break;
        }
        case DNS_TYPE_NS: {
            /* NS record: name server name */
            std::string ns_name = fdp.ConsumeRandomLengthString(63);
            if (ns_name.empty()) ns_name = "ns1.example.com";
            size_t ns_rdata_len = ns_name.length() + 2; /* name length */
            push_uint16(response, (uint16_t)ns_rdata_len);
            encode_name(response, ns_name.c_str());
            break;
        }
        case DNS_TYPE_PTR: {
            /* PTR record: domain name */
            std::string ptr_name = fdp.ConsumeRandomLengthString(63);
            if (ptr_name.empty()) ptr_name = "ptr.example.com";
            size_t ptr_rdata_len = ptr_name.length() + 2; /* name length */
            push_uint16(response, (uint16_t)ptr_rdata_len);
            encode_name(response, ptr_name.c_str());
            break;
        }
        case DNS_TYPE_SRV: {
            /* SRV record: priority(2) + weight(2) + port(2) + target */
            std::string srv_target = fdp.ConsumeRandomLengthString(63);
            if (srv_target.empty()) srv_target = "srv.example.com";
            size_t srv_rdata_len = 6 + srv_target.length() + 2; /* priority+weight+port+name */
            push_uint16(response, (uint16_t)srv_rdata_len);
            push_uint16(response, fdp.ConsumeIntegral<uint16_t>()); /* priority */
            push_uint16(response, fdp.ConsumeIntegral<uint16_t>()); /* weight */
            push_uint16(response, fdp.ConsumeIntegral<uint16_t>()); /* port */
            encode_name(response, srv_target.c_str());
            break;
        }
        case DNS_TYPE_SOA: {
            /* SOA record: mname + rname + serial + refresh + retry + expire + minimum */
            std::string mname = fdp.ConsumeRandomLengthString(63);
            if (mname.empty()) mname = "ns1.example.com";
            std::string rname = fdp.ConsumeRandomLengthString(63);
            if (rname.empty()) rname = "hostmaster.example.com";
            size_t soa_rdata_len = mname.length() + 2 + rname.length() + 2 + 20; /* names + 5x4-byte fields */
            push_uint16(response, (uint16_t)soa_rdata_len);
            encode_name(response, mname.c_str()); /* mname */
            encode_name(response, rname.c_str()); /* rname */
            push_uint32(response, fdp.ConsumeIntegral<uint32_t>()); /* serial */
            push_uint32(response, fdp.ConsumeIntegral<uint32_t>()); /* refresh */
            push_uint32(response, fdp.ConsumeIntegral<uint32_t>()); /* retry */
            push_uint32(response, fdp.ConsumeIntegral<uint32_t>()); /* expire */
            push_uint32(response, fdp.ConsumeIntegral<uint32_t>()); /* minimum */
            break;
        }
        case DNS_TYPE_NAPTR: {
            /* NAPTR record: order(2) + preference(2) + flags + services + regexp + replacement */
            std::string replacement = fdp.ConsumeRandomLengthString(63);
            if (replacement.empty()) replacement = "naptr.example.com";
            size_t naptr_rdata_len = 8 + replacement.length() + 2; /* order+preference+flags+services+regexp+replacement */
            push_uint16(response, (uint16_t)naptr_rdata_len);
            push_uint16(response, fdp.ConsumeIntegral<uint16_t>()); /* order */
            push_uint16(response, fdp.ConsumeIntegral<uint16_t>()); /* preference */
            response.push_back((uint8_t)'U'); /* flags */
            response.push_back((uint8_t)'S'); /* services */
            response.push_back((uint8_t)'*'); /* regexp */
            response.push_back((uint8_t)0);   /* regexp terminator */
            encode_name(response, replacement.c_str()); /* replacement */
            break;
        }
        case DNS_TYPE_URI: {
            /* URI record: priority(2) + weight(2) + target */
            push_uint16(response, 20); /* Fixed size */
            push_uint16(response, fdp.ConsumeIntegral<uint16_t>()); /* priority */
            push_uint16(response, fdp.ConsumeIntegral<uint16_t>()); /* weight */
            std::string uri = fdp.ConsumeRandomLengthString(100);
            response.insert(response.end(), uri.begin(), uri.end());
            break;
        }
        default:
            /* Generic fallback for unknown types */
            push_uint16(response, 4);
            response.push_back(0x01);
            response.push_back(0x02);
            response.push_back(0x03);
            response.push_back(0x04);
            break;
    }
    return response;
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    /* Minimum size check - need enough data for operation selection and DNS response generation */
    if (size < 32) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    /* Initialize c-ares library */
    ares_library_init(ARES_LIB_INIT_ALL);
    
    /* Select which parsing function to test */
    uint8_t parser_select = fdp.ConsumeIntegral<uint8_t>() % 12;
    
    /* Generate DNS response based on selected parser */
    std::vector<uint8_t> dns_response;
    uint16_t dns_type = DNS_TYPE_A;
    
    switch (parser_select) {
        case 0:  dns_type = DNS_TYPE_CAA; break;
        case 1:  dns_type = DNS_TYPE_NS; break;
        case 2:  dns_type = DNS_TYPE_AAAA; break;
        case 3:  dns_type = DNS_TYPE_NAPTR; break;
        case 4:  dns_type = DNS_TYPE_A; break;
        case 5:  dns_type = DNS_TYPE_SOA; break;
        case 6:  dns_type = DNS_TYPE_URI; break;
        case 7:  dns_type = DNS_TYPE_SRV; break;
        case 8:  dns_type = DNS_TYPE_MX; break;
        case 9:  dns_type = DNS_TYPE_PTR; break;
        case 10: dns_type = DNS_TYPE_TXT; break;
        case 11: dns_type = DNS_TYPE_TXT; break; /* TXT and TXT_EXT use same type */
        default: dns_type = DNS_TYPE_A; break;
    }
    
    dns_response = generate_dns_response(dns_type, fdp);
    
    /* Call the appropriate parsing function */
    int parse_result = ARES_ENODATA;
    
    switch (parser_select) {
        case 0: { /* ares_parse_caa_reply */
            struct ares_caa_reply *caa_out = NULL;
            parse_result = ares_parse_caa_reply(dns_response.data(), (int)dns_response.size(), &caa_out);
            if (caa_out) ares_free_data(caa_out);
            break;
        }
        case 1: { /* ares_parse_ns_reply */
            struct hostent *host_out = NULL;
            parse_result = ares_parse_ns_reply(dns_response.data(), (int)dns_response.size(), &host_out);
            if (host_out) ares_free_hostent(host_out);
            break;
        }
        case 2: { /* ares_parse_aaaa_reply */
            struct hostent *host_out = NULL;
            struct ares_addr6ttl addrttls[2];
            int naddrttls = 2;
            parse_result = ares_parse_aaaa_reply(dns_response.data(), (int)dns_response.size(), 
                                                 &host_out, addrttls, &naddrttls);
            if (host_out) ares_free_hostent(host_out);
            break;
        }
        case 3: { /* ares_parse_naptr_reply */
            struct ares_naptr_reply *naptr_out = NULL;
            parse_result = ares_parse_naptr_reply(dns_response.data(), (int)dns_response.size(), &naptr_out);
            if (naptr_out) ares_free_data(naptr_out);
            break;
        }
        case 4: { /* ares_parse_a_reply */
            struct hostent *host_out = NULL;
            struct ares_addrttl addrttls[2];
            int naddrttls = 2;
            parse_result = ares_parse_a_reply(dns_response.data(), (int)dns_response.size(), 
                                              &host_out, addrttls, &naddrttls);
            if (host_out) ares_free_hostent(host_out);
            break;
        }
        case 5: { /* ares_parse_soa_reply */
            struct ares_soa_reply *soa_out = NULL;
            parse_result = ares_parse_soa_reply(dns_response.data(), (int)dns_response.size(), &soa_out);
            if (soa_out) ares_free_data(soa_out);
            break;
        }
        case 6: { /* ares_parse_uri_reply */
            struct ares_uri_reply *uri_out = NULL;
            parse_result = ares_parse_uri_reply(dns_response.data(), (int)dns_response.size(), &uri_out);
            if (uri_out) ares_free_data(uri_out);
            break;
        }
        case 7: { /* ares_parse_srv_reply */
            struct ares_srv_reply *srv_out = NULL;
            parse_result = ares_parse_srv_reply(dns_response.data(), (int)dns_response.size(), &srv_out);
            if (srv_out) ares_free_data(srv_out);
            break;
        }
        case 8: { /* ares_parse_mx_reply */
            struct ares_mx_reply *mx_out = NULL;
            parse_result = ares_parse_mx_reply(dns_response.data(), (int)dns_response.size(), &mx_out);
            if (mx_out) ares_free_data(mx_out);
            break;
        }
        case 9: { /* ares_parse_ptr_reply */
            struct hostent *host_out = NULL;
            /* Generate a simple address for PTR lookup */
            struct in_addr addr;
            addr.s_addr = fdp.ConsumeIntegral<uint32_t>();
            parse_result = ares_parse_ptr_reply(dns_response.data(), (int)dns_response.size(), 
                                                &addr, sizeof(addr), AF_INET, &host_out);
            if (host_out) ares_free_hostent(host_out);
            break;
        }
        case 10: { /* ares_parse_txt_reply */
            struct ares_txt_reply *txt_out = NULL;
            parse_result = ares_parse_txt_reply(dns_response.data(), (int)dns_response.size(), &txt_out);
            if (txt_out) ares_free_data(txt_out);
            break;
        }
        case 11: { /* ares_parse_txt_reply_ext */
            struct ares_txt_ext *txt_out = NULL;
            parse_result = ares_parse_txt_reply_ext(dns_response.data(), (int)dns_response.size(), &txt_out);
            if (txt_out) ares_free_data(txt_out);
            break;
        }
    }
    
    /* Clean up */
    ares_library_cleanup();
    
    /* Ignore parse result - fuzzer cares about code paths, not correctness */
    (void)parse_result;
    
    return 0;
}

} // namespace harness_004
