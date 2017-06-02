#include "CreditsScene.h"
#include "j1Textures.h"
#include "j1Scene.h"
#include "j1Window.h"
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
	
	int spacing = 20; 

	content.logo = App->tex->LoadTexture("textures/catpad_logo.png"); 

	App->win->GetWindowSize(w,h);

	window = App->gui->UI_CreateWin({0,0}, w,h);

	// rects

	SDL_Rect twitter_logo_rect = { 0,0,0,0 }; 
	SDL_Rect github_logo_rect = { 0,0,0,0 };
	SDL_Rect mail_logo_rect = { 0,0,0,0 };

	// ---

	iPoint catpad_logo_pos = {0,0};

	iPoint iban_line_pos = { 0,0 }; 
	iPoint ivan_line_pos = {iban_line_pos.x, iban_line_pos.y + spacing };
	iPoint guillem_line_pos = { ivan_line_pos.x, ivan_line_pos.y + spacing };
	iPoint domi_line_pos = { guillem_line_pos.x, guillem_line_pos.y + spacing };
	iPoint sergi_line_pos = { domi_line_pos.x, domi_line_pos.y + spacing };
	iPoint roger_line_pos = { sergi_line_pos.x, sergi_line_pos.y + spacing };

	iPoint special_thanks_text_pos = { 0,0 }; 

	iPoint voice_line_pos = { 0,0 };
	iPoint voice_editor_pos = { voice_line_pos.x, voice_line_pos.y + spacing };

	iPoint twitter_logo_pos = { 0,0 }; 
	iPoint twitter_text_pos = { twitter_logo_pos.x + 20, twitter_logo_pos .y};

	iPoint github_logo_pos = { 0,0 };
	iPoint github_text_pos = { twitter_logo_pos.x + 20, twitter_logo_pos.y };
	
	iPoint mail_logo_pos = { 0,0 };
	iPoint mail_text_pos = { twitter_logo_pos.x + 20, twitter_logo_pos.y };

	content.iban_line = window->CreateText(iban_line_pos, App->font->game_font_40);
	content.iban_line->SetText("- Team Leader & Management: Iban Mas"); 

	content.ivan_line = window->CreateText(ivan_line_pos, App->font->game_font_40);
	content.ivan_line->SetText("- Art: Ivan Papiol");

	content.guillem_line = window->CreateText(guillem_line_pos, App->font->game_font_40);
	content.guillem_line->SetText("- Code: Guillem Sunyer");

	content.domi_line = window->CreateText(domi_line_pos, App->font->game_font_40);
	content.domi_line->SetText("- QA: Guillem Dominguez");

	content.sergi_line = window->CreateText(sergi_line_pos, App->font->game_font_40);
	content.sergi_line->SetText("- Game Design: Sergi Perez");

	content.roger_line = window->CreateText(roger_line_pos, App->font->game_font_40);
	content.roger_line->SetText("- UI: Roger Tello");

	content.special_thanks = window->CreateText(special_thanks_text_pos, App->font->game_font_40);
	
	content.voice_line = window->CreateText(voice_line_pos, App->font->game_font_40);
	content.voice_editor_line = window->CreateText(voice_editor_pos, App->font->game_font_40);

	content.twitter_logo = window->CreateImage(twitter_logo_pos, twitter_logo_rect, false);
	content.twitter_text = window->CreateText(roger_line_pos, App->font->game_font_40);
	content.twitter_text->SetText("https://twitter.com/catpad_games");

	content.github_logo = window->CreateImage(github_logo_pos, github_logo_rect, false);
	content.github_text = window->CreateText(roger_line_pos, App->font->game_font_40);
	content.github_text->SetText("https://github.com/rogerta97/Project2_Zelda");

	content.mail_logo = window->CreateImage(mail_logo_pos, mail_logo_rect, false);
	content.mail_text = window->CreateText(roger_line_pos, App->font->game_font_40);
	content.mail_text->SetText("catpadgames@gmail.com");

	return false;
}

bool CreditsScene::Update(float dt)
{

	key_mapping back_key = App->scene->players[0].mapping->GetMapping(m_k_back); 

	if (App->input->GetControllerButton(0, back_key.key_id) == KEY_DOWN)
	{
		//tornar
	}

	return false;
}

bool CreditsScene::CleanUp()
{
	return false;
}
