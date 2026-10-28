#pragma once

#include "raylib.h"
#include "Game/MegaMan.h"

enum class TextureType
{
	Logo = 0,
	Map1,

	IconLifes,
};

class TextureManager
{
public:
	TextureManager();

	void InitTexturesFiles();
	void UnloadTexturesFiles();

	//Functons For textures
	Texture2D& GetTexture(TextureType texture);


	void DrawTextureExCustom(Texture2D texture, Vector2 position, float rotation, float scale, Color tint);
	void DrawTextureExCustomFromBottom(Texture2D texture, Vector2 position, float rotation, float scale, Color tint);
	void DrawTextureRecCustom(Texture2D texture, Rectangle Rectangle, Vector2 position, Color tint);

private:
	Texture2D textLogo;
	Texture2D textMap1;

	//UI
	Texture2D textIconLifes;



};