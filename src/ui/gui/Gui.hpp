#pragma once

#include "src/ui/Ui.hpp"

class Ui::Gui {
public:
    Gui(sf::RenderWindow& window);
    ~Gui() = default;

    void draw() noexcept;
    void handleEvent(const sf::Event& event) noexcept;
private:
    tgui::Gui m_gui;
};