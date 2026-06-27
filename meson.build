project(
    'my-dbus-service',
    'cpp',
    default_options: ['cpp_std=c++23', 'warning_level=2'],
    version: '0.1.0',
)

executable(
    'sensor-service',
    'src/main.cpp',
    'generated/com/example/Sensor/Value/server.cpp',
    include_directories: include_directories('generated'),
    dependencies: [
        dependency('sdbusplus'),
        dependency('libsystemd'),
        dependency('threads'),
    ],
    install: false,
)