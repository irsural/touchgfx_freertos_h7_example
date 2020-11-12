#include <gui/audioplayer_screen/AudioPlayerView.hpp>
#include <gui/audioplayer_screen/AudioPlayerPresenter.hpp>
#include <cmsis_os2.h>
#include <vector>
#include "codecvt_oem866.h"
#include "cfg.h"
#include "file_operations.h"
#include "thread.h"

AudioPlayerPresenter::AudioPlayerPresenter(AudioPlayerView& v)
    : view(v)
{

}

void AudioPlayerPresenter::activate()
{
  view.show_initializing();
  initialize_file_system();
  view.close_initializing();
}

void AudioPlayerPresenter::initialize_file_system()
{
  if (cfg_t::instance().is_sd_detected()) {
    fatfs::dir root_dir("0:/");

    std::string file_name = root_dir.get_next();
    while (file_name != "") {
      std::vector<touchgfx::Unicode::UnicodeChar> unicode_name = oem866_to_unicode(file_name.c_str());
      view.add_element_to_playlist(std::move(unicode_name));
      file_name = root_dir.get_next();
    }
  }
}


void AudioPlayerPresenter::deactivate()
{

}
