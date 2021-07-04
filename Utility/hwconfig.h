#ifndef INCLUDED_UTILITY_HWCONFIG
#define INCLUDED_UTILITY_HWCONFIG

#include <string>

namespace Utility
{

class HwConfig
{
public:
    HwConfig();

    void Load(const std::string& filename);
    void Save(const std::string& filename);

    void SetWindowWidth(int window_width)       { window_width_ = window_width; }
    void SetWindowHeight(int window_height)     { window_height_ = window_height; }
    void SetBpp(int bpp)                        { bpp_ = bpp; }
    void SetDepthBits(int depth_bits)           { depth_bits_ = depth_bits; }
    void SetRefreshRate(int refresh_rate)       { refresh_rate_ = refresh_rate; }
    void SetFullscreen(bool fullscreen)          { fullscreen_ = fullscreen; }

    void SetAudioEnabled(bool enabled)  { audio_enabled_ = enabled; }
    void SetVolume(int volume)          { volume_ = volume; }
    void SetSps(int sps)                { sps_ = sps; }
    void SetBps(int bps)                { bps_ = bps; }
    void SetChannels(int channels)      { channels_ = channels; }

    void SetNetworkPort(unsigned short port) { network_port_ = port; }

    int GetWindowWidth() const      { return window_width_; }
    int GetWindowHeight() const     { return window_height_; }
    int GetBpp() const              { return bpp_; }
    int GetDepthBits() const        { return depth_bits_; }
    int GetRefreshRate() const      { return refresh_rate_; }
    bool IsFullscreen() const       { return fullscreen_; }

    bool IsAudioEnabled() const     { return audio_enabled_; }
    int GetVolume() const           { return volume_; }
    int GetSps() const              { return sps_; }
    int GetBps() const              { return bps_; }
    int GetChannels() const         { return channels_; }

    unsigned short GetNetworkPort() const { return network_port_; }

private:
    void SetValue(const std::string& key, const std::string& value);

private:
    int window_width_;
    int window_height_;
    int bpp_;
    int depth_bits_;
    int refresh_rate_;
    bool fullscreen_;

    bool audio_enabled_;
    int volume_;
    int sps_;
    int bps_;
    int channels_;

    unsigned short network_port_;
};

}       // namespace Utility

#endif  // INCLUDED_UTILITY_HWCONFIG
