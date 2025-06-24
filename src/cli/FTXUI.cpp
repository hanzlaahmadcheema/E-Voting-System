#include <custom/config.h>


int ShowMenu(ScreenInteractive& screen, 
             const string& heading, 
             const vector<string>& options) {

    system("cls");
    int selected = 0;
    int animation_step = 0;
    const int max_animation = 8; // Number of animation steps

    auto menu = Menu(&options, &selected);
    auto menu_with_enter = CatchEvent(menu, [&](Event event) {
        if (event == Event::Return) {
            screen.Exit();  // Exit loop on Enter
            return true;
        }
        return false;
    });

    auto renderer = Renderer(menu_with_enter, [&] {
        // Heading in green, options in white, border in Cyan
        return vbox({
                   text(heading) | bold | center | color(Color::Green),
                   separator(),
                   menu->Render() | color(Color::White) | frame,
               }) | border | color(Color::Cyan) | size(WIDTH, EQUAL, 50) | center;
    });

    // Animation thread: increase animation_step until max_animation
    std::atomic<bool> running = true;
    std::thread anim([&] {
        while (animation_step < max_animation && running) {
            std::this_thread::sleep_for(std::chrono::milliseconds(40));
            ++animation_step;
            screen.PostEvent(Event::Custom);
        }
    });

    screen.Loop(renderer);
    running = false;
    anim.join();
    system("cls");
    return selected;
}

void ShowTableFTXUI(const string& heading, 
                    const vector<string>& headers, 
                    const vector<vector<string>>& rows) {
    system("cls");
    // Calculate max width for each column
    vector<int> col_widths(headers.size(), 0);
    for (size_t i = 0; i < headers.size(); i++) {
        col_widths[i] = headers[i].size();
    }
    for (const auto& row : rows) {
        for (size_t i = 0; i < row.size(); i++) {
            col_widths[i] = max(col_widths[i], (int)row[i].size());
        }
    }

    // Helper: make a horizontal line with +---+---+ style
    auto make_separator = [&]() {
        Elements parts;
        parts.push_back(text("+"));
        for (auto w : col_widths) {
            parts.push_back(text(string(w + 2, '-')));
            parts.push_back(text("+"));
        }
        return hbox(move(parts));
    };

    // Helper: make a row with | data | data | style
    auto make_row = [&](const vector<string>& row_data, bool is_header = false) {
        Elements parts;
        parts.push_back(text("|"));
        for (size_t i = 0; i < row_data.size(); i++) {
            string cell = " " + row_data[i] + string(col_widths[i] - row_data[i].size() + 1, ' ');
            parts.push_back(text(cell) | (is_header ? bold : nothing));
            parts.push_back(text("|"));
        }
        return hbox(move(parts));
    };

    int scroll = 0;
    const int max_results = 5;
    int total_rows = (int)rows.size();

    auto renderer = Renderer([&] {
        Elements table_elements;

        // Heading
        table_elements.push_back(text(heading) | bold | center | border);

        // Table top border
        table_elements.push_back(make_separator());

        // Header row
        table_elements.push_back(make_row(headers, true));

        // Header-bottom border
        table_elements.push_back(make_separator());

        // Data rows (with scrolling)
        int start = scroll;
        int end = min(scroll + max_results, total_rows);
        for (int i = start; i < end; ++i) {
            table_elements.push_back(make_row(rows[i]));
            table_elements.push_back(make_separator());
        }

        // Scrollbar
        if (total_rows > max_results) {
            float ratio = max_results / (float)total_rows;
            int bar_size = max(1, int(ratio * max_results));
            int bar_pos = int((scroll / (float)total_rows) * max_results);
            Elements scrollbar;
            for (int i = 0; i < max_results; ++i) {
                if (i >= bar_pos && i < bar_pos + bar_size)
                    scrollbar.push_back(text("█"));
                else
                    scrollbar.push_back(text("│") | dim);
            }
            table_elements.push_back(hbox({filler(), vbox(scrollbar), filler()}));
        }

        table_elements.push_back(text("Use ↑/↓ to scroll, any other key to continue...") | center | dim);
        return vbox(move(table_elements)) | center | border;
    });

    bool done = false;
    auto event_handler = CatchEvent(renderer, [&](Event event) {
        if (event == Event::ArrowDown && scroll + max_results < total_rows) {
            scroll++;
            return true;
        }
        if (event == Event::ArrowUp && scroll > 0) {
            scroll--;
            return true;
        }
        done = true;
        screen.Exit();
        return true;
    });
    screen.Loop(event_handler);
    system("cls");
}

bool ShowForm(ScreenInteractive& screen, const string& title, vector<InputField>& fields) {
    vector<Component> components;
    bool submitted = false;
    system("cls");
    for (auto& field : fields) {
        switch (field.type) {
            case InputField::TEXT: {
                components.push_back(Input(field.value, ""));
                break;
            }
            case InputField::NUMBER: {
                components.push_back(Input(field.value, ""));
                break;
            }
            case InputField::DROPDOWN: {
                int* selected = new int(0);
                auto dropdown = Dropdown(&field.options, selected);
                components.push_back(Renderer(dropdown, [&, selected] {
                    if (!field.options.empty()) *field.value = field.options[*selected];
                    return dropdown->Render() | color(Color::White);
                }));
                break;
            }
            case InputField::RADIO: {
                int* selected = new int(0);
                auto radio = Radiobox(&field.options, selected);
                components.push_back(Renderer(radio, [&, selected] {
                    if (!field.options.empty()) *field.value = field.options[*selected];
                    return radio->Render() | color(Color::White);
                }));
                break;
            }
        }
    }

    // Submit Button Component
    auto submit_button = Button("Submit", [&] {
        submitted = true;
        screen.Exit();  // Ends the form loop
    });

    // Custom renderer for button: blue background, green text, fixed width
    auto button_renderer = Renderer(submit_button, [&] {
        return submit_button->Render()
            | color(Color::Green)
            | bgcolor(Color::Blue)
            | size(WIDTH, EQUAL, 18)
            | center;
    });

    components.push_back(button_renderer);

    auto container = Container::Vertical(components);

    auto renderer = Renderer(container, [&] {
        vector<Element> elems = {
            text(title) | bold | center | color(Color::Green),
            separator()
        };

        for (size_t i = 0; i < fields.size(); ++i) {
            elems.push_back(
                hbox({
                    text(fields[i].label + ": ") | color(Color::White),
                    components[i]->Render()
                }) | border | color(Color::Cyan)
            );
        }

        elems.push_back(separator());
        elems.push_back(components.back()->Render() | center);

        return vbox(elems)
            | border
            | color(Color::Cyan)
            | size(WIDTH, EQUAL, 50)
            | center;
    });

    screen.Loop(renderer);
    system("cls");
    return submitted;
}

void ShowSpinner(ScreenInteractive& screen, const string& message) {
    atomic<bool> loading = true;
    vector<string> spinner_frames = { "⠋", "⠙", "⠹", "⠸", "⠼", "⠴", "⠦", "⠧", "⠇", "⠏" };
    size_t frame = 0;

    system("cls");
    auto renderer = Renderer([&] {
        return vbox({
            filler(),
            hbox({
                filler(),
                text(spinner_frames[frame]) | bold | color(Color::Cyan),
                text("  "),
                text(message) | bold | color(Color::Green),
                filler()
            }),
            filler()
        }) | center;
    });

    thread anim([&] {
        while (loading) {
            frame = (frame + 1) % spinner_frames.size();
            screen.PostEvent(Event::Custom);
            this_thread::sleep_for(chrono::milliseconds(120));
        }
    });

    thread closer([&] {
        this_thread::sleep_for(chrono::seconds(3)); // Simulate loading
        loading = false;
        screen.Exit();
    });

    screen.Loop(renderer);
    anim.join();
    closer.join();
    system("cls");
}

void ShowProgressBar(ScreenInteractive& screen, const string& label) {
    int progress = 0;
    system("cls");

    auto renderer = Renderer([&] {
        return vbox({
            text(label) | bold | center,
            separator(),
            hbox({
                filler(),
                gauge(progress / 100.0f) | color(Color::Green) | size(WIDTH, EQUAL, 40),
                text(" ") | size(WIDTH, EQUAL, 2),
                text((to_string(progress) + "%")) | bold,
                filler()
            }) | center,
        }) | center;
    });

    thread worker([&] {
        while (progress < 100) {
            this_thread::sleep_for(chrono::milliseconds(18 + (progress % 7))); // Animation: variable speed
            progress++;
            screen.PostEvent(Event::Custom);
        }
        this_thread::sleep_for(chrono::milliseconds(400)); // Pause at 100%
        screen.Exit();
    });

    screen.Loop(renderer);
    worker.join();
    system("cls");
}

void ShowMessage(ScreenInteractive& screen, const string& msg, const string& type) {
    system("cls");
    Color msg_color = Color::White;
    string prefix;

    if (type == "success") {
        msg_color = Color::Green;
        prefix = "✔ Success: ";
    } else if (type == "error") {
        msg_color = Color::Red;
        prefix = "✖ Error: ";
    } else if (type == "info") {
        msg_color = Color::Blue;
        prefix = "ℹ Info: ";
    } else {
        prefix = "";
    }

    vector<string> words;
    string word, full_msg = prefix + msg;
    istringstream iss(full_msg);
    while (iss >> word) {
        words.push_back(word);
    }

    atomic<size_t> shown_words = 0;

    auto renderer = Renderer([&] {
        string animated;
        for (size_t i = 0; i < shown_words; ++i) {
            animated += words[i];
            if (i + 1 < shown_words) animated += " ";
        }
        return vbox({
            filler(),
            hbox({
                filler(),
                text(animated) | bold | color(msg_color) | border,
                filler(),
            }),
            filler(),
        });
    });

    thread animator([&] {
        for (size_t i = 1; i <= words.size(); ++i) {
            shown_words = i;
            screen.PostEvent(Event::Custom);
            this_thread::sleep_for(chrono::milliseconds(250));
        }
        this_thread::sleep_for(chrono::seconds(2));
        screen.Exit();
    });

    screen.Loop(renderer);
    animator.join();
    system("cls");
}