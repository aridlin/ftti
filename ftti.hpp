// ftti.hpp - Fuck This Terminal Interface
// Single-header immediate-mode terminal UI for C++17.
//
// Usage:
//   #define FTTI_IMPLEMENTATION
//   #include "ftti.hpp"
//
// Linux/macOS:
//   c++ app.cpp -std=c++17 -o app
//
// Windows:
//   cl /std:c++17 app.cpp
//
// Define FTTI_IMPLEMENTATION in exactly one translation unit.

#pragma once

#include <algorithm>
#include <cctype>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <functional>
#include <initializer_list>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#ifndef FTTI_DEFAULT_STYLE
#define FTTI_DEFAULT_STYLE ftti::default_dark_style
#endif

#ifndef FTTI_VERSION_MAJOR
#define FTTI_VERSION_MAJOR 0
#endif
#ifndef FTTI_VERSION_MINOR
#define FTTI_VERSION_MINOR 1
#endif
#ifndef FTTI_VERSION_PATCH
#define FTTI_VERSION_PATCH 0
#endif

namespace ftti {

enum class Align { Start, Center, End };

enum class BuiltinIcon {
    Symbol,
    SymbolWithText,
};

enum class BackdropEffect {
    Blur,
    BayerDither,
};

struct Color {
    float r = 1.0f;
    float g = 1.0f;
    float b = 1.0f;
    float a = 1.0f;
};

struct Style {
    Color background = {12, 14, 18};
    Color panel = {25, 29, 38};
    Color panel_alt = {34, 40, 52};
    Color text = {230, 235, 245};
    Color text_dim = {142, 154, 171};
    Color border = {73, 84, 104};
    Color button = {34, 40, 52};
    Color button_hover = {52, 61, 78};
    Color button_active = {77, 91, 117};
    Color input_bg = {10, 12, 16};
    Color input_focus = {101, 143, 255};
    Color accent = {101, 143, 255};
    Color warning = {239, 161, 99};
    Color success = {99, 202, 139};
    float window_padding = 2.0f;
    float item_spacing = 1.0f;
    float item_height = 3.0f;
    float rounding = 0.0f;
    float border_width = 1.0f;
    float font_size = 1.0f;
    bool boxed_widgets = true;
};

enum class ColorRole {
    Background,
    Panel,
    PanelAlt,
    Text,
    TextDim,
    Border,
    Button,
    ButtonHover,
    ButtonActive,
    InputBg,
    InputFocus,
    Accent,
    Warning,
    Success,
};

Style default_dark_style();
Style light_style();
Style terminal_green_style();
Style midnight_style();
Style amber_style();
Style catppuccin_mocha_style();
Style nord_style();
Style gruvbox_dark_style();
Style one_dark_style();
Style ghostty_green_style();

void set_style(const Style& style);
const Style& get_style();
Color color_from_hex(const char* hex);
Color color_from_hex(std::string_view hex);
void push_color(ColorRole role, Color color);
void pop_color();
void set_next_color(ColorRole role, Color color);

struct Config {
    const char* title = "FTTI App";
    int width = 960;
    int height = 640;
    int fps_limit = 30;
    bool resizable = true;
    bool center_window = true;
    void* icon = nullptr;
    bool enable_effects = true;
    BackdropEffect backdrop_effect = BackdropEffect::Blur;
    int dither_size = 4;
    bool alternate_screen = true;
    bool enable_mouse = true;
    bool quit_on_ctrl_q = true;
    bool animate = true;
};

bool create_window(const Config& cfg = {});
bool pump();
void begin();
void end();
void shutdown();
void request_redraw();
void set_quit_on_ctrl_q(bool enabled);
void set_fps_limit(int fps);
int get_fps_limit();
void set_backdrop_effect(BackdropEffect effect);
void set_dither_size(int px);
void set_window_icon(void* native_icon);
void set_window_icon_builtin(BuiltinIcon variant = BuiltinIcon::Symbol);

enum class InputFlags : unsigned {
    Default = 0,
    Password = 1 << 0,
    ReadOnly = 1 << 1,
    CharsDecimal = 1 << 2,
    CharsHexadecimal = 1 << 3,
    CharsUppercase = 1 << 4,
    CharsNoBlank = 1 << 5,
};

inline InputFlags operator|(InputFlags a, InputFlags b) {
    return static_cast<InputFlags>(static_cast<unsigned>(a) | static_cast<unsigned>(b));
}

inline bool operator&(InputFlags a, InputFlags b) {
    return (static_cast<unsigned>(a) & static_cast<unsigned>(b)) != 0;
}

enum class TextAreaFlags : unsigned {
    Default = 0,
    ReadOnly = 1 << 0,
    WordWrap = 1 << 1,
    AutoScrollBottom = 1 << 2,
};

inline TextAreaFlags operator|(TextAreaFlags a, TextAreaFlags b) {
    return static_cast<TextAreaFlags>(static_cast<unsigned>(a) | static_cast<unsigned>(b));
}

inline bool operator&(TextAreaFlags a, TextAreaFlags b) {
    return (static_cast<unsigned>(a) & static_cast<unsigned>(b)) != 0;
}

enum class LogViewFlags : unsigned {
    Default = 0,
    WordWrap = 1 << 0,
    AutoScrollBottom = 1 << 1,
};

inline LogViewFlags operator|(LogViewFlags a, LogViewFlags b) {
    return static_cast<LogViewFlags>(static_cast<unsigned>(a) | static_cast<unsigned>(b));
}

inline bool operator&(LogViewFlags a, LogViewFlags b) {
    return (static_cast<unsigned>(a) & static_cast<unsigned>(b)) != 0;
}

void label(const char* text);
void text(const char* text);
void text_wrapped(const char* text);
void separator();
void spacing(int rows = 1);
void spacing(float rows);

bool button(const char* label);
bool button(const char* label, ColorRole role);
bool button(const char* label, Color color);
bool input(const char* label, char* buffer, int buffer_size,
           InputFlags flags = InputFlags::Default, bool* enter_pressed = nullptr);
bool text_area(const char* label, char* buffer, int buffer_size, int rows = 5);
bool text_area_ex(const char* label, char* buffer, int buffer_size, int rows = 5,
                  TextAreaFlags flags = TextAreaFlags::Default);
void log_view(const char* label, const char* text, int rows = 8,
              LogViewFlags flags = LogViewFlags::AutoScrollBottom);
bool checkbox(const char* label, bool* value);
bool slider_float(const char* label, float* value, float min_v, float max_v);
bool tabs(const char* const* labels, int count, int* selected);
bool side_menu(const char* label, const char* const* items, int count, int* selected);
bool dropdown(const char* label, const char* const* items, int count, int* selected, int popup_rows = 8);
bool listbox(const char* label, const char* const* items, int count, int* selected, int visible_rows = 6);
bool radio_group(const char* label, const char* const* items, int count, int* selected, int columns = 1);
bool collapsing_header(const char* label, bool* open = nullptr);
void row(int cols, std::function<void()> fn);
void row(std::initializer_list<float> weights, std::function<void()> fn);
void split(std::initializer_list<float> columns, std::function<void()> fn);
void side_layout(float side_width, std::function<void()> fn);
void content(std::function<void()> fn);
void scroll_area(const char* label, float height, std::function<void()> fn);
void set_next_width(float cols);
void set_next_fill();
void set_next_percent(float pct);
void set_next_limits(float min_cols, float max_cols);
void set_next_align(Align align);
void open_modal(const char* label);
bool modal(const char* label, std::function<void()> fn);
void close_modal();
void begin_disabled();
void end_disabled();
void tooltip(const char* text);
void request_focus(const char* label);
float calc_text_width(const char* text);
float calc_text_height(const char* text, float wrap_width);

enum class ToastType {
    Info,
    Success,
    Warning,
    Error,
};

struct Toast {
    const char* message = "";
    ToastType type = ToastType::Info;
    int duration_ms = 3500;
    bool dismissible = true;
};

void toast(const Toast& toast);
void toast(const char* message);
void toast_info(const char* message);
void toast_success(const char* message);
void toast_warning(const char* message);
void toast_error(const char* message);
void clear_toasts();

struct ProgressStyle {
    const char* label = nullptr;
    const char* mask_path = nullptr;
    ColorRole fill_role = ColorRole::Accent;
    Color fill_color = {-1.0f, -1.0f, -1.0f, -1.0f};
    float height = 1.0f;
    bool show_percent = true;
    bool wave_front = false;
    bool glint = false;
};

void progress_bar(float progress);
void progress_bar(float progress, const char* label_or_mask_path);
void progress_bar(float progress, const ProgressStyle& style);

struct ImageHandle { void* _impl = nullptr; };
void image(ImageHandle* img, float width, float height);
ImageHandle* load_image(const char* utf8_path);
void free_image(ImageHandle* img);

struct FileFilter {
    const char* name;
    const char* spec;
};
std::string open_file_dialog(const char* title = "Open File",
                             const FileFilter* filters = nullptr,
                             int filter_count = 0);

void open_child_window(const Config& cfg, std::function<void()> fn);

struct DebugState {
    bool show_layout_rects = false;
    bool show_hovered_id = false;
    bool show_active_id = false;
    bool show_fps = false;
    bool log_widget_calls = false;
};
DebugState& debug();

float animation_time();
int terminal_width();
int terminal_height();

} // namespace ftti

#ifdef FTTI_IMPLEMENTATION

#include <cstdlib>
#include <thread>

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <conio.h>
#include <windows.h>
#else
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <termios.h>
#include <unistd.h>
#endif

namespace ftti {
namespace detail {

struct Cell {
    char ch = ' ';
    Color fg = {230, 235, 245};
    Color bg = {12, 14, 18};
    bool bold = false;
};

struct Rect {
    int x = 0;
    int y = 0;
    int w = 0;
    int h = 0;
};

struct InputState {
    bool enter = false;
    bool space = false;
    bool backspace = false;
    bool tab = false;
    bool shift_tab = false;
    bool left = false;
    bool right = false;
    bool up = false;
    bool down = false;
    bool ctrl_q = false;
    bool escape = false;
    bool mouse_pressed = false;
    bool mouse_released = false;
    bool mouse_event = false;
    bool mouse_down = false;
    int wheel = 0;
    int mouse_x = -1;
    int mouse_y = -1;
    char text[64] = {};
    int text_count = 0;
};

struct Motion {
    unsigned id = 0;
    float focus = 0.0f;
    float active = 0.0f;
    float hover = 0.0f;
};

struct ScrollState {
    unsigned id = 0;
    float current = 0.0f;
    float target = 0.0f;
};

struct TabState {
    unsigned id = 0;
    float current = 0.0f;
    float target = 0.0f;
};

struct ToastState {
    Toast toast;
    std::chrono::steady_clock::time_point created;
};

struct App {
    Config cfg;
    Style style;
    bool running = false;
    bool began = false;
    bool initialized = false;
    bool redraw = true;
    int width = 80;
    int height = 24;
    int cursor_x = 2;
    int cursor_y = 2;
    int content_x = 2;
    int content_w = 76;
    bool clip_active = false;
    Rect clip_rect;
    int focused = 0;
    int hovered = 0;
    int active = 0;
    bool mouse_down = false;
    int disabled_depth = 0;
    int request_focus_id = 0;
    int modal_id = 0;
    bool close_modal_requested = false;
    Rect last_item_rect;
    unsigned last_item_id = 0;
    bool command_mode = false;
    std::string command_buffer;
    std::string command_message;
    int active_tabs_id = 0;
    int tab_command_delta = 0;
    int text_focus_id = 0;
    std::vector<int> focus_order;
    std::vector<Cell> cells;
    InputState in;
    std::string input_pending;
    std::vector<Motion> motions;
    std::vector<ScrollState> scrolls;
    std::vector<TabState> tabs;
    std::vector<ToastState> toasts;
    std::vector<std::string> graphics_commands;
    std::vector<std::pair<ColorRole, Color>> color_stack;
    std::string pending_tooltip;
    Rect pending_tooltip_anchor;
    unsigned pending_tooltip_owner = 0;
    unsigned tooltip_owner = 0;
    int tooltip_early_hides = 0;
    bool tooltip_visible_last_frame = false;
    std::chrono::steady_clock::time_point tooltip_owner_since;
    std::chrono::steady_clock::time_point tooltip_visible_since;
    DebugState dbg;
    bool next_color_active = false;
    ColorRole next_color_role = ColorRole::Button;
    Color next_color = {255, 255, 255};
    int next_width = 0;
    bool next_fill = false;
    float next_percent = 0.0f;
    int next_min = 0;
    int next_max = 0;
    Align next_align = Align::Start;
    std::chrono::steady_clock::time_point start_time;
    std::chrono::steady_clock::time_point last_frame;
#if defined(_WIN32)
    HANDLE out = INVALID_HANDLE_VALUE;
    HANDLE in_handle = INVALID_HANDLE_VALUE;
    DWORD old_out_mode = 0;
    DWORD old_in_mode = 0;
#else
    termios old_term = {};
    int old_flags = 0;
#endif
};

static App& app() {
    static App a;
    return a;
}

static int clamp_i(int v, int lo, int hi) {
    return std::max(lo, std::min(hi, v));
}

static float clamp_f(float v, float lo, float hi) {
    return std::max(lo, std::min(hi, v));
}

static Color lerp(Color a, Color b, float t) {
    t = clamp_f(t, 0.0f, 1.0f);
    return {
        a.r + (b.r - a.r) * t,
        a.g + (b.g - a.g) * t,
        a.b + (b.b - a.b) * t,
        a.a + (b.a - a.a) * t,
    };
}

static unsigned color_byte(float v) {
    if (v <= 1.0f) v *= 255.0f;
    return (unsigned)clamp_i((int)std::round(v), 0, 255);
}

static unsigned hash_label(const char* s) {
    unsigned h = 2166136261u;
    for (; s && *s; ++s) h = (h ^ (unsigned char)*s) * 16777619u;
    return h ? h : 1u;
}

static std::string visible_label(const char* label) {
    if (!label) return {};
    const char* sep = std::strstr(label, "##");
    return sep ? std::string(label, sep) : std::string(label);
}

static bool contains(Rect r, int x, int y) {
    return x >= r.x && x < r.x + r.w && y >= r.y && y < r.y + r.h;
}

static void write_raw(const char* s) {
#if defined(_WIN32)
    DWORD written = 0;
    WriteFile(app().out, s, (DWORD)std::strlen(s), &written, nullptr);
#else
    ::write(STDOUT_FILENO, s, std::strlen(s));
#endif
}

static void write_raw(const std::string& s) {
#if defined(_WIN32)
    DWORD written = 0;
    WriteFile(app().out, s.data(), (DWORD)s.size(), &written, nullptr);
#else
    ::write(STDOUT_FILENO, s.data(), s.size());
#endif
}

static void query_size() {
    App& a = app();
#if defined(_WIN32)
    CONSOLE_SCREEN_BUFFER_INFO info = {};
    if (GetConsoleScreenBufferInfo(a.out, &info)) {
        a.width = info.srWindow.Right - info.srWindow.Left + 1;
        a.height = info.srWindow.Bottom - info.srWindow.Top + 1;
    }
#else
    winsize ws = {};
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == 0 && ws.ws_col > 0 && ws.ws_row > 0) {
        a.width = ws.ws_col;
        a.height = ws.ws_row;
    }
#endif
    a.width = std::max(20, a.width);
    a.height = std::max(8, a.height);
}

static void resize_canvas() {
    App& a = app();
    a.cells.assign((size_t)a.width * (size_t)a.height, Cell{' ', a.style.text, a.style.background, false});
}

static void put(int x, int y, char ch, Color fg, Color bg, bool bold = false) {
    App& a = app();
    if (x < 0 || y < 0 || x >= a.width || y >= a.height) return;
    if (a.clip_active && !contains(a.clip_rect, x, y)) return;
    Cell& c = a.cells[(size_t)y * (size_t)a.width + (size_t)x];
    c.ch = ch;
    c.fg = fg;
    c.bg = bg;
    c.bold = bold;
}

static void fill_rect(Rect r, Color bg) {
    App& a = app();
    int x0 = clamp_i(r.x, 0, a.width);
    int y0 = clamp_i(r.y, 0, a.height);
    int x1 = clamp_i(r.x + r.w, 0, a.width);
    int y1 = clamp_i(r.y + r.h, 0, a.height);
    for (int y = y0; y < y1; ++y) {
        for (int x = x0; x < x1; ++x) put(x, y, ' ', a.style.text, bg);
    }
}

static void draw_text(int x, int y, const std::string& s, Color fg, Color bg, bool bold = false, int max_w = -1) {
    if (max_w < 0) max_w = (int)s.size();
    int n = std::min((int)s.size(), max_w);
    for (int i = 0; i < n; ++i) put(x + i, y, s[(size_t)i], fg, bg, bold);
}

static void draw_hline(int x, int y, int w, char ch, Color fg, Color bg) {
    for (int i = 0; i < w; ++i) put(x + i, y, ch, fg, bg);
}

static void draw_box(Rect r, Color fg, Color bg) {
    if (r.w < 2 || r.h < 2) return;
    fill_rect(r, bg);
    put(r.x, r.y, '+', fg, bg);
    put(r.x + r.w - 1, r.y, '+', fg, bg);
    put(r.x, r.y + r.h - 1, '+', fg, bg);
    put(r.x + r.w - 1, r.y + r.h - 1, '+', fg, bg);
    draw_hline(r.x + 1, r.y, r.w - 2, '-', fg, bg);
    draw_hline(r.x + 1, r.y + r.h - 1, r.w - 2, '-', fg, bg);
    for (int y = r.y + 1; y < r.y + r.h - 1; ++y) {
        put(r.x, y, '|', fg, bg);
        put(r.x + r.w - 1, y, '|', fg, bg);
    }
}

static void draw_tooltip_box() {
    App& a = app();
    if (a.pending_tooltip.empty()) return;
    auto now = std::chrono::steady_clock::now();
    if (a.pending_tooltip_owner != a.tooltip_owner) {
        if (a.tooltip_visible_last_frame) {
            int visible_ms = (int)std::chrono::duration_cast<std::chrono::milliseconds>(now - a.tooltip_visible_since).count();
            if (visible_ms < 550) a.tooltip_early_hides = std::min(3, a.tooltip_early_hides + 1);
            else if (a.tooltip_early_hides > 0) --a.tooltip_early_hides;
        }
        a.tooltip_owner = a.pending_tooltip_owner;
        a.tooltip_owner_since = now;
        a.tooltip_visible_last_frame = false;
        return;
    }
    int delay_ms = 650 + a.tooltip_early_hides * 300;
    int stable_ms = (int)std::chrono::duration_cast<std::chrono::milliseconds>(now - a.tooltip_owner_since).count();
    if (stable_ms < delay_ms) {
        a.tooltip_visible_last_frame = false;
        return;
    }
    if (!a.tooltip_visible_last_frame) {
        a.tooltip_visible_since = now;
        a.tooltip_visible_last_frame = true;
    }
    int w = std::min((int)a.pending_tooltip.size() + 4, std::max(12, a.width - 4));
    int x = a.pending_tooltip_anchor.x;
    int y = a.pending_tooltip_anchor.y + a.pending_tooltip_anchor.h;
    if (x + w >= a.width) x = a.width - w - 1;
    if (x < 1) x = 1;
    if (y + 3 >= a.height) y = std::max(2, a.pending_tooltip_anchor.y - 3);
    Rect r{x, y, w, 3};
    draw_box(r, a.style.accent, a.style.panel_alt);
    draw_text(r.x + 2, r.y + 1, a.pending_tooltip, a.style.text, a.style.panel_alt, false, r.w - 4);
}

static void draw_toasts() {
    App& a = app();
    auto now = std::chrono::steady_clock::now();
    int y = a.height - 2;
    for (int i = (int)a.toasts.size() - 1; i >= 0 && y >= 2; --i) {
        ToastState& ts = a.toasts[(size_t)i];
        int age = (int)std::chrono::duration_cast<std::chrono::milliseconds>(now - ts.created).count();
        if (ts.toast.duration_ms > 0 && age > ts.toast.duration_ms) {
            a.toasts.erase(a.toasts.begin() + i);
            continue;
        }
        const char* msg = ts.toast.message ? ts.toast.message : "";
        int w = std::min((int)std::strlen(msg) + 4, std::max(16, a.width - 4));
        Rect r{a.width - w - 2, y - 2, w, 3};
        Color border = a.style.accent;
        if (ts.toast.type == ToastType::Success) border = a.style.success;
        if (ts.toast.type == ToastType::Warning || ts.toast.type == ToastType::Error) border = a.style.warning;
        draw_box(r, border, a.style.panel_alt);
        draw_text(r.x + 2, r.y + 1, msg, a.style.text, a.style.panel_alt, false, r.w - 4);
        y -= 4;
    }
}

static void draw_command_line() {
    App& a = app();
    if (a.height < 3) return;
    Rect r{0, a.height - 1, a.width, 1};
    Color bg = a.command_mode ? a.style.panel_alt : a.style.background;
    fill_rect(r, bg);
    if (a.command_mode) {
        draw_text(0, a.height - 1, ":" + a.command_buffer, a.style.accent, bg, true, a.width);
    } else if (!a.command_message.empty()) {
        draw_text(0, a.height - 1, a.command_message, a.style.text_dim, bg, false, a.width);
    }
}

static Motion& motion(unsigned id) {
    App& a = app();
    for (Motion& m : a.motions) {
        if (m.id == id) return m;
    }
    a.motions.push_back(Motion{id, 0.0f, 0.0f});
    return a.motions.back();
}

static ScrollState& scroll_state(unsigned id) {
    App& a = app();
    for (ScrollState& s : a.scrolls) {
        if (s.id == id) return s;
    }
    a.scrolls.push_back(ScrollState{id, 0.0f, 0.0f});
    return a.scrolls.back();
}

static TabState& tab_state(unsigned id, int selected) {
    App& a = app();
    for (TabState& t : a.tabs) {
        if (t.id == id) return t;
    }
    a.tabs.push_back(TabState{id, (float)selected, (float)selected});
    return a.tabs.back();
}

static void animate_to(float& value, float target) {
    float speed = app().cfg.animate ? 0.28f : 1.0f;
    value += (target - value) * speed;
    if (std::abs(value - target) < 0.01f) value = target;
}

static void ease_to(float& value, float target, float speed = 0.18f) {
    if (!app().cfg.animate) {
        value = target;
        return;
    }
    float d = target - value;
    value += d * speed;
    if (std::abs(d) < 0.03f) value = target;
}

static bool register_focus(unsigned id, Rect r) {
    App& a = app();
    a.last_item_rect = r;
    a.last_item_id = id;
    a.focus_order.push_back((int)id);
    if (!a.focused) a.focused = (int)id;
    if (a.request_focus_id == (int)id) {
        a.focused = (int)id;
        a.request_focus_id = 0;
    }
    bool focused = a.focused == (int)id;
    Motion& m = motion(id);
    animate_to(m.focus, focused ? 1.0f : 0.0f);
    bool mouse_hit = contains(r, a.in.mouse_x, a.in.mouse_y);
    animate_to(m.hover, mouse_hit ? 1.0f : 0.0f);
    if (mouse_hit) a.hovered = (int)id;
    if (mouse_hit && a.in.mouse_pressed && a.disabled_depth == 0) a.focused = (int)id;
    return focused;
}

static bool disabled() {
    return app().disabled_depth > 0;
}

static void execute_command(const std::string& raw) {
    App& a = app();
    std::string cmd = raw;
    while (!cmd.empty() && std::isspace((unsigned char)cmd.front())) cmd.erase(cmd.begin());
    while (!cmd.empty() && std::isspace((unsigned char)cmd.back())) cmd.pop_back();
    if (cmd == "q" || cmd == "quit" || cmd == "exit") {
        a.running = false;
        return;
    }
    if (cmd == "tab next" || cmd == "tn") {
        a.tab_command_delta = 1;
        a.command_message = "tab next";
        return;
    }
    if (cmd == "tab prev" || cmd == "tp" || cmd == "tab previous") {
        a.tab_command_delta = -1;
        a.command_message = "tab prev";
        return;
    }
    if (cmd == "theme dark") {
        a.style = default_dark_style();
        a.command_message = "theme dark";
        return;
    }
    if (cmd == "theme light") {
        a.style = light_style();
        a.command_message = "theme light";
        return;
    }
    if (cmd == "theme green") {
        a.style = terminal_green_style();
        a.command_message = "theme green";
        return;
    }
    if (cmd == "theme amber") {
        a.style = amber_style();
        a.command_message = "theme amber";
        return;
    }
    if (cmd == "help" || cmd == "?") {
        a.command_message = "commands: q, theme dark/light/green/amber, tab next, tab prev";
        return;
    }
    a.command_message = cmd.empty() ? "" : "not a command: " + cmd;
}

static void consume_widget_input() {
    App& a = app();
    a.in.enter = false;
    a.in.space = false;
    a.in.backspace = false;
    a.in.tab = false;
    a.in.shift_tab = false;
    a.in.left = false;
    a.in.right = false;
    a.in.up = false;
    a.in.down = false;
    a.in.escape = false;
    a.in.text_count = 0;
    a.in.text[0] = '\0';
}

static void focus_delta(int delta) {
    App& a = app();
    if (a.focus_order.empty()) return;
    auto it = std::find(a.focus_order.begin(), a.focus_order.end(), a.focused);
    int idx = it == a.focus_order.end() ? 0 : (int)(it - a.focus_order.begin());
    idx = (idx + delta + (int)a.focus_order.size()) % (int)a.focus_order.size();
    a.focused = a.focus_order[(size_t)idx];
}

static void reset_input() {
    App& a = app();
    int x = a.in.mouse_x;
    int y = a.in.mouse_y;
    a.in = InputState{};
    a.in.mouse_x = x;
    a.in.mouse_y = y;
    a.in.mouse_down = a.mouse_down;
}

static void push_text_char(char ch) {
    App& a = app();
    if (a.in.text_count < (int)sizeof(a.in.text) - 1) {
        a.in.text[a.in.text_count++] = ch;
        a.in.text[a.in.text_count] = '\0';
    }
}

#if defined(_WIN32)
static void read_input() {
    App& a = app();
    while (_kbhit()) {
        int c = _getch();
        if (c == 0 || c == 224) {
            int k = _getch();
            if (k == 72) a.in.up = true;
            else if (k == 80) a.in.down = true;
            else if (k == 75) a.in.left = true;
            else if (k == 77) a.in.right = true;
            continue;
        }
        if (c == 17) a.in.ctrl_q = true;
        else if (c == 9) a.in.tab = true;
        else if (c == 13) a.in.enter = true;
        else if (c == 27) a.in.escape = true;
        else if (c == 8) a.in.backspace = true;
        else if (c == ' ') {
            a.in.space = true;
            push_text_char(' ');
        }
        else if (c >= 32 && c < 127) push_text_char((char)c);
    }
}
#else
static bool parse_escape_sequence(const std::string& buf, size_t& i) {
    App& a = app();
    if (i + 1 >= buf.size()) return false;
    if (buf[i] != 27 || buf[i + 1] != '[') {
        a.in.escape = true;
        ++i;
        return true;
    }
    i += 2;
    if (i >= buf.size()) return false;
    if (buf[i] == 'Z') {
        a.in.shift_tab = true;
        ++i;
        return true;
    }
    if (buf[i] == '<') {
        size_t start = i - 2;
        ++i;
        int b = 0, x = 0, y = 0;
        while (i < buf.size() && std::isdigit((unsigned char)buf[i])) b = b * 10 + (buf[i++] - '0');
        if (i >= buf.size()) { i = start; return false; }
        if (buf[i] != ';') { i = start + 1; a.in.escape = true; return true; }
        ++i;
        while (i < buf.size() && std::isdigit((unsigned char)buf[i])) x = x * 10 + (buf[i++] - '0');
        if (i >= buf.size()) { i = start; return false; }
        if (buf[i] != ';') { i = start + 1; a.in.escape = true; return true; }
        ++i;
        while (i < buf.size() && std::isdigit((unsigned char)buf[i])) y = y * 10 + (buf[i++] - '0');
        if (i >= buf.size()) { i = start; return false; }
        if (buf[i] == 'M' || buf[i] == 'm') {
            a.in.mouse_x = x - 1;
            a.in.mouse_y = y - 1;
            a.in.mouse_event = true;
            int button = b & ~(4 | 8 | 16);
            if (button == 64 || button == 66) a.in.wheel = -1;
            else if (button == 65 || button == 67) a.in.wheel = 1;
            else if ((b & 32) != 0) {
                // Hover/motion report.
            } else if (buf[i] == 'M') {
                a.in.mouse_pressed = true;
                a.mouse_down = true;
                a.in.mouse_down = true;
            } else {
                a.in.mouse_released = true;
                a.mouse_down = false;
                a.in.mouse_down = false;
            }
            ++i;
            return true;
        }
        i = start + 1;
        a.in.escape = true;
        return true;
    }
    if (buf[i] == 'M') {
        size_t start = i - 2;
        if (i + 3 >= buf.size()) { i = start; return false; }
        int b = (unsigned char)buf[i + 1] - 32;
        int x = (unsigned char)buf[i + 2] - 33;
        int y = (unsigned char)buf[i + 3] - 33;
        a.in.mouse_x = x;
        a.in.mouse_y = y;
        a.in.mouse_event = true;
        int button = b & ~(4 | 8 | 16);
        if (button == 64 || button == 66) a.in.wheel = -1;
        else if (button == 65 || button == 67) a.in.wheel = 1;
        else if ((b & 3) == 3) {
            a.in.mouse_released = true;
            a.mouse_down = false;
            a.in.mouse_down = false;
        } else {
            a.in.mouse_pressed = true;
            a.mouse_down = true;
            a.in.mouse_down = true;
        }
        i += 4;
        return true;
    }
    char final = 0;
    size_t start = i - 2;
    while (i < buf.size()) {
        char ch = buf[i];
        if ((ch >= 'A' && ch <= 'Z') || ch == '~') {
            final = ch;
            break;
        }
        ++i;
    }
    if (!final) { i = start; return false; }
    if (final == 'A') a.in.up = true;
    else if (final == 'B') a.in.down = true;
    else if (final == 'C') a.in.right = true;
    else if (final == 'D') a.in.left = true;
    ++i;
    return true;
}

static void read_input() {
    App& a = app();
    char buf[256];
    for (;;) {
        int n = (int)::read(STDIN_FILENO, buf, sizeof(buf));
        if (n <= 0) break;
        a.input_pending.append(buf, (size_t)n);
    }
    size_t i = 0;
    while (i < a.input_pending.size()) {
        unsigned char c = (unsigned char)a.input_pending[i];
        if (c == 27) {
            size_t before = i;
            if (!parse_escape_sequence(a.input_pending, i)) {
                a.input_pending.erase(0, before);
                return;
            }
        } else {
            ++i;
            if (c == 17) a.in.ctrl_q = true;
            else if (c == 9) a.in.tab = true;
            else if (c == 10 || c == 13) a.in.enter = true;
            else if (c == 127 || c == 8) a.in.backspace = true;
            else if (c == ' ') {
                a.in.space = true;
                push_text_char(' ');
            }
            else if (c >= 32 && c < 127) push_text_char((char)c);
        }
    }
    a.input_pending.clear();
}
#endif

static bool accept_char(char& ch, InputFlags flags) {
    if ((flags & InputFlags::CharsNoBlank) && std::isspace((unsigned char)ch)) return false;
    if (flags & InputFlags::CharsUppercase) ch = (char)std::toupper((unsigned char)ch);
    if (flags & InputFlags::CharsDecimal) return std::isdigit((unsigned char)ch) || ch == '.' || ch == '-';
    if (flags & InputFlags::CharsHexadecimal) return std::isxdigit((unsigned char)ch) != 0;
    return true;
}

static std::string color_seq(Color fg, Color bg, bool bold) {
    char buf[96];
    std::snprintf(buf, sizeof(buf), "\x1b[%d;38;2;%u;%u;%u;48;2;%u;%u;%um",
                  bold ? 1 : 22,
                  color_byte(fg.r), color_byte(fg.g), color_byte(fg.b),
                  color_byte(bg.r), color_byte(bg.g), color_byte(bg.b));
    return buf;
}

static std::string base64_encode(std::string_view input) {
    static const char table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::string out;
    out.reserve(((input.size() + 2) / 3) * 4);
    unsigned val = 0;
    int valb = -6;
    for (unsigned char c : input) {
        val = (val << 8) | c;
        valb += 8;
        while (valb >= 0) {
            out.push_back(table[(val >> valb) & 0x3f]);
            valb -= 6;
        }
    }
    if (valb > -6) out.push_back(table[((val << 8) >> (valb + 8)) & 0x3f]);
    while (out.size() % 4) out.push_back('=');
    return out;
}

struct ImageData {
    std::string path;
    unsigned id = 0;
};

static Color role_color(ColorRole role) {
    App& a = app();
    for (auto it = a.color_stack.rbegin(); it != a.color_stack.rend(); ++it) {
        if (it->first == role) return it->second;
    }
    const Style& s = app().style;
    switch (role) {
        case ColorRole::Background: return s.background;
        case ColorRole::Panel: return s.panel;
        case ColorRole::PanelAlt: return s.panel_alt;
        case ColorRole::Text: return s.text;
        case ColorRole::TextDim: return s.text_dim;
        case ColorRole::Border: return s.border;
        case ColorRole::Button: return s.button;
        case ColorRole::ButtonHover: return s.button_hover;
        case ColorRole::ButtonActive: return s.button_active;
        case ColorRole::InputBg: return s.input_bg;
        case ColorRole::InputFocus: return s.input_focus;
        case ColorRole::Accent: return s.accent;
        case ColorRole::Warning: return s.warning;
        case ColorRole::Success: return s.success;
    }
    return s.text;
}

static Color consume_role_color(ColorRole role) {
    App& a = app();
    if (a.next_color_active && a.next_color_role == role) {
        a.next_color_active = false;
        return a.next_color;
    }
    return role_color(role);
}

static int consume_width(int fallback) {
    App& a = app();
    int w = fallback;
    if (a.next_fill) w = a.content_w;
    if (a.next_percent > 0.0f) w = (int)(a.content_w * clamp_f(a.next_percent, 0.0f, 1.0f));
    if (a.next_width > 0) w = a.next_width;
    if (a.next_min > 0) w = std::max(w, a.next_min);
    if (a.next_max > 0) w = std::min(w, a.next_max);
    w = clamp_i(w, 4, a.content_w);
    a.next_width = 0;
    a.next_fill = false;
    a.next_percent = 0.0f;
    a.next_min = 0;
    a.next_max = 0;
    a.next_align = Align::Start;
    return w;
}

} // namespace detail

Style default_dark_style() {
    return {};
}

Style light_style() {
    Style s;
    s.background = {244, 247, 251};
    s.panel = {255, 255, 255};
    s.panel_alt = {232, 238, 247};
    s.text = {23, 29, 39};
    s.text_dim = {91, 105, 126};
    s.border = {185, 197, 213};
    s.button = {237, 242, 248};
    s.button_hover = {222, 231, 243};
    s.button_active = {203, 216, 235};
    s.input_bg = {255, 255, 255};
    s.input_focus = {37, 99, 235};
    s.accent = {37, 99, 235};
    s.warning = {180, 83, 9};
    s.success = {22, 128, 76};
    return s;
}

Style terminal_green_style() {
    Style s;
    s.background = {0, 12, 8};
    s.panel = {4, 26, 17};
    s.panel_alt = {7, 40, 27};
    s.text = {205, 255, 223};
    s.text_dim = {105, 188, 135};
    s.border = {55, 132, 84};
    s.button = {5, 35, 23};
    s.button_hover = {10, 57, 37};
    s.button_active = {18, 84, 54};
    s.input_bg = {0, 9, 6};
    s.input_focus = {80, 255, 145};
    s.accent = {80, 255, 145};
    s.warning = {236, 196, 84};
    s.success = {112, 245, 150};
    return s;
}

Style midnight_style() {
    Style s;
    s.background = {8, 10, 28};
    s.panel = {18, 22, 48};
    s.panel_alt = {29, 35, 70};
    s.text = {232, 238, 255};
    s.text_dim = {144, 158, 201};
    s.border = {70, 84, 142};
    s.button = {24, 30, 62};
    s.button_hover = {38, 48, 96};
    s.button_active = {60, 76, 138};
    s.input_bg = {7, 9, 24};
    s.input_focus = {117, 154, 255};
    s.accent = {117, 154, 255};
    s.warning = {255, 177, 107};
    s.success = {100, 220, 178};
    return s;
}

Style amber_style() {
    Style s;
    s.background = {18, 12, 5};
    s.panel = {38, 27, 12};
    s.panel_alt = {58, 41, 18};
    s.text = {255, 236, 201};
    s.text_dim = {199, 161, 105};
    s.border = {120, 82, 31};
    s.button = {49, 34, 14};
    s.button_hover = {72, 50, 21};
    s.button_active = {98, 67, 27};
    s.input_bg = {14, 9, 4};
    s.input_focus = {255, 184, 77};
    s.accent = {255, 184, 77};
    s.warning = {255, 113, 91};
    s.success = {144, 207, 130};
    return s;
}

Style catppuccin_mocha_style() {
    Style s = midnight_style();
    s.background = {17, 17, 27};
    s.panel = {30, 30, 46};
    s.panel_alt = {49, 50, 68};
    s.text = {205, 214, 244};
    s.text_dim = {166, 173, 200};
    s.border = {88, 91, 112};
    s.accent = {137, 180, 250};
    s.input_focus = s.accent;
    s.warning = {250, 179, 135};
    s.success = {166, 227, 161};
    return s;
}

Style nord_style() {
    Style s = default_dark_style();
    s.background = {46, 52, 64};
    s.panel = {59, 66, 82};
    s.panel_alt = {67, 76, 94};
    s.text = {236, 239, 244};
    s.text_dim = {216, 222, 233};
    s.border = {76, 86, 106};
    s.button = {67, 76, 94};
    s.button_hover = {76, 86, 106};
    s.button_active = {94, 129, 172};
    s.input_bg = {46, 52, 64};
    s.input_focus = {136, 192, 208};
    s.accent = {136, 192, 208};
    s.warning = {235, 203, 139};
    s.success = {163, 190, 140};
    return s;
}

Style gruvbox_dark_style() {
    Style s = amber_style();
    s.background = {29, 32, 33};
    s.panel = {40, 40, 40};
    s.panel_alt = {60, 56, 54};
    s.text = {235, 219, 178};
    s.text_dim = {168, 153, 132};
    s.border = {102, 92, 84};
    s.accent = {131, 165, 152};
    s.input_focus = s.accent;
    s.warning = {250, 189, 47};
    s.success = {184, 187, 38};
    return s;
}

Style one_dark_style() {
    return default_dark_style();
}

Style ghostty_green_style() {
    return terminal_green_style();
}

void set_style(const Style& style) {
    detail::app().style = style;
    request_redraw();
}

const Style& get_style() {
    return detail::app().style;
}

Color color_from_hex(std::string_view hex) {
    if (!hex.empty() && hex[0] == '#') hex.remove_prefix(1);
    auto nib = [](char c) -> int {
        if (c >= '0' && c <= '9') return c - '0';
        if (c >= 'a' && c <= 'f') return c - 'a' + 10;
        if (c >= 'A' && c <= 'F') return c - 'A' + 10;
        return 0;
    };
    if (hex.size() == 3) {
        return {
            (float)(nib(hex[0]) * 17),
            (float)(nib(hex[1]) * 17),
            (float)(nib(hex[2]) * 17),
            1.0f,
        };
    }
    if (hex.size() >= 6) {
        return {
            (float)(nib(hex[0]) * 16 + nib(hex[1])),
            (float)(nib(hex[2]) * 16 + nib(hex[3])),
            (float)(nib(hex[4]) * 16 + nib(hex[5])),
            1.0f,
        };
    }
    return {255.0f, 255.0f, 255.0f, 1.0f};
}

Color color_from_hex(const char* hex) {
    return color_from_hex(std::string_view(hex ? hex : ""));
}

void push_color(ColorRole role, Color color) {
    detail::app().color_stack.push_back({role, color});
}

void pop_color() {
    detail::App& a = detail::app();
    if (!a.color_stack.empty()) a.color_stack.pop_back();
}

void set_next_color(ColorRole role, Color color) {
    detail::App& a = detail::app();
    a.next_color_active = true;
    a.next_color_role = role;
    a.next_color = color;
}

bool create_window(const Config& cfg) {
    using namespace detail;
    App& a = app();
    a.cfg = cfg;
    a.style = FTTI_DEFAULT_STYLE();
    a.running = true;
    a.initialized = true;
    a.redraw = true;
    a.start_time = std::chrono::steady_clock::now();
    a.last_frame = a.start_time;
#if defined(_WIN32)
    a.out = GetStdHandle(STD_OUTPUT_HANDLE);
    a.in_handle = GetStdHandle(STD_INPUT_HANDLE);
    GetConsoleMode(a.out, &a.old_out_mode);
    GetConsoleMode(a.in_handle, &a.old_in_mode);
    SetConsoleMode(a.out, a.old_out_mode | ENABLE_PROCESSED_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    SetConsoleMode(a.in_handle, ENABLE_PROCESSED_INPUT);
#else
    if (!isatty(STDIN_FILENO) || !isatty(STDOUT_FILENO)) return false;
    tcgetattr(STDIN_FILENO, &a.old_term);
    termios raw = a.old_term;
    raw.c_lflag &= (tcflag_t)~(ECHO | ICANON | IEXTEN);
    raw.c_iflag &= (tcflag_t)~(IXON | ICRNL);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
    a.old_flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, a.old_flags | O_NONBLOCK);
#endif
    if (cfg.alternate_screen) write_raw("\x1b[?1049h");
    write_raw("\x1b[?25l\x1b[2J\x1b[H");
    if (cfg.enable_mouse) write_raw("\x1b[?1000h\x1b[?1003h\x1b[?1006h");
    query_size();
    resize_canvas();
    return true;
}

bool pump() {
    using namespace detail;
    App& a = app();
    if (!a.running) return false;

    int fps = a.cfg.fps_limit <= 0 ? 60 : a.cfg.fps_limit;
    auto now = std::chrono::steady_clock::now();
    auto min_frame = std::chrono::microseconds(1000000 / std::max(1, fps));
    auto elapsed = now - a.last_frame;
    if (elapsed < min_frame) std::this_thread::sleep_for(min_frame - elapsed);
    a.last_frame = std::chrono::steady_clock::now();

    reset_input();
    read_input();
    if (a.command_mode) {
        if (a.in.escape) {
            a.command_mode = false;
            a.command_buffer.clear();
        } else if (a.in.backspace && !a.command_buffer.empty()) {
            a.command_buffer.pop_back();
        } else {
            for (int i = 0; i < a.in.text_count; ++i) a.command_buffer.push_back(a.in.text[i]);
            if (a.in.enter) {
                execute_command(a.command_buffer);
                a.command_buffer.clear();
                a.command_mode = false;
            }
        }
        consume_widget_input();
    } else if (a.in.text_count == 1 && a.in.text[0] == ':' && a.focused != a.text_focus_id) {
        a.command_mode = true;
        a.command_buffer.clear();
        consume_widget_input();
    }
    if (a.cfg.quit_on_ctrl_q && a.in.ctrl_q) {
        a.running = false;
        return false;
    }
    if (a.in.tab) focus_delta(1);
    if (a.in.shift_tab) focus_delta(-1);
    query_size();
    return a.running;
}

void begin() {
    using namespace detail;
    App& a = app();
    a.began = true;
    a.focus_order.clear();
    a.hovered = 0;
    a.pending_tooltip.clear();
    a.graphics_commands.clear();
    a.cursor_x = 2;
    a.cursor_y = 2;
    a.content_x = 2;
    a.content_w = std::max(10, a.width - 4);
    resize_canvas();

    std::string title = a.cfg.title ? a.cfg.title : "FTTI";
    draw_text(2, 0, title, a.style.accent, a.style.background, true, a.width - 4);
    std::string hint = "tab/shift-tab focus  enter activate  ctrl-q quit";
    if ((int)hint.size() < a.width - 2) {
        draw_text(a.width - (int)hint.size() - 2, 0, hint, a.style.text_dim, a.style.background, false);
    }
    draw_hline(0, 1, a.width, '-', a.style.border, a.style.background);
}

void end() {
    using namespace detail;
    App& a = app();
    if (!a.began) return;
    a.began = false;
    if (!a.focused && !a.focus_order.empty()) a.focused = a.focus_order.front();
    draw_toasts();
    draw_tooltip_box();
    draw_command_line();

    std::string out;
    out.reserve((size_t)a.width * (size_t)a.height * 16);
    out += "\x1b[H";
    Color last_fg = {0, 0, 0};
    Color last_bg = {0, 0, 0};
    bool last_bold = false;
    bool have_color = false;
    for (int y = 0; y < a.height; ++y) {
        for (int x = 0; x < a.width; ++x) {
            const Cell& c = a.cells[(size_t)y * (size_t)a.width + (size_t)x];
            if (!have_color || std::memcmp(&last_fg, &c.fg, sizeof(Color)) != 0 ||
                std::memcmp(&last_bg, &c.bg, sizeof(Color)) != 0 || last_bold != c.bold) {
                out += color_seq(c.fg, c.bg, c.bold);
                last_fg = c.fg;
                last_bg = c.bg;
                last_bold = c.bold;
                have_color = true;
            }
            out += c.ch;
        }
        if (y != a.height - 1) out += "\r\n";
    }
    out += "\x1b[0m";
    for (const std::string& cmd : a.graphics_commands) out += cmd;
    write_raw(out);
}

void shutdown() {
    using namespace detail;
    App& a = app();
    if (!a.initialized) {
        return;
    }
    if (a.cfg.enable_mouse) write_raw("\x1b[?1006l\x1b[?1003l\x1b[?1000l");
    write_raw("\x1b[?25h\x1b[0m");
    if (a.cfg.alternate_screen) write_raw("\x1b[?1049l");
#if defined(_WIN32)
    if (a.out != INVALID_HANDLE_VALUE) SetConsoleMode(a.out, a.old_out_mode);
    if (a.in_handle != INVALID_HANDLE_VALUE) SetConsoleMode(a.in_handle, a.old_in_mode);
#else
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &a.old_term);
    fcntl(STDIN_FILENO, F_SETFL, a.old_flags);
#endif
    a.running = false;
    a.began = false;
    a.initialized = false;
}

void request_redraw() {
    detail::app().redraw = true;
}

void set_quit_on_ctrl_q(bool enabled) {
    detail::app().cfg.quit_on_ctrl_q = enabled;
}

void set_fps_limit(int fps) {
    detail::app().cfg.fps_limit = fps;
}

int get_fps_limit() {
    return detail::app().cfg.fps_limit;
}

void set_backdrop_effect(BackdropEffect effect) {
    detail::app().cfg.backdrop_effect = effect;
}

void set_dither_size(int px) {
    detail::app().cfg.dither_size = px;
}

void set_window_icon(void* native_icon) {
    detail::app().cfg.icon = native_icon;
}

void set_window_icon_builtin(BuiltinIcon variant) {
    (void)variant;
}

void label(const char* s) {
    text(s);
}

void text(const char* s) {
    using namespace detail;
    App& a = app();
    std::string value = s ? s : "";
    draw_text(a.content_x, a.cursor_y, value, a.style.text, a.style.background, false, a.content_w);
    a.cursor_y += 1;
}

void text_wrapped(const char* s) {
    using namespace detail;
    App& a = app();
    std::string value = s ? s : "";
    int w = std::max(8, a.content_w);
    int x = a.content_x;
    int y = a.cursor_y;
    size_t pos = 0;
    while (pos < value.size() && y < a.height) {
        while (pos < value.size() && value[pos] == ' ') ++pos;
        size_t line_start = pos;
        size_t line_end = std::min(value.size(), line_start + (size_t)w);
        size_t split = line_end;
        if (line_end < value.size()) {
            for (size_t i = line_end; i > line_start; --i) {
                if (value[i - 1] == ' ') {
                    split = i - 1;
                    break;
                }
            }
        }
        if (split == line_start) split = line_end;
        draw_text(x, y++, value.substr(line_start, split - line_start), a.style.text_dim, a.style.background, false, w);
        pos = split;
    }
    a.cursor_y = y;
}

void separator() {
    using namespace detail;
    App& a = app();
    draw_hline(a.content_x, a.cursor_y, a.content_w, '-', a.style.border, a.style.background);
    a.cursor_y += 1;
}

void spacing(int rows) {
    detail::app().cursor_y += std::max(0, rows);
}

void spacing(float rows) {
    detail::app().cursor_y += std::max(0, (int)std::round(rows));
}

static bool button_impl(const char* label, bool has_color, Color custom_color) {
    using namespace detail;
    App& a = app();
    std::string name = visible_label(label);
    unsigned id = hash_label(label);
    int natural_w = std::min(a.content_w, std::max(12, (int)name.size() + 6));
    Rect r{a.content_x, a.cursor_y, consume_width(natural_w), 3};
    bool focused = register_focus(id, r);
    bool mouse_hit = contains(r, a.in.mouse_x, a.in.mouse_y);
    bool pressed = !disabled() && ((focused && (a.in.enter || a.in.space)) || (mouse_hit && a.in.mouse_released));
    Motion& m = motion(id);
    animate_to(m.active, (focused || mouse_hit) ? 1.0f : 0.0f);
    Color base = has_color ? custom_color : consume_role_color(ColorRole::Button);
    Color active = has_color ? lerp(custom_color, a.style.text, 0.22f) : role_color(ColorRole::ButtonActive);
    float lit = std::max({m.focus, m.active, m.hover * 0.7f});
    Color hover_base = has_color ? lerp(base, a.style.text, 0.12f) : role_color(ColorRole::ButtonHover);
    Color bg = lerp(lerp(base, hover_base, m.hover), active, lit * 0.65f);
    Color border = lerp(role_color(ColorRole::Border), role_color(ColorRole::Accent), m.focus);
    draw_box(r, border, bg);
    std::string face = " " + name + " ";
    int tx = r.x + std::max(1, (r.w - (int)face.size()) / 2);
    Color fg = disabled() ? a.style.text_dim : a.style.text;
    draw_text(tx, r.y + 1, face, fg, bg, focused, r.w - 2);
    if (focused) {
        char spinner[] = {'-', '\\', '|', '/'};
        int frame = (int)(animation_time() * 8.0f) & 3;
        put(r.x + 1, r.y + 1, spinner[frame], a.style.accent, bg, true);
    }
    a.cursor_y += 4;
    return pressed;
}

bool button(const char* label) {
    return button_impl(label, false, {});
}

bool button(const char* label, ColorRole role) {
    return button_impl(label, true, detail::role_color(role));
}

bool button(const char* label, Color color) {
    return button_impl(label, true, color);
}

bool input(const char* label, char* buffer, int buffer_size, InputFlags flags, bool* enter_pressed) {
    using namespace detail;
    App& a = app();
    if (enter_pressed) *enter_pressed = false;
    if (!buffer || buffer_size <= 0) return false;
    std::string name = visible_label(label);
    unsigned id = hash_label(label);
    int label_w = std::min((int)name.size(), std::max(8, a.content_w / 3));
    Rect r{a.content_x, a.cursor_y + 1, consume_width(a.content_w), 3};
    draw_text(a.content_x, a.cursor_y, name, a.style.text_dim, a.style.background, false, a.content_w);
    bool focused = register_focus(id, r);
    if (focused) a.text_focus_id = (int)id;
    bool changed = false;
    if (focused && !disabled() && !(flags & InputFlags::ReadOnly)) {
        int len = (int)std::strlen(buffer);
        if (a.in.backspace && len > 0) {
            buffer[len - 1] = '\0';
            changed = true;
        }
        for (int i = 0; i < a.in.text_count; ++i) {
            char ch = a.in.text[i];
            if (!accept_char(ch, flags)) continue;
            len = (int)std::strlen(buffer);
            if (len < buffer_size - 1) {
                buffer[len] = ch;
                buffer[len + 1] = '\0';
                changed = true;
            }
        }
        if (a.in.enter && enter_pressed) *enter_pressed = true;
    }
    Motion& m = motion(id);
    animate_to(m.focus, focused ? 1.0f : 0.0f);
    Color border = lerp(a.style.border, a.style.input_focus, std::max(m.focus, m.hover * 0.55f));
    Color bg = lerp(a.style.input_bg, a.style.panel_alt, std::max(m.focus * 0.45f, m.hover * 0.28f));
    draw_box(r, border, bg);
    std::string visible;
    if (flags & InputFlags::Password) visible.assign(std::strlen(buffer), '*');
    else visible = buffer;
    int max_text = std::max(1, r.w - 4);
    if ((int)visible.size() > max_text) visible = visible.substr(visible.size() - (size_t)max_text);
    draw_text(r.x + 2, r.y + 1, visible, a.style.text, bg, false, max_text);
    if (focused && (int)visible.size() < max_text) {
        put(r.x + 2 + (int)visible.size(), r.y + 1, '_', a.style.accent, bg, true);
    }
    (void)label_w;
    a.cursor_y += 5;
    return changed;
}

static std::vector<std::string> split_lines_simple(const char* text) {
    std::vector<std::string> lines;
    std::string cur;
    for (const char* p = text ? text : ""; *p; ++p) {
        if (*p == '\n') {
            lines.push_back(cur);
            cur.clear();
        } else {
            cur.push_back(*p);
        }
    }
    lines.push_back(cur);
    return lines;
}

bool text_area(const char* label, char* buffer, int buffer_size, int rows) {
    return text_area_ex(label, buffer, buffer_size, rows, TextAreaFlags::Default);
}

bool text_area_ex(const char* label, char* buffer, int buffer_size, int rows, TextAreaFlags flags) {
    using namespace detail;
    App& a = app();
    if (!buffer || buffer_size <= 0) return false;
    rows = std::max(1, rows);
    std::string name = visible_label(label);
    unsigned id = hash_label(label);
    draw_text(a.content_x, a.cursor_y, name, a.style.text_dim, a.style.background, false, a.content_w);
    Rect r{a.content_x, a.cursor_y + 1, consume_width(a.content_w), rows + 2};
    bool focused = register_focus(id, r);
    if (focused) a.text_focus_id = (int)id;
    bool hit = contains(r, a.in.mouse_x, a.in.mouse_y);
    bool changed = false;
    if (focused && !disabled() && !(flags & TextAreaFlags::ReadOnly)) {
        int len = (int)std::strlen(buffer);
        if (a.in.backspace && len > 0) {
            buffer[len - 1] = '\0';
            changed = true;
        }
        if (a.in.enter && len < buffer_size - 1) {
            buffer[len++] = '\n';
            buffer[len] = '\0';
            changed = true;
        }
        for (int i = 0; i < a.in.text_count; ++i) {
            len = (int)std::strlen(buffer);
            if (len < buffer_size - 1) {
                buffer[len] = a.in.text[i];
                buffer[len + 1] = '\0';
                changed = true;
            }
        }
    }
    Motion& m = motion(id);
    Color border = lerp(a.style.border, a.style.input_focus, std::max(m.focus, m.hover * 0.55f));
    Color bg = lerp(a.style.input_bg, a.style.panel_alt, std::max(m.focus * 0.35f, m.hover * 0.22f));
    draw_box(r, border, bg);
    std::vector<std::string> lines = split_lines_simple(buffer);
    ScrollState& ss = scroll_state(id);
    int max_scroll = std::max(0, (int)lines.size() - rows);
    if (hit && a.in.wheel != 0) ss.target += (float)a.in.wheel * 3.0f;
    if (focused && a.in.up) ss.target -= 1.0f;
    if (focused && a.in.down) ss.target += 1.0f;
    ss.target = clamp_f(ss.target, 0.0f, (float)max_scroll);
    ease_to(ss.current, ss.target, 0.22f);
    int first = clamp_i((int)std::round(ss.current), 0, max_scroll);
    int max_w = std::max(1, r.w - 4);
    for (int i = 0; i < rows && first + i < (int)lines.size(); ++i) {
        std::string line = lines[(size_t)(first + i)];
        if ((int)line.size() > max_w) line = line.substr(line.size() - (size_t)max_w);
        draw_text(r.x + 2, r.y + 1 + i, line, a.style.text, bg, false, max_w);
    }
    if (focused) put(r.x + r.w - 2, r.y, '*', a.style.accent, bg, true);
    a.cursor_y += rows + 4;
    return changed;
}

void log_view(const char* label, const char* text_value, int rows, LogViewFlags flags) {
    using namespace detail;
    App& a = app();
    rows = std::max(1, rows);
    std::string name = visible_label(label);
    unsigned id = hash_label(label);
    draw_text(a.content_x, a.cursor_y, name, a.style.text_dim, a.style.background, false, a.content_w);
    Rect r{a.content_x, a.cursor_y + 1, consume_width(a.content_w), rows + 2};
    bool focused = register_focus(id, r);
    std::vector<std::string> lines = split_lines_simple(text_value);
    ScrollState& ss = scroll_state(id);
    int max_scroll = std::max(0, (int)lines.size() - rows);
    bool hit = contains(r, a.in.mouse_x, a.in.mouse_y);
    bool manual_scroll = (focused || hit) && (a.in.wheel != 0 || a.in.up || a.in.down);
    if ((flags & LogViewFlags::AutoScrollBottom) && !manual_scroll) ss.target = (float)max_scroll;
    if ((focused || hit) && a.in.wheel != 0) ss.target += (float)a.in.wheel * 3.0f;
    if (focused && a.in.up) ss.target -= 1.0f;
    if (focused && a.in.down) ss.target += 1.0f;
    ss.target = clamp_f(ss.target, 0.0f, (float)max_scroll);
    ease_to(ss.current, ss.target, 0.22f);
    int first = clamp_i((int)std::round(ss.current), 0, max_scroll);
    Color bg = a.style.panel;
    draw_box(r, a.style.border, bg);
    int max_w = std::max(1, r.w - 4);
    for (int i = 0; i < rows && first + i < (int)lines.size(); ++i) {
        draw_text(r.x + 2, r.y + 1 + i, lines[(size_t)(first + i)], a.style.text_dim, bg, false, max_w);
    }
    if (max_scroll > 0) {
        char pct[32];
        std::snprintf(pct, sizeof(pct), "%d/%d", first, max_scroll);
        draw_text(r.x + r.w - (int)std::strlen(pct) - 2, r.y, pct, a.style.text_dim, bg, false);
    }
    a.cursor_y += rows + 4;
}

bool checkbox(const char* label, bool* value) {
    using namespace detail;
    App& a = app();
    if (!value) return false;
    std::string name = visible_label(label);
    unsigned id = hash_label(label);
    Rect r{a.content_x, a.cursor_y, std::min(a.content_w, (int)name.size() + 8), 1};
    bool focused = register_focus(id, r);
    bool hit = contains(r, a.in.mouse_x, a.in.mouse_y);
    bool changed = false;
    if (!disabled() && ((focused && (a.in.enter || a.in.space)) || (hit && a.in.mouse_released))) {
        *value = !*value;
        changed = true;
    }
    Color bg = focused ? a.style.panel_alt : a.style.background;
    fill_rect(r, bg);
    std::string face = std::string("[") + (*value ? "x" : " ") + "] " + name;
    draw_text(r.x, r.y, face, *value ? a.style.accent : a.style.text, bg, focused, r.w);
    a.cursor_y += 2;
    return changed;
}

bool slider_float(const char* label, float* value, float min_v, float max_v) {
    using namespace detail;
    App& a = app();
    if (!value) return false;
    std::string name = visible_label(label);
    unsigned id = hash_label(label);
    Rect r{a.content_x, a.cursor_y, consume_width(a.content_w), 2};
    bool focused = register_focus(id, r);
    bool hit = contains(r, a.in.mouse_x, a.in.mouse_y);
    float old = *value;
    if (!disabled() && focused) {
        float step = (max_v - min_v) / 100.0f;
        if (step <= 0.0f) step = 0.01f;
        if (a.in.left) *value -= step;
        if (a.in.right) *value += step;
    }
    int bar_w = std::max(8, r.w - (int)name.size() - 12);
    int bx = r.x + (int)name.size() + 2;
    if (!disabled() && hit && (a.in.mouse_pressed || a.mouse_down)) {
        a.focused = (int)id;
        float t_mouse = bar_w <= 1 ? 0.0f : (float)(a.in.mouse_x - bx) / (float)(bar_w - 1);
        *value = min_v + clamp_f(t_mouse, 0.0f, 1.0f) * (max_v - min_v);
    }
    *value = clamp_f(*value, min_v, max_v);
    bool changed = std::abs(*value - old) > 0.0001f;
    float t = max_v == min_v ? 0.0f : (*value - min_v) / (max_v - min_v);
    int handle = clamp_i((int)std::round(t * (bar_w - 1)), 0, std::max(0, bar_w - 1));
    char val[32];
    std::snprintf(val, sizeof(val), " %.2f", *value);
    Color label_color = hit ? a.style.text : a.style.text_dim;
    draw_text(r.x, r.y, name, label_color, a.style.background, focused || hit, r.w);
    for (int i = 0; i < bar_w; ++i) {
        Color rail = hit ? lerp(a.style.border, a.style.accent, 0.35f) : a.style.border;
        char ch = i == handle ? 'O' : (i < handle ? '<' : '-');
        Color fg = i == handle ? a.style.accent : rail;
        put(bx + i, r.y, ch, fg, a.style.background, focused || hit);
    }
    draw_text(bx + bar_w + 1, r.y, val, a.style.text, a.style.background, false, r.w - bar_w - 1);
    a.cursor_y += 2;
    return changed;
}

bool tabs(const char* const* labels, int count, int* selected) {
    using namespace detail;
    App& a = app();
    if (!labels || count <= 0 || !selected) return false;
    *selected = clamp_i(*selected, 0, count - 1);
    unsigned id = hash_label("##ftti_tabs");
    for (int i = 0; i < count; ++i) id = (id ^ hash_label(labels[i])) * 16777619u;
    id ^= (unsigned)(a.cursor_x * 131 + a.cursor_y * 313);
    Rect r{a.content_x, a.cursor_y, a.content_w, 3};
    bool focused = register_focus(id, r);
    bool bar_hit = contains(r, a.in.mouse_x, a.in.mouse_y);
    if (focused || bar_hit) a.active_tabs_id = (int)id;
    TabState& ts = tab_state(id, *selected);
    ts.target = (float)*selected;
    ease_to(ts.current, ts.target, 0.24f);
    bool changed = false;
    if (!disabled() && a.tab_command_delta != 0 && a.active_tabs_id == (int)id) {
        int next = clamp_i(*selected + a.tab_command_delta, 0, count - 1);
        changed = changed || next != *selected;
        *selected = next;
        a.tab_command_delta = 0;
    }
    if (!disabled() && focused && a.in.left && *selected > 0) {
        --*selected;
        changed = true;
    }
    if (!disabled() && focused && a.in.right && *selected < count - 1) {
        ++*selected;
        changed = true;
    }
    if (!disabled() && focused && (a.in.enter || a.in.space)) {
        *selected = (*selected + 1) % count;
        changed = true;
    }

    int x = r.x;
    std::vector<int> tab_xs;
    std::vector<int> tab_ws;
    for (int i = 0; i < count; ++i) {
        std::string name = visible_label(labels[i]);
        int w = (int)name.size() + 4;
        tab_xs.push_back(x);
        tab_ws.push_back(w);
        Rect tr{x, r.y, w, 3};
        bool hit = contains(tr, a.in.mouse_x, a.in.mouse_y);
        if (!disabled() && hit && (a.in.mouse_pressed || a.in.mouse_released) && *selected != i) {
            *selected = i;
            changed = true;
            a.focused = (int)id;
        }
        bool active = i == *selected;
        Color bg = active ? a.style.panel_alt : (hit ? a.style.button_hover : a.style.background);
        Color fg = active || hit ? a.style.text : a.style.text_dim;
        fill_rect(tr, bg);
        draw_text(x + 2, r.y + 1, name, fg, bg, active, w - 4);
        x += w + 1;
        if (x >= r.x + r.w) break;
    }
    int underline_x = r.x;
    int underline_w = 0;
    if (!tab_xs.empty()) {
        float cur = clamp_f(ts.current, 0.0f, (float)tab_xs.size() - 1.0f);
        int lo = clamp_i((int)std::floor(cur), 0, (int)tab_xs.size() - 1);
        int hi = clamp_i((int)std::ceil(cur), 0, (int)tab_xs.size() - 1);
        float frac = cur - (float)lo;
        underline_x = (int)std::round((float)tab_xs[(size_t)lo] + ((float)tab_xs[(size_t)hi] - (float)tab_xs[(size_t)lo]) * frac);
        underline_w = (int)std::round((float)tab_ws[(size_t)lo] + ((float)tab_ws[(size_t)hi] - (float)tab_ws[(size_t)lo]) * frac);
    }
    if (underline_w > 2) draw_hline(underline_x + 1, r.y + 2, underline_w - 2, '=', a.style.accent, a.style.background);
    if (focused) put(r.x, r.y + 1, '>', a.style.accent, a.style.background, true);
    a.cursor_y += 4;
    return changed;
}

bool side_menu(const char* label, const char* const* items, int count, int* selected) {
    using namespace detail;
    App& a = app();
    if (!items || count <= 0 || !selected) return false;
    *selected = clamp_i(*selected, 0, count - 1);
    unsigned id = hash_label(label ? label : "##ftti_side_menu");
    int width = 18;
    for (int i = 0; i < count; ++i) width = std::max(width, (int)visible_label(items[i]).size() + 6);
    width = std::min(width, std::max(18, a.width / 3));
    Rect panel{0, 2, width, a.height - 2};
    fill_rect(panel, a.style.panel);
    draw_hline(panel.x, panel.y, panel.w, '-', a.style.border, a.style.panel);
    std::string title = visible_label(label);
    draw_text(2, 3, title, a.style.text, a.style.panel, true, width - 4);
    Rect focus_rect{panel.x, panel.y, panel.w, panel.h};
    bool focused = register_focus(id, focus_rect);
    bool changed = false;
    if (!disabled() && focused && a.in.up && *selected > 0) {
        --*selected;
        changed = true;
    }
    if (!disabled() && focused && a.in.down && *selected < count - 1) {
        ++*selected;
        changed = true;
    }
    int y = 5;
    for (int i = 0; i < count && y < a.height - 1; ++i, ++y) {
        Rect item{1, y, width - 2, 1};
        bool hit = contains(item, a.in.mouse_x, a.in.mouse_y);
        if (!disabled() && hit && a.in.mouse_released && *selected != i) {
            *selected = i;
            changed = true;
            a.focused = (int)id;
        }
        bool active = i == *selected;
        Color bg = active ? a.style.panel_alt : a.style.panel;
        Color fg = active ? a.style.text : a.style.text_dim;
        fill_rect(item, bg);
        put(2, y, active ? '>' : ' ', active ? a.style.accent : fg, bg, active);
        draw_text(4, y, visible_label(items[i]), fg, bg, active, width - 6);
    }
    if (focused) put(width - 2, 3, '*', a.style.accent, a.style.panel, true);
    a.content_x = width + 2;
    a.cursor_x = a.content_x;
    a.cursor_y = 3;
    a.content_w = std::max(10, a.width - a.content_x - 2);
    return changed;
}

bool listbox(const char* label, const char* const* items, int count, int* selected, int visible_rows) {
    using namespace detail;
    App& a = app();
    if (!items || count <= 0 || !selected) return false;
    *selected = clamp_i(*selected, 0, count - 1);
    visible_rows = clamp_i(visible_rows, 1, count);
    std::string name = visible_label(label);
    unsigned id = hash_label(label);
    draw_text(a.content_x, a.cursor_y, name, a.style.text_dim, a.style.background, false, a.content_w);
    Rect r{a.content_x, a.cursor_y + 1, consume_width(a.content_w), visible_rows + 2};
    bool focused = register_focus(id, r);
    bool changed = false;
    bool box_hit = contains(r, a.in.mouse_x, a.in.mouse_y);
    if (!disabled() && (focused || box_hit)) {
        if (a.in.up && *selected > 0) { --*selected; changed = true; }
        if (a.in.down && *selected < count - 1) { ++*selected; changed = true; }
        if (a.in.wheel != 0) {
            int next = clamp_i(*selected + a.in.wheel, 0, count - 1);
            changed = changed || next != *selected;
            *selected = next;
        }
    }
    int first = clamp_i(*selected - visible_rows / 2, 0, std::max(0, count - visible_rows));
    draw_box(r, focused ? a.style.accent : a.style.border, a.style.panel);
    for (int i = 0; i < visible_rows; ++i) {
        int item_i = first + i;
        Rect item{r.x + 1, r.y + 1 + i, r.w - 2, 1};
        bool hit = contains(item, a.in.mouse_x, a.in.mouse_y);
        if (!disabled() && hit && a.in.mouse_released && *selected != item_i) {
            *selected = item_i;
            a.focused = (int)id;
            changed = true;
        }
        bool active = item_i == *selected;
        Color bg = active ? a.style.panel_alt : a.style.panel;
        fill_rect(item, bg);
        put(item.x + 1, item.y, active ? '>' : ' ', active ? a.style.accent : a.style.text_dim, bg, active);
        draw_text(item.x + 3, item.y, visible_label(items[item_i]), active ? a.style.text : a.style.text_dim, bg, active, item.w - 4);
    }
    a.cursor_y += visible_rows + 4;
    return changed;
}

bool dropdown(const char* label, const char* const* items, int count, int* selected, int popup_rows) {
    using namespace detail;
    App& a = app();
    if (!items || count <= 0 || !selected) return false;
    *selected = clamp_i(*selected, 0, count - 1);
    std::string name = visible_label(label);
    std::string current = visible_label(items[*selected]);
    unsigned id = hash_label(label);
    Rect r{a.content_x, a.cursor_y, consume_width(a.content_w), 3};
    bool focused = register_focus(id, r);
    bool changed = false;
    if (!disabled() && focused) {
        if ((a.in.down || a.in.enter || a.in.space) && *selected < count - 1) { ++*selected; changed = true; }
        if (a.in.up && *selected > 0) { --*selected; changed = true; }
    }
    draw_box(r, focused ? a.style.accent : a.style.border, a.style.panel);
    std::string line = name + ": " + current;
    draw_text(r.x + 2, r.y + 1, line, a.style.text, a.style.panel, focused, r.w - 4);
    if (focused) {
        int rows = std::min(count, popup_rows);
        int first = clamp_i(*selected - rows / 2, 0, std::max(0, count - rows));
        Rect pop{r.x + 2, r.y + 3, std::min(r.w - 4, 32), rows + 2};
        if (pop.y + pop.h >= a.height) pop.y = std::max(2, r.y - pop.h);
        draw_box(pop, a.style.accent, a.style.panel_alt);
        for (int i = 0; i < rows; ++i) {
            int idx = first + i;
            bool active = idx == *selected;
            Color bg = active ? a.style.button_active : a.style.panel_alt;
            Rect item{pop.x + 1, pop.y + 1 + i, pop.w - 2, 1};
            fill_rect(item, bg);
            draw_text(item.x + 1, item.y, visible_label(items[idx]), active ? a.style.text : a.style.text_dim, bg, active, item.w - 2);
        }
    }
    a.cursor_y += 4;
    return changed;
}

bool radio_group(const char* label, const char* const* items, int count, int* selected, int columns) {
    (void)columns;
    using namespace detail;
    App& a = app();
    if (!items || count <= 0 || !selected) return false;
    *selected = clamp_i(*selected, 0, count - 1);
    std::string name = visible_label(label);
    unsigned id = hash_label(label);
    draw_text(a.content_x, a.cursor_y, name, a.style.text_dim, a.style.background, false, a.content_w);
    Rect r{a.content_x, a.cursor_y + 1, consume_width(a.content_w), count};
    bool focused = register_focus(id, r);
    bool changed = false;
    if (!disabled() && focused) {
        if (a.in.up && *selected > 0) { --*selected; changed = true; }
        if (a.in.down && *selected < count - 1) { ++*selected; changed = true; }
    }
    for (int i = 0; i < count; ++i) {
        Rect item{r.x, r.y + i, r.w, 1};
        bool hit = contains(item, a.in.mouse_x, a.in.mouse_y);
        if (!disabled() && hit && a.in.mouse_released && *selected != i) {
            *selected = i;
            a.focused = (int)id;
            changed = true;
        }
        bool active = i == *selected;
        Color bg = active ? a.style.panel_alt : a.style.background;
        fill_rect(item, bg);
        std::string line = std::string(active ? "(o) " : "( ) ") + visible_label(items[i]);
        draw_text(item.x, item.y, line, active ? a.style.accent : a.style.text_dim, bg, active, item.w);
    }
    a.cursor_y += count + 2;
    return changed;
}

bool collapsing_header(const char* label, bool* open) {
    using namespace detail;
    App& a = app();
    static bool fallback_open = true;
    bool* state = open ? open : &fallback_open;
    std::string name = visible_label(label);
    unsigned id = hash_label(label);
    Rect r{a.content_x, a.cursor_y, std::min(a.content_w, (int)name.size() + 6), 1};
    bool focused = register_focus(id, r);
    bool hit = contains(r, a.in.mouse_x, a.in.mouse_y);
    if (!disabled() && ((focused && (a.in.enter || a.in.space)) || (hit && a.in.mouse_released))) {
        *state = !*state;
    }
    Color bg = focused ? a.style.panel_alt : a.style.background;
    fill_rect(r, bg);
    std::string line = std::string(*state ? "v " : "> ") + name;
    draw_text(r.x, r.y, line, a.style.text, bg, true, r.w);
    a.cursor_y += 2;
    return *state;
}

void row(int cols, std::function<void()> fn) {
    (void)cols;
    if (fn) fn();
}

void row(std::initializer_list<float> weights, std::function<void()> fn) {
    (void)weights;
    if (fn) fn();
}

void split(std::initializer_list<float> columns, std::function<void()> fn) {
    (void)columns;
    if (fn) fn();
}

void side_layout(float side_width, std::function<void()> fn) {
    detail::App& a = detail::app();
    a.content_x = std::max(2, (int)side_width + 2);
    a.cursor_x = a.content_x;
    a.content_w = std::max(10, a.width - a.content_x - 2);
    if (fn) fn();
}

void content(std::function<void()> fn) {
    if (fn) fn();
}

void scroll_area(const char* label, float height, std::function<void()> fn) {
    using namespace detail;
    App& a = app();
    unsigned id = hash_label(label);
    int rows = std::max(1, (int)height);
    if (height > 24.0f) rows = std::max(1, (int)(height / 24.0f));
    Rect r{a.content_x, a.cursor_y, a.content_w, std::min(rows, std::max(1, a.height - a.cursor_y - 1))};
    bool focused = register_focus(id, r);
    ScrollState& ss = scroll_state(id);
    bool hit = contains(r, a.in.mouse_x, a.in.mouse_y);
    if ((focused || hit) && a.in.wheel != 0) ss.target += (float)a.in.wheel * 2.0f;
    ss.target = std::max(0.0f, ss.target);
    ease_to(ss.current, ss.target, 0.20f);
    draw_box({r.x, r.y, r.w, r.h}, focused ? a.style.accent : a.style.border, a.style.panel);
    int old_x = a.content_x;
    int old_y = a.cursor_y;
    int old_w = a.content_w;
    bool old_clip_active = a.clip_active;
    Rect old_clip = a.clip_rect;
    a.content_x = r.x + 2;
    a.cursor_y = r.y + 1 - (int)std::round(ss.current);
    a.content_w = std::max(4, r.w - 4);
    a.clip_active = true;
    a.clip_rect = {r.x + 1, r.y + 1, std::max(0, r.w - 2), std::max(0, r.h - 2)};
    if (fn) fn();
    a.clip_active = old_clip_active;
    a.clip_rect = old_clip;
    int used = a.cursor_y + (int)std::round(ss.current) - (r.y + 1);
    ss.target = clamp_f(ss.target, 0.0f, (float)std::max(0, used - (r.h - 2)));
    a.content_x = old_x;
    a.cursor_y = old_y + r.h + 1;
    a.content_w = old_w;
}

void set_next_width(float cols) {
    detail::app().next_width = std::max(0, (int)cols);
}

void set_next_fill() {
    detail::app().next_fill = true;
}

void set_next_percent(float pct) {
    detail::app().next_percent = pct;
}

void set_next_limits(float min_cols, float max_cols) {
    detail::App& a = detail::app();
    a.next_min = std::max(0, (int)min_cols);
    a.next_max = std::max(0, (int)max_cols);
}

void set_next_align(Align align) {
    detail::app().next_align = align;
}

void open_modal(const char* label) {
    detail::app().modal_id = (int)detail::hash_label(label);
}

bool modal(const char* label, std::function<void()> fn) {
    using namespace detail;
    App& a = app();
    int id = (int)hash_label(label);
    if (a.modal_id != id) return false;
    Rect r{std::max(2, a.width / 8), std::max(2, a.height / 5), std::max(20, a.width * 3 / 4), std::max(8, a.height * 3 / 5)};
    fill_rect({0, 2, a.width, a.height - 2}, lerp(a.style.background, a.style.panel, 0.45f));
    draw_box(r, a.style.accent, a.style.panel);
    draw_text(r.x + 2, r.y, visible_label(label), a.style.text, a.style.panel, true, r.w - 4);
    int old_x = a.content_x;
    int old_y = a.cursor_y;
    int old_w = a.content_w;
    a.content_x = r.x + 2;
    a.cursor_y = r.y + 2;
    a.content_w = std::max(4, r.w - 4);
    if (fn) fn();
    if (a.close_modal_requested || a.in.escape) {
        a.modal_id = 0;
        a.close_modal_requested = false;
    }
    a.content_x = old_x;
    a.cursor_y = old_y;
    a.content_w = old_w;
    return true;
}

void close_modal() {
    detail::app().close_modal_requested = true;
}

void begin_disabled() {
    ++detail::app().disabled_depth;
}

void end_disabled() {
    detail::App& a = detail::app();
    if (a.disabled_depth > 0) --a.disabled_depth;
}

void tooltip(const char* text_value) {
    detail::App& a = detail::app();
    if (!text_value || !*text_value || !a.last_item_id) return;
    if (a.hovered == (int)a.last_item_id || a.focused == (int)a.last_item_id) {
        a.pending_tooltip = text_value;
        a.pending_tooltip_anchor = a.last_item_rect;
        a.pending_tooltip_owner = a.last_item_id;
    }
}

void request_focus(const char* label) {
    detail::app().request_focus_id = (int)detail::hash_label(label);
}

float calc_text_width(const char* text_value) {
    return (float)std::strlen(text_value ? text_value : "");
}

float calc_text_height(const char* text_value, float wrap_width) {
    int w = std::max(1, (int)wrap_width);
    int n = (int)std::strlen(text_value ? text_value : "");
    return (float)std::max(1, (n + w - 1) / w);
}

void toast(const Toast& value) {
    detail::app().toasts.push_back({value, std::chrono::steady_clock::now()});
}

void toast(const char* message) {
    toast(Toast{message, ToastType::Info, 3500, true});
}

void toast_info(const char* message) {
    toast(Toast{message, ToastType::Info, 3500, true});
}

void toast_success(const char* message) {
    toast(Toast{message, ToastType::Success, 3500, true});
}

void toast_warning(const char* message) {
    toast(Toast{message, ToastType::Warning, 3500, true});
}

void toast_error(const char* message) {
    toast(Toast{message, ToastType::Error, 3500, true});
}

void clear_toasts() {
    detail::app().toasts.clear();
}

void progress_bar(float progress) {
    progress_bar(progress, ProgressStyle{});
}

void progress_bar(float progress, const char* label_or_mask_path) {
    ProgressStyle style;
    style.label = label_or_mask_path;
    style.mask_path = label_or_mask_path;
    progress_bar(progress, style);
}

void progress_bar(float progress, const ProgressStyle& style) {
    using namespace detail;
    App& a = app();
    progress = clamp_f(progress, 0.0f, 1.0f);
    int w = consume_width(a.content_w);
    Rect r{a.content_x, a.cursor_y, w, 1};
    int fill = clamp_i((int)std::round(progress * w), 0, w);
    Color fill_color = style.fill_color.r >= 0.0f ? style.fill_color : role_color(style.fill_role);
    for (int i = 0; i < w; ++i) put(r.x + i, r.y, i < fill ? '=' : '-', i < fill ? fill_color : a.style.border, a.style.background, true);
    if (style.label) draw_text(r.x, r.y, style.label, a.style.text, a.style.background, true, w);
    if (style.show_percent && w > 8) {
        char pct[16];
        std::snprintf(pct, sizeof(pct), "%3d%%", (int)std::round(progress * 100.0f));
        draw_text(r.x + w - 5, r.y, pct, a.style.text, a.style.background, true, 5);
    }
    a.cursor_y += 2;
}

void image(ImageHandle* img, float width, float height) {
    using namespace detail;
    App& a = app();
    int cols = std::max(4, (int)std::round(width));
    int rows = std::max(2, (int)std::round(height));
    if (width > 80.0f) cols = std::max(4, (int)std::round(width / 8.0f));
    if (height > 24.0f) rows = std::max(2, (int)std::round(height / 16.0f));
    cols = std::min(cols, a.content_w);
    rows = std::min(rows, std::max(2, a.height - a.cursor_y - 1));
    Rect r{a.content_x, a.cursor_y, cols, rows};
    draw_box(r, a.style.border, a.style.panel);

    ImageData* data = img ? static_cast<ImageData*>(img->_impl) : nullptr;
    if (data && !data->path.empty()) {
        std::string payload = base64_encode(data->path);
        char prefix[160];
        std::snprintf(prefix, sizeof(prefix),
                      "\x1b[%d;%dH\x1b_Ga=T,f=100,t=f,q=2,i=%u,c=%d,r=%d;",
                      r.y + 1, r.x + 1, data->id, cols, rows);
        a.graphics_commands.push_back(std::string(prefix) + payload + "\x1b\\");
    } else {
        draw_text(r.x + 2, r.y + rows / 2, "[image]", a.style.text_dim, a.style.panel, false, r.w - 4);
    }
    a.cursor_y += rows + 1;
}

ImageHandle* load_image(const char* utf8_path) {
    if (!utf8_path || !*utf8_path) return nullptr;
    static unsigned next_id = 1000;
    ImageHandle* h = new ImageHandle;
    detail::ImageData* data = new detail::ImageData;
    data->path = utf8_path;
    data->id = next_id++;
    h->_impl = data;
    return h;
}

void free_image(ImageHandle* img) {
    if (!img) return;
    delete static_cast<detail::ImageData*>(img->_impl);
    delete img;
}

std::string open_file_dialog(const char* title, const FileFilter* filters, int filter_count) {
    (void)title;
    (void)filters;
    (void)filter_count;
    return "";
}

void open_child_window(const Config& cfg, std::function<void()> fn) {
    (void)cfg;
    if (fn) fn();
}

DebugState& debug() {
    return detail::app().dbg;
}

float animation_time() {
    using namespace std::chrono;
    detail::App& a = detail::app();
    return duration_cast<duration<float>>(steady_clock::now() - a.start_time).count();
}

int terminal_width() {
    return detail::app().width;
}

int terminal_height() {
    return detail::app().height;
}

} // namespace ftti

#endif // FTTI_IMPLEMENTATION
