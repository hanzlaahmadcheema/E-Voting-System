#include <custom/config.h>

int ShowMenu(ScreenInteractive& screen, 
             const string& heading, 
             const vector<string>& options) {

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

    return selected;
}

void ShowTableFTXUI(const string& heading, 
                    const vector<string>& headers, 
                    const vector<vector<string>>& rows) {

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

    auto screen = ScreenInteractive::TerminalOutput();

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
        return vbox(move(table_elements)) | center | border;
    });

    screen.Loop(renderer);
}

bool ShowForm(ScreenInteractive& screen, const string& title, vector<InputField>& fields) {
    vector<Component> components;
    vector<Element> field_elements;
    bool submitted = false;

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
    return submitted;
}