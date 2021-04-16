//!
//! @file     arcore/sx_system.h
//! @author   Usilin Sergey
//! @author   Sholomov Dmitry
//! @brief    File operations, system operations
//!

#include "../sx_system.h"

#ifdef _MSC_VER
#  ifndef NOMINMAX
#     define NOMINMAX
#  endif
#  include <windows.h>
#  include <direct.h>
#  include <limits.h>
#  include <shlobj.h>
#  ifndef S_ISDIR
#      define S_ISDIR(mode)  ((mode & _S_IFMT) == _S_IFDIR)
#  endif
#  ifndef S_ISREG
#     define S_ISREG(mode)  ((mode & _S_IFMT) == _S_IFREG)
#  endif

#elif __linux__
#include <cstdlib>
#include <unistd.h>
#endif

#include <sys/types.h>
#include <sys/stat.h>

#include "../portability/file_system.hpp"

#include <cstring>
#include <ctime>
#include <iostream>

std::string sx::Filesystem::moduleDirPath()
{
  char libraryPathBuf[256] = {0};

#ifdef _MSC_VER
  HMODULE hModule = 0;
  BOOL ret = GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPCTSTR)&__FUNCTION__, &hModule);
  DWORD nLen = GetModuleFileNameA(hModule, libraryPathBuf, sizeof(libraryPathBuf));
  if (nLen == sizeof(libraryPathBuf))
    _getcwd(libraryPathBuf, sizeof(libraryPathBuf));

  char drive[256] = {0}, directory[1024] = {0};
  _splitpath(libraryPathBuf, drive, directory, 0, 0);
  _makepath(libraryPathBuf, drive, directory, 0, 0);
#elif __linux__
  if (getcwd(libraryPathBuf, sizeof(libraryPathBuf)) == NULL)
      return std::string();
#endif

  std::string libraryPath(libraryPathBuf);
  const char lastChar = libraryPath[libraryPath.length() - 1];
  if (lastChar != '\\' && lastChar != '/')
    libraryPath.append("/");

  return canonizePath(libraryPath);
}

//!< Возвращает текущую рабочую директорию (в конце - '/')
std::string sx::Filesystem::currentDirectory()
{
  char buf[256] = {0};
#ifdef _MSC_VER
  if(!::GetCurrentDirectory(sizeof(buf), buf))
    return std::string();
#elif __linux__
  if (getcwd(buf, sizeof(buf)) == NULL)
    return std::string();
#endif
  return canonizePath(std::string(buf));
}

//!< Возвращает директорию для временных файлов (в конце - '/')
std::string sx::Filesystem::tempDirectory()
{
#ifdef _MSC_VER
  char buf[256] = {0};
  if(!::GetTempPath( sizeof(buf), buf ))
    return std::string();
  return canonizePath(std::string(buf));
#elif __linux__
  std::string tempDir = "/tmp"; // Typically the global temporary directory under linux
  ar::Filesystem::ensureFolder(tempDir);
  return canonizePath(tempDir);
#endif
}

bool sx::Filesystem::isPathRelative(const std::string &path)
{
#ifdef _MSC_VER
  bool withoutDriveLetter = path.empty() || ( path.size() > 0 && path[0] != '/' && path[0] != '\\');
  char driveBuf[256] = {0};
  _splitpath(path.c_str(), driveBuf, 0, 0, 0);
  return strlen(driveBuf) <= 0 && withoutDriveLetter;
#elif __linux__
  return path.empty() || ( path.size() > 0 && path[0] != '/' );
#endif
}

std::string sx::Filesystem::join(const std::string &path1, const std::string &path2)
{
  if (!sx::Filesystem::isPathRelative(path2))
    return sx::Filesystem::canonizePath(path2);
  std::string canonizedPath1 = sx::Filesystem::canonizePath(path1);
  if (canonizedPath1.length() > 0 && canonizedPath1[canonizedPath1.length() - 1] != '/')
    canonizedPath1 += "/";
  return canonizedPath1 + sx::Filesystem::canonizePath(path2);
}

std::string sx::Filesystem::absolutePath(const std::string &path)
{
  return canonizePath(isPathRelative(path) ? join(moduleDirPath(), path) : path);
}

std::string sx::Filesystem::canonizePath(const std::string &path)
{
  std::string canonizedPath = path;
  for (size_t i = 0; i < canonizedPath.length(); i++)
    if (canonizedPath[i] == '\\')
      canonizedPath[i] = '/';

  return canonizedPath;
}

std::string sx::Filesystem::fileName(const std::string &path)
{
  size_t pos = canonizePath(path).rfind("/");
  return (pos == std::string::npos) ? path : path.substr(pos + 1);
}

std::string sx::Filesystem::fileBaseName(const std::string &path)
{
  size_t pos = canonizePath(path).rfind(".");
  return fileName(pos == std::string::npos ? path : path.substr(0, pos));
}

std::string sx::Filesystem::fileExtension(const std::string &path)
{
  size_t pos = canonizePath(path).rfind(".");
  return (pos == std::string::npos) ? std::string() : path.substr(pos + 1);
}

bool sx::Filesystem::isFileExist(const std::string &path)
{
  int res(-1);
#ifdef _WINDOWS
  struct _stat64 info = {0};
  std::vector<WCHAR> pathW(MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, path.c_str(), -1, 0, 0));
  if (0 < MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, path.c_str(), -1, &pathW[0],
    static_cast<int>(pathW.size())))
  {
    res = _wstat64(pathW.data(), &info);
  }
#else
  struct stat info = {0};
  res = stat(path.c_str(), &info);
#endif
  return (0 == res) && S_ISREG(info.st_mode);
}

bool sx::Filesystem::isDirExist(const std::string &path)
{
  // TODO: remove trailing slash
  int res(-1);
#ifdef _WINDOWS
  struct _stat info = {0};
  std::vector<WCHAR> pathW(MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, path.c_str(), -1, 0, 0));
  if (0 < MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, path.c_str(), -1, &pathW[0],
    static_cast<int>(pathW.size())))
  {
    if (2 < static_cast<int>(pathW.size()))
      if (pathW[static_cast<int>(pathW.size()) - 2] == '/' ||
          pathW[static_cast<int>(pathW.size()) - 2] == '\\') {
      pathW.erase(pathW.end() - 2);
    }
    res = _wstat(pathW.data(), &info);
  }
#else
  struct stat info = {0};
  res = stat(path.c_str(), &info);
#endif
  return (0 == res) && S_ISDIR(info.st_mode);
}

std::string sx::Filesystem::getFileDir(const std::string &path)
{
  std::string cpath = sx::Filesystem::canonizePath(path);
  std::string fileName = sx::Filesystem::fileName(path);
  std::string fileDir = cpath.substr(0, cpath.size() - fileName.size());
  while (fileDir.size() > 1 && fileDir[fileDir.size() - 1] == '/')
    fileDir.erase(fileDir.size() - 1, 1);
  return fileDir;
}


bool sx::Filesystem::ensureFolder(const std::string &path)
{
  if (0 < path.size())
  // not empty path
  {
#ifdef _WINDOWS
    std::vector<WCHAR> pathW(MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, path.c_str(), -1, 0, 0));
    if (0 < MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, path.c_str(), -1, &pathW[0],
      static_cast<int>(pathW.size())))
    {
      std::vector<WCHAR> pathFull(GetFullPathNameW(&pathW[0], 0, 0, 0));
      if (0 < GetFullPathNameW(&pathW[0], static_cast<DWORD>(pathFull.size()), &pathFull[0], 0))
      {
        if (ERROR_SUCCESS == SHCreateDirectoryExW(0, &pathFull[0], 0) || 
            ERROR_ALREADY_EXISTS == GetLastError())
        return true;
      }
    }
#elif __linux__
    struct stat st = {0};
    if (!isDirExist(path))
      int res = system(("mkdir -p " + path).c_str());

    if (stat(path.c_str(), &st) == 0)
      return true;
#endif
    std::cerr << "Filesystem::ensureFolder can't create directory " << path << std::endl;
  }
  else
  {
    std::cerr << "Filesystem::ensureFolder: empty path " << std::endl;
  }

  return false;
}

// TODO: сделать работоспособные кириллические имена
bool sx::Filesystem::remove(const std::string &path)
{
  bool res(true);
  if (0 < path.size())
  // not empty path
  {
    if (isDirExist(path))
    {
      res = stlplus::folder_delete(path, true);
    }
    else if (isFileExist(path))
    {
      res = stlplus::file_delete(path);
    }
    else
    {
      std::cerr << "Filesystem::remove can't detect path type for " << path << std::endl;
      res = false;
    }
  }
  else
  {
    std::cerr << "Filesystem::remove empty path" << std::endl;
  }
  return res;
}

bool sx::Filesystem::rename(const std::string &pathFrom, const std::string &pathTo)
{
  bool res(true);
  if ((0 < pathFrom.size()) && (0 < pathTo.size()))
  // not empty path
  {
    if (isDirExist(pathFrom))
    {
      res = stlplus::folder_rename(pathFrom, pathTo);
    }
    else if (isFileExist(pathFrom))
    {
      res = stlplus::file_rename(pathFrom, pathTo);
    }
    else
    {
      std::cerr << "Filesystem::rename can't detect path type for " << pathFrom << std::endl;
      res = false;
    }
  }
  else
  {
    std::cerr << "Filesystem::rename empty path" << std::endl;
  }
  return res;
}
  
bool sx::Filesystem::copy(const std::string &pathFrom, const std::string &pathTo)                
{
  bool res(true);
  if ((0 < pathFrom.size()) && (0 < pathTo.size()))
  // not empty path
  {
    if (isDirExist(pathFrom))
    {
      std::cerr << "Filesystem::copy not implimented for folder " << pathFrom << std::endl;
      res = false;
    }
    else if (isFileExist(pathFrom))
    {
      res = stlplus::file_copy(pathFrom, pathTo);
    }
    else
    {
      std::cerr << "Filesystem::copy can't detect path type for " << pathFrom << std::endl;
      res = false;
    }
  }
  else
  {
    std::cerr << "Filesystem::copy empty path" << std::endl;
  }
  return res;
}


