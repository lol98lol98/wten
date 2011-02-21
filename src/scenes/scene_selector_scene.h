
namespace wten { namespace scenes {

class SceneSelectorScene : public SceneBase {
public:
	typedef boost::tuple<boost::shared_ptr<const std::string>, boost::shared_ptr<Scene> > SCENE_PAIR;

	SceneSelectorScene(boost::shared_ptr<const std::string> title, const std::vector<SCENE_PAIR>& scene_list);
	~SceneSelectorScene();

	boost::optional<boost::shared_ptr<Error> > SceneInitialize(void);
	boost::optional<boost::shared_ptr<Error> > OnSelect(boost::shared_ptr<Event> event);
	boost::variant<boost::shared_ptr<Error>, boost::optional<boost::shared_ptr<Scene> >, boost::shared_ptr<SceneExit> > EnterFrame(void);
private:
	const boost::shared_ptr<const std::string> title;
	const std::vector<SCENE_PAIR> scene_list;
	const boost::shared_ptr<windows::ScriptWindow> script_window;
	boost::shared_ptr<Scene> next_scene;
};

} // scenes

} // wten

