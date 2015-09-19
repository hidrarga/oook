#ifndef __FILESYSTEMUTILS__H_
#define __FILESYSTEMUTILS__H_

#define BOOST_NO_CXX11_SCOPED_ENUMS

#include <boost/filesystem/path.hpp>

namespace fs = boost::filesystem;

class FileSystemUtils {
public:
    static void copyFile(fs::path src, fs::path dst, fs::path const& filename, bool force);
    static void copyFile(fs::path const& src, fs::path const& dst, bool force) {
        copyFile(src, dst, "", force);
    }
    static void copyDirectory(fs::path const& src, fs::path const& dst, bool force, bool recursive);
};

#endif
