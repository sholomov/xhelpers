#pragma once
#ifndef __STR_EX_H
#define __STR_EX_H

#include <string>
#include <vector>
#include <algorithm>
#include <stdarg.h>

#include <xhelpers/sx_types.h>

#ifdef WIN32
	#pragma warning(push)
	#pragma warning(disable : 4566)
#else
    #include <cstring>
#endif

namespace sx {

template<class T>
inline void erase_sym_right(std::basic_string<T>& sString,const std::basic_string<T>& sSymbols)
{
	typedef typename std::basic_string<T>::iterator TBSI;
	TBSI it=sString.end();
	while(it!=sString.begin() && sSymbols.find(*--it)!=sSymbols.npos)
		sString.erase(it,it+1);
}

template<class T>
inline void erase_sym_left(std::basic_string<T>& sString,const std::basic_string<T>& sSymbols)
{
	typedef typename std::basic_string<T>::iterator TBSI;
	while(sString.begin()!=sString.end() && sSymbols.find(*sString.begin())!=sSymbols.npos)
		sString.erase(sString.begin(),sString.begin()+1);
}

template<class T>
inline void erase_sym(std::basic_string<T>& sString,const std::basic_string<T>& sSymbols)
{
	typedef std::basic_string<T> TBS;
	typedef typename std::basic_string<T>::iterator TBSI;
	TBS sTmp;
	for(TBSI it=sString.begin();it!=sString.end();it++)
		if(sSymbols.find(*it)==sSymbols.npos)
			sTmp+=*it;
	sString=sTmp;
}

template<class T>
inline void change_sym(std::basic_string<T>& sString, const std::basic_string<T>& sSymbolsFrom, const std::basic_string<T>& sSymbolsTo)
{
	typedef std::basic_string<T> TBS;
	typedef typename std::basic_string<T>::iterator TBSI;
	typedef typename std::basic_string<T>::size_type TBSS;
	for(TBSI it=sString.begin();it!=sString.end();it++)
	{
		TBSS pos=sSymbolsFrom.find(*it);
		if(pos!=sSymbolsFrom.npos)
			*it=*(sSymbolsTo.begin()+pos);
	}
}

template<class T>
inline void change_sym_right(std::basic_string<T>& sString, const std::basic_string<T>& sSymbolsFrom, const std::basic_string<T>& sSymbolsTo)
{
	typedef typename std::basic_string<T>::iterator TBSI;
	typedef typename std::basic_string<T>::size_type TBSS;
	TBSI it=sString.end(); TBSS pos;
	while((pos=sSymbolsFrom.find(*--it))!=sSymbolsFrom.npos)
		*it=*(sSymbolsTo.begin()+pos);
}

template<class T>
inline bool symbol_exist(std::basic_string<T>& sString, const std::basic_string<T>& sSymbols)
{
	typedef typename std::basic_string<T>::iterator TBSI;
	typedef typename std::basic_string<T>::size_type TBSS;
	for(TBSI it=sString.begin();it!=sString.end();it++)
		if(sSymbols.find(*it)!=sSymbols.npos)
			return true;
	return false;
}

const std::string sUpperEng("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
const std::string sLowerEng("abcdefghijklmnopqrstuvwxyz");
const std::string sUpperRus("АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ");
const std::string sLowerRus("абвгдеёжзийклмнопрстуфхцчшщъыьэюя");
const std::string sCyrNorm("АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдеёжзийклмнопрстуфхцчшщъыьэюя");
const std::string sCyrInEng("F,DULT`;PBQRKVYJGHCNEA[WXIO]SM'.Zf,dult`;pbqrkvyjghcnea[wxio]sm'.z");
const std::string sEngNorm("ABCDEFGHIGKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
const std::string sEngInCyr("ФИСВУАПРШОЛДЬТЩЗЙКЫЕГМЦЧНЯфисвуапршолдьтщзйкыегмцчня");
const std::string sDigits("0123456789");
const std::string sCyrConsonant("бвгджзклмнпрстфхцчшщБВГДЖЗКЛМНПРСТФХЦЧШЩ");
const std::string sLatConsonant("bcdfghjklmnpqrstvwxzBCDFGHJKLMNPQRSTVWXZ");
const std::string sSeparators(" .,;:-\'\"");

const std::wstring wsUpperEng(L"ABCDEFGHIGKLMNOPQRSTUVWXYZ");
const std::wstring wsLowerEng(L"abcdefghijklmnopqrstuvwxyz");
const std::wstring wsUpperRus(L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ");
const std::wstring wsLowerRus(L"абвгдеёжзийклмнопрстуфхцчшщъыьэюя");
const std::wstring wsCyrNorm(L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдеёжзийклмнопрстуфхцчшщъыьэюя");
const std::wstring wsCyrInEng(L"F,DULT`;PBQRKVYJGHCNEA[WXIO]SM'.Zf,dult`;pbqrkvyjghcnea[wxio]sm'.z");
const std::wstring wsEngNorm(L"ABCDEFGHIGKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
const std::wstring wsEngInCyr(L"ФИСВУАПРШОЛДЬТЩЗЙКЫЕГМЦЧНЯфисвуапршолдьтщзйкыегмцчня");
const std::wstring wsDigits(L"0123456789");
const std::wstring wsSeparators(L" .,;:-\'\"");

inline bool is_consonant(char c)
{
	if(sCyrConsonant.find(c)!=sCyrConsonant.npos)
		return true;
	if(sLatConsonant.find(c)!=sLatConsonant.npos)
		return true;
	return false;
}

inline bool is_digit(char c)
{
	if(sDigits.find(c)!=sDigits.npos)
		return true;
	return false;
}

inline bool is_separator(char c)
{
	if(sSeparators.find(c)!=sSeparators.npos)
		return true;
	return false;
}

inline bool is_letter(char c)
{
	if(sUpperEng.find(c)!=sUpperEng.npos)
		return true;
	if(sLowerEng.find(c)!=sLowerEng.npos)
		return true;
	if(sUpperRus.find(c)!=sUpperRus.npos)
		return true;
	if(sLowerRus.find(c)!=sLowerRus.npos)
		return true;
	return false;
}

inline void lower(std::string& str)
{
	sx::change_sym(str,sUpperEng,sLowerEng);
	sx::change_sym(str,sUpperRus,sLowerRus);
}

inline void upper(std::string& str)
{
	sx::change_sym(str,sLowerEng,sUpperEng);
	sx::change_sym(str,sLowerRus,sUpperRus);
}

template<class T>
inline bool begins_with(const std::basic_string<T>& sAll, const std::basic_string<T>& sBegin)
{
	return sAll.compare(0,sBegin.length(),sBegin)==0 ? true : false;
}

template<class T>
inline bool ends_with(const std::basic_string<T>& sAll, const std::basic_string<T>& sEnd)
{
	std::string srAll = sAll;
	std::string srEnd = sEnd;

	reverse(srAll.begin(),srAll.end());
	reverse(srEnd.begin(),srEnd.end());

	return srAll.compare(0,srEnd.length(),srEnd)==0 ? true : false;
}

inline bool begins_with(const char* pAll,const char* pBegin)
{
	return begins_with<char>( std::string(pAll), std::string(pBegin) );
}

inline bool consist_of(const char* pStr,const char* pSymb)
{
	return 	strspn(pStr,pSymb)==strlen(pStr);
}

template<class T>
inline int split(std::vector< std::basic_string<T> >& vsVec, 
                 const std::basic_string<T>& sStr, 
                 const std::basic_string<T>& sSep
                 )
{
    vsVec.clear();
	typedef typename std::basic_string<T>::const_iterator TBSI;
	TBSI it_token_beg=sStr.begin();
	TBSI it_token_end=it_token_beg;
	while(it_token_beg<sStr.end())
	{
		it_token_end=it_token_beg;
		while(it_token_end<sStr.end() && sSep.find(*it_token_end)==sSep.npos)
			it_token_end++;
		vsVec.push_back(std::string(it_token_beg,it_token_end));
	    it_token_beg = it_token_end;
        if (it_token_beg != sStr.end())
        {
            ++it_token_beg;
		while(it_token_beg<sStr.end() && sSep.find(*it_token_beg)!=sSep.npos)
			it_token_beg++;
        }
	}
	return (int) vsVec.size();
}

// split string with token positions information
template<class T>
inline int split_ex(std::vector< std::basic_string<T> >& vsVec,
                    const std::basic_string<T>& sStr, 
                    const std::basic_string<T>& sSep, 
                    std::vector< std::pair<int,int> >& vsPositions
                    )
{
  using namespace std;
	vsVec.clear();
	typedef typename basic_string<T>::const_iterator TBSI;
	TBSI it_token_beg=sStr.begin();
	TBSI it_token_end=it_token_beg;
	while(it_token_beg<sStr.end())
	{
		it_token_end=it_token_beg;
		while(it_token_end<sStr.end() && sSep.find(*it_token_end)==sSep.npos)
			it_token_end++;
		vsVec.push_back(string(it_token_beg,it_token_end));
		pair<int,int> ppos;
		ppos.first = it_token_beg-sStr.begin();
		ppos.second = it_token_end-sStr.begin();
		vsPositions.push_back(ppos);
		it_token_beg = it_token_end;
		if (it_token_beg != sStr.end())
		{
			++it_token_beg;
			while(it_token_beg<sStr.end() && sSep.find(*it_token_beg)!=sSep.npos)
				it_token_beg++;
		}
	}
	return (int) vsVec.size();
}

// join strings with separator
template<class T>
inline int join(std::basic_string<T> &vsStr,
  const std::vector<std::basic_string<T> > &sVec,
  const std::basic_string<T> &sSep)
{
  vsStr.clear();

  if (sVec.size() > 0)
  {
    vsStr = sVec[0];
    for (size_t i = 1; i < sVec.size(); ++i)
    {
      vsStr += sSep + sVec[i];
    }
  }

  return (int) vsStr.size();
}

template<class T>
inline void replace_first(std::basic_string<T>& sStr, const std::basic_string<T>& sFrom, const std::basic_string<T>& sTo)
{
	typename std::basic_string<T>::size_type pos=0;
	if((pos=sStr.find(sFrom))!=sStr.npos)
		sStr.replace(sStr.begin()+pos,sStr.begin()+pos+sFrom.length(),sTo.begin(),sTo.end());
}

template<class T>
inline void replace_all(std::basic_string<T>& sStr, const std::basic_string<T>& sFrom, const std::basic_string<T>& sTo)
{
	typename std::basic_string<T>::size_type pos=0;
	std::basic_string<T> sPrev=sStr;
	while((pos=sStr.find(sFrom))!=sStr.npos)
	{
		sStr.replace(sStr.begin()+pos,sStr.begin()+pos+sFrom.length(),sTo.begin(),sTo.end());
		if(sStr==sPrev)
			return;
		sPrev=sStr;
	}
}

template<class T>
inline void replace_from_end(std::basic_string<T>& sStr, const std::basic_string<T>& sFrom, const std::basic_string<T>& sTo)
{
	typename std::basic_string<T>::size_type pos=0;
	if((pos=sStr.find(sFrom))==sStr.length()-sFrom.length() && pos!=sStr.npos)
		sStr.replace(sStr.begin()+pos,sStr.begin()+pos+sFrom.length(),sTo.begin(),sTo.end());
}

// Replaces the sequences of chars from sDelims to the given sTo string
// Example: replace_delimeters("rabbit","bijk","--") "rabbit" -> "ra--t"
template<class T>
inline void replace_delimeters(std::basic_string<T>& sStr, const std::basic_string<T>& sDelims, const std::basic_string<T>& sTo)
{
  using namespace std;
	if(sStr.length()==0)
		return;
	STL_TYPENAME_EX(std::vector<basic_string<T> >,TVec);
	std::string sOut;
	TVec vecParts;
	split(vecParts,sStr,sDelims);
	if(sDelims.find(sStr[0])!=sDelims.npos)	// prefix
		sOut+=sTo;
	for(TVec_i it=vecParts.begin();it!=vecParts.end();it++)
	{
		sOut+=*it;
		if(it!=vecParts.end()-1)
			sOut+=sTo;
	}
	if(sDelims.find(sStr[sStr.length()-1])!=sDelims.npos)	// postfix
		sOut+=sTo;
	sStr=sOut;
}

template<class T>
inline std::basic_string<T> concat(std::vector<std::basic_string<T> > vs, std::basic_string<T> delim)
{
  using namespace std;
	vector<string>::iterator i;
	string s_out;
	for(i=vs.begin(); i!=vs.end(); i++)
	{
		if(i!=vs.begin())
			s_out += delim;
		s_out += *i;
	}
	return s_out;
}

template<class T>
inline std::basic_string<T> concat(std::vector<std::basic_string<T> > vs, const char* delim)
{
  using namespace std;
	vector<string>::iterator i;
	string s_out;
	for(i=vs.begin(); i!=vs.end(); i++)
	{
		if(i!=vs.begin())
			s_out += delim;
		s_out += *i;
	}
	return s_out;
}
}	// namespace sx

#ifdef WIN32
	#pragma warning(pop) 
#endif

#endif