#include "ui_newapp.hpp"
#include "portapack.hpp"
#include <cstring>

using namespace portapack;

namespace ui
{
    NewAppView::NewAppView(NavigationView &nav)
    {
     
        add_children({
            &label_progress,
            &field_progress
        });

    }

    void NewAppView::update()
    {
    }
}