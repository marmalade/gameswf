//--------------------------------------------------------------------------
// Munters main file
//--------------------------------------------------------------------------


#include "s3e.h"
#include "IwGL.h"			//Ideaworks GL
#include "gameswf/gameswf.h"
#include <stdlib.h>
#include <stdio.h> 
#include <iostream>

#include "base/utility.h"
#include "base/container.h"
#include "base/tu_file.h"
#include "base/tu_types.h"
#include "base/tu_timer.h"
#include "gameswf/gameswf_types.h"
#include "gameswf/gameswf_impl.h"
#include "gameswf/gameswf_root.h"
#include "gameswf/gameswf_freetype.h"
#include "gameswf/gameswf_player.h"
#include "string.h"
using namespace std;
//--------------------------------------------------------------------------
// Main global function
//--------------------------------------------------------------------------

static tu_file*	file_opener(const char* url)
// Callback function.  This opens files for the gameswf library.
{
	
	return new tu_file(url, "rb");
	
}	

#define OVERSIZE	1.0f
S3E_MAIN_DECL void IwMain()
{
	gameswf::render_handler*	render = NULL;
	
	IwGLInit();
	gameswf::gc_ptr<gameswf::player> player = new gameswf::player();
	player->verbose_action(true);
	player->verbose_parse(true);
	player->set_separate_thread(false);
	player->set_log_bitmap_info(true);
	render =  gameswf::create_render_handler_iwgl();
	gameswf::set_render_handler(render);
	
	gameswf::register_file_opener_callback(file_opener);
	
	gameswf::gc_ptr<gameswf::root>	m = player->load_file("start.swf");
	if (m == NULL)
	{
		exit(1);
	}
	
	
	render->open();
	render->set_antialiased(true);
	
	// Turn on alpha blending.
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	// Turn on line smoothing.  Antialiased lines can be used to
	// smooth the outsides of shapes.
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);	// GL_NICEST, GL_FASTEST, GL_DONT_CARE
	
	glMatrixMode(GL_PROJECTION);
	
	//TODO pete - glOrtho not defined?
	//glOrtho(-OVERSIZE, OVERSIZE, OVERSIZE, -OVERSIZE, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	// We don't need lighting effects
	glDisable(GL_LIGHTING);
	// glColorPointer(4, GL_UNSIGNED_BYTE, 0, *);
	// glInterleavedArrays(GL_T2F_N3F_V3F, 0, *)
	
	//TODO - pete GL_ALL_ATTRIB_BITS not defined?
	//glPushAttrib (GL_ALL_ATTRIB_BITS);
	
	
	glDisable(GL_DEPTH_TEST);	// Disable depth testing.
	//glDrawBuffer(GL_BACK);
	
	while (1)
	{
		m = player->get_root();
		m->set_display_viewport(0, 0, 480, 800);
		m->set_background_alpha(1.0f);
		s3ePointerUpdate();
		m->notify_mouse_state(s3ePointerGetX	() 	, s3ePointerGetY	(), s3ePointerGetTouchState	(0));
		

		m->advance(50);
		m->display();
		
		IwGLSwapBuffers	(		 ) 	;	
		s3eDeviceYield(10);		
	}

	
	delete player;
	cout << "bye" <<  endl;
	
}
 




