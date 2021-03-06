#include "stdafx.h"

namespace wten {

using namespace utility;

EventManager::EventManager() {
	boost::shared_ptr<EventChecker> key_checker(new events::KeyEventChecker());
	checker_list.push_back(key_checker);
}

boost::optional<boost::shared_ptr<Error> > EventManager::DoCheck(void) {
	BOOST_FOREACH(boost::shared_ptr<EventChecker> checker, checker_list) {
		OPT_ERROR(checker->DoCheck());
	}
	return boost::none;
}

boost::optional<boost::shared_ptr<Error> > EventManager::Enqueue(boost::shared_ptr<Event> event) {
	event_queue.push(event);
	return boost::none;
}

opt_error<boost::optional<boost::shared_ptr<Event> > >::type EventManager::Dequeue(void) {
	if(event_queue.empty()) {
		return boost::none;
	}
	boost::shared_ptr<Event> result = event_queue.front();
	event_queue.pop();
	return result;
}

boost::optional<boost::shared_ptr<Error> > EventManager::Clear(void) {
	while(!event_queue.empty()) {
		event_queue.pop();
	}
	return boost::none;
}

} // wten
