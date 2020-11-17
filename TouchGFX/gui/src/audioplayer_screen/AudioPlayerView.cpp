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
