#include "ui_pong.hpp"
#include "portapack.hpp"
#include <cstring>

using namespace portapack;

namespace ui
{

    void PongView::focus() {                               // Default selection to button_helloWorld when app starts
        player_one_control.focus();
    }

    void PongView::player_two_update(int x) {
        player_two.set_parent_rect({x, 20, 60, 20}); // Update Position
        (x > player_two_x) ? player_two_negitive.set_parent_rect({0, 20, x, 20}) : player_two_negitive.set_parent_rect({x+60, 20, 180-x, 20}); //Hack to repaint screen
            
        player_two_x = x;
    }

    PongView::PongView(NavigationView &nav)              // Application Main 
    {
     
        add_children({                                       // Add pointers for widgets
            &ball,
            &player_one_control,
            &player_one_negitive,
            &player_one,

            &player_two,
            &player_two_negitive,
        });

        // Player One's Controls INIT
        player_one_control.visible(false);
        player_one_control.set_value(90);

        // Player One's Controls Logic
        player_one_control.on_change = [this](int32_t v) {          

            player_one.set_parent_rect({v, 264, 60, 20}); // Update Position
            (v > player_one_x) ? player_one_negitive.set_parent_rect({0, 264, v, 20}) : player_one_negitive.set_parent_rect({v+60, 264, 180-v, 20}); //Hack to repaint screen
            
            player_one_x = v;

        };

        // player_one_control.on_select {

        // };

    }

    void PongView::update()                                // Every time you get a DisplayFrameSync message this
    {                                                        // function will be ran.
         // Message code
    }
}