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
            if (tab == 0) {
                ftti::label("Run");
                ftti::text_wrapped("This tab owns the primary action surface.");
                if (ftti::button("Increment counter", ftti::ColorRole::Accent)) {
                    ++clicks;
                }
                char line[96];
                std::snprintf(line, sizeof(line), "Counter: %d", clicks);
                ftti::text(line);
            } else if (tab == 1) {
                ftti::label("Inspect");
                ftti::text_wrapped("Tabs are immediate-mode state. Switching tabs changes which branch the app renders.");
                char line[96];
                std::snprintf(line, sizeof(line), "Current section: %s", sections[section]);
                ftti::text(line);
            } else {
                ftti::label("Settings");
                ftti::text_wrapped("Use the side menu for major app areas and tabs for local views inside the current area.");
                ftti::checkbox("Demo toggle", &submitted);
            }
        } else if (section == 1) {
            if (tab == 0) {
                ftti::input("Name", name, sizeof(name));
                ftti::input("Command", command, sizeof(command), ftti::InputFlags::Default, &submitted);
                if (submitted) ++clicks;
                if (ftti::button("Submit")) ++clicks;
            } else if (tab == 1) {
                ftti::text("Input preview");
                ftti::text(name);
                ftti::text(command);
            } else {
                ftti::text_wrapped("Use Tab and Shift+Tab to move focus. Enter activates focused buttons and submits focused inputs.");
                ftti::text_wrapped("Spaces are accepted inside focused text inputs.");
            }
        } else {
            if (tab == 0) {
                if (ftti::button("Default dark")) ftti::set_style(ftti::default_dark_style());
                if (ftti::button("Light")) ftti::set_style(ftti::light_style());
            } else if (tab == 1) {
                if (ftti::button("Terminal green")) ftti::set_style(ftti::terminal_green_style());
                if (ftti::button("Amber")) ftti::set_style(ftti::amber_style());
            } else {
                ftti::text_wrapped("Theme changes apply immediately and reuse the same widget calls.");
            }
        }

        ftti::end();
    }

    ftti::shutdown();
    return 0;
}
