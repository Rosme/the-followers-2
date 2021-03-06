/*
Copyright (C) - Jean-Sébastien Fauteux 2017
This work is licensed under the Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License.
To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-sa/3.0/ or send a letter to
Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
*/

#include "message_dispatcher.hpp"
#include "message_handler.hpp"

namespace Core {


	void MessageDispatcher::registerHandler(const std::string& name, MessageHandler& handler) {
		m_handlers.emplace(std::make_pair(name, &handler));
	}

	void MessageDispatcher::unregisterHandler(const std::string& name, MessageHandler& handler) {
		auto range = m_handlers.equal_range(name);
		for(auto it = range.first; it != range.second;) {
			if(it->second == &handler) {
				it = m_handlers.erase(it);
			} else {
				++it;
			}
		}
	}

	void MessageDispatcher::pushMessage(const std::string& name, const Message& message) {
		m_messages.emplace(std::make_pair(name, message));
	}

	void MessageDispatcher::dispatch() {
		while(!m_messages.empty()) {
			auto messagePair = m_messages.front();
			auto range = m_handlers.equal_range(messagePair.first);
			for(auto it = range.first; it != range.second; ++it) {
				it->second->onMessage(messagePair.second, messagePair.first);
			}
			m_messages.pop();
		}
	}

}
