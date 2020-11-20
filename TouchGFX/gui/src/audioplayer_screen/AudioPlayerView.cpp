#include <gui/audioplayer_screen/AudioPlayerView.hpp>

AudioPlayerView::AudioPlayerView() :
  play_song_callback(this, &AudioPlayerView::play_song)
{
  playlist_container.setItemChoosenCallback(play_song_callback);
//  frequency_graph.addDataPoint(43);
//  frequency_graph.addDataPoint(90);
}

void AudioPlayerView::setupScreen()
{
  AudioPlayerViewBase::setupScreen();
}

void AudioPlayerView::tearDownScreen()
{
  AudioPlayerViewBase::tearDownScreen();
}

void AudioPlayerView::show_initializing()
{
  init_modal_window.setVisible(true);
}

void AudioPlayerView::close_initializing()
{
  init_modal_window.setVisible(false);
  init_modal_window.invalidate();
}

void AudioPlayerView::add_element_to_playlist(std::vector<touchgfx::Unicode::UnicodeChar> a_song_name)
{
  playlist_container.add_item(a_song_name);
}

void AudioPlayerView::play_song(const std::vector<touchgfx::Unicode::UnicodeChar>& a_song_name)
{
  presenter->start_playback(a_song_name);
}

void AudioPlayerView::set_play_state()
{
  play_button.setVisible(false);
  pause_button.setVisible(true);
  play_button.invalidate();
  pause_button.invalidate();
}

void AudioPlayerView::set_track_slider(double a_value)
{
  track_slider.setValue(static_cast<int>(round(a_value * 10)));
}

void AudioPlayerView::track_slider_value_changed(int a_value)
{
  presenter->set_track_position(a_value);
}

void AudioPlayerView::volume_slider_value_changed(int a_value)
{
  presenter->set_volume(a_value);
}

void AudioPlayerView::set_pause_state()
{
  play_button.setVisible(true);
  pause_button.setVisible(false);
  play_button.invalidate();
  pause_button.invalidate();
}

void AudioPlayerView::play_button_clicked()
{
  if (presenter->resume()) {
    set_play_state();
  }
}

void AudioPlayerView::pause_button_clicked()
{
  presenter->pause();
  set_pause_state();
}
