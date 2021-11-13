namespace game_over
{
    template<typename T>
    void interface(UI_elements <T> &gui_elements, main_gui_info *game_info) {
        gui_elements.add_button(Rect<double>(960, 714, 362, 73), "textures/UI/menu.png", pause::exit);
        gui_elements.add_object(Rect<double>(960, 540, 1920, 1080), "textures/UI/dark_blur.png");
        gui_elements.add_object(Rect<double>(960, 550, 626, 226), "textures/UI/your score.png");
        gui_elements.add_object(Rect<double>(960, 347, 355, 157), "textures/UI/death.png");

        gui_elements.add_object(new gui::Text(to_string(constants::score), Rect<double>(960, 570, 429, 155)));
        gui_elements.draw(game_info->window);
        game_info->window->display();
    }

    void game_over(main_gui_info *gui_info) {
        UI_elements<main_gui_info*> gui_elements;
        interface(gui_elements, gui_info);

        while (gui_info->window->isOpen() && *gui_info->name == "game_over") {
            sf::Event event;
            while (gui_info->window->pollEvent(event)) {
                if (event.type == Event::Closed)
                    gui_info->window->close();
                if (event.type == Event::KeyReleased) {
                    if (event.key.code == sf::Keyboard::Escape)
                        pause::exit(gui_info);
                }
                if (event.type == Event::MouseButtonReleased)
                    click(gui_elements.button, Rect<double>(event.mouseButton.x, event.mouseButton.y, 1, 1), gui_info);
            }
        }
        gui_info->time_point = system_clock::now();
    }
}