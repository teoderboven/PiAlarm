#ifndef VIEWOUTPUTCONFIG_H
#define VIEWOUTPUTCONFIG_H


#if defined(DISPLAY_SSD1322)
    #include "hardware/SSD1322.h"
    #include "gfx/Canvas.h"
    using ScreenType = PiAlarm::hardware::SSD1322;
    using RenderType = PiAlarm::gfx::Canvas;
#elif defined(DISPLAY_CONSOLE)
    #include <iostream>
    #include <sstream>
    using ScreenType = std::ostream;
    using RenderType = std::ostringstream;
#else
    #error "No display type defined"
#endif


#endif //VIEWOUTPUTCONFIG_H
