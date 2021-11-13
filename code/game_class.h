using namespace chrono;
using namespace std;
using namespace sf;
namespace game
{
    struct game_info
    {
        int fps_enemy;
        game_info(): fps_enemy(0){}
    };
    class game_object : public Object
    {
    protected:
        bool was_in_screen;
        pair<double, double> speed;
        double perspective_coef;
        bool is_rotate;
    public:
        game_object(){}
        game_object(Rect<double> new_rect, string new_texture, pair<int, int> new_speed = {0, 0}):
            Object(new_rect, new_texture), speed(new_speed)
        {
            was_in_screen = false;
            is_rotate = false;
            perspective_coef = 0;
            speed.first *= constants::window_coef;
            speed.second *= constants::window_coef;
        }
        void set_texture(string s)
        {
            texture_source = s;
            sprite.setTexture(textures::get_texture(s));
        }
        bool was_in_screen_check()
        {
            was_in_screen |= in_screen();
            return was_in_screen;
        }

        double& get_perspective_coef()
        {
            return perspective_coef;
        }
        pair<double, double>& get_speed()
        {
            return speed;
        };
        void draw(RenderTarget& target, RenderStates states) const
        {
            if(!in_screen())
                return;
            double x_k = sprite.getGlobalBounds().width * 0.03;
            double y_k = sprite.getGlobalBounds().height * 0.02;
            double x_1 = sprite.getGlobalBounds().left;
            double x_2 = sprite.getGlobalBounds().left + sprite.getGlobalBounds().width;
            double y_1 = sprite.getGlobalBounds().top;
            double y_2 = sprite.getGlobalBounds().top + sprite.getGlobalBounds().height;
            sf::VertexArray quads(sf::Quads, 4);

            quads[0].texCoords = sf::Vector2f(0, 0);
            quads[1].texCoords = sf::Vector2f(textures::textures[texture_source].getSize().x - 1, 0);
            quads[2].texCoords = sf::Vector2f(textures::textures[texture_source].getSize().x - 1, textures::textures[texture_source].getSize().y - 1);
            quads[3].texCoords = sf::Vector2f(0, textures::textures[texture_source].getSize().y - 1);
            if(is_rotate)
            {
                quads[0].texCoords = sf::Vector2f(0, textures::textures[texture_source].getSize().y - 1);
                quads[1].texCoords = sf::Vector2f(textures::textures[texture_source].getSize().x - 1, textures::textures[texture_source].getSize().y - 1);
                quads[2].texCoords = sf::Vector2f(textures::textures[texture_source].getSize().x - 1, 0);
                quads[3].texCoords = sf::Vector2f(0, 0);
            }
            quads[0].color = sprite.getColor();
            quads[1].color = sprite.getColor();
            quads[2].color = sprite.getColor();
            quads[3].color = sprite.getColor();

            quads[0].position = sf::Vector2f(x_1 + x_k * abs(perspective_coef), y_1 - y_k * perspective_coef);
            quads[1].position = sf::Vector2f(x_2 - x_k * abs(perspective_coef), y_1 + y_k * perspective_coef);
            quads[2].position = sf::Vector2f(x_2 - x_k * abs(perspective_coef), y_2 + y_k * perspective_coef);
            quads[3].position = sf::Vector2f(x_1 + x_k * abs(perspective_coef), y_2 - y_k * perspective_coef);
//            if(texture_source == "textures/game/enemy_ship/1.png" ||
//                    texture_source == "textures/game/enemy_ship/2.png")
//                            cout << perspective_coef << ' ' << texture_source << ' ' << quads[0].position.x << ' ' << quads[0].position.y << '\n';
            target.draw(quads, &textures::textures[texture_source]);
        }
    };
    class engine : public game_object
    {
        pair<double, double> cord;
        game_object* obj;
        int engine_fps;
        int engine_type;
        pair<double, double> standart_scale;
    public:
        engine(pair<double, double> new_cord, game_object* new_obj, int rot = 0): cord(new_cord), obj(new_obj),
                game_object(Rect<double>(0, 0, 60.0, 61.0), "textures/game/engine/1.png")
        {
            perspective_coef = 0;
            is_rotate = rot;
            cord.first = cord.first * obj->get_sprite().getGlobalBounds().width;
            cord.second = cord.second * obj->get_sprite().getGlobalBounds().height;
            engine_fps = 0;
            engine_type = 1;
            standart_scale = {sprite.getScale().x, sprite.getScale().y};
        }
        void change_texture(int fps)
        {
            sprite.setOrigin(sprite.getOrigin().x, 0);
            sprite.setPosition(obj->get_sprite().getGlobalBounds().left + cord.first, obj->get_sprite().getGlobalBounds().top + cord.second);
            perspective_coef = obj->get_perspective_coef();
            if(fps - engine_fps > 15 / constants::delta_time)
            {
                engine_type = engine_type % 3 + 1;
                engine_fps = fps;
                texture_source = "textures/game/engine/" + to_string(engine_type) + ".png";
                sprite.setTexture(textures::get_texture(texture_source));
            }
            if(is_rotate)
                sprite.setRotation(180);
        }
        void change_scale(double scale)
        {
            sprite.setScale(scale * standart_scale.first, scale * standart_scale.second);
        }
    };
    class ship : public game_object
    {
    protected:
        double hp;
        int type;
        int points;
        bool had_damage;
        int take_damage_fps;
    public:
        int bullet_speed;
        int bullet_fps;
        vector<pair<double, double>> bullet_cords;
        vector<engine> engines;
        ship(){
            rect = *(new Rect<double>(0, 0, 0, 0));
            texture_source = "textures/empty.png";
        }
        ship(Rect<double> new_rect, string new_texture, pair<int, int> new_speed ):
                game_object(new_rect, new_texture, new_speed){
            hp = 200;
            take_damage_fps = -100;
            had_damage = false;
            bullet_speed = 30;
            points = 0;
            bullet_fps = 0;
        }
        int get_points()
        {
            return points;
        }
        int get_hp()
        {
            return hp;
        }
        virtual void take_damage(double damage, bool f = false)
        {
            //cout << "HP "<< hp << '\n';
            hp -= damage;
            if(hp < 0)
            {
                //cout << "KILL\n";
                this->sprite.setPosition(-10000 * constants::window_coef, -10000 * constants::window_coef);
                this->rect = Rect<double>(-10000 * constants::window_coef, -10000 * constants::window_coef, 1, 1);
                this->speed = {0, 0};
                this->sprite.setScale(0.000001, 0.00001);
                if(f)
                    constants::score += this->points;
            }
            had_damage = 1;
        }
        void add_bullet_cords(pair<double, double> cords)
        {
            bullet_cords.push_back({sprite.getGlobalBounds().width * cords.first, sprite.getGlobalBounds().height * cords.second});
        };
        void add_engine_cords(pair<double, double> cords, bool is_rotate = false)
        {
            engines.push_back(engine(cords, this, is_rotate));
        }
        void engine_draw(main_gui_info* gui_info)
        {
            if(had_damage)
            {
                had_damage = false;
                take_damage_fps = gui_info->fps;
            }
            if(gui_info->fps - take_damage_fps < 10 / constants::delta_time)
            {
                sprite.setColor(Color::Red);
            }
            else
                sprite.setColor(Color::White);
            for(auto& i : engines)
            {
                i.change_texture(gui_info->fps);
                gui_info->window->draw(i);
            }
        }

    };
    class enemy_ship : public ship
    {
        int type;
        bool direction;
        int move_fps;
        pair<double, double> start;
        game_elements* game_element;
    public:
        double(*function)(double x);
        int function_type;
        int bullets_count;
        int max_bullets_count;
        int bullets_cooldown;
        int last_bullets;
        enemy_ship(game_elements* new_game_elements, main_gui_info* gui_info) : ship()
        {
            take_damage_fps = -100;
            was_in_screen = false;
            is_rotate = false;
            perspective_coef = 0;
            game_element = new_game_elements;
            last_bullets = 0;
            type = random_int(1, 100) % 2 + 1;
            function_type = random_int(1, 100) % 2;
            bullets_count = 0;
            speed = {random_double(0.5, 2), random_double(0.3, 1.8)};
            bullet_fps = gui_info->fps;
            double k = random_double(0.5, 1.5);
            if(type == 1)
            {
                max_bullets_count = 5;
                bullets_cooldown = 600;
                hp = random_int(50, 100);
                points = hp * 3 / 10;
                bullet_speed = 50;
                texture_source = "textures/game/enemy_ship/1.png";
                rect.width = 110 * k;
                rect.height = 87 * k;
                Object* obj = new Object(rect, texture_source);
                sprite = obj->get_sprite();
                delete obj;
                add_bullet_cords({0.4, 1});
                add_bullet_cords({0.6, 1});
                add_engine_cords({0.3, 0.35}, 1);
                add_engine_cords({0.7, 0.35}, 1);
            }
            if(type == 2)
            {
                bullets_cooldown = 350;
                max_bullets_count = 5;
                hp = random_int(100, 150);
                points = hp / 10;
                texture_source = "textures/game/enemy_ship/2.png";
                bullet_speed = 50;
                rect.width = 110 * k;
                rect.height = 87 * k;
                Object* obj = new Object(rect, texture_source);
                sprite = obj->get_sprite();
                delete obj;
                reload_texture();
                add_bullet_cords({0.5, 1});
                add_engine_cords({0.5, 0.2}, 1);
            }
            if (function_type == 1)
            {
                rect.top = random_double(1, 1080 * constants::window_coef);
                if (random_int(1, 100) % 2 + 1 == 1)
                {
                    direction = 1;
                    rect.left = -15 * constants::window_coef;

                } else
                {
                    direction = 0;
                    rect.left = 1935 * constants::window_coef;
                }
            } else
            {
                rect.left = random_double(1, 1920 * constants::window_coef);
                if (random_int(1, 100) % 2 + 1 == 1) {
                    direction = 1;
                    rect.top = -15 * constants::window_coef;

                } else {
                    direction = 0;
                    rect.top = 1095 * constants::window_coef;
                }
            }
            if(direction == 0)
            {
                speed.first *= -1;
                speed.second *= -1;
            }
            Object* obj = new Object(rect, texture_source);
            *this = obj;
            delete obj;
            start.first = rect.left;
            start.second = rect.top;
            function = get_random_function({rect.left, rect.top}, function);
            //cout << "kekys " << texture_source << ' ' << rect.left << ' ' << rect.top << '\n';
            //take_damage(10000);
        }
        pair<double, double> get_start()
        {
            return start;
        }
        void check_to_kill();
        Object* operator=(Object* a)
        {
            sprite = a->get_sprite();
            rect = a->get_rect();
            return a;
        }
        void take_damage(double damage, bool f = false)
        {
            hp -= damage;
            //cout << "HP "<< hp << '\n';
            if(hp < 0)
            {
                //cout << "KILLL\n";
                this->sprite.setPosition(-10000 * constants::window_coef, -10000 * constants::window_coef);
                this->rect = Rect<double>(-10000 * constants::window_coef, -10000 * constants::window_coef, 1, 1);
                this->speed = {0, 0};
                this->function = [](double x){return -10000.0;};
                if(f)
                    constants::score += this->points;
            }
            had_damage = 1;
        }
        bool operator !=(enemy_ship a)
        {
            return a.speed != speed || a.direction != direction || a.rect != this->rect;
        }
    };

    class bullet : public game_object
    {
        set<ship*>* ship_to_destroy;
        set<bullet*>* bullets;
        int id;
        double damage;
    public:
        bullet(Rect<double> new_rect, string new_texture, pair<int, int> new_speed, double new_damage, set<ship*>* new_ship_to_destroy, set<bullet*>* new_bullets_to_destroy):
                game_object(new_rect, new_texture, new_speed), damage(new_damage), ship_to_destroy(new_ship_to_destroy), bullets(new_bullets_to_destroy){
            was_in_screen = true;
            id = random_int(0, 100000);
        }
        bullet(const bullet& a)
        {
            *this = a;
        }
        void try_to_kill()
        {
            if(!in_screen())
                return;
            for(auto ships : *ship_to_destroy)
                if(collision(ships->sprite_rect()) && ships->in_screen())
                {
                    //cout << "HPPP " << ships->get_hp() << '\n';
                    if(dynamic_cast<enemy_ship*>(ships))
                        dynamic_cast<enemy_ship*>(ships)->take_damage(damage, true); else
                    ships->take_damage(damage, true);
                    this->sprite.setPosition(-10000 * constants::window_coef, -10000 * constants::window_coef);
                    this->get_speed() = {0, 0};
                    return;
                }

            for(auto bullet : *bullets)
                if(collision(bullet) && bullet != this)
                {
                    bullet->sprite.setPosition(-10000 * constants::window_coef, -10000 * constants::window_coef);
                    this->sprite.setPosition(-10000 * constants::window_coef, -10000 * constants::window_coef);
                    this->speed = {0, 0};
                    bullet->speed = {0, 0};
                    return;
                }
        }
        bool operator!=(const bullet& a)
        {
            return id != a.id;
        }
        bool operator<(bullet a) const
        {
            return id < a.id;
        }
    };
    class game_elements
    {
        void to_draw(RenderTarget* window, game_object* obj)
        {
            window->draw(*obj);
        }
    public:
        game_elements()
        {
            enemy_ships = new set<enemy_ship*>;
            enemy_ships_to_destroy = new set<ship*>;
            bullets = new set<bullet*>;
        }
        ship* player_ship;
        set<enemy_ship*>* enemy_ships;
        set<ship*>* enemy_ships_to_destroy;
        set<bullet*>* bullets;
        void set_ship(ship* new_ship)
        {
            player_ship = new_ship;
        }
        void add_enemy_ship(enemy_ship* new_ship)
        {
            enemy_ships->insert(new_ship);
            enemy_ships_to_destroy->insert(new_ship);
        }
        void add_bullet(bullet* new_bullet)
        {
            bullets->insert(new_bullet);
        }
        void draw(main_gui_info* gui_info)
        {
            player_ship->engine_draw(gui_info);
            to_draw(gui_info->window, player_ship);
            vector<enemy_ship*> ship_to_delete;
            vector<bullet*> bullets_to_delete;

            for(auto i : *enemy_ships)
                if (i->was_in_screen_check() && !i->in_screen())
                {
                    //cout << "NO WAIT\n";
                    ship_to_delete.push_back(i);
                }
            for(auto i : *bullets)
                if (i->was_in_screen_check() && !i->in_screen())
                    bullets_to_delete.push_back(i);

            for(auto i : bullets_to_delete)
            {
                bullets->erase(i);
                delete i;
            }
            for(auto i : ship_to_delete)
            {
                enemy_ships->erase(i);
                enemy_ships_to_destroy->erase(i);
                delete i;
            }
            //cout << bullets->size() << ' ' << enemy_ships->size() << '\n';
            for(auto i : *enemy_ships)
            {
                i->engine_draw(gui_info);
                to_draw(gui_info->window, i);
            }
            for(auto i : *bullets){
                //cout << i->get_sprite().getGlobalBounds().left << ' ' << i->get_sprite().getGlobalBounds().top << '\n';
                to_draw(gui_info->window, i);}
        }
        ~game_elements()
        {
            cout << "DELETE IT ALL\n";
            delete player_ship;
            for(auto i : *enemy_ships)
                delete i;
            for(auto i : *bullets)
                delete i;
            delete enemy_ships;
            delete bullets;
            delete enemy_ships_to_destroy;
        }
    };
    void enemy_ship::check_to_kill()
    {
        if(this->get_sprite().getGlobalBounds().left == -10000*constants::window_coef)
        {
            //cout <<"kek\n";
            return;}
        if(this->collision(game_element->player_ship))
        {
            game_element->player_ship->take_damage(20);
            this->take_damage(10000, 1);
        }
        for(auto i : *game_element->enemy_ships)
            if(this->collision(i) && i != this)
            {
                i->take_damage(10000);
                take_damage(10000);
            }

    }
}