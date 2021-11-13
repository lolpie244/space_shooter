using namespace sf;
using namespace std;

template<typename T = int>
class Button : public Object
{
    string id;
public:
    void (*function)(T main_elements);
    Button(Rect<double> new_rects, string new_texture, void(*new_function)(T element) = empty_function<T>):
        Object(new_rects, new_texture), function(new_function){}
    Button(const Button& a): Object(Rect<double>(a.rect.left / constants::window_coef, a.rect.top / constants::window_coef,
                                                 a.rect.width / constants::window_coef, a.rect.height / constants::window_coef),
                                    a.texture_source), function(a.function){}
    string get_id()
    {
        return id;
    }
    Button operator=(Button a)
    {
        texture_source = a.texture_source;
        sprite = a.sprite;
        rect = a.rect;
        function = a.function;
    }

};
namespace gui
{
    class Text : public Object
    {
        sf::Text text;
        sf::Font font;
    public:
        Text(string new_text, Rect<double> new_rect, sf::Color new_color = sf::Color::White)
        {
            font.loadFromFile("textures/UI/font.TTF");
            text.setFont(font);
            new_rect.left *= constants::window_coef;
            new_rect.top *= constants::window_coef;
            new_rect.height *= constants::window_coef;
            new_rect.width *= constants::window_coef;
            double size = 100 * constants::window_coef;

            text.setString(new_text);
            text.setCharacterSize(size);
            text.setFillColor(new_color);
            text.setOrigin(text.getGlobalBounds().width / 2.0, text.getGlobalBounds().height / 2.0);
            text.setPosition(new_rect.left, new_rect.top);
        }
        void draw(RenderTarget& target, RenderStates states) const
        {
//            string s = text.getString();
            //cout << "DRAW THIS " << s << '\n';
            target.draw(text);
        }
    };
    string load_from_file(string name)
    {
        string s;
        ifstream is(name);
        is >> s;
        is.close();
        return s;
    }
}
template<typename T>
struct UI_elements
{
    vector<Button<T>> button;
    vector<Object*> object_1;
    vector<Object> object_2;
    UI_elements()
    {}
    void add_button(Rect<double> new_rects, string new_texture, void(*new_function)(T element) = empty_function<T>)
    {
        button.push_back(Button<T>(new_rects, new_texture, new_function));
    }
    void add_object(Object* a)
    {
        object_1.push_back(a);
    }
    void add_object(Rect<double> new_rects, string new_texture)
    {
        object_2.push_back(Object(new_rects, new_texture));
    }
    void draw(RenderTarget* window)
    {
        for(auto i : object_2)
            window->draw(i);
        for(auto i : object_1)
            if(dynamic_cast<gui::Text*>(i) != nullptr)
                window->draw(*dynamic_cast<gui::Text*>(i));
            else
                window->draw(*i);
        for(auto i : button)
            window->draw(i);
    }
};

template<typename T>
T* get_function(vector<T> button, Rect<double> cords)
{
    for(int i = button.size() - 1; i >= 0; i--)
        if(button[i].collision(cords))
            return &button[i];
    return nullptr;
}
template<typename T, typename P>
void click(vector<T> button, Rect<double> cords, P information)
{
    T* obj = get_function(button, cords);
    if(obj == nullptr)
        return;
    obj->function(information);
}
template<typename T>
void empty_function(T k)
{}