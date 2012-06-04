/** Example 006 2D Graphics

This Tutorial shows how to do 2d graphics with the Irrlicht Engine.
It shows how to draw images, keycolor based sprites,
transparent rectangles, and different fonts. You may consider
this useful if you want to make a 2d game with the engine, or if
you want to draw a cool interface or head up display for your 3d game.

As always, I include the header files, use the irr namespace,
and tell the linker to link with the .lib file.
*/
#include <irrlicht.h>
#ifndef _IRR_COMPILE_WITH_ANDROID_DEVICE_
#include "driverChoice.h"
#endif

using namespace irr;

#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif

using namespace irr;
#ifdef _IRR_COMPILE_WITH_ANDROID_DEVICE_
#include "os.h"
#include <android/log.h>
#include <android/window.h>
#include <android/sensor.h>
#include <android_native_app_glue.h>
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "sensor", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", __VA_ARGS__))
#endif


/*
At first, we let the user select the driver type, then start up the engine, set
a caption, and get a pointer to the video driver.
*/
#ifndef _IRR_COMPILE_WITH_ANDROID_DEVICE_
int main()
{
	// ask user for driver
	video::E_DRIVER_TYPE driverType=driverChoiceConsole();
	if (driverType==video::EDT_COUNT)
		return 1;

	// create device

	IrrlichtDevice *device = createDevice(driverType,
		core::dimension2d<u32>(512, 384));

	if (device == 0)
		return 1; // could not create selected driver.

	device->setWindowCaption(L"Irrlicht Engine - 2D Graphics Demo");

	video::IVideoDriver* driver = device->getVideoDriver();

	/*
	All 2d graphics in this example are put together into one texture,
	2ddemo.png. Because we want to draw colorkey based sprites, we need to
	load this texture and tell the engine, which part of it should be
	transparent based on a colorkey.

	In this example, we don't tell it the color directly, we just say "Hey
	Irrlicht Engine, you'll find the color I want at position (0,0) on the
	texture.". Instead, it would be also possible to call
	driver->makeColorKeyTexture(images, video::SColor(0,0,0,0)), to make
	e.g. all black pixels transparent. Please note that
	makeColorKeyTexture just creates an alpha channel based on the color.
	*/
	video::ITexture* images = driver->getTexture("../../media/2ddemo.png");
	driver->makeColorKeyTexture(images, core::position2d<s32>(0,0));

	/*
	To be able to draw some text with two different fonts, we first load
	them. Ok, we load just one. As the first font we just use the default
	font which is built into the engine. Also, we define two rectangles
	which specify the position of the images of the red imps (little flying
	creatures) in the texture.
	*/
	gui::IGUIFont* font = device->getGUIEnvironment()->getBuiltInFont();
	gui::IGUIFont* font2 =
		device->getGUIEnvironment()->getFont("../../media/fonthaettenschweiler.bmp");

	core::rect<s32> imp1(349,15,385,78);
	core::rect<s32> imp2(387,15,423,78);

	/*
	Prepare a nicely filtering 2d render mode for special cases.
	*/
	driver->getMaterial2D().TextureLayer[0].BilinearFilter=true;
	driver->getMaterial2D().AntiAliasing=video::EAAM_FULL_BASIC;

	/*
	Everything is prepared, now we can draw everything in the draw loop,
	between the begin scene and end scene calls. In this example, we are
	just doing 2d graphics, but it would be no problem to mix them with 3d
	graphics. Just try it out, and draw some 3d vertices or set up a scene
	with the scene manager and draw it.
	*/
	while(device->run() && driver)
	{
		if (device->isWindowActive())
		{
			u32 time = device->getTimer()->getTime();

			driver->beginScene(true, true, video::SColor(255,120,102,136));

			/*
			First, we draw 3 sprites, using the alpha channel we
			created with makeColorKeyTexture. The last parameter
			specifies that the drawing method should use this alpha
			channel. The last-but-one parameter specifies a
			color, with which the sprite should be colored.
			(255,255,255,255) is full white, so the sprite will
			look like the original. The third sprite is drawn
			with the red channel modulated based on the time.
			*/

			// draw fire & dragons background world
			driver->draw2DImage(images, core::position2d<s32>(50,50),
				core::rect<s32>(0,0,342,224), 0,
				video::SColor(255,255,255,255), true);

			// draw flying imp
			driver->draw2DImage(images, core::position2d<s32>(164,125),
				(time/500 % 2) ? imp1 : imp2, 0,
				video::SColor(255,255,255,255), true);

			// draw second flying imp with colorcylce
			driver->draw2DImage(images, core::position2d<s32>(270,105),
				(time/500 % 2) ? imp1 : imp2, 0,
				video::SColor(255,(time) % 255,255,255), true);

			/*
			Drawing text is really simple. The code should be self
			explanatory.
			*/

			// draw some text
			if (font)
				font->draw(L"This demo shows that Irrlicht is also capable of drawing 2D graphics.",
					core::rect<s32>(130,10,300,50),
					video::SColor(255,255,255,255));

			// draw some other text
			if (font2)
				font2->draw(L"Also mixing with 3d graphics is possible.",
					core::rect<s32>(130,20,300,60),
					video::SColor(255,time % 255,time % 255,255));

			/*
			Next, we draw the Irrlicht Engine logo (without
			using a color or an alpha channel). Since we slightly scale
			the image we use the prepared filter mode.
			*/
			driver->enableMaterial2D();
			driver->draw2DImage(images, core::rect<s32>(10,10,108,48),
				core::rect<s32>(354,87,442,118));
			driver->enableMaterial2D(false);

			/*
			Finally draw a half-transparent rect under the mouse cursor.
			*/
			core::position2d<s32> m = device->getCursorControl()->getPosition();
			driver->draw2DRectangle(video::SColor(100,255,255,255),
				core::rect<s32>(m.X-20, m.Y-20, m.X+20, m.Y+20));

			driver->endScene();
		}
	}

	device->drop();

	return 0;
}
#else 
struct AppData
{
	IrrlichtDevice *device;
	irr::video::IVideoDriver* driver;
	irr::scene::ISceneManager* smgr;
	gui::IGUIFont* font;
	gui::IGUIFont* font2;
	video::ITexture* images;
	bool bAnimating;
	struct android_app* pApp;
};
static void handle_cmd(struct android_app* app, int32_t cmd)
{
    struct AppData* data = (struct AppData*) app->userData;

    switch (cmd)
    {
        case APP_CMD_INIT_WINDOW:
			{
				data->device = createDevice( video::EDT_OGLES2, 
								core::dimension2d<u32>(800, 480), //TODO[reizen]: doesn't work at the moment, always max resolution
								16,
								false, 
								false, 
								false, 
								0,
								(void*)data->pApp->window);
					data->driver = data->device->getVideoDriver();
					data->smgr = data->device->getSceneManager();
					data->images = data->driver->getTexture("/sdcard/irr/media/2ddemo.png");
					data->driver->makeColorKeyTexture(data->images, core::position2d<s32>(0,0));
					
#ifdef _IRR_COMPILE_WITH_GUI_
					data->font = data->device->getGUIEnvironment()->getBuiltInFont();
					data->font2 = data->device->getGUIEnvironment()->getFont("/sdcard/irr/media/fonthaettenschweiler.bmp");
#endif

					data->driver->getMaterial2D().TextureLayer[0].BilinearFilter=true;
					data->driver->getMaterial2D().AntiAliasing=video::EAAM_FULL_BASIC;
        	}
            break;

        case APP_CMD_TERM_WINDOW:
            break;
        case APP_CMD_GAINED_FOCUS:
            data->bAnimating = true;
            break;
        case APP_CMD_LOST_FOCUS:
            data->bAnimating = false;
            break;
    }
}

/**
 * Process the next input event.
 */
static int32_t handle_input(struct android_app* app, AInputEvent* event) {
    struct AppData* engine = (struct AppData*)app->userData;
	float x = 0.0f;
	float y = 0.0f;
	int pc = 0;
	int action = 0;
    if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) {
		pc = AMotionEvent_getPointerCount(event);
		action = AMotionEvent_getAction(event);
        x = AMotionEvent_getX(event, 0);
        y = AMotionEvent_getY(event, 0);
		LOGI("%x count: %d  x:%f y:$f", action, pc, x, y);
        return 1;
    }
    return 0;
}

void android_main(struct android_app* state)
{
    AppData data;
	memset(&data, 0, sizeof(AppData));

	// Make sure glue isn't stripped.
	app_dummy();

	state->userData = &data;
	state->onAppCmd = handle_cmd;
	state->onInputEvent = handle_input;
	data.pApp = state;
	int lastFPS = -1;
	core::rect<s32> imp1(349,15,385,78);
	core::rect<s32> imp2(387,15,423,78);

	for(;;)
	{
		// Read all pending events.
		int ident;
		int events;
		struct android_poll_source* source;

		// If not animating, we will block forever waiting for events.
		// If animating, we loop until all events are read, then continue
		// to draw the next frame of animation.
		while((ident=ALooper_pollAll(data.bAnimating ? 0 : -1, NULL, &events, (void**)&source)) >= 0)
		{
			// Process this event.
			if (source != NULL)
			{
				source->process(state, source);
			}

			// Check if we are exiting.
			if (state->destroyRequested != 0)
			{
				data.device->drop();
//				DeInitAPI(data);
				return;
			}
		}
        if(data.bAnimating && data.device->run())
        {     
			if (data.device->isWindowActive())
			{
				u32 time = data.device->getTimer()->getTime();
				data.driver->beginScene(true, true, video::SColor(255,120,102,136));
				

				// draw fire & dragons background world
				data.driver->draw2DImage(data.images, core::position2d<s32>(50,50),
					core::rect<s32>(0,0,342,224), 0,
					video::SColor(255,255,255,255), true);

				// draw flying imp
				data.driver->draw2DImage(data.images, core::position2d<s32>(164,125),
					(time/500 % 2) ? imp1 : imp2, 0,
					video::SColor(255,255,255,255), true);

				// draw second flying imp with colorcylce
				data.driver->draw2DImage(data.images, core::position2d<s32>(270,105),
					(time/500 % 2) ? imp1 : imp2, 0,
					video::SColor(255,(time) % 255,255,255), true);

				/*
				Drawing text is really simple. The code should be self
				explanatory.
				*/

#ifdef _IRR_COMPILE_WITH_GUI_
			// draw some text
			if (data.font)
				data.font->draw(L"This demo shows that Irrlicht is also capable of drawing 2D graphics.",
					core::rect<s32>(130,10,300,50),
					video::SColor(255,255,255,255));

			// draw some other text
			if (data.font2)
				data.font2->draw(L"Also mixing with 3d graphics is possible.",
					core::rect<s32>(130,20,300,60),
					video::SColor(255,time % 255,time % 255,255));
#endif
					
				data.driver->enableMaterial2D();
				data.driver->draw2DImage(data.images, core::rect<s32>(10,10,108,48),
					core::rect<s32>(354,87,442,118));
				data.driver->enableMaterial2D(false);

				/*
				Finally draw a half-transparent rect under the mouse cursor.
				*/
				//core::position2d<s32> m = device->getCursorControl()->getPosition();
				//driver->draw2DRectangle(video::SColor(100,255,255,255),
				//	core::rect<s32>(m.X-20, m.Y-20, m.X+20, m.Y+20));

				data.driver->endScene();				
				
				
				
			}
			else
				data.device->yield();			
        }		
	}
}
#endif
/*
That's all. I hope it was not too difficult.
**/
