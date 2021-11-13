template<typename T>
void empty_function(T);


namespace game
{
    void game(main_gui_info*);
    struct main_game_info;
    class game_elements;
    class game_object;
    class ship;
    class bullet;
    class game_info;

    void game_process(main_gui_info* gui_info, game_elements& game_elements, game_info& info);
}
namespace main_menu
{
    void main_menu(main_gui_info*);
}
namespace pause
{
    void pause(main_gui_info*);
}
namespace game_over
{
    void game_over(main_gui_info*);
}
