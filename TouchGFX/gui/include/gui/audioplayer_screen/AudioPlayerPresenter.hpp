#ifndef AUDIOPLAYERPRESENTER_HPP
#define AUDIOPLAYERPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

#include <vector>

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

    void initialize_file_system();
    void get_song_name();

private:
    AudioPlayerPresenter();

    AudioPlayerView& view;
};

#endif // AUDIOPLAYERPRESENTER_HPP
