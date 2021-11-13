
using namespace sf;
using namespace std;

namespace textures
{
    map<string, Texture> textures;
    Texture& get_texture(string s)
    {
        if(textures.count(s) == 0)
            textures[s].loadFromFile(s);
        return textures[s];
    }
}


class Object : public Drawable
{
protected:
    Sprite sprite;
    string texture_source;
    Rect<double> rect;

    template<typename T = float>
    bool screen_check(pair<T, T> cords) const
    {
        return cords.first >= 0 && cords.first <= 1920 * constants::window_coef &&
                cords.second >= 0 && cords.second <= 1080 * constants::window_coef;
    }
public:
    Object(){
        texture_source = "";
    }
    Object(Rect<double> new_rect, string new_texture)
    {
        texture_source = new_texture;
        rect = new_rect;
        sprite.setTexture(textures::get_texture(texture_source));
        sprite.setOrigin(sprite.getGlobalBounds().width / 2.0, sprite.getGlobalBounds().height / 2.0);
        sprite.setScale(constants::window_coef * new_rect.width / sprite.getGlobalBounds().width, constants::window_coef * new_rect.height / sprite.getGlobalBounds().height);
        rect.top *= constants::window_coef;
        rect.left *= constants::window_coef;
        rect.width = sprite.getGlobalBounds().width;
        rect.height = sprite.getGlobalBounds().height;
        sprite.setPosition(rect.left, rect.top);
    }
    Object(const Object& a): Object(Rect<double>(a.rect.left / constants::window_coef, a.rect.top / constants::window_coef,
                                                 a.rect.width / constants::window_coef, a.rect.height / constants::window_coef),
                                    a.texture_source){}
    Sprite& get_sprite()
    {
        return sprite;
    }
    Rect<double>& get_rect()
    {
        return rect;
    }
    void reload_texture()
    {
        sprite.setTexture(textures::get_texture(texture_source));
        //cout << texture_source << '\n';
    }
    Rect<float> sprite_rect()
    {
        return sprite.getGlobalBounds();
    }
    virtual void draw(RenderTarget& target, RenderStates states) const
    {
        target.draw(sprite, states);
    }
    bool in_screen() const
    {
        return screen_check({sprite.getGlobalBounds().left, sprite.getGlobalBounds().top}) ||
                screen_check({sprite.getGlobalBounds().left + sprite.getGlobalBounds().width, sprite.getGlobalBounds().top + sprite.getGlobalBounds().height}) ||
                screen_check({sprite.getGlobalBounds().left + sprite.getGlobalBounds().width, sprite.getGlobalBounds().top}) ||
                screen_check({sprite.getGlobalBounds().left, sprite.getGlobalBounds().top + sprite.getGlobalBounds().height});
    }
    template<typename T>
    bool collision(const Rect<T> rect) const
    {
        return static_cast<Rect<T>>(this->sprite.getGlobalBounds()).intersects(rect);
    }
    bool collision(const Object* obj) const
    {
        //cout << this->texture_source << ' ' << obj->texture_source << '\n';
        return this->sprite.getGlobalBounds().intersects(obj->sprite.getGlobalBounds()) ||
                obj->sprite.getGlobalBounds().intersects(this->sprite.getGlobalBounds());
    }
    Object operator=(Object a)
    {
        texture_source = a.texture_source;
        sprite = a.sprite;
        rect = a.rect;
    }

};

struct main_gui_info
{
    RenderWindow* window;
    string* name;
    int fps;
    std::chrono::system_clock::time_point time_point;
    main_gui_info(RenderWindow* new_window = nullptr)
    {
        time_point = std::chrono::system_clock::now();
        fps = 0;
        window = new_window;
        name = new string("main_menu");
    }
};
