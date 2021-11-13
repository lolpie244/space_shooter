namespace game
{
    template<typename T>
    void create_interface(UI_elements<T>& gui_elements, main_gui_info* gui_info)
    {
        gui_elements.add_button(Rect<double>(109, 110, 100, 100), "textures/UI/pause.png", pause);
        gui_elements.add_object(Rect<double>(960, 540, 1920, 1080), "textures/background.png");
    }

    void create_element(game_elements& game_elements, main_gui_info* gui_info)
    {
        game_elements.set_ship(new ship(Rect<double>(960, 800, 126, 128.8), "textures/game/player_ship/standart.png", {2.5, 2.5}));
        game_elements.player_ship->add_bullet_cords({0.5, 0});
        game_elements.player_ship->add_engine_cords({0.5, 0.95});
    }

    template <typename T>
    void event_check(main_gui_info* gui_info, UI_elements<T>& gui_elements, game_elements& game_elements)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            animation::player::to_left(game_elements.player_ship);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            animation::player::to_right(game_elements.player_ship);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            animation::player::to_up(game_elements.player_ship);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            animation::player::to_down(game_elements.player_ship);

        if (!sf::Keyboard::isKeyPressed(sf::Keyboard::D) && !sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            animation::player::to_stay(game_elements.player_ship);

        if (!sf::Keyboard::isKeyPressed(sf::Keyboard::W) && !sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            animation::player::to_stay_vertical(game_elements.player_ship);

        sf::Event event;
        while (gui_info->window->pollEvent(event))
        {
            if (event.type == Event::Closed)
                gui_info->window->close();

            if (event.type == Event::KeyReleased)
                if (event.key.code == sf::Keyboard::Escape)
                    game::pause(gui_info);

            if (event.type == Event::MouseButtonReleased)
                click(gui_elements.button, Rect<double>(event.mouseButton.x, event.mouseButton.y, 1, 1), gui_info);
        }
        //cout << "DELTA" << constants::delta_time << '\n';
    }
    void game_process(main_gui_info* gui_info, game_elements& game_elements, game_info& info)
    {
        if((gui_info->fps - info.fps_enemy) > random_int(100, 150) / constants::delta_time)
        {
            game_elements.add_enemy_ship(new enemy_ship(&game_elements, gui_info));
            info.fps_enemy = gui_info->fps;
        }
        if(!game_elements.player_ship->in_screen())
            game_over(gui_info);
        animation::create_bullet(gui_info, game_elements);
        for(auto i : *game_elements.enemy_ships)
            animation::enemy::move(i);
        for(auto i : *game_elements.bullets)
            animation::bullets::move(i);
    }
    template<typename T>
    void draw(main_gui_info* gui_info, UI_elements<T>& gui_elements, game_elements& game_elements)
    {
        gui_info->window->clear();
        gui_elements.draw(gui_info->window);
        game_elements.draw(gui_info);
        gui_info->window->display();
    }
}
