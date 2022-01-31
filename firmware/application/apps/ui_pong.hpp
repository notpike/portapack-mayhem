#include "ui.hpp"
#include "ui_widget.hpp"
#include "ui_navigation.hpp"
#include "string_format.hpp"


namespace ui
{
    enum ballDirection{STOP = 0, UP = 1, UP_RIGHT = 2, UP_LEFT = 3, DOWN = 4, DOWN_RIGHT = 5, DOWN_LEFT = 6};

    class PongView : public View                          // App class declaration
    {
    public:

        // Public declarations
    	void focus() override;                            // ui::View function override

        PongView(NavigationView &nav);                    // App class init function declaration
        std::string title() const override { 
            return "PONG!";                               // App Title
        };

    private:

        // Private declarations
        void update();                                      // Function declaration
        void player_two_update(int x);
        void ball_update(int x, int y);


        MessageHandlerRegistration message_handler_update{  // Example, not required: MessageHandlerRegistration class
            Message::ID::DisplayFrameSync,                  // relays machine states to your app code. Every time you 
            [this](const Message *const) {                  // get a  DisplayFrameSync message the update() function will
                this->update();                             // be triggered. 
            }
        };

        // Variables
        uint32_t player_one_x = 90; 
        uint32_t player_two_x = 90; 

        // Widgets
        // Note: Usable screen space is 240x304px
        // Note: Each char takes up 8x8px so you can multiply 
        //       the amount of spaces and rows you want by 8.
        //       This gives you 30x38 char

        Rectangle ball{
            {110, 240, 20, 20}, 
            ui::Color::white(),  
        };

        Rectangle ball_negitive{
            {0, 40, 240, 264}, 
            ui::Color::black(),  
        };
        
        NumberField player_one_control{
            {0, 304},          // Coordinates are: int:x (px), int:y (px)
            3,                 // Length
            {0, 180},          // MIN -> MAX (240-60)
            9,                 // 20 positions on the board
            '0',               // Fill Char
            false              // Can Loop
        };
        
        Rectangle player_one{
            {90, 264, 60, 20}, 
            ui::Color::white(), 
        };

        Rectangle player_one_negitive{
            {0, 264, 0, 20}, 
            ui::Color::black(), 
        };

        Rectangle player_two{
            {90, 20, 60, 20}, 
            ui::Color::white(), 
        };

        Rectangle player_two_negitive{
            {0, 264, 0, 20}, 
            ui::Color::black(), 
        };
    };

    class Ball {
        public:
            Ball(int posX, int posY, bool posession) {
                x = posX;
                y = posY;
                my_ball = posession;
                direction = STOP;
            }

            void Ball::set_x(int posX) {
                x = posX;
            }

            void Ball::set_Y(int posY) {
                y = posY;
            }

            int Ball::get_x() {
                return x;
            } 

            int Ball::get_y() {
                return y;
            }

            void Ball::set_posession(bool posession) {
                my_ball = posession;
            }

            bool Ball::get_posession() {
                return my_ball;
            }

            void Ball::set_direction(ballDirection dir) {
                direction = dir;
            }

            ballDirection Ball::get_direction() {
                return direction;
            }

        private:
            int x, y;
            bool my_ball;
            ballDirection direction;
    };
} 