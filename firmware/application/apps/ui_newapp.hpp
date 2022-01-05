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
        NewAppView(NavigationView &nav);
        std::string title() const override { 
            return "New App"; 
        };

    private:

        uint32_t progress; 

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
            {6*8, 1*8},
            3,
            {0,PROGRESS_MAX},
            1,
            '0'
        };

        
    };
} 