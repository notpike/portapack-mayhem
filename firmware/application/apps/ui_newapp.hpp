#include "ui.hpp"
#include "ui_widget.hpp"
#include "ui_navigation.hpp"
#include "string_format.hpp"

namespace ui
{
    class NewAppView : public View
    {
    public:
        NewAppGameView(NavigationView &nav);
        std::string title() const override { return "Block Game"; };

    private:
        void update();
        MessageHandlerRegistration message_handler_update{
            Message::ID::DisplayFrameSync,
            [this](const Message *const) {
                this->update();
            }};
    };
} 