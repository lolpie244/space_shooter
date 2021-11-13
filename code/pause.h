namespace pause
{
    template<typename T>
    void interface(UI_elements <T> &gui_elements, main_gui_info *game_info) {
        gui_elements.add_button(Rect<double>(960, 455, 544, 129), "textures/UI/resume.png", resume);
        gui_elements.add_button(Rect<double>(960, 620, 544, 129), "textures/UI/exit.png", exit);
        gui_elements.add_object(Rect<double>(960, 540, 1920, 1080), "textures/UI/blur.png");
        gui_elements.add_object(Rect<double>(960, 150, 626, 226), "textures/UI/your score.png");

        gui_elements.add_object(new gui::Text(to_string(constants::score), Rect<double>(960, 170, 429, 155)));
        gui_elements.draw(game_info->window);
        game_info->window->display();
    }

    void pause(main_gui_info *gui_info) {
        UI_elements<main_gui_info*> gui_elements;
        interface(gui_elements, gui_info);

        while (gui_info->window->isOpen() && *gui_info->name == "pause") {
            sf::Event event;
            while (gui_info->window->pollEvent(event)) {
                if (event.type == Event::Closed)
                    gui_info->window->close();
                if (event.type == Event::KeyReleased) {
                    if (event.key.code == sf::Keyboard::Escape)
                        pause::resume(gui_info);
                }
                if (event.type == Event::MouseButtonReleased)
                    click(gui_elements.button, Rect<double>(event.mouseButton.x, event.mouseButton.y, 1, 1), gui_info);
            }
        }
    }
}