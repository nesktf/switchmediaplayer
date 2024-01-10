//
// Created by fang on 2022/8/12.
//

#pragma once

#include <borealis.hpp>
#include <borealis/core/singleton.hpp>

#include <mpv/client.h>
#include <mpv/render_gl.h>

enum MpvEvent {
  MPV_LOADED,
  MPV_START_FILE,
  MPV_RESUME,
  MPV_FILE_ERROR,
  MPV_EOF,
  MPV_STOP,
  MPV_LOADING_END,
  MPV_LOADING_START,
  MPV_PAUSE,
  MPV_UPDATE_DURATION,
  MPV_UPDATE_PROGRESS,
  MPV_CACHE_SPEED_CHANGE,
  MPV_VIDEO_SPEED_CHANGE
};

class MPV : public brls::Singleton<MPV> {
public:
  MPV();
  void init();
  void stop();
  void clean();

  void restart();
  void reset();

  void draw(brls::Rect rect, float alpha = 1.0);

  template <typename... Args>
  void command(Args &&...args) {
    const char* cmd[] = {args..., nullptr};
    brls::Logger::debug("MPV => COMMAND : {}", args...);
    if (mpv) mpv_command_async(mpv, 0, cmd);
  }

public:
  void seek(int64_t val, const std::string& flags);
  void togglePlay();
  void showOSD(const std::string& val, int duration);

  bool isPaused();
  bool isStopped() const;
  bool isValid();

  void setUrl(const std::string& url, const std::string& extra = "", const std::string& method = "replace", uint64_t userdata = 0);

  void setSpeed(double val);
  double getSpeed() const;

  void setDouble(const std::string& key, double val);
  double getDouble(const std::string& key);

  void setInt(const std::string& key, int64_t val);
  int64_t getInt(const std::string& key);

  std::string getString(const std::string& key);

  brls::Event<MpvEvent>* getEvent() { return &mpv_core_event; }
  brls::Event<uint64_t, int64_t>* getCommandReply() { return &mpv_command_reply; }
  brls::Event<std::string, void*>* getCustomEvent() { return &mpv_custom_event; }

private:
  void eventMainLoop();

private:
  mpv_handle* mpv = nullptr;
  mpv_render_context* mpv_context = nullptr;

  mpv_opengl_fbo mpv_fbo;
  int flip_y{1};
  mpv_render_param mpv_params[3] = {
    {MPV_RENDER_PARAM_OPENGL_FBO, &mpv_fbo},
    {MPV_RENDER_PARAM_FLIP_Y, &flip_y},
    {MPV_RENDER_PARAM_INVALID, nullptr},
  };

  int64_t duration = 0;
  int64_t video_progress = 0;
  double video_speed = 0;
  double playback_time = 0;
  int64_t cache_speed = 0;
  bool video_stopped = true;
  
  brls::Event<bool>::Subscription focus_sub;
  brls::Event<MpvEvent> mpv_core_event;
  brls::Event<uint64_t, int64_t> mpv_command_reply;
  brls::Event<std::string, void*> mpv_custom_event;

private:
  static void on_wakeup(void* self);
  static void on_update(void* self);
};
