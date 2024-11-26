#ifndef RestInterface_hpp
#define RestInterface_hpp

class RestInterface {
public:
  virtual ~RestInterface() = default;
  virtual void start() {
    throw std::runtime_error("Called base RestInterface::start() method");
  }
  virtual void stop() {
    throw std::runtime_error("Called base RestInterface:: stop() method");
  }
  virtual void add(Information_Model::NonemptyDevicePtr /*device*/) {
    throw std::runtime_error(
        "Called base RestInterface::add(Information_Model::NonemptyDevicePtr) "
        "method");
  }

  virtual void remove(const std::string& id) {
    throw std::runtime_error(
        "Called base RestInterface::remove(const std::string&) method")
  }
};
using RestInterfacePtr = std::shared_ptr<RestInterface>;

RestInterfacePtr makeRestInterface();
#endif // RestInterface_hpp