#include "TexturesManager.h"

TextureManager::TextureManager()
{

}


void TextureManager::InitTexturesFiles()
{
    // Menu
    textLogo = LoadTexture("resources/Menu/Title.png");

    //Maps Images
    textMap1 = LoadTexture("resources/Game/Stages/Level2.png");

    //UI Mario Lifes
    Image MarioLifesIconImg = LoadImage("resources/UI/guiMario.png");
    textIconLifes = LoadTextureFromImage(MarioLifesIconImg);
    UnloadImage(MarioLifesIconImg);

}

Texture2D& TextureManager::GetTexture(TextureType texturetype)
{
    switch (texturetype)
    {
    case TextureType::Logo:
        return textLogo;
        break;

    case TextureType::Map1:
        return textMap1;
        break;

    case TextureType::IconLifes:
        return textIconLifes;
        break;

    default:
        break;
    }
}

void TextureManager::DrawTextureExCustom(Texture2D texture, Vector2 position, float rotation, float scale, Color tint)
{
    Rectangle source = { 0.0f, 0.0f, (float)texture.width, (float)texture.height };
    Rectangle dest = { position.x, position.y, (float)texture.width * scale, (float)texture.height * scale };
    Vector2 origin = { (float)texture.width / 2.f, (float)texture.height / 2.f };

    DrawTexturePro(texture, source, dest, origin, rotation, tint);
}

void TextureManager::DrawTextureExCustomFromBottom(Texture2D texture, Vector2 position, float rotation, float scale, Color tint)
{
    Rectangle source = { 0.0f, 0.0f, (float)texture.width, (float)texture.height };
    Rectangle dest = { position.x, position.y, (float)texture.width * scale, (float)texture.height * scale };
    Vector2 origin = { (float)texture.width / 2.f, (float)texture.height };

    DrawTexturePro(texture, source, dest, origin, rotation, tint);
}


void TextureManager::DrawTextureRecCustom(Texture2D texture, Rectangle Rectangle, Vector2 position, Color tint)
{
    Vector2 origin = { position.x - (Rectangle.width/2.f) , position.y - (Rectangle.height / 2.f) };
    DrawTextureRec(texture, Rectangle, origin, tint);
}

void TextureManager::UnloadTexturesFiles()
{
    UnloadTexture(textLogo);
    UnloadTexture(textMap1);
    //UnloadTexture(textPacman);
    //UnloadTexture(textSpritePacman);
    //UnloadTexture(textSpritePacmanDead);
    //UnloadTexture(textIconLifes);
    //UnloadTexture(textSpriteEnemy);

}

