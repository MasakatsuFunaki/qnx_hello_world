// Tests that includes via strip_include_prefix work
// This header is accessed via the stripped prefix path
#ifndef MYAPI_H
#define MYAPI_H

namespace myapi {

inline int get_version() { return 1; }
inline const char* get_name() { return "MyAPI"; }

}  // namespace myapi

#endif
