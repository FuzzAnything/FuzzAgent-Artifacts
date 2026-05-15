#include <fuzzer/FuzzedDataProvider.h>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>
#include <random>
#include "magic.h"

namespace harness_016 {
/*
 * Fuzzing harness for libmagic library - Deep Magic Pattern Matching
 * Targets: mget (191 blocked branches), magiccheck (176 blocked branches), 
 *          match (134 blocked branches), file_mdump (143 blocked branches), parse (178 blocked branches)
 * Focus: Deep indirect pattern matching with high indir_count and name_count in softmagic.c
 * Goal: Generate complex inputs to trigger:
 *       1) Deep indirect pattern nesting (high indir_count)
 *       2) Complex name references (high name_count)
 *       3) Multi-level magic rule evaluation
 * Focus formats that trigger recursive magic pattern matching:
 *       Archive formats (ZIP, TAR), document formats (PDF, Office documents),
 *       executable formats (ELF, PE), container formats with layered content
 * Edge cases: offset calculations, type conversions, malformed but parseable headers
 * Semantic differentiation: Existing harnesses test specific formats; this harness
 *       focuses on deep pattern matching complexity with synthetic nested structures
 * Strategy: Use FuzzedDataProvider to generate complex nested binary structures,
 *       test through magic_buffer API with MAGIC_CONTINUE flag for deep matching
 * Expected coverage gain: Target 191+ blocked branches in mget and hundreds more
 *       in related pattern matching functions
 */


// Include libmagic header

// Structure generation helpers for complex nested formats

// Generate a ZIP-like structure (simplified for pattern matching)
void generate_zip_like_structure(FuzzedDataProvider& fdp, std::vector<uint8_t>& output, 
                                 size_t depth, size_t max_depth) {
    if (depth >= max_depth) return;
    
    // Local file header signature
    uint32_t local_header_sig = 0x04034b50;
    output.insert(output.end(), reinterpret_cast<uint8_t*>(&local_header_sig), 
                  reinterpret_cast<uint8_t*>(&local_header_sig) + 4);
    
    // Version needed to extract
    uint16_t version = fdp.ConsumeIntegralInRange<uint16_t>(10, 63);
    output.insert(output.end(), reinterpret_cast<uint8_t*>(&version), 
                  reinterpret_cast<uint8_t*>(&version) + 2);
    
    // General purpose bit flag
    uint16_t gp_flag = fdp.ConsumeIntegral<uint16_t>();
    output.insert(output.end(), reinterpret_cast<uint8_t*>(&gp_flag), 
                  reinterpret_cast<uint8_t*>(&gp_flag) + 2);
    
    // Compression method
    uint16_t compression = fdp.ConsumeIntegralInRange<uint16_t>(0, 20);
    output.insert(output.end(), reinterpret_cast<uint8_t*>(&compression), 
                  reinterpret_cast<uint8_t*>(&compression) + 2);
    
    // Last mod file time and date
    uint16_t mod_time = fdp.ConsumeIntegral<uint16_t>();
    uint16_t mod_date = fdp.ConsumeIntegral<uint16_t>();
    output.insert(output.end(), reinterpret_cast<uint8_t*>(&mod_time), 
                  reinterpret_cast<uint8_t*>(&mod_time) + 2);
    output.insert(output.end(), reinterpret_cast<uint8_t*>(&mod_date), 
                  reinterpret_cast<uint8_t*>(&mod_date) + 2);
    
    // CRC32 (can be valid or invalid)
    uint32_t crc32 = fdp.ConsumeIntegral<uint32_t>();
    output.insert(output.end(), reinterpret_cast<uint8_t*>(&crc32), 
                  reinterpret_cast<uint8_t*>(&crc32) + 4);
    
    // Compressed and uncompressed size
    uint32_t compressed_size = fdp.ConsumeIntegralInRange<uint32_t>(0, 1024);
    uint32_t uncompressed_size = fdp.ConsumeIntegralInRange<uint32_t>(0, 1024);
    output.insert(output.end(), reinterpret_cast<uint8_t*>(&compressed_size), 
                  reinterpret_cast<uint8_t*>(&compressed_size) + 4);
    output.insert(output.end(), reinterpret_cast<uint8_t*>(&uncompressed_size), 
                  reinterpret_cast<uint8_t*>(&uncompressed_size) + 4);
    
    // File name length and extra field length
    uint16_t name_len = fdp.ConsumeIntegralInRange<uint16_t>(1, 256);
    uint16_t extra_len = fdp.ConsumeIntegralInRange<uint16_t>(0, 256);
    output.insert(output.end(), reinterpret_cast<uint8_t*>(&name_len), 
                  reinterpret_cast<uint8_t*>(&name_len) + 2);
    output.insert(output.end(), reinterpret_cast<uint8_t*>(&extra_len), 
                  reinterpret_cast<uint8_t*>(&extra_len) + 2);
    
    // File name (can trigger name references)
    std::string filename = fdp.ConsumeRandomLengthString(name_len);
    output.insert(output.end(), filename.begin(), filename.end());
    
    // Extra field (can contain nested structures)
    if (extra_len > 0) {
        std::vector<uint8_t> extra = fdp.ConsumeBytes<uint8_t>(extra_len);
        output.insert(output.end(), extra.begin(), extra.end());
    }
    
    // File data (potentially with nested structure)
    if (compressed_size > 0 && fdp.remaining_bytes() >= compressed_size) {
        std::vector<uint8_t> file_data = fdp.ConsumeBytes<uint8_t>(compressed_size);
        output.insert(output.end(), file_data.begin(), file_data.end());
        
        // Recursively generate nested structure within file data
        if (depth < max_depth - 1 && fdp.ConsumeBool()) {
            generate_zip_like_structure(fdp, output, depth + 1, max_depth);
        }
    }
}

// Generate a TAR-like structure
void generate_tar_like_structure(FuzzedDataProvider& fdp, std::vector<uint8_t>& output,
                                 size_t depth, size_t max_depth) {
    if (depth >= max_depth) return;
    
    // File name (100 bytes in tar)
    std::string filename = fdp.ConsumeRandomLengthString(100);
    if (filename.length() < 100) {
        filename.resize(100, '\0');
    }
    output.insert(output.end(), filename.begin(), filename.begin() + 100);
    
    // File mode (8 bytes)
    std::string mode = fdp.ConsumeRandomLengthString(8);
    if (mode.length() < 8) mode.resize(8, '0');
    output.insert(output.end(), mode.begin(), mode.begin() + 8);
    
    // Owner and group IDs (16 bytes total)
    std::string owner = fdp.ConsumeRandomLengthString(8);
    std::string group = fdp.ConsumeRandomLengthString(8);
    if (owner.length() < 8) owner.resize(8, '0');
    if (group.length() < 8) group.resize(8, '0');
    output.insert(output.end(), owner.begin(), owner.begin() + 8);
    output.insert(output.end(), group.begin(), group.begin() + 8);
    
    // File size (12 bytes octal)
    uint64_t file_size = fdp.ConsumeIntegralInRange<uint64_t>(0, 999999999999);
    char size_str[13];
    snprintf(size_str, sizeof(size_str), "%011lo", file_size);
    output.insert(output.end(), size_str, size_str + 11);
    output.push_back('\0'); // NUL terminator
    
    // Modification time (12 bytes)
    uint64_t mtime = fdp.ConsumeIntegral<uint64_t>();
    char mtime_str[13];
    snprintf(mtime_str, sizeof(mtime_str), "%011lo", mtime);
    output.insert(output.end(), mtime_str, mtime_str + 11);
    output.push_back('\0');
    
    // Checksum (8 bytes, will be invalid but parseable)
    std::string checksum = fdp.ConsumeRandomLengthString(8);
    if (checksum.length() < 8) checksum.resize(8, ' ');
    output.insert(output.end(), checksum.begin(), checksum.begin() + 8);
    
    // Type flag
    char typeflag = fdp.ConsumeIntegral<uint8_t>() % 8;
    output.push_back(typeflag);
    
    // Link name (100 bytes)
    std::string linkname = fdp.ConsumeRandomLengthString(100);
    if (linkname.length() < 100) linkname.resize(100, '\0');
    output.insert(output.end(), linkname.begin(), linkname.begin() + 100);
    
    // Magic "ustar" and version
    std::string magic = "ustar";
    std::string version = "00";
    output.insert(output.end(), magic.begin(), magic.end());
    output.insert(output.end(), version.begin(), version.end());
    
    // User and group names (64 bytes total)
    std::string uname = fdp.ConsumeRandomLengthString(32);
    std::string gname = fdp.ConsumeRandomLengthString(32);
    if (uname.length() < 32) uname.resize(32, '\0');
    if (gname.length() < 32) gname.resize(32, '\0');
    output.insert(output.end(), uname.begin(), uname.begin() + 32);
    output.insert(output.end(), gname.begin(), gname.begin() + 32);
    
    // Device major/minor (16 bytes)
    std::string devmajor = fdp.ConsumeRandomLengthString(8);
    std::string devminor = fdp.ConsumeRandomLengthString(8);
    if (devmajor.length() < 8) devmajor.resize(8, '\0');
    if (devminor.length() < 8) devminor.resize(8, '\0');
    output.insert(output.end(), devmajor.begin(), devmajor.begin() + 8);
    output.insert(output.end(), devminor.begin(), devminor.begin() + 8);
    
    // Prefix (155 bytes)
    std::string prefix = fdp.ConsumeRandomLengthString(155);
    if (prefix.length() < 155) prefix.resize(155, '\0');
    output.insert(output.end(), prefix.begin(), prefix.begin() + 155);
    
    // Pad to 512 bytes
    size_t current_pos = output.size() % 512;
    if (current_pos > 0) {
        size_t pad_size = 512 - current_pos;
        std::vector<uint8_t> padding(pad_size, 0);
        output.insert(output.end(), padding.begin(), padding.end());
    }
    
    // File content (if regular file)
    if ((typeflag == '0' || typeflag == '\0') && file_size > 0) {
        size_t content_size = (file_size + 511) / 512 * 512; // Round up to 512-byte blocks
        if (fdp.remaining_bytes() >= content_size) {
            std::vector<uint8_t> content = fdp.ConsumeBytes<uint8_t>(content_size);
            output.insert(output.end(), content.begin(), content.end());
            
            // Recursively generate nested tar within content
            if (depth < max_depth - 1 && fdp.ConsumeBool()) {
                generate_tar_like_structure(fdp, output, depth + 1, max_depth);
            }
        }
    }
}

// Generate a PDF-like structure (simplified)
void generate_pdf_like_structure(FuzzedDataProvider& fdp, std::vector<uint8_t>& output,
                                 size_t depth, size_t max_depth) {
    if (depth >= max_depth) return;
    
    // PDF header
    std::string header = "%PDF-";
    header += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 2));
    header += "." + std::to_string(fdp.ConsumeIntegralInRange<int>(0, 7));
    header += "\n%";
    // Add some binary bytes to trigger different parsers
    for (int i = 0; i < 4; i++) {
        header.push_back(fdp.ConsumeIntegral<uint8_t>());
    }
    header += "\n";
    output.insert(output.end(), header.begin(), header.end());
    
    // Object references (trigger indirection)
    size_t num_objects = fdp.ConsumeIntegralInRange<size_t>(1, 10);
    for (size_t i = 0; i < num_objects; i++) {
        std::string obj = std::to_string(i + 1) + " 0 obj\n";
        obj += "<<\n";
        
        // Add dictionary entries
        size_t num_entries = fdp.ConsumeIntegralInRange<size_t>(1, 5);
        for (size_t j = 0; j < num_entries; j++) {
            std::string key = "/" + fdp.ConsumeRandomLengthString(10);
            obj += key + " ";
            
            // Different value types
            uint8_t val_type = fdp.ConsumeIntegral<uint8_t>() % 4;
            switch (val_type) {
                case 0:
                    obj += std::to_string(fdp.ConsumeIntegral<int>());
                    break;
                case 1:
                    obj += "/" + fdp.ConsumeRandomLengthString(10);
                    break;
                case 2:
                    obj += "(" + fdp.ConsumeRandomLengthString(20) + ")";
                    break;
                case 3:
                    obj += "[" + std::to_string(fdp.ConsumeIntegral<int>()) + " " +
                           std::to_string(fdp.ConsumeIntegral<int>()) + " " +
                           std::to_string(fdp.ConsumeIntegral<int>()) + " " +
                           std::to_string(fdp.ConsumeIntegral<int>()) + "]";
                    break;
            }
            obj += "\n";
        }
        
        obj += ">>\n";
        
        // Stream with potentially nested content
        if (fdp.ConsumeBool()) {
            obj += "stream\n";
            size_t stream_len = fdp.ConsumeIntegralInRange<size_t>(10, 100);
            std::string stream_data = fdp.ConsumeRandomLengthString(stream_len);
            obj += stream_data + "\n";
            obj += "endstream\n";
        }
        
        obj += "endobj\n";
        output.insert(output.end(), obj.begin(), obj.end());
    }
    
    // Cross-reference table (simplified)
    std::string xref_header = "xref\n0 " + std::to_string(num_objects + 1) + "\n";
    std::string xref_first = "0000000000 65535 f \n";
    output.insert(output.end(), xref_header.begin(), xref_header.end());
    output.insert(output.end(), xref_first.begin(), xref_first.end());
    
    for (size_t i = 0; i < num_objects; i++) {
        char xref[21];
        snprintf(xref, sizeof(xref), "%010lu 00000 n \n", (unsigned long)(output.size() + i * 20));
        output.insert(output.end(), xref, xref + 20);
    }
    
    // Trailer
    std::string trailer = "trailer\n<<\n/Size " + std::to_string(num_objects + 1) +
                         "\n/Root 1 0 R\n>>\nstartxref\n" + 
                         std::to_string(output.size() - 100) + "\n%%EOF\n";
    output.insert(output.end(), trailer.begin(), trailer.end());
    // Recursively generate nested PDF structures
    if (depth < max_depth - 1 && fdp.ConsumeBool()) {
        generate_pdf_like_structure(fdp, output, depth + 1, max_depth);
    }
}

// Generate an ELF-like structure (simplified)
void generate_elf_like_structure(FuzzedDataProvider& fdp, std::vector<uint8_t>& output,
                                 size_t depth, size_t max_depth) {
    if (depth >= max_depth) return;
    
    // ELF magic
    output.push_back(0x7f);
    output.push_back('E');
    output.push_back('L');
    output.push_back('F');
    
    // Class: 32-bit or 64-bit
    uint8_t elf_class = fdp.ConsumeBool() ? 1 : 2; // 1 = 32-bit, 2 = 64-bit
    output.push_back(elf_class);
    
    // Endianness
    uint8_t endian = fdp.ConsumeBool() ? 1 : 2; // 1 = little, 2 = big
    output.push_back(endian);
    
    // Version
    output.push_back(1);
    
    // OS ABI
    output.push_back(fdp.ConsumeIntegral<uint8_t>());
    
    // ABI version
    output.push_back(fdp.ConsumeIntegral<uint8_t>());
    
    // Padding
    for (int i = 0; i < 7; i++) {
        output.push_back(0);
    }
    
    // Type
    uint16_t e_type = fdp.ConsumeIntegralInRange<uint16_t>(1, 4); // 1 = reloc, 2 = exec, 3 = shared, 4 = core
    output.insert(output.end(), reinterpret_cast<uint8_t*>(&e_type), 
                  reinterpret_cast<uint8_t*>(&e_type) + 2);
    
    // Machine
    uint16_t e_machine = fdp.ConsumeIntegral<uint16_t>();
    output.insert(output.end(), reinterpret_cast<uint8_t*>(&e_machine), 
                  reinterpret_cast<uint8_t*>(&e_machine) + 2);
    
    // Version
    uint32_t e_version = 1;
    output.insert(output.end(), reinterpret_cast<uint8_t*>(&e_version), 
                  reinterpret_cast<uint8_t*>(&e_version) + 4);
    
    // Entry point
    uint64_t e_entry = fdp.ConsumeIntegral<uint64_t>();
    output.insert(output.end(), reinterpret_cast<uint8_t*>(&e_entry), 
                  reinterpret_cast<uint8_t*>(&e_entry) + (elf_class == 1 ? 4 : 8));
    
    // Program header offset
    uint64_t e_phoff = fdp.ConsumeIntegral<uint64_t>();
    output.insert(output.end(), reinterpret_cast<uint8_t*>(&e_phoff), 
                  reinterpret_cast<uint8_t*>(&e_phoff) + (elf_class == 1 ? 4 : 8));
    
    // Section header offset
    uint64_t e_shoff = fdp.ConsumeIntegral<uint64_t>();
    output.insert(output.end(), reinterpret_cast<uint8_t*>(&e_shoff), 
                  reinterpret_cast<uint8_t*>(&e_shoff) + (elf_class == 1 ? 4 : 8));
    
    // Flags
    uint32_t e_flags = fdp.ConsumeIntegral<uint32_t>();
    output.insert(output.end(), reinterpret_cast<uint8_t*>(&e_flags), 
                  reinterpret_cast<uint8_t*>(&e_flags) + 4);
    
    // Header size
    uint16_t e_ehsize = elf_class == 1 ? 52 : 64;
    output.insert(output.end(), reinterpret_cast<uint8_t*>(&e_ehsize), 
                  reinterpret_cast<uint8_t*>(&e_ehsize) + 2);
    
    // Program header entry size
    uint16_t e_phentsize = elf_class == 1 ? 32 : 56;
    output.insert(output.end(), reinterpret_cast<uint8_t*>(&e_phentsize), 
                  reinterpret_cast<uint8_t*>(&e_phentsize) + 2);
    
    // Number of program headers
    uint16_t e_phnum = fdp.ConsumeIntegralInRange<uint16_t>(0, 10);
    output.insert(output.end(), reinterpret_cast<uint8_t*>(&e_phnum), 
                  reinterpret_cast<uint8_t*>(&e_phnum) + 2);
    
    // Section header entry size
    uint16_t e_shentsize = elf_class == 1 ? 40 : 64;
    output.insert(output.end(), reinterpret_cast<uint8_t*>(&e_shentsize), 
                  reinterpret_cast<uint8_t*>(&e_shentsize) + 2);
    
    // Number of section headers
    uint16_t e_shnum = fdp.ConsumeIntegralInRange<uint16_t>(0, 10);
    output.insert(output.end(), reinterpret_cast<uint8_t*>(&e_shnum), 
                  reinterpret_cast<uint8_t*>(&e_shnum) + 2);
    
    // Section header string table index
    uint16_t e_shstrndx = fdp.ConsumeIntegral<uint16_t>();
    output.insert(output.end(), reinterpret_cast<uint8_t*>(&e_shstrndx), 
                  reinterpret_cast<uint8_t*>(&e_shstrndx) + 2);
    
    // Generate program headers (trigger offset calculations)
    for (uint16_t i = 0; i < e_phnum; i++) {
        // p_type
        uint32_t p_type = fdp.ConsumeIntegralInRange<uint32_t>(1, 8); // 1 = load, 2 = dynamic, etc.
        output.insert(output.end(), reinterpret_cast<uint8_t*>(&p_type), 
                      reinterpret_cast<uint8_t*>(&p_type) + 4);
        
        // p_flags
        uint32_t p_flags = fdp.ConsumeIntegral<uint32_t>();
        output.insert(output.end(), reinterpret_cast<uint8_t*>(&p_flags), 
                      reinterpret_cast<uint8_t*>(&p_flags) + 4);
        
        // p_offset
        uint64_t p_offset = fdp.ConsumeIntegral<uint64_t>();
        output.insert(output.end(), reinterpret_cast<uint8_t*>(&p_offset), 
                      reinterpret_cast<uint8_t*>(&p_offset) + (elf_class == 1 ? 4 : 8));
        
        // p_vaddr, p_paddr
        uint64_t p_vaddr = fdp.ConsumeIntegral<uint64_t>();
        uint64_t p_paddr = fdp.ConsumeIntegral<uint64_t>();
        output.insert(output.end(), reinterpret_cast<uint8_t*>(&p_vaddr), 
                      reinterpret_cast<uint8_t*>(&p_vaddr) + (elf_class == 1 ? 4 : 8));
        output.insert(output.end(), reinterpret_cast<uint8_t*>(&p_paddr), 
                      reinterpret_cast<uint8_t*>(&p_paddr) + (elf_class == 1 ? 4 : 8));
        
        // p_filesz, p_memsz
        uint64_t p_filesz = fdp.ConsumeIntegralInRange<uint64_t>(0, 1024);
        uint64_t p_memsz = fdp.ConsumeIntegralInRange<uint64_t>(0, 1024);
        output.insert(output.end(), reinterpret_cast<uint8_t*>(&p_filesz), 
                      reinterpret_cast<uint8_t*>(&p_filesz) + (elf_class == 1 ? 4 : 8));
        output.insert(output.end(), reinterpret_cast<uint8_t*>(&p_memsz), 
                      reinterpret_cast<uint8_t*>(&p_memsz) + (elf_class == 1 ? 4 : 8));
        
        // p_align
        uint64_t p_align = fdp.ConsumeIntegral<uint64_t>();
        output.insert(output.end(), reinterpret_cast<uint8_t*>(&p_align), 
                      reinterpret_cast<uint8_t*>(&p_align) + (elf_class == 1 ? 4 : 8));
    }
    
    // Recursively generate nested ELF-like content
    if (depth < max_depth - 1 && fdp.ConsumeBool()) {
        generate_elf_like_structure(fdp, output, depth + 1, max_depth);
    }
}

// Main fuzzer function
int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough for complex structure generation
    if (size < 64) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume flags for magic_open - focus on CONTINUE flag for deep matching
    int flags = 0;
    
    // Consume a byte to determine which flags to set
    uint8_t flag_byte = fdp.ConsumeIntegral<uint8_t>();
    if (flag_byte & 0x01) flags |= MAGIC_DEBUG;
    if (flag_byte & 0x02) flags |= MAGIC_SYMLINK;
    if (flag_byte & 0x04) flags |= MAGIC_COMPRESS;
    if (flag_byte & 0x08) flags |= MAGIC_DEVICES;
    if (flag_byte & 0x10) flags |= MAGIC_MIME_TYPE;
    if (flag_byte & 0x20) flags |= MAGIC_CONTINUE; // Critical for deep matching
    if (flag_byte & 0x40) flags |= MAGIC_CHECK;
    if (flag_byte & 0x80) flags |= MAGIC_RAW;
    
    // Open magic handle
    magic_t magic = magic_open(flags);
    if (magic == NULL) {
        return 0; // Failed to open magic handle
    }
    
    // Try to load magic database - use default path
    const char* magic_path = NULL; // NULL means use default
    int load_result = magic_load(magic, magic_path);
    
    // If loading fails, close and return
    if (load_result == -1) {
        magic_close(magic);
        return 0;
    }
    
    // Determine structure type to generate
    uint8_t structure_type = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    // Generate complex nested structure
    std::vector<uint8_t> buffer;
    size_t max_depth = fdp.ConsumeIntegralInRange<size_t>(1, 5); // Up to 5 levels deep
    
    switch (structure_type) {
        case 0:
            generate_zip_like_structure(fdp, buffer, 0, max_depth);
            break;
        case 1:
            generate_tar_like_structure(fdp, buffer, 0, max_depth);
            break;
        case 2:
            generate_pdf_like_structure(fdp, buffer, 0, max_depth);
            break;
        case 3:
            generate_elf_like_structure(fdp, buffer, 0, max_depth);
            break;
    }
    
    // If buffer is empty or too small, use remaining bytes
    if (buffer.empty() || buffer.size() < 16) {
        size_t remaining = std::min(fdp.remaining_bytes(), static_cast<size_t>(4096));
        std::vector<uint8_t> remaining_data = fdp.ConsumeBytes<uint8_t>(remaining);
        buffer.insert(buffer.end(), remaining_data.begin(), remaining_data.end());
    }
    
    // Test with magic_buffer - this should trigger deep pattern matching
    if (!buffer.empty()) {
        const char* result = magic_buffer(magic, buffer.data(), buffer.size());
        (void)result; // Result can be NULL, that's OK
    }
    
    // Additional test: Try with MIME type detection
    if (fdp.ConsumeBool() && !buffer.empty()) {
        // Temporarily set MIME flag
        int old_flags = magic_getflags(magic);
        magic_setflags(magic, old_flags | MAGIC_MIME_TYPE);
        const char* mime_result = magic_buffer(magic, buffer.data(), buffer.size());
        (void)mime_result;
        magic_setflags(magic, old_flags); // Restore original flags
    }
    
    // Test error handling
    if (fdp.ConsumeBool()) {
        const char* err = magic_error(magic);
        (void)err;
    }
    
    // Test getting magic version (using magic_version() function)
    if (fdp.ConsumeBool()) {
        int version = magic_version();
        (void)version;
    }
    // Clean up
    magic_close(magic);
    
    return 0;
}

} // namespace harness_016
