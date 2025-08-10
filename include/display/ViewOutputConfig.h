#ifndef VIEWOUTPUTCONFIG_H
#define VIEWOUTPUTCONFIG_H


#if defined(DISPLAY_SSD1322)
    #include "hardware/SSD1322.h"
    #include "gfx/Canvas.h"
    using ScreenType = PiAlarm::hardware::SSD1322;  ///< Type for the display controller
    using RenderType = PiAlarm::gfx::Canvas;        ///< Type for the rendering context
#elif defined(DISPLAY_CONSOLE)
    #include <iostream>
    #include <sstream>
    using ScreenType = std::ostream;        ///< Type for the console output stream
    using RenderType = std::ostringstream;  ///< Type for the console rendering context
#else
    #error "No display type defined"
#endif


#endif //VIEWOUTPUTCONFIG_H
