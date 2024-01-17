//
// Created by fang on 2022/8/12.
//

#include "core/mpv.hpp"

#include <glad/glad.h>
#include <mpv/client.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace core {
static inline void check_error(int status) {
  if (status < 0) brls::Logger::error("MPV ERROR => {}", mpv_error_string(status));
}

static void* get_proc_address(void* u, const char* name) {
  glfwGetCurrentContext();
  return (void*)glfwGetProcAddress(name);
}

void MPV::on_wakeup(void* self) {
  MPV* mpv = reinterpret_cast<MPV*>(self);
  brls::sync([mpv]() { mpv->eventMainLoop(); });
}

void MPV::on_update(void* self) {
  MPV* mpv = reinterpret_cast<MPV*>(self);
  brls::sync([mpv]() { mpv_render_context_update(mpv->mpv_context); });
}


MPV::MPV() {
  init();
  brls::Application::getExitEvent()->subscribe([this]() {
    clean();
  });
  brls::Logger::debug("Inited MPV");
}

void MPV::init() {
  this->mpv = mpv_create();
  if (!mpv)
    brls::fatal("Error creating mpv handle");

  mpv_set_option_string(mpv, "ytdl", "no");
  mpv_set_option_string(mpv, "osd-level", "0");

  mpv_set_option_string(mpv, "cache", "no");

#if defined(__SWITCH__)
  mpv_set_option_string(mpv, "vd-lavc-dr", "yes");
  mpv_set_option_string(mpv, "vd-lavc-threads", "3");
#endif
  mpv_set_option_string(mpv, "hwdec", "auto");

  mpv_set_option_string(mpv, "loop", "yes");

  if (mpv_initialize(mpv) < 0) {
    mpv_terminate_destroy(mpv);
    brls::fatal("Failed to init mpv");
  }

  check_error(mpv_observe_property(mpv, 1, "core-idle", MPV_FORMAT_FLAG));
  check_error(mpv_observe_property(mpv, 2, "pause", MPV_FORMAT_FLAG));
  check_error(mpv_observe_property(mpv, 3, "duration", MPV_FORMAT_INT64));
  check_error(mpv_observe_property(mpv, 4, "playback-time", MPV_FORMAT_DOUBLE));
  check_error(mpv_observe_property(mpv, 5, "cache-speed", MPV_FORMAT_INT64));
  check_error(mpv_observe_property(mpv, 9, "speed", MPV_FORMAT_DOUBLE));

  mpv_opengl_init_params gl_init_params{get_proc_address, nullptr};
  mpv_render_param params[] = {
    {MPV_RENDER_PARAM_API_TYPE, const_cast<char*>(MPV_RENDER_API_TYPE_OPENGL)},
    {MPV_RENDER_PARAM_OPENGL_INIT_PARAMS, &gl_init_params},
    {MPV_RENDER_PARAM_INVALID, nullptr},
  };

  if (mpv_render_context_create(&mpv_context, mpv, params) < 0) {
    mpv_terminate_destroy(mpv);
    brls::fatal("Failed to init mpv context");
  }

  command("set", "audio-client-name", "switchmp");
  mpv_set_wakeup_callback(mpv, on_wakeup, this);
  mpv_render_context_set_update_callback(mpv_context, on_update, this);

  focus_sub = brls::Application::getWindowFocusChangedEvent()->subscribe([this](bool focus) {
    static bool playing = false;
    if (!focus) {
      // app sleeping, save current state
      playing = !isPaused();
      command("set", "pause", "yes");
    }
    else if (playing) {
      command("set", "pause", "no");
    }
  });
}

void MPV::stop() { command("stop"); }

void MPV::clean() {
  check_error(mpv_command_string(mpv, "quit"));
  brls::Application::getWindowFocusChangedEvent()->unsubscribe(focus_sub);

  brls::Logger::info("trying to free mpv context");
  if (mpv_context) {
    mpv_render_context_free(mpv_context);
    mpv_context = nullptr;
  }

  brls::Logger::info("trying to terminate mpv");
  if (mpv) {
    mpv_terminate_destroy(mpv);
    mpv = nullptr;
  }
}

void MPV::restart() {
  clean();
  init();
}

void MPV::reset() {
  brls::Logger::debug("MPV::reset");
  video_stopped = true;
  duration = 0;
  cache_speed = 0;
  playback_time = 0;
  video_progress = 0;
}

void MPV::draw(brls::Rect rect, float alpha) {
  if (mpv_context == nullptr) return;

  if (alpha >= 1) {
    mpv_fbo.w = brls::Application::windowWidth;
    mpv_fbo.h = brls::Application::windowHeight;
    mpv_render_context_render(mpv_context, mpv_params);
    glViewport(0, 0, brls::Application::windowWidth, brls::Application::windowHeight);
    mpv_render_context_report_swap(mpv_context);
  }
}


void MPV::seek(int64_t val, const std::string& flags) {
  std::string pos = std::to_string(val);
  command("seek", pos.c_str(), flags.c_str());
}

void MPV::togglePlay() { command("cycle", "pause"); }

void MPV::showOSD(const std::string& val, int duration) {
  std::string dur = std::to_string(duration);
  command("show-text", val.c_str(), dur.c_str());
}


bool MPV::isPaused() {
  int ret = -1;
  mpv_get_property(mpv, "pause", MPV_FORMAT_FLAG, &ret);
  return ret == 1;
}

bool MPV::isStopped() const { return video_stopped; }

bool MPV::isValid() { return mpv_context != nullptr; }

void MPV::setUrl(const std::string& url, const std::string& extra, const std::string& method, uint64_t userdata) {
  brls::Logger::debug("{} URL: {}, extra: {}", method, url, extra);
  if (extra.empty()) {
    command("loadfile", url.c_str(), method.c_str());
    // const char* cmd[] = {"loadfile", url.c_str(), method.c_str(), nullptr};
    // mpv_command_async(mpv, userdata, cmd);
  } else {
    command("loadfile", url.c_str(), method.c_str(), extra.c_str());
    // const char* cmd[] = {"loadfile", url.c_str(), method.c_str(), extra.c_str(), nullptr};
    // mpv_command_async(mpv, userdata, cmd);
  }
}

void MPV::setSpeed(double val) {
  std::string speed = std::to_string(val);
  command("set", "speed", speed.c_str());
}

double MPV::getSpeed() const { return video_speed; }

void MPV::setDouble(const std::string& key, double val) {
  mpv_set_property_async(mpv, 0, key.c_str(), MPV_FORMAT_DOUBLE, &val);
}

double MPV::getDouble(const std::string& key) {
  double val = 0;
  mpv_get_property(mpv, key.c_str(), MPV_FORMAT_DOUBLE, &val);
  return val;
}

void MPV::setInt(const std::string& key, int64_t val) {
  mpv_set_property_async(mpv, 0, key.c_str(), MPV_FORMAT_INT64, &val);
}

int64_t MPV::getInt(const std::string& key) {
  int64_t val = 0;
  mpv_get_property(mpv, key.c_str(), MPV_FORMAT_INT64, &val);
  return val;
}

std::string MPV::getString(const std::string& key) {
  char* val = nullptr;
  mpv_get_property(mpv, key.c_str(), MPV_FORMAT_STRING, &val);
  if (!val) return "";
  std::string result = std::string{val};
  mpv_free(val);
  return result;
}


void MPV::eventMainLoop() {
  while (true) {
    mpv_event* event = mpv_wait_event(mpv, 0);
    switch (event->event_id) {
      case MPV_EVENT_NONE:
        return;
      case MPV_EVENT_LOG_MESSAGE: {
        auto log = (mpv_event_log_message*)event->data;
        if (log->log_level <= MPV_LOG_LEVEL_ERROR) {
          brls::Logger::error("{}: {}", log->prefix, log->text);
        } else if (log->log_level <= MPV_LOG_LEVEL_WARN) {
          brls::Logger::warning("{}: {}", log->prefix, log->text);
        } else if (log->log_level <= MPV_LOG_LEVEL_INFO) {
          brls::Logger::info("{}: {}", log->prefix, log->text);
        } else if (log->log_level <= MPV_LOG_LEVEL_V) {
          brls::Logger::info("{}: {}", log->prefix, log->text);
        } else {
          brls::Logger::verbose("{}: {}", log->prefix, log->text);
        }
        break;
      }
      case MPV_EVENT_SHUTDOWN:
        brls::Logger::debug("MPV => EVENT_SHUTDOWN");
        return;
      case MPV_EVENT_FILE_LOADED:
        brls::Logger::info("MPV => EVENT_FILE_LOADED");
        mpv_core_event.fire(MpvEvent::MPV_LOADED);
        break;
      case MPV_EVENT_START_FILE:
        brls::Logger::info("MPV => EVENT_PLAYBACK_RESTART");
        mpv_core_event.fire(MpvEvent::MPV_START_FILE);
        break;
      case MPV_EVENT_PLAYBACK_RESTART:
        brls::Logger::info("MPV => EVENT_PLAYBACK_RESTART");
        video_stopped = true;
        mpv_core_event.fire(MpvEvent::MPV_RESUME);
        break;
      case MPV_EVENT_END_FILE: {
        video_stopped = true;
        auto node = (mpv_event_end_file*)event->data;
        if (node->reason == MPV_END_FILE_REASON_ERROR) {
          brls::Logger::error("MPV => FILE ERROR {}", mpv_error_string(node->error));
          stop();
          mpv_core_event.fire(MpvEvent::MPV_FILE_ERROR);
        } else if(node->reason == MPV_END_FILE_REASON_EOF) {
          brls::Logger::info("MPV => EOF");
          mpv_core_event.fire(MpvEvent::MPV_EOF);
        } else {
          brls::Logger::info("MPV => STOP");
          mpv_core_event.fire(MpvEvent::MPV_STOP);
        }
        break;
      }
      case MPV_EVENT_COMMAND_REPLY: {
        mpv_event_command* cmd =  (mpv_event_command*)event->data;
        if (event->error) {
          brls::Logger::error("MPV => COMMAND ERROR: {}", mpv_error_string(event->error));
          break;
        }
        if (event->reply_userdata > 0 && cmd->result.format == MPV_FORMAT_NODE_MAP) {
          mpv_node_list* node_list = cmd->result.u.list;
          for (int i = 0; i < node_list->num; ++i) {
            std::string key = node_list->keys[i];
            auto& value = node_list->values[i];
            if (key == "playlist_entry_id" && value.format == MPV_FORMAT_INT64) {
              mpv_command_reply.fire(event->reply_userdata, value.u.int64);
            }
          }
        }
        break;
      }
      case MPV_EVENT_PROPERTY_CHANGE: {
        mpv_event_property* prop = (mpv_event_property*)event->data;
        if (prop->format == MPV_FORMAT_NONE) break;
        switch(event->reply_userdata) {
          case 1: { // core-idle
            if(!*(int*)prop->data) {
              mpv_core_event.fire(MpvEvent::MPV_LOADING_END);
            } else if (!video_stopped) {
              brls::Logger::debug("MPV => IDLE");
              mpv_core_event.fire(MpvEvent::MPV_LOADING_START);
            }
            break;
          }
          case 2: { // pause
            if (!!*(int*)prop->data) {
              brls::Logger::info("MPV => PAUSE");
              mpv_core_event.fire(MpvEvent::MPV_PAUSE);
            } else if (!video_stopped) {
              brls::Logger::info("MPV => RESUME");
              mpv_core_event.fire(MpvEvent::MPV_RESUME);
            }
            break;
          }
          case 3: { // duration
            duration = *(int64_t*)prop->data;
            if (duration >= 0) {
              brls::Logger::debug("MPV => DURATION: {}", duration);
              mpv_core_event.fire(MpvEvent::MPV_UPDATE_DURATION);
            }
            break;
          }
          case 4: { // playback-time
            playback_time = *(double*)prop->data;
            if (video_progress != (int64_t)playback_time) {
              video_progress = (int64_t)playback_time;
              mpv_core_event.fire(MpvEvent::MPV_UPDATE_PROGRESS);
            }
            break;
          }
          case 5: { // cache-speed
            cache_speed = *(int64_t*) prop->data;
            mpv_core_event.fire(MpvEvent::MPV_CACHE_SPEED_CHANGE);
            break;
          }
          case 9: { // speed
            video_speed = *(double*)prop->data;
            mpv_core_event.fire(MpvEvent::MPV_VIDEO_SPEED_CHANGE);
            break;
          }
          default: {
            brls::Logger::debug("MPV => PROPERTY_CHANGE {} type {}", prop->name, int(prop->format));
          }
        }
        break;
      }
      default:;
    }
  }
}

}
