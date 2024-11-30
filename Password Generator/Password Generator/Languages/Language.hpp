#pragma once
#include <string>

class Language
{
public:
    virtual const char* password_size() = 0;
    virtual const char* include_symbs() = 0;
    virtual const char* include_nums() = 0;
    virtual const char* include_lower_letters() = 0;
    virtual const char* include_upper_letters() = 0;
    virtual const char* include_similar_letters() = 0;
    virtual const char* generate_password() = 0;

    virtual const wchar_t* first_text() = 0;
    virtual const wchar_t* second_text() = 0;
};
