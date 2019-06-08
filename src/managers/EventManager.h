//
// Created by robin on 01.05.17.
//

#ifndef PMGUI_EVENTMANAGER_H
#define PMGUI_EVENTMANAGER_H

#include "cpplinq.hpp"
#include "SFML/Graphics.hpp"
#include <deque>
using namespace cpplinq;

namespace pmgui
{
    class EventManager
    {
        public:
            EventManager() = default;
            EventManager(sf::RenderWindow *renderWindow);
            ~EventManager();

            void initialize(sf::RenderWindow *renderWindow);
            void update();
            std::vector<sf::Event> getEvents(const sf::Event::EventType &eventType = sf::Event::KeyPressed);
            std::vector<sf::Event> & getAllEvents();
            bool isKeyPressed(const sf::Keyboard::Key &key);
            bool isKeyDown(const sf::Keyboard::Key &key);
            bool isKeyReleased(const sf::Keyboard::Key &key);

        protected:
            void fillEventList();
            void manageKeyReleased();

            std::vector<sf::Keyboard::Key> m_pressedKeys;
            std::vector<sf::Event> m_events;
            sf::RenderWindow *m_window;
    };
}

#endif //PMGUI_EVENTMANAGER_H
