#include "../headers/logic.h"

void main_player_Behavior(GameLogic *logic, GameWindow *window, GameDev *dev, GameInput *input)
{
  if (logic->players[0].is_spawned == 0)
  {
    spawn_Players(logic, window);
    logic->players[0].is_spawned = 1;
  }

  aim_Input(logic, window, input);
  move_Input(logic, window, input);

  if (logic->players[0].is_moving == 1)
  {
    move_player(&(logic->players[0]), logic->obstacles, logic->obstacle_num, dev->deltaTime);
  }
}

void bird_Behavior(GameLogic *logic, GameWindow *window, GameDev *dev)
{
  // spawn
  float target_distance_walked = 10000; ///TODO: make it dependent on stage

  if (logic->players[0].distance_walked - (logic->enemy_types[0].num_spawned * target_distance_walked) > target_distance_walked)
  {
    spawn_on_edge(logic, window, 0);
  }

  for (int i = 0; i < logic->enemy_num; i++)
  {
    if (strcmp(logic->enemies[i].name, "BIRD") == 0)
    {
      logic->enemies[i].action_ang = get_ang(logic->enemies[i].coords, logic->players[0].coords);

      // attack
      ///TODO: make this a function
      if (get_distance(logic->enemies[i].coords, logic->players[0].coords) < logic->enemies[i].attack_range)
        logic->enemies[i].is_moving = 0;
      else
        logic->enemies[i].is_moving = 1;

      // move
      if (logic->enemies[i].is_moving == 1)
      {
        move_enemie(&(logic->enemies[i]), logic->players[0], logic->obstacles, logic->obstacle_num, dev->deltaTime);
      }
    }
  }
}

void sheep_Behavior(GameLogic *logic, GameWindow *window, GameDev *dev)
{
  // spawn
  // int herd_num = 1; ///TODO: make this stage dependent
  // int sheep_num;
  // if (logic->enemy_types[1].num_spawned == 0)
  // {
  //   for (int i = 0; i < herd_num; i++)
  //   {
  //     sheep_num = get_Random_Number(&(window->r), 2, 3);
  //     spawn_herd(logic, window, dev, 1, sheep_num);
  //   }
  // }

  // for (int i = 0; i < logic->enemy_num; i++)
  // {
  //   if (strcmp(logic->enemies[i].name, "SHEEP") == 0)
  //   {
  //     // alert herd
  //     alert_herd(logic, &(logic->enemies[i]), 250);

  //     // roam
  //     if (logic->enemies[i].is_alerted == 0)
  //     {
  //       roam(&(logic->enemies[i]), window, dev);
  //     }
  //     else if (logic->enemies[i].is_alerted == 1)
  //     {
  //       // get player angle
  //       logic->enemies[i].action_ang = get_ang(logic->enemies[i].coords, logic->players[0].coords);

  //       // move
  //       if (logic->enemies[i].is_moving == 1)
  //       {
  //         move_enemie(&(logic->enemies[i]), logic->obstacles, logic->obstacle_num, dev->deltaTime);
  //       }
  //     }
  //   }
  // }
}

void melee_skeleton_Behavior(GameLogic *logic, GameWindow *window, GameDev *dev)
{
  ///////////////////////////////////////////////
  ////////////// --- spawn ---
  ///////////////////////////////////////////////
}

void archer_skeleton_Behavior(GameLogic *logic, GameWindow *window, GameDev *dev)
{
}

void obstacles_Behavior(GameLogic *logic, GameWindow *window, GameDev *dev)
{
  if (logic->obstacle_types[0].num_spawned == 0)
  {
    spawn_Obstacle(logic, window, 0);
  }
}

void update_Cam_Coords(GameLogic *logic, GameWindow *window, In_Game_UI *ui)
{
  // REAL system
  logic->cam_Coords.w = 2560;
  logic->cam_Coords.h = logic->cam_Coords.w * (float)9 / 16;
  logic->cam_Coords.x = logic->players[0].coords.x - logic->cam_Coords.w / 2;
  logic->cam_Coords.y = logic->players[0].coords.y + logic->cam_Coords.h / 2;

  if (logic->cam_Coords.x + logic->cam_Coords.w > logic->MAP_Map_Coords.w / 2)
  {
    logic->cam_Coords.x = (logic->MAP_Map_Coords.w / 2) - logic->cam_Coords.w;
  }
  else if (logic->cam_Coords.x < -logic->MAP_Map_Coords.w / 2)
  {
    logic->cam_Coords.x = -logic->MAP_Map_Coords.w / 2;
  }

  if (logic->cam_Coords.y - logic->cam_Coords.h < -logic->MAP_Map_Coords.h / 2)
  {
    logic->cam_Coords.y = (-logic->MAP_Map_Coords.h / 2) + logic->cam_Coords.h;
  }
  else if (logic->cam_Coords.y > logic->MAP_Map_Coords.h / 2)
  {
    logic->cam_Coords.y = logic->MAP_Map_Coords.h / 2;
  }

  // MAP system
  logic->MAP_Cam_Coords.w = logic->cam_Coords.w;
  logic->MAP_Cam_Coords.h = logic->cam_Coords.h;
  logic->MAP_Cam_Coords.x = logic->MAP_Origin_Coords.x + logic->cam_Coords.x;
  logic->MAP_Cam_Coords.y = logic->MAP_Origin_Coords.y - logic->cam_Coords.y;

  // SDL system
  ui->src_Map_Coords.x = logic->MAP_Cam_Coords.x;
  ui->src_Map_Coords.y = logic->MAP_Cam_Coords.y;
  ui->src_Map_Coords.w = logic->MAP_Cam_Coords.w;
  ui->src_Map_Coords.h = logic->MAP_Cam_Coords.h;

  // cam ratio
  logic->CAM_REAL_Cam_w_Ratio = (float)window->w / logic->cam_Coords.w;
}

void update_Minimap_Coords(GameLogic *logic, GameWindow *window, In_Game_UI *ui)
{
  float x_ratio = 0;
  float y_ratio = 0;

  // minimap background
  ui->minimap_coords.w = window->w * 0.2;
  ui->minimap_coords.h = window->h * 0.2;
  ui->minimap_coords.x = window->w - ui->minimap_coords.w * 1.03;
  ui->minimap_coords.y = ui->minimap_coords.h * 0.04;

  // minimap frame
  ui->minimap_frame_coords.w = ui->minimap_coords.w * 1.06;
  ui->minimap_frame_coords.h = ui->minimap_coords.h * 1.08;
  ui->minimap_frame_coords.x = ui->minimap_coords.x - (ui->minimap_frame_coords.w - ui->minimap_coords.w) / 2;
  ui->minimap_frame_coords.y = ui->minimap_coords.y - (ui->minimap_frame_coords.h - ui->minimap_coords.h) / 2;

  // camera
  if (logic->MAP_Cam_Coords.x != 0)
    x_ratio = logic->MAP_Cam_Coords.x / logic->MAP_Map_Coords.w;
  else
    x_ratio = 0;

  if (logic->MAP_Cam_Coords.y != 0)
    y_ratio = logic->MAP_Cam_Coords.y / logic->MAP_Map_Coords.h;
  else
    y_ratio = 0;

  ui->minimap_camera_coords.w = ((float)logic->cam_Coords.w / logic->MAP_Map_Coords.w) * ui->minimap_coords.w;
  ui->minimap_camera_coords.h = ((float)logic->cam_Coords.h / logic->MAP_Map_Coords.h) * ui->minimap_coords.h;
  ui->minimap_camera_coords.x = ui->minimap_coords.x + ui->minimap_coords.w * x_ratio + 1;
  ui->minimap_camera_coords.y = ui->minimap_coords.y + ui->minimap_coords.h * y_ratio;

  // players
  for (int i = 0; i < 2; i++)
  {
    x_ratio = fabs(-logic->MAP_Map_Coords.w / 2 - logic->players[i].coords.x) / logic->MAP_Map_Coords.w;
    y_ratio = fabs(logic->MAP_Map_Coords.h / 2 - logic->players[i].coords.y) / logic->MAP_Map_Coords.h;
    ui->minimap_players_coords[i].w = 3 * window->win_width_ratio;
    ui->minimap_players_coords[i].h = 3 * window->win_width_ratio;
    ui->minimap_players_coords[i].x = ui->minimap_coords.x + ui->minimap_coords.w * x_ratio - ui->minimap_players_coords[i].w / 2;
    ui->minimap_players_coords[i].y = ui->minimap_coords.y + ui->minimap_coords.h * y_ratio - ui->minimap_players_coords[i].h / 2;
  }

  // enemies
  ui->enemies_num = logic->enemy_num;
  for (int i = 0; i < logic->enemy_num; i++)
  {
    x_ratio = fabs(-logic->MAP_Map_Coords.w / 2 - logic->enemies[i].coords.x) / logic->MAP_Map_Coords.w;
    y_ratio = fabs(logic->MAP_Map_Coords.h / 2 - logic->enemies[i].coords.y) / logic->MAP_Map_Coords.h;
    ui->minimap_enemies_coords[i].w = 3 * window->win_width_ratio;
    ui->minimap_enemies_coords[i].h = 3 * window->win_width_ratio;
    ui->minimap_enemies_coords[i].x = ui->minimap_coords.x + ui->minimap_coords.w * x_ratio - ui->minimap_enemies_coords[i].w / 2;
    ui->minimap_enemies_coords[i].y = ui->minimap_coords.y + ui->minimap_coords.h * y_ratio - ui->minimap_enemies_coords[i].h / 2;
  }
}

void set_collisions(GameLogic *logic)
{
  int collision_side = -1;
  int collision_num = 0;

  // player <-> player

  // player <-> enemies
  for (int i = 0; i < 2; i++)
  {
    for (int j = 0; j < logic->enemy_num; j++)
    {
      collision_side = check_collision(logic->players[i].checkpoints, logic->enemies[j].checkpoints, logic->players[i].coords, logic->enemies[j].coords);

      if (collision_side == -1)
      {
        collision_side = check_collision(logic->enemies[j].checkpoints, logic->players[i].checkpoints, logic->enemies[j].coords, logic->players[i].coords);
        if (collision_side != -1)
          collision_side = (collision_side + 2) % 4;
      }

      if (collision_side != -1)
      {
        collision_num = logic->players[i].collision_sides[collision_side].collision_num;
        logic->players[i].collision_sides[collision_side].collision_type[collision_num] = 2;
        logic->players[i].collision_sides[collision_side].collider_id[collision_num] = logic->enemies[j].id;

        collision_num = logic->enemies[j].collision_sides[(collision_side + 2) % 4].collision_num;
        logic->enemies[j].collision_sides[(collision_side + 2) % 4].collision_type[collision_num] = 1;
        logic->enemies[j].collision_sides[(collision_side + 2) % 4].collider_id[collision_num] = logic->players[i].id;

        (logic->enemies[j].collision_sides[(collision_side + 2) % 4].collision_num)++;
        (logic->players[i].collision_sides[collision_side].collision_num)++;
      }
    }
  }

  // player <-> obstacle
  for (int i = 0; i < 2; i++)
  {
    for (int j = 0; j < logic->obstacle_num; j++)
    {
      collision_side = check_collision(logic->players[i].checkpoints, logic->obstacles[j].checkpoints, logic->players[i].coords, logic->obstacles[j].coords);

      if (collision_side == -1)
      {
        collision_side = check_collision(logic->obstacles[j].checkpoints, logic->players[i].checkpoints, logic->obstacles[j].coords, logic->players[i].coords);
        if (collision_side != -1)
          collision_side = (collision_side + 2) % 4;
      }

      if (collision_side != -1)
      {
        collision_num = logic->players[i].collision_sides[collision_side].collision_num;
        logic->players[i].collision_sides[collision_side].collision_type[collision_num] = 3;
        logic->players[i].collision_sides[collision_side].collider_id[collision_num] = logic->obstacles[j].id;

        collision_num = logic->obstacles[j].collision_sides[(collision_side + 2) % 4].collision_num;
        logic->obstacles[j].collision_sides[(collision_side + 2) % 4].collision_type[collision_num] = 1;
        logic->obstacles[j].collision_sides[(collision_side + 2) % 4].collider_id[collision_num] = logic->players[i].id;

        (logic->obstacles[j].collision_sides[(collision_side + 2) % 4].collision_num)++;
        (logic->players[i].collision_sides[collision_side].collision_num)++;
      }
    }
  }

  // enemie <-> enemie
  for (int i = 0; i < logic->enemy_num; i++)
  {
    for (int j = i + 1; j < logic->enemy_num; j++)
    {
      collision_side = check_collision(logic->enemies[i].checkpoints, logic->enemies[j].checkpoints, logic->enemies[i].coords, logic->enemies[j].coords);

      if (collision_side == -1)
      {
        collision_side = check_collision(logic->enemies[j].checkpoints, logic->enemies[i].checkpoints, logic->enemies[j].coords, logic->enemies[i].coords);
        if (collision_side != -1)
          collision_side = (collision_side + 2) % 4;
      }

      if (collision_side != -1)
      {
        collision_num = logic->enemies[i].collision_sides[collision_side].collision_num;
        logic->enemies[i].collision_sides[collision_side].collision_type[collision_num] = 2;
        logic->enemies[i].collision_sides[collision_side].collider_id[collision_num] = logic->enemies[j].id;

        collision_num = logic->enemies[j].collision_sides[(collision_side + 2) % 4].collision_num;
        logic->enemies[j].collision_sides[(collision_side + 2) % 4].collision_type[collision_num] = 2;
        logic->enemies[j].collision_sides[(collision_side + 2) % 4].collider_id[collision_num] = logic->enemies[i].id;

        (logic->enemies[j].collision_sides[(collision_side + 2) % 4].collision_num)++;
        (logic->enemies[i].collision_sides[collision_side].collision_num)++;
      }
    }

    // enemie <-> obstacle
    for (int i = 0; i < logic->enemy_num; i++)
    {
      for (int j = 0; j < logic->obstacle_num; j++)
      {
        collision_side = check_collision(logic->enemies[i].checkpoints, logic->obstacles[j].checkpoints, logic->enemies[i].coords, logic->obstacles[j].coords);

        if (collision_side == -1)
        {
          collision_side = check_collision(logic->obstacles[j].checkpoints, logic->enemies[i].checkpoints, logic->obstacles[j].coords, logic->enemies[i].coords);
          if (collision_side != -1)
            collision_side = (collision_side + 2) % 4;
        }

        if (collision_side != -1)
        {
          collision_num = logic->enemies[i].collision_sides[collision_side].collision_num;
          logic->enemies[i].collision_sides[collision_side].collision_type[collision_num] = 3;
          logic->enemies[i].collision_sides[collision_side].collider_id[collision_num] = logic->obstacles[j].id;

          collision_num = logic->obstacles[j].collision_sides[(collision_side + 2) % 4].collision_num;
          logic->obstacles[j].collision_sides[(collision_side + 2) % 4].collision_type[collision_num] = 2;
          logic->obstacles[j].collision_sides[(collision_side + 2) % 4].collider_id[collision_num] = logic->enemies[i].id;

          (logic->obstacles[j].collision_sides[(collision_side + 2) % 4].collision_num)++;
          (logic->enemies[i].collision_sides[collision_side].collision_num)++;
        }
      }
    }
  }
}

void reset_collisions(GameLogic *logic)
{
  // players

  for (int i = 0; i < 2; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      logic->players[i].collision_sides[j].collision_num = 0;
    }
  }

  // enemeies
  for (int i = 0; i < logic->enemy_num; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      logic->enemies[i].collision_sides[j].collision_num = 0;
    }
  }

  // obstacles
  for (int i = 0; i < logic->obstacle_num; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      logic->obstacles[i].collision_sides[j].collision_num = 0;
    }
  }
}
