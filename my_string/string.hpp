#ifndef STRING_HPP
#define STRING_HPP
#include <iostream>
#include <cstring>
#include <utility>
#include <stdexcept>

const size_t npos = -1;
class String {
private: 
    size_t m_capacity {};
    size_t m_size {}; 
    union { 
        char onstack[16]; 
        char* m_ptr = nullptr;
    } string;
    bool isOnHeap {false};
    void reallocation();
    void reallocation(int size);
        
public: //constructors and destructor
    String();
    String(const String& src); 
    String(const char* str);
    String(const String& src, size_t pos, size_t len = npos);
    String(size_t count, char sym);
    String(String&& src) noexcept;
    ~String();

public: //operators
    String& operator=(const String& rhs); 
    String& operator=(String&& rhs) noexcept;
    String& operator=(const char* str); 
    String& operator=(char c);
    constexpr char& operator[](size_t pos);
    constexpr const char& operator[](size_t pos) const;
    constexpr String& operator+=(const String& str);
    constexpr String& operator+=(const char* c_str);
    constexpr String& operator+=(char sym);

public: //member functions
    constexpr const char* begin() const noexcept;
    constexpr const char* end() const noexcept;
    constexpr size_t size() const noexcept;
    constexpr size_t length() const noexcept;
    constexpr size_t max_size() const noexcept;
    constexpr size_t capacity() const noexcept;
    constexpr void resize(size_t count);
    constexpr void resize(size_t count, char sym);
    constexpr void clear() noexcept;
    constexpr bool empty() const noexcept;
    constexpr char& at(size_t pos);
    constexpr const char& at(size_t pos) const;
    constexpr const char& back() const;
    constexpr const char& front() const;
    constexpr String& append(size_t count, char sym);
    constexpr String& append(const String& str);
    constexpr void push_back(char sym);
    constexpr void pop_back();
    constexpr String& assign(const String& str );
    constexpr String& assign(const char* str);
    constexpr String& insert(size_t index, const String& str);
    constexpr String& erase(size_t index = 0, size_t count = npos);
    constexpr const char* c_str() const noexcept;
    String substr(size_t start = 0) const;  
    void swap(String& other);
};
#endif //STRING_HPP 