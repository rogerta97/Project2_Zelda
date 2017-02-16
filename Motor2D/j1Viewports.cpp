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

	// Viewports ------
	view2_1 = { 0, 0, (int)win_w / 2, (int)win_h };
	view2_2 = { (int)win_w / 2, 0, (int)win_w, (int)win_h };

	view4_1 = { 0, 0, (int)win_w / 2, (int)win_h / 2 };
	view4_2 = { (int)win_w / 2, 0, (int)win_w, (int)win_h / 2 };
	view4_3 = { 0, (int)win_h / 2, (int)win_w / 2, (int)win_h };
	view4_4 = { (int)win_w / 2, (int)win_h / 2, (int)win_w, (int)win_h };
	// -------------

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

void j1Viewports::SetViews(uint number)
{
	if (number > 0 && number < 5 && number != 3)
		number_of_views = number;
}

uint j1Viewports::GetViews()
{
	return number_of_views;
}

void j1Viewports::LayerBlit(int layer, SDL_Texture * texture, iPoint pos, const SDL_Rect section, float scale, SDL_RendererFlip _flip, double angle, int pivot_x, int pivot_y)
{
	layer_blit lblit(texture, pos, section, scale, _flip, angle, pivot_x, pivot_y);
	layer_list.Push(lblit, layer);
}

void j1Viewports::LayerDrawQuad(const SDL_Rect & rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool filled, bool use_camera) const
{
	switch (number_of_views)
	{
	case 1:
	{
		App->render->DrawQuad({ rect.x + camera1.x, rect.y + camera1.x, rect.w, rect.h }, r, g, b, a, filled, use_camera);
	}
	break;
	case 2:
	{
		App->render->SetViewPort(view2_1);

		App->render->DrawQuad({ rect.x + camera1.x, rect.y + camera1.y, rect.w, rect.h }, r, g, b, a, filled, use_camera);

		App->render->SetViewPort(view2_2);

		App->render->DrawQuad({ rect.x + camera2.x, rect.y + camera2.y, rect.w, rect.h }, r, g, b, a, filled, use_camera);

		App->render->ResetViewPort();
	}
	break;
	case 4:
	{
		App->render->SetViewPort(view4_1);

		App->render->DrawQuad({ rect.x + camera1.x, rect.y + camera1.y, rect.w, rect.h }, r, g, b, a, filled, use_camera);

		App->render->SetViewPort(view4_2);

		App->render->DrawQuad({ rect.x + camera2.x, rect.y + camera2.y, rect.w, rect.h }, r, g, b, a, filled, use_camera);

		App->render->SetViewPort(view4_3);

		App->render->DrawQuad({ rect.x + camera3.x, rect.y + camera3.y, rect.w, rect.h }, r, g, b, a, filled, use_camera);

		App->render->SetViewPort(view4_4);

		App->render->DrawQuad({ rect.x + camera4.x, rect.y + camera4.y, rect.w, rect.h }, r, g, b, a, filled, use_camera);

		App->render->ResetViewPort();
	}
	break;

	}
}

void j1Viewports::LayerDrawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera) const
{
	switch (number_of_views)
	{
	case 1:
	{
		App->render->DrawLine(x1 + camera1.x, y1 + camera1.y, x2 + camera1.x, y2 + camera1.y, r, g, b, a, use_camera);
	}
	break;
	case 2:
	{
		App->render->SetViewPort(view2_1);

		App->render->DrawLine(x1 + camera1.x, y1 + camera1.y, x2 + camera1.x, y2 + camera1.y, r, g, b, a, use_camera);

		App->render->SetViewPort(view2_2);

		App->render->DrawLine(x1 + camera2.x, y1 + camera2.y, x2 + camera2.x, y2 + camera2.y, r, g, b, a, use_camera);

		App->render->ResetViewPort();
	}
	break;
	case 4:
	{
		App->render->SetViewPort(view4_1);

		App->render->DrawLine(x1 + camera1.x, y1 + camera1.y, x2 + camera1.x, y2 + camera1.y, r, g, b, a, use_camera);

		App->render->SetViewPort(view4_2);

		App->render->DrawLine(x1 + camera2.x, y1 + camera2.y, x2 + camera2.x, y2 + camera2.y, r, g, b, a, use_camera);

		App->render->SetViewPort(view4_3);

		App->render->DrawLine(x1 + camera3.x, y1 + camera3.y, x2 + camera3.x, y2 + camera3.y, r, g, b, a, use_camera);

		App->render->SetViewPort(view4_4);

		App->render->DrawLine(x1 + camera4.x, y1 + camera4.y, x2 + camera4.x, y2 + camera4.y, r, g, b, a, use_camera);

		App->render->ResetViewPort();
	}
	break;

	}
}

void j1Viewports::LayerDrawCircle(int x1, int y1, int redius, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera) const
{
	switch (number_of_views)
	{
	case 1:
	{
		App->render->DrawCircle(x1 + camera1.x, y1 + camera1.y, redius, r, g, b, a, use_camera);
	}
	break;
	case 2:
	{
		App->render->SetViewPort(view2_1);

		App->render->DrawCircle(x1 + camera1.x, y1 + camera1.y, redius, r, g, b, a, use_camera);

		App->render->SetViewPort(view2_2);

		App->render->DrawCircle(x1 + camera2.x, y1 + camera2.y, redius, r, g, b, a, use_camera);

		App->render->ResetViewPort();
	}
	break;
	case 4:
	{
		App->render->SetViewPort(view4_1);

		App->render->DrawCircle(x1 + camera1.x, y1 + camera1.y, redius, r, g, b, a, use_camera);

		App->render->SetViewPort(view4_2);

		App->render->DrawCircle(x1 + camera2.x, y1 + camera2.y, redius, r, g, b, a, use_camera);

		App->render->SetViewPort(view4_3);

		App->render->DrawCircle(x1 + camera3.x, y1 + camera3.y, redius, r, g, b, a, use_camera);

		App->render->SetViewPort(view4_4);

		App->render->DrawCircle(x1 + camera4.x, y1 + camera4.y, redius, r, g, b, a, use_camera);

		App->render->ResetViewPort();
	}
	break;

	}
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
			if (App->debug_mode)
			{
				// Debug
				App->render->DrawQuad(view2_1, 255, 0, 0, 100);
				App->render->DrawQuad(view2_2, 0, 255, 0, 100);
			}

			// View 1
			App->render->SetViewPort(view2_1);

			App->render->Blit(current.texture, current.pos.x + camera1.x, current.pos.y + camera1.y, &current.section, current.scale, current.flip, current.angle, current.pivot_x, current.pivot_y);

			// View 2
			App->render->SetViewPort(view2_2);

			App->render->Blit(current.texture, current.pos.x + camera2.x, current.pos.y + camera2.y, &current.section, current.scale, current.flip, current.angle, current.pivot_x, current.pivot_y);

			App->render->ResetViewPort();
		}
		break;
		case 4:
		{
			// Debug
			if (App->debug_mode)
			{
				App->render->DrawQuad(view4_1, 255, 0, 0, 100);
				App->render->DrawQuad(view4_2, 0, 255, 0, 100);
				App->render->DrawQuad(view4_3, 0, 0, 255, 100);
				App->render->DrawQuad(view4_4, 255, 0, 255, 100);
			}

			// View 1
			App->render->SetViewPort(view4_1);

			App->render->Blit(current.texture, current.pos.x + camera1.x, current.pos.y + camera1.y, &current.section, current.scale, current.flip, current.angle, current.pivot_x, current.pivot_y);

			// View 2
			App->render->SetViewPort(view4_2);

			App->render->Blit(current.texture, current.pos.x + camera2.x, current.pos.y + camera2.y, &current.section, current.scale, current.flip, current.angle, current.pivot_x, current.pivot_y);

			// View 3
			App->render->SetViewPort(view4_3);

			App->render->Blit(current.texture, current.pos.x + camera3.x, current.pos.y + camera3.y, &current.section, current.scale, current.flip, current.angle, current.pivot_x, current.pivot_y);

			// View 4
			App->render->SetViewPort(view4_4);

			App->render->Blit(current.texture, current.pos.x + camera4.x, current.pos.y + camera4.y, &current.section, current.scale, current.flip, current.angle, current.pivot_x, current.pivot_y);

			App->render->ResetViewPort();
		}
		break;
		}
	}
}

void j1Viewports::OnCommand(std::list<std::string>& tokens)
{
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

