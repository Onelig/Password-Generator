#pragma once
#include "Language.hpp"

class English : public Language
{
public:
    virtual const char* password_size()           override { return "Password Size"; }
    virtual const char* include_symbs()           override { return "Include Symbols(~!`)"; }
    virtual const char* include_nums()            override { return "Include Numbers(0-9)"; }
    virtual const char* include_lower_letters()   override { return "Include Lowercase Letters(a-z)"; }
    virtual const char* include_upper_letters()   override { return "Include Uppercase Letters(A-Z)"; }
    virtual const char* include_similar_letters() override { return "Include Similar Letters"; }
    virtual const char* generate_password()       override { return "GENERATE PASSWORD"; }

    virtual const wchar_t* first_text()  override { return L"Password copied!"; }
    virtual const wchar_t* second_text() override { return L"The password has been successfully copied to your clipboard."; }
};
