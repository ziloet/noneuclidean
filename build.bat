@echo off
setlocal

set build_dir="build"
set files_to_compile=../main_win32.c

if not exist %build_dir% (
	mkdir %build_dir%
)
pushd %build_dir%
set msvc_flags=/nologo /Zi /W3 /Fe:"main_win32.exe"
set link_flags=/subsystem:windows /WX /opt:ref /opt:icf /incremental:no
set libs_to_link=kernel32.lib user32.lib opengl32.lib
call cl %msvc_flags% %files_to_compile% /link %link_flags% %libs_to_link%
popd