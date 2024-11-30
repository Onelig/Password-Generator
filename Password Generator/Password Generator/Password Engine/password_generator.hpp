#pragma once
#include <vector>
#include <string>

class password_generator
{
private:
    size_t password_size_ = 32;
    unsigned int max_pass_size_ = 200;

    bool include_symb_ = true;
    bool include_numbers_ = true;
    bool include_lower_letters_ = true;
    bool include_upper_letters_ = true;
    bool include_similar_letters_ = true;

	const std::string symbols_ = " ~!@#$%^&*()-=+[{]}\\;:'""/?.>,<";
	const std::string digits_ = "0123456789";
	const std::string lowercase_ = "abcdefghijklmnopqrstuvwxyz";
	const std::string uppercase_ = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

public:
    void SetActualSize(int& size) { password_size_ = size; }
    void SetIncludeSymb(const bool& isAllow) { include_symb_ = isAllow; }
    void SetIncludeNumbers(const bool& isAllow) { include_numbers_ = isAllow; }
    void SetIncludeLowerLetters(const bool& isAllow) { include_lower_letters_ = isAllow; }
    void SetIncludeUpperLetters(const bool& isAllow) { include_upper_letters_ = isAllow; }
    void SetIncludeSimilarLetters(const bool& isAllow);
    void update_show_password_p();

    size_t GetMaxSize() { return max_pass_size_; }
	std::string CreatePassword();
};

