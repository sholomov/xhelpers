//! @file     arcore/sx_system.h
//! @author   Usilin Sergey
//! @author   Sholomov Dmitry
//! @brief    File operations, system operations

#pragma once
#ifndef SX_SYSTEM_H
#define SX_SYSTEM_H

#include <string>
#include <vector>

namespace sx
{
//! @addtogroup prj_core_arcore_sysutils 
//! @{

//! @class Filesystem filesystem.h arcore/sysutils.h
//! @brief Фaйловые операции.
class Filesystem
{
public:
  static std::string moduleDirPath();                         //!< Возвращает путь к исполняемому модулю (в конце - '/')
  static std::string currentDirectory();                      //!< Возвращает текущую рабочую директорию (в конце - '/')
  static std::string tempDirectory();                         //!< Возвращает директорию для временных файлов (в конце - '/')
  static bool isPathRelative(const std::string &path);        //!< Возвращает @c true в случае относительно пути
  static std::string absolutePath(const std::string &path);   //!< Возвращает абсолютный путь для относительного
  static std::string canonizePath(const std::string &path);   //!< Возвращает путь в канонизированном виде (прямой слеш) 
  static std::string fileName(const std::string &path);       //!< Возвращает имя файла (включая расширение)
  static std::string fileBaseName(const std::string &path);   //!< Возвращает базовое имя файла (без расширения)
  static std::string fileExtension(const std::string &path);  //!< Возвращает расширение файла
  static bool isFileExist(const std::string &path);           //!< Возвращает true, если файл существует
  static bool isDirExist(const std::string &path);            //!< Возвращает true, если директория существует
  static std::string getFileDir(const std::string &path);     //!< Возвращает путь к директории по имени файла.
    
  static bool ensureFolder(const std::string &path);          //!< Обеспечивает существование папки по заданному пути.
    
  //! Возвращает true, если удалось удалить указаный файл или папку
  static bool remove(const std::string &path);
  //! Возвращает true, если удалось переименовать указаный файл или папку
  static bool rename(const std::string &pathFrom, const std::string &pathTo);
  //! Возвращает true, если удалось скопировать указаный файл (для папок не реализовано)
  static bool copy(const std::string &pathFrom, const std::string &pathTo);                

private:
  Filesystem();                                               //!< Конструктор по-умолчанию (запрещен)
  Filesystem(const Filesystem &);                             //!< Конструктор копирования (запрещен)
  Filesystem &operator=(const Filesystem &);                  //!< Оператор присваивания (запрещен)

  static std::string join(const std::string &path1, const std::string &path2);
};

//! @}
} // namespace ar


#endif  // ARCORE_FILESYSTEM_H
