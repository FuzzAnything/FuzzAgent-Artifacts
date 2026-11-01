// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5797:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5797:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::value at pugixml.cpp:5700:39 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
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
#include <string_view>

static class CustomWalker : public pugi::xml_tree_walker {
public:
    CustomWalker() : depth_limit(0), stop_traversal(false) {}

    bool begin(pugi::xml_node& node) override {
        if (!node) return false;
        
        size_t name_len = 0;
        if (node.name()) {
            name_len = std::strlen(node.name());
        }
        
        if (depth_limit > 0 && depth_limit < 5) {
            depth_limit++;
        }
        
        if (depth_limit > 3 || name_len == 0) {
            return false;
        }
        
        return true;
    }

    bool end(pugi::xml_node& node) override {
        if (!node) return false;
        
        pugi::xml_node parent = node.parent();
        pugi::xml_node root = node.root();
        
        if (parent) {
            (void)parent.name();
        }
        
        if (root) {
            (void)root.name();
        }
        
        if (stop_traversal) {
            return false;
        }
        
        return true;
    }

    bool for_each(pugi::xml_node& node) override {
        if (!node) return false;
        
        (void)node.name();
        (void)node.value();
        
        pugi::xml_node parent = node.parent();
        pugi::xml_node root = node.root();
        
        if (parent) {
            (void)parent.name();
        }
        if (root) {
            (void)root.name();
        }
        
        return true;
    }

    void set_depth_limit(int limit) { depth_limit = limit; }
    void set_stop(bool stop) { stop_traversal = stop; }

private:
    int depth_limit;
    bool stop_traversal;
};

extern "C" int LLVMFuzzerTestOneInput_88(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    pugi::xml_document doc;
    
    // Ensure the input is null-terminated before passing to load_string
    // We create a temporary buffer to hold the data plus a null terminator
    std::vector<char> buffer(Size + 1);
    std::memcpy(buffer.data(), Data, Size);
    buffer[Size] = '\0';

    pugi::xml_parse_result result = doc.load_string(buffer.data());
    
    CustomWalker walker;
    
    if (Size > 0) {
        walker.set_depth_limit(Data[0] % 10);
        walker.set_stop((Data[0] & 1) != 0);
    }

    if (doc.root()) {
        doc.traverse(walker);
    }

    for (pugi::xml_node node = doc.first_child(); node; node = node.next_sibling()) {
        pugi::xml_node parent = node.parent();
        if (parent) {
            (void)parent.name();
        }

        pugi::xml_node root = node.root();
        if (root) {
            (void)root.name();
        }

        pugi::xml_node next_sib = node.next_sibling();
        if (next_sib) {
            (void)next_sib.name();
        }

        std::string_view name_view;
        if (Size > 5) {
            size_t len = (Size > 20) ? 20 : Size;
            // Create a string_view from the buffer which is null-terminated
            name_view = std::string_view(buffer.data(), len);
        } else {
            name_view = std::string_view("test");
        }

        pugi::xml_node next_sib_named = node.next_sibling(name_view);
        if (next_sib_named) {
            (void)next_sib_named.name();
        }
    }

    pugi::xml_node root_node = doc.root();
    if (root_node) {
        (void)root_node.parent();
        (void)root_node.root();
        (void)root_node.next_sibling();
        (void)root_node.next_sibling(std::string_view("dummy"));
    }

    return 0;
}