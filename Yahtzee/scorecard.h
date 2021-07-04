#ifndef INCLUDED_SCORECARD
#define INCLUDED_SCORECARD

#include <boost/lexical_cast.hpp>
#include <string>

class Roll;

class Scorecard
{
public:
    Scorecard() { Reset(); }
    void Reset();

    bool AwardOnes(const Roll& roll);
    bool AwardTwos(const Roll& roll);
    bool AwardThrees(const Roll& roll);
    bool AwardFours(const Roll& roll);
    bool AwardFives(const Roll& roll);
    bool AwardSixes(const Roll& roll);

    bool AwardThreeOfAKind(const Roll& roll);
    bool AwardFourOfAKind(const Roll& roll);
    bool AwardFullHouse(const Roll& roll);
    bool AwardSmallStraight(const Roll& roll);
    bool AwardLargeStraight(const Roll& roll);
    bool AwardYahtzee(const Roll& roll);
    bool AwardBonusYahtzee(const Roll& roll);
    bool AwardChance(const Roll& roll);

    std::string GetOnes() const                 { return ones_ == -1 ? std::string() : boost::lexical_cast<std::string>(ones_); }
    std::string GetTwos() const                 { return twos_ == -1 ? std::string() : boost::lexical_cast<std::string>(twos_); }
    std::string GetThrees() const               { return threes_ == -1 ? std::string() : boost::lexical_cast<std::string>(threes_); }
    std::string GetFours() const                { return fours_ == -1 ? std::string() : boost::lexical_cast<std::string>(fours_); }
    std::string GetFives() const                { return fives_ == -1 ? std::string() : boost::lexical_cast<std::string>(fives_); }
    std::string GetSixes() const                { return sixes_ == -1 ? std::string() : boost::lexical_cast<std::string>(sixes_); }
    std::string GetSubTotal() const;
    std::string GetBonus() const                { return bonus_ == -1 ? std::string() : boost::lexical_cast<std::string>(bonus_); }
    std::string GetUpperTotal() const;

    std::string GetThreeOfAKind() const         { return three_of_a_kind_ == -1 ? std::string() : boost::lexical_cast<std::string>(three_of_a_kind_); }
    std::string GetFourOfAKind() const          { return four_of_a_kind_ == -1 ? std::string() : boost::lexical_cast<std::string>(four_of_a_kind_); }
    std::string GetFullHouse() const            { return full_house_ == -1 ? std::string() : boost::lexical_cast<std::string>(full_house_); }
    std::string GetSmallStraight() const        { return small_straight_ == -1 ? std::string() : boost::lexical_cast<std::string>(small_straight_); }
    std::string GetLargeStraight() const        { return large_straight_ == -1 ? std::string() : boost::lexical_cast<std::string>(large_straight_); }
    std::string GetYahtzee() const;
    std::string GetChance() const               { return chance_ == -1 ? std::string() : boost::lexical_cast<std::string>(chance_); }
    std::string GetLowerTotal() const;

    std::string GetGrandTotal() const;
    int GetGrandTotal_i() const;

    bool IsUpperComplete() const;
    bool IsLowerComplete() const;
    bool IsEverythingComplete() const;

private:
    void AwardBonus();
    int GetUpperTotal_i() const;
    bool IsBonusYahtzee(const Roll& roll) const;

    static const int BONUS_THRESHOLD = 63;
    static const int BONUS_AMOUNT    = 35;

private:
    int ones_;
    int twos_;
    int threes_;
    int fours_;
    int fives_;
    int sixes_;
    int bonus_;
    int three_of_a_kind_;
    int four_of_a_kind_;
    int full_house_;
    int small_straight_;
    int large_straight_;
    int yahtzee_;
    int bonus_yahtzee_;
    int chance_;
};

#endif      // INCLUDED_SCORECARD
