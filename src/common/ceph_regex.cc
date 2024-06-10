#include "common/ceph_regex.h"

#ifdef CEPH_USE_RE2

std::unique_ptr<ceph_regex_t> make_ceph_regex(const std::string & pattern, bool posix)
{
    re2::RE2::Options options;
    options.set_posix_syntax(posix);
    return std::make_unique<ceph_regex_t>(pattern, options);
}

size_t get_mark_count(const ceph_regex_t & re)
{
    return re.NumberOfCapturingGroups();
}

bool ceph_regex_match(const std::string & str, const std::string & pattern, bool posix)
{
    re2::RE2::Options options;
    options.set_posix_syntax(posix);
    ceph_regex_t re(pattern, options);
    return re2::RE2::FullMatch(str, re);
}
std::vector<std::string> ceph_regex_search(const std::string & str, const ceph_regex_t & re)
{
    std::vector<std::string> result;
    re2::StringPiece input(str);
    re2::StringPiece match;
    while (re2::RE2::FindAndConsume(&input, re, &match))
        result.push_back(std::string(match));
}

#else

std::unique_ptr<ceph_regex_t> make_ceph_regex(const std::string & pattern, bool posix = false)
{
    try
    {
        return std::make_unique<ceph_regex_t>(pattern, posix ? std::regex::extended : std::regex_constants::ECMAScript);
    }
    catch (const std::regex_error &)
    {
        return nullptr;
    }
}

size_t get_mark_count(const ceph_regex_t & re)
{
    return re.mark_count();
}

bool ceph_regex_match(const std::string &str, const std::string &pattern, bool posix = false)
{
    std::regex re(pattern, pattern, posix ? std::regex::extended : std::regex_constants::ECMAScript);
    return std::regex_match(str, re);
}

std::vector<std::string> ceph_regex_search(const std::string & str, const ceph_regex_t & re)
{
    std::smatch match;
    if (std::regex_search(str, match, re))
    {
        for (size_t i = 1; i < match.size(); i++)
            result.push_back(match[i].str());
    }
}

#endif
