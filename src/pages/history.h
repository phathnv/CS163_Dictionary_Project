#pragma once
#include "home.h"

class History
{
private:
    std::vector<Word *> word;
    Rectangle rec_result[20];
    Rectangle rec_dictionary{ 1030, 115, 155, 65 };
    Rectangle rec_modes[5];
    std::vector<std::string> modes = {"Search Word", "Search Def", "Favorite", "History", "Game"};
    std::vector<std::string> dictionary = {"ENG-ENG", "ENG-VIE", "SLANG", "EMOTIONAL"};
    short menuChosen = 3;
    bool dropDowmBox = false;

public:
    History()
    {
        for (int i = 0; i < 20; i++)
            rec_result[i] = {320, (float)200 + 125 * i, 830, 120};
        for (int i = 0; i < 5; i++)
            rec_modes[i] = {30, (float)125 + 90 * i, 245, 60};
        rec_modes[menuChosen] = {20, (float)115 + 90 * menuChosen, 265, 80};
    }
    Screen update();
    void draw();
};