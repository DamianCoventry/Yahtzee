#include "stdafx.h"
#include "roll.h"
#include <stdlib.h>

void Roll::SetDiceValues(int dice_values[5])
{
    dice_[0].value_ = dice_values[0];
    dice_[1].value_ = dice_values[1];
    dice_[2].value_ = dice_values[2];
    dice_[3].value_ = dice_values[3];
    dice_[4].value_ = dice_values[4];
}

void Roll::ThrowAYahtzee()
{
    dice_[0].value_ = rand()%6+1;
    dice_[1].value_ = dice_[0].value_;
    dice_[2].value_ = dice_[0].value_;
    dice_[3].value_ = dice_[0].value_;
    dice_[4].value_ = dice_[0].value_;
}

void Roll::ThrowDice()
{
    for(int i = 0; i < NUM_DICE; i++)
    {
        if(!dice_[i].hold_)
        {
            dice_[i].value_ = rand()%6+1;
        }
    }
}

int Roll::GetValue(int dice) const
{
    if(dice >= 0 && dice < NUM_DICE)
    {
        return dice_[dice].value_;
    }
    return -1;
}

int Roll::Sum(int value) const
{
    int sum = 0;
    for(int i = 0; i < NUM_DICE; i++)
    {
        if(dice_[i].value_ == value) sum += value;
    }
    return sum;
}

int Roll::Sum() const
{
    return dice_[0].value_ + dice_[1].value_ + dice_[2].value_ + dice_[3].value_ + dice_[4].value_;
}

int Roll::Count(int value) const
{
    int count = 0;
    for(int i = 0; i < NUM_DICE; i++)
    {
        if(dice_[i].value_ == value) count++;
    }
    return count;
}

int Roll::CountContiguous() const
{
    int largest_run = 0;
    for(int value = 1; value <= 6; value++)
    {
        int run = 0;
        for(int searching_for = value; searching_for <= 6; searching_for++)
        {
            bool found = false;
            for(int i = 0; i < NUM_DICE && !found; i++)
            {
                if(dice_[i].value_ == searching_for)
                {
                    run++;
                    found = true;
                }
            }
            if(!found)
            {
                break;
            }
        }
        if(run > largest_run)
        {
            largest_run = run;
        }
    }
    return largest_run;
}

bool Roll::IsYahtzee() const
{
    for(int i = 1; i <= 6; i++)
    {
        if(Count(i) == 5) return true;
    }
    return false;
}

void Roll::SetHold(int dice, bool hold)
{
    if(dice >= 0 && dice < NUM_DICE)
    {
        dice_[dice].hold_ = hold;
    }
}

bool Roll::GetHold(int dice) const
{
    if(dice >= 0 && dice < NUM_DICE)
    {
        return dice_[dice].hold_;
    }
    return false;
}

void Roll::ToggleHold(int dice)
{
    if(dice >= 0 && dice < NUM_DICE)
    {
        dice_[dice].hold_ = !dice_[dice].hold_;
    }
}

void Roll::ClearHolds()
{
    for(int i = 0; i < NUM_DICE; i++)
    {
        dice_[i].hold_ = false;
    }
}
