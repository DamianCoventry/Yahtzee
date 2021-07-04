#ifndef INCLUDED_HIGHSCORES
#define INCLUDED_HIGHSCORES

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <vector>
#include <string>
#include <boost/shared_ptr.hpp>
#include "../Utility/logcontext.h"

class Highscores :
    public Utility::LogContext

{
public:
    Highscores(const std::string& filename);
    ~Highscores();

    bool IsNewHighscore(long score) const;
    int Insert(const std::string& name, long score);
    void Reset();

    void SetName(int index, const std::string& name);

    std::string GetName(int index) const;
    long GetScore(int index) const;
    SYSTEMTIME GetDateTime(int index) const;

    static const int NUM_HIGHSCORES = 10;

private:
    void Encrypt(const std::string& name, char* bytes, int num_bytes);
    std::string Decrypt(char* bytes, int num_bytes);

private:
    struct Highscore
    {
        std::string name_;
        long score_;
        SYSTEMTIME date_time_;
    };
    typedef std::vector<Highscore> HighscoreTable;
    HighscoreTable highscore_table_;
    std::string filename_;
};

typedef boost::shared_ptr<Highscores> HighscoresPtr;

#endif  // INCLUDED_HIGHSCORES
