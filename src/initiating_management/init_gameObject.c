#include "../headers/init.h"

void init_GameWindow(GameWindow *window)
{

  // calculating and creating the dimentions of the main window
  SDL_Rect max_Window;
  int getBounds = SDL_GetDisplayBounds(0, &max_Window);
  if (getBounds != 0)
    lib_errorLog("failed at getting bounds", SDL_GetError());
  window->max_w = max_Window.w;
  window->max_h = max_Window.h;
  window->w = window->max_w / 2;
  window->h = window->max_h / 2;
  window->x = (window->max_w / 2) - (window->w / 2);
  window->y = (window->max_h / 2) - (window->h / 2);
  window->default_w = 1920;
  window->default_h = 1080;

  window->r = seedRand(time(NULL));
  window->fullScreen = 0;
  window->running = 1;
  window->win_width_ratio = (float)window->w / window->default_w;
  ///NOTE: change this to 0 after dev
  window->game_scene = 1;
  window->game_stage = 1;
  window->in_game_init = 0;
  window->menu_scene = 0;
  window->popUp = 0;

  window->mainWindow = NULL;
  window->mainWindow = SDL_CreateWindow("Surrounded By Water", window->x, window->y, window->w, window->h, SDL_WINDOW_RESIZABLE);
  if (window->mainWindow == NULL)
    lib_errorLog("failed at creating a window", SDL_GetError());

  SDL_SetWindowSize(window->mainWindow, window->w, window->h); // to avoid initin all the coords and then update them on resize

  // game icon
  SDL_RWops *icon_Path = SDL_RWFromFile("./assets/icons/game_icon.ico", "rb");
  if (icon_Path == NULL)
    lib_errorLog("failed at getting icon path", SDL_GetError());

  SDL_Surface *game_Icon = IMG_LoadICO_RW(icon_Path);
  if (game_Icon == NULL)
    lib_errorLog("failed at loading icon", SDL_GetError());

  SDL_SetWindowIcon(window->mainWindow, game_Icon);

  SDL_FreeRW(icon_Path);
  SDL_FreeSurface(game_Icon);

  // main renderer
  window->mainRenderer = SDL_CreateRenderer(window->mainWindow, -1, SDL_RENDERER_ACCELERATED);
  if (window->mainRenderer == NULL)
    lib_errorLog("failed at creating renderer form window", SDL_GetError());
}

void init_GameDev(GameDev *dev)
{
  // states
  dev->show_FPS = 0;
  dev->show_boxes = 0;
  dev->show_ranges = 0;
  dev->change_character = 0;
  dev->spawn_enemy = 0;

  // fps things
  dev->FPS = 0;
  dev->FPS_cap = 60;
  dev->currTick = 0;
  dev->prevTick = SDL_GetTicks();
  dev->frameDelayCount = 0;
  dev->frameDelayIndex = 0;
  dev->frameDelayAvg = 0;
}

void init_GameInput(GameInput *input)
{
  input->mouse_x = 0;
  input->mouse_y = 0;
  input->num_keys = 0;
}

void init_GameSound(GameSound *sound)
{

  int open_Audio = Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);
  if (open_Audio != 0)
    lib_errorLog("Failed at opnening Audio", Mix_GetError());

  sound->menu_music = Mix_LoadMUS("./assets/music/menu_music.mp3");
  if (sound->menu_music == NULL)
    lib_errorLog("Failed at load music File", Mix_GetError());

  sound->bird_SFX = Mix_LoadWAV("./assets/SFX/menu/birds_SFX.wav");
  if (sound->bird_SFX == NULL)
    lib_errorLog("Failed at load music File1", Mix_GetError());

  sound->wind_SFX = Mix_LoadWAV("./assets/SFX/menu/wind_SFX.wav");
  if (sound->wind_SFX == NULL)
    lib_errorLog("Failed at load music File2", Mix_GetError());

  sound->waves_SFX = Mix_LoadWAV("./assets/SFX/menu/waves_SFX.wav");
  if (sound->waves_SFX == NULL)
    lib_errorLog("Failed at load music File3", Mix_GetError());

  sound->click_SFX = Mix_LoadWAV("./assets/SFX/menu/click_SFX.wav");
  if (sound->click_SFX == NULL)
    lib_errorLog("Failed at load music File4", Mix_GetError());

  Mix_VolumeChunk(sound->click_SFX, 40);
  Mix_VolumeChunk(sound->wind_SFX, 25);
  Mix_VolumeChunk(sound->waves_SFX, 20);

  sound->mute = 0;
  sound->music_volume = 100;
  sound->SFX_volume = 100;

  sound->wind_play = 0;
  sound->button_click_play = 0;
  sound->bird_play = 0;
  sound->wave_play = 1;
}

void init_GameUI(GameUI *ui, GameWindow *window)
{
  init_DevUI(&(ui->dev_UI));
  init_Common_Menu(&(ui->common_UI), window);
  init_Scene0_Menu(&(ui->scene0_UI), &(ui->common_UI), window);
  init_Scene1_Menu(&(ui->scene1_UI), &(ui->common_UI), window);
  init_Scene2_Menu(&(ui->scene2_UI), &(ui->common_UI), window);
  init_Scene3_Menu(&(ui->scene3_UI), &(ui->common_UI), window);
  init_Quit_PopUp(&(ui->quit_PopUp), &(ui->common_UI), window);
  init_In_Game_UI(&(ui->in_game_UI), window);
}

void init_GameLogic(GameLogic *logic, In_Game_UI *ui)
{
  ///TODO: finish initGameLogic
  // MAP system constant coords
  int map_w = 0, map_h = 0;
  int query = SDL_QueryTexture(ui->map, NULL, NULL, &map_w, &map_h);
  if (query != 0)
    lib_errorLog("failed at getting texutre dimentions", SDL_GetError());

  logic->MAP_Map_Coords.x = 0;
  logic->MAP_Map_Coords.y = 0;
  logic->MAP_Map_Coords.w = map_w;
  logic->MAP_Map_Coords.h = map_h;

  logic->MAP_Origin_Coords.x = logic->MAP_Map_Coords.w / 2;
  logic->MAP_Origin_Coords.y = logic->MAP_Map_Coords.h / 2;
  logic->MAP_Origin_Coords.h = 0;
  logic->MAP_Origin_Coords.w = 0;

  init_survivors(logic);
  init_bird(logic, 1);

  // stage objects
  logic->enemy_num = 0;
  logic->obstacle_num = 0;
}

/// TODO: put these in seperate file
void init_survivors(GameLogic *logic)
{
  // common
  static int first_time = 1;
  for (int i = 0; i < 4; i++)
  {
    if (first_time == 1)
    {
      logic->survivors[i].xp = 0;
      logic->survivors[i].lvl = 0;
      first_time = 0;
    }

    logic->survivors[i].distance_walked = 0;
    logic->survivors[i].is_spawned = 0;
  }

  //////////////////////////////////////////////////////
  ////////////// JAMES
  //////////////////////////////////////////////////////
  // misc
  strcpy(logic->survivors[0].name, "JAMES");

  // properties
  logic->survivors[0].speed = 5;

  // ranges
  logic->survivors[0].dmg_range = 700;

  // coords
  logic->survivors[0].coords.w = 70;
  logic->survivors[0].coords.h = 100;
  logic->survivors[0].coords.x = 0;
  logic->survivors[0].coords.y = 0;

  //////////////////////////////////////////////////////
  ////////////// MARIE
  //////////////////////////////////////////////////////
  // misc
  strcpy(logic->survivors[1].name, "MARIE");

  // ranges
  logic->survivors[1].dmg_range = 500;

  // coords
  logic->survivors[1].coords.w = 60;
  logic->survivors[1].coords.h = 80;
  logic->survivors[1].coords.x = 0;
  logic->survivors[1].coords.y = 0;

  // properties
  logic->survivors[1].speed = 6;
}

void init_bird(GameLogic *logic, int stage)
{
  ///TODO: finish init bird

  // misc
  strcpy(logic->enemy_types[0].name, "BIRD");
  logic->enemy_types[0].herd_id = 0;
  logic->enemy_types[0].num_spawned = 0;

  // properties
  logic->enemy_types[0].speed = 8;

  // coords
  logic->enemy_types[0].coords.x = 0;
  logic->enemy_types[0].coords.y = 0;
  logic->enemy_types[0].coords.w = 50;
  logic->enemy_types[0].coords.h = 50;

  // ranges
  logic->enemy_types[0].roam_range.w = 0;
  logic->enemy_types[0].detection_range = 0;
  logic->enemy_types[0].avoid_obstacle_range = 500;
  logic->enemy_types[0].attack_range = 100;
  logic->enemy_types[0].dmg_range = 150;
  logic->enemy_types[0].escape_range = 0;

  // states
  logic->enemy_types[0].is_alerted = 1;
  logic->enemy_types[0].is_moving = 0;
  logic->enemy_types[0].is_attacking = 0;
  logic->enemy_types[0].is_aiming = 0;
  logic->enemy_types[0].is_dead = 0;
}