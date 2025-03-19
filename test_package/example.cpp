#include "Event_Model/EventSource.hpp"
#include "HaSLL/LoggerManager.hpp"
#include "REST_DCA/Adapter.hpp"
#include <iostream>

using namespace std;
using namespace HaSLL;
using namespace Information_Model;
using namespace Data_Consumer_Adapter;

class EventSourceFake : public Event_Model::EventSource<ModelRepositoryEvent> {
  void handleException(exception_ptr eptr) { // NOLINT
    if (eptr) {
      rethrow_exception(eptr);
    }
  }

public:
  EventSourceFake()
      : EventSource(
            bind(&EventSourceFake::handleException, this, placeholders::_1)) {}

  void sendEvent(const ModelRepositoryEventPtr& event) { notify(event); }
};
int main() {
  try {
    LoggerManager::initialise(makeDefaultRepository());
    auto event_source = make_shared<EventSourceFake>();
    auto adapter = Adapter(event_source);
    adapter.start();
    this_thread::sleep_for(chrono::seconds(1));
    adapter.stop();
    cout << "Integration Test Success" << endl;
    exit(EXIT_SUCCESS);
  } catch (const exception& ex) {
    cerr << ex.what();
    exit(EXIT_FAILURE);
  }
}
