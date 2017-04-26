#include "j1Viewports.h"
#include "j1Window.h"
#include "j1Input.h"
#include "j1Console.h"
#include "p2Log.h"

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

	timer.Start();

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

	viewport_size = GetViewportRect(1);

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

SDL_Rect j1Viewports::GetViewportSize()
{
	return viewport_size;
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

iPoint j1Viewports::GetCameraPos(uint viewport)
{
	iPoint ret = NULLPOINT;

	switch (viewport)
	{
	case 1:
		ret = camera1;
		break;
	case 2:
		ret = camera2;
			break;
	case 3: 
		ret = camera3;
			break;
	case 4:
		ret = camera4;
		break;
	}

	return ret;
}

void j1Viewports::LayerBlit(int layer, SDL_Texture * texture, iPoint pos, const SDL_Rect section, int viewport, float scale, bool use_camera, SDL_RendererFlip _flip, double angle, int pivot_x, int pivot_y)
{
	layer_blit lblit(layer, texture, pos, section, viewport, scale, use_camera, _flip, angle, pivot_x, pivot_y);

	SDL_Rect view = GetViewportSize();

	if(viewport == 0 || viewport == 1)
	{
		if (use_camera && !(-camera1.x > pos.x + section.w || -camera1.x + view.w < pos.x || -camera1.y > pos.y + section.h || -camera1.y + view.h < pos.y))
			layer_list1.push(lblit);
		else if (!use_camera)
			layer_list1.push(lblit);
	}
	if (viewport == 0 || viewport == 2)
	{
		if (use_camera && !(-camera2.x > pos.x + section.w || -camera2.x + view.w < pos.x || -camera2.y > pos.y + section.h || -camera2.y + view.h < pos.y))
			layer_list2.push(lblit);
		else if (!use_camera)
			layer_list2.push(lblit);
	}
	if (viewport == 0 || viewport == 3)
	{
		if (use_camera && !(-camera3.x > pos.x + section.w || -camera3.x + view.w < pos.x || -camera3.y > pos.y + section.h || -camera3.y + view.h < pos.y))
			layer_list3.push(lblit);
		else if (!use_camera)
			layer_list3.push(lblit);
	}
	if (viewport == 0 || viewport == 4)
	{
		if (use_camera && !(-camera4.x > pos.x + section.w || -camera4.x + view.w < pos.x || -camera4.y > pos.y + section.h || -camera4.y + view.h < pos.y))
			layer_list4.push(lblit);
		else if (!use_camera)
			layer_list4.push(lblit);
	}
}

void j1Viewports::LayerDrawQuad(const SDL_Rect rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool filled, int layer, int viewport, bool use_camera)
{
	layer_quad q(layer, rect, r, g, b, a, filled, use_camera);

	switch (viewport)
	{
	case 1:
		quad_list1.push(q);
		break;
	case 2:
		quad_list2.push(q);
		break;
	case 3:
		quad_list3.push(q);
		break;
	case 4:
		quad_list4.push(q);
		break;
	default:
		quad_list1.push(q);
		if (number_of_views > 1)
			quad_list2.push(q);
		if (number_of_views > 3)
		{
			quad_list3.push(q);
			quad_list4.push(q);
		}
		break;
	}
}

void j1Viewports::LayerDrawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera)
{
	layer_line l(0, x1, y1, x2, y2, r, g, b, a, use_camera);
	line_list.push_back(l);
}

void j1Viewports::LayerDrawCircle(int x1, int y1, int redius, Uint8 r, Uint8 g, Uint8 b, Uint8 a, int layer, int viewport, bool filled, bool use_camera)
{
	layer_circle c(layer, x1, y1, redius, r, g, b, a, filled, use_camera);

	switch (viewport)
	{
	case 1:
		circle_list1.push(c);
		break;
	case 2:
		circle_list2.push(c);
		break;
	case 3:
		circle_list3.push(c);
		break;
	case 4:
		circle_list4.push(c);
		break;
	default:
		circle_list1.push(c);
		if (number_of_views > 1)
			circle_list2.push(c);
		if (number_of_views > 3)
		{
			circle_list3.push(c);
			circle_list4.push(c);
		}
		break;
	}
}

void j1Viewports::DoLayerPrint()
{
	// Viewports
		
	switch (number_of_views)
	{
		case 1:
		{
			while (!layer_list1.empty())
			{
				layer_blit curr = layer_list1.top();

				float blit_scale = (curr.scale != -1.0f) ? curr.scale : scale;
				if (curr.use_camera)
					App->render->Blit(curr.texture, curr.pos.x + camera1.x, curr.pos.y + camera1.y, &curr.section, blit_scale, curr.use_camera, curr.flip, curr.angle, curr.pivot_x, curr.pivot_y);
				else
					App->render->Blit(curr.texture, curr.pos.x, curr.pos.y, &curr.section, blit_scale, curr.use_camera, curr.flip, curr.angle, curr.pivot_x, curr.pivot_y);

				layer_list1.pop();
			}

			while (!quad_list1.empty())
			{
				layer_quad curr = quad_list1.top();
				if (curr.use_camera)
					App->render->DrawQuad({ curr.rect.x + camera1.x, curr.rect.y + camera1.y, curr.rect.w, curr.rect.h }, curr.r, curr.g, curr.b, scale, curr.a, curr.filled, curr.use_camera);
				else
					App->render->DrawQuad({ curr.rect.x, curr.rect.y, curr.rect.w, curr.rect.h }, curr.r, curr.g, curr.b, scale, curr.a, curr.filled, curr.use_camera);

				quad_list1.pop();
			}
		
			for (int i = 0; i < line_list.size(); i++)
			{
				layer_line curr = line_list.at(i);
				App->render->DrawLine(curr.x1 + camera1.x, curr.y1 + camera1.y, curr.x2 + camera1.x, curr.y2 + camera1.y, curr.r, curr.g, curr.b, scale, curr.a, curr.use_camera);
			}

			while (!circle_list1.empty())
			{
				layer_circle curr = circle_list1.top();
				App->render->DrawCircle(curr.x1 + camera1.x, curr.y1 + camera1.y, curr.redius, curr.r, curr.g, curr.b, scale, curr.a, curr.filled, curr.use_camera);

				circle_list1.pop();
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

			while (!layer_list1.empty())
			{
				layer_blit curr = layer_list1.top();

				float blit_scale = (curr.scale != -1.0f) ? curr.scale : scale;
				if (curr.use_camera)
					App->render->Blit(curr.texture, curr.pos.x + camera1.x, curr.pos.y + camera1.y, &curr.section, blit_scale, curr.use_camera, curr.flip, curr.angle, curr.pivot_x, curr.pivot_y);
				else
					App->render->Blit(curr.texture, curr.pos.x, curr.pos.y, &curr.section, blit_scale, curr.use_camera, curr.flip, curr.angle, curr.pivot_x, curr.pivot_y);

				layer_list1.pop();
			}

			while (!quad_list1.empty())
			{
				layer_quad curr = quad_list1.top();
				if (curr.use_camera)
					App->render->DrawQuad({ curr.rect.x + camera1.x, curr.rect.y + camera1.y, curr.rect.w, curr.rect.h }, curr.r, curr.g, curr.b, scale, curr.a, curr.filled, curr.use_camera);
				else
					App->render->DrawQuad({ curr.rect.x, curr.rect.y, curr.rect.w, curr.rect.h }, curr.r, curr.g, curr.b, scale, curr.a, curr.filled, curr.use_camera);

				quad_list1.pop();
			}

			for (int i = 0; i < line_list.size(); i++)
			{
				layer_line curr = line_list.at(i);
				App->render->DrawLine(curr.x1 + camera1.x, curr.y1 + camera1.y, curr.x2 + camera1.x, curr.y2 + camera1.y, curr.r, curr.g, curr.b, scale, curr.a, curr.use_camera);
			}

			while (!circle_list1.empty())
			{
				layer_circle curr = circle_list1.top();
				App->render->DrawCircle(curr.x1 + camera1.x, curr.y1 + camera1.y, curr.redius, curr.r, curr.g, curr.b, scale, curr.a, curr.filled, curr.use_camera);

				circle_list1.pop();
			}

			// View 2
			App->render->SetViewPort(view2_2);

			while (!layer_list2.empty())
			{
				layer_blit curr = layer_list2.top();

				float blit_scale = (curr.scale != -1.0f) ? curr.scale : scale;
				if (curr.use_camera)
					App->render->Blit(curr.texture, curr.pos.x + camera2.x, curr.pos.y + camera2.y, &curr.section, blit_scale, curr.use_camera, curr.flip, curr.angle, curr.pivot_x, curr.pivot_y);
				else
					App->render->Blit(curr.texture, curr.pos.x, curr.pos.y, &curr.section, blit_scale, curr.use_camera, curr.flip, curr.angle, curr.pivot_x, curr.pivot_y);

				layer_list2.pop();
			}

			while (!quad_list2.empty())
			{
				layer_quad curr = quad_list2.top();
				if (curr.use_camera)
					App->render->DrawQuad({ curr.rect.x + camera2.x, curr.rect.y + camera2.y, curr.rect.w, curr.rect.h }, curr.r, curr.g, curr.b, scale, curr.a, curr.filled, curr.use_camera);
				else
					App->render->DrawQuad({ curr.rect.x, curr.rect.y, curr.rect.w, curr.rect.h }, curr.r, curr.g, curr.b, scale, curr.a, curr.filled, curr.use_camera);

				quad_list2.pop();
			}

			while (!circle_list2.empty())
			{
				layer_circle curr = circle_list2.top();
				App->render->DrawCircle(curr.x1 + camera2.x, curr.y1 + camera2.y, curr.redius, curr.r, curr.g, curr.b, scale, curr.a, curr.filled, curr.use_camera);

				circle_list2.pop();
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

			while (!layer_list1.empty())
			{
				layer_blit curr = layer_list1.top();

				float blit_scale = (curr.scale != -1.0f) ? curr.scale : scale;
				if (curr.use_camera)
					App->render->Blit(curr.texture, curr.pos.x + camera1.x, curr.pos.y + camera1.y, &curr.section, blit_scale, curr.use_camera, curr.flip, curr.angle, curr.pivot_x, curr.pivot_y);
				else
					App->render->Blit(curr.texture, curr.pos.x, curr.pos.y, &curr.section, blit_scale, curr.use_camera, curr.flip, curr.angle, curr.pivot_x, curr.pivot_y);

				layer_list1.pop();
			}

			while (!quad_list1.empty())
			{
				layer_quad curr = quad_list1.top();
				if (curr.use_camera)
					App->render->DrawQuad({ curr.rect.x + camera1.x, curr.rect.y + camera1.y, curr.rect.w, curr.rect.h }, curr.r, curr.g, curr.b, scale, curr.a, curr.filled, curr.use_camera);
				else
					App->render->DrawQuad({ curr.rect.x, curr.rect.y, curr.rect.w, curr.rect.h }, curr.r, curr.g, curr.b, scale, curr.a, curr.filled, curr.use_camera);

				quad_list1.pop();
			}

			for (int i = 0; i < line_list.size(); i++)
			{
				layer_line curr = line_list.at(i);
				App->render->DrawLine(curr.x1 + camera1.x, curr.y1 + camera1.y, curr.x2 + camera1.x, curr.y2 + camera1.y, curr.r, curr.g, curr.b, scale, curr.a, curr.use_camera);
			}

			while (!circle_list1.empty())
			{
				layer_circle curr = circle_list1.top();
				App->render->DrawCircle(curr.x1 + camera1.x, curr.y1 + camera1.y, curr.redius, curr.r, curr.g, curr.b, scale, curr.a, curr.filled, curr.use_camera);

				circle_list1.pop();
			}

			// View 2
			App->render->SetViewPort(view4_2);

			while (!layer_list2.empty())
			{
				layer_blit curr = layer_list2.top();

				float blit_scale = (curr.scale != -1.0f) ? curr.scale : scale;
				if (curr.use_camera)
					App->render->Blit(curr.texture, curr.pos.x + camera2.x, curr.pos.y + camera2.y, &curr.section, blit_scale, curr.use_camera, curr.flip, curr.angle, curr.pivot_x, curr.pivot_y);
				else
					App->render->Blit(curr.texture, curr.pos.x, curr.pos.y, &curr.section, blit_scale, curr.use_camera, curr.flip, curr.angle, curr.pivot_x, curr.pivot_y);

				layer_list2.pop();
			}

			while (!quad_list2.empty())
			{
				layer_quad curr = quad_list2.top();
				if (curr.use_camera)
					App->render->DrawQuad({ curr.rect.x + camera2.x, curr.rect.y + camera2.y, curr.rect.w, curr.rect.h }, curr.r, curr.g, curr.b, scale, curr.a, curr.filled, curr.use_camera);
				else
					App->render->DrawQuad({ curr.rect.x, curr.rect.y, curr.rect.w, curr.rect.h }, curr.r, curr.g, curr.b, scale, curr.a, curr.filled, curr.use_camera);

				quad_list2.pop();
			}

			while (!circle_list2.empty())
			{
				layer_circle curr = circle_list2.top();
				App->render->DrawCircle(curr.x1 + camera2.x, curr.y1 + camera2.y, curr.redius, curr.r, curr.g, curr.b, scale, curr.a, curr.filled, curr.use_camera);

				circle_list2.pop();
			}

			// View 3
			App->render->SetViewPort(view4_3);

			while (!layer_list3.empty())
			{
				layer_blit curr = layer_list3.top();

				float blit_scale = (curr.scale != -1.0f) ? curr.scale : scale;
				if (curr.use_camera)
					App->render->Blit(curr.texture, curr.pos.x + camera3.x, curr.pos.y + camera3.y, &curr.section, blit_scale, curr.use_camera, curr.flip, curr.angle, curr.pivot_x, curr.pivot_y);
				else
					App->render->Blit(curr.texture, curr.pos.x, curr.pos.y, &curr.section, blit_scale, curr.use_camera, curr.flip, curr.angle, curr.pivot_x, curr.pivot_y);

				layer_list3.pop();
			}

			while (!quad_list3.empty())
			{
				layer_quad curr = quad_list3.top();
				if (curr.use_camera)
					App->render->DrawQuad({ curr.rect.x + camera3.x, curr.rect.y + camera3.y, curr.rect.w, curr.rect.h }, curr.r, curr.g, curr.b, scale, curr.a, curr.filled, curr.use_camera);
				else
					App->render->DrawQuad({ curr.rect.x, curr.rect.y, curr.rect.w, curr.rect.h }, curr.r, curr.g, curr.b, scale, curr.a, curr.filled, curr.use_camera);

				quad_list3.pop();
			}

			while (!circle_list3.empty())
			{
				layer_circle curr = circle_list3.top();
				App->render->DrawCircle(curr.x1 + camera3.x, curr.y1 + camera3.y, curr.redius, curr.r, curr.g, curr.b, scale, curr.a, curr.filled, curr.use_camera);

				circle_list3.pop();
			}

			// View 4
			App->render->SetViewPort(view4_4);

			while (!layer_list4.empty())
			{
				layer_blit curr = layer_list4.top();

				float blit_scale = (curr.scale != -1.0f) ? curr.scale : scale;
				if (curr.use_camera)
					App->render->Blit(curr.texture, curr.pos.x + camera4.x, curr.pos.y + camera4.y, &curr.section, blit_scale, curr.use_camera, curr.flip, curr.angle, curr.pivot_x, curr.pivot_y);
				else
					App->render->Blit(curr.texture, curr.pos.x, curr.pos.y, &curr.section, blit_scale, curr.use_camera, curr.flip, curr.angle, curr.pivot_x, curr.pivot_y);

				layer_list4.pop();
			}

			while (!quad_list4.empty())
			{
				layer_quad curr = quad_list4.top();
				if (curr.use_camera)
					App->render->DrawQuad({ curr.rect.x + camera4.x, curr.rect.y + camera4.y, curr.rect.w, curr.rect.h }, curr.r, curr.g, curr.b, scale, curr.a, curr.filled, curr.use_camera);
				else
					App->render->DrawQuad({ curr.rect.x, curr.rect.y, curr.rect.w, curr.rect.h }, curr.r, curr.g, curr.b, scale, curr.a, curr.filled, curr.use_camera);

				quad_list4.pop();
			}

			while (!circle_list4.empty())
			{
				layer_circle curr = circle_list4.top();
				App->render->DrawCircle(curr.x1 + camera4.x, curr.y1 + camera4.y, curr.redius, curr.r, curr.g, curr.b, scale, curr.a, curr.filled, curr.use_camera);

				circle_list4.pop();
			}

			App->render->ResetViewPort();
		}
		break;
	}

	line_list.clear();
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

void j1Viewports::ResetCameras()
{
	camera1.x = 0;
	camera1.y = 0;

	camera2.x = 0;
	camera2.y = 0;

	camera3.x = 0;
	camera3.y = 0;

	camera4.x = 0;
	camera4.y = 0;
}

