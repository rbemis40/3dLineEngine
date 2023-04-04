main : *.cpp *.h
	clang++ loop_timer.cpp rufont.cpp pfblock.cpp pfblock_tl.cpp pfblock_point3d.cpp pfblock_plane.cpp cylinder.cpp threadpool.cpp floorplane.cpp cube.cpp drawtools.cpp lineseg.cpp plane.cpp vector2d.cpp prog_inst.cpp camera.cpp main.cpp  -I/Library/Frameworks/SDL2_ttf.framework/Headers -I/Library/Frameworks/SDL2.framework/Headers  -F/Library/Frameworks -framework SDL2_ttf -framework SDL2 -std=c++11 -o build/main 

libfiles_cpp = loop_timer.cpp rufont.cpp camera.cpp cube.cpp cylinder.cpp floorplane.cpp pfblock_plane.cpp pfblock_point3d.cpp pfblock_tl.cpp pfblock.cpp plane.cpp prog_inst.cpp threadpool.cpp vector2d.cpp
libfiles_h = loop_timer.h rufont.h points.h camera.h cube.h cylinder.h floorplane.h pfblock_plane.h pfblock_point3d.h pfblock_tl.h pfblock.h plane.h prog_inst.h threadpool.h vector2d.h
lib : $(libfiles_cpp) $(libfiles_h)
	clang++ $(libfiles_cpp)  -I/Library/Frameworks/SDL2_ttf.framework/Headers -I/Library/Frameworks/SDL2.framework/Headers -F/Library/Frameworks -std=c++11 -c
	ar -r build/libengine3d.a *.o
	rm *.o

	cp $(libfiles_h) build/includes
