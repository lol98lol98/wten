#include "stdafx.h"

#define ITEM_MAX (8)

namespace wten {

using namespace utility;
using namespace boost::assign;

namespace {

typedef boost::tuple<boost::shared_ptr<const Job>, boost::shared_ptr<const actions::SpellBase> > SPELL_PAIR;

boost::tuple<std::vector<unsigned int>, std::vector<unsigned int> > CalcMP(const std::vector<SPELL_PAIR>& spell_list) {
	std::vector<unsigned int> mage_mp(7, 0);
	std::vector<unsigned int> priest_mp(7, 0);
	BOOST_FOREACH(SPELL_PAIR pair, spell_list) {
		boost::shared_ptr<const Job> job;
		boost::shared_ptr<const actions::SpellBase> spell;
		boost::tie(job, spell) = pair;
		BOOST_ASSERT(job);
		BOOST_ASSERT(spell);
		BOOST_ASSERT(spell->GetLv() <= 7);
		BOOST_ASSERT(spell->GetLv() > 0);
		switch(spell->GetUseJob()) {
			case actions::SpellBase::SPELL_JOB_MAGE:
				mage_mp[spell->GetLv() - 1]++;
				break;
			case actions::SpellBase::SPELL_JOB_PRIEST:
				priest_mp[spell->GetLv() - 1]++;
				break;
			default:
				BOOST_ASSERT(false);
				CREATE_ERROR(ERROR_CODE_INTERNAL_ERROR).get()->Abort();
		}
	}
	return boost::make_tuple(mage_mp, priest_mp);
}

struct SpellValue {
public:
	typedef boost::shared_ptr<const actions::SpellBase> result_type;
	SpellValue() { }
	result_type operator ()(SPELL_PAIR spell_pair) const {
		return spell_pair.get<1>();
	}
};

int CalcUpRank(unsigned int bed_level, unsigned int lv) {
	BOOST_ASSERT(bed_level <= BED_LEVEL_MAX);
	int result = static_cast<int>(bed_level);
	if(lv > 40) {
		result -= 4;
	} else {
		result -= lv / 10;
	}
	return result;
}

boost::optional<bool> RankUpChallenge(unsigned int status, int up_rank) {
	boost::optional<bool> result;
	if(up_rank <= -2) {
		result = false;
	} else {
		const unsigned int rand = DxLibWrapper::GetRand(static_cast<unsigned int>(up_rank+2));
		if(rand == 0) {
			result = false;
		} else if(rand == 1) {
			result = boost::none;
		} else {
			result = true;
		}
	}

	if(result && result.get() == false && status == 0) {
		return boost::none;
	}
	if(result && result.get() == true && status == 10) {
		return boost::none;
	}
	return result;
}

std::vector<boost::optional<bool> > RankUpChallenge(std::vector<unsigned int> status_list, int up_rank) {
	std::vector<boost::optional<bool> > result;
	BOOST_FOREACH(unsigned int status, status_list) {
		result.push_back(RankUpChallenge(status, up_rank));
	}
	return result;
}

} // anonymous

CharStatus::CharStatus(boost::shared_ptr<const std::wstring> name, boost::shared_ptr<const Job> job, ALIGNMENT alignment, unsigned int lv, unsigned int hp,
	unsigned int str, unsigned int iq, unsigned int pie, unsigned int vit, unsigned int agi, unsigned int luk,
	unsigned int tg, unsigned int exp, const std::vector<boost::shared_ptr<Item> >& item_list,
	const std::vector<SPELL_PAIR>& spell_list)
:
	name(name), job(job), alignment(alignment), lv(lv), hp(hp), str(str), iq(iq), pie(pie), vit(vit), agi(agi), luk(luk),
	tg(tg), exp(exp), item_list(item_list), spell_list(spell_list)
{
	BOOST_ASSERT(name);
	BOOST_ASSERT(!name->empty());
	BOOST_ASSERT(job);
	BOOST_ASSERT(lv > 0);
	BOOST_ASSERT(hp > 0);
	BOOST_ASSERT(str <= 10);
	BOOST_ASSERT(iq <= 10);
	BOOST_ASSERT(pie <= 10);
	BOOST_ASSERT(vit <= 10);
	BOOST_ASSERT(agi <= 10);
	BOOST_ASSERT(luk <= 10);
	BOOST_ASSERT(item_list.size() <= ITEM_MAX);

	boost::tie(mage_mp, priest_mp) = CalcMP(this->spell_list);
	BOOST_ASSERT(mage_mp.size() == 7);
	BOOST_ASSERT(priest_mp.size() == 7);
}

CharStatus::~CharStatus(void) {
}

boost::shared_ptr<const std::wstring> CharStatus::GetName(void) const {
	return name;
}

boost::shared_ptr<const Job> CharStatus::GetJob(void) const {
	return job;
}

CharStatus::ALIGNMENT CharStatus::GetAlignment(void) const {
	return alignment;
}

unsigned int CharStatus::GetLv(void) const {
	return lv;
}

unsigned int CharStatus::GetHP(void) const {
	return hp;
}

unsigned int CharStatus::GetStr(void) const {
	BOOST_ASSERT(job);
	return job->GetStr() + str;
}

unsigned int CharStatus::GetIQ(void) const {
	BOOST_ASSERT(job);
	return job->GetIQ() + iq;
}

unsigned int CharStatus::GetPie(void) const {
	BOOST_ASSERT(job);
	return job->GetPie() + pie;
}

unsigned int CharStatus::GetVit(void) const {
	BOOST_ASSERT(job);
	return job->GetVit() + vit;
}

unsigned int CharStatus::GetAgi(void) const {
	BOOST_ASSERT(job);
	return job->GetAgi() + agi;
}

unsigned int CharStatus::GetLuk(void) const {
	BOOST_ASSERT(job);
	return job->GetLuk() + luk;
}

std::vector<unsigned int> CharStatus::GetMageMP(void) const {
	BOOST_ASSERT(mage_mp.size() == 7);
	return mage_mp;
}

std::vector<unsigned int> CharStatus::GetPriestMP(void) const {
	BOOST_ASSERT(priest_mp.size() == 7);
	return priest_mp;
}

unsigned int CharStatus::GetTG(void) const {
	return tg;
}

unsigned int CharStatus::GetExp(void) const {
	return exp;
}

std::vector<boost::shared_ptr<Item> > CharStatus::GetItemList(void) const {
	BOOST_ASSERT(item_list.size() <= ITEM_MAX);
	return item_list;
}

std::vector<boost::shared_ptr<const actions::SpellBase> > CharStatus::GetSpells(void) const {
	std::vector<boost::shared_ptr<const actions::SpellBase> > result;
	boost::copy(spell_list | boost::adaptors::transformed(SpellValue()) | boost::adaptors::uniqued, std::back_inserter(result));
	return result;
}

boost::optional<boost::shared_ptr<Error> > CharStatus::ChangeName(boost::shared_ptr<const std::wstring> new_name) {
	if(!new_name) {
		return CREATE_ERROR(ERROR_CODE_INVALID_PARAMETER);
	}
	if(new_name->empty()) {
		return CREATE_ERROR(ERROR_CODE_INVALID_PARAMETER);
	}
	name = new_name;
	return boost::none;
}

void CharStatus::AddExp(unsigned int value) {
	BOOST_ASSERT(UINT_MAX - exp >= value);
	exp += value;
}

void CharStatus::AddTG(unsigned int value) {
	BOOST_ASSERT(UINT_MAX - tg >= value);
	tg += value;
}

void CharStatus::DecTG(unsigned int value) {
	BOOST_ASSERT(tg >= value);
	tg -= value;
}

boost::optional<boost::shared_ptr<Error> > CharStatus::AddItem(boost::shared_ptr<Item> item) {
	if(item_list.size() == ITEM_MAX) {
		return CREATE_ERROR(ERROR_CODE_ITEM_COUNT_OVER);
	}
	item_list.push_back(item);
	return boost::none;
}

boost::optional<boost::shared_ptr<Error> > CharStatus::DeleteItem(boost::shared_ptr<Item> item) {
	std::vector<boost::shared_ptr<Item> >::iterator it = std::find(item_list.begin(), item_list.end(), item);
	if(it == item_list.end()) {
		return CREATE_ERROR(ERROR_CODE_ITEM_NOT_FOUND);
	}
	item_list.erase(it);
	return boost::none;
}

boost::optional<boost::shared_ptr<Error> > CharStatus::LevelDrain(unsigned int value) {
	if(lv <= value) {
		lv = 0;
		hp = 0;
		exp = 0;
		return CREATE_ERROR(ERROR_CODE_CHAR_LOST);
	}
	const unsigned int dec_hp = hp / lv + 1;
	hp -= dec_hp * value;
	lv -= value;
	BOOST_ASSERT(job);
	exp = job->CalcExp(lv);
	BOOST_ASSERT(hp > 0);
	return boost::none;
}

opt_error<boost::optional<boost::shared_ptr<std::wstring> > >::type CharStatus::CheckLevelUP(unsigned int bed_lv) {
	BOOST_ASSERT(bed_lv < 6);
	BOOST_ASSERT(job);
	unsigned int check_level = job->CalcLv(exp);
	BOOST_ASSERT(lv >= check_level);
	if(lv == check_level) {
		return boost::none;
	}
	return LevelUP(bed_lv);
}

opt_error<boost::shared_ptr<std::wstring> >::type CharStatus::LevelUP(unsigned int bed_lv) {
	BOOST_ASSERT(job);
	lv++;
	unsigned int min_exp = job->CalcExp(lv);
	if(exp < min_exp) {
		exp = min_exp;
	}

	const int up_rank = CalcUpRank(bed_lv, lv);
	std::vector<unsigned int> status_list;
	status_list += str, iq, pie, vit, agi, luk;
	std::vector<boost::optional<bool> > up_list = RankUpChallenge(status_list, up_rank);
	BOOST_ASSERT(up_list.size() == 6);

	wchar_t text_char[1024];
	WSPRINTF(text_char, L"%sはレベルが上がった(%d => %d)\n", GetName()->c_str(), GetLv() - 1, GetLv());

	{
		const unsigned int before = hp;
		hp = GetJob()->CalcMaxHP(lv, hp);
		const unsigned int after = hp;
		WSTRCATF(text_char, L"HPが%d上がった(%d => %d)\n", after-before, before, after);
	}

	unsigned int i = 0;
	BOOST_FOREACH(boost::optional<bool> flag, up_list) {
		int up_down;
		if(flag) {
			if(flag.get() == true) {
				up_down = 1;
			} else {
				up_down = -1;
			}
		} else {
			continue;
		}
		switch(i) {
#define ADD_STATUS(id, var, name, geter)																			\
			case id: {																				\
					const unsigned int before = geter();														\
					var += up_down;																	\
					const unsigned int after = geter();														\
					WSTRCATF(text_char, L"%sが%d%s(%d => %d)\n", name, after - before, (up_down == 1 ? L"上がった" : L"下がった"), before, after);	\
				}																				\
				break
			
			ADD_STATUS(0, str, L"STR", GetStr);
			ADD_STATUS(1, iq,  L"IQ",  GetIQ);
			ADD_STATUS(2, pie, L"PIE", GetPie);
			ADD_STATUS(3, vit, L"VIT", GetVit);
			ADD_STATUS(4, agi, L"AGI", GetAgi);
			ADD_STATUS(5, luk, L"LUK", GetLuk);
#undef ADD_STATUS
			default:
				BOOST_ASSERT(false);
				return CREATE_ERROR(ERROR_CODE_INTERNAL_ERROR);
		}
		i++;
	}

	// TODO スペルの習得など
	return boost::shared_ptr<std::wstring>(new std::wstring(text_char));
}

} // wten

