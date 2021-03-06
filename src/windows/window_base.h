
namespace wten {

namespace windows {

#pragma warning(push)
#pragma warning(disable: 4625)
#pragma warning(disable: 4626)
class WindowBase : public Window, public boost::enable_shared_from_this<WindowBase> {
public:
	WindowBase(boost::shared_ptr<const std::wstring> default_frame_filename);
	WindowBase(boost::shared_ptr<const Graph> default_frame_graph);
	~WindowBase();
	boost::optional<boost::shared_ptr<Error> > WindowInitialize(void);
	boost::optional<boost::shared_ptr<Error> > OnForeground(void);
	utility::opt_error<boost::tuple<unsigned int, unsigned int> >::type GetPoint(void) const;
	utility::opt_error<boost::tuple<unsigned int, unsigned int> >::type GetSize(void) const;
	boost::optional<boost::shared_ptr<Error> > Move(unsigned int x, unsigned int y);
	boost::optional<boost::shared_ptr<Error> > Resize(unsigned int width, unsigned int height);
	boost::optional<boost::shared_ptr<Error> > Draw(void);
	utility::opt_error<boost::optional<boost::shared_ptr<Event> > >::type NotifyEvent(boost::shared_ptr<Event> event);

	boost::optional<boost::shared_ptr<Error> > AddUI(boost::shared_ptr<UI> ui);
	utility::opt_error<bool>::type RemoveUI(boost::shared_ptr<UI> ui);
	boost::optional<boost::shared_ptr<Error> > ClearUI(void);
	boost::optional<boost::shared_ptr<Error> > RemoveThisWindow(void);

	boost::optional<boost::shared_ptr<Error> > AddUI(boost::shared_ptr<uis::UIBase> ui, uis::UIBase::MOVE_MODE move_mode, unsigned int x, unsigned int y, unsigned int width, unsigned int height);
	boost::optional<boost::shared_ptr<Error> > AddBoxUI(uis::UIBase::MOVE_MODE move_mode, unsigned int x, unsigned int y, unsigned int width, unsigned int height);
	boost::optional<boost::shared_ptr<Error> > AddImageUI(boost::shared_ptr<const std::wstring> image_filename, uis::UIBase::MOVE_MODE move_mode, unsigned int x, unsigned int y);
	boost::optional<boost::shared_ptr<Error> > AddTextUI(boost::shared_ptr<const std::wstring> text, uis::UIBase::MOVE_MODE move_mode, unsigned int x, unsigned int y, unsigned int width, unsigned int height);
	boost::optional<boost::shared_ptr<Error> > AddPTStatusUI(boost::shared_ptr<const PTData> pt_data, uis::UIBase::MOVE_MODE move_mode, unsigned int x, unsigned int y, unsigned int width, unsigned int height);
	boost::optional<boost::shared_ptr<Error> > AddCharStatusUI(boost::shared_ptr<const PTData> pt_data, unsigned int char_index, uis::UIBase::MOVE_MODE move_mode, unsigned int x, unsigned int y, unsigned int width, unsigned int height);
protected:
	std::vector<boost::shared_ptr<UI> > ui_stack;
	unsigned int x;
	unsigned int y;
	unsigned int width;
	unsigned int height;
	boost::shared_ptr<const Graph> default_frame_graph;
};
#pragma warning(pop)

} // windows

} // wten
