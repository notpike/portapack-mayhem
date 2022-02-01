#include "ui.hpp"
#include "ui_widget.hpp"
#include "ui_navigation.hpp"
#include "string_format.hpp"

// Add these to include File Class and SD Card helper functions.
#include "file.hpp" 
#include "sd_card.hpp"

namespace ui
{
    // Struct that outlines file information
    struct file_entry {
        std::filesystem::path entry_path { };
        uint32_t size { };
        bool is_directory { };
    };

    class NewAppView : public View                                // App class declaration
    {
    public:

        // Public declarations
    	void focus() override;                                    // ui::View function override

        NewAppView(NavigationView &nav);                          // App class init function declaration
        std::string title() const override { return "New App"; }; // App title

    private:

        // Function declarations

        // Error check
        bool check_sd_card();

        // DIR CRUD                                            
        bool create_dir(const std::filesystem::path& path, std::string name);
        std::vector<file_entry> list_dir(const std::filesystem::path& path);
        void delete_dir_or_file(const std::filesystem::path& path, std::string name);

        // File CRUD
        void create_file(const std::filesystem::path& path, std::string name);
        std::string read_file(const std::filesystem::path& path, std::string name);
        void write_file(const std::filesystem::path& path, std::string name, std::string data);
        void append_file(const std::filesystem::path& path, std::string name, std::string data);

        // Widgets
        Console my_console {
            { 1*8, 1*8, 224, 296 },    // Coordinates are: int:x (px), int:y (px), int:width (px), int:height (px)
        };
    };
} 