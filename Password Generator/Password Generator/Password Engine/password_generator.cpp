#include "password_generator.hpp"
#include <random>
#define SumElements ()

namespace
{
	std::random_device random_device;
	std::mt19937 mt19937(random_device());
	std::uniform_int_distribution<int> UID;
}

void password_generator::SetIncludeSimilarLetters(const bool& isAllow)
{
    include_similar_letters_ = isAllow;

    if (!include_similar_letters_) // false
    { 
        int temp_max_size = NULL;

        if (include_symb_)
            temp_max_size += symbols_.size();
        if (include_numbers_)
            temp_max_size += digits_.size();
        if (include_lower_letters_)
            temp_max_size += lowercase_.size();
        if (include_upper_letters_)
            temp_max_size += uppercase_.size();

        max_pass_size_ = temp_max_size;
    }
}

void password_generator::update_show_password_p()
{
    if (include_similar_letters_)
    {
        int temp_max_size = NULL;

        if (include_symb_)
            temp_max_size += symbols_.size();
        if (include_numbers_)
            temp_max_size += digits_.size();
        if (include_lower_letters_)
            temp_max_size += lowercase_.size();
        if (include_upper_letters_)
            temp_max_size += uppercase_.size();

        temp_max_size == NULL ? max_pass_size_ = NULL : max_pass_size_ = 200;
    }
}


std::string password_generator::CreatePassword()
{
	std::string all_avaible_chars;
	if (include_symb_)
		all_avaible_chars += symbols_;
	if (include_numbers_)
		all_avaible_chars += digits_;
	if (include_lower_letters_)
		all_avaible_chars += lowercase_;
	if (include_upper_letters_)
		all_avaible_chars += uppercase_;

	std::string ret_str;
    if (all_avaible_chars.size() != 0)
    {

        if (include_similar_letters_)
        {
            for (size_t i = 0; i < password_size_; ++i)
            {
                ret_str += all_avaible_chars[UID(mt19937) % all_avaible_chars.size()];
            }
        }
        else
        {
            for (size_t i = 0; i < password_size_; ++i)
            {
                size_t element = UID(mt19937) % all_avaible_chars.size();
                ret_str += all_avaible_chars[element];
                all_avaible_chars.erase(all_avaible_chars.begin() + element);
            }
        }
    }
	return ret_str;
}
