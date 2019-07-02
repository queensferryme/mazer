#include "App.h"
#include "StartMenu.h"
#include "audio/include/SimpleAudioEngine.h"

using namespace CocosDenshion;
using namespace cocos2d;

static cocos2d::Size designResolutionSize = cocos2d::Size(480, 320);
static cocos2d::Size smallResolutionSize = cocos2d::Size(480, 320);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(960, 720);
static cocos2d::Size largeResolutionSize = cocos2d::Size(2048, 1536);

App::App() {}

App::~App() { SimpleAudioEngine::end(); }

/* initialize OpenGl context attributes */
void App::initGLContextAttrs() {
  // OpenGL context attributes:
  // red,green,blue,alpha,depth,stencil,multisamplesCount
  GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8, 0};
  GLView::setGLContextAttrs(glContextAttrs);
}

/* initialize Director & Scene */
bool App::applicationDidFinishLaunching() {
  // initialize director
  auto director = Director::getInstance();
  auto glview = director->getOpenGLView();
  if (!glview) {
    glview = GLViewImpl::createWithRect(
        "game", cocos2d::Rect(0, 0, mediumResolutionSize.width,
                              mediumResolutionSize.height));
    director->setOpenGLView(glview);
  }
  // set display FPS
  director->setDisplayStats(true);
  director->setAnimationInterval(1.0f / 60);
  // set frame size & resolution
  auto frameSize = glview->getFrameSize();
  dynamic_cast<GLViewImpl *>(glview)->setWindowed(frameSize.width,
                                                  frameSize.height);
  glview->setDesignResolutionSize(designResolutionSize.width,
                                  designResolutionSize.height,
                                  ResolutionPolicy::NO_BORDER);
  if (frameSize.height > mediumResolutionSize.height)
    director->setContentScaleFactor(
        MIN(largeResolutionSize.height / designResolutionSize.height,
            largeResolutionSize.width / designResolutionSize.width));
  else if (frameSize.height > smallResolutionSize.height)
    director->setContentScaleFactor(
        MIN(mediumResolutionSize.height / designResolutionSize.height,
            mediumResolutionSize.width / designResolutionSize.width));
  else
    director->setContentScaleFactor(
        MIN(smallResolutionSize.height / designResolutionSize.height,
            smallResolutionSize.width / designResolutionSize.width));
  // register start menu scene
  auto scene = StartMenu::createScene();
  director->runWithScene(scene);
  return true;
}

/* stop animation & sound in background */
void App::applicationDidEnterBackground() {
  Director::getInstance()->stopAnimation();
  SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
  SimpleAudioEngine::getInstance()->pauseAllEffects();
}

/* start animation & sound in foreground */
void App::applicationWillEnterForeground() {
  Director::getInstance()->startAnimation();
  SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
  SimpleAudioEngine::getInstance()->resumeAllEffects();
}