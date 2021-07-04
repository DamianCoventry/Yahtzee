#include "stdafx.h"
#include "scorecard.h"
#include "roll.h"

void Scorecard::Reset()
{
    ones_ = twos_ = threes_ = fours_ = fives_ = sixes_ = bonus_ = -1;
    three_of_a_kind_ = four_of_a_kind_ = full_house_ = small_straight_ = large_straight_ = yahtzee_ = bonus_yahtzee_ = chance_ = -1;
}

bool Scorecard::AwardOnes(const Roll& roll)
{
    if(ones_ != -1)
    {
        return false;
    }
    ones_ = roll.Sum(1);
    AwardBonus();
    return true;
}

bool Scorecard::AwardTwos(const Roll& roll)
{
    if(twos_ != -1)
    {
        return false;
    }
    twos_ = roll.Sum(2);
    AwardBonus();
    return true;
}

bool Scorecard::AwardThrees(const Roll& roll)
{
    if(threes_ != -1)
    {
        return false;
    }
    threes_ = roll.Sum(3);
    AwardBonus();
    return true;
}

bool Scorecard::AwardFours(const Roll& roll)
{
    if(fours_ != -1)
    {
        return false;
    }
    fours_ = roll.Sum(4);
    AwardBonus();
    return true;
}

bool Scorecard::AwardFives(const Roll& roll)
{
    if(fives_ != -1)
    {
        return false;
    }
    fives_ = roll.Sum(5);
    AwardBonus();
    return true;
}

bool Scorecard::AwardSixes(const Roll& roll)
{
    if(sixes_ != -1)
    {
        return false;
    }
    sixes_ = roll.Sum(6);
    AwardBonus();
    return true;
}

bool Scorecard::AwardThreeOfAKind(const Roll& roll)
{
    if(three_of_a_kind_ != -1)
    {
        return false;
    }
    three_of_a_kind_ = 0;
    for(int i = 1; i <= 6; i++)
    {
        if(roll.Count(i) >= 3)
        {
            three_of_a_kind_ = roll.Sum();
            break;
        }
    }
    return true;
}

bool Scorecard::AwardFourOfAKind(const Roll& roll)
{
    if(four_of_a_kind_ != -1)
    {
        return false;
    }
    four_of_a_kind_ = 0;
    for(int i = 1; i <= 6; i++)
    {
        if(roll.Count(i) >= 4)
        {
            four_of_a_kind_ = roll.Sum();
            break;
        }
    }
    return true;
}

bool Scorecard::AwardFullHouse(const Roll& roll)
{
    if(full_house_ != -1)
    {
        return false;
    }
    if(IsBonusYahtzee(roll))    // Bonus yahtzees are used as 'jokers', or 'wildcards'
    {
        full_house_ = 25;
        return true;
    }
    full_house_ = 0;
    int i, two = -1, three = -1;
    for(i = 1; i <= 6; i++)
    {
        if(roll.Count(i) == 3)
        {
            three = i;
            break;
        }
    }
    if(three != -1)
    {
        for(i = 1; i <= 6; i++)
        {
            if(i != three && roll.Count(i) == 2)
            {
                two = i;
                break;
            }
        }
        if(two != -1)
        {
            full_house_ = 25;
        }
    }
    return true;
}

bool Scorecard::AwardSmallStraight(const Roll& roll)
{
    if(small_straight_ != -1)
    {
        return false;
    }
    if(IsBonusYahtzee(roll))    // Bonus yahtzees are used as 'jokers', or 'wildcards'
    {
        small_straight_ = 30;
        return true;
    }
    small_straight_ = 0;
    if(roll.CountContiguous() >= 4)
    {
        small_straight_ = 30;
    }
    return true;
}

bool Scorecard::AwardLargeStraight(const Roll& roll)
{
    if(large_straight_ != -1)
    {
        return false;
    }
    if(IsBonusYahtzee(roll))    // Bonus yahtzees are used as 'jokers', or 'wildcards'
    {
        large_straight_ = 40;
        return true;
    }
    large_straight_ = 0;
    if(roll.CountContiguous() == 5)
    {
        large_straight_ = 40;
    }
    return true;
}

bool Scorecard::AwardYahtzee(const Roll& roll)
{
    if(yahtzee_ != -1)
    {
        return false;
    }
    yahtzee_ = 0;
    for(int i = 1; i <= 6; i++)
    {
        if(roll.Count(i) == 5)
        {
            yahtzee_ = 50;
            break;
        }
    }
    return true;
}

bool Scorecard::AwardBonusYahtzee(const Roll& roll)
{
    if(yahtzee_ <= 0)
    {
        return false;       // Can't get a bonus yahtzee until you've got a yahtzee
    }
    for(int i = 1; i <= 6; i++)
    {
        if(roll.Count(i) == 5)
        {
            bonus_yahtzee_ == -1 ?
                bonus_yahtzee_ = 100 : bonus_yahtzee_ += 100;
            return true;
        }
    }
    return false;
}

bool Scorecard::AwardChance(const Roll& roll)
{
    if(chance_ != -1)
    {
        return false;
    }
    chance_ = roll.Sum();
    return true;
}

std::string Scorecard::GetSubTotal() const
{
    int total = 0;
    if(ones_ != -1) total += ones_;
    if(twos_ != -1) total += twos_;
    if(threes_ != -1) total += threes_;
    if(fours_ != -1) total += fours_;
    if(fives_ != -1) total += fives_;
    if(sixes_ != -1) total += sixes_;
    return total == 0 ? std::string() : boost::lexical_cast<std::string>(total);
}

std::string Scorecard::GetUpperTotal() const
{
    int total = 0;
    if(ones_ != -1) total += ones_;
    if(twos_ != -1) total += twos_;
    if(threes_ != -1) total += threes_;
    if(fours_ != -1) total += fours_;
    if(fives_ != -1) total += fives_;
    if(sixes_ != -1) total += sixes_;
    if(bonus_ != -1) total += bonus_;
    return total == 0 ? std::string() : boost::lexical_cast<std::string>(total);
}

std::string Scorecard::GetYahtzee() const
{
    if(yahtzee_ == -1)
    {
        return std::string();
    }
    int total = yahtzee_;
    if(bonus_yahtzee_ != -1) total += bonus_yahtzee_;
    return boost::lexical_cast<std::string>(total);
}

int Scorecard::GetUpperTotal_i() const
{
    int total = 0;
    if(ones_ != -1) total += ones_;
    if(twos_ != -1) total += twos_;
    if(threes_ != -1) total += threes_;
    if(fours_ != -1) total += fours_;
    if(fives_ != -1) total += fives_;
    if(sixes_ != -1) total += sixes_;
    if(bonus_ != -1) total += bonus_;
    return total;
}

std::string Scorecard::GetLowerTotal() const
{
    int total = 0;
    if(three_of_a_kind_ != -1) total += three_of_a_kind_;
    if(four_of_a_kind_ != -1) total += four_of_a_kind_;
    if(full_house_ != -1) total += full_house_;
    if(small_straight_ != -1) total += small_straight_;
    if(large_straight_ != -1) total += large_straight_;
    if(yahtzee_ != -1) total += yahtzee_;
    if(bonus_yahtzee_ != -1) total += bonus_yahtzee_;
    if(chance_ != -1) total += chance_;
    return total == 0 ? std::string() : boost::lexical_cast<std::string>(total);
}

std::string Scorecard::GetGrandTotal() const
{
    int total = 0;

    if(ones_ != -1) total += ones_;
    if(twos_ != -1) total += twos_;
    if(threes_ != -1) total += threes_;
    if(fours_ != -1) total += fours_;
    if(fives_ != -1) total += fives_;
    if(sixes_ != -1) total += sixes_;
    if(bonus_ != -1) total += bonus_;

    if(three_of_a_kind_ != -1) total += three_of_a_kind_;
    if(four_of_a_kind_ != -1) total += four_of_a_kind_;
    if(full_house_ != -1) total += full_house_;
    if(small_straight_ != -1) total += small_straight_;
    if(large_straight_ != -1) total += large_straight_;
    if(yahtzee_ != -1) total += yahtzee_;
    if(bonus_yahtzee_ != -1) total += bonus_yahtzee_;
    if(chance_ != -1) total += chance_;

    return total == 0 ? std::string() : boost::lexical_cast<std::string>(total);
}

int Scorecard::GetGrandTotal_i() const
{
    int total = 0;

    if(ones_ != -1) total += ones_;
    if(twos_ != -1) total += twos_;
    if(threes_ != -1) total += threes_;
    if(fours_ != -1) total += fours_;
    if(fives_ != -1) total += fives_;
    if(sixes_ != -1) total += sixes_;
    if(bonus_ != -1) total += bonus_;

    if(three_of_a_kind_ != -1) total += three_of_a_kind_;
    if(four_of_a_kind_ != -1) total += four_of_a_kind_;
    if(full_house_ != -1) total += full_house_;
    if(small_straight_ != -1) total += small_straight_;
    if(large_straight_ != -1) total += large_straight_;
    if(yahtzee_ != -1) total += yahtzee_;
    if(bonus_yahtzee_ != -1) total += bonus_yahtzee_;
    if(chance_ != -1) total += chance_;

    return total;
}

bool Scorecard::IsUpperComplete() const
{
    return (ones_ != -1) && (twos_ != -1) && (threes_ != -1) && (fours_ != -1) && (fives_ != -1) && (sixes_ != -1);
}

bool Scorecard::IsLowerComplete() const
{
    return (three_of_a_kind_ != -1) && (four_of_a_kind_ != -1) && (full_house_ != -1) && (small_straight_ != -1)
            && (large_straight_ != -1) && (yahtzee_ != -1) && (chance_ != -1);
}

bool Scorecard::IsEverythingComplete() const
{
    return IsUpperComplete() && IsLowerComplete();
}

void Scorecard::AwardBonus()
{
    if(bonus_ == -1 && GetUpperTotal_i() >= BONUS_THRESHOLD)
    {
        bonus_ = BONUS_AMOUNT;
    }
}

bool Scorecard::IsBonusYahtzee(const Roll& roll) const
{
    return (roll.IsYahtzee() && yahtzee_ != -1);
}
