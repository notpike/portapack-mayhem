#include "ui.hpp"
#include "ui_widget.hpp"
#include "ui_navigation.hpp"
#include "string_format.hpp"

// Define a constant
#define PROGRESS_MAX 100

namespace ui
{
    class NewAppView : public View
    {
    public:

    	void focus() override;

        NewAppView(NavigationView &nav);
        std::string title() const override { 
            return "New App";                 // App Title
        };

    private:

        void update();
        MessageHandlerRegistration message_handler_update{
            Message::ID::DisplayFrameSync,
            [this](const Message *const) {
                this->update();
            }
        };

        // Variables
        uint32_t progress = 0; 

        // Widgets
        // Usable screen space is 240x304px
        // Note: Each char takes up 8x8px so you can multiply 
        //       the amount of spaces and rows you want by 8.
        //       This gives you 30x38 char
        Button button_helloWorld{
            {70, 128, 100, 24},             // Coordinates are: int:x (px), int:y (px), int:width (px), int:height (px)
            "Hello World!"                  // Title
        };

        LiveDateTime timestamp {
		    {6*8, 22*8, 19*8, 20 }          // Coordinates and Dimensions
	    };

        Labels label_progress {
            {{8*8, 33*8},                  // Coordinates are: int:x(px), int:y(px)
            "Progress:     %",            // Title
            Color::light_grey()}           // Title color
        };

        NumberField numberField_progress {
            {18*8, 33*8},                  // Coordinates
            3,                             // Length of number
            {0,PROGRESS_MAX},              // Range
            1,                             // Step
            '0',                           // Fill Char
            false                          // Loop?
        };

        ProgressBar progressBar_progress {
            {2*8, 35*8, 208, 16 },          // Coordinates and Dimensions
        };
        
    };
} 