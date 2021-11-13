namespace main_menu
{
    void exit(main_gui_info *gui_info)
    {
        gui_info->window->close();
    }

    void start(main_gui_info *gui_info)
    {
        *gui_info->name = "game";
        game::game(gui_info);
    }
}
namespace game
{
    void pause(main_gui_info* gui_info)
    {
        constants::old_delta_time = constants::delta_time;
        *gui_info->name = "pause";
        pause::pause(gui_info);
    }
    void game_over(main_gui_info* gui_info)
    {
        *gui_info->name = "game_over";
        game_over::game_over(gui_info);
    }

}
namespace pause
{
    void resume(main_gui_info* gui_info)
    {
        *gui_info->name = "game";
    }
    void exit(main_gui_info* gui_info)
    {
        *gui_info->name = "main_menu";
    }
}
