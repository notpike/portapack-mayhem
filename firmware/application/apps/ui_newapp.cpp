#include "ui_newapp.hpp"
#include "portapack.hpp"
#include <cstring>

using namespace portapack;

namespace ui
{
    // Default selection to button_helloWorld when app starts
    void NewAppView::focus() {
	    button_helloWorld.focus();
    }

    // Application Main 
    NewAppView::NewAppView(NavigationView &nav)
    {
     
        // Add pointers for widgets
        add_children({
            &button_helloWorld,
            &label_progress,
            &numberField_progress,
            &progressBar_progress,
            &timestamp,
        });

        // Set max for progress bar
        progressBar_progress.set_max(PROGRESS_MAX);

        // Button Logic
        button_helloWorld.on_select = [this](Button &){
            if(progress < 100) { 
                numberField_progress.set_value(100);   // Because numberField_progress has an on_change function,
            } else {                                   // progressBar_progress will update automatically.
                numberField_progress.set_value(0);
            }
        };                                                  

        // When NumberField is changed
        numberField_progress.on_change = [this](int32_t v) {
            progress = v;
            progressBar_progress.set_value(progress);
        };

        // DateTime enable seconds
        timestamp.set_seconds_enabled(true);
    }

    void NewAppView::update()
    {
    }
}