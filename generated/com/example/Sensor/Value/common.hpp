#pragma once
#include <algorithm>
#include <array>
#include <optional>
#include <string>
#include <string_view>
#include <tuple>

#include <sdbusplus/exception.hpp>
#include <sdbusplus/message.hpp>
#include <sdbusplus/utility/dedup_variant.hpp>


namespace sdbusplus::common::com::example::sensor
{

struct Value
{
    static constexpr auto interface = "com.example.Sensor.Value";


    struct properties_t
    {
        double value = {};
        std::string unit = {};
        double max_value = {};
        double min_value = {};
    };

    using PropertiesVariant = sdbusplus::utility::dedup_variant_t<
        double,
        std::string>;


    struct set_thresholds_t
    {
        static constexpr auto name = "SetThresholds";
        using value_types = std::tuple<double, double>;
        using return_type = void;
    };

    struct method_names
    {
        static constexpr auto set_thresholds = set_thresholds_t::name;
    };

    struct signal_names
    {
        static constexpr auto threshold_alert = "ThresholdAlert";
    };



    struct value_t
    {
        static constexpr auto name = "Value";
        using value_type = double;
        value_t() = default;
        explicit value_t(value_type) {}
    };

    struct unit_t
    {
        static constexpr auto name = "Unit";
        using value_type = std::string;
        unit_t() = default;
        explicit unit_t(value_type) {}
    };

    struct max_value_t
    {
        static constexpr auto name = "MaxValue";
        using value_type = double;
        max_value_t() = default;
        explicit max_value_t(value_type) {}
    };

    struct min_value_t
    {
        static constexpr auto name = "MinValue";
        using value_type = double;
        min_value_t() = default;
        explicit min_value_t(value_type) {}
    };

    struct property_names
    {
        static constexpr auto value = value_t::name;
        static constexpr auto unit = unit_t::name;
        static constexpr auto max_value = max_value_t::name;
        static constexpr auto min_value = min_value_t::name;
    };

};



} // sdbusplus::common::com::example::sensor

namespace sdbusplus::message::details
{
} // namespace sdbusplus::message::details

