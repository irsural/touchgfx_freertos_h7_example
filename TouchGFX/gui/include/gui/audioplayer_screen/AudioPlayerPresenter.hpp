#ifndef AUDIOPLAYERPRESENTER_HPP
#define AUDIOPLAYERPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

#include <vector>
#include <memory>

#include "read_dir_thread.h"
#include "play_wav_thread.h"
#include "fft_thread.h"

using namespace touchgfx;

class AudioPlayerView;

class AudioPlayerPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    AudioPlayerPresenter(AudioPlayerView& v);

    /**
     * The activate function is called automatically when this screen is "switched in"
     * (ie. made active). Initialization logic can be placed here.
     */
    virtual void activate();

    /**
     * The deactivate function is called automatically when this screen is "switched out"
     * (ie. made inactive). Teardown functionality can be placed here.
     */
    virtual void deactivate();

    virtual ~AudioPlayerPresenter() {};
    virtual void presenter_tick() override;

    void initialize_file_system();

    void start_playback(const std::vector<touchgfx::Unicode::UnicodeChar>& a_song_name);
    bool resume();
    void pause();

    void set_track_position(int a_track_position);
    void set_volume(int a_volume);

private:
    AudioPlayerPresenter();

    AudioPlayerView& view;

    QueueHandle_t m_read_folred_queue;
    std::shared_ptr<read_dir_thread_t> m_read_wav_files_thread;

    QueueHandle_t m_play_wav_cmd_queue;
    QueueHandle_t m_percents_played_queue;
    std::shared_ptr<play_wav_thread_t> m_play_wav_thread;

    SemaphoreHandle_t m_fft_samples_ready_smph;
    SemaphoreHandle_t m_fft_samples_processed_smph;
    SemaphoreHandle_t m_fft_done_smph;
    std::shared_ptr<fft_thread_t> m_fft_thread;
};

#endif // AUDIOPLAYERPRESENTER_HPP
