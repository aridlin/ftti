#define FTTI_IMPLEMENTATION
#include "ftti.hpp"

#include <cstdio>

int main() {
    ftti::Config cfg;
    cfg.title = "FTTI demo";
    cfg.fps_limit = 30;

    if (!ftti::create_window(cfg)) {
        std::fprintf(stderr, "FTTI needs an interactive terminal.\n");
        return 1;
    }

    char name[64] = "operator";
    char command[128] = "";
    int section = 0;
    int tab = 0;
    int clicks = 0;
    bool submitted = false;

    const char* sections[] = {"Overview", "Input", "Themes"};
    const char* tabs[] = {"Run", "Inspect", "Settings"};

    while (ftti::pump()) {
        ftti::begin();

        ftti::side_menu("FTTI", sections, 3, &section);
        ftti::text("Fuck This Terminal Interface");
        ftti::text_wrapped("Single-header immediate-mode terminal UI with FTUI-shaped calls.");
        ftti::separator();
        ftti::tabs(tabs, 3, &tab);

        if (section == 0) {
            ftti::label("Status");
            ftti::text_wrapped("Buttons, tabs, side menus, labels, text inputs, themes, and light focus animation are in the WIP surface.");
            if (ftti::button("Increment counter", ftti::ColorRole::Accent)) {
                ++clicks;
            }
            char line[96];
            std::snprintf(line, sizeof(line), "Counter: %d", clicks);
            ftti::text(line);
        } else if (section == 1) {
            ftti::input("Name", name, sizeof(name));
            ftti::input("Command", command, sizeof(command), ftti::InputFlags::Default, &submitted);
            if (submitted) ++clicks;
            if (ftti::button("Submit")) ++clicks;
            ftti::text_wrapped("Use Tab and Shift+Tab to move focus. Enter activates focused buttons and submits focused inputs.");
        } else {
            if (ftti::button("Default dark")) ftti::set_style(ftti::default_dark_style());
            if (ftti::button("Light")) ftti::set_style(ftti::light_style());
            if (ftti::button("Terminal green")) ftti::set_style(ftti::terminal_green_style());
            if (ftti::button("Amber")) ftti::set_style(ftti::amber_style());
        }

        ftti::end();
    }

    ftti::shutdown();
    return 0;
}
