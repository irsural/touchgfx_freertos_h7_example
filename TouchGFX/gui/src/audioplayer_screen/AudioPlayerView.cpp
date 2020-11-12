#include <gui/audioplayer_screen/AudioPlayerView.hpp>

AudioPlayerView::AudioPlayerView()
{
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
}

void AudioPlayerView::add_element_to_playlist(std::vector<touchgfx::Unicode::UnicodeChar> a_song_name)
{
  playlist_container.add_item(a_song_name);
}
