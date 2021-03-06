#include "stdafx.h"

namespace wten { namespace windows {

using namespace utility;
using namespace boost::assign;

namespace {

boost::shared_ptr<uis::UICharStatus> CreateCharStatusUI(boost::shared_ptr<PTData> pt, boost::shared_ptr<const Graph> default_frame_graph) {
	boost::shared_ptr<uis::UICharStatus> result;
	if(pt->Size() > 0) {
		result.reset(new uis::UICharStatus(default_frame_graph, pt, 0));
	}
	return result;
}

} // anonymous

CampStatusWindow::CampStatusWindow(boost::shared_ptr<PTData> pt, boost::shared_ptr<const std::wstring> default_frame_filename) :
	CampBaseWindow(pt, default_frame_filename), char_status_ui(CreateCharStatusUI(pt, default_frame_graph))
{
}

CampStatusWindow::CampStatusWindow(boost::shared_ptr<PTData> pt, boost::shared_ptr<const Graph> default_frame_graph) :
	CampBaseWindow(pt, default_frame_graph), char_status_ui(CreateCharStatusUI(pt, default_frame_graph))
{
}

CampStatusWindow::~CampStatusWindow() {
}

boost::optional<boost::shared_ptr<Error> > CampStatusWindow::WindowInitialize(void) {
	OPT_ERROR(CampBaseWindow::WindowInitialize());
	if(!char_status_ui) {
		OPT_ERROR(SendNextTextWindowEvent(WChar2Ptr(L"PTに所属しているキャラクターが居ません")));
		return boost::none;
	}
	OPT_ERROR(AddUI(char_status_ui, uis::UIBase::MOVE_MODE_CENTER_FREE, 50, 100, 540, 150));

	const wchar_t char_text[] =
		L"A:呪文一覧\n"
		L"B:戻る";
	boost::shared_ptr<std::wstring> text(new std::wstring(char_text));
	text_window.reset(new TextWindow(text, default_frame_graph));
	OPT_ERROR(text_window->Move(380, 280));
	OPT_ERROR(text_window->Resize(180, 50));
	text_window->SetOkClose(false);
	SendNextWindowEvent(text_window);

	return boost::none;
}

utility::opt_error<boost::optional<boost::shared_ptr<Event> > >::type CampStatusWindow::NotifyEvent(boost::shared_ptr<Event> event) {
	BOOST_ASSERT(event);
	switch(event->GetEventType()) {
		case EVENT_TYPE_NEXT_STEP: {
			if(!char_status_ui) {
				OPT_ERROR(RemoveThisWindow());
				return boost::none;
			}
			// TODO
			return boost::none;
		}
		case EVENT_TYPE_KEY: {
			if(!char_status_ui) {
				return boost::none;
			}
			boost::shared_ptr<events::KeyEvent> key_event = boost::static_pointer_cast<events::KeyEvent>(event);
			if(key_event->GetAction() == events::KeyEvent::KEY_PRESS) {
				switch(key_event->GetKey()) {
					case events::KeyEvent::KEY_R:
						OPT_ERROR(char_status_ui->ChangeChar(uis::UICharStatus::CHANGE_INDEX_NEXT));
						break;
					case events::KeyEvent::KEY_L:
						OPT_ERROR(char_status_ui->ChangeChar(uis::UICharStatus::CHANGE_INDEX_BACK));
						break;
					case events::KeyEvent::KEY_B:
						SendPopWindowEvent(text_window);
						OPT_ERROR(RemoveThisWindow());
						break;
					default:
						break;
				}
			}
			return boost::none;
		}
		default:
			break;
	}
	return CampBaseWindow::NotifyEvent(event);
}

} // windows

} // wten
