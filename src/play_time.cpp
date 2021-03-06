#include "stdafx.h"

namespace wten {

namespace {

unsigned int GetCurrentSystemTime(void) {
	const time_t result = time(NULL);
	BOOST_ASSERT(result != -1);
	return static_cast<unsigned int>(result);
}

boost::shared_ptr<PlayTime> this_ptr;

} // anonymous

PlayTime::PlayTime() :
	start_time(GetCurrentSystemTime())
{
}

PlayTime::~PlayTime() {
}

//static
boost::shared_ptr<PlayTime> PlayTime::GetCurrentInstance(void) {
	if(!this_ptr) {
		this_ptr.reset(new PlayTime());
	}
	return this_ptr;
}

void PlayTime::ResetPlayTime(void) {
	start_time = GetCurrentSystemTime();
}

unsigned int PlayTime::GetCurrentPlayTime(void) const {
	return GlobalData::GetCurrentInstance()->GetPlayTime() + GetCurrentSystemTime() - start_time;
}

boost::shared_ptr<std::wstring> PlayTime::GetCurrentPlayTimeString(void) const {
	const unsigned int current_time = GetCurrentPlayTime();
	const unsigned int hour = current_time / 60 / 60;
	const unsigned int min = (current_time / 60) % 60;
	const unsigned int sec = current_time % 60;
	wchar_t text_char[256];
	WSPRINTF(text_char, L"%d:%02d:%02d", hour, min, sec);
	return boost::shared_ptr<std::wstring>(new std::wstring(text_char));
}

} // wten

