#include "stdafx.h"

using namespace wten;
using namespace utility;
using namespace boost::assign;

namespace {

boost::shared_ptr<const Job> GetRandomJob() {
	boost::shared_ptr<JobList> list = JobList::GetCurrentInstance();
	return list->GetList()[DxLibWrapper::GetRand(list->GetList().size()-1)];
}

boost::shared_ptr<CharData> CreateDummyCharData(const wchar_t *char_name) {
	boost::shared_ptr<const Job> job = GetRandomJob();
	boost::shared_ptr<const std::wstring> name(new std::wstring(char_name));
	unsigned int lv = 1;
	unsigned int hp = job->CalcMaxHP(lv, 1);
	unsigned int str,iq,pie,vit,agi,luk,tg,exp;
	str = iq = pie = vit = agi = luk = exp = 0;
	tg = 123456789;
	std::vector<boost::shared_ptr<Item> > item_list;
	std::vector<boost::tuple<boost::shared_ptr<const Job>, boost::shared_ptr<const actions::SpellBase> > > spell_list;
	boost::shared_ptr<CharStatus> char_status(new CharStatus(name, job, CharStatus::ALIGNMENT_GOOD, lv, hp, str, iq, pie, vit, agi, luk, tg, exp, item_list, spell_list));
	boost::shared_ptr<CharCondition> char_condition(new CharCondition());
	boost::shared_ptr<CharData> character(new CharData(char_status, char_condition, false));
	CharacterList::GetCurrentInstance()->AddChar(character);
	return character;
}

boost::shared_ptr<PTData> CreateDummyPT() {
	std::vector<boost::shared_ptr<CharData> > characters;
	characters += CreateDummyCharData(L"やる夫");
	characters += CreateDummyCharData(L"やらない夫");
	characters += CreateDummyCharData(L"坂本美緒");
	characters += CreateDummyCharData(L"黒沢");
	characters += CreateDummyCharData(L"高槻やよい");
	characters += CreateDummyCharData(L"コクリコ");
	boost::shared_ptr<PTCondition> pt_condition(new PTCondition());
	boost::shared_ptr<PTData> pt(new PTData(pt_condition, characters, false));
	PTList::GetCurrentInstance()->AddPT(pt);
	return pt;
}

void callback(void) {
	boost::optional<boost::shared_ptr<Error> > error = Graph::AllGraphReload();
	if(error) {
		error.get()->Abort();
		BOOST_ASSERT(false);
	}
}

} // anonymous

#ifndef _DEBUG
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int){
#else
int main() {
#endif
	::setlocale(LC_ALL, _T(""));

	ItemInfoList::GetCurrentInstance();

	boost::shared_ptr<WTen> game(new WTen());/*
	{
		boost::shared_ptr<Scene> scene(new scenes::DebugScene());
		game->SetScene(scene);
	}*/
	{
		boost::shared_ptr<Scene> scene(new scenes::OpeningScene(WChar2Ptr(L"data/ui/box1.png")));
		game->SetScene(scene);
	}
	::SetRestoreGraphCallback(callback);
	boost::optional<boost::shared_ptr<Error> > result = game->DoStart(CreateDummyPT());
	game.reset();
	if(result) {
		result.get()->Abort();
		return 1;
	}

	return 0;
}

