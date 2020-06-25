#pragma once

#include <string_view>
#include <memory>
#include <unordered_map>
#include <vector>
#include <climits>
#include <iostream>

#include "BaseDictionary.hpp"

#include <iostream>
#include <unordered_map>

class Trie
{
public:
    uint32_t code;
    std::unordered_map<char, Trie*> map;
    Trie(uint32_t _code): code(_code) {}
    ~Trie() {
        for(const auto &kv: map) {
            delete kv.second;
        }
    } 
};


class EncoderDictionary : BaseDictionary {
    Trie *m_trie = nullptr;
    uint32_t m_size = 0;

    void insert(char c, uint32_t code) override {
        char* pc = &c;
        insertNewCode(pc, 1);
    }
public:
    EncoderDictionary() {
        initializeDictionary();
    }

    void reset() {
        delete m_trie;
        initializeDictionary();
    }

    uint32_t insertNewCode(char*& str, size_t len)
    {
        if (m_trie == nullptr) {
            // code don't matter here since we add all characters in initialization
            m_trie = new Trie(0);
        }

        // start from root node
        Trie* current_node = m_trie;
        size_t idx = 0;

        while (idx < len)
        {
            char c = str[idx];
            if (current_node->map.find(c) == current_node->map.end())  {
                // we found place where sequence don't exists in trie
                // add it with new code; 
                // std::cout << "adding new code " << std::string_view(str, idx) << " " << m_size + 1 << std::endl;

                current_node->map[c] = new Trie(m_size++);
                // move ptr
                str += idx;
                // return old code
                return current_node->code;
            }

            // node exists in trie, go to next node
            current_node = current_node->map[c];

            // move to next character
            idx++;
        }

        str += idx;
        // mark current node as leaf
        return current_node->code;
    }

    size_t size() const {
        return m_size;
    }

    ~EncoderDictionary() {
        delete m_trie;
    }
};