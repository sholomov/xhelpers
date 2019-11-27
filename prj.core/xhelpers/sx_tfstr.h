//!
//!@file    xhelpers/sx_tfstr.h
//!@author  Sholomov Dmitry
//!@brief   Helper class for the string with extended functionality
//!

#ifndef SX_TFSTR_H
#define SX_TFSTR_H

#include <ar10/sx_str.h>

#include <string>
#include <vector>

namespace sx {

class tf_string : public std::string
{
public:
    tf_string(const char* p="") { std::string::operator=(p); }
    tf_string(char* p) { std::string::operator=(p); }
    tf_string(std::string s) { std::string::operator=(s); }
    virtual ~tf_string() { ; }
    tf_string& replace(const char* from, const char* to)

    {
        sx::replace_all(*this,std::string(from),std::string(to));
        return *this;
    }

    tf_string& lreplace(const char* from, const char* to)
    {
        using namespace std;
        sx::replace_first(*this,std::string(from),std::string(to));
        return *this;
    }
    
    tf_string& rreplace(const char* from, const char* to)
    {   
        sx::replace_from_end(*this,std::string(from),std::string(to));
        return *this;
    }

    tf_string& erase_sym(const char* symbols)
    {
        sx::erase_sym(*this,std::string(symbols));
        return *this;
    }

    tf_string& erase_sym_left(const char* symbols)
    {
        sx::erase_sym_left(*this,std::string(symbols));
        return *this;
    }

    tf_string& erase_sym_right(const char* symbols)
    {
        sx::erase_sym_right(*this,std::string(symbols));
        return *this;
    }

    tf_string& change_sym(const char* from, const char* to)
    {
        sx::change_sym(*this,std::string(from),std::string(to));
        return *this;
    }

    tf_string& change_sym_right(const char* from, const char* to)
    {
        sx::change_sym_right(*this,std::string(from),std::string(to));
        return *this;
    }

    bool symbol_exist(const char* symbols)
    {
        return sx::symbol_exist(*this,std::string(symbols));
    }

    bool is_consonant(char c)
    {
        return sx::is_consonant(c);
    }

    inline tf_string& lower()
    {
        sx::lower(*this);
        return *this;
    }

    inline tf_string& upper()
    {
        sx::upper(*this);
        return *this;
    }
    
    bool begins_with(const char* prefix)
    {
        return sx::begins_with(*this,std::string(prefix));
    }

    bool ends_with(const char* postfix)
    {
		    return sx::ends_with(*this,std::string(postfix));
	  }

    bool consist_of(const char* symbols)
    {
        return sx::consist_of(this->c_str(),symbols);
    }

    int split(std::vector<std::string>& vsVec,const char* delimeters)
    {
        return sx::split(vsVec,*this,std::string(delimeters));
    }

    std::vector<std::string> split(const char* delimeters)
    {
        using namespace std;
        vector<string> vsVec;
        sx::split(vsVec,*this,string(delimeters));
        return vsVec;
    }

    // Joins a vector of strings sVec into vsStr using *this as delimiter
    // similar to python str join method. Also has convinient alias.
    // example code
    //
    // auto res = tf_string(", ").join(vector<string>{"abc", "bca", "aaa"})
    // tf_string(", ").join(res, vector<string>{"abc", "bca", "aaa"})
    //
    // in both cases, res would have value "abc, bca, aaa";
    int join(std::string &vsStr, const std::vector<std::string> &sVec)
    {
      return sx::join(vsStr, sVec, *this);
    }

    std::string join(const std::vector<std::string> &sVec)
    {
      std::string vsStr;
      sx::join(vsStr, sVec, *this);
      return vsStr;
    }

    // Replaces the sequences of chars from sDelims to the given sTo string
    // Example: replace_delimeters("rabbit","bijk","--") "rabbit" -> "ra--t"
    tf_string& replace_delimeters(const char* delimeters=" .,\"\':;", const char* replace_by=" ")
    {
        sx::replace_delimeters(*this,std::string(delimeters),std::string(replace_by));
        return *this;
    }

    // Returns true if string matches template $-letter #-digit *-any symbol
    bool match_template(const char* _templ)
    {
        using namespace std;
        tf_string templ(_templ);
        if(templ.length()!=this->length())
            return false;
        for(tf_string::iterator i = templ.begin(); i!=templ.end(); i++)
        {
            char& c = (*this)[i-templ.begin()];
            if(*i==c)
                continue;
            switch(*i)
            {
            case '#':
                if(is_digit(c))
                    continue;
                return false;
            case '$':
                if(is_letter(c))
                    continue;
                return false;
            case '*':
                continue;
            default:
                return false;
            }
        }
        
        return true;
    }
  
    // sprintf parameters
    tf_string&  format(const char* _templ, ... )
    {
        char buff[4096]; buff[0]=0;
        va_list list;
        va_start(list, _templ );
        vsprintf(buff, _templ, list );
        va_end( list );
        (*this) = buff;
        return *this;
    }

};

};

#endif