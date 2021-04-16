//!
//!@file    xhelpers/sx_jsonstring.h
//!@author  Sholomov Dmitry
//!@date    30.11.2017
//!@brief   Helper class for json parsing and convertion 
//!

#ifndef SX_JSON_STRING_H
#define SX_JSON_STRING_H

#pragma once

#include <typeinfo>
#include <string>
#include <vector>
#include <sstream>
#include <ctime>
#include <iomanip>

#include <sx_string.h>
#include <sx_timestamp.h>

namespace sx {

//! Forward declaration for the json_cast helper class, used in json_string
template <class T> class json_cast;

//! @class json_string
//! @brief Class for operating with json strings - creating and parsing
class json_string : public std::string
{
public:
  // Constructors and destructors
  json_string() {}                                      //!< Default constructor for empty string
  json_string(const json_string& str);                  //!< Copy-constructor from json_string
  json_string(const char* str);                         //!< Constructor from const char*
  explicit json_string(const std::string& str);         //!< Constructor from std::string. explicit is used to 
                                                        //!  avoid ambuguity with json_string construction
  template <class T> 
  json_string(json_cast<T> jcast);                      //!< Constructor from any standart or user defined type.
                                                        //!  Standart type is a type, that could be serialized to ostream
  virtual ~json_string() {}                             //!< Destructor, may be overloaded

  json_string append(const std::string& s);             //!< Append json string s to the current json string "A" -> "A, s"
  json_string embrace();                                //!< Put json string into brackets "A" -> "{ A }"
  json_string debrace();                                //!< Remove brackets from json string  "{ A }" -> "A"

  std::string styledString();                           //!< Create pretty-visualizable string with line breaks

};


//! @class json_cast
//! Helper class for casting standart and user template data types to json string format and vice versa -
//! json parsing and convertion to data types. Standart type is a type, serializable to ostream
template <class T>
class json_cast
{
public:
  // Constructors and destructors
  json_cast(json_string s);                             //!< Constructor from json_string to be casted to template datatype T

  json_cast(                                            //!< Constructor from standart or custom data type
    std::string tag,                                      //!< @param [in] tag   - json data tag, e.g. "tag: value"
    const T& value                                        //!< @param [in] value - standart or custom data type converted to json string
    );

  json_cast(                                            //!< Constructor from standart or custom data type
    const T& value                                        //!< @param [in] value - standart or custom data type converted to json string
    );

  json_cast(                                            //!< Constructor from array of standart or custom data values, e.g. "tag: [val1, val2, val3]"
    std::string tag,                                      //!< @param [in] tag  - json data tag, e.g. "tag: value"
    const std::vector<T>& vals                            //!< @param [in] vals - array of standart or custom data type converted to json string
    );

  json_cast(                                            //!< Constructor from array of standart or custom data values without tag, e.g. "[val1, val2, val3]"
    const std::vector<T>& vals                            //!< @param [in] vals - array of standart or custom data type converted to json string
    );

  virtual ~json_cast() {}                               //!< Destructor, may be overloaded

  // Casting operators
  operator const T&();                                  //!< Parse json_string and converting it to the datatype T 
  operator json_string();                               //!< Generate json_string for the type T
  operator std::string();                               //!< Generate std::string for the type T

  // Explicit string convertion
  bool isStringType();                                  //!< Returns if datatype T is of a string type to be converted in quotes, e.g. tag: "value" 
  bool isFloatType();                                  //!< Returns if datatype T is of a float type to be converted with some precision
  std::string toString();                               //!< Generate std::string for the type T
  std::string toString(int float_precision);          //!< Function overload for floating point types
  const T& fromString(json_string str);                 //!< Parse json_string and converting it to the datatype T 

protected:
  std:: string tag;                                     //!< Json data tag stored for datatype T type
  T type;                                               //!< Value for the datatype T
  std::vector<T> vtypes;                                //!< Array of values for the datatype T, to be converted as "[val1, val2, val3]"
};


////////////////////////////////////////////////////////
//////  Class json_string implementation section

//! Constructor from json_string
inline json_string::json_string(const json_string& str)
{ 
  assign(str); 
}

//! Constructor from const char*
inline json_string::json_string(const char* str)
{ 
  assign(std::string(str)); 
}

//! Constructor from std::string. explicit is used to 
inline json_string::json_string(const std::string& str)
{ 
  assign(str); 
}

//! Constructor from any standart or user defined type.
template <class T>
inline json_string::json_string(json_cast<T> jcast)
{ 
  assign(jcast.toString());
}

//! Append json string s to the current json string "A" -> "A, s"
inline json_string json_string::append(const std::string& s_app)
{
  using namespace sx;
  bool bPutComma = true;
  if(empty())
    bPutComma = false;
  if(xstring(*this).erase_sym(" ").ends_with("["))
    bPutComma = false;
  if(xstring(*this).erase_sym(" ").ends_with("{"))
    bPutComma = false;
  *this = bPutComma ? xstring().format("%s, %s", this->c_str(), s_app.c_str()).c_str() : 
                      xstring().format("%s%s", this->c_str(), s_app.c_str()).c_str();
  return *this;
}

//!< Remove brackets from json string  "{ A }" -> "A"
inline json_string json_string::debrace()
{
  using namespace sx;
  *this = xstring(*this).erase_sym_left(" ").erase_sym_right(" ").c_str();
  if(length()>=2 && (*this)[0]=='{' && (*this)[length()-1]=='}')
  {
    std::string s(begin()+1, begin()+length()-2);
    *this = json_string(s);
  }
  return *this;
}

//! Put json string into brackets "A" -> "{ A }"
inline json_string json_string::embrace()
{
  using namespace sx;
  xstring s = json_string(*this).debrace();
  *this = xstring().format("{ %s }",s.c_str()).c_str();
  return *this;
}

std::string styledString();                           //!< Create pretty-visualizable string with line breaks

////////////////////////////////////////////////////////
//////  Class json_cast implementation section

//! Constructor from json_string to be casted to template datatype T
template <class T>
inline json_cast<T>::json_cast(json_string s) 
  : type() 
{ 
  fromString(s); 
}

//! Constructor from standart or custom data type
template <class T>
inline json_cast<T>::json_cast(std::string _tag, const T& _type)
  : tag(_tag)
  , type(_type)
{
}

//! Constructor from standart or custom data type
template <class T>
inline json_cast<T>::json_cast(const T& _type)
  : tag("")
  , type(_type)
{
}

//! Constructor from array of standart or custom data values, e.g. "tag: [val1, val2, val3]"
template <class T>
inline json_cast<T>::json_cast(std::string _tag, const std::vector<T>& _vtypes)
  : tag(_tag)
  , vtypes(_vtypes)
{
}

//! Constructor from array of standart or custom data values without tag, e.g. "[val1, val2, val3]"
template <class T>
inline json_cast<T>::json_cast(const std::vector<T>& _vtypes)
  : tag()
  , vtypes(_vtypes)
{
}

//!< Parse json_string and converting it to the datatype T 
template <class T>
inline json_cast<T>::operator const T&()
{ 
  return type;
}

//!< Generate json_string for the type T
template <class T>
inline json_cast<T>::operator json_string() 
{
  return json_string(toString());
}

//!< Generate std::string for the type T
template <class T>
inline json_cast<T>::operator std::string() 
{
  return toString();
}

//! Returns if datatype T is of a string type to be converted in quotes, e.g. tag: "value"
template <class T>
inline bool json_cast<T>::isStringType()
{
  if(typeid(T)==typeid(std::string))
    return true;
  if(typeid(T)==typeid(char*))
    return true;
  if(typeid(T)==typeid(const char*))
    return true;
  return false;
}

//! Returns if datatype T is of a float type to be converted with some precision
template <class T>
inline bool json_cast<T>::isFloatType()
{
  return std::is_floating_point<T>::value;
}

template<>
inline std::string json_cast<double>::toString(int float_precision)
{
  std::stringstream ss;
  if(!tag.empty())
    ss << '"' << tag << "\": ";
  ss << std::setprecision(float_precision) << type;
  return ss.str();
}

template<>
inline std::string json_cast<float>::toString(int float_precision)
{
  return json_cast<double>(this -> tag, this -> type).toString(float_precision);
}

//! Function template for every class specialization except float and double
template <class T>
inline std::string json_cast<T>::toString(int float_precision) 
{
  static_assert(
    std::is_floating_point<T>::value, 
    "This function exists only for floating point types"
  );
  return "";
}

//! Generate std::string for the type T
template <class T>
inline std::string json_cast<T>::toString() 
{
  using namespace std;
  std::stringstream ss;
  if(!tag.empty())
    ss << '"' << tag << "\": ";
  if(vtypes.size()>0)
  {
    ss << "[ ";
    typename std::vector<T>::iterator el;
    for(el=vtypes.begin(); el!=vtypes.end(); el++)
    {
      ss << *el;
      if(el!=vtypes.end()-1)
        ss << ", ";

    }
    ss << " ]";
    return ss.str();
  }

  if(isStringType())
    ss << '"' << type << '"';
  else
    ss << type;

  return ss.str(); 
}

//!< Parse json_string and converting it to the datatype T
template <class T>
inline const T& json_cast<T>::fromString(json_string str) 
{
  using namespace std;
  sx::xstring xstr(str);
  vector<string> vs = xstr.erase_sym("{ }").split(":");
  if(vs.size()<2)
    return type;
    
  tag = vs[0];
  stringstream ss(vs[1]);
  ss >> type;

  return type; 
}

};  // namespace sx

#endif