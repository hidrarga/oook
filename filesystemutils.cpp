#include "filesystemutils.hpp"

#include <boost/filesystem.hpp>

void FileSystemUtils::copyFile(fs::path src, fs::path dst, fs::path const& filename, bool force) {
    if(!filename.empty()) {
        src /= filename;
        dst /= filename;
    }
    
    try { 
        if(fs::exists(dst) and fs::is_regular(dst)) {
            if(force or fs::last_write_time(src) > fs::last_write_time(dst) or fs::file_size(src) != fs::file_size(dst))
                fs::copy_file(src, dst, fs::copy_option::overwrite_if_exists);
            else
                throw std::runtime_error("Error: Can't overwrite file.");
        } else
            fs::copy_file(src, dst);
    } catch(std::runtime_error const& e) {
        throw std::runtime_error("Error: Can't copy file " + src.string() + " to " + dst.string() + ".");
    }
}

void FileSystemUtils::copyDirectory(const fs::path & src, const fs::path& dst, bool force, bool recursive)
{
    if(!fs::exists(src) or !fs::is_directory(src))
        throw std::runtime_error("Error: Can't copy directory: Source directory " + src.string() + " doesn't exists or isn't a directory.");
    
    fs::create_directories(dst);
    
    if(!fs::is_directory(dst))
        throw std::runtime_error("Error: Can't copy directory: Unable to create destination directory " + dst.string() + ".");
    
    for(fs::directory_iterator it(src); it != fs::directory_iterator(); ++it) {
        fs::path current = it->path();
        
        if(fs::is_directory(current)) {
            if(recursive)
                copyDirectory(current, dst / current.filename(), force, recursive);
        } else
            copyFile(current, dst / current.filename(), force);
    }
}
