#define FTUI_IMPLEMENTATION
#include "../ftui.hpp"

int main() {
    ftui::Config cfg;
    cfg.title = "FTUI compatibility smoke";
    cfg.enable_effects = true;
    cfg.backdrop_effect = ftui::BackdropEffect::Blur;

    if (!ftui::create_window(cfg)) return 1;

    char name[64] = "";
    bool enabled = true;
    float amount = 0.5f;
    int tab = 0;
    const char* tabs[] = {"One", "Two", "Three"};

    while (ftui::pump()) {
        ftui::begin();
        ftui::tabs(tabs, 3, &tab);
        ftui::input("Name", name, sizeof(name));
        ftui::checkbox("Enabled", &enabled);
        ftui::slider_float("Amount", &amount, 0.0f, 1.0f);
        if (ftui::button("Toast", ftui::ColorRole::Accent)) {
            ftui::toast_info("Shim path works");
        }
        ftui::end();
    }

    ftui::shutdown();
    return 0;
}
