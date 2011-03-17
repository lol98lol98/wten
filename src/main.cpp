#include "stdafx.h"

using namespace wten;
using namespace boost::assign;

namespace {

boost::shared_ptr<const Job> CreateDummyJob() {
	static unsigned int id = 1;
	boost::shared_ptr<const std::wstring> name(new std::wstring(L"DUMMY"));
	unsigned int hp_base = 10, hp_count_bonus = 0, str = 10, iq = 10, pie = 10, vit = 10, agi = 10, luk = 10, thief_skill = 0, exp_base = 1000;
	std::vector<boost::tuple<unsigned int, boost::shared_ptr<const actions::SpellBase> > > spells;
	boost::shared_ptr<const Job> job(new Job(id, name, hp_base, hp_count_bonus, str, iq, pie, vit, agi, luk, thief_skill, exp_base, spells));
	id++;
	return job;
}

boost::shared_ptr<CharData> CreateDummyCharData(const wchar_t *char_name) {
	boost::shared_ptr<const Job> job = CreateDummyJob();
	boost::shared_ptr<const std::wstring> name(new std::wstring(char_name));
	unsigned int lv = 1;
	unsigned int hp = job->CalcMaxHP(lv, 1);
	unsigned int str,iq,pie,vit,agi,luk,tg,exp;
	str = iq = pie = vit = agi = luk = tg = exp = 0;
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

} // anonymous

//int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow ){
int main() {
	::setlocale(LC_ALL, _T(""));

	ItemInfoList::GetCurrentInstance();

	boost::shared_ptr<WTen> game;/*
	{
		boost::shared_ptr<Scene> scene(new scenes::DebugScene());
		game.reset(new WTen(scene));
	}*/
	{
		boost::shared_ptr<Scene> scene(new scenes::OpeningScene());
		game.reset(new WTen(scene));
	}
	boost::optional<boost::shared_ptr<Error> > result = game->DoStart(CreateDummyPT());
	game.reset();
	if(result) {
		result.get()->Abort();
		return 1;
	}

	return 0;
}

