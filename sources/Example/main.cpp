#include "Adapter.hpp"
#include "Event_Model/EventSource.hpp"
#include "HaSLL/LoggerManager.hpp"
#include "Information_Model/mocks/DeviceMockBuilder.hpp"
#include "Information_Model/mocks/Metric_MOCK.hpp"
#include <iostream>

using ::testing::NiceMock;
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
void registerDevices(const shared_ptr<EventSourceFake>& event_source);
void deregisterDevices(const shared_ptr<EventSourceFake>& event_source);

void runForTime(int durationSeconds) {
  cout << "Programm wird nach " << durationSeconds << " Sekunden gestoppt"
       << endl;
  this_thread::sleep_for(chrono::seconds(durationSeconds));
}

void waitForQuit() {
  cout << "Drücke 'Q' zum Beenden..." << endl;
  while (true) {
    if (tolower(cin.get()) == 'q') {
      cout << "Programm wird gestoppt..." << endl;
      break;
    }
    this_thread::sleep_for(chrono::seconds(1));
  }
}
int main(int argc, char* argv[]) {
  LoggerManager::initialise(makeDefaultRepository());

  auto event_source = make_shared<EventSourceFake>();
  auto adapter = Adapter(event_source);
  registerDevices(event_source);
  adapter.start();

  if (argc == 2) {
    int duration = stoi(argv[1]);
    cout << "Runner läuft für " << duration << "Sekunden" << endl;
    runForTime(duration);
  } else {
    waitForQuit();
  }
  adapter.stop();
  return 0;
}
const static vector<string> device_ids{"base_id_1", "base_id_2"};

Information_Model::NonemptyDevicePtr buildDevice1() {
  auto mock_builder =
      make_shared<Information_Model::testing::DeviceMockBuilder>();

  mock_builder->buildDeviceBase(device_ids[0], "Example 1",
      "This is an example temperature sensor system");
  { // Power group
    auto subgroup_1_ref_id = mock_builder->addDeviceElementGroup(
        "Power", "Groups information regarding the power supply");
    mock_builder->addReadableMetric(subgroup_1_ref_id, "Power",
        "Indicates if system is running on batter power",
        Information_Model::DataType::Boolean,
        []() -> DataVariant { return true; });
    auto subgroup_2_ref_id =
        mock_builder->addDeviceElementGroup(subgroup_1_ref_id, "State",
            "Groups information regarding the power supply");
    mock_builder->addReadableMetric(subgroup_2_ref_id, "Error",
        "Indicates the current error message, regarding power supply",
        Information_Model::DataType::String, []() -> DataVariant {
          return string("Main Power Supply Interrupted");
        });
    mock_builder->addWritableMetric(
        subgroup_2_ref_id, "Reset Power Supply",
        "Resets power supply and any related error messages",
        Information_Model::DataType::Boolean,
        [](const DataVariant&) { /*There is nothing to do*/ },
        []() -> DataVariant { return false; });
  }
  mock_builder->addReadableMetric("Temperature",
      "Current measured temperature value in °C",
      Information_Model::DataType::Double,
      []() -> DataVariant { return (double)20.1; }); // NOLINT

  return Information_Model::NonemptyDevicePtr(mock_builder->getResult());
}

Information_Model::NonemptyDevicePtr buildDevice2() {
  auto mock_builder =
      make_shared<Information_Model::testing::DeviceMockBuilder>();

  mock_builder->buildDeviceBase(device_ids[1], "Example 2",
      "This is an example power measurement sensor system");
  { // Phase 1 group
    auto subgroup_1_ref_id = mock_builder->addDeviceElementGroup(
        "Phase 1", "Groups first phase's power measurements");
    mock_builder->addReadableMetric(subgroup_1_ref_id, "Voltage",
        "Current measured phase voltage in V",
        Information_Model::DataType::Double, []() -> DataVariant {
          // NOLINTNEXTLINE(readability-magic-numbers)
          return (double)239.1;
        });
    mock_builder->addReadableMetric(subgroup_1_ref_id, "Current",
        "Current measured phase current in A",
        Information_Model::DataType::Double, []() -> DataVariant {
          // NOLINTNEXTLINE(readability-magic-numbers)
          return (double)8.8;
        });
  }
  { // Phase 2 group
    auto subgroup_1_ref_id = mock_builder->addDeviceElementGroup(
        "Phase 2", "Groups second phase's power measurements");
    mock_builder->addReadableMetric(subgroup_1_ref_id, "Voltage",
        "Current measured phase voltage in V",
        Information_Model::DataType::Double, []() -> DataVariant {
          // NOLINTNEXTLINE(readability-magic-numbers)
          return (double)239.1;
        });
    mock_builder->addReadableMetric(subgroup_1_ref_id, "Current",
        "Current measured phase current in A",
        Information_Model::DataType::Double, []() -> DataVariant {
          // NOLINTNEXTLINE(readability-magic-numbers)
          return (double)8.8;
        });
  }
  { // Phase 3 group
    auto subgroup_1_ref_id = mock_builder->addDeviceElementGroup(
        "Phase 3", "Groups third phase's power measurements");
    mock_builder->addReadableMetric(subgroup_1_ref_id, "Voltage",
        "Current measured phase voltage in V",
        Information_Model::DataType::Double, []() -> DataVariant {
          // NOLINTNEXTLINE(readability-magic-numbers)
          return (double)239.1;
        });
    mock_builder->addReadableMetric(subgroup_1_ref_id, "Current",
        "Current measured phase current in A",
        Information_Model::DataType::Double, []() -> DataVariant {
          // NOLINTNEXTLINE(readability-magic-numbers)
          return (double)8.8;
        });
  }
  return Information_Model::NonemptyDevicePtr(mock_builder->getResult());
}

void registerDevice(const Information_Model::NonemptyDevicePtr& device,
    const shared_ptr<EventSourceFake>& event_source) {

  event_source->sendEvent(std::make_shared<ModelRepositoryEvent>(device));
}

void registerDevices(const shared_ptr<EventSourceFake>& event_source) {
  registerDevice(buildDevice1(), event_source);
  registerDevice(buildDevice2(), event_source);
}

void deregisterDevices(const shared_ptr<EventSourceFake>& event_source) {
  for (const auto& device_id : device_ids) {
    cout << "Deregistrating device: " << device_id << endl;
    event_source->sendEvent( // deregistrade first device
        std::make_shared<ModelRepositoryEvent>(device_id));
    this_thread::sleep_for(5s);
  }
}
