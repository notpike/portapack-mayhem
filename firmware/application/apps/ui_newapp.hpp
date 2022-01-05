#include "ui.hpp"
#include "ui_widget.hpp"
#include "ui_navigation.hpp"
#include "string_format.hpp"

#define PROGRESS_MAX 100

namespace ui
{
    class NewAppView : public View
    {
    public:

        uint32_t progress; 

        NewAppView(NavigationView &nav);
        std::string title() const override { 
            return "New App"; 
        };

    private:
        void update();
        MessageHandlerRegistration message_handler_update{
            Message::ID::DisplayFrameSync,
            [this](const Message *const) {
                this->update();
            }
        };

        Labels label_progress {
            {{2*8, 1*8},
            "Progress:",
            Color::light_grey()}           
        };

        NumberField field_progress {
            {12*8, 1*8},
            3,
            {0,PROGRESS_MAX},
            1,
            '0'
        };

        ProgressBar my_progressBar {
            {2*8, 4*8, 208, 16 },    // Coordinates are: int:x, int:y, int:width, int:height
        };

        LiveDateTime live_timestamp {
		    {2*8, 8*8, 11*8, 20 }
	    };

        
    };
} 