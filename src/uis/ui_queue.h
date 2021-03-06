
namespace wten {

namespace uis {

#pragma pack(push, 4)
#pragma warning(push)
#pragma warning(disable: 4625)
#pragma warning(disable: 4626)
class UIQueue : public UIBase {
public:
	enum POSITION {
		COL_POSITION_LEFT,
		COL_POSITION_RIGHT,
		ROW_POSITION_TOP,
		ROW_POSITION_BOTTOM,
	};
	enum INNER_POSITION {
		INNER_POSITION_LEFT,
		INNER_POSITION_RIGHT,
		INNER_POSITION_CENTER,
		INNER_POSITION_TOP,
		INNER_POSITION_BOTTOM,
	};
	UIQueue(const std::vector<boost::tuple<POSITION, boost::shared_ptr<UIBase> > >& ui_list, boost::optional<INNER_POSITION> inner_position = boost::none);
	~UIQueue();
	boost::optional<boost::shared_ptr<Error> > SetOwnerWindow(boost::weak_ptr<const windows::WindowBase> window);
	boost::optional<boost::shared_ptr<Error> > ClearOwnerWindow(void);
	boost::optional<boost::shared_ptr<Error> > Move(void);
	boost::optional<boost::shared_ptr<Error> > Move(unsigned int x, unsigned int y);
	boost::optional<boost::shared_ptr<Error> > Resize(unsigned int width, unsigned int height);
	boost::optional<boost::shared_ptr<Error> > Resize(void);
	boost::optional<boost::shared_ptr<Error> > Draw(void);
	boost::optional<boost::shared_ptr<Error> > Draw(unsigned int abs_x, unsigned int abs_y);
	utility::opt_error<unsigned int>::type CalcWidth() const;
	utility::opt_error<unsigned int>::type CalcHeight() const;
protected:
	boost::optional<boost::shared_ptr<Error> > ReloadInnerUI(void);

	const bool col_split;
	unsigned char padding[3]; //unused
	const std::vector<boost::tuple<POSITION, boost::shared_ptr<UIBase> > > ui_list;
	const INNER_POSITION inner_position;
};
#pragma warning(pop)
#pragma pack(pop)

} // uis

} // wten
