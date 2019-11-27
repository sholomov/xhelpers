//!
//!@file    xhelpers/sx_findfile.h
//!@author  Sholomov Dmitry
//!@author  Postnikov Vassili
//!@brief   Helper class for iterative file access by directory mask. Analog for XFindFile-class in CForms project
//!@note    Implementation is partly based on arcore library, so arcore.lib linking is required
//!

#ifndef SX_FINDFILE_H
#define SX_FINDFILE_H

#include <stdio.h>

#include <ar10/filesystem.h>
#include <ar10/sx_path.h>

#ifdef WIN32
#include <io.h>
#else
#include <glob.h>
#include <sys/stat.h>
#endif

namespace sx {

//! @class std::xpath xhelpers/sx_findfile.h
//! @brief Класс-хэлпер для итеративного перебора файлов по маске
class xfindfile
{
public:

  xfindfile(void);                                      //!< Constructors and destructors
  virtual ~xfindfile(void);

  xfindfile(const char* mask);
  xfindfile(const xstring& mask);

  bool init(void);                                      //!< Умолчательная инициализация класса
  bool done(void);                                      //!< Деинициализация класса

  bool restart(const char* mask);                       //!< Инициализация нового перебора файлов по маске
  bool stop();                                          //!< Останов текущего итеративного перебора

  xfindfile& operator ++ (void);                        //!< Очередная итерация переход к следующему файлу
  operator const char* (void);                          //!< Полный путь к текущему в рамках перебора файлу
  operator std::string (void);                          //!< Полный путь к текущему в рамках перебора файлу
  const char* safeStr(void);                            //!< Полный путь к текущему файлу, при отсутствии - ""

  const char* mask();                                   //!< Текущая файловая маска перебора
  const char* filePath();                               //!< Полный путь к текущему в рамках перебора файлу
  const char* fileName();                               //!< Имя текущего файла в рамках перебора

  bool isArchive(void);                                 //!< Флажок: архивный файл
  bool isHidden(void);                                  //!< Флажок: скрытый файл
  bool isNormal(void);                                  //!< Флажок: обычный файл
  bool isReadOnly(void);                                //!< Флажок: файл только для чтения
  bool isSubdir(void);                                  //!< Флажок: поддиректория
  bool isSystem(void);                                  //!< Флажок: системный файл
  
  time_t getFileCreationTime(void);                     //!< Время создания файла
  time_t getFileAccessTime(void);                       //!< Время последнего доступа к файлу
  time_t getFileWriteTime(void);                        //!< Время последнего изменения файла
  size_t getFileSize(void);                             //!< Размер файла

protected:

#ifdef WIN32
  _finddata_t fileinfo;
  intptr_t  hFind;
#else
  glob_t gv;
  size_t gv_off;
#endif

  xpath xpMask;
  xpath xpFilePath;
  bool  can_reply;

#ifndef WIN32
  struct stat st;
  int stat();
#endif

};

#ifdef WIN32
  #define XFF_INITFIELDS hFind(0), xpMask(""), xpFilePath(""), can_reply(false)
#else
  #define XFF_INITFIELDS gv_off(0), xpMask(""), xpFilePath(""), can_reply(false)
#endif

inline xfindfile::xfindfile(void) : XFF_INITFIELDS
{ 
  init();
}

inline xfindfile::xfindfile(const char* mask) : XFF_INITFIELDS
{ 
  init();
  restart(mask);
}

inline xfindfile::xfindfile(const xstring& mask) : XFF_INITFIELDS
{ 
  init();
  restart(mask.c_str());
}

inline xfindfile::~xfindfile(void)
{
  done();
}

// Умолчательная инициализация класса
inline bool xfindfile::init(void)
{ 
#ifdef WIN32
  hFind = -1;
#else
  gv.gl_pathc = 0;
  gv.gl_pathv = 0;
  gv_off = 0;
#endif
  can_reply = false;
  return true;
}

// Деинициализация класса
inline bool xfindfile::done(void)
{
  return stop();
}

// Инициализация нового перебора файлов по маске
inline bool xfindfile::restart(const char* mask)
{  
  if(mask)
    xpMask = mask;

#ifdef WIN32
  if(hFind != -1)
    _findclose(hFind);
  hFind = _findfirst( xpMask.c_str(), &fileinfo );
  can_reply = (hFind != -1);
#else
  stop();
  can_reply = glob(xpMask.c_str(), 0, 0, &gv) == 0;
  gv_off = 0;
#endif
  return can_reply;
}

// Останов текущего итеративного перебора
inline bool xfindfile::stop()
{
#ifdef WIN32
  if (hFind != -1)
      _findclose(hFind);
  hFind = -1;
#else
  for (size_t i = 0; i < gv.gl_pathc; ++i)
    free(gv.gl_pathv[i]);
  free(gv.gl_pathv);
  init();
#endif
  return true;
}

inline xfindfile& xfindfile::operator ++ (void) // prefix form:  use '++xff' to get next file
{ 
#ifdef WIN32
  can_reply = (_findnext(hFind, &fileinfo)==0);
#else
  ++gv_off; can_reply = gv_off < gv.gl_pathc;
#endif
  return *this;
}

// Полный путь к текущему в рамках перебора файлу
inline xfindfile::operator const char* (void)
{ 
  return can_reply ? filePath() : NULL;
}

inline const char* xfindfile::safeStr(void) 
{
  return can_reply ? filePath() : "";
}

// Полный путь к текущему в рамках перебора файлу
inline xfindfile::operator std::string (void)
{ 
  return std::string(safeStr());
}

// Текущая файловая маска перебора
inline const char* xfindfile::mask()
{  
  return xpMask.c_str(); 
}


// Имя текущего файла в рамках перебора
inline const char* xfindfile::fileName()
{  
#ifdef WIN32
  return can_reply ? (const char*)fileinfo.name : NULL;
#else
  if (!can_reply) 
    return NULL;
  char *ch = strrchr(gv.gl_pathv[gv_off], '/');
  return ch ? ch + 1 : (char *)gv.gl_pathv[gv_off];
#endif
}

// Полный путь к текущему в рамках перебора файлу
inline const char* xfindfile::filePath()
{ 
  xpFilePath = can_reply ? xpath(xpMask).cutPath().checkSlash()+=xpath(fileName()) : "";
  return can_reply ? xpFilePath.c_str() : 0;
}

//!< Флажок: архивный файл
inline bool xfindfile::isArchive(void)
{
#ifdef WIN32
  return fileinfo.attrib & _A_ARCH ? true : false;
#endif
  return false;
}
  
//!< Флажок: скрытый файл
inline bool xfindfile::isHidden(void)
{
#ifdef WIN32
  return fileinfo.attrib & _A_HIDDEN ? true : false;
#endif
  return false;
}
  
//!< Флажок: обычный файл
inline bool xfindfile::isNormal(void)
{
#ifdef WIN32
  return fileinfo.attrib & _A_NORMAL ? true : false;
#else
  return !stat() ? S_ISREG(st.st_mode) && (st.st_mode & (S_IRUSR | S_IWUSR)) : false;
#endif
}

//!< Флажок: файл только для чтения
inline bool xfindfile::isReadOnly(void)
{
#ifdef WIN32
  return fileinfo.attrib & _A_RDONLY ? true : false;
#else
  return !stat() ? S_ISREG(st.st_mode) && (st.st_mode & !S_IWUSR) : false;
#endif
}

//!< Флажок: поддиректория
inline bool xfindfile::isSubdir(void)
{
#ifdef WIN32
  return fileinfo.attrib & _A_SUBDIR ? true : false;
#else
  return !stat() ? S_ISDIR(st.st_mode) : false;
#endif
}

//!< Флажок: системный файл
inline bool xfindfile::isSystem(void)
{
#ifdef WIN32
  return fileinfo.attrib & _A_SYSTEM ? true : false;
#endif
  return false;
}

//!< Время создания файла
inline time_t xfindfile::getFileCreationTime(void)
{
#ifdef WIN32
  return fileinfo.time_create;
#else
  return !stat() ? st.st_ctime : (time_t)-1;
#endif
}

//!< Время последнего доступа к файлу
inline time_t xfindfile::getFileAccessTime(void)
{
#ifdef WIN32
  return fileinfo.time_access;
#else
  return !stat() ? st.st_atime : (time_t)-1;
#endif
}

//!< Время последнего изменения файла
inline time_t xfindfile::getFileWriteTime(void)
{
#ifdef WIN32
  return fileinfo.time_write;
#else
  return !stat() ? st.st_mtime : (time_t)-1;
#endif
}

//!< Размер файла
inline size_t xfindfile::getFileSize(void)
{
#ifdef WIN32
  return fileinfo.size;
#else
  return !stat() ? st.st_size : (off_t)-1;
#endif
}

#ifndef WIN32

inline int xfindfile::stat()
{
  if (!can_reply) 
    return -1;
  return ::stat(gv.gl_pathv[gv_off], &st);
}

#endif

}; // namespace sx

#endif // SX_PATH_H
