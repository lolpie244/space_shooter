
namespace game
{

    void game(main_gui_info* gui_info)
    {
        constants::score = 0;
        gui_info->window->clear();
        UI_elements<main_gui_info*> gui_elements;
        create_interface(gui_elements, gui_info);
        game::game_elements game_elements;
        create_element(game_elements, gui_info);

        game_info info;
        while(gui_info->window->isOpen() && *gui_info->name == "game")
        {
            if(constants::delta_time == 0)
                constants::delta_time = 1;
            gui_info->fps++;

            game_process(gui_info, game_elements, info);
            draw(gui_info, gui_elements, game_elements);


            event_check(gui_info, gui_elements, game_elements);event_check(gui_info, gui_elements, game_elements);
            constants::set_delta_time(duration_cast<std::chrono::milliseconds>(system_clock::now() - gui_info->time_point).count());
            gui_info->time_point = system_clock::now();
        }
    }
}