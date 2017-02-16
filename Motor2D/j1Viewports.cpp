#include "j1Viewports.h"
#include "j1Window.h"
#include "j1Input.h"
#include "j1Console.h"

j1Viewports::j1Viewports()
{
	name = "viewports";
}

j1Viewports::~j1Viewports()
{
}

bool j1Viewports::Awake()
{
	bool ret = true;

	return ret;
}

bool j1Viewports::Start()
{
	bool ret = true;

	number_of_views = 1;

	App->win->GetWindowSize(win_w, win_h);

	App->console->AddCVar("viewports.set", this, "Set the number of viewports on screen (1, 2, 4)");

	return ret;
}

bool j1Viewports::PreUpdate()
{
	bool ret = true;

	return ret;
}

bool j1Viewports::Update(float dt)
{
	bool ret = true;

	// Blit different layers
	DoLayerBlit();
	// ---------------------

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		camera1.x++;
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		camera1.x--;
	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		camera1.y++;
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		camera1.y--;

	return ret;
}

bool j1Viewports::PostUpdate()
{
	bool ret = true;

	return ret;
}

bool j1Viewports::CleanUp()
{
	bool ret = true;

	return ret;
}

void j1Viewports::LayerBlit(int layer, SDL_Texture * texture, iPoint pos, const SDL_Rect section, float scale, SDL_RendererFlip _flip, double angle, int pivot_x, int pivot_y)
{
	layer_blit lblit(texture, pos, section, scale, _flip, angle, pivot_x, pivot_y);
	layer_list.Push(lblit, layer);
}

void j1Viewports::SetViews(uint number)
{
	if (number > 0 && number < 5 && number != 3)
		number_of_views = number;
}

uint j1Viewports::GetViews()
{
	return number_of_views;
}

void j1Viewports::OnCommand(std::list<std::string>& tokens)
{
}

void j1Viewports::DoLayerBlit()
{
	while (layer_list.Count() > 0)
	{
		layer_blit current;
		layer_list.Pop(current);

		// Viewports
		
		switch (number_of_views)
		{
		case 1:
		{
			App->render->Blit(current.texture, current.pos.x + camera1.x, current.pos.y + camera1.y, &current.section, current.scale, current.flip, current.angle, current.pivot_x, current.pivot_y);
		}
		break;
		case 2:
		{
			SDL_Rect view1 = { 0, 0, (int)win_w / 2, (int)win_h };
			SDL_Rect view2 = { (int)win_w / 2, 0, (int)win_w, (int)win_h };

			if (App->debug_mode)
			{
				// Debug
				App->render->DrawQuad(view1, 255, 0, 0, 100);
				App->render->DrawQuad(view2, 0, 255, 0, 100);
			}

			// View 1
			App->render->SetViewPort(view1);

			App->render->Blit(current.texture, current.pos.x + camera1.x, current.pos.y + camera1.y, &current.section, current.scale, current.flip, current.angle, current.pivot_x, current.pivot_y);

			// View 2
			App->render->SetViewPort(view2);

			App->render->Blit(current.texture, current.pos.x + camera2.x, current.pos.y + camera2.y, &current.section, current.scale, current.flip, current.angle, current.pivot_x, current.pivot_y);

			App->render->ResetViewPort();
		}
		break;
		case 4:
		{
			SDL_Rect view1 = { 0, 0, (int)win_w / 2, (int)win_h / 2 };
			SDL_Rect view2 = { (int)win_w / 2, 0, (int)win_w, (int)win_h / 2 };
			SDL_Rect view3 = { 0, (int)win_h / 2, (int)win_w / 2, (int)win_h };
			SDL_Rect view4 = { (int)win_w / 2, (int)win_h / 2, (int)win_w, (int)win_h };

			// Debug
			if (App->debug_mode)
			{
				App->render->DrawQuad(view1, 255, 0, 0, 100);
				App->render->DrawQuad(view2, 0, 255, 0, 100);
				App->render->DrawQuad(view3, 0, 0, 255, 100);
				App->render->DrawQuad(view4, 255, 0, 255, 100);
			}

			// View 1
			App->render->SetViewPort(view1);

			App->render->Blit(current.texture, current.pos.x + camera1.x, current.pos.y + camera1.y, &current.section, current.scale, current.flip, current.angle, current.pivot_x, current.pivot_y);

			// View 2
			App->render->SetViewPort(view2);

			App->render->Blit(current.texture, current.pos.x + camera2.x, current.pos.y + camera2.y, &current.section, current.scale, current.flip, current.angle, current.pivot_x, current.pivot_y);

			// View 3
			App->render->SetViewPort(view3);

			App->render->Blit(current.texture, current.pos.x + camera3.x, current.pos.y + camera3.y, &current.section, current.scale, current.flip, current.angle, current.pivot_x, current.pivot_y);

			// View 4
			App->render->SetViewPort(view4);

			App->render->Blit(current.texture, current.pos.x + camera4.x, current.pos.y + camera4.y, &current.section, current.scale, current.flip, current.angle, current.pivot_x, current.pivot_y);

			App->render->ResetViewPort();
		}
		break;
		}
	}
}

void j1Viewports::OnCVar(std::list<std::string>& tokens)
{
	if (tokens.front() == "viewports.set")
	{
		list<string>::iterator it = tokens.begin();
		it++;
		float value = atof((*it).c_str());
		SetViews(value);

		if (value > 0 && value < 5 && value != 3)
		{
			string output("Number of set viewports to: ");
			output.insert(output.size(), *it);
			App->console->AddText(output.c_str(), Output);
		}
		else
		{
			string output("Error: It's only possible to set 1, 2 and 4 viewports");
			App->console->AddText(output.c_str(), Error);
		}
	}
}

void j1Viewports::SaveCVar(std::string & cvar_name, pugi::xml_node & node) const
{
}

