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
	DoLayerPrint();
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

void j1Viewports::LayerDrawQuad(const SDL_Rect & rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool filled, bool use_camera)
{
	layer_quad q(rect, r, g, b, a, filled, use_camera);
	quad_list.push_back(q);
}

void j1Viewports::LayerDrawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera)
{
	layer_line l(x1, y1, x2, y2, r, g, b, a, use_camera);
	line_list.push_back(l);
}

void j1Viewports::LayerDrawCircle(int x1, int y1, int redius, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera)
{
	layer_circle c(x1, y1, redius, r, g, b, a, use_camera);
	circle_list.push_back(c);
}

void j1Viewports::DoLayerPrint()
{
	vector<layer_blit> blit;
	while (layer_list.Count() > 0)
	{
		layer_blit current;
		layer_list.Pop(current);
		blit.push_back(current);
	}

		// Viewports
		
		switch (number_of_views)
		{
			case 1:
			{
				for(int i = 0; i < blit.size(); i++)
					App->render->Blit(blit.at(i).texture, blit.at(i).pos.x + camera1.x, blit.at(i).pos.y + camera1.y, &blit.at(i).section, blit.at(i).scale, blit.at(i).flip, blit.at(i).angle, blit.at(i).pivot_x, blit.at(i).pivot_y);
		
				for (int i = 0; i < quad_list.size(); i++)
					App->render->DrawQuad({ quad_list.at(i).rect.x + camera1.x, quad_list.at(i).rect.y + camera1.y, quad_list.at(i).rect.w, quad_list.at(i).rect.h }, quad_list.at(i).r, quad_list.at(i).g, quad_list.at(i).b, quad_list.at(i).a, quad_list.at(i).filled, quad_list.at(i).use_camera);
			
				for (int i = 0; i < line_list.size(); i++)
					App->render->DrawLine(line_list.at(i).x1 + camera1.x, line_list.at(i).y1 + camera1.y, line_list.at(i).x2 + camera1.x, line_list.at(i).y2 + camera1.y, line_list.at(i).r, line_list.at(i).g, line_list.at(i).b, line_list.at(i).a, line_list.at(i).use_camera);

				for (int i = 0; i < circle_list.size(); i++)
					App->render->DrawCircle(circle_list.at(i).x1 + camera1.x, circle_list.at(i).y1 + camera1.y, circle_list.at(i).redius, circle_list.at(i).r, circle_list.at(i).g, circle_list.at(i).b, circle_list.at(i).a, circle_list.at(i).use_camera);

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

				for (int i = 0; i < blit.size(); i++)
					App->render->Blit(blit.at(i).texture, blit.at(i).pos.x + camera1.x, blit.at(i).pos.y + camera1.y, &blit.at(i).section, blit.at(i).scale, blit.at(i).flip, blit.at(i).angle, blit.at(i).pivot_x, blit.at(i).pivot_y);

				for (int i = 0; i < quad_list.size(); i++)
					App->render->DrawQuad({ quad_list.at(i).rect.x + camera1.x, quad_list.at(i).rect.y + camera1.y, quad_list.at(i).rect.w, quad_list.at(i).rect.h }, quad_list.at(i).r, quad_list.at(i).g, quad_list.at(i).b, quad_list.at(i).a, quad_list.at(i).filled, quad_list.at(i).use_camera);

				for (int i = 0; i < line_list.size(); i++)
					App->render->DrawLine(line_list.at(i).x1 + camera1.x, line_list.at(i).y1 + camera1.y, line_list.at(i).x2 + camera1.x, line_list.at(i).y2 + camera1.y, line_list.at(i).r, line_list.at(i).g, line_list.at(i).b, line_list.at(i).a, line_list.at(i).use_camera);

				for (int i = 0; i < circle_list.size(); i++)
					App->render->DrawCircle(circle_list.at(i).x1 + camera1.x, circle_list.at(i).y1 + camera1.y, circle_list.at(i).redius, circle_list.at(i).r, circle_list.at(i).g, circle_list.at(i).b, circle_list.at(i).a, circle_list.at(i).use_camera);

				// View 2
				App->render->SetViewPort(view2_2);

				for (int i = 0; i < blit.size(); i++)
					App->render->Blit(blit.at(i).texture, blit.at(i).pos.x + camera2.x, blit.at(i).pos.y + camera2.y, &blit.at(i).section, blit.at(i).scale, blit.at(i).flip, blit.at(i).angle, blit.at(i).pivot_x, blit.at(i).pivot_y);

				for (int i = 0; i < quad_list.size(); i++)
					App->render->DrawQuad({ quad_list.at(i).rect.x + camera2.x, quad_list.at(i).rect.y + camera2.y, quad_list.at(i).rect.w, quad_list.at(i).rect.h }, quad_list.at(i).r, quad_list.at(i).g, quad_list.at(i).b, quad_list.at(i).a, quad_list.at(i).filled, quad_list.at(i).use_camera);

				for (int i = 0; i < line_list.size(); i++)
					App->render->DrawLine(line_list.at(i).x1 + camera2.x, line_list.at(i).y1 + camera2.y, line_list.at(i).x2 + camera2.x, line_list.at(i).y2 + camera2.y, line_list.at(i).r, line_list.at(i).g, line_list.at(i).b, line_list.at(i).a, line_list.at(i).use_camera);

				for (int i = 0; i < circle_list.size(); i++)
					App->render->DrawCircle(circle_list.at(i).x1 + camera2.x, circle_list.at(i).y1 + camera2.y, circle_list.at(i).redius, circle_list.at(i).r, circle_list.at(i).g, circle_list.at(i).b, circle_list.at(i).a, circle_list.at(i).use_camera);

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

				for (int i = 0; i < blit.size(); i++)
					App->render->Blit(blit.at(i).texture, blit.at(i).pos.x + camera1.x, blit.at(i).pos.y + camera1.y, &blit.at(i).section, blit.at(i).scale, blit.at(i).flip, blit.at(i).angle, blit.at(i).pivot_x, blit.at(i).pivot_y);

				for (int i = 0; i < quad_list.size(); i++)
					App->render->DrawQuad({ quad_list.at(i).rect.x + camera1.x, quad_list.at(i).rect.y + camera1.y, quad_list.at(i).rect.w, quad_list.at(i).rect.h }, quad_list.at(i).r, quad_list.at(i).g, quad_list.at(i).b, quad_list.at(i).a, quad_list.at(i).filled, quad_list.at(i).use_camera);

				for (int i = 0; i < line_list.size(); i++)
					App->render->DrawLine(line_list.at(i).x1 + camera1.x, line_list.at(i).y1 + camera1.y, line_list.at(i).x2 + camera1.x, line_list.at(i).y2 + camera1.y, line_list.at(i).r, line_list.at(i).g, line_list.at(i).b, line_list.at(i).a, line_list.at(i).use_camera);

				for (int i = 0; i < circle_list.size(); i++)
					App->render->DrawCircle(circle_list.at(i).x1 + camera1.x, circle_list.at(i).y1 + camera1.y, circle_list.at(i).redius, circle_list.at(i).r, circle_list.at(i).g, circle_list.at(i).b, circle_list.at(i).a, circle_list.at(i).use_camera);


				// View 2
				App->render->SetViewPort(view4_2);

				for (int i = 0; i < blit.size(); i++)
					App->render->Blit(blit.at(i).texture, blit.at(i).pos.x + camera2.x, blit.at(i).pos.y + camera2.y, &blit.at(i).section, blit.at(i).scale, blit.at(i).flip, blit.at(i).angle, blit.at(i).pivot_x, blit.at(i).pivot_y);

				for (int i = 0; i < quad_list.size(); i++)
					App->render->DrawQuad({ quad_list.at(i).rect.x + camera2.x, quad_list.at(i).rect.y + camera2.y, quad_list.at(i).rect.w, quad_list.at(i).rect.h }, quad_list.at(i).r, quad_list.at(i).g, quad_list.at(i).b, quad_list.at(i).a, quad_list.at(i).filled, quad_list.at(i).use_camera);

				for (int i = 0; i < line_list.size(); i++)
					App->render->DrawLine(line_list.at(i).x1 + camera2.x, line_list.at(i).y1 + camera2.y, line_list.at(i).x2 + camera2.x, line_list.at(i).y2 + camera2.y, line_list.at(i).r, line_list.at(i).g, line_list.at(i).b, line_list.at(i).a, line_list.at(i).use_camera);

				for (int i = 0; i < circle_list.size(); i++)
					App->render->DrawCircle(circle_list.at(i).x1 + camera2.x, circle_list.at(i).y1 + camera2.y, circle_list.at(i).redius, circle_list.at(i).r, circle_list.at(i).g, circle_list.at(i).b, circle_list.at(i).a, circle_list.at(i).use_camera);

				// View 3
				App->render->SetViewPort(view4_3);

				for (int i = 0; i < blit.size(); i++)
					App->render->Blit(blit.at(i).texture, blit.at(i).pos.x + camera3.x, blit.at(i).pos.y + camera3.y, &blit.at(i).section, blit.at(i).scale, blit.at(i).flip, blit.at(i).angle, blit.at(i).pivot_x, blit.at(i).pivot_y);

				for (int i = 0; i < quad_list.size(); i++)
					App->render->DrawQuad({ quad_list.at(i).rect.x + camera3.x, quad_list.at(i).rect.y + camera3.y, quad_list.at(i).rect.w, quad_list.at(i).rect.h }, quad_list.at(i).r, quad_list.at(i).g, quad_list.at(i).b, quad_list.at(i).a, quad_list.at(i).filled, quad_list.at(i).use_camera);

				for (int i = 0; i < line_list.size(); i++)
					App->render->DrawLine(line_list.at(i).x1 + camera3.x, line_list.at(i).y1 + camera3.y, line_list.at(i).x2 + camera3.x, line_list.at(i).y2 + camera3.y, line_list.at(i).r, line_list.at(i).g, line_list.at(i).b, line_list.at(i).a, line_list.at(i).use_camera);

				for (int i = 0; i < circle_list.size(); i++)
					App->render->DrawCircle(circle_list.at(i).x1 + camera3.x, circle_list.at(i).y1 + camera3.y, circle_list.at(i).redius, circle_list.at(i).r, circle_list.at(i).g, circle_list.at(i).b, circle_list.at(i).a, circle_list.at(i).use_camera);

				// View 4
				App->render->SetViewPort(view4_4);

				for (int i = 0; i < blit.size(); i++)
					App->render->Blit(blit.at(i).texture, blit.at(i).pos.x + camera4.x, blit.at(i).pos.y + camera4.y, &blit.at(i).section, blit.at(i).scale, blit.at(i).flip, blit.at(i).angle, blit.at(i).pivot_x, blit.at(i).pivot_y);

				for (int i = 0; i < quad_list.size(); i++)
					App->render->DrawQuad({ quad_list.at(i).rect.x + camera4.x, quad_list.at(i).rect.y + camera4.y, quad_list.at(i).rect.w, quad_list.at(i).rect.h }, quad_list.at(i).r, quad_list.at(i).g, quad_list.at(i).b, quad_list.at(i).a, quad_list.at(i).filled, quad_list.at(i).use_camera);

				for (int i = 0; i < line_list.size(); i++)
					App->render->DrawLine(line_list.at(i).x1 + camera4.x, line_list.at(i).y1 + camera4.y, line_list.at(i).x2 + camera4.x, line_list.at(i).y2 + camera4.y, line_list.at(i).r, line_list.at(i).g, line_list.at(i).b, line_list.at(i).a, line_list.at(i).use_camera);

				for (int i = 0; i < circle_list.size(); i++)
					App->render->DrawCircle(circle_list.at(i).x1 + camera4.x, circle_list.at(i).y1 + camera4.y, circle_list.at(i).redius, circle_list.at(i).r, circle_list.at(i).g, circle_list.at(i).b, circle_list.at(i).a, circle_list.at(i).use_camera);

				App->render->ResetViewPort();
			}
			break;
		}

		blit.clear();
		quad_list.clear();
		line_list.clear();
		circle_list.clear();
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

