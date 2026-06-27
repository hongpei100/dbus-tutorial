#include <exception>
#include <map>
#include <sdbusplus/sdbus.hpp>
#include <sdbusplus/sdbuspp_support/server.hpp>
#include <sdbusplus/server.hpp>
#include <string>
#include <tuple>

#include <com/example/Sensor/Value/server.hpp>

namespace sdbusplus::server::com::example::sensor
{

int Value::_callback_SetThresholds(
        sd_bus_message* msg, void* context, sd_bus_error* error)
{
    auto o = static_cast<Value*>(context);

    try
    {
        return sdbusplus::sdbuspp::method_callback(
                msg, o->get_bus().getInterface(), error,
                std::function(
                    [=](double&& high, double&& low)
                    {
                        return o->setThresholds(
                                high, low);
                    }
                ));
    }
    catch (const std::exception&)
    {
        o->get_bus().set_current_exception(std::current_exception());
        return 1;
    }
}

namespace details
{
namespace Value
{
static const auto _param_SetThresholds =
        utility::tuple_to_array(message::types::type_id<
                double, double>());
static const auto _return_SetThresholds =
        utility::tuple_to_array(message::types::type_id<
                >());
}
}


void Value::thresholdAlert(
            double sensorValue,
            std::string direction)
{
    auto& i = _com_example_sensor_Value_interface;
    auto m = i.new_signal("ThresholdAlert");

    m.append(sensorValue, direction);
    m.signal_send();
}

namespace details
{
namespace Value
{
static const auto _signal_ThresholdAlert =
        utility::tuple_to_array(message::types::type_id<
                double, std::string>());
}
}

auto Value::value() const ->
        double
{
    return _value;
}

int Value::_callback_get_Value(
        sd_bus* /*bus*/, const char* /*path*/, const char* /*interface*/,
        const char* /*property*/, sd_bus_message* reply, void* context,
        sd_bus_error* error)
{
    auto o = static_cast<Value*>(context);

    try
    {
        return sdbusplus::sdbuspp::property_callback(
                reply, o->get_bus().getInterface(), error,
                std::function(
                    [=]()
                    {
                        return o->value();
                    }
                ));
    }
    catch (const std::exception&)
    {
        o->get_bus().set_current_exception(std::current_exception());
        return 1;
    }
}

auto Value::value(double value,
                                         bool skipSignal) ->
        double
{
    if (_value != value)
    {
        _value = value;
        if (!skipSignal)
        {
            _com_example_sensor_Value_interface.property_changed("Value");
        }
    }

    return _value;
}

auto Value::value(double val) ->
        double
{
    return value(val, false);
}

int Value::_callback_set_Value(
        sd_bus* /*bus*/, const char* /*path*/, const char* /*interface*/,
        const char* /*property*/, sd_bus_message* value, void* context,
        sd_bus_error* error)
{
    auto o = static_cast<Value*>(context);

    try
    {
        return sdbusplus::sdbuspp::property_callback(
                value, o->get_bus().getInterface(), error,
                std::function(
                    [=](double&& arg)
                    {
                        o->value(std::move(arg));
                    }
                ));
    }
    catch (const std::exception&)
    {
        o->get_bus().set_current_exception(std::current_exception());
        return 1;
    }
}

namespace details
{
namespace Value
{
static const auto _property_Value =
    utility::tuple_to_array(message::types::type_id<
            double>());
}
}

auto Value::unit() const ->
        std::string
{
    return _unit;
}

int Value::_callback_get_Unit(
        sd_bus* /*bus*/, const char* /*path*/, const char* /*interface*/,
        const char* /*property*/, sd_bus_message* reply, void* context,
        sd_bus_error* error)
{
    auto o = static_cast<Value*>(context);

    try
    {
        return sdbusplus::sdbuspp::property_callback(
                reply, o->get_bus().getInterface(), error,
                std::function(
                    [=]()
                    {
                        return o->unit();
                    }
                ));
    }
    catch (const std::exception&)
    {
        o->get_bus().set_current_exception(std::current_exception());
        return 1;
    }
}

auto Value::unit(std::string value,
                                         bool skipSignal) ->
        std::string
{
    if (_unit != value)
    {
        _unit = value;
        if (!skipSignal)
        {
            _com_example_sensor_Value_interface.property_changed("Unit");
        }
    }

    return _unit;
}

auto Value::unit(std::string val) ->
        std::string
{
    return unit(val, false);
}

int Value::_callback_set_Unit(
        sd_bus* /*bus*/, const char* /*path*/, const char* /*interface*/,
        const char* /*property*/, sd_bus_message* value, void* context,
        sd_bus_error* error)
{
    auto o = static_cast<Value*>(context);

    try
    {
        return sdbusplus::sdbuspp::property_callback(
                value, o->get_bus().getInterface(), error,
                std::function(
                    [=](std::string&& arg)
                    {
                        o->unit(std::move(arg));
                    }
                ));
    }
    catch (const std::exception&)
    {
        o->get_bus().set_current_exception(std::current_exception());
        return 1;
    }
}

namespace details
{
namespace Value
{
static const auto _property_Unit =
    utility::tuple_to_array(message::types::type_id<
            std::string>());
}
}

auto Value::maxValue() const ->
        double
{
    return _maxValue;
}

int Value::_callback_get_MaxValue(
        sd_bus* /*bus*/, const char* /*path*/, const char* /*interface*/,
        const char* /*property*/, sd_bus_message* reply, void* context,
        sd_bus_error* error)
{
    auto o = static_cast<Value*>(context);

    try
    {
        return sdbusplus::sdbuspp::property_callback(
                reply, o->get_bus().getInterface(), error,
                std::function(
                    [=]()
                    {
                        return o->maxValue();
                    }
                ));
    }
    catch (const std::exception&)
    {
        o->get_bus().set_current_exception(std::current_exception());
        return 1;
    }
}

auto Value::maxValue(double value,
                                         bool skipSignal) ->
        double
{
    if (_maxValue != value)
    {
        _maxValue = value;
        if (!skipSignal)
        {
            _com_example_sensor_Value_interface.property_changed("MaxValue");
        }
    }

    return _maxValue;
}

auto Value::maxValue(double val) ->
        double
{
    return maxValue(val, false);
}

int Value::_callback_set_MaxValue(
        sd_bus* /*bus*/, const char* /*path*/, const char* /*interface*/,
        const char* /*property*/, sd_bus_message* value, void* context,
        sd_bus_error* error)
{
    auto o = static_cast<Value*>(context);

    try
    {
        return sdbusplus::sdbuspp::property_callback(
                value, o->get_bus().getInterface(), error,
                std::function(
                    [=](double&& arg)
                    {
                        o->maxValue(std::move(arg));
                    }
                ));
    }
    catch (const std::exception&)
    {
        o->get_bus().set_current_exception(std::current_exception());
        return 1;
    }
}

namespace details
{
namespace Value
{
static const auto _property_MaxValue =
    utility::tuple_to_array(message::types::type_id<
            double>());
}
}

auto Value::minValue() const ->
        double
{
    return _minValue;
}

int Value::_callback_get_MinValue(
        sd_bus* /*bus*/, const char* /*path*/, const char* /*interface*/,
        const char* /*property*/, sd_bus_message* reply, void* context,
        sd_bus_error* error)
{
    auto o = static_cast<Value*>(context);

    try
    {
        return sdbusplus::sdbuspp::property_callback(
                reply, o->get_bus().getInterface(), error,
                std::function(
                    [=]()
                    {
                        return o->minValue();
                    }
                ));
    }
    catch (const std::exception&)
    {
        o->get_bus().set_current_exception(std::current_exception());
        return 1;
    }
}

auto Value::minValue(double value,
                                         bool skipSignal) ->
        double
{
    if (_minValue != value)
    {
        _minValue = value;
        if (!skipSignal)
        {
            _com_example_sensor_Value_interface.property_changed("MinValue");
        }
    }

    return _minValue;
}

auto Value::minValue(double val) ->
        double
{
    return minValue(val, false);
}

int Value::_callback_set_MinValue(
        sd_bus* /*bus*/, const char* /*path*/, const char* /*interface*/,
        const char* /*property*/, sd_bus_message* value, void* context,
        sd_bus_error* error)
{
    auto o = static_cast<Value*>(context);

    try
    {
        return sdbusplus::sdbuspp::property_callback(
                value, o->get_bus().getInterface(), error,
                std::function(
                    [=](double&& arg)
                    {
                        o->minValue(std::move(arg));
                    }
                ));
    }
    catch (const std::exception&)
    {
        o->get_bus().set_current_exception(std::current_exception());
        return 1;
    }
}

namespace details
{
namespace Value
{
static const auto _property_MinValue =
    utility::tuple_to_array(message::types::type_id<
            double>());
}
}


void Value::setPropertyByName(const std::string& _name,
                                     const PropertiesVariant& val,
                                     bool skipSignal)
{
    if (_name == "Value")
    {
        auto& v = std::get<double>(val);
        value(v, skipSignal);
        return;
    }
    if (_name == "Unit")
    {
        auto& v = std::get<std::string>(val);
        unit(v, skipSignal);
        return;
    }
    if (_name == "MaxValue")
    {
        auto& v = std::get<double>(val);
        maxValue(v, skipSignal);
        return;
    }
    if (_name == "MinValue")
    {
        auto& v = std::get<double>(val);
        minValue(v, skipSignal);
        return;
    }
}

auto Value::getPropertyByName(const std::string& _name) ->
        PropertiesVariant
{
    if (_name == "Value")
    {
        return value();
    }
    if (_name == "Unit")
    {
        return unit();
    }
    if (_name == "MaxValue")
    {
        return maxValue();
    }
    if (_name == "MinValue")
    {
        return minValue();
    }

    return PropertiesVariant();
}



const vtable_t Value::_vtable[] = {
    vtable::start(),

    vtable::method("SetThresholds",
                   details::Value::_param_SetThresholds.data(),
                   details::Value::_return_SetThresholds.data(),
                   _callback_SetThresholds),

    vtable::signal("ThresholdAlert",
                   details::Value::_signal_ThresholdAlert.data()),

    vtable::property("Value",
                     details::Value::_property_Value.data(),
                     _callback_get_Value,
                     _callback_set_Value,
                     vtable::property_::emits_change),

    vtable::property("Unit",
                     details::Value::_property_Unit.data(),
                     _callback_get_Unit,
                     _callback_set_Unit,
                     vtable::property_::emits_change),

    vtable::property("MaxValue",
                     details::Value::_property_MaxValue.data(),
                     _callback_get_MaxValue,
                     _callback_set_MaxValue,
                     vtable::property_::emits_change),

    vtable::property("MinValue",
                     details::Value::_property_MinValue.data(),
                     _callback_get_MinValue,
                     _callback_set_MinValue,
                     vtable::property_::emits_change),

    vtable::end()
};

} // namespace sdbusplus::server::com::example::sensor

