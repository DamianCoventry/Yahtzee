#ifndef INCLUDED_ROLL
#define INCLUDED_ROLL

class Roll
{
public:
    Roll() { ThrowDice(); }

    void SetDiceValues(int dice_values[5]);
    void ThrowAYahtzee();
    void ThrowDice();

    int GetValue(int dice) const;

    int Sum(int value) const;
    int Sum() const;

    int Count(int value) const;
    int CountContiguous() const;
    bool IsYahtzee() const;

    void SetHold(int dice, bool hold = true);
    bool GetHold(int dice) const;
    void ToggleHold(int dice);
    void ClearHolds();

private:
    static const int NUM_DICE = 5;
    struct Dice
    {
        Dice() : value_(-1), hold_(false) {}
        int value_;
        bool hold_;
    };
    Dice dice_[NUM_DICE];
};

#endif  // INCLUDED_ROLL
