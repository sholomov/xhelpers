//!
//! @file   xhelpers_test.cpp
//! @author Sholomov Dmitry
//! @date   16.04.2021
//! @brief  Basic test of xhelpers classes.
//!

#include <xhelpers/sx_string.h>
#include <xhelpers/sx_path.h>

using namespace std;
using namespace sx;

int main()
{
  // check xfile
  xpath xpFile = xpath().getModuleFileName();
  
  // check xfindfile
  for(xfindfile xff("/*"); xff; ++xff)
  {
    if(xff.isSubdir())
      continue;
    xpath path = xff.filePath();
  }

  return 1;
}
