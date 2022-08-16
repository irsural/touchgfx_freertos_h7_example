#ifndef MODEL_HPP
#define MODEL_HPP

#include <touchgfx/Utils.hpp>
#include <sstream>
#include <vector>
#include <thread.h>

#include "defs.h"

class ModelListener;

/**
 * The Model class defines the data model in the model-view-presenter paradigm.
 * The Model is a singular object used across all presenters. The currently active
 * presenter will have a pointer to the Model through deriving from ModelListener.
 *
 * The Model will typically contain UI state information that must be kept alive
 * through screen transitions. It also usually provides the interface to the rest
 * of the system (the backend). As such, the Model can receive events and data from
 * the backend and inform the current presenter of such events through the modelListener
 * pointer, which is automatically configured to point to the current presenter.
 * Conversely, the current presenter can trigger events in the backend through the Model.
 */
class Model
{
public:
  Model();

  /**
   * Sets the modelListener to point to the currently active presenter. Called automatically
   * when switching screen.
   */
  void bind(ModelListener* listener)
  {
      modelListener = listener;
  }

  /**
   * This function will be called automatically every frame. Can be used to e.g. sample hardware
   * peripherals or read events from the surrounding system and inject events to the GUI through
   * the ModelListener interface.
   */
  void tick();

  void set_gui_thread(thread_base_t* a_thread);
  thread_base_t* get_gui_thread();
protected:
  /**
   * Pointer to the currently active presenter.
   */
  ModelListener* modelListener;

private:
  std::string m_debug_str;
  uint32_t m_debug_tick;

  thread_base_t* m_gui_thread;

  void show_mcu_load();
};

#endif /* MODEL_HPP */
