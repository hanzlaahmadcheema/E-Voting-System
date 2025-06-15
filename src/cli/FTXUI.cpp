#include <custom/config.h>


int ShowMenu(ScreenInteractive& screen, 
             const string& heading, 
             const vector<string>& options) {

    system("cls");
    int selected = 0;

    auto menu = Menu(&options, &selected);
    auto menu_with_enter = CatchEvent(menu, [&](Event event) {
        if (event == Event::Return) {
            screen.Exit();  // Exit loop on Enter
            return true;
        }
        return false;
    });

    auto renderer = Renderer(menu_with_enter, [&] {
        return vbox({
                   text(heading) | bold | center,
                   separator(),
                   menu->Render() | frame,
               }) | border | size(WIDTH, EQUAL, 50) | center;
    });

    screen.Loop(renderer);
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

    bool done = false;
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

        // Data rows
        for (const auto& row : rows) {
            table_elements.push_back(make_row(row));
            table_elements.push_back(make_separator());
        }

        // Compose whole table vertically
        table_elements.push_back(text("Press any key to continue...") | center | dim);
        return vbox(move(table_elements)) | center | border;
    });
auto screen = ScreenInteractive::TerminalOutput();

    auto event_handler = CatchEvent(renderer, [&](Event event) {
        done = true;
        screen.Exit();
        return true;
    });
    screen.Loop(event_handler);
        system("cls");
}

bool ShowForm(ScreenInteractive& screen, const string& title, vector<InputField>& fields) {
    vector<Component> components;
    vector<Element> field_elements;
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
                    return dropdown->Render();
                }));
                break;
            }
            case InputField::RADIO: {
                int* selected = new int(0);
                auto radio = Radiobox(&field.options, selected);
                components.push_back(Renderer(radio, [&, selected] {
                    if (!field.options.empty()) *field.value = field.options[*selected];
                    return radio->Render();
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

    components.push_back(submit_button);

    auto container = Container::Vertical(components);

    auto renderer = Renderer(container, [&] {
        vector<Element> elems = {
            text(title) | bold | center,
            separator()
        };

        for (size_t i = 0; i < fields.size(); ++i) {
            elems.push_back(hbox({ text(fields[i].label + ": "), components[i]->Render() }) | border);
        }

        elems.push_back(separator());
        elems.push_back(components.back()->Render() | center);  // Submit button

        return vbox(elems) | border | size(WIDTH, LESS_THAN, 60) | center;
    });

    screen.Loop(renderer);
        system("cls");
    return submitted;
}

void ShowSpinner(ScreenInteractive& screen, const string& message) {
    atomic<bool> loading = true;
    string dots = "";
    
    system("cls");
    auto renderer = Renderer([&] {
        return hbox({
            text(message) | bold | color(Color::Green),
            text(dots)
        }) | center;
    });

    thread anim([&] {
        while (loading) {
            dots = (dots == "...") ? "" : dots + ".";
            screen.PostEvent(Event::Custom);
            this_thread::sleep_for(chrono::milliseconds(300));
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
            gauge(progress / 100.0f) | color(Color::Green) | size(WIDTH, EQUAL, 50)
        }) | center;
    });

    thread worker([&] {
        while (progress < 100) {
            this_thread::sleep_for(chrono::milliseconds(40));
            progress++;
            screen.PostEvent(Event::Custom);
        }
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