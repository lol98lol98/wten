cc  = cc
opt = /errorReport:none /W1 /MT /EHsc /Ox /Fpwten.pch
obj = DxLib_lib.obj utility.obj graph.obj char_condition.obj pt_condition.obj enemy_condition.obj enemy_info.obj ui_base.obj ui_box.obj ui_string.obj ui_image.obj ui_selector.obj ui_string_box.obj event_base.obj key_event.obj on_select_event.obj select_window.obj script_window.obj window_base.obj window_manager.obj event_checker_base.obj key_event_checker.obj event_manager.obj debug_scene.obj error_normal.obj event_notify.obj scene_base.obj wten.obj stdafx.obj main.obj
exe = wten.exe

.c.obj:
	$(cc) /c /Yustdafx.h $(opt) /Tp $<
.cpp.obj:
	$(cc) /c /Yustdafx.h $(opt) /Tp $<
$(exe).exe: $(obj)
	version.exe
	$(cc) $(opt) /Fe$* $** $(lib)

src/stdafx.h: src/DxLib_lib.h src/error.h src/char_condition.h src/pt_condition.h src/enemy_condition.h src/enemy_info.h src/event.h src/events/event_base.h src/events/key_event.h src/events/on_select_event.h src/event_checker.h src/events/event_checker_base.h src/events/key_event_checker.h src/event_manager.h src/graph.h src/ui.h src/utility.h src/window.h src/window_manager.h src/scene.h src/wten.h src/debug_scene.h src/error_normal.h src/event_notify.h src/scene_base.h src/windows/window_base.h src/windows/script_window.h src/windows/select_window.h src/uis/ui_base.h src/uis/ui_box.h src/uis/ui_string.h src/uis/ui_image.h src/uis/ui_selector.h src/uis/ui_string_box.h
	touch src/stdafx.h
stdafx.obj: src/stdafx.h
        $(cc) $(opt) /c /Yc /Tp src/stdafx.cpp

DxLib_lib.obj: src/DxLib_lib.cpp stdafx.obj
utility.obj: src/utility.cpp stdafx.obj
graph.obj: src/graph.cpp stdafx.obj
char_condition.obj: src/char_condition.cpp stdafx.obj
pt_condition.obj: src/pt_condition.cpp stdafx.obj
enemy_condition.obj: src/enemy_condition.cpp stdafx.obj
enemy_info.obj: src/enemy_info.cpp stdafx.obj
ui_base.obj: src/uis/ui_base.cpp stdafx.obj
ui_box.obj: src/uis/ui_box.cpp stdafx.obj
ui_string.obj: src/uis/ui_string.cpp stdafx.obj
ui_image.obj: src/uis/ui_image.cpp stdafx.obj
ui_selector.obj: src/uis/ui_selector.cpp stdafx.obj
ui_string_box.obj: src/uis/ui_string_box.cpp stdafx.obj
event_base.obj: src/events/event_base.cpp stdafx.obj
key_event.obj: src/events/key_event.cpp stdafx.obj
on_select_event.obj: src/events/on_select_event.cpp stdafx.obj
window_base.obj: src/windows/window_base.cpp stdafx.obj
select_window.obj: src/windows/select_window.cpp stdafx.obj
script_window.obj: src/windows/script_window.cpp stdafx.obj
window_manager.obj: src/window_manager.cpp stdafx.obj
event_checker_base.obj: src/events/event_checker_base.cpp stdafx.obj
key_event_checker.obj: src/events/key_event_checker.cpp stdafx.obj
event_manager.obj: src/event_manager.cpp stdafx.obj
event_notify.obj: src/event_notify.cpp stdafx.obj
error_normal.obj: src/error_normal.cpp stdafx.obj
scene_base.obj: src/scene_base.cpp stdafx.obj
wten.obj: src/wten.cpp stdafx.obj src/version.h
main.obj: src/main.cpp stdafx.obj

debug_scene.obj:  src/debug_scene.cpp stdafx.obj
