#pragma once
#include <limits>
#include <map>
#include <sdbusplus/sdbus.hpp>
#include <sdbusplus/server.hpp>
#include <string>
#include <systemd/sd-bus.h>

#include <com/example/Sensor/Value/common.hpp>

namespace sdbusplus::server::com::example::sensor
{

class Value :
    public sdbusplus::common::com::example::sensor::Value
{
    public:
        /* Define all of the basic class operations:
         *     Not allowed:
         *         - Default constructor to avoid nullptrs.
         *         - Copy operations due to internal unique_ptr.
         *         - Move operations due to 'this' being registered as the
         *           'context' with sdbus.
         *     Allowed:
         *         - Destructor.
         */
        Value() = delete;
        Value(const Value&) = delete;
        Value& operator=(const Value&) = delete;
        Value(Value&&) = delete;
        Value& operator=(Value&&) = delete;
        virtual ~Value() = default;

        /** @brief Constructor to put object onto bus at a dbus path.
         *  @param[in] bus - Bus to attach to.
         *  @param[in] path - Path to attach at.
         */
        Value(sdbusplus::bus_t& bus,
                     const char* path) :
            _com_example_sensor_Value_interface(
                bus, path, interface, _vtable, this),
            _sdbusplus_bus(bus) {}

        Value(sdbusplus::bus_t& bus,
                     const sdbusplus::object_path& path) :
            Value(bus, path.str.c_str()) {}

        /** @brief Constructor to initialize the object from a map of
         *         properties.
         *
         *  @param[in] bus - Bus to attach to.
         *  @param[in] path - Path to attach at.
         *  @param[in] vals - Map of property name to value for initialization.
         */
        Value(sdbusplus::bus_t& bus, const char* path,
                     const std::map<std::string, PropertiesVariant>& vals,
                     bool skipSignal = false) :
            Value(bus, path)
        {
            for (const auto& v : vals)
            {
                setPropertyByName(v.first, v.second, skipSignal);
            }
        }

        Value(sdbusplus::bus_t& bus,
                     const sdbusplus::object_path& path,
                     const std::map<std::string, PropertiesVariant>& vals,
                     bool skipSignal = false) :
            Value(bus, path.str.c_str(), vals, skipSignal) {}

        /** @brief Implementation for SetThresholds
         *  Set upper and lower alert thresholds.
         *
         *  @param[in] high - Upper threshold.
         *  @param[in] low - Lower threshold.
         */
        virtual void setThresholds(
            double high,
            double low) = 0;

        /** @brief Send signal 'ThresholdAlert'
         *
         *  Emitted when the sensor value crosses a threshold.
         *
         *  @param[in] sensorValue - The value that triggered the alert.
         *  @param[in] direction - Either "High" or "Low".
         */
        void thresholdAlert(
            double sensorValue = {},
            std::string direction = {});
        /** Get value of Value */
        virtual double value() const;
        /** Set value of Value with option to skip sending signal */
        virtual double value(double value,
               bool skipSignal);
        /** Set value of Value */
        virtual double value(double value);
        /** Get value of Unit */
        virtual std::string unit() const;
        /** Set value of Unit with option to skip sending signal */
        virtual std::string unit(std::string value,
               bool skipSignal);
        /** Set value of Unit */
        virtual std::string unit(std::string value);
        /** Get value of MaxValue */
        virtual double maxValue() const;
        /** Set value of MaxValue with option to skip sending signal */
        virtual double maxValue(double value,
               bool skipSignal);
        /** Set value of MaxValue */
        virtual double maxValue(double value);
        /** Get value of MinValue */
        virtual double minValue() const;
        /** Set value of MinValue with option to skip sending signal */
        virtual double minValue(double value,
               bool skipSignal);
        /** Set value of MinValue */
        virtual double minValue(double value);

        /** @brief Sets a property by name.
         *  @param[in] _name - A string representation of the property name.
         *  @param[in] val - A variant containing the value to set.
         */
        void setPropertyByName(const std::string& _name,
                               const PropertiesVariant& val,
                               bool skipSignal = false);

        /** @brief Gets a property by name.
         *  @param[in] _name - A string representation of the property name.
         *  @return - A variant containing the value of the property.
         */
        PropertiesVariant getPropertyByName(const std::string& _name);



        /** @brief Emit interface added */
        void emit_added()
        {
            _com_example_sensor_Value_interface.emit_added();
        }

        /** @brief Emit interface removed */
        void emit_removed()
        {
            _com_example_sensor_Value_interface.emit_removed();
        }

        /** @return the bus instance */
        sdbusplus::bus_t& get_bus()
        {
            return  _sdbusplus_bus;
        }

    private:
        /** @brief sd-bus callback for SetThresholds
         */
        static int _callback_SetThresholds(
            sd_bus_message*, void*, sd_bus_error*);

        /** @brief sd-bus callback for get-property 'Value' */
        static int _callback_get_Value(
            sd_bus*, const char*, const char*, const char*,
            sd_bus_message*, void*, sd_bus_error*);
        /** @brief sd-bus callback for set-property 'Value' */
        static int _callback_set_Value(
            sd_bus*, const char*, const char*, const char*,
            sd_bus_message*, void*, sd_bus_error*);

        /** @brief sd-bus callback for get-property 'Unit' */
        static int _callback_get_Unit(
            sd_bus*, const char*, const char*, const char*,
            sd_bus_message*, void*, sd_bus_error*);
        /** @brief sd-bus callback for set-property 'Unit' */
        static int _callback_set_Unit(
            sd_bus*, const char*, const char*, const char*,
            sd_bus_message*, void*, sd_bus_error*);

        /** @brief sd-bus callback for get-property 'MaxValue' */
        static int _callback_get_MaxValue(
            sd_bus*, const char*, const char*, const char*,
            sd_bus_message*, void*, sd_bus_error*);
        /** @brief sd-bus callback for set-property 'MaxValue' */
        static int _callback_set_MaxValue(
            sd_bus*, const char*, const char*, const char*,
            sd_bus_message*, void*, sd_bus_error*);

        /** @brief sd-bus callback for get-property 'MinValue' */
        static int _callback_get_MinValue(
            sd_bus*, const char*, const char*, const char*,
            sd_bus_message*, void*, sd_bus_error*);
        /** @brief sd-bus callback for set-property 'MinValue' */
        static int _callback_set_MinValue(
            sd_bus*, const char*, const char*, const char*,
            sd_bus_message*, void*, sd_bus_error*);

        static const vtable_t _vtable[];
        sdbusplus::server::interface_t
                _com_example_sensor_Value_interface;
        sdbusplus::bus_t&  _sdbusplus_bus;

        double _value = {};
        std::string _unit = {};
        double _maxValue = {};
        double _minValue = {};
};

} // namespace sdbusplus::server::com::example::sensor

#ifndef SDBUSPP_REMOVE_DEPRECATED_NAMESPACE
namespace sdbusplus::com::example::Sensor::server {

using sdbusplus::server::com::example::sensor::Value;

} // namespace sdbusplus::com::example::Sensor::server
#endif

