#pragma once

#include "cocos2d.h"

/**
@brief   Cocos2d-x application
*/
class  App : private cocos2d::Application {
public:
    App();
    virtual ~App();

    /**
    @brief    Initialize OpenGL context attributes
    @return   void
    */
    virtual void initGLContextAttrs();

    /**
    @brief          Implement Director and Scene initialization.
    @return true    Initialize success, app continue
    @return false   Initialize failed, app terminate
    */
    virtual bool applicationDidFinishLaunching();

    /**
    @brief    Called when the application enters background.
    @return   void
    */
    virtual void applicationDidEnterBackground();

    /**
    @brief    Called when the application enters foreground.
    @return   void
    */
    virtual void applicationWillEnterForeground();
};
