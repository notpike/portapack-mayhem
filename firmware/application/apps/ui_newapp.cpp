#include "ui_newapp.hpp"
#include "portapack.hpp"
#include <cstring>

using namespace portapack;

namespace ui
{

    // Main 
    NewAppView::NewAppView(NavigationView &nav)
    {
     
        // Add pointers for widgets
        add_children({
            &label_progress,
            &field_progress,
            &my_progressBar,
            &live_timestamp,
        });

        // Set max for progress bar
        my_progressBar.set_max(PROGRESS_MAX);

        // When NumberField is changed
        field_progress.on_change = [this](int32_t v) {
            progress = v;
            my_progressBar.set_value(progress);
        };

        //DateTime
        live_timestamp.set_seconds_enabled(true);

    }

    void NewAppView::update()
    {
    }
}