#pragma once
#include <raylib.h>
#include "../include/raygui.h"
#include "utility/screen.h"
#include "pages/home.h"
#include "pages/favorite.h"
#include "pages/definition.h"

const int WIDTH = 1200, HEIGHT = 600;
Font fnt;
class Program
{
private:
    Rectangle rec_top{0, 0, 1200, 100};
    Home Home;
    Favorite Favorite;
    Definitionmenu Definitionmenu;
    Screen currScreen = HOME;
public:
    Program()
    {
        fnt = LoadFont("CS163_github/data/Font.ttf");
        Font font = fnt;
        font.baseSize /= 1.4;
        GuiSetFont(font);
        GuiSetStyle(TEXTBOX, BASE_COLOR_PRESSED, 0xffffffff);
    }
    void run()
    {
        switch (currScreen)
        {
        case HOME:
            currScreen = Home.update();
            break;
        case FAVORITE:
            currScreen = Favorite.update();
            break;
        case DEFINITION:
            currScreen = Definitionmenu.update();
            break;
        default:
            break;
        }
        BeginDrawing();
        ClearBackground(RAYWHITE);
        switch (currScreen)
        {
        case HOME:
            Home.draw();
            break;
        case FAVORITE:
            Favorite.draw();
            break;
        case DEFINITION:
            Definitionmenu.draw();
            break;
        default:
            break;
        }
        DrawRectangleRec(rec_top, DARKBLUE);
        DrawText("CS163_GROUP8_DICTIONARY", 390, 60, 28, WHITE);
        EndDrawing();
    }
};