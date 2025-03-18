#include "plugin_manager.h"

// 1. increase this number when adding new plugins
const int pluginCount = 0;

// 2. add your plugin in plugins folder and include it here
// example: #include "plugins/examplePlugin.h"

void (*pluginPtrs[])(Analyzer*, Trace*) = {
    // 3. add your plugin func here
    // example: examplePlugin
};

// 4. include your_plugin.c file in setup.py

// TODO: do i need to free it?
