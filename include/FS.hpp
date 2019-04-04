#ifndef _FS_hpp
#define _FS_hpp

#include <FS.h>

#include <string>

namespace FS {

void setup();

namespace MIME {

std::string get(std::string filepath);
void add(std::string endsWith, std::string MIMEtype);
} // namespace MIME

} // namespace FS

#endif
