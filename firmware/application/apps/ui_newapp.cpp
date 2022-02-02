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
        //return (sd_card::status() == sd_card::Status::Mounted) ? true : false; 
        return true;
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
                                                                                            

    bool NewAppView::delete_dir_or_file(const std::filesystem::path& path, std::string name) { // Deletes file or dir.
        return !(delete_file(path.string() + "/" + name));                                     // 0 is sucess.
    }


    bool NewAppView::create_file(const std::filesystem::path& path, std::string name) {
        File file = { };                                                                // Create File object
        Optional<File::Error> sucess = file.create(path.string() + "/" + name);         // Create File
        return !(sucess.is_valid());                                                    // 0 is sucess
    }


    bool NewAppView::rename_dir_or_file(const std::filesystem::path& path, std::string old_name, std::string new_name) {
        return !(rename_file(path.string() + "/" + old_name, new_name));                              // 0 is sucess
    }


    std::string NewAppView::read_file(const std::filesystem::path& path, std::string name) { // Read file
        std::string return_string = "";                                                      // String to be returned
        File file;                                                                           // Create File object
        auto sucess = file.open(path.string() + "/" + name);                               // Open file to write

        if(!sucess.is_valid()) {                                                             // 0 is sucess
            char one_char[1];                                                                // Read file char by char
            for(size_t pointer = 0; pointer < file.size() ; pointer++) {                     // Example won't work for large files
                file.seek(pointer);                                                          // Sets file to next pointer
                file.read(one_char, 1);                                                      // sets char to one_char[]
			    return_string += one_char[0];				                                 // Add it to the return_string               
            }
        } else {
            return "0";                                                                      // Basic error handling
        }
        return return_string;
    }


    bool NewAppView::write_file(const std::filesystem::path& path, std::string name, std::string data) {
        File file;                                                                     // Create File object
        auto sucess = file.append(path.string() + "/" + name);                         // Open file
        if(!sucess.is_valid()) {                                                       // 0 is sucess
            file.write_line(data);
            return true;
        } else {
            return false;
        }
    }


    NewAppView::NewAppView(NavigationView &nav) // Application Main
    {
        add_children({
            &my_console,                        // Add pointers for widgets
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
                my_console.writeln("+ New directory created");       // If new dir succeeded 
            } else {
                my_console.writeln("- New directory FAILED");        // Else new dir failed
            } 
        } else {                                                     // Else, check_sd_card() returned false
            my_console.writeln("- New directory FAILED");
        }

        // List Directory
        std::string dir_contents = "";                                   // String var that displays the dir contents
        std::vector<file_entry> files = { };                             // file_entry Vector
        if(check_sd_card()) {                                            // Check to see if SD Card is mounted
            files = list_dir(u"");                                       // dir of SD Card, u"" is root, u"DIRECTORY_NAME" for other directorys
                                                                         // Vector has a capacity of 64 objects
            if(files.size()) {                                           // If files are not empty
                dir_contents += "+ dir SD Card: ";
                for (const auto& f : files) {                            // For each f of files
                    dir_contents += f.entry_path.string() + ", ";        // Copy name to dir_contents
                }
            } else {                                                     // Else, files are empty 
                my_console.writeln("- dir SD Card FAILED");              
            }
        } else {                                                         // Else, check_sd_card() returned false
            my_console.writeln("- dir SD Card FAILED");
        }
        //my_console.writeln(dir_contents);                              // Write results to my_console
                                                                         // Write total count to my_console
        my_console.writeln("+ dir SD Card: " + std::to_string(files.size()) + " items"); 

        // Rename directory
        if(check_sd_card()) {                                       // Check to see if SD Card is mounted
            if(rename_dir_or_file(u"", "NEW_DIR", "NEWER_DIR")) {   // Renames dir in root of SD Card, returns true if sucessfull
                my_console.writeln("+ Directory renamed");          // If dir renamed succeeded 
            } else {
                my_console.writeln("- Directory renamed FAILED");   // Else new dir renamed failed
            }   
        } else {                                                    // Else, check_sd_card() returned false
            my_console.writeln("- Directory renamed FAILED");
        }

        // Delete file or directory 
        if(check_sd_card()) {                                         // Check to see if SD Card is mounted
            if(delete_dir_or_file(u"", "NEWER_DIR")) {                  // New dir in root of SD Card
                my_console.writeln("+ New directory deleted");
            } else {
                my_console.writeln("- New directory deleted FAILED");
            }
        } else {                                                      // Else, check_sd_card() returned false
            my_console.writeln("- New directory deleted FAILED");
        }

        // New file
        if(check_sd_card()) {                                        // Check to see if SD Card is mounted
            if(create_file(u"", "NEW_FILE.txt")) {                   // New dir in root of SD Card, returns true if sucessfull
                my_console.writeln("+ New file created");            // If new file succeeded 
            } else {
                my_console.writeln("- New file FAILED");             // Else new file failed
            } 
        } else {                                                     // Else, check_sd_card() returned false
            my_console.writeln("- New file FAILED");
        }

        // Rename file
        if(check_sd_card()) {                                               // Check to see if SD Card is mounted
            if(rename_dir_or_file(u"", "NEW_FILE.txt", "NEWER_FILE.TXT")) { // Renames file, returns true if sucessfull
                my_console.writeln("+ File renamed");                       // If file renamed succeeded 
            } else {
                my_console.writeln("- File renamed FAILED");                // Else new file renamed failed
            }   
        } else {                                                            // Else, check_sd_card() returned false
            my_console.writeln("- File renamed FAILED");
        }

        // Write file
        if(check_sd_card()) {                                         // Check to see if SD Card is mounted
            std::string data = "Your mother was a hamster!";
            if(write_file(u"", "NEWER_FILE.TXT", data)) {             // Sucess is anything but 0
                my_console.writeln("+ Write File");                   // Write data to my_console
            } else {
                my_console.writeln("- Write file FAILED");
            }
        } else {                                                     // Else, check_sd_card() returned false
            my_console.writeln("- Write file FAILED");
        }

        // Read file
        if(check_sd_card()) {                                        // Check to see if SD Card is mounted
            std::string data = "";                                   // Create output string
            data = read_file(u"", "NEWER_FILE.TXT");                 // read_file()
            if(data != "0") {                                        // Sucess is anything but 0
                my_console.writeln("+ Read file: " + data);          // Write data to my_console
            } else {
                my_console.writeln("- Read file FAILED");
            }
        } else {                                                     // Else, check_sd_card() returned false
            my_console.writeln("- Read file FAILED");
        }

        // Delete file or directory 
        if(check_sd_card()) {                                         // Check to see if SD Card is mounted
            if(delete_dir_or_file(u"", "NEWER_FILE.TXT")) {           // New dir in root of SD Card
                my_console.writeln("+ New file deleted");
            } else {
                my_console.writeln("- New file deleted FAILED");
            }
        } else {                                                      // Else, check_sd_card() returned false
            my_console.writeln("- New file deleted FAILED");
        }

        // Done
        my_console.writeln("+ Demo Complete");

    }
}