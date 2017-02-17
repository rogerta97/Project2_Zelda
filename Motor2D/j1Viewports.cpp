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

void j1Viewports::LayerBlit(int layer, SDL_Texture * texture, iPoint pos, const SDL_Rect section, int viewport, float scale, SDL_RendererFlip _flip, double angle, int pivot_x, int pivot_y)
{
	layer_blit lblit(texture, pos, section, viewport, scale, _flip, angle, pivot_x, pivot_y);
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
		// Viewports
		
		switch (number_of_views)
		{
			case 1:
			{
				for (p2PQueue_item<layer_blit>* curr = layer_list.start; curr != nullptr; curr = curr->next)
				{
					if(curr->data.viewport == 0 || curr->data.viewport == 1)
						App->render->Blit(curr->data.texture, curr->data.pos.x + camera1.x, curr->data.pos.y + camera1.y, &curr->data.section, curr->data.scale, curr->data.flip, curr->data.angle, curr->data.pivot_x, curr->data.pivot_y);
				}
		
				for (int i = 0; i < quad_list.size(); i++)
				{
					layer_quad curr = quad_list.at(i);
					App->render->DrawQuad({ curr.rect.x + camera1.x, curr.rect.y + camera1.y, curr.rect.w, curr.rect.h }, curr.r, curr.g, curr.b, curr.a, curr.filled, curr.use_camera);
				}
			
				for (int i = 0; i < line_list.size(); i++)
				{
					layer_line curr = line_list.at(i);
					App->render->DrawLine(curr.x1 + camera1.x, curr.y1 + camera1.y, curr.x2 + camera1.x, curr.y2 + camera1.y, curr.r, curr.g, curr.b, curr.a, curr.use_camera);
				}

				for (int i = 0; i < circle_list.size(); i++)
				{
					layer_circle curr = circle_list.at(i);
					App->render->DrawCircle(curr.x1 + camera1.x, curr.y1 + camera1.y, curr.redius, curr.r, curr.g, curr.b, curr.a, curr.use_camera);
				}

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

				for (p2PQueue_item<layer_blit>* curr = layer_list.start; curr != nullptr; curr = curr->next)
				{
					if (curr->data.viewport == 0 || curr->data.viewport == 1)
						App->render->Blit(curr->data.texture, curr->data.pos.x + camera1.x, curr->data.pos.y + camera1.y, &curr->data.section, curr->data.scale, curr->data.flip, curr->data.angle, curr->data.pivot_x, curr->data.pivot_y);
				}

				for (int i = 0; i < quad_list.size(); i++)
				{
					layer_quad curr = quad_list.at(i);
					App->render->DrawQuad({ curr.rect.x + camera1.x, curr.rect.y + camera1.y, curr.rect.w, curr.rect.h }, curr.r, curr.g, curr.b, curr.a, curr.filled, curr.use_camera);
				}

				for (int i = 0; i < line_list.size(); i++)
				{
					layer_line curr = line_list.at(i);
					App->render->DrawLine(curr.x1 + camera1.x, curr.y1 + camera1.y, curr.x2 + camera1.x, curr.y2 + camera1.y, curr.r, curr.g, curr.b, curr.a, curr.use_camera);
				}

				for (int i = 0; i < circle_list.size(); i++)
				{
					layer_circle curr = circle_list.at(i);
					App->render->DrawCircle(curr.x1 + camera1.x, curr.y1 + camera1.y, curr.redius, curr.r, curr.g, curr.b, curr.a, curr.use_camera);
				}

				// View 2
				App->render->SetViewPort(view2_2);

				for (p2PQueue_item<layer_blit>* curr = layer_list.start; curr != nullptr; curr = curr->next)
				{
					if (curr->data.viewport == 0 || curr->data.viewport == 2)
						App->render->Blit(curr->data.texture, curr->data.pos.x + camera2.x, curr->data.pos.y + camera2.y, &curr->data.section, curr->data.scale, curr->data.flip, curr->data.angle, curr->data.pivot_x, curr->data.pivot_y);
				}

				for (int i = 0; i < quad_list.size(); i++)
				{
					layer_quad curr = quad_list.at(i);
					App->render->DrawQuad({ curr.rect.x + camera2.x, curr.rect.y + camera2.y, curr.rect.w, curr.rect.h }, curr.r, curr.g, curr.b, curr.a, curr.filled, curr.use_camera);
				}

				for (int i = 0; i < line_list.size(); i++)
				{
					layer_line curr = line_list.at(i);
					App->render->DrawLine(curr.x1 + camera2.x, curr.y1 + camera2.y, curr.x2 + camera2.x, curr.y2 + camera2.y, curr.r, curr.g, curr.b, curr.a, curr.use_camera);
				}

				for (int i = 0; i < circle_list.size(); i++)
				{
					layer_circle curr = circle_list.at(i);
					App->render->DrawCircle(curr.x1 + camera2.x, curr.y1 + camera2.y, curr.redius, curr.r, curr.g, curr.b, curr.a, curr.use_camera);
				}
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

				for (p2PQueue_item<layer_blit>* curr = layer_list.start; curr != nullptr; curr = curr->next)
				{
					if (curr->data.viewport == 0 || curr->data.viewport == 1)
						App->render->Blit(curr->data.texture, curr->data.pos.x + camera1.x, curr->data.pos.y + camera1.y, &curr->data.section, curr->data.scale, curr->data.flip, curr->data.angle, curr->data.pivot_x, curr->data.pivot_y);
				}

				for (int i = 0; i < quad_list.size(); i++)
				{
					layer_quad curr = quad_list.at(i);
					App->render->DrawQuad({ curr.rect.x + camera1.x, curr.rect.y + camera1.y, curr.rect.w, curr.rect.h }, curr.r, curr.g, curr.b, curr.a, curr.filled, curr.use_camera);
				}

				for (int i = 0; i < line_list.size(); i++)
				{
					layer_line curr = line_list.at(i);
					App->render->DrawLine(curr.x1 + camera1.x, curr.y1 + camera1.y, curr.x2 + camera1.x, curr.y2 + camera1.y, curr.r, curr.g, curr.b, curr.a, curr.use_camera);
				}

				for (int i = 0; i < circle_list.size(); i++)
				{
					layer_circle curr = circle_list.at(i);
					App->render->DrawCircle(curr.x1 + camera1.x, curr.y1 + camera1.y, curr.redius, curr.r, curr.g, curr.b, curr.a, curr.use_camera);
				}

				// View 2
				App->render->SetViewPort(view4_2);

				for (p2PQueue_item<layer_blit>* curr = layer_list.start; curr != nullptr; curr = curr->next)
				{
					if (curr->data.viewport == 0 || curr->data.viewport == 2)
						App->render->Blit(curr->data.texture, curr->data.pos.x + camera2.x, curr->data.pos.y + camera2.y, &curr->data.section, curr->data.scale, curr->data.flip, curr->data.angle, curr->data.pivot_x, curr->data.pivot_y);
				}

				for (int i = 0; i < quad_list.size(); i++)
				{
					layer_quad curr = quad_list.at(i);
					App->render->DrawQuad({ curr.rect.x + camera2.x, curr.rect.y + camera2.y, curr.rect.w, curr.rect.h }, curr.r, curr.g, curr.b, curr.a, curr.filled, curr.use_camera);
				}

				for (int i = 0; i < line_list.size(); i++)
				{
					layer_line curr = line_list.at(i);
					App->render->DrawLine(curr.x1 + camera2.x, curr.y1 + camera2.y, curr.x2 + camera2.x, curr.y2 + camera2.y, curr.r, curr.g, curr.b, curr.a, curr.use_camera);
				}

				for (int i = 0; i < circle_list.size(); i++)
				{
					layer_circle curr = circle_list.at(i);
					App->render->DrawCircle(curr.x1 + camera2.x, curr.y1 + camera2.y, curr.redius, curr.r, curr.g, curr.b, curr.a, curr.use_camera);
				}

				// View 3
				App->render->SetViewPort(view4_3);

				for (p2PQueue_item<layer_blit>* curr = layer_list.start; curr != nullptr; curr = curr->next)
				{
					if (curr->data.viewport == 0 || curr->data.viewport == 3)
						App->render->Blit(curr->data.texture, curr->data.pos.x + camera3.x, curr->data.pos.y + camera3.y, &curr->data.section, curr->data.scale, curr->data.flip, curr->data.angle, curr->data.pivot_x, curr->data.pivot_y);
				}

				for (int i = 0; i < quad_list.size(); i++)
				{
					layer_quad curr = quad_list.at(i);
					App->render->DrawQuad({ curr.rect.x + camera3.x, curr.rect.y + camera3.y, curr.rect.w, curr.rect.h }, curr.r, curr.g, curr.b, curr.a, curr.filled, curr.use_camera);
				}

				for (int i = 0; i < line_list.size(); i++)
				{
					layer_line curr = line_list.at(i);
					App->render->DrawLine(curr.x1 + camera3.x, curr.y1 + camera3.y, curr.x2 + camera3.x, curr.y2 + camera3.y, curr.r, curr.g, curr.b, curr.a, curr.use_camera);
				}

				for (int i = 0; i < circle_list.size(); i++)
				{
					layer_circle curr = circle_list.at(i);
					App->render->DrawCircle(curr.x1 + camera3.x, curr.y1 + camera3.y, curr.redius, curr.r, curr.g, curr.b, curr.a, curr.use_camera);
				}

				// View 4
				App->render->SetViewPort(view4_4);

				for (p2PQueue_item<layer_blit>* curr = layer_list.start; curr != nullptr; curr = curr->next)
				{
					if (curr->data.viewport == 0 || curr->data.viewport == 4)
						App->render->Blit(curr->data.texture, curr->data.pos.x + camera4.x, curr->data.pos.y + camera4.y, &curr->data.section, curr->data.scale, curr->data.flip, curr->data.angle, curr->data.pivot_x, curr->data.pivot_y);
				}

				for (int i = 0; i < quad_list.size(); i++)
				{
					layer_quad curr = quad_list.at(i);
					App->render->DrawQuad({ curr.rect.x + camera4.x, curr.rect.y + camera4.y, curr.rect.w, curr.rect.h }, curr.r, curr.g, curr.b, curr.a, curr.filled, curr.use_camera);
				}

				for (int i = 0; i < line_list.size(); i++)
				{
					layer_line curr = line_list.at(i);
					App->render->DrawLine(curr.x1 + camera4.x, curr.y1 + camera4.y, curr.x2 + camera4.x, curr.y2 + camera4.y, curr.r, curr.g, curr.b, curr.a, curr.use_camera);
				}

				for (int i = 0; i < circle_list.size(); i++)
				{
					layer_circle curr = circle_list.at(i);
					App->render->DrawCircle(curr.x1 + camera4.x, curr.y1 + camera4.y, curr.redius, curr.r, curr.g, curr.b, curr.a, curr.use_camera);
				}

				App->render->ResetViewPort();
			}
			break;
		}

		layer_list.Clear();
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

