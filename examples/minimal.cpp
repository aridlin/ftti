#define FTTI_IMPLEMENTATION
#include "../ftti.hpp"

#include <cstdio>

int main() {
    ftti::Config cfg;
    cfg.title = "FTTI minimal";
    if (!ftti::create_window(cfg)) return 1;

    char name[64] = "";
    int clicks = 0;

    while (ftti::pump()) {
        ftti::begin();
        ftti::text("Minimal FTTI app");
        ftti::input("Name", name, sizeof(name));
        if (ftti::button("Click")) ++clicks;
        char status[32];
        std::snprintf(status, sizeof(status), "Clicks: %d", clicks);
        ftti::text(status);
        ftti::end();
    }

    ftti::shutdown();
    return 0;
}
