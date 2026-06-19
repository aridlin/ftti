#define FTTI_IMPLEMENTATION
#include "../ftti.hpp"

#include <cstdio>
#include <string>

int main() {
    ftti::Config cfg;
    cfg.title = "FTTI showcase";
    cfg.fps_limit = 30;

    if (!ftti::create_window(cfg)) return 1;

    char username[64] = "operator";
    char password[64] = "";
    char port[16] = "8080";
    char note[128] = "";
    char editor[512] = "Type here.\nEnter adds a line.";
    std::string logs =
        "[00:00] FTTI started\n"
        "[00:01] Terminal backend online\n"
        "[00:02] Waiting for input\n"
        "[00:03] Mouse reporting enabled\n"
        "[00:04] Wheel events mapped\n"
        "[00:05] Scroll easing active\n"
        "[00:06] Hover state active\n"
        "[00:07] Tooltip delay armed\n"
        "[00:08] Queue idle\n"
        "[00:09] Operator ready\n"
        "[00:10] Long enough to scroll\n"
        "[00:11] Keep wheeling\n"
        "[00:12] End of sample";

    int section = 0;
    int tab = 0;
    int theme = 0;
    int mode = 0;
    int profile = 0;
    int shell = 0;
    int actions = 0;
    bool submitted = false;
    bool safe_mode = true;
    bool advanced_open = true;
    float progress = 0.45f;

    const char* sections[] = {"Dashboard", "Inputs", "Themes"};
    const char* tabs[] = {"Live", "Queue", "Settings"};
    const char* theme_names[] = {"Dark", "Light", "Green", "Midnight", "Amber"};
    const char* modes[] = {"Local", "Staging", "Production"};
    const char* profiles[] = {"Admin", "Observer", "Maintainer", "Support"};
    const char* shells[] = {"Bash", "Zsh", "Fish"};

    while (ftti::pump()) {
        ftti::begin();

        ftti::side_menu("FTTI", sections, 3, &section);
        ftti::text("FTTI showcase");
        ftti::text_wrapped("Immediate-mode terminal widgets with FTUI-shaped calls and no ncurses dependency.");
        ftti::separator();
        ftti::tabs(tabs, 3, &tab);

        if (section == 0) {
            ftti::label("Dashboard");
            char line[128];
            if (tab == 0) {
                ftti::text("Live controls");
                std::snprintf(line, sizeof(line), "Actions: %d", actions);
                ftti::text(line);
                ftti::separator();
                if (ftti::button("Primary action", ftti::ColorRole::Accent)) ++actions;
                ftti::tooltip("Enter or click activates a focused button.");
                if (ftti::button("Warning action", ftti::ColorRole::Warning)) ++actions;
                if (ftti::button("Success action", ftti::ColorRole::Success)) ++actions;
                ftti::progress_bar(progress, "Progress");
                ftti::slider_float("Progress", &progress, 0.0f, 1.0f);
            } else if (tab == 1) {
                ftti::text("Queue");
                ftti::text_wrapped("This tab intentionally renders different content so tab switching is visible.");
                ftti::log_view("Queue log", logs.c_str(), 8, ftti::LogViewFlags::WordWrap);
                if (ftti::button("Append queue event")) {
                    ++actions;
                    char extra[64];
                    std::snprintf(extra, sizeof(extra), "\n[%02d] Queued event appended", actions);
                    logs += extra;
                    ftti::toast_info("Queued event appended");
                }
                ftti::scroll_area("Scrollable rows", 8, [&]() {
                    for (int i = 0; i < 24; ++i) {
                        char row[64];
                        std::snprintf(row, sizeof(row), "row %02d - wheel over this panel", i + 1);
                        ftti::text(row);
                    }
                });
            } else {
                ftti::text("Dashboard settings");
                ftti::checkbox("Safe mode", &safe_mode);
                if (ftti::button("Toast")) ftti::toast_success("Action completed");
                if (ftti::button("Open modal")) ftti::open_modal("Confirm action");
                ftti::modal("Confirm action", [&]() {
                    ftti::text_wrapped("This is a terminal modal. Escape or Close dismisses it.");
                    if (ftti::button("Close")) ftti::close_modal();
                });
            }
        } else if (section == 1) {
            if (tab == 0) {
                ftti::input("Username", username, sizeof(username));
                ftti::input("Password", password, sizeof(password), ftti::InputFlags::Password);
                ftti::input("Port", port, sizeof(port), ftti::InputFlags::CharsDecimal);
                ftti::input("Note", note, sizeof(note), ftti::InputFlags::Default, &submitted);
                if (submitted) ++actions;
                if (ftti::button("Submit form")) ++actions;
            } else if (tab == 1) {
                ftti::dropdown("Environment", modes, 3, &mode);
                ftti::listbox("Profile", profiles, 4, &profile, 4);
                ftti::radio_group("Shell", shells, 3, &shell);
            } else {
                if (ftti::collapsing_header("Editor", &advanced_open)) {
                    ftti::text_area_ex("Notes", editor, sizeof(editor), 5, ftti::TextAreaFlags::WordWrap);
                    ftti::log_view("Output", logs.c_str(), 5, ftti::LogViewFlags::WordWrap);
                }
            }
        } else {
            if (tab == 0) {
                ftti::text("Theme selector");
                ftti::tabs(theme_names, 5, &theme);
                if (theme == 0) ftti::set_style(ftti::default_dark_style());
                if (theme == 1) ftti::set_style(ftti::light_style());
                if (theme == 2) ftti::set_style(ftti::terminal_green_style());
                if (theme == 3) ftti::set_style(ftti::midnight_style());
                if (theme == 4) ftti::set_style(ftti::amber_style());
            } else if (tab == 1) {
                ftti::text_wrapped("Themes are semantic color packs. Widgets resolve roles like Button, Accent, Warning, Success, and InputFocus.");
                ftti::push_color(ftti::ColorRole::Button, ftti::color_from_hex("#7c3aed"));
                if (ftti::button("Scoped purple button")) ++actions;
                ftti::pop_color();
            } else {
                ftti::text_wrapped("Unsupported FTUI visual effects are accepted as no-ops so swapped code can keep compiling.");
                if (ftti::button("Set blur no-op")) ftti::set_backdrop_effect(ftti::BackdropEffect::Blur);
                if (ftti::button("Set dither no-op")) ftti::set_backdrop_effect(ftti::BackdropEffect::BayerDither);
            }
        }

        ftti::end();
    }

    ftti::shutdown();
    return 0;
}
