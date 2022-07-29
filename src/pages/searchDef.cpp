#include "searchDef.h"
#include "definition.h"
#include "../../include/raygui.h"

SearchDef::SearchDef()
{
    modeChosen = new int(0);
    for (int i = 0; i < 50; i++)
        rec_result[i] = { 320, (float)200 + 125 * i, 830, 120 };
    for (int i = 0; i < 5; i++)
        rec_modes[i] = { 30, (float)125 + 90 * i, 245, 60 };
    rec_modes[menuChosen] = { 20, (float)115 + 90 * menuChosen, 265, 80 };
}

Screen SearchDef::update()
{
    if (GetMouseWheelMove() == -1 && rec_result[word.size() - 1].y > 475)
    {
        for (int i = 0; i < word.size(); i++)
        {
            rec_result[i].y -= 40;
        }
    }
    else if (GetMouseWheelMove() == 1 && rec_result[0].y < 200)
    {
        for (int i = 0; i < word.size(); i++)
        {
            rec_result[i].y += 40;
        }
    }
    if (IsMouseButtonPressed(0) && !dropDowmBox)
    {
        for (int i = 0; i < word.size(); i++)
        {
            if (GetMousePosition().y > 180 && CheckCollisionPointRec(GetMousePosition(), rec_result[i]))
            {
                selectedWord = word[i];
                slang.getFullDefinition(selectedWord->data);

                for (int i = 0; i < 50; i++)
                    rec_result[i] = { 320, (float)200 + 125 * i, 830, 120 };
                return DEFINITION;
            }
        }
    }

    if (menuChosen != 1)
    {
        word.clear();
        short tmp = menuChosen;
        menuChosen = 1;
        return static_cast<Screen>(tmp);
    }
    return SEARCH_DEF;
}

void SearchDef::draw()
{
    Vector2 mousePos = GetMousePosition();

    for (int i = 0; i < 5; i++)
    {
        if (GuiButton(rec_modes[i], modes[i].c_str()))
            menuChosen = i;
    }
    for (int i = 0; i < word.size(); i++)
    {
        DrawRectangleRec(rec_result[i], DARKBLUE);
        if (CheckCollisionPointRec(mousePos, rec_result[i]) && mousePos.y > 180 && !dropDowmBox)
            DrawRectangleRec(rec_result[i], BLUE);

        DrawTextEx(fnt, word[i]->data.c_str(), { rec_result[i].x + 10, rec_result[i].y + 10 }, 34, 2, WHITE);
        for (int j = 0; j < std::min(2, int(word[i]->defs.size())); j++)
        {
            std::string s = word[i]->defs[j]->data;
            if (s.length() * 13 > rec_result[i].width - 13)
            {
                for (int k = 0; k < 3; k++)
                    s.insert(s.begin() + rec_result[i].width / 13 + k, '.');
                s.insert(s.begin() + rec_result[i].width / 13 + 3, '\0');
            }
            DrawTextEx(fnt, s.c_str(), { rec_result[i].x + 15, rec_result[i].y + 35 * j + 50 }, 25, 2, LIGHTGRAY);
        }
    }
    DrawRectangle(310, 100, 850, 90, RAYWHITE);
    DrawRectangleLinesEx(rec_search, 3, GREEN);
    if (GuiTextBox(rec_search, SearchInput, 200, SearchEdit))
    {
        SearchEdit ^= 1;
    }
    if (GuiDropdownBox(rec_dictionary, (dictionary[0] + "\n" + dictionary[1] + "\n" + dictionary[2] + "\n" + dictionary[3]).c_str(), modeChosen, dropDowmBox))
        dropDowmBox ^= 1;
    if (SearchInput[0] == '\0')
        DrawText("Search bar", 325, 135, 30, LIGHTGRAY);

    if (SearchEdit)
    {
        if (GetKeyPressed())
        {
            if (SearchInput[0] != '\0')
                word = slang.SearchDef(SearchInput);
            else word.clear();
            for (int i = 0; i < 50; i++)
                rec_result[i] = { 320, (float)200 + 125 * i, 830, 120 };
        }
    }
}
