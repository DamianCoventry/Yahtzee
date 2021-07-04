#include "stdafx.h"
#include "hwconfig.h"
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <fstream>

using namespace Utility;

HwConfig::HwConfig()
: window_width_(640)
, window_height_(480)
, bpp_(32)
, depth_bits_(24)
, refresh_rate_(75)
, fullscreen_(false)
, audio_enabled_(true)
, volume_(100)
, sps_(11025)
, bps_(8)
, channels_(1)
, network_port_(21112)
{
}

void HwConfig::Load(const std::string& filename)
{
    std::ifstream file(filename.c_str());
    if(!file)
    {
        return;
    }
    std::string line;
    while(std::getline(file, line))
    {
        if(line.empty())
        {
            continue;
        }
        std::string::size_type pos = line.find_first_of('=');
        if(pos == std::string::npos)
        {
            continue;
        }
        std::string key(boost::algorithm::trim_copy(line.substr(0, pos)));
        std::string value(boost::algorithm::trim_copy(line.substr(pos+1)));
        SetValue(key, value);
    }
}

void HwConfig::Save(const std::string& filename)
{
    std::ofstream file(filename.c_str(), std::ios_base::out | std::ios_base::trunc);
    if(!file)
    {
        return;
    }
    file << "WindowWidth = " << window_width_ << std::endl;
    file << "WindowHeight = " << window_height_ << std::endl;
    file << "WindowBpp = " << bpp_ << std::endl;
    file << "WindowDepthBits = " << depth_bits_ << std::endl;
    file << "WindowRefreshRate = " << refresh_rate_ << std::endl;
    file << "WindowFullscreen = " << (fullscreen_ ? "True" : "False") << std::endl;
    file << "AudioEnabled = " << (audio_enabled_ ? "True" : "False") << std::endl;
    file << "AudioVolume = " << volume_ << std::endl;
    file << "AudioSps = " << sps_ << std::endl;
    file << "AudioBps = " << bps_ << std::endl;
    file << "AudioChannels = " << channels_ << std::endl;
    file << "NetworkPort = " << network_port_ << std::endl;
}

void HwConfig::SetValue(const std::string& key, const std::string& value)
{
    if(boost::algorithm::iequals(key, "WindowWidth"))
    {
        try
        {
            window_width_ = boost::lexical_cast<int>(value);
        }
        catch(boost::bad_lexical_cast& )
        { }
        return;
    }
    if(boost::algorithm::iequals(key, "WindowHeight"))
    {
        try
        {
            window_height_ = boost::lexical_cast<int>(value);
        }
        catch(boost::bad_lexical_cast& )
        { }
        return;
    }
    if(boost::algorithm::iequals(key, "WindowBpp"))
    {
        try
        {
            bpp_ = boost::lexical_cast<int>(value);
        }
        catch(boost::bad_lexical_cast& )
        { }
    }
    if(boost::algorithm::iequals(key, "WindowDepthBits"))
    {
        try
        {
            depth_bits_ = boost::lexical_cast<int>(value);
        }
        catch(boost::bad_lexical_cast& )
        { }
        return;
    }
    if(boost::algorithm::iequals(key, "WindowRefreshRate"))
    {
        try
        {
            refresh_rate_ = boost::lexical_cast<int>(value);
        }
        catch(boost::bad_lexical_cast& )
        { }
        return;
    }
    if(boost::algorithm::iequals(key, "WindowFullscreen"))
    {
        fullscreen_ = boost::algorithm::iequals(value, "True");
        return;
    }
    if(boost::algorithm::iequals(key, "AudioEnabled"))
    {
        audio_enabled_ = boost::algorithm::iequals(value, "True");
        return;
    }
    if(boost::algorithm::iequals(key, "AudioVolume"))
    {
        try
        {
            volume_ = boost::lexical_cast<int>(value);
        }
        catch(boost::bad_lexical_cast& )
        { }
        return;
    }
    if(boost::algorithm::iequals(key, "AudioSps"))
    {
        try
        {
            sps_ = boost::lexical_cast<int>(value);
        }
        catch(boost::bad_lexical_cast& )
        { }
        return;
    }
    if(boost::algorithm::iequals(key, "AudioBps"))
    {
        try
        {
            bps_ = boost::lexical_cast<int>(value);
        }
        catch(boost::bad_lexical_cast& )
        { }
        return;
    }
    if(boost::algorithm::iequals(key, "AudioChannels"))
    {
        try
        {
            channels_ = boost::lexical_cast<int>(value);
        }
        catch(boost::bad_lexical_cast& )
        { }
        return;
    }
    if(boost::algorithm::iequals(key, "NetworkPort"))
    {
        try
        {
            network_port_ = boost::lexical_cast<unsigned short>(value);
        }
        catch(boost::bad_lexical_cast& )
        { }
        return;
    }
}
