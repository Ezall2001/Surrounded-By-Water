#include "../headers/event.h"
#include "../headers/logic.h"

void window_events(SDL_Event *event, GameObject *G)
{

  switch (event->window.event)
  {
  case SDL_WINDOWEVENT_CLOSE:
  {
    G->window.running = 0;
    break;
  }
  case SDL_WINDOWEVENT_SIZE_CHANGED:
  {
    update_Window_Coords(&(G->window));
    update_Menu_Common_Coords(&(G->UI.common_UI), &(G->window));
    update_Menu_Scene0_Coords(&(G->UI.scene0_UI), &(G->UI.common_UI), &(G->window));
    update_Menu_Scene1_Coords(&(G->UI.scene1_UI), &(G->UI.common_UI), &(G->window));
    update_Menu_Scene2_Coords(&(G->UI.scene2_UI), &(G->UI.common_UI), &(G->window), &(G->sound));
    update_Menu_Scene3_Coords(&(G->UI.scene3_UI), &(G->UI.common_UI), &(G->window));
    update_Quit_PopUp_Coords(&(G->UI.quit_PopUp), &(G->UI.common_UI), &(G->window));

    break;
  }
  case SDL_WINDOWEVENT_MOVED:
  {
    update_Window_Coords(&(G->window));
    break;
  }

  default:
    break;
  }
}

void mouse_motion_events(SDL_Event *event, GameObject *G)
{
  G->input.mouse_x = event->motion.x;
  G->input.mouse_y = event->motion.y;

  if (G->window.game_scene == 0)
  {

    if (G->window.popUp == 1)
    {
      mouse_Button_Collision(G->UI.quit_PopUp.confirm, 2, &(G->input), &(G->sound));
    }
    else if (G->window.popUp == 0)
    {
      mouse_Button_Collision(G->UI.common_UI.scene_shortcuts, 2, &(G->input), &(G->sound));

      switch (G->window.menu_scene)
      {
      case 0:
        mouse_Button_Collision(G->UI.scene0_UI.scene_buttons, 4, &(G->input), &(G->sound));
        break;

      case 1:
        ///TODO: finish scene 1 events
        break;

      case 2:
        mouse_Button_Collision(G->UI.scene2_UI.volume_Controllers, 4, &(G->input), &(G->sound));
        mouse_Button_Collision(G->UI.scene2_UI.resolution_Controllers, 3, &(G->input), &(G->sound));
        mouse_Button_Collision(G->UI.scene2_UI.fps_Controllers, 3, &(G->input), &(G->sound));
        mouse_Button_Collision(G->UI.scene2_UI.credits, 1, &(G->input), &(G->sound));
        drag_Volume(G->UI.scene2_UI.volume_Scorllers, 2, &(G->input), &(G->sound), G->UI.scene2_UI.music_Bar_Coords);
        mouse_Button_Collision(G->UI.scene2_UI.volume_Scorllers, 2, &(G->input), &(G->sound));
        break;

      default:
        break;
      }
    }
  }
}

void mouse_Input_events(SDL_Event *event, GameObject *G)
{
  if (G->window.game_scene == 0)
  {
    if (event->type == SDL_MOUSEBUTTONDOWN)
    {
      if (G->window.popUp == 1)
      {
        stage_Button(G->UI.quit_PopUp.confirm, 2);
      }
      else if (G->window.popUp == 0)
      {
        stage_Button(G->UI.common_UI.scene_shortcuts, 2);

        switch (G->window.menu_scene)
        {
        case 0:
          stage_Button(G->UI.scene0_UI.scene_buttons, 4);
          break;

        case 1:
          ////TODO: finish scene 1 events
          break;

        case 2:
          stage_Button(G->UI.scene2_UI.volume_Controllers, 4);
          stage_Button(G->UI.scene2_UI.resolution_Controllers, 3);
          stage_Button(G->UI.scene2_UI.fps_Controllers, 3);
          stage_Button(G->UI.scene2_UI.credits, 1);
          stage_Button(G->UI.scene2_UI.volume_Scorllers, 2);
          break;

        case 3:
          ////TODO: finish scene 3 events
          break;

        default:
          break;
        }
      }
    }
    else if (event->type == SDL_MOUSEBUTTONUP)
    {
      if (G->window.popUp == 1)
      {
        click_Button(G->UI.quit_PopUp.confirm, 2, &(G->window), &(G->sound), &(G->dev), &(G->UI));
      }
      else if (G->window.popUp == 0)
      {

        click_Button(G->UI.common_UI.scene_shortcuts, 2, &(G->window), &(G->sound), &(G->dev), &(G->UI));

        switch (G->window.menu_scene)
        {
        case 0:
          click_Button(G->UI.scene0_UI.scene_buttons, 4, &(G->window), &(G->sound), &(G->dev), &(G->UI));
          break;

        case 1:
          ////TODO: finish scene 1 events
          break;

        case 2:
          click_Button(G->UI.scene2_UI.volume_Controllers, 4, &(G->window), &(G->sound), &(G->dev), &(G->UI));
          click_Button(G->UI.scene2_UI.resolution_Controllers, 3, &(G->window), &(G->sound), &(G->dev), &(G->UI));
          click_Button(G->UI.scene2_UI.fps_Controllers, 3, &(G->window), &(G->sound), &(G->dev), &(G->UI));
          click_Button(G->UI.scene2_UI.credits, 1, &(G->window), &(G->sound), &(G->dev), &(G->UI));
          click_Button(G->UI.scene2_UI.volume_Scorllers, 2, &(G->window), &(G->sound), &(G->dev), &(G->UI));
          break;

        case 3:
          ////TODO: finish scene 3 events
          break;

        default:
          break;
        }
      }
    }
  }
  else if (G->window.game_scene == 1)
  {
  }
}

void keyboard_Input_events(SDL_Event *event, GameInput *input)
{
  if (event->key.keysym.sym == SDLK_RCTRL || event->key.keysym.sym == SDLK_LCTRL)
    input->ctrl = 1;
  else
  {
    input->keys[input->num_keys] = event->key.keysym.sym;
    (input->num_keys)++;
  }
}
