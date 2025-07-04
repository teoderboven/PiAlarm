#ifndef DISPLAYCONFIG_HPP
#define DISPLAYCONFIG_HPP


#if defined(DISPLAY_SSD1322)
    #include <Adafruit_SSD1322.h>
    using DisplayType = Adafruit_SSD1322;
#elif defined(DISPLAY_CONSOLE)
    #include <iostream>
    using DisplayType = std::ostream;
#else
    #error "No display type defined"
#endif


#endif //DISPLAYCONFIG_HPP
