//!
//!@file    xhelpers/sx_path.h
//!@author  Sholomov Dmitry
//!@author  Postnikov Vassili
//!@brief   Helper class for filepath operations. Analog for XPath-class in CForms project
//!@note    Implementation is partly based on arcore library, so arcore.lib linking is required
//!

#ifndef SX_PATH_H
#define SX_PATH_H

#include <stdlib.h>

#include <ar10/filesystem.h>
#include <ar10/sx_string.h>
#include <ar10/sx_findfile.h>

namespace sx {

  //! @class std::xpath xhelpers/sx_path.h
  //! @brief Класс-хэлпер для операций с файловыми путями
  class xpath : public xstring 
  {
  public:

    xpath(void) {};                                       //!< Constructors and destructors
    virtual ~xpath(void) {};

    xpath(const char* path ) : xstring(path) {};
    xpath(const char* drv, const char* dir, const char* name, const char* ext);
    xpath(const xstring& path ) : xstring(path) {};
    xpath(const std::string& path ) : xstring(path) {};

    xpath& operator = (const char * path );               //!< Операторы присваивания
    xpath& operator = (const xstring& path );
    xpath& operator = (const std::string& path );

    xpath& getModuleFileName();                           //!< Возвращает полный путь к исполняемому модулю (в конце - '/')
    xpath& getModuleDirPath();                            //!< Возвращает полный путь к исполняемому модулю (в конце - '/')
    xpath& getCurrentDirectory();                         //!< Возвращает полный путь к текущей директории (в конце - '/')
    xpath& getTempPath();                                 //!< Возвращает полный путь к директории для временных файлов (в конце - '/')
    xpath& getFullPathName();                             //!< Преобразует данный xpath к полному пути

    xpath& canonize(bool bWindows = false);               //!< Нормализация разделителей "\"->"/" и прочее

    xpath& checkSlash();                                  //!< Проверяем и добавляем слэш в конце если нет
    xpath& stripSlash();                                  //!< Проверяем и убираем слэш в конце если есть

    xpath& cutPath();                                     //!< Оставляем только путь (без имени и расширения, в конце - '/')
    xpath& cutName();                                     //!< Оставляем только имя (без расширения)
    xpath& cutNameEx();                                   //!< Оставляем только имя (с расширением)
    xpath& cutExt();                                      //!< Оставляем только расширение
    xpath& cutDrive();                                    //!< Оставляем только диск (в конце - ':') Для unix - пустое значение

    xpath& setPath(const char* path);                     //!< Заменяем путь (без имени и расширения, в конце - '/')
    xpath& setName(const char* name);                     //!< Заменяем или добавляем новое имя и расширение
    xpath& setNameEx(const char* name);                   //!< Заменяем имя (с расширением)
    xpath& setExt(const char* ext);                       //!< Заменяем или добавляем новое расширение
    xpath& setDrive(const char* drive);                   //!< Заменяем название диска

    xpath& eraseDrive();                                  //!< Удаляем название диска

    void splitPath(xpath& drv, xpath& dir, xpath& name, xpath& ext);                          //!< Разбиение xpath на фрагменты пути
    xpath& makePath(const xpath& drv, const xpath& dir, const xpath& name, const xpath& ext); //!< Формирование xpath из фрагментов пути
    xpath& makePath(const char* drv, const char* dir, const char* name, const char* ext);     //!< Формирование xpath из фрагментов пути

    friend bool equal(const xpath& xp1, const xpath& xp2);

    //!< перегрузка операторов == и != для сравнения без учета регистра
    bool operator == ( const char* str ) const { return equal(*this, str); }
    bool operator == ( char* str )       const { return equal(*this, str); }
    bool operator != ( const char* str ) const { return !equal(*this, str); }
    bool operator != ( char* str )       const { return !equal(*this, str); }

    //!< перегрузка операторов += 
    xpath& operator += (const char * sub);                            //!< Строковая конкатенация
    xpath& operator += (const xpath& sub);                            //!< Строковая конкатенация

                                                                      // перегрузка операторов -, -=, &, &=
    xpath& operator &= (const xpath& sub);                            //!< Общее начало путей a/b/c & a/b/d = a/b
    xpath& operator -= (const xpath& sub);                            //!< Разность путей a/b/c - a/b = d
    xpath& concat(const xpath& sub);                                  //!< Конкатенация путей a/b/c + d/e = a/b/c/d/e

    friend xpath operator & (const xpath& path, const xpath& sub);    //!< Общее начало путей a/b/c & a/b/d = a/b
    friend xpath operator - (const xpath& path, const xpath& sub);    //!< Разность путей a/b/c - a/b = d
    friend xpath concat(const xpath& path, const xpath& sub);         //!< Конкатенация путей a/b/c + d/e = a/b/c/d/e (добавляет slash)

    bool isAbsolutePath() const;                                      //!< Абсолютность пути. true если путь полный
    bool isRelativePath() const;                                      //!< Относительность пути. true если путь относительный
    bool isNetworkPath() const;                                       //!< Сетевой путь. true если путь сетевой

    bool isArchive(void);                                             //!< Флажок: архивный файл
    bool isHidden(void);                                              //!< Флажок: скрытый файл
    bool isNormal(void);                                              //!< Флажок: обычный файл
    bool isReadOnly(void);                                            //!< Флажок: файл только для чтения
    bool isSubdir(void);                                              //!< Флажок: поддиректория
    bool isSystem(void);                                              //!< Флажок: системный файл

    time_t getFileCreationTime(void);                                 //!< Время создания файла
    time_t getFileAccessTime(void);                                   //!< Время последнего доступа к файлу
    time_t getFileWriteTime(void);                                    //!< Время последнего изменения файла
    size_t getFileSize(void);                                         //!< Размер файла

    bool ensureFolder();                                              //!< Обеспечивает существование папки по заданному пути
    bool copyTo(const char* file);                                    //!< Копирует файл по заданному пути
    bool moveTo(const char* file);                                    //!< Передвигает файл по заданному пути
    bool remove();                                                    //!< Удаление файла по текущему пути
    bool exists();                                                    //!< Проверка существования файла/директории по текущему пути

                                                                      // исторические, возможно малоиспользуемые функции класса XPath
    xpath& getFullPathName(const char* szHomeDir);                    //!< Полный путь относительно директории <szHomeDirF>
    xpath& dotPath();                                                 //!< Замена пути на последовательность вида ../../../
    xpath& makeRelativePath(const char* szHomeDir);                   //!< Путь относительно <szHomeDir>
    xpath& makeRelativePath();	                                      //!< Путь относительно текущей рабочей директории

  };

  inline xpath& xpath::operator = (const char * path )
  { 
    *(xstring*)this = path;
    return *this;
  }

  inline xpath& xpath::operator = (const xstring& path )
  { 
    *(xstring*)this = path;
    return *this;
  }

  inline xpath& xpath::operator = (const std::string& path )
  { 
    *(std::string*)this = path;
    return *this;
  }

  inline xpath::xpath(const char* drv, const char* dir, const char* name, const char* ext)
  {
    makePath(drv, dir, name, ext);
  }

  // Возвращает полный путь кисполняемому модулю (в конце - '/')
  // Todo: reimplement to return module filename, not directory
  inline xpath& xpath::getModuleFileName()
  {
    *this = ar::Filesystem::moduleDirPath();
    return *this;
  }

  // Возвращает полный путь кисполняемому модулю (в конце - '/')
  inline xpath& xpath::getModuleDirPath()                            
  {
    *this = ar::Filesystem::moduleDirPath();
    return *this;
  }

#ifndef _MAX_PATH
#define _MAX_PATH   260 // max. length of full pathname
#endif

  // Возвращает полный путь к текущей директории
  inline xpath& xpath::getCurrentDirectory()
  {
    *this = ar::Filesystem::currentDirectory();
    canonize();

    return *this;         
  }

  inline xpath& xpath::getTempPath(void)
  {
    *this = ar::Filesystem::tempDirectory();
    canonize();

    return *this;         
  }

  // Нормализация разделителей "\"->"/" и прочее
  inline xpath& xpath::canonize(bool bWindows)
  {
    if(bWindows)
      change_sym("/","\\");
    else
      change_sym("\\","/");

    return *this;
  }


  // Проверяем и добавляем слэш в конце если нет
  inline xpath& xpath::checkSlash()
  {
    size_t len = this->length();
    if(len>0 && (*this)[len-1]!='/' && (*this)[len-1]!='\\')
      *this+="/";

    return *this;
  }

  // Оставляем только путь (без имени и расширения, в конце - '/')
  inline xpath& xpath::cutPath()
  {
    using namespace std;
    char buf[_MAX_PATH] = {0};

    strcpy(buf, this->c_str());
    char *p = max(strrchr(buf, '/'), strrchr(buf, '\\'));
    if (p) p[1] = 0;
    else buf[0] = 0;

    *this = buf;
    checkSlash();
    return *this;
  }

  // Оставляем только имя (без расширения)
  inline xpath& xpath::cutName()
  {
    char buf[_MAX_PATH] = {0};

    cutNameEx();
    strcpy(buf, this->c_str());
    char *p = strrchr(buf, '.');
    if (p) *p = 0;

    *this = buf;
    return *this;
  }

  // Оставляем только имя (с расширением)
  inline xpath& xpath::cutNameEx()
  {
    using namespace std;
    char buf[_MAX_PATH] = {0};

    const char *p = max(strrchr(this->c_str(), '/'), strrchr(this->c_str(), '\\'));
    if (p)
      memmove(buf, p + 1, strlen(p));

    *this = buf;
    return *this;
  }

  // Оставляем только расширение
  inline xpath& xpath::cutExt()
  {
    char buf[_MAX_PATH] = {0};

    cutNameEx();
    strcpy(buf, this->c_str());
    char *p = strrchr(buf, '.');
    if (p)
    {
      if (p == buf || p[-1] == '/')
        *buf = 0;
      else
        memmove(buf, p+1, strlen(p));
      *this = buf;
    }
    else
      *this = "";

    return *this;
  }

  // Оставляем только диск (в конце - ':') Для unix - пустое значение)
  inline xpath& xpath::cutDrive()
  {
    using namespace std;
    if(length()>=2 && (*this)[1]==':')
      *this = string(begin(),begin()+2);
    return *this;
  }

  // Заменяем или добавляем новое расширение
  inline xpath& xpath::setExt(const char* ext) 
  {
    using namespace std;
    char buf[_MAX_PATH] = {0};
    strcpy(buf, this->c_str());

    const char *s = max(strrchr(buf, '/'), strrchr(buf, '\\'));
    char *d = strrchr(buf, '.');
    if (d && d > s + 1)
      *d = 0;
    if (*ext != '.')
      strcat(buf, ".");
    strcat(buf, ext);

    *this = buf;
    return *this;
  };

  // Проверяем и убираем слэш в конце если есть
  inline xpath& xpath::stripSlash()
  {
    erase_sym_right("\\/");
    return *this;
  }

  // Заменяем или добавляем новое имя и расширение
  inline xpath& xpath::setName(const char* name)
  {
    xpath drv, dir, oldName, ext;

    splitPath(drv, dir, oldName, ext);
    makePath(drv, dir, xpath(name), ext);

    return *this;
  };

  //!< Заменяем путь (без имени и расширения, в конце - '/')
  inline xpath& xpath::setPath(const char* path)
  {
    xpath drv, dir, name, ext;

    splitPath(drv, dir, name, ext);
    *this = xpath(path).checkSlash().setName(name.c_str()).setExt(ext.c_str());

    return *this;
  }

  //!< Заменяем имя (с расширением)
  inline xpath& xpath::setNameEx(const char* nameex)
  {
    *this = xpath(*this).cutPath().checkSlash()+=nameex;

    return *this;
  }

  // устанавливаем название диска
  inline xpath& xpath::setDrive(const char* drive)
  {
    xpath drv, dir, name, ext;

    splitPath(drv, dir, name, ext);
    makePath(xpath(drive), dir, name, ext);

    return *this;
  };

  // Удаляем название диска
  inline xpath& xpath::eraseDrive()
  {
    if(length()>=2 && (*this)[1]==':')
      *this = std::string(begin()+2,end());

    return *this;
  };

  inline void xpath::splitPath(xpath& drv, xpath& dir, xpath& name, xpath& ext)
  {
    drv  = xpath(*this).cutDrive().stripSlash();
    dir  = xpath(*this).cutPath().stripSlash().eraseDrive().erase_sym_left("\\/");
    name = xpath(*this).cutName();
    ext  = xpath(*this).cutExt();
  }

  inline xpath& xpath::makePath(const char* drv, const char* dir, const char* name, const char* ext)
  {
#ifdef WIN32
    if(drv)
      this->format("%s:/%s/%s.%s", xstring(drv).erase_sym_right(":\\/").c_str(), dir, name, ext);
    else
      this->format("/%s/%s.%s", dir, name, ext);
#else
    this->format("%s/%s.%s", dir, name, ext);
#endif
    this->erase_sym_right(".");

    return *this;
  }

  inline xpath& xpath::makePath(const xpath& drv, const xpath& dir, const xpath& name, const xpath& ext)
  {
    return makePath(drv.c_str(), dir.c_str(), name.c_str(), ext.c_str());
  }

  // Преобразует данный xpath к полному пути
  inline xpath& xpath::getFullPathName()
  {
    *this = ar::Filesystem::absolutePath(*this);
    canonize();

    return *this;
  }

  inline bool isSlash(int c)
  { 
    return c!=0 && 0!=strchr("/\\", c);
  }

  inline bool isEndSlash(int c)      //!< \0 = тоже слеш
  {
    return !c || isSlash(c);
  }

  // перегрузка операторов == и != для сравнения без учета регистра
  inline bool equal(const xpath& xp1, const xpath& xp2)
  {
    const char* psz1 = xp1.c_str();
    const char* psz2 = xp2.c_str();
    bool bRC = true;
    for (; bRC && (*psz1||*psz2); psz1++, psz2++){
      bRC = toupper(*psz1)==toupper(*psz2);
      bRC=bRC || (isEndSlash(*psz1) && isEndSlash(*psz2));
    }
    return bRC;
  }

  // Строковая конкатенация
  inline xpath& xpath::operator += (const char * sub)
  { 
    using namespace std;
    *(string*)this += string(sub);
    return *this;
  }

  inline xpath& xpath::operator += (const xpath& sub )
  { 
    *this += sub.c_str();
    return *this;
  }

  //!< Общее начало путей a/b/c & a/b/d = a/b
  inline xpath& xpath::operator &= (const xpath& sub)
  {
    const char* pszM = this->c_str();
    const char* pszS = sub.c_str();
    const char* pszT = pszM;
    bool bEq = true, bSlash = false;
    for (; bEq; pszM++, pszS++)
    {
      bSlash = isEndSlash(*pszM) && isEndSlash(*pszS);
      bEq = bSlash || toupper(*pszM)==toupper(*pszS);
      if (bSlash)
        pszT = pszM+1;
      if (!*pszM) 
        break;
    }
    if (!bEq)
      *this = std::string(begin(),begin()+(pszT-this->c_str()));

    return *this;
  }

  //!< Конкатенация путей a/b/c + d/e = a/b/c/d/e
  inline xpath& xpath::concat(const xpath& sub)
  {
    xpath path = xpath(*this).checkSlash();
    xpath ssub = xpath(sub).erase_sym_left("\\/");
    *this = path += ssub;
    return *this;
  }

  //!< Разность путей a/b/c - a/b = d
  inline xpath& xpath::operator -= (const xpath& sub)
  {
    using namespace std;
    const char* pszM = this->c_str();
    const char* pszS = sub.c_str();
    const char* pszT = pszM;
    bool bEq = true, bSlash = false;
    for (; bEq; pszM++, pszS++)
    {
      bSlash = isEndSlash(*pszM) && isEndSlash(*pszS);
      bEq = bSlash || toupper(*pszM)==toupper(*pszS);
      if (bSlash)
        pszT = pszM+1;
      if (!*pszM) 
        break;
    }

    *this = string(begin()+(pszT-this->c_str()), end());
    this->erase_sym_left("\\/");

    return *this;
  }


  // Общее начало путей a/b/c & a/b/d = a/b
  inline xpath operator & (const xpath& path, const xpath& sub)
  {
    return xpath(path) &= sub;
  }

  //!< Конкатенация путей a/b/c + d/e = a/b/c/d/e
  inline xpath concat(const xpath& path, const xpath& sub)
  {
    return xpath(xpath(path) += sub);
  }

  //!< Разность путей a/b/c - a/b = d
  inline xpath operator - (const xpath& path, const xpath& sub)
  {
    return xpath(xpath(path) -= sub);
  }

  //!< Абсолютный путь. true если путь полный
  inline bool xpath::isAbsolutePath() const
  {
    const char* buf = this->c_str();
    if (length() > 1)
    {
      if (buf[1] == ':' || buf[0] == '/' || 
        (buf[0] == '\\' && buf[1] == '\\')
        )
        return true;
    }
    return false;
  }

  // Абсолютный путь. true если путь полный
  inline bool xpath::isRelativePath() const
  {
    if(length()==0)
      return false;
    return !isAbsolutePath();
  }

  //!< Сетевой путь. true если путь сетевой
  inline bool xpath::isNetworkPath() const 
  {
    const char* buf = this->c_str();
    if (length() > 1)
    {
#ifdef WIN32
      return ('\\' == buf[0]) && ('\\' == buf[1]);
#else
      return ('/' == buf[0]) && ('/' == buf[1]);
#endif
    }
    return false;
  }

  //!< Флажок: архивный файл
  inline bool xpath::isArchive(void)
  {
    xfindfile xff(xpath(*this).stripSlash());
    return xff.isArchive();
  }

  //!< Флажок: скрытый файл
  inline bool xpath::isHidden(void)
  {
    xfindfile xff(xpath(*this).stripSlash());
    return xff.isHidden();
  }

  //!< Флажок: обычный файл
  inline bool xpath::isNormal(void)
  {
    xfindfile xff(xpath(*this).stripSlash());
    return xff.isNormal();
  }

  //!< Флажок: файл только для чтения
  inline bool xpath::isReadOnly(void)
  {
    xfindfile xff(xpath(*this).stripSlash());
    return xff.isReadOnly();
  }

  //!< Флажок: поддиректория
  inline bool xpath::isSubdir(void)
  {
    xfindfile xff(xpath(*this).stripSlash());
    return xff.isSubdir();
  }

  //!< Флажок: системный файл
  inline bool xpath::isSystem(void)
  {
    xfindfile xff(xpath(*this).stripSlash());
    return xff.isSystem();
  }

  //!< Время создания файла
  inline time_t xpath::getFileCreationTime(void)
  {
    xfindfile xff(xpath(*this).stripSlash());
    return xff.getFileCreationTime();
  }

  //!< Время последнего доступа к файлу
  inline time_t xpath::getFileAccessTime(void)
  {
    xfindfile xff(xpath(*this).stripSlash());
    return xff.getFileAccessTime();
  }

  //!< Время последнего изменения файла
  inline time_t xpath::getFileWriteTime(void)
  {
    xfindfile xff(xpath(*this).stripSlash());
    return xff.getFileWriteTime();
  }

  //!< Размер файла
  inline size_t xpath::getFileSize(void)
  {
    xfindfile xff(xpath(*this).stripSlash());
    return xff.getFileSize();
  }

  // Обеспечивает существование папки по заданному пути
  inline bool xpath::ensureFolder()
  {
    return ar::Filesystem::ensureFolder(*this);
  }

  // Копирует файл по заданному пути
  inline bool xpath::copyTo(const char* file)
  {
    return ar::Filesystem::copy(*this, std::string(file));
  }

  // Передвигает файл по заданному пути
  inline bool xpath::moveTo(const char* file)
  {
    return ar::Filesystem::rename(*this, std::string(file));
  }

  // Удаление файла по текущему пути
  inline bool xpath::remove()
  {
    if(!exists())
      return true;
    return ar::Filesystem::remove(*this);
  }

  // Проверка существования файла/директории по текущему пути
  inline bool xpath::exists()
  {
    return ar::Filesystem::isFileExist(*this) || ar::Filesystem::isDirExist(*this);
  }

  //!< Получить полный путь относительно директории <szHomeDir>
  inline xpath& xpath::getFullPathName(const char* szHomeDir)
  {
    if (isRelativePath() && !isNetworkPath())
    {
      xpath xpHomeDir = szHomeDir;
      if (xpHomeDir.isRelativePath()) 
        return *this;
      *this = sx::concat(xpHomeDir, *this);
      getFullPathName();
    }
    return *this;
  }


  //!< Замена пути на последовательность вида ../../../
  inline xpath& xpath::dotPath()
  {
    xpath xp2;
    //const char* pszS = this->c_str();
    xpath xpNorm = (*this).stripSlash();
    const char* pszS = xpNorm.c_str();

    bool bSlash = 0, bPath = 0;
    for (;; pszS++){
      bSlash = isEndSlash(*pszS);
      if (bSlash && bPath)
        xp2 += "../";
      bPath = !bSlash;
      if(!*pszS) 
        break;
    }
    return *this=xp2;
  }

  // Получаем относительный <szHomeDir> путь
  inline xpath& xpath::makeRelativePath(const char* szHomeDir)
  {
    getFullPathName(szHomeDir);
    xpath xpHomeDir=szHomeDir;
    checkSlash(); xpHomeDir.checkSlash();

    // 1. Find root part
    xpath xpRoot = xpHomeDir & *this;
    if (xpRoot.length()==0)
      return *this;
    xpRoot.checkSlash();
    // 2. 
    xpath xp1 = xpHomeDir - xpRoot;
    // 3. replace xp1 onto ../../../ sequence
    xp1.dotPath();
    // 4.
    xpath xp2 = *this - xpRoot;
    // 5. 
    *this = sx::concat(xp1, xp2);
    if (length()==0)
      *this = "./";
    return *this;
  }

  inline xpath& xpath::makeRelativePath()	// получаем относительный текущей рабочей директории путь
  {
    return makeRelativePath(xpath().getCurrentDirectory().c_str());
  }

}; // namespace sx

#endif // SX_PATH_H
