#include "src/ui/gui/Gui.hpp"

void Ui::Gui::draw() noexcept {
    m_gui.draw();
}

void Ui::Gui::handleEvent(const sf::Event& event) const noexcept {
    m_gui.handleEvent(event);
}