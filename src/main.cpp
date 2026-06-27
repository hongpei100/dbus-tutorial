#include <iostream>
#include <thread>
#include <random>
#include <sdbusplus/bus.hpp>
#include <sdbusplus/server.hpp>

// Generated from YAML
#include <com/example/Sensor/Value/server.hpp>

namespace sensor_server = sdbusplus::server::com::example::sensor;

class SensorService : public sensor_server::Value
{
  public:
    SensorService(sdbusplus::bus_t& bus, const char* path)
        : sensor_server::Value(bus, path),
          bus_(bus)
    {
        // skipSignal = true: 初始化時不發 PropertiesChanged
        value(0.0, true);
        unit("DegreesC", true);
        maxValue(80.0, true);
        minValue(10.0, true);
    }

    // YAML 裡定義了 SetThresholds method → codegen 生成 pure virtual
    // 你必須 implement 它
    void setThresholds(double high, double low) override
    {
        std::cout << "[Service] SetThresholds called: high=" << high
                  << " low=" << low << std::endl;
        maxValue(high);
        minValue(low);
    }

    void updateReading(double newValue)
    {
        double oldVal = value();
        value(newValue);  // 自動 emit PropertiesChanged signal
        std::cout << "[Service] Value updated: " << oldVal
                  << " -> " << newValue << std::endl;

        if (newValue > maxValue())
        {
            std::cout << "[Service] !!! HIGH threshold crossed !!!" << std::endl;
            thresholdAlert(newValue, "High");  // emit 自定義 signal
        }
        else if (newValue < minValue())
        {
            std::cout << "[Service] !!! LOW threshold crossed !!!" << std::endl;
            thresholdAlert(newValue, "Low");
        }
    }

  private:
    sdbusplus::bus_t& bus_;
};

int main()
{
    // new_user() → 連 session bus，不需要 root
    auto bus = sdbusplus::bus::new_user();

    constexpr auto busName = "com.example.Sensor";
    constexpr auto objPath = "/com/example/sensor/temperature/cpu0";

    bus.request_name(busName);
    std::cout << "Bus name acquired: " << busName << std::endl;

    SensorService sensor(bus, objPath);
    sensor.emit_added();
    std::cout << "Sensor service ready.\n" << std::endl;

    // 模擬 sensor 讀值
    std::thread simulator([&sensor]() {
        std::mt19937 rng(42);
        std::normal_distribution<double> dist(45.0, 15.0);
        for (int i = 0; i < 20; ++i)
        {
            std::this_thread::sleep_for(std::chrono::seconds(3));
            sensor.updateReading(dist(rng));
        }
    });

    // D-Bus event loop
    while (true)
    {
        bus.process_discard();
        bus.wait();
    }

    simulator.join();
    return 0;
}