
namespace wten {

namespace uis {

class UIBox : public UIBase {
public:
	UIBox(const std::string& filename);
	~UIBox();
	boost::optional<boost::shared_ptr<Error> > Draw(void);
	utility::opt_error<unsigned int>::type CalcWidth();
	utility::opt_error<unsigned int>::type CalcHeight();
protected:
	boost::shared_ptr<Graph> left_up;
	boost::shared_ptr<Graph> left_down;
	boost::shared_ptr<Graph> right_up;
	boost::shared_ptr<Graph> right_down;
	boost::shared_ptr<Graph> left_line;
	boost::shared_ptr<Graph> right_line;
	boost::shared_ptr<Graph> up_line;
	boost::shared_ptr<Graph> down_line;
	boost::shared_ptr<Graph> blank;
};

} // uis

} // wten
