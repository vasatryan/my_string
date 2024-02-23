#include "string.hpp"

std::ostream& operator<<(std::ostream& os, const String& str) {
    for(size_t i = 0; i < str.size(); ++i) {
        os << str[i];
    }
    return os;
}

std::istream& operator>>(std::istream& is, String& str) {
    str.clear();
    char ch;
    while (is.get(ch))
    {
        if(ch == '\n')
            break;
        str.push_back(ch);
    }
    return is;
}

void String::reallocation() {
    m_capacity *= 2;
    char* tmp = new char[m_capacity];

    if (isOnHeap) {
        // If the string is on the heap, copy its content and null-terminate
        strcpy(tmp, string.m_ptr);
        tmp[m_size] = '\0';  // Ensure null-termination
        delete[] string.m_ptr;
    } else {
        // If the string is on the stack, use std::copy to copy the content
        std::copy(string.onstack, string.onstack + m_size, tmp);
        tmp[m_size] = '\0';  // Ensure null-termination
    }
    isOnHeap = true;
    string.m_ptr = tmp;
}

void String::reallocation(int newCapacity) {
    m_capacity = newCapacity;
    char* tmp = new char[m_capacity];

    if (isOnHeap) {
        // If the string is on the heap, copy its content and null-terminate
        strcpy(tmp, string.m_ptr);
        tmp[m_size] = '\0';  // Ensure null-termination
        delete[] string.m_ptr;
    } else {
        // If the string is on the stack, use std::copy to copy the content
        std::copy(string.onstack, string.onstack + m_size, tmp);
        tmp[m_size] = '\0';  // Ensure null-termination
    }
    isOnHeap = true;
    string.m_ptr = tmp;
}


String::String() : m_capacity(16), m_size(0), isOnHeap(false) {

}

String::String(const String& src) : m_capacity(src.m_capacity), m_size(src.m_size), isOnHeap(src.isOnHeap) {    
    if(isOnHeap) {
        reallocation();
        strcpy(string.m_ptr, src.string.m_ptr);
        string.m_ptr[m_size] = '\0';
    } else {
        strcpy(string.onstack, src.string.onstack);
        string.onstack[m_size] = '\0'; 
    }
}

String::String(const char* src) {
    m_size = strlen(src);
    if (m_size < 16) {
        isOnHeap = false;
        strcpy(string.onstack, src);
        string.onstack[m_size] = '\0'; 
    } else {
        // isOnHeap = true;
        reallocation();
        strcpy(string.m_ptr, src);
        string.m_ptr[m_size] = '\0';
    }
}

String::String(const String& src, size_t pos, size_t len) {
    m_size = std::min(len, src.size() - pos);
    if (m_size < 16) {
        isOnHeap = false;
        strncpy(string.onstack, src.begin() + pos, m_size);
        string.onstack[m_size] = '\0'; 
    } else {
        // isOnHeap = true;
        reallocation();
        strncpy(string.m_ptr, src.begin() + pos, m_size);
        string.m_ptr[m_size] = '\0'; 
    }
}

String::String(size_t count, char sym) {
    m_size = count;
    if (m_size < 16) {
        isOnHeap = false;
        while (count)
        {
           push_back(sym);
        }
        string.onstack[m_size] = '\0';
    } else {
        // isOnHeap = true;
        reallocation();
        while (count)
        {
           push_back(sym);
        }
        string.m_ptr[m_size] = '\0';
    }
}

String::String(String&& src) noexcept {
    if(src.isOnHeap) {
        m_capacity = std::exchange(src.m_capacity, 0);
        m_size = std::exchange(src.m_size, 0);
        string.m_ptr = std::exchange(src.string.m_ptr, nullptr);
    } else {
        m_capacity = std::exchange(src.m_capacity, 0);
        m_size = std::exchange(src.m_size, 0);
        std::copy(src.string.onstack, src.string.onstack + m_size, string.onstack);
    }
}

String::~String() {
    if(isOnHeap) {
        delete[] string.m_ptr;
    }
}

String& String::operator=(const String& rhs){
    if(this != &rhs) {
        if(rhs.isOnHeap && isOnHeap) {
            m_capacity = rhs.m_capacity;
            m_size = rhs.m_size;
            delete[] string.m_ptr;
            string.m_ptr = new char[m_capacity];
            strcpy(string.m_ptr, rhs.string.m_ptr);
        } else if(rhs.isOnHeap && !isOnHeap) {
            // isOnHeap = true;
            m_capacity = rhs.m_capacity;
            m_size = rhs.m_size;
            reallocation(rhs.m_capacity);
            strcpy(string.m_ptr, rhs.string.m_ptr);
        } else if (!rhs.isOnHeap) {
            if(isOnHeap) {
                isOnHeap = false;
                delete[] string.m_ptr;
            }
            m_capacity = rhs.m_capacity;
            m_size = rhs.m_size;
            strcpy(string.onstack, rhs.string.onstack);
        }
    }
    return *this;
}



String& String::operator=(String&& rhs) noexcept{
    if(this != &rhs) {
        if(rhs.isOnHeap) {
            m_capacity = std::exchange(rhs.m_capacity, 0);
            m_size = std::exchange(rhs.m_size, 0);
            string.m_ptr = std::exchange(rhs.string.m_ptr, nullptr);
        } else {
            m_capacity = std::exchange(rhs.m_capacity, 0);
            m_size = std::exchange(rhs.m_size, 0);
            std::copy(rhs.string.onstack, rhs.string.onstack + m_size, string.onstack);
        }
    }
    return *this;    
}

String& String::operator=(const char* str) {
    if(str == nullptr) {
        clear();
        return *this;
    }
    m_size = strlen(str);
    m_capacity = m_size + 1;
    if(m_size < 16) {
        isOnHeap = false;
        strcpy(string.onstack, str);

    } else {
        isOnHeap = true;
        delete[] string.m_ptr;
        string.m_ptr = new char[m_capacity];
        strcpy(string.m_ptr, str);
    }
    return *this;
}

String& String::operator=(char sym) {
    if(isOnHeap) {
        delete[] string.m_ptr;
        m_capacity = 16;
        m_size = 1;
        isOnHeap = false;
        string.onstack[0] = sym;
    } else {
        m_size = 1;
        string.onstack[0] = sym;
    }
    return *this;   
}

constexpr char& String::operator[](size_t pos) {
    if(pos >= m_size) {
        throw std::out_of_range("Index out of range");
    } else {
        if(isOnHeap) {
            return string.m_ptr[pos];
        } else {
            return string.onstack[pos];
        }
    }
}

constexpr const char& String::operator[](size_t pos) const {
    if(pos >= m_size) {
        throw std::out_of_range("Index out of range");
    } else {
        if(isOnHeap) {
            return string.m_ptr[pos];
        } else {
            return string.onstack[pos];
        }
    }
}
constexpr String& String::operator+=(const String& str) {
    m_size += str.m_size;

    if (m_size > 16) {
        // isOnHeap = true;
        reallocation(m_size);

        if (str.isOnHeap) {
            for (size_t i = 0; i < str.m_size; ++i) {
                string.m_ptr[m_size - str.m_size + i] = str.string.m_ptr[i];
            }
        } else {
            for (size_t i = 0; i < str.m_size; ++i) {
                string.m_ptr[m_size - str.m_size + i] = str.string.onstack[i];
            }
        }
    } else {
        for (size_t i = 0; i < str.m_size; ++i) {
            string.onstack[m_size - str.m_size + i] = str.isOnHeap ? str.string.m_ptr[i] : str.string.onstack[i];
        }
    }

    return *this;
}
constexpr String& String::operator+=(const char* c_str) {
    size_t c_str_len = strlen(c_str);
    m_size += c_str_len;

    if (m_size > m_capacity) {
        // isOnHeap = true;
        reallocation(m_size);
        strcpy(string.m_ptr + m_size - c_str_len, c_str);
    } else {
        strcpy(string.onstack + m_size - c_str_len, c_str);
    }

    return *this;
}

constexpr String& String::operator+=(char sym) {
    ++m_size;  

    if (m_size >= 16) {
        // isOnHeap = true;
        reallocation(m_size); 
        string.m_ptr[m_size - 1] = sym;
    } else {
        string.onstack[m_size - 1] = sym;
    }


    return *this;
}

constexpr const char* String::begin() const noexcept {
    return isOnHeap ? string.m_ptr : string.onstack;
}

constexpr const char* String::end() const noexcept {
    return isOnHeap ? string.m_ptr + m_size : string.onstack + m_size;
}

constexpr size_t String::size() const noexcept {
    return m_size;
}

constexpr size_t String::length() const noexcept {
    return m_size;
}

constexpr size_t String::max_size() const noexcept {
    return isOnHeap ? (size_t) -1 : 16;
}   

constexpr size_t String::capacity() const noexcept {
    return m_capacity;
}

constexpr void String::resize(size_t count) {
    m_size = count;
    m_capacity = m_size;
    if(count > 16) {
        reallocation(m_size);
    } else {
        string.onstack[m_size] = '\0';
    }
}

constexpr void String::resize(size_t count, char sym) {
    if (count > 16) {
        reallocation(count);
        while (m_size < count) {
            string.m_ptr[m_size++] = sym;
        }
        string.m_ptr[m_size] = '\0';
    } else {
        while (m_size < count) {
            string.onstack[m_size++] = sym;
        }
        string.onstack[m_size] = '\0';
    }
    m_capacity = m_size;
}


constexpr void String::clear() noexcept {
    m_size = 0;
    isOnHeap = false;
}

constexpr bool String::empty() const noexcept {
    return !m_size;
}

constexpr char& String::at(size_t pos) {
    if(pos >= m_size) {
        throw std::out_of_range("Index out of range");
    } else {
        if(isOnHeap) {
            return string.m_ptr[pos];
        } else {
            return string.onstack[pos];
        }
    }
}

constexpr const char& String::at(size_t pos) const {
    if(pos >= m_size) {
        throw std::out_of_range("Index out of range");
    } else {
        if(isOnHeap) {
            return string.m_ptr[pos];
        } else {
            return string.onstack[pos];
        }
    }
}

constexpr const char& String::back() const {
    return isOnHeap ? string.m_ptr[m_size] : string.onstack[m_size];
}

constexpr const char& String::front() const {
    return isOnHeap ? string.m_ptr[0] : string.onstack[0];
}

constexpr String& String::append(size_t count, char sym) {
    m_size += count;
    if(isOnHeap) {
        if(m_capacity < m_size) {
            reallocation(m_size + 1);
        }
        while (count) {
            string.m_ptr[m_size - count] = sym;
            --count;
        }
        string.m_ptr[m_size] = '\0';
    } else {
        if(m_size >= m_capacity) {
            // isOnHeap = true;
            reallocation();
            while (count) {
            string.m_ptr[m_size - count] = sym;
            --count;
        }
        string.m_ptr[m_size] = '\0';
        } else {
            while (count) {
                string.onstack[m_size - count] = sym;
                --count;
            }
            string.onstack[m_size] = '\0';
        }
        
    }
    return *this;
}

constexpr String& String::append(const String& str) {
    m_size += str.m_size;
    if(isOnHeap) {
        if(m_size > m_capacity) {
            reallocation(m_size);
        }
        strcpy(string.m_ptr + m_size - str.m_size, str.begin());
    } else {
        if(m_size > 16) {
            // isOnHeap = true;
            reallocation(m_size);
            strcpy(string.m_ptr + m_size - str.m_size, str.begin());
        } else {
            strcpy(string.onstack + m_size - str.m_size, str.begin());
        }
    }
    return *this;
}

constexpr void String::push_back(char sym) {
    if(isOnHeap) {
        if(m_size >= m_capacity) {
            reallocation();
        }
        string.m_ptr[m_size++] = sym;
        string.m_ptr[m_size] = '\0';
    } else {
        if (m_size + 1 > m_capacity) {
            // isOnHeap = true;
            reallocation();
            string.m_ptr[m_size++] = sym;
            string.m_ptr[m_size] = '\0';
        } else {
            string.onstack[m_size++] = sym;
            string.onstack[m_size] = '\0';
        }
    }
    
}

constexpr void String::pop_back() {
    --m_size;
    if(isOnHeap) {
        if(m_size < 16) {
            string.m_ptr[m_size] = '\0';
            strcpy(string.onstack, string.m_ptr);
            delete[] string.m_ptr;  
        } else {
            string.m_ptr[m_size] = '\0';
        }
    } else {
        string.onstack[m_size] = '\0';
    }
    m_capacity = m_size;
}

constexpr String& String::assign(const String& str) {
    if(str.isOnHeap && isOnHeap) {
        m_capacity = str.m_capacity;
        m_size = str.m_size;
        delete[] string.m_ptr;
        string.m_ptr = new char[m_capacity];
        strcpy(string.m_ptr, str.string.m_ptr);
        
    } else if(str.isOnHeap && !isOnHeap) {
        // isOnHeap = true;
        m_capacity = str.m_capacity;
        m_size = str.m_size;
        reallocation(str.m_capacity);
        strcpy(string.m_ptr, str.string.m_ptr);
    } else if (!str.isOnHeap) {
        if(isOnHeap) {
            isOnHeap = false;
            delete[] string.m_ptr;
        }
        m_capacity = str.m_capacity;
        m_size = str.m_size;
        strcpy(string.onstack, str.string.onstack);
    }
    return *this;
}

constexpr String& String::assign(const char* str) {
    if(str == nullptr) {
        clear();
        return *this;
    }
    m_size = strlen(str);
    m_capacity = m_size + 1;
    if(m_size < 16) {
        isOnHeap = false;
        strcpy(string.onstack, str);

    } else {
        isOnHeap = true;
        delete[] string.m_ptr;
        string.m_ptr = new char[m_capacity];
        strcpy(string.m_ptr, str);
    }
    return *this;
}

constexpr String& String::insert(size_t index, const String& str) {
    if(index > m_size) {
        throw std::out_of_range("Invalid index");
    }

    size_t newSize = m_size + str.m_size;
    
    if(newSize > m_capacity) {
        // isOnHeap = true;
        reallocation();
    }
    if(isOnHeap) {
        for (size_t i = m_size; i > index; --i) {
            string.m_ptr[i + str.m_size - 1] = string.m_ptr[i - 1];
        }
        for (size_t i = 0; i < str.m_size; ++i) {
            string.m_ptr[index + i] = str[i];
        }
    } else {
        for (size_t i = m_size; i > index; --i) {
            string.onstack[i + str.m_size - 1] = string.onstack[i - 1];
        }
        for (size_t i = 0; i < str.m_size; ++i) {
            string.onstack[index + i] = str[i];
        }
    }
    m_size = newSize;
    return *this;
}

// esel
constexpr String& String::erase(size_t index, size_t count) {
    if (index >= m_size || count > m_size - index) {
        throw std::out_of_range("Invalid erase");
    }

    size_t newSize = (count == npos) ? m_size - index : m_size - count;

    if (isOnHeap && newSize <= 16) {
        for (size_t i = index; i < m_size - count; ++i) {
            string.m_ptr[i] = string.m_ptr[i + count];
        }
        string.m_ptr[newSize] = '\0';

        for (size_t i = 0; i < newSize; ++i) {
            string.onstack[i] = string.m_ptr[i];
        }
        string.onstack[newSize] = '\0';

            
        isOnHeap = false;
        delete[] string.m_ptr;
        string.m_ptr = nullptr;
    } else if (!isOnHeap) {
        for (size_t i = index; i < m_size - count; ++i) {
            string.onstack[i] = string.onstack[i + count];
        }
        string.onstack[newSize] = '\0';
    } else {
        for (size_t i = index; i < m_size - count; ++i) {
            string.m_ptr[i] = string.m_ptr[i + count];
        }
        string.m_ptr[newSize] = '\0';
    }

    m_size = newSize;
    return *this;
}



constexpr const char* String::c_str() const noexcept {
    return isOnHeap ? string.m_ptr : string.onstack;
}

String String::substr(size_t index) const { 
    if(index >= m_size) {
        throw std::out_of_range("invalid index for substr");
    }
   
    String new_str(*this, index, npos);
    return new_str;

}

void String::swap(String& other) {
    if (isOnHeap && other.isOnHeap) {
        std::swap(string.m_ptr, other.string.m_ptr);
    } else if (!isOnHeap && !other.isOnHeap) {
        size_t minSize = std::min(m_size, other.m_size);

        for (size_t i = 0; i < minSize; ++i) {
            std::swap(string.onstack[i], other.string.onstack[i]);
        }

        for (size_t i = minSize; i < m_size; ++i) {
            string.onstack[i] = '\0';
        }

        for (size_t i = minSize; i < other.m_size; ++i) {
            other.string.onstack[i] = '\0';
        }

        if (m_size > minSize) {
            other.m_size = m_size;
            other.m_capacity = m_size + 1;
        } else if (other.m_size > minSize) {
            m_size = other.m_size;
            m_capacity = other.m_size + 1;
        }
    } else {
        String temp(std::move(*this));
        *this = std::move(other);
        other = std::move(temp);
    }

    std::swap(m_capacity, other.m_capacity);
    std::swap(m_size, other.m_size);
    std::swap(isOnHeap, other.isOnHeap);
}


int main() {
    // Test default constructor
    String str1;

    // // Test copy constructor
    String str2 = "Hello, World!";
    String str3(str2);

    // // Test move constructor
    String str4 = std::move(str2);

    // // Test assignment operators
    String str5;
    str5 = "Assignment Test";

    // Test subscript operator
    char ch = str5[5];  // Accessing the character at index 5

    // Test concatenation operator
    String str6 = "Concatenation ";
    str6 += "Test";

    // Test size, length, and capacity
    size_t size = str6.size();
    size_t length = str6.length();
    size_t capacity = str6.capacity();

    // // Test resizing
    str6.resize(20, '*');

    // Test clearing
    str6.clear();

    // Test empty
    bool isEmpty = str6.empty();

    // Test appending
    str6.append(5, 'A'); 

    // Test inserting
    str6.insert(2, "Inserted");                 
    std::cout << str6<< "\n";                   

    // Test erasing
    str6.erase(1, 3);
    std::cout << str6<< "\n";                                  

    // Test substr
    String substr = str6.substr(1);

    // Test swap
    String str7 = "Swap Test";
    str6.swap(str7);
    std::cout << str6 << "\n \n \n ";                

    // Test c_str
    const char* cString = str6.c_str();

    // // Output results
    std::cout << "str1: " << str1 << std::endl;
    std::cout << "str3: " << str3 << std::endl;
    std::cout << "str4: " << str4 << std::endl;
    std::cout << "str5: " << str5 << std::endl;
    std::cout << "ch: " << ch << std::endl;
    std::cout << "str6: " << str6 << std::endl;
    std::cout << "size: " << size << std::endl;
    std::cout << "length: " << length << std::endl;
    std::cout << "capacity: " << capacity << std::endl;
    std::cout << "isEmpty: " << (isEmpty ? "true" : "false") << std::endl;
    std::cout << "substr: " << substr << std::endl;
    std::cout << "str7: " << str7 << std::endl;
    std::cout << "cString: " << cString << std::endl;

    return 0;
}