#pragma once
#include "Language.hpp"

class Finnish : public Language
{
public:
    virtual const char* password_size()           override { return "Salasanan koko"; }
    virtual const char* include_symbs()           override { return "Sisällytä symbolit(~!`)"; }
    virtual const char* include_nums()            override { return "Sisällytä numerot(0-9)"; }
    virtual const char* include_lower_letters()   override { return "Sisällytä pienet kirjaimet(a-z)"; }
    virtual const char* include_upper_letters()   override { return "Sisällytä isot kirjaimet (A-Z)"; }
    virtual const char* include_similar_letters() override { return "Sisällytä vastaavat kirjaimet"; }
    virtual const char* generate_password()       override { return "LUO SALASANA"; }

    virtual const wchar_t* first_text()  override { return L"Salasana kopioitu!"; }
    virtual const wchar_t* second_text() override { return L"Salasana on kopioitu leikepöydälle."; }
};
