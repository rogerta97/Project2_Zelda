#include "CreditsScene.h"
#include "j1Textures.h"
#include "j1Scene.h"
#include "j1Viewports.h"
#include "j1Window.h"
#include "p2Log.h"
#include "Mapping.h"
#include "j1Input.h"

CreditsScene::CreditsScene()
{
}

CreditsScene::~CreditsScene()
{
}

bool CreditsScene::Start()
{
	uint w, h; 

	int spacing = 45; 

	content.logo = App->tex->LoadTexture("textures/catpad_logo.png"); 
	logo_rect = { 0,0, 306, 306 }; 

	content.icons_tex = App->tex->LoadTexture("textures/icons.png");
	content.background_image = App->tex->LoadTexture("gui/credits_background.png"); 

	App->win->GetWindowSize(w, h);
	
	size = {0,0, (int)w, (int)h};

	window = App->gui->UI_CreateWin({0,0}, size.w, size.h, 0, false);

	content.logo_pos = iPoint(size.w / 4 - logo_rect.w / 2 - 10, size.h / 2 - logo_rect.h / 2 - 20);

	content.twitter_pos = { content.logo_pos.x - 45, content.logo_pos.y + 380 };
	content.github_pos = { content.logo_pos.x + 105, content.logo_pos.y + 380 };
	content.mail_pos = { content.logo_pos.x + 255, content.logo_pos.y + 380 };

	// rects

	content.twitter_logo_rect = { 192, 0, 96, 96 };
	content.github_logo_rect = { 96, 0,96,96 };
	content.mail_logo_rect = { 0,0,96,96 };

	// ---

	iPoint iban_line1_pos = {(int) size.w - (size.w/2 + 40),(int)size.h/2 - 270};
	iPoint iban_line2_pos = { iban_line1_pos.x, iban_line1_pos.y + spacing };
	iPoint ivan_line_pos = {iban_line2_pos.x, iban_line2_pos.y + spacing };
	iPoint guillem_line_pos = { ivan_line_pos.x, ivan_line_pos.y + spacing };
	iPoint domi_line_pos = { guillem_line_pos.x, guillem_line_pos.y + spacing };
	iPoint sergi_line_pos = { domi_line_pos.x, domi_line_pos.y + spacing };
	iPoint roger_line_pos = { sergi_line_pos.x, sergi_line_pos.y + spacing };

	iPoint special_thanks_text_pos = { roger_line_pos.x - 40 ,roger_line_pos.y + spacing*2 };

	iPoint voice_line_pos = { roger_line_pos.x, roger_line_pos.y + spacing*3 + 5 };
	iPoint voice_editor_pos = { voice_line_pos.x, voice_line_pos.y + spacing };

	iPoint twitter_logo_pos = { 0,0 }; 
	iPoint twitter_text_pos = { twitter_logo_pos.x + 20, twitter_logo_pos .y};

	iPoint github_logo_pos = { 0,0 };
	iPoint github_text_pos = { twitter_logo_pos.x + 20, twitter_logo_pos.y };
	
	iPoint mail_logo_pos = { 0,0 };
	iPoint mail_text_pos = { twitter_logo_pos.x + 20, twitter_logo_pos.y };

	content.iban_line_1 = window->CreateText(iban_line1_pos, App->font->game_font_60, 0, 0, 0,0,0);
	content.iban_line_1->SetText("· Team Leader: Iban Mas"); 

	content.iban_line_2 = window->CreateText(iban_line2_pos, App->font->game_font_60, 0, 0, 0, 0, 0);
	content.iban_line_2->SetText("· Management: Iban Mas");

	content.ivan_line = window->CreateText(ivan_line_pos, App->font->game_font_60, 0, 0, 0, 0, 0);
	content.ivan_line->SetText("· Art: Ivan Papiol");

	content.guillem_line = window->CreateText(guillem_line_pos, App->font->game_font_60, 0, 0, 0, 0, 0);
	content.guillem_line->SetText("· Code: Guillem Sunyer");

	content.domi_line = window->CreateText(domi_line_pos, App->font->game_font_60, 0, 0, 0, 0, 0);
	content.domi_line->SetText("· QA: Guillem Dominguez");

	content.sergi_line = window->CreateText(sergi_line_pos, App->font->game_font_60, 0, 0, 0, 0, 0);
	content.sergi_line->SetText("· Game Design: Sergi Perez");

	content.roger_line = window->CreateText(roger_line_pos, App->font->game_font_60, 0, 0, 0, 0,0);
	content.roger_line->SetText("· UI: Roger Tello");

	content.special_thanks = window->CreateText(special_thanks_text_pos, App->font->game_font_60, 0, 0, 0, 0,0);
	content.special_thanks->SetText("Special thanks to:"); 

	content.voice_line = window->CreateText(voice_line_pos, App->font->game_font_60, 0, 0, 0, 0,0);
	content.voice_line->SetText("· Jasmin Schaefer: Voice Acting"); 

	content.voice_editor_line = window->CreateText(voice_editor_pos, App->font->game_font_60, 0, 0, 0, 0,0);
	content.voice_editor_line->SetText("· Neku: Voice Editing"); 

	content.twitter_button = window->CreateButton(content.twitter_pos, 96,96);

	content.github_button = window->CreateButton(content.github_pos, 96, 96);

	content.mail_button = window->CreateButton(content.mail_pos, 96, 96);

	return false;
}

bool CreditsScene::Update(float dt)
{
	App->render->Blit(content.background_image, 0, 0);
	App->render->Blit(content.logo, content.logo_pos.x, content.logo_pos.y, &logo_rect);

	App->render->Blit(content.icons_tex, content.twitter_pos.x, content.twitter_pos.y, &content.twitter_logo_rect);
	App->render->Blit(content.icons_tex, content.github_pos.x, content.github_pos.y, &content.github_logo_rect);
	App->render->Blit(content.icons_tex, content.mail_pos.x, content.mail_pos.y, &content.mail_logo_rect);

	key_mapping back_key = App->scene->players[0].mapping->GetMapping(m_k_back); 

	if (App->input->GetControllerButton(0, back_key.key_id) == KEY_DOWN)
	{
		App->scene->ChangeScene((Scene*)App->scene->menu_scene); 
	}

	if (content.twitter_button->MouseEnter())
	{
		is_in_twitter = true;
	}
	else if(content.twitter_button->MouseOut())
		is_in_twitter = false; 

	// ---

	if (content.github_button->MouseEnter())
	{
		is_in_github = true;
	}
	else if(content.github_button->MouseOut())
		is_in_github = false;

	// ---

	if (content.mail_button->MouseEnter())
	{
		is_in_mail = true;
	}
	else if(content.mail_button->MouseOut())
		is_in_mail = false;


	if (is_in_twitter && content.twitter_button->MouseClickEnterLeft())
		App->OpenWebPage("https://twitter.com/catpad_games");

	if (is_in_github && content.github_button->MouseClickEnterLeft())
		App->OpenWebPage("https://github.com/rogerta97/Project2_Zelda");

	else if (is_in_mail && content.mail_button->MouseClickEnterLeft())
		App->OpenWebPage("https://rogerta97.github.io/Project2_Zelda");

	return false;
}

bool CreditsScene::CleanUp()
{
	if (App->scene->GetCurrentScene() != App->scene->credits_scene)
	{
		App->gui->DeleteElement(window);
	}

	App->tex->UnLoadTexture(content.logo); 
	
	return false;
}
