#pragma once
#include <FS.h>
#include <string>

namespace FS {

void setup();

namespace MIME {

char* get(const char* filepath);
void add(const char* ends_with, const char* mime_type);

} // namespace MIME

} // namespace FS
