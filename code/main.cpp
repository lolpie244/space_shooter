#include "library.h"
#define _GLIBCXX_DEBUG
using namespace sf;
using namespace std;
namespace constants
{
    double window_coef;
    double delta_time;
    double old_delta_time = -1;
    int score;
    time_t rand_time;
    void set_delta_time(int a)
    {
        delta_time = a / 4;
        if (constants::old_delta_time != -1)
        {
            constants::delta_time = constants::old_delta_time;
            constants::old_delta_time = -1;
        }
    }
    void set_window_coef()
    {
        score = 0;
        rand_time = time(0);
        srand(time(0));
        constants::window_coef = min(VideoMode::getDesktopMode().width / 1920.0, VideoMode::getDesktopMode().height / 1080.0);
    }
}


#include "headers.h"


int main()
{

    RenderWindow window(VideoMode::getDesktopMode(), "GAME", Style::Fullscreen);
    window.setFramerateLimit(240);
    constants::set_window_coef();
    main_gui_info gui_info(&window);
    main_menu::main(&gui_info);
}
