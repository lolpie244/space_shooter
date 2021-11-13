
using namespace chrono;
using namespace std;
namespace main_menu
{
    template<typename T>
    void interface(UI_elements<T>& gui_elements, main_gui_info* gui_info)
    {

        gui_elements.add_button(Rect<double>(960, 647.5, 544, 129), "textures/UI/exit.png", exit);
        gui_elements.add_button(Rect<double>(960, 475.5, 544, 129), "textures/UI/start.png", start);
        gui_elements.add_object(Rect<double>(960, 540, 1920, 1080), "textures/background.png");
    }
    void main(main_gui_info* gui_info)
    {
        gui_info->window->clear();
        UI_elements<main_gui_info*> gui_elements;
        interface(gui_elements, gui_info);

        while(gui_info->window->isOpen() && *gui_info->name == "main_menu") {
            sf::Event event;
            while (gui_info->window->pollEvent(event)) {
                if (event.type == Event::Closed)
                    gui_info->window->close();
                if (event.type == Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Escape)
                        gui_info->window->close();
                }
                if (event.type == Event::MouseButtonReleased)
                    click(gui_elements.button, Rect<double>(event.mouseButton.x, event.mouseButton.y, 1, 1), gui_info);
            }
            gui_info->window->clear();
            gui_elements.draw(gui_info->window);
            gui_info->window->display();
            constants::set_delta_time(duration_cast<std::chrono::milliseconds>(system_clock::now() - gui_info->time_point).count());
            gui_info->time_point = system_clock::now();
        }
    }

}