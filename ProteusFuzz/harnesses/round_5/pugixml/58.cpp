// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::append_buffer at pugixml.cpp:6628:42 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::remove_children at pugixml.cpp:6607:30 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5977:30 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::remove_attributes at pugixml.cpp:6561:30 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6846:30 in pugixml.hpp
// pugi::xml_node::append_buffer at pugixml.cpp:6628:42 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5977:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5977:30 in pugixml.hpp
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <pugixml.hpp>

// Helper class to write XML to a string buffer
class StringStreamWriter : public pugi::xml_writer {
public:
    StringStreamWriter(std::string& out) : m_out(out) {}

    // pugixml xml_writer::write takes const void*, not char_t*
    void write(const void* data, size_t size) override {
        if (data && size > 0) {
            m_out.append(static_cast<const char*>(data), size);
        }
    }

private:
    std::string& m_out;
};

// Helper to generate random integers
static uint32_t get_random_int(const uint8_t* data, size_t size, size_t offset) {
    if (offset + 4 > size) return 0;
    return static_cast<uint32_t>(data[offset]) |
           (static_cast<uint32_t>(data[offset + 1]) << 8) |
           (static_cast<uint32_t>(data[offset + 2]) << 16) |
           (static_cast<uint32_t>(data[offset + 3]) << 24);
}

extern "C" int LLVMFuzzerTestOneInput_58(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    pugi::xml_document doc;

    // Write data to dummy file
    FILE* f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }

    // pugixml uses unsigned int for options, not a specific enum type like xml_parse_options
    unsigned int options = pugi::parse_default;
    pugi::xml_encoding encoding = pugi::encoding_auto;
    
    uint32_t flags = get_random_int(Data, Size, 0);
    
    if (flags & 1) encoding = pugi::encoding_utf8;
    else if (flags & 2) encoding = pugi::encoding_utf16_le;
    else if (flags & 4) encoding = pugi::encoding_utf16_be;
    
    // Use correct option flags
    if (flags & 8) options |= pugi::parse_minimal;
    if (flags & 16) options |= pugi::parse_wconv_attribute;
    // Note: parse_wconv_text is not a standard flag in all versions, skipping or using parse_escapes if needed
    // For safety, we stick to known flags. If parse_escapes exists, we could use it.
    // Assuming standard flags: parse_default, parse_minimal, parse_wconv_attribute, parse_escapes
    if (flags & 32) options |= pugi::parse_escapes;

    // Attempt to parse the input as a document fragment or full document
    doc.append_buffer(Data, Size, options, encoding);

    // Get the root node
    pugi::xml_node root = doc.root();

    // API 6: type() - Check node type safely
    pugi::xml_node_type nodeType = root.type();
    (void)nodeType;

    // API 2: append_child - Add children dynamically
    if (root.type() != pugi::node_null) {
        // Add an element
        pugi::xml_node child1 = root.append_child(pugi::node_element);
        if (child1) {
            child1.set_name("fuzz_element");
            // Add text
            pugi::xml_node textNode = child1.append_child(pugi::node_pcdata);
            if (textNode) {
                textNode.set_value("fuzz_text");
            }
        }
        
        // Add a comment
        pugi::xml_node commentNode = root.append_child(pugi::node_comment);
        if (commentNode) {
            commentNode.set_value("fuzz_comment");
        }

        // Add a processing instruction
        pugi::xml_node piNode = root.append_child(pugi::node_pi);
        if (piNode) {
            piNode.set_name("fuzz_pi");
            piNode.set_value("target");
        }
    }

    // API 5: append_buffer - Parse more fragments into the current node
    if (root.type() == pugi::node_element || root.type() == pugi::node_document) {
        pugi::xml_parse_result res = root.append_buffer(Data, Size, options, encoding);
        (void)res;
    }

    // API 3: remove_children - Delete all children
    if (root.type() != pugi::node_null) {
        bool removed = root.remove_children();
        (void)removed;
    }

    // Re-add some children to test remove_attributes
    if (root.type() != pugi::node_null) {
        pugi::xml_node testNode = root.append_child(pugi::node_element);
        if (testNode) {
            testNode.set_name("test");
            testNode.append_attribute("attr1").set_value("val1");
            testNode.append_attribute("attr2").set_value("val2");
            
            // API 1: remove_attributes - Delete all attributes
            bool attrsRemoved = testNode.remove_attributes();
            (void)attrsRemoved;
        }
    }

    // API 4: print - Serialize the tree
    std::string output;
    StringStreamWriter writer(output);
    
    // Print with different flags
    unsigned int fmtFlags = pugi::format_default;
    uint32_t printFlags = get_random_int(Data, Size, 4);
    
    if (printFlags & 1) fmtFlags |= pugi::format_indent;
    if (printFlags & 2) fmtFlags |= pugi::format_raw;
    if (printFlags & 4) fmtFlags |= pugi::format_no_escapes;
    if (printFlags & 8) fmtFlags |= pugi::format_no_declaration;
    if (printFlags & 16) fmtFlags |= pugi::format_skip_control_chars;
    if (printFlags & 32) fmtFlags |= pugi::format_write_bom;
    
    // Print the document
    doc.print(writer, "\t", fmtFlags, pugi::encoding_utf8, 0);

    return 0;
}