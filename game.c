
//Renderer

internal void
draw_rect_in_pixels(Game_Offscreen_Buffer *buffer, u32 color, s32 min_x, s32 min_y, s32 max_x, s32 max_y) {

    min_x = max(min_x, 0);
    min_y = max(min_y, 0);
    max_x = min(max_x, buffer->width);
    max_y = min(max_y, buffer->height);

    u8* row = (u8*)buffer->memory + min_x*sizeof(u32) + min_y*buffer->pitch;
    for (s32 y = min_y; y < max_y; y++) {
        u32 *pixel = (u32*)row;
        for (s32 x = min_x; x < max_x; x++) {
            *pixel++ = color;
        }
        row += buffer->pitch;
    }
}

internal void
clear(Game_Offscreen_Buffer *buffer, u32 color) {
    draw_rect_in_pixels(buffer, color, 0, 0, buffer->width, buffer->height);
}

internal void
draw_rect(Game_Offscreen_Buffer *buffer, u32 color, f32 x, f32 y, f32 half_size_x, f32 half_size_y) {

    //@Hardcoded to always keep the playing field area on screen, no matter the aspect ratio
    f32 scale = .01f;
    f32 relative_axis = (f32)buffer->height;
    if ((f32)buffer->width / (f32)buffer->height < 1.77f) {
        relative_axis = (f32)buffer->width;
        scale = .0056f;
    }

    half_size_x *= relative_axis * scale;
    half_size_y *= relative_axis * scale;
    x *= relative_axis * scale;
    y *= relative_axis * scale;

    x = x + buffer->width/2;
    y = y + buffer->height/2;

    s32 min_x = (u32)(x - half_size_x);
    s32 min_y = (u32)(y - half_size_y);
    s32 max_x = (u32)(x + half_size_x);
    s32 max_y = (u32)(y + half_size_y);

    draw_rect_in_pixels(buffer, color, min_x, min_y, max_x, max_y);

}

internal void
draw_number(Game_Offscreen_Buffer *buffer, u32 number, u32 color, f32 x, f32 y) {

    f32 at_x = x;

    do {
        u32 alg = number % 10;
        number /= 10;

        switch (alg) {
            case 0: {
                draw_rect(buffer, color, at_x - 2.f, y, .5f, 4.f);
                draw_rect(buffer, color, at_x + 2.f, y, .5f, 4.f);
                draw_rect(buffer, color, at_x, y + 4.f, 2.5f, .5f);
                draw_rect(buffer, color, at_x, y - 4.f, 2.5f, .5f);
            } break;

            case 1: {
                draw_rect(buffer, color, at_x + 2.f, y, .5f, 4.5f);
            } break;

            case 2: {
                draw_rect(buffer, color, at_x - 2.f, y-2.f, .5f, 2.f);
                draw_rect(buffer, color, at_x + 2.f, y+2.f, .5f, 2.f);
                draw_rect(buffer, color, at_x, y + 4.f, 2.5f, .5f);
                draw_rect(buffer, color, at_x, y, 2.5f, .5f);
                draw_rect(buffer, color, at_x, y - 4.f, 2.5f, .5f);
            } break;

            case 3: {
                draw_rect(buffer, color, at_x + 2.f, y, .5f, 4.f);
                draw_rect(buffer, color, at_x, y + 4.f, 2.5f, .5f);
                draw_rect(buffer, color, at_x, y, 2.5f, .5f);
                draw_rect(buffer, color, at_x, y - 4.f, 2.5f, .5f);
            } break;

            case 4: {
                draw_rect(buffer, color, at_x, y, 2.5f, .5f);
                draw_rect(buffer, color, at_x + 2.f, y, .5f, 4.5f);
                draw_rect(buffer, color, at_x - 2.f, y+2.5f, .5f, 2.f);
            } break;

            case 5: {
                draw_rect(buffer, color, at_x + 2.f, y-2.f, .5f, 2.f);
                draw_rect(buffer, color, at_x - 2.f, y+2.f, .5f, 2.f);
                draw_rect(buffer, color, at_x, y + 4.f, 2.5f, .5f);
                draw_rect(buffer, color, at_x, y, 2.5f, .5f);
                draw_rect(buffer, color, at_x, y - 4.f, 2.5f, .5f);
            } break;

            case 6: {
                draw_rect(buffer, color, at_x + 2.f, y-2.f, .5f, 2.f);
                draw_rect(buffer, color, at_x - 2.f, y, .5f, 4.f);
                draw_rect(buffer, color, at_x, y + 4.f, 2.5f, .5f);
                draw_rect(buffer, color, at_x, y, 2.5f, .5f);
                draw_rect(buffer, color, at_x, y - 4.f, 2.5f, .5f);
            } break;

            case 7: {
                draw_rect(buffer, color, at_x + 2.f, y, .5f, 4.5f);
                draw_rect(buffer, color, at_x, y + 4.f, 2.5f, .5f);
            } break;

            case 8: {
                draw_rect(buffer, color, at_x - 2.f, y, .5f, 4.f);
                draw_rect(buffer, color, at_x + 2.f, y, .5f, 4.f);
                draw_rect(buffer, color, at_x, y + 4.f, 2.5f, .5f);
                draw_rect(buffer, color, at_x, y - 4.f, 2.5f, .5f);
                draw_rect(buffer, color, at_x, y, 2.5f, .5f);
            } break;

            case 9: {
                draw_rect(buffer, color, at_x - 2.f, y+2.f, .5f, 2.f);
                draw_rect(buffer, color, at_x + 2.f, y, .5f, 4.f);
                draw_rect(buffer, color, at_x, y + 4.f, 2.5f, .5f);
                draw_rect(buffer, color, at_x, y, 2.5f, .5f);
                draw_rect(buffer, color, at_x, y - 4.f, 2.5f, .5f);
            } break;

            invalid_default_case;
        }
        at_x -= 7.f;
    } while (number > 0);
}


global_variable f32 player1_py;
global_variable f32 player1_dpy;
global_variable f32 player2_py;
global_variable f32 player2_dpy;
global_variable f32 player2_speed;
global_variable f32 ball_px;
global_variable f32 ball_py;
global_variable f32 ball_dpx;
global_variable f32 ball_dpy;
global_variable f32 ball_speed;
global_variable b32 is_initialized;
global_variable u32 player1_score;
global_variable u32 player2_score;
global_variable f32 current_play_points;

internal void
game_update_and_render (Game_Input *input, Game_Offscreen_Buffer *draw_buffer) {

    f32 initial_ball_speed = 80.f;
    f32 playing_field_x = 85.f;
    f32 playing_field_y = 48.f;
    f32 player_size_x = 2.5f;
    f32 player_size_y = 10.f;

    if (!is_initialized) {
        is_initialized = 1;
        ball_px = 0;
        ball_py = 0;
        ball_dpx = initial_ball_speed;
        ball_dpy = 0;
        current_play_points = 10.f;
        player2_speed = 80.f;
    }

    {
        f32 speed = 50.f;
        player1_dpy = 0.f;

        if (is_down(input->buttons[B_SPEED_UP])) {
            speed = 100.f;
        }
        if (is_down(input->buttons[B_MOVE_UP])) {
            if (player1_py < playing_field_y - player_size_y) {
                player1_dpy = speed;
            }
            if (player1_py < -playing_field_y + player_size_y){
                player1_py = -playing_field_y + player_size_y;
                player1_dpy = 0.f;
            }
        }
        if (is_down(input->buttons[B_MOVE_DOWN])) {
            if (player1_py > -playing_field_y + player_size_y) {
                player1_dpy = -speed;
            }
            if (player1_py < -playing_field_y + player_size_y){
                player1_py = -playing_field_y + player_size_y;
                player1_dpy = 0.f;
            }
        }
        player1_py += player1_dpy * input->last_dt;
    }

    {
        player2_dpy = (ball_py - player2_py) * 100.f;
        player2_dpy = min(player2_dpy, player2_speed);
        player2_dpy = max(player2_dpy, -player2_speed);
        player2_py += player2_dpy * input->last_dt;
        if (player2_py < -playing_field_y + player_size_y){
            player2_py = -playing_field_y + player_size_y;
            player2_dpy = 0.f;
        }
        if (player2_py > playing_field_y - player_size_y){
            player2_py = playing_field_y - player_size_y;
            player2_dpy = 0.f;
        }

    }

    ball_px += ball_dpx * input->last_dt;
    if (ball_dpx > 0) {
        ball_dpx += 10.f * input->last_dt;
    } else {
        ball_dpx -= 10.f * input->last_dt;
    }
    ball_py += ball_dpy * input->last_dt;

    if (ball_py > playing_field_y - 1.f) {
        ball_py = playing_field_y - 1.f;
        ball_dpy *= -1.f;
    } else if(ball_py < -playing_field_y + 1) {
        ball_py = -playing_field_y + 1.f;
        ball_dpy *= -1;
    }

    if (ball_px > 80.f - 2.5f - 1.f) { //@Hardcoded
        if ((ball_py >= (player2_py - 10.f)) && (ball_py <= (player2_py + 10.f))) {
            ball_dpx *= -1.f;
            ball_px = 80.f - 2.5f - 1.f; //@Hardcoded
            ball_dpy = (ball_py - player2_py) + player2_dpy;
            ++current_play_points;
        } else if (ball_px >= playing_field_x - 1) {
            ball_px = 0;
            ball_py = 0;
            ball_dpy = 0;
            ball_dpx = -initial_ball_speed;
            player2_score += (u32)current_play_points;
            current_play_points = 10.f;
        }
    } else if (ball_px < -80 + 2.5f + 1.f) { //@Hardcoded
        if ((ball_py >= (player1_py - 10.f)) && (ball_py <= (player1_py + 10.f))) {
            ball_dpx *= -1.f;
            ball_px = -80 + 2.5f + 1.f; //@Hardcoded
            ball_dpy = (ball_py - player1_py) + player1_dpy;
            ++current_play_points;
        } else if (ball_px <= -playing_field_x + 1) {
            ball_px = 0;
            ball_py = 0;
            ball_dpy = 0;
            ball_dpx = initial_ball_speed;
            player1_score += (u32)current_play_points;
            current_play_points = 10.f;
            player2_speed += current_play_points * 0.01f;
        }
    }

    clear(draw_buffer, 0x021077);
    draw_rect(draw_buffer, 0x000530, 0.f, 0.f, playing_field_x, playing_field_y);

    draw_rect(draw_buffer, 0x00ffff, -80.f, player1_py, player_size_x, player_size_y);
    draw_rect(draw_buffer, 0x00ffff, 80.f, player2_py, player_size_x, player_size_y);

    draw_rect(draw_buffer, 0xffff00, ball_px, ball_py, 1.f, 1.f);

    draw_number(draw_buffer, (u32)current_play_points, 0xaaaaaa, 0.f, 38.f);
    draw_number(draw_buffer, player1_score, 0xff6611, 20.f, 38.f);
    draw_number(draw_buffer, player2_score, 0xff6611, -20.f, 38.f);

}
