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
	scale = 2.0f;

	App->win->GetWindowSize(win_w, win_h);

	// Viewports ------
	view2_1 = { 0, 0, (int)win_w / 2, (int)win_h };
	view2_2 = { (int)win_w / 2, 0, (int)win_w/2, (int)win_h };

	view4_1 = { 0, 0, (int)win_w / 2, (int)win_h / 2 };
	view4_2 = { (int)win_w / 2, 0, (int)win_w/2, (int)win_h / 2 };
	view4_3 = { 0, (int)win_h / 2, (int)win_w / 2, (int)win_h/2 };
	view4_4 = { (int)win_w / 2, (int)win_h / 2, (int)win_w/2, (int)win_h/2 };
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
	{
		number_of_views = number;

		switch (number)
		{
		case 1:
			scale = 2.0f;
			break;
		case 2:
			scale = 1.0f;
			break;
		case 4:
			scale = 1.0f;
			break;
		default:
			break;
		}
	}
}

uint j1Viewports::GetViews()
{
	return number_of_views;
}

SDL_Rect j1Viewports::GetViewportRect(uint viewport)
{
	SDL_Rect ret = NULLRECT;

	switch (number_of_views)
	{
	case 1:
	{
		ret = { -camera1.x, -camera1.y, view4_1.w, view4_1.h };
		break;
	}
	case 2:
	{
		switch (viewport)
		{
		case 1:
			ret = { -camera1.x, -camera1.y, view4_1.w, view2_1.h };
			break;
		case 2:
			ret = { -camera2.x, -camera2.y, view4_2.w, view2_2.h };
			break;
		}
		break;
	}
	case 4:
	{
		switch (viewport)
		{
		case 1:
			ret = { -camera1.x, -camera1.y, view4_1.w, view4_1.h };
			break;
		case 2:
			ret = { -camera2.x, -camera2.y, view4_2.w, view4_2.h };
			break;
		case 3:
			ret = { -camera3.x, -camera3.y, view4_3.w, view4_3.h };
			break;
		case 4:
			ret = { -camera4.x, -camera4.y, view4_4.w, view4_4.h };
			break;
		}
		break;
	}
	}
	return ret;
}

void j1Viewports::MoveCamera(int id, int x, int y)
{
	switch (id)
	{
	case 1:
		camera1.x += x;
		camera1.y += y;
		break;
	case 2:
		camera2.x += x;
		camera2.y += y;
		break;
	case 3:
		camera3.x += x;
		camera3.y += y;
		break;
	case 4:
		camera4.x += x;
		camera4.y += y;
		break;
	default:
		break;
	}
}

void j1Viewports::SetCamera(int id, int x, int y)
{
	switch (id)
	{
	case 1:
		camera1.x = x;
		camera1.y = y;
		break;
	case 2:
		camera2.x = x;
		camera2.y = y;
		break;
	case 3:
		camera3.x = x;
		camera3.y = y;
		break;
	case 4:
		camera4.x = x;
		camera4.y = y;
		break;
	default:
		break;
	}
}

void j1Viewports::CenterCamera(int id, int x, int y)
{
	switch (number_of_views)
	{
	case 1:
		if (id == 1)
		{
			camera1.x = -x + win_w / 4;
			camera1.y = -y + win_h / 4;
		}
		break;
	case 2:
		switch (id)
		{
		case 1:
			camera1.x = -x + win_w / 4;
			camera1.y = -y + win_h / 2;
			break;
		case 2:
			camera2.x = -x + win_w / 4;
			camera2.y = -y + win_h / 2;
			break;
		default:
			break;
		}
		break;
	case 4:
		switch (id)
		{
		case 1:
			camera1.x = -x + win_w / 4;
			camera1.y = -y + win_h / 4;
			break;		
		case 2:			
			camera2.x = -x + win_w / 4;
			camera2.y = -y + win_h / 4;
			break;		
		case 3:			
			camera3.x = -x + win_w / 4;
			camera3.y = -y + win_h / 4;
			break;		
		case 4:			
			camera4.x = -x + win_w / 4;
			camera4.y = -y + win_h / 4;
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

void j1Viewports::LayerBlit(int layer, SDL_Texture * texture, iPoint pos, const SDL_Rect section, int viewport, float scale, bool use_camera, SDL_RendererFlip _flip, double angle, int pivot_x, int pivot_y)
{
	layer_blit lblit(texture, pos, section, viewport, scale, use_camera, _flip, angle, pivot_x, pivot_y);

	switch (viewport)
	{
	case 1:
		layer_list1.Push(lblit, layer);
		break;
	case 2:
		layer_list2.Push(lblit, layer);
		break;
	case 3:
		layer_list3.Push(lblit, layer);
		break;
	case 4:
		layer_list4.Push(lblit, layer);
		break;
	default:
		layer_list1.Push(lblit, layer);
		if(number_of_views > 1)
			layer_list2.Push(lblit, layer);
		if (number_of_views > 3)
		{
			layer_list3.Push(lblit, layer);
			layer_list4.Push(lblit, layer);
		}
		break;
	}
}

void j1Viewports::LayerDrawQuad(const SDL_Rect rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool filled, int layer, int viewport, bool use_camera)
{
	layer_quad q(rect, r, g, b, a, filled, use_camera);

	switch (viewport)
	{
	case 1:
		quad_list1.Push(q, layer);
		break;
	case 2:
		quad_list2.Push(q, layer);
		break;
	case 3:
		quad_list3.Push(q, layer);
		break;
	case 4:
		quad_list4.Push(q, layer);
		break;
	default:
		quad_list1.Push(q, layer);
		if (number_of_views > 1)
			quad_list2.Push(q, layer);
		if (number_of_views > 3)
		{
			quad_list3.Push(q, layer);
			quad_list4.Push(q, layer);
		}
		break;
	}
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
			for (p2PQueue_item<layer_blit>* curr = layer_list1.start; curr != nullptr; curr = curr->next)
			{
				float blit_scale = (curr->data.scale != -1.0f) ? curr->data.scale : scale;
				if(curr->data.use_camera)
					App->render->Blit(curr->data.texture, curr->data.pos.x + camera1.x, curr->data.pos.y + camera1.y, &curr->data.section, blit_scale, curr->data.use_camera, curr->data.flip, curr->data.angle, curr->data.pivot_x, curr->data.pivot_y);
				else
					App->render->Blit(curr->data.texture, curr->data.pos.x, curr->data.pos.y, &curr->data.section, blit_scale, curr->data.use_camera, curr->data.flip, curr->data.angle, curr->data.pivot_x, curr->data.pivot_y);

			}
	
			for (p2PQueue_item<layer_quad>* curr = quad_list1.start; curr != nullptr; curr = curr->next)
			{
				App->render->DrawQuad({ curr->data.rect.x + camera1.x, curr->data.rect.y + camera1.y, curr->data.rect.w, curr->data.rect.h }, curr->data.r, curr->data.g, curr->data.b, scale, curr->data.a, curr->data.filled, curr->data.use_camera);
			}
		
			for (int i = 0; i < line_list.size(); i++)
			{
				layer_line curr = line_list.at(i);
				App->render->DrawLine(curr.x1 + camera1.x, curr.y1 + camera1.y, curr.x2 + camera1.x, curr.y2 + camera1.y, curr.r, curr.g, curr.b, scale, curr.a, curr.use_camera);
			}

			for (int i = 0; i < circle_list.size(); i++)
			{
				layer_circle curr = circle_list.at(i);
				App->render->DrawCircle(curr.x1 + camera1.x, curr.y1 + camera1.y, curr.redius, curr.r, curr.g, curr.b, scale, curr.a, curr.use_camera);
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

			for (p2PQueue_item<layer_blit>* curr = layer_list1.start; curr != nullptr; curr = curr->next)
			{
				float blit_scale = (curr->data.scale != -1.0f) ? curr->data.scale : scale;
				if(curr->data.use_camera)
					App->render->Blit(curr->data.texture, curr->data.pos.x + camera1.x, curr->data.pos.y + camera1.y, &curr->data.section, blit_scale, curr->data.use_camera, curr->data.flip, curr->data.angle, curr->data.pivot_x, curr->data.pivot_y);
				else
					App->render->Blit(curr->data.texture, curr->data.pos.x, curr->data.pos.y, &curr->data.section, blit_scale, curr->data.use_camera, curr->data.flip, curr->data.angle, curr->data.pivot_x, curr->data.pivot_y);

			}

			for (p2PQueue_item<layer_quad>* curr = quad_list1.start; curr != nullptr; curr = curr->next)
			{
				App->render->DrawQuad({ curr->data.rect.x + camera1.x, curr->data.rect.y + camera1.y, curr->data.rect.w, curr->data.rect.h }, curr->data.r, curr->data.g, curr->data.b, scale, curr->data.a, curr->data.filled, curr->data.use_camera);
			}

			for (int i = 0; i < line_list.size(); i++)
			{
				layer_line curr = line_list.at(i);
				App->render->DrawLine(curr.x1 + camera1.x, curr.y1 + camera1.y, curr.x2 + camera1.x, curr.y2 + camera1.y, curr.r, curr.g, curr.b, scale, curr.a, curr.use_camera);
			}

			for (int i = 0; i < circle_list.size(); i++)
			{
				layer_circle curr = circle_list.at(i);
				App->render->DrawCircle(curr.x1 + camera1.x, curr.y1 + camera1.y, curr.redius, curr.r, curr.g, curr.b, scale, curr.a, curr.use_camera);
			}

			// View 2
			App->render->SetViewPort(view2_2);

			for (p2PQueue_item<layer_blit>* curr = layer_list2.start; curr != nullptr; curr = curr->next)
			{
				float blit_scale = (curr->data.scale != -1.0f) ? curr->data.scale : scale;
				if(curr->data.use_camera)
					App->render->Blit(curr->data.texture, curr->data.pos.x + camera2.x, curr->data.pos.y + camera2.y, &curr->data.section, blit_scale, curr->data.use_camera, curr->data.flip, curr->data.angle, curr->data.pivot_x, curr->data.pivot_y);
				else
					App->render->Blit(curr->data.texture, curr->data.pos.x, curr->data.pos.y, &curr->data.section, blit_scale, curr->data.use_camera, curr->data.flip, curr->data.angle, curr->data.pivot_x, curr->data.pivot_y);

			}

			for (p2PQueue_item<layer_quad>* curr = quad_list2.start; curr != nullptr; curr = curr->next)
			{
				App->render->DrawQuad({ curr->data.rect.x + camera2.x, curr->data.rect.y + camera2.y, curr->data.rect.w, curr->data.rect.h }, curr->data.r, curr->data.g, curr->data.b, scale, curr->data.a, curr->data.filled, curr->data.use_camera);
			}

			for (int i = 0; i < line_list.size(); i++)
			{
				layer_line curr = line_list.at(i);
				App->render->DrawLine(curr.x1 + camera2.x, curr.y1 + camera2.y, curr.x2 + camera2.x, curr.y2 + camera2.y, curr.r, curr.g, curr.b, scale, curr.a, curr.use_camera);
			}

			for (int i = 0; i < circle_list.size(); i++)
			{
				layer_circle curr = circle_list.at(i);
				App->render->DrawCircle(curr.x1 + camera2.x, curr.y1 + camera2.y, curr.redius, curr.r, curr.g, curr.b, scale, curr.a, curr.use_camera);
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

			for (p2PQueue_item<layer_blit>* curr = layer_list1.start; curr != nullptr; curr = curr->next)
			{
				float blit_scale = (curr->data.scale != -1.0f) ? curr->data.scale : scale;
				if(curr->data.use_camera)
					App->render->Blit(curr->data.texture, curr->data.pos.x + camera1.x, curr->data.pos.y + camera1.y, &curr->data.section, blit_scale, curr->data.use_camera, curr->data.flip, curr->data.angle, curr->data.pivot_x, curr->data.pivot_y);
				else
					App->render->Blit(curr->data.texture, curr->data.pos.x, curr->data.pos.y, &curr->data.section, blit_scale, curr->data.use_camera, curr->data.flip, curr->data.angle, curr->data.pivot_x, curr->data.pivot_y);
			}

			for (p2PQueue_item<layer_quad>* curr = quad_list1.start; curr != nullptr; curr = curr->next)
			{
				App->render->DrawQuad({ curr->data.rect.x + camera1.x, curr->data.rect.y + camera1.y, curr->data.rect.w, curr->data.rect.h }, curr->data.r, curr->data.g, curr->data.b, scale, curr->data.a, curr->data.filled, curr->data.use_camera);
			}

			for (int i = 0; i < line_list.size(); i++)
			{
				layer_line curr = line_list.at(i);
				App->render->DrawLine(curr.x1 + camera1.x, curr.y1 + camera1.y, curr.x2 + camera1.x, curr.y2 + camera1.y, curr.r, curr.g, curr.b, scale, curr.a, curr.use_camera);
			}

			for (int i = 0; i < circle_list.size(); i++)
			{
				layer_circle curr = circle_list.at(i);
				App->render->DrawCircle(curr.x1 + camera1.x, curr.y1 + camera1.y, curr.redius, curr.r, curr.g, curr.b, scale, curr.a, curr.use_camera);
			}

			// View 2
			App->render->SetViewPort(view4_2);

			for (p2PQueue_item<layer_blit>* curr = layer_list2.start; curr != nullptr; curr = curr->next)
			{
				float blit_scale = (curr->data.scale != -1.0f) ? curr->data.scale : scale;
				if(curr->data.use_camera)
					App->render->Blit(curr->data.texture, curr->data.pos.x + camera2.x, curr->data.pos.y + camera2.y, &curr->data.section, blit_scale, curr->data.use_camera, curr->data.flip, curr->data.angle, curr->data.pivot_x, curr->data.pivot_y);
				else
					App->render->Blit(curr->data.texture, curr->data.pos.x, curr->data.pos.y, &curr->data.section, blit_scale, curr->data.use_camera, curr->data.flip, curr->data.angle, curr->data.pivot_x, curr->data.pivot_y);
			}

			for (p2PQueue_item<layer_quad>* curr = quad_list2.start; curr != nullptr; curr = curr->next)
			{
				App->render->DrawQuad({ curr->data.rect.x + camera2.x, curr->data.rect.y + camera2.y, curr->data.rect.w, curr->data.rect.h }, curr->data.r, curr->data.g, curr->data.b, scale, curr->data.a, curr->data.filled, curr->data.use_camera);
			}

			for (int i = 0; i < line_list.size(); i++)
			{
				layer_line curr = line_list.at(i);
				App->render->DrawLine(curr.x1 + camera2.x, curr.y1 + camera2.y, curr.x2 + camera2.x, curr.y2 + camera2.y, curr.r, curr.g, curr.b, scale, curr.a, curr.use_camera);
			}

			for (int i = 0; i < circle_list.size(); i++)
			{
				layer_circle curr = circle_list.at(i);
				App->render->DrawCircle(curr.x1 + camera2.x, curr.y1 + camera2.y, curr.redius, curr.r, curr.g, curr.b, scale, curr.a, curr.use_camera);
			}

			// View 3
			App->render->SetViewPort(view4_3);

			for (p2PQueue_item<layer_blit>* curr = layer_list3.start; curr != nullptr; curr = curr->next)
			{
				float blit_scale = (curr->data.scale != -1.0f) ? curr->data.scale : scale;
				if(curr->data.use_camera)
					App->render->Blit(curr->data.texture, curr->data.pos.x + camera3.x, curr->data.pos.y + camera3.y, &curr->data.section, blit_scale, curr->data.use_camera, curr->data.flip, curr->data.angle, curr->data.pivot_x, curr->data.pivot_y);
				else
					App->render->Blit(curr->data.texture, curr->data.pos.x, curr->data.pos.y, &curr->data.section, blit_scale, curr->data.use_camera, curr->data.flip, curr->data.angle, curr->data.pivot_x, curr->data.pivot_y);
			}

			for (p2PQueue_item<layer_quad>* curr = quad_list3.start; curr != nullptr; curr = curr->next)
			{
				App->render->DrawQuad({ curr->data.rect.x + camera3.x, curr->data.rect.y + camera3.y, curr->data.rect.w, curr->data.rect.h }, curr->data.r, curr->data.g, curr->data.b, scale, curr->data.a, curr->data.filled, curr->data.use_camera);
			}

			for (int i = 0; i < line_list.size(); i++)
			{
				layer_line curr = line_list.at(i);
				App->render->DrawLine(curr.x1 + camera3.x, curr.y1 + camera3.y, curr.x2 + camera3.x, curr.y2 + camera3.y, curr.r, curr.g, curr.b, scale, curr.a, curr.use_camera);
			}

			for (int i = 0; i < circle_list.size(); i++)
			{
				layer_circle curr = circle_list.at(i);
				App->render->DrawCircle(curr.x1 + camera3.x, curr.y1 + camera3.y, curr.redius, curr.r, curr.g, curr.b, scale, curr.a, curr.use_camera);
			}

			// View 4
			App->render->SetViewPort(view4_4);

			for (p2PQueue_item<layer_blit>* curr = layer_list4.start; curr != nullptr; curr = curr->next)
			{
				float blit_scale = (curr->data.scale != -1.0f) ? curr->data.scale : scale;
				if(curr->data.use_camera)
					App->render->Blit(curr->data.texture, curr->data.pos.x + camera4.x, curr->data.pos.y + camera4.y, &curr->data.section, blit_scale, curr->data.use_camera, curr->data.flip, curr->data.angle, curr->data.pivot_x, curr->data.pivot_y);
				else
					App->render->Blit(curr->data.texture, curr->data.pos.x, curr->data.pos.y, &curr->data.section, blit_scale, curr->data.use_camera, curr->data.flip, curr->data.angle, curr->data.pivot_x, curr->data.pivot_y);
			}

			for (p2PQueue_item<layer_quad>* curr = quad_list4.start; curr != nullptr; curr = curr->next)
			{
				App->render->DrawQuad({ curr->data.rect.x + camera4.x, curr->data.rect.y + camera4.y, curr->data.rect.w, curr->data.rect.h }, curr->data.r, curr->data.g, curr->data.b, scale, curr->data.a, curr->data.filled, curr->data.use_camera);
			}

			for (int i = 0; i < line_list.size(); i++)
			{
				layer_line curr = line_list.at(i);
				App->render->DrawLine(curr.x1 + camera4.x, curr.y1 + camera4.y, curr.x2 + camera4.x, curr.y2 + camera4.y, curr.r, curr.g, curr.b, scale, curr.a, curr.use_camera);
			}

			for (int i = 0; i < circle_list.size(); i++)
			{
				layer_circle curr = circle_list.at(i);
				App->render->DrawCircle(curr.x1 + camera4.x, curr.y1 + camera4.y, curr.redius, curr.r, curr.g, curr.b, scale, curr.a, curr.use_camera);
			}

			App->render->ResetViewPort();
		}
		break;
	}

		layer_list1.Clear();
		layer_list2.Clear();
		layer_list3.Clear();
		layer_list4.Clear();

		quad_list1.Clear();
		quad_list2.Clear();
		quad_list3.Clear();
		quad_list4.Clear();

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
		int value = atoi((*it).c_str());
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

