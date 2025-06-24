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

void ShowTableFTXUI(ScreenInteractive& screen,
                    const string& heading,
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

    int scroll = 0;
    const int max_results = 5;
    int total_rows = (int)rows.size();

    auto renderer = Renderer([&] {
        Elements table_elements;

        // Heading
        table_elements.push_back(text(heading) | bold | center | color(Color::Green));

        // Header row (green)
        Elements header_cells;
        for (size_t i = 0; i < headers.size(); ++i) {
            string cell = " " + headers[i] + string(col_widths[i] - headers[i].size() + 1, ' ');
            header_cells.push_back(text(cell) | bold | color(Color::Green));
        }
        table_elements.push_back(hbox(move(header_cells)));
        // Header separator line
        Elements header_sep;
        for (size_t i = 0; i < headers.size(); ++i) {
            header_sep.push_back(text(string(col_widths[i] + 2, '─')) | color(Color::Cyan));
        }
        table_elements.push_back(hbox(move(header_sep)));

        // Data rows (white) with row separators
        int start = scroll;
        int end = min(scroll + max_results, total_rows);
        for (int i = start; i < end; ++i) {
            Elements row_cells;
            for (size_t j = 0; j < rows[i].size(); ++j) {
                string cell = " " + rows[i][j] + string(col_widths[j] - rows[i][j].size() + 1, ' ');
                row_cells.push_back(text(cell) | color(Color::White));
            }
            table_elements.push_back(hbox(move(row_cells)));
            // Row separator line (except after last row)
            if (i + 1 < end) {
                Elements row_sep;
                for (size_t j = 0; j < headers.size(); ++j) {
                    row_sep.push_back(text(string(col_widths[j] + 2, '─')) | color(Color::Cyan) | dim);
                }
                table_elements.push_back(hbox(move(row_sep)));
            }
        }

        table_elements.push_back(text("Use ↑/↓ to scroll, press Enter to continue...") | center | dim);
        return vbox(move(table_elements)) | border | color(Color::Cyan) | center;
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
        if (event == Event::Return) {
            done = true;
            screen.Exit();
            return true;
        }
        return false;
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
    Color header_color = Color::White;
    string header_text;

    if (type == "success") {
        header_color = Color::Green;
        header_text = "✔ Success";
    } else if (type == "error") {
        header_color = Color::Red;
        header_text = "✖ Error";
    } else if (type == "info") {
        header_color = Color::Blue;
        header_text = "ℹ Info";
    } else {
        header_text = "";
    }

    // Animate message text word by word
    vector<string> words;
    string word;
    istringstream iss(msg);
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
            text(header_text) | bold | center | color(header_color),
            separator(),
            hbox({
                filler(),
                text(animated) | color(Color::White) | center,
                filler()
            }),
        }) | border | color(Color::Cyan) | size(WIDTH, EQUAL, 50) | center;
    });

    thread animator([&] {
        for (size_t i = 1; i <= words.size(); ++i) {
            shown_words = i;
            screen.PostEvent(Event::Custom);
            this_thread::sleep_for(chrono::milliseconds(120));
        }
        this_thread::sleep_for(chrono::seconds(2));
        screen.Exit();
    });

    screen.Loop(renderer);
    animator.join();
    system("cls");
}