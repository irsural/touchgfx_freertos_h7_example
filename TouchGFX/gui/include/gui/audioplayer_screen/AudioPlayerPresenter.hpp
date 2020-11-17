#ifndef AUDIOPLAYERPRESENTER_HPP
#define AUDIOPLAYERPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

#include <vector>
#include <memory>

#include "read_dir_thread.h"

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

private:
    AudioPlayerPresenter();

    AudioPlayerView& view;

    QueueHandle_t m_read_folred_queue;
    std::shared_ptr<read_dir_thread_t> m_read_wav_files_thread;
};

#endif // AUDIOPLAYERPRESENTER_HPP
