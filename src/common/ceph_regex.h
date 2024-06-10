#pragma once

#include <memory>

#ifdef CEPH_USE_RE2

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wzero-as-null-pointer-constant"
#include <re2/re2.h>
#pragma clang diagnostic pop

using ceph_regex_t = re2::RE2;

#else

#include <regex>

using ceph_regex_t = std::regex;

#endif

std::unique_ptr<ceph_regex_t> make_ceph_regex(const std::string & pattern, bool posix = false);
size_t get_mark_count(const ceph_regex_t & re);
bool ceph_regex_match(const std::string & str, const std::string & pattern, bool posix = false);
std::vector<std::string> ceph_regex_search(const std::string & str, const ceph_regex_t & re);
