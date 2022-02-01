#include "ui_newapp.hpp"
#include "portapack.hpp"
#include <cstring>

using namespace portapack;

namespace ui
{

    void NewAppView::focus() {                               // Default selection to my_console when app starts
	    my_console.focus();
    }
    
    // Checks SD Card, returns true if Mounted, false if otherwise
    bool NewAppView::check_sd_card() {
        return (sd_card::status() == sd_card::Status::Mounted) ? true : false; 
    }

    // Lists all files and directorys in path
    std::vector<file_entry> NewAppView::list_dir(const std::filesystem::path& path) {

        // Files and directories list
        std::vector<file_entry> entry_list { };

        // For each entry in the file system's directory
        // Adds files in directorys into entry_list{} 
        // Directorys are inserted infront of files
        for (const auto& entry : std::filesystem::directory_iterator(path, u"*")) {

            // Dose not add directorys or files starting with '.' (hidden / tmp)
            if (entry.path().string().length() && entry.path().filename().string()[0] != '.') {

                // If file 
                if (std::filesystem::is_regular_file(entry.status())) {
                    entry_list.push_back({ entry.path(), (uint32_t)entry.size(), false });

                // Else If directory    
                } else if (std::filesystem::is_directory(entry.status())) {
                    entry_list.insert(entry_list.begin(), { entry.path(), 0, true });
                
                // Other
                } else {
                    continue;
                }
            }

        }
        return entry_list;
    }

    // Creates dir, returns true if sucessfull 
    bool NewAppView::create_dir(const std::filesystem::path& path, std::string name) {  // make_new_directory() returns 0 for sucess
        return !(make_new_directory(path.string() + "/" + name));                       // and other values means failure. Not operation  
    }                                                                                   // will return true only when 0 is returned. 
                                                                                            

    void NewAppView::delete_dir_or_file(const std::filesystem::path& path, std::string name) {
        delete_file(path.string() + "/" + name);
    }


    void NewAppView::create_file(const std::filesystem::path& path, std::string name) {

    }

    std::string NewAppView::read_file(const std::filesystem::path& path, std::string name) {

    }

    void NewAppView::write_file(const std::filesystem::path& path, std::string name, std::string data) {

    }

    void NewAppView::append_file(const std::filesystem::path& path, std::string name, std::string data) {

    }


    NewAppView::NewAppView(NavigationView &nav) // Application Main
    {
        add_children({
            &my_console,                       // Add pointers for widgets
        });

        // Enable scrolling
        my_console.enable_scrolling(true);

        // Check SD Card
        if(check_sd_card()) {                                       // Check to see if SD Card is mounted
            my_console.writeln("+ SD Card is Mounted");
        } else {                                                    // Else, check_sd_card() returned false 
            my_console.writeln("- SD Card is NOT Mounted");
        }

        // New directory
        if(check_sd_card()) {                                        // Check to see if SD Card is mounted
            if(create_dir(u"", "NEW_DIR")) {                         // New dir in root of SD Card, returns true if sucessfull
                my_console.writeln("+ New dir created");             // If new dir succeeded 
            } else {
                my_console.writeln("- New dir FAILED");              // Else new dir failed
            } 
        } else {                                                     // Else, check_sd_card() returned false
            my_console.writeln("- New dir FAILED");
        }

        // std::string dir_contents = "";                                   // String var that displays the dir contents
        // if(check_sd_card()) {                                            // Check to see if SD Card is mounted
        //     std::vector<file_entry> files = list_dir(u"");               // dir of SD Card, u"" is root, u"DIRECTORY_NAME" for other directorys
        //                                                                  // Vector has a capacity of 64 objects
        //     if(files.size()) {                                           // If files are not empty
        //         dir_contents += "+ dir SD Card: ";
        //         for (const auto& f : files) {                            // For each f of files
        //             dir_contents += f.entry_path.string() + ", ";        // Copy name to dir_contents
        //         }
        //     } else {                                                     // Else, files are empty 
        //         my_console.writeln("- dir SD Card FAILED");              
        //     }
        // } else {                                                         // Else, check_sd_card() returned false
        //     my_console.writeln("- dir SD Card FAILED");
        // }
        // my_console.writeln(dir_contents);                                // Write results to my_console


        // Delete directory
        if(check_sd_card()) {                                         // Check to see if SD Card is mounted
            delete_dir_or_file(u"", "NEW_DIR");                       // New dir in root of SD Card
            my_console.writeln("+ New dir deleted");
        } else {                                                      // Else, check_sd_card() returned false
            my_console.writeln("- New dir deleted FAILED");
        }


        // // List files and directorys of SD Card
        // dir_contents = "";
        // if(true) {
        //     std::vector<file_entry> files = list_dir(u"");               // dir of SD Card, u"" is root, u"DIRECTORY_NAME" for other directorys
        //                                                                  // capacity: 64
        //     if(files.size()) {
        //         dir_contents += "+ dir SD Card: ";
        //         for (const auto& f : files) {                           // For each f of files
        //             dir_contents += f.entry_path.string() + ", ";
        //         }
        //     } else {
        //         my_console.writeln("- dir SD Card FAILED");
        //     }
        // } else {
        //     my_console.writeln("- dir SD Card FAILED");
        // }
        // my_console.writeln(dir_contents);

        my_console.writeln("Complete");

    }

}