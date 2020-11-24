#ifndef AUDIOPLAYERVIEW_HPP
#define AUDIOPLAYERVIEW_HPP

#include <gui_generated/audioplayer_screen/AudioPlayerViewBase.hpp>
#include <gui/audioplayer_screen/AudioPlayerPresenter.hpp>
#include <gui/containers/TextContainer.hpp>
#include <vector>

class AudioPlayerView : public AudioPlayerViewBase
{
public:
  AudioPlayerView();
  virtual ~AudioPlayerView() {}
  virtual void setupScreen();
  virtual void tearDownScreen();
  void show_initializing();
  void close_initializing();
  void add_element_to_playlist(std::vector<touchgfx::Unicode::UnicodeChar> a_song_name);

  virtual void play_button_clicked() override;
  virtual void pause_button_clicked() override;

  void set_play_state();
  void set_pause_state();

  void set_track_slider(double a_value);
  virtual void track_slider_value_changed(int a_value) override;
  virtual void volume_slider_value_changed(int a_value) override;
  touchgfx::GraphWrapAndClear<32>& graph() {return frequency_graph; };
private:
  Callback<AudioPlayerView, const std::vector<touchgfx::Unicode::UnicodeChar>&> play_song_callback;

  void play_song(const std::vector<touchgfx::Unicode::UnicodeChar>& a_song_name);
};

#endif // AUDIOPLAYERVIEW_HPP
