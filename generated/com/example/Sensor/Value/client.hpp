#pragma once
#include <sdbusplus/async/client.hpp>
#include <sdbusplus/async/execution.hpp>
#include <type_traits>
#include <variant>

#include <com/example/Sensor/Value/common.hpp>

namespace sdbusplus::client::com::example::sensor
{

namespace details
{
// forward declaration
template <typename Client, typename Proxy>
class Value;
} // namespace details

/** Alias class so we can use the client in both a client_t aggregation
 *  and individually.
 *
 *  sdbusplus::async::client_t<Value>() or
 *  Value() both construct an equivalent instance.
 */
template <typename Client = void, typename Proxy = void>
struct Value :
    public std::conditional_t<std::is_void_v<Client>,
                              sdbusplus::async::client_t<details::Value>,
                              details::Value<Client, Proxy>>
{
    template <typename... Args>
    Value(Args&&... args) :
        std::conditional_t<std::is_void_v<Client>,
                           sdbusplus::async::client_t<details::Value>,
                           details::Value<Client, Proxy>>(
            std::forward<Args>(args)...)
    {}
};

namespace details
{

template <typename Client, typename Proxy>
class Value :
    public sdbusplus::common::com::example::sensor::Value,
    private sdbusplus::async::client::details::client_context_friend
{
  public:
    friend Client;
    template <typename, typename>
    friend struct sdbusplus::client::com::example::sensor::Value;

    // Delete default constructor as these should only be constructed
    // indirectly through sdbusplus::async::client_t.
    Value() = delete;

    /** @brief SetThresholds
     *  Set upper and lower alert thresholds.
     *
     *  @param[in] high - Upper threshold.
     *  @param[in] low - Lower threshold.
     */
    auto set_thresholds(double high, double low)
    {
        return proxy.template call<>(context(), "SetThresholds", high, low);
    }

    /** Get value of Value
     *  The current sensor reading.
     */
    auto value()
    {
        return proxy.template get_property<double>(context(), "Value");
    }

    /** Set value of Value
     *  The current sensor reading.
     */
    auto value(auto value)
    {
        return proxy.template set_property<double>(
            context(), "Value", std::forward<decltype(value)>(value));
    }

    /** Get value of Unit
     *  Unit of measurement (e.g. DegreesC, RPM).
     */
    auto unit()
    {
        return proxy.template get_property<std::string>(context(), "Unit");
    }

    /** Set value of Unit
     *  Unit of measurement (e.g. DegreesC, RPM).
     */
    auto unit(auto value)
    {
        return proxy.template set_property<std::string>(
            context(), "Unit", std::forward<decltype(value)>(value));
    }

    /** Get value of MaxValue
     *  Upper threshold for alerting.
     */
    auto max_value()
    {
        return proxy.template get_property<double>(context(), "MaxValue");
    }

    /** Set value of MaxValue
     *  Upper threshold for alerting.
     */
    auto max_value(auto value)
    {
        return proxy.template set_property<double>(
            context(), "MaxValue", std::forward<decltype(value)>(value));
    }

    /** Get value of MinValue
     *  Lower threshold for alerting.
     */
    auto min_value()
    {
        return proxy.template get_property<double>(context(), "MinValue");
    }

    /** Set value of MinValue
     *  Lower threshold for alerting.
     */
    auto min_value(auto value)
    {
        return proxy.template set_property<double>(
            context(), "MinValue", std::forward<decltype(value)>(value));
    }


    auto properties()
    {
        return proxy.template get_all_properties<PropertiesVariant>(context()) |
               sdbusplus::async::execution::then([](auto&& v) {
                   properties_t result;
                   for (const auto& [property, value] : v)
                   {
                       std::visit(
                           [&](auto v) {
                               if (property == "Value")
                               {
                                   if constexpr (std::is_same_v<
                                                     std::decay_t<decltype(v)>,
                                                     double>)
                                   {
                                       result.value = v;
                                       return;
                                   }
                                   else
                                   {
                                       throw exception::UnpackPropertyError(
                                           property,
                                           UnpackErrorReason::wrongType);
                                   }
                               }
                               if (property == "Unit")
                               {
                                   if constexpr (std::is_same_v<
                                                     std::decay_t<decltype(v)>,
                                                     std::string>)
                                   {
                                       result.unit = v;
                                       return;
                                   }
                                   else
                                   {
                                       throw exception::UnpackPropertyError(
                                           property,
                                           UnpackErrorReason::wrongType);
                                   }
                               }
                               if (property == "MaxValue")
                               {
                                   if constexpr (std::is_same_v<
                                                     std::decay_t<decltype(v)>,
                                                     double>)
                                   {
                                       result.max_value = v;
                                       return;
                                   }
                                   else
                                   {
                                       throw exception::UnpackPropertyError(
                                           property,
                                           UnpackErrorReason::wrongType);
                                   }
                               }
                               if (property == "MinValue")
                               {
                                   if constexpr (std::is_same_v<
                                                     std::decay_t<decltype(v)>,
                                                     double>)
                                   {
                                       result.min_value = v;
                                       return;
                                   }
                                   else
                                   {
                                       throw exception::UnpackPropertyError(
                                           property,
                                           UnpackErrorReason::wrongType);
                                   }
                               }
                           },
                           value);
                   }
                   return result;
               });
    }

  private:
    // Conversion constructor from proxy used by client_t.
    explicit constexpr Value(Proxy p) :
        proxy(p.interface(interface))
    {}

    sdbusplus::async::context& context()
    {
        return sdbusplus::async::client::details::client_context_friend::
            context<Client, Value>(this);
    }

    decltype(std::declval<Proxy>().interface(interface)) proxy = {};
};

} // namespace details

} // namespace sdbusplus::client::com::example::sensor

