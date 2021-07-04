#include "stdafx.h"
#include "highscores.h"
#include <fstream>
#include "../Utility/logmacros.h"
#include "../Utility/exception.h"

namespace
{
    struct HighscoreDiskFormat
    {
        char name_[32];
        char garbage0_[12];
        long score_;
        char garbage1_[14];
        SYSTEMTIME date_time_;
        char garbage2_[16];
    };
}

Highscores::Highscores(const std::string& filename)
: Utility::LogContext("Highscores")
{
    filename_ = filename;
    highscore_table_.resize(NUM_HIGHSCORES);

    std::ifstream file(filename.c_str(), std::ios_base::binary | std::ios_base::in);
    if(file)
    {
        LOG("Loading [" << filename_ << "]");

        for(int i = 0; i < NUM_HIGHSCORES; i++)
        {
            HighscoreDiskFormat highscore;
            file.read((char*)&highscore, sizeof(HighscoreDiskFormat));
            if(file.gcount() != sizeof(HighscoreDiskFormat))
            {
                Reset();
                break;
            }
            highscore_table_[i].name_       = Decrypt(highscore.name_, 32);
            highscore_table_[i].score_      = highscore.score_;
            highscore_table_[i].date_time_  = highscore.date_time_;
        }
    }
    else
    {
        Reset();
    }
}

Highscores::~Highscores()
{
    std::ofstream file(filename_.c_str(), std::ios_base::binary | std::ios_base::trunc | std::ios_base::out);
    if(file)
    {
        LOG("Saving [" << filename_ << "]");

        for(int i = 0; i < NUM_HIGHSCORES; i++)
        {
            HighscoreDiskFormat highscore;
            memset(highscore.name_, 0, sizeof(char)*32);

            Encrypt(highscore_table_[i].name_, highscore.name_, 32);
            highscore.score_        = highscore_table_[i].score_;
            highscore.date_time_    = highscore_table_[i].date_time_;

            file.write((char*)&highscore, sizeof(HighscoreDiskFormat));
        }
    }
}

bool Highscores::IsNewHighscore(long score) const
{
    for(int i = NUM_HIGHSCORES-1; i >= 0; i--)
    {
        if(score < highscore_table_[i].score_)
        {
            return ((i+1) < NUM_HIGHSCORES);
        }
    }
    return true;
}

int Highscores::Insert(const std::string& name, long score)
{
    int i, insert_index = -1;
    for(i = NUM_HIGHSCORES-1; i >= 0 && insert_index == -1; i--)
    {
        if(score < highscore_table_[i].score_)
        {
            insert_index = i+1;
        }
    }
    if(insert_index >= NUM_HIGHSCORES)
    {
        // Then this score is too small to be in the table.
        return -1;
    }
    if(insert_index == -1)
    {
        // Then this score is larger than all other scores.
        insert_index = 0;
    }

    // Move all scores below this one down the table.
    for(i = NUM_HIGHSCORES-1; i > insert_index; i--)
    {
        highscore_table_[i] = highscore_table_[i-1];
    }

    // Set the new score
    highscore_table_[insert_index].name_       = name;
    highscore_table_[insert_index].score_      = score;
    ::GetLocalTime(&highscore_table_[insert_index].date_time_);

    LOG("Insert [" << name << "][" << score << "] at index [" << insert_index << "]");

    return insert_index;
}

void Highscores::Reset()
{
    static const std::string names[] =
    { "Damian", "Jane", "Bob", "Christie", "James", "Claire", "Peter", "Aimee", "Mel", "Zoe" };

    LOG("Resetting");

    for(int i = NUM_HIGHSCORES-1; i >= 0; i--)
    {
        highscore_table_[i].name_       = names[rand()%10];
        highscore_table_[i].score_      = 500 - ((i * 50) + ((rand()%50)-25));
        ::GetLocalTime(&highscore_table_[i].date_time_);
    }
}

void Highscores::SetName(int index, const std::string& name)
{
    if(index >= 0 && index < NUM_HIGHSCORES)
    {
        highscore_table_[index].name_ = name;
    }
}

std::string Highscores::GetName(int index) const
{
    if(index >= 0 && index < NUM_HIGHSCORES)
    {
        return highscore_table_[index].name_;
    }
    return std::string();
}

long Highscores::GetScore(int index) const
{
    if(index >= 0 && index < NUM_HIGHSCORES)
    {
        return highscore_table_[index].score_;
    }
    return -1;
}

SYSTEMTIME Highscores::GetDateTime(int index) const
{
    if(index >= 0 && index < NUM_HIGHSCORES)
    {
        return highscore_table_[index].date_time_;
    }
    SYSTEMTIME temp;
    memset(&temp, 0, sizeof(SYSTEMTIME));
    return temp;
}

void Highscores::Encrypt(const std::string& name, char* bytes, int num_bytes)
{
    for(int i = 0; i < num_bytes; i++)
    {
        if(i < int(name.size()))
        {
            bytes[i] = name[i] - 'A';
        }
        else
        {
            bytes[i] = 0 - 'A';
        }
    }
}

std::string Highscores::Decrypt(char* bytes, int num_bytes)
{
    std::string decyrpted;
    for(int i = 0; i < num_bytes; i++)
    {
        int c = bytes[i] + 'A';
        if(c)
        {
            decyrpted.push_back(c);
        }
    }
    return decyrpted;
}
