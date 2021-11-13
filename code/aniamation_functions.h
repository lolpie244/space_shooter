#pragma once
using namespace std;
namespace game {
    namespace animation {
        namespace player {
            void to_left(ship *obj, bool f = false) {
                if (obj->get_sprite().getGlobalBounds().left - obj->get_speed().first < 0
                    && !f)
                    return;
                if (obj->get_perspective_coef() < 5)
                    obj->get_perspective_coef() += 0.2 * constants::delta_time;
                obj->get_rect().left += -1 * obj->get_speed().first * constants::delta_time;
                obj->get_sprite().move(-1 * obj->get_speed().first * constants::delta_time, 0);
            }

            void to_right(ship *obj, bool f = false) {
                if (obj->get_sprite().getGlobalBounds().left + obj->get_sprite().getGlobalBounds().width +
                    obj->get_speed().first > 1920 * constants::window_coef
                    && !f)
                    return;
                if (obj->get_perspective_coef() > -5)
                    obj->get_perspective_coef() -= 0.2 * constants::delta_time;
                obj->get_rect().left += obj->get_speed().first * constants::delta_time;
                obj->get_sprite().move(obj->get_speed().first * constants::delta_time, 0);
            }

            void to_up(ship *obj, bool f = false) {
                if (obj->get_sprite().getGlobalBounds().top - obj->get_speed().second < 10 * constants::window_coef
                    && !f)
                    return;
                for (auto &i : obj->engines)
                    i.change_scale(1.3);
                obj->get_rect().top += (!f ? -1 : 1) * obj->get_speed().second * constants::delta_time;
                obj->get_sprite().move(0, (-1 * (!f) + 1 * f) * obj->get_speed().second * constants::delta_time);
            }

            void to_down(ship *obj, bool f = false) {
                if (obj->get_sprite().getGlobalBounds().top + obj->get_sprite().getGlobalBounds().height +
                    obj->get_speed().second > 1080 * constants::window_coef
                    && !f)
                    return;
                for (auto &i : obj->engines)
                    i.change_scale(0.7);
                obj->get_rect().top += (!f ? 0.4 : 1) * obj->get_speed().second * constants::delta_time;
                obj->get_sprite().move(0, (!f ? 0.4 : 1) * obj->get_speed().second * constants::delta_time);
            }

            void to_stay(ship *obj) {
                if (obj->get_perspective_coef() < 0)
                    obj->get_perspective_coef() = std::min(obj->get_perspective_coef() + 0.15 * constants::delta_time,
                                                           0.0);
                if (obj->get_perspective_coef() > 0)
                    obj->get_perspective_coef() = std::max(obj->get_perspective_coef() - 0.15 * constants::delta_time,
                                                           0.0);
            }

            void to_stay_vertical(ship *obj) {
                for (auto &i : obj->engines)
                    i.change_scale(1);
            }

            void destroy(game_object *obj) {

            }

            void create_bullet(Rect<double> cords, pair<double, double> speed, game_elements &game_elements) {
                cords.top /= constants::window_coef;
                cords.left /= constants::window_coef;
                game_elements.add_bullet(new bullet(cords, "textures/game/player_bullet.png", speed, 10,
                                                    game_elements.enemy_ships_to_destroy, game_elements.bullets));
            }
        }
        namespace bullets {
            void move(bullet *obj) {
                obj->get_sprite().move(obj->get_speed().first * constants::delta_time,
                                       obj->get_speed().second * constants::delta_time);
                obj->try_to_kill();
            }
        }
        namespace enemy {
            void create_bullet(Rect<double> cords, pair<double, double> speed, game_elements &game_elements) {
                cords.top /= constants::window_coef;
                cords.left /= constants::window_coef;
                set<ship*>* sett = new set<ship*>;
                sett->insert(game_elements.player_ship);
                game_elements.add_bullet(new bullet(cords, "textures/game/bullet.png", speed, 5, sett, game_elements.bullets));
            }

            void move(enemy_ship *ship) {
                //ship->reload_texture();
                ship->check_to_kill();
                if (ship->function_type == 1) {
                    double y = ship->function(ship->get_rect().left + ship->get_speed().first - ship->get_start().first) + ship->get_start().second;
                    ship->get_speed().second = (y - ship->get_rect().top) / constants::delta_time;
                    double x = ship->get_speed().first;
                    ship->get_speed().first = abs(x);
                    if (x == 0)
                        player::to_stay(ship);
                    else if (x < 0)
                        player::to_left(ship, 1);
                    else
                        player::to_right(ship, 1);

                    if (ship->get_speed().second == 0)
                        player::to_stay_vertical(ship);
                    else if (ship->get_speed().second < 0)
                        player::to_down(ship, 1);
                    else
                        player::to_up(ship, 1);

                    ship->get_speed().first = x;
                }
                else {
                    double x = ship->function(ship->get_rect().top + ship->get_speed().second - ship->get_start().second) + ship->get_start().first;
                    ship->get_speed().first = (x - ship->get_rect().left) / constants::delta_time;
                    double y = ship->get_speed().second;
                    x = ship->get_speed().first;
                    ship->get_speed().first = abs(ship->get_speed().first);
                    if (y == 0)
                        player::to_stay_vertical(ship);
                    else if (y < 0)
                        player::to_down(ship, 1);
                    else
                        player::to_up(ship, 1);
                    if (x == 0)
                        player::to_stay(ship);
                    else if (x < 0)
                        player::to_left(ship, 1);
                    else
                        player::to_right(ship, 1);

                    ship->get_speed().second = y;
                }
            }


        }

        void create_bullet(main_gui_info *gui_info, game_elements &game_elements) {
            if (gui_info->fps - game_elements.player_ship->bullet_fps >
                game_elements.player_ship->bullet_speed / constants::delta_time &&
                !sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
                for (auto i : game_elements.player_ship->bullet_cords) {
                    Rect<double> cords(game_elements.player_ship->sprite_rect().left + i.first,
                                       game_elements.player_ship->sprite_rect().top + i.second,
                                       16, 54);
                    game_elements.player_ship->bullet_fps = gui_info->fps;
                    animation::player::create_bullet(cords, {0, -5}, game_elements);
                }
            }
            for (auto &i : *game_elements.enemy_ships) {
                if (gui_info->fps - i->bullet_fps > i->bullet_speed / constants::delta_time && i->in_screen() &&
                    (gui_info->fps - i->last_bullets> i->bullets_cooldown / constants::delta_time) ){
                    i->bullets_count++;
                    for (auto j : i->bullet_cords) {
                        Rect<double> cords(i->sprite_rect().left + j.first, i->sprite_rect().top + j.second,
                                           16, 54);
                        animation::enemy::create_bullet(cords, {0, 3}, game_elements);
                    }
                    if (i->bullets_count >= i->max_bullets_count) {
                        i->bullets_count = 0;
                        i->last_bullets = gui_info->fps;
                    }
                    i->bullet_fps = gui_info->fps;
                }
            }
        }

    }
}