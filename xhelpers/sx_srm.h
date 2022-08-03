//!
//!@file    xhelpers/sx_srm.h
//!@author  Sholomov Dmitry
//!@date    14.11.2001
//!@brief   Helper class for streams which extends std::stream classes
//!

#ifndef _SX_SRM
#define _SX_SRM

#ifdef WINDOWS
#pragma warning (disable:4786)
#endif

#include <fstream>
#include <vector>
#include <string>
#include <sstream>

#include <cstring>

#ifndef _NOEXCEPT
# define _NOEXCEPT
#endif

namespace sx
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Classes idelimstream, odelimstream, ifdelimstream, ofdelimstream 
// can read vector<string> and vector<vector<string>> structures from the stream

template <class Stream>
Stream& read_vec_string(Stream& is,std::vector<std::string>& vec,const char* delim="\t\n\r ",char delim_line='\n', bool bDelimSingle = false)
{  // последний параметр задаёт как рассматривать несколько разделителей подряд, false - как один, true - как несколько
	std::string str_line;
	size_t max_col=0;
	std::vector<std::string> _vec;
	if(getline(is,str_line,delim_line))	// read next line
	{
		char* strbuf=(char*)str_line.c_str();
		_vec.reserve(max_col);

        char* pCurWdBeg = NULL;  
        int istrspn = (int) strspn(strbuf,delim);
        if (!bDelimSingle)
           pCurWdBeg=strbuf+istrspn; //strspn(strbuf,delim);
        else
           pCurWdBeg = strbuf;

		char* pCurWdEnd=pCurWdBeg;
		do
		{	if(!(pCurWdBeg && *pCurWdBeg))
				break;
            
            istrspn = (int) strspn(pCurWdBeg,delim);
            if (!bDelimSingle)
			   pCurWdEnd=pCurWdBeg+strcspn(pCurWdBeg,delim);
            else
               pCurWdEnd = strpbrk(pCurWdBeg, delim);
            if ( !pCurWdEnd )
               _vec.push_back((std::string)pCurWdBeg);
			else 
               _vec.push_back(std::string(pCurWdBeg,pCurWdEnd)); 
			if(_vec.size()>max_col)
				max_col=_vec.size();
            if(pCurWdEnd && *pCurWdEnd)
			   pCurWdBeg=pCurWdEnd+1;
		} while(pCurWdBeg && pCurWdEnd && *pCurWdBeg && *pCurWdEnd);
		vec=_vec;
	}
	return is;
}

template <class Stream>
Stream& write_vec_string(Stream& os,std::vector<std::string>& vec,const char* delim="\t")
{
	for(std::vector<std::string>::iterator it=vec.begin();it!=vec.end();it++)
	{
		os << *it;
		if(it!=vec.begin()+vec.size()-1)
			os << delim;
	}
	return os;
}

class idelimstream : public std::istream
{
	std::string delim;
	char delim_line;
public:
	idelimstream(std::basic_streambuf<char, std::char_traits<char> > *sb=nullptr, const char* dl="\t\n\r ", char _delim_line='\n') : 
		std::istream(sb), delim(dl), delim_line(_delim_line)	{ ; } 
	virtual ~idelimstream() _NOEXCEPT {;}
	idelimstream& operator >>(std::vector<std::string>& vec)
	{
		return read_vec_string(*this,vec,delim.c_str(),delim_line);
	}
	idelimstream& operator >>(std::vector<std::vector<std::string> >& tbl)
	{
		std::vector<std::string> new_row;
		while(*this >> new_row)
			tbl.push_back(new_row);
		return *this;
	}
};

class odelimstream : public std::ostream
{
	std::string delim;
	std::string delim_line;
public:
	odelimstream(std::basic_streambuf<char, std::char_traits<char> > *sb=NULL, const char* dl="\t", const char* dl_line="\n") : 
		std::ostream(sb), delim(dl), delim_line(dl_line)	{ ; }
	virtual ~odelimstream() _NOEXCEPT { ; }
	odelimstream& operator <<(std::vector<std::string>& vec)
	{
		return write_vec_string(*this,vec,delim.c_str());
	}
	odelimstream& operator <<(std::vector<std::vector<std::string> >& tbl)
	{
		for(std::vector<std::vector<std::string> >::iterator it=tbl.begin();it!=tbl.end();it++)
			*this << *it << delim_line;
		return *this;
	}
};

// class which can read vector<string>& vec from the stream
class ifdelimstream : public std::ifstream
{
public:
  // construction/destruction
  ifdelimstream() : delim(""), delim_line('\n'), bDelimSingle(false) {}
  ifdelimstream(const char* filename, const char* dl="\t\n\r ", const char _delim_line='\n', bool _bDelimSingle=false) :
    std::ifstream(filename), delim(dl), delim_line(_delim_line), bDelimSingle(_bDelimSingle)	{}
  virtual ~ifdelimstream() _NOEXCEPT {;}
	
  /// reading vector data
  ifdelimstream& operator >>(std::vector<std::string>& vec)
	{
		return read_vec_string(*this,vec,delim.c_str(),delim_line, bDelimSingle);
	}
  /// reading table data
  ifdelimstream& operator >>(std::vector<std::vector<std::string> >& tbl)
	{
		std::vector<std::string> new_row;
		while(*this >> new_row)
			tbl.push_back(new_row);
		return *this;
	}

protected:
	std::string delim;
	char delim_line;
  bool bDelimSingle;     // параметр задающий как рассматривать несколько разделителей подряд, false - как один, true - как несколько

};

class ofdelimstream : public std::ofstream
{
	std::string delim;
	std::string delim_line;
public:
	ofdelimstream(const char* filename, const char* dl="\t", const char* dl_line="\n") : std::ofstream(filename), delim(dl), delim_line(dl_line) {}
	virtual ~ofdelimstream() _NOEXCEPT { ; }
	ofdelimstream& operator <<(std::vector<std::string>& vec)
	{
		return write_vec_string(*this,vec,delim.c_str());
	}
	ofdelimstream& operator <<(std::vector<std::vector<std::string> >& tbl)
	{
		for(std::vector<std::vector<std::string> >::iterator it=tbl.begin();it!=tbl.end();it++)
			*this << *it << delim_line;
		return *this;
	}
};

} // namespace sx;

#endif